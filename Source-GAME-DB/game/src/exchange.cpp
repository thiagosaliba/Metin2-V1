#include "stdafx.h"
#include "../../libgame/include/grid.h"
#include "utils.h"
#include "desc.h"
#include "desc_client.h"
#include "char.h"
#include "item.h"
#include "item_manager.h"
#include "packet.h"
#include "log.h"
#include "db.h"
#include "locale_service.h"
#include "../../common/length.h"
#include "exchange.h"
#include "DragonSoul.h"
#if defined(__MESSENGER_BLOCK_SYSTEM__)
#include "messenger_manager.h"
#endif

void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, DWORD arg1, TItemPos arg2, DWORD arg3, void* pvData = NULL);

// ��ȯ ��Ŷ
void exchange_packet(LPCHARACTER ch, BYTE sub_header, bool is_me, DWORD arg1, TItemPos arg2, DWORD arg3, void* pvData)
{
	if (!ch->GetDesc())
		return;

	struct packet_exchange pack_exchg;

	pack_exchg.header = HEADER_GC_EXCHANGE;
	pack_exchg.sub_header = sub_header;
	pack_exchg.is_me = is_me;
	pack_exchg.arg1 = arg1;
	pack_exchg.arg2 = arg2;
	pack_exchg.arg3 = arg3;

	if (sub_header == EXCHANGE_SUBHEADER_GC_ITEM_ADD && pvData)
	{
		pack_exchg.arg4 = TItemPos(((LPITEM)pvData)->GetWindow(), ((LPITEM)pvData)->GetCell());

		thecore_memcpy(&pack_exchg.alSockets, ((LPITEM)pvData)->GetSockets(), sizeof(pack_exchg.alSockets));
#if defined(__ITEM_APPLY_RANDOM__)
		thecore_memcpy(&pack_exchg.aApplyRandom, ((LPITEM)pvData)->GetRandomApplies(), sizeof(pack_exchg.aApplyRandom));
#endif
		thecore_memcpy(&pack_exchg.aAttr, ((LPITEM)pvData)->GetAttributes(), sizeof(pack_exchg.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
		pack_exchg.dwTransmutationVnum = static_cast<LPITEM>(pvData)->GetTransmutationVnum();
#endif
	}
	else
	{
		pack_exchg.arg4 = TItemPos(RESERVED_WINDOW, 0);

		memset(&pack_exchg.alSockets, 0, sizeof(pack_exchg.alSockets));
#if defined(__ITEM_APPLY_RANDOM__)
		memset(&pack_exchg.aApplyRandom, 0, sizeof(pack_exchg.aApplyRandom));
#endif
		memset(&pack_exchg.aAttr, 0, sizeof(pack_exchg.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
		pack_exchg.dwTransmutationVnum = 0;
#endif
	}

	ch->GetDesc()->Packet(&pack_exchg, sizeof(pack_exchg));
}

// ��ȯ�� ����
bool CHARACTER::ExchangeStart(LPCHARACTER victim)
{
	if (this == victim) // �ڱ� �ڽŰ��� ��ȯ�� ���Ѵ�.
		return false;

	if (IsObserverMode())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���¿����� ��ȯ�� �� �� �����ϴ�."));
		return false;
	}

	if (victim->IsNPC())
		return false;

#if defined(__MESSENGER_BLOCK_SYSTEM__)
	if (MessengerManager::instance().IsBlocked(GetName(), victim->GetName()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Unblock %s to continue."), victim->GetName());
		return false;
	}
	else if (MessengerManager::instance().IsBlocked(victim->GetName(), GetName()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s has blocked you."), victim->GetName());
		return false;
	}
#endif

	// PREVENT_TRADE_WINDOW
	if (PreventTradeWindow(WND_EXCHANGE, true/*except*/))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ٸ� �ŷ�â�� ����������� �ŷ��� �Ҽ� �����ϴ�."));
		return false;
	}

	if (victim->PreventTradeWindow(WND_EXCHANGE, true/*except*/))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ٸ� �ŷ����̶� �ŷ��� �Ҽ� �����ϴ�."));
		return false;
	}
	// END_PREVENT_TRADE_WINDOW

	int iDist = DISTANCE_APPROX(GetX() - victim->GetX(), GetY() - victim->GetY());

	// �Ÿ� üũ
	if (iDist >= EXCHANGE_MAX_DISTANCE)
		return false;

	if (GetExchange())
		return false;

	if (victim->GetExchange())
	{
		exchange_packet(this, EXCHANGE_SUBHEADER_GC_ALREADY, 0, 0, NPOS, 0);
		return false;
	}

	if (victim->IsBlockMode(BLOCK_EXCHANGE))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ��ȯ �ź� �����Դϴ�."));
		return false;
	}

	SetExchange(M2_NEW CExchange(this));
	victim->SetExchange(M2_NEW CExchange(victim));

	victim->GetExchange()->SetCompany(GetExchange());
	GetExchange()->SetCompany(victim->GetExchange());

	//
	SetExchangeTime();
	victim->SetExchangeTime();

	exchange_packet(victim, EXCHANGE_SUBHEADER_GC_START, 0, GetVID(), NPOS, 0);
	exchange_packet(this, EXCHANGE_SUBHEADER_GC_START, 0, victim->GetVID(), NPOS, 0);

	return true;
}

CExchange::CExchange(LPCHARACTER pOwner)
{
	m_pCompany = NULL;

	m_bAccept = false;

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		m_apItems[i] = NULL;
		m_aItemPos[i] = NPOS;
		m_abItemDisplayPos[i] = 0;
	}

	m_lGold = 0;
#if defined(__CHEQUE_SYSTEM__)
	m_lCheque = 0;
#endif

	m_pOwner = pOwner;
	pOwner->SetExchange(this);

	m_pGrid = M2_NEW CGrid(4, 3);
}

CExchange::~CExchange()
{
	M2_DELETE(m_pGrid);
}

bool CExchange::AddItem(TItemPos item_pos, BYTE display_pos)
{
	assert(m_pOwner != NULL && GetCompany());

	if (!item_pos.IsValidItemPosition())
		return false;

	// ���� ��ȯ�� �� ����
	if (item_pos.IsEquipPosition())
		return false;

	LPITEM item;

	if (!(item = m_pOwner->GetItem(item_pos)))
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_GIVE))
	{
		m_pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�������� �ǳ��� �� �����ϴ�."));
		return false;
	}

	if (true == item->isLocked())
	{
		return false;
	}

#if defined(__SOUL_BIND_SYSTEM__)
	if (item->IsSealed())
	{
		m_pOwner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot give away a soulbound item."));
		return false;
	}
#endif

	// �̹� ��ȯâ�� �߰��� �������ΰ�?
	if (item->IsExchanging())
	{
		sys_log(0, "EXCHANGE under exchanging");
		return false;
	}

	if (!m_pGrid->IsEmpty(display_pos, 1, item->GetSize()))
	{
		sys_log(0, "EXCHANGE not empty item_pos %d %d %d", display_pos, 1, item->GetSize());
		return false;
	}

	Accept(false);
	GetCompany()->Accept(false);

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_apItems[i])
			continue;

		m_apItems[i] = item;
		m_aItemPos[i] = item_pos;
		m_abItemDisplayPos[i] = display_pos;
		m_pGrid->Put(display_pos, 1, item->GetSize());

		item->SetExchanging(true);

		exchange_packet(m_pOwner,
			EXCHANGE_SUBHEADER_GC_ITEM_ADD,
			true,
			item->GetVnum(),
			TItemPos(RESERVED_WINDOW, display_pos),
			item->GetCount(),
			item);

		exchange_packet(GetCompany()->GetOwner(),
			EXCHANGE_SUBHEADER_GC_ITEM_ADD,
			false,
			item->GetVnum(),
			TItemPos(RESERVED_WINDOW, display_pos),
			item->GetCount(),
			item);

		sys_log(0, "EXCHANGE AddItem success %s pos(%d, %d) %d", item->GetName(), item_pos.window_type, item_pos.cell, display_pos);

		return true;
	}

	// �߰��� ������ ����
	return false;
}

bool CExchange::RemoveItem(BYTE pos)
{
	if (pos >= EXCHANGE_ITEM_MAX_NUM)
		return false;

	if (!m_apItems[pos])
		return false;

	TItemPos PosOfInventory = m_aItemPos[pos];
	m_apItems[pos]->SetExchanging(false);

	m_pGrid->Get(m_abItemDisplayPos[pos], 1, m_apItems[pos]->GetSize());

	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_ITEM_DEL, true, pos, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_ITEM_DEL, false, pos, PosOfInventory, 0);

	Accept(false);
	GetCompany()->Accept(false);

	m_apItems[pos] = NULL;
	m_aItemPos[pos] = NPOS;
	m_abItemDisplayPos[pos] = 0;
	return true;
}

bool CExchange::AddGold(long gold)
{
	if (gold <= 0)
		return false;

	if (GetOwner()->GetGold() < gold)
	{
		// ������ �ִ� ���� ����.
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_GOLD, 0, 0, NPOS, 0);
		return false;
	}

	if (LC_IsCanada() == true || LC_IsEurope() == true)
	{
		if (m_lGold > 0)
		{
			return false;
		}
	}

	Accept(false);
	GetCompany()->Accept(false);

	m_lGold = gold;

	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_GOLD_ADD, true, m_lGold, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_GOLD_ADD, false, m_lGold, NPOS, 0);
	return true;
}

#if defined(__CHEQUE_SYSTEM__)
bool CExchange::AddCheque(long cheque)
{
	if (cheque <= 0)
		return false;

	if (GetOwner()->GetCheque() < cheque)
	{
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_CHEQUE, 0, 0, NPOS, 0);
		return false;
	}

	LPCHARACTER victim = GetCompany()->GetOwner();

	if (m_lCheque)
	{
		long vic_cheque = victim->GetCheque() + m_lCheque;
		if (vic_cheque > CHEQUE_MAX)
		{
			exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_CHEQUE, 0, 0, NPOS, 0);
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This user can not accept that much cheque."));
			return false;
		}
	}

	if (LC_IsCanada() == true || LC_IsEurope() == true)
	{
		if (m_lCheque > 0)
		{
			return false;
		}
	}

	Accept(false);
	GetCompany()->Accept(false);

	m_lCheque = cheque;

	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_CHEQUE_ADD, true, m_lCheque, NPOS, 0);
	exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_CHEQUE_ADD, false, m_lCheque, NPOS, 0);
	return true;
}
#endif

// ���� ����� �ִ���, ��ȯ�Ϸ��� �������� ������ �ִ��� Ȯ�� �Ѵ�.
bool CExchange::Check(int* piItemCount)
{
	if (GetOwner()->GetGold() < m_lGold)
		return false;

#if defined(__CHEQUE_SYSTEM__)
	if (GetOwner()->GetCheque() < m_lCheque)
		return false;
#endif

	int item_count = 0;

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!m_apItems[i])
			continue;

		if (!m_aItemPos[i].IsValidItemPosition())
			return false;

		if (m_apItems[i] != GetOwner()->GetItem(m_aItemPos[i]))
			return false;

		++item_count;
	}

	*piItemCount = item_count;
	return true;
}

bool CExchange::CheckSpace()
{
	static CGrid s_grid1(5, INVENTORY_MAX_NUM / 5 / INVENTORY_PAGE_COUNT);
	static CGrid s_grid2(5, INVENTORY_MAX_NUM / 5 / INVENTORY_PAGE_COUNT);
#if defined(__INVENTORY_4PAGES__)
	static CGrid s_grid3(5, INVENTORY_MAX_NUM / 5 / INVENTORY_PAGE_COUNT);
	static CGrid s_grid4(5, INVENTORY_MAX_NUM / 5 / INVENTORY_PAGE_COUNT);
#endif

	s_grid1.Clear();
	s_grid2.Clear();
#if defined(__INVENTORY_4PAGES__)
	s_grid3.Clear();
	s_grid4.Clear();
#endif

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	static CGrid s_grid5(5, SKILL_BOOK_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid6(5, SKILL_BOOK_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid7(5, SKILL_BOOK_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid8(5, SKILL_BOOK_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid9(5, SKILL_BOOK_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);

	static CGrid s_grid10(5, UPGRADE_ITEMS_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid11(5, UPGRADE_ITEMS_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid12(5, UPGRADE_ITEMS_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid13(5, UPGRADE_ITEMS_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid14(5, UPGRADE_ITEMS_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);

	static CGrid s_grid15(5, STONE_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid16(5, STONE_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid17(5, STONE_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid18(5, STONE_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid19(5, STONE_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);

	static CGrid s_grid20(5, GIFT_BOX_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid21(5, GIFT_BOX_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid22(5, GIFT_BOX_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid23(5, GIFT_BOX_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);
	static CGrid s_grid24(5, GIFT_BOX_INVENTORY_MAX_NUM / 5 / SPECIAL_INVENTORY_PAGE_COUNT);

	s_grid5.Clear();
	s_grid6.Clear();
	s_grid7.Clear();
	s_grid8.Clear();
	s_grid9.Clear();

	s_grid10.Clear();
	s_grid11.Clear();
	s_grid12.Clear();
	s_grid13.Clear();
	s_grid15.Clear();

	s_grid15.Clear();
	s_grid16.Clear();
	s_grid17.Clear();
	s_grid18.Clear();
	s_grid19.Clear();

	s_grid20.Clear();
	s_grid21.Clear();
	s_grid22.Clear();
	s_grid23.Clear();
	s_grid24.Clear();
#endif

	LPCHARACTER victim = GetCompany()->GetOwner();
	LPITEM item;

	int i;

	for (i = 0; i < INVENTORY_PAGE_SIZE * 1; ++i)
	{
		if (!(item = victim->GetInventoryItem(i)))
			continue;

		s_grid1.Put(i, 1, item->GetSize());
	}
	for (i = INVENTORY_PAGE_SIZE * 1; i < INVENTORY_PAGE_SIZE * 2; ++i)
	{
		if (!(item = victim->GetInventoryItem(i)))
			continue;

		s_grid2.Put(i - INVENTORY_PAGE_SIZE * 1, 1, item->GetSize());
	}
#if defined(__INVENTORY_4PAGES__)
	for (i = INVENTORY_PAGE_SIZE * 2; i < INVENTORY_PAGE_SIZE * 3; ++i)
	{
		if (!(item = victim->GetInventoryItem(i)))
			continue;

		s_grid3.Put(i - INVENTORY_PAGE_SIZE * 2, 1, item->GetSize());
	}
	for (i = INVENTORY_PAGE_SIZE * 3; i < INVENTORY_PAGE_SIZE * 4; ++i)
	{
		if (!(item = victim->GetInventoryItem(i)))
			continue;

		s_grid4.Put(i - INVENTORY_PAGE_SIZE * 3, 1, item->GetSize());
	}
#endif

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	int x;

	// SKILL_BOOK_INVENTORY
	for (x = 0; x < SKILL_BOOK_INVENTORY_MAX_NUM; ++x)
	{
		if (!(item = victim->GetSkillBookInventoryItem(SKILL_BOOK_INVENTORY_SLOT_START + x)))
			continue;

		if (x < SPECIAL_INVENTORY_PAGE_SIZE)
			s_grid5.Put(x, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 2)
			s_grid6.Put(x - SPECIAL_INVENTORY_PAGE_SIZE, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 3)
			s_grid7.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 2, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 4)
			s_grid8.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 3, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 5)
			s_grid9.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 4, 1, item->GetSize());
	}
	// END_OF_SKILL_BOOK_INVENTORY

	// UPGRADE_ITEMS_INVENTORY
	for (x = 0; x < UPGRADE_ITEMS_INVENTORY_MAX_NUM; ++x)
	{
		if (!(item = victim->GetUpgradeItemsInventoryItem(UPGRADE_ITEMS_INVENTORY_SLOT_START + x)))
			continue;

		if (x < SPECIAL_INVENTORY_PAGE_SIZE)
			s_grid10.Put(x, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 2)
			s_grid11.Put(x - SPECIAL_INVENTORY_PAGE_SIZE, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 3)
			s_grid12.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 2, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 4)
			s_grid13.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 3, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 5)
			s_grid14.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 4, 1, item->GetSize());
	}
	// END_OF_UPGRADE_ITEMS_INVENTORY

	// STONE_INVENTORY
	for (x = 0; x < STONE_INVENTORY_MAX_NUM; ++x)
	{
		if (!(item = victim->GetStoneInventoryItem(STONE_INVENTORY_SLOT_START + x)))
			continue;

		if (x < SPECIAL_INVENTORY_PAGE_SIZE)
			s_grid15.Put(x, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 2)
			s_grid16.Put(x - SPECIAL_INVENTORY_PAGE_SIZE, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 3)
			s_grid17.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 2, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 4)
			s_grid18.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 3, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 5)
			s_grid19.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 4, 1, item->GetSize());
	}
	// END_OF_STONE_INVENTORY

	// GIFT_BOX
	for (x = 0; x < GIFT_BOX_INVENTORY_MAX_NUM; ++x)
	{
		if (!(item = victim->GetGiftBoxInventoryItem(GIFT_BOX_INVENTORY_SLOT_START + x)))
			continue;

		if (x < SPECIAL_INVENTORY_PAGE_SIZE)
			s_grid20.Put(x, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 2)
			s_grid21.Put(x - SPECIAL_INVENTORY_PAGE_SIZE, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 3)
			s_grid22.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 2, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 4)
			s_grid23.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 3, 1, item->GetSize());
		else if (x < SPECIAL_INVENTORY_PAGE_SIZE * 5)
			s_grid24.Put(x - SPECIAL_INVENTORY_PAGE_SIZE * 4, 1, item->GetSize());
	}
	// END_OF_GIFT_BOX
#endif

	// ��... ���� ������ ������... ��ȥ�� �κ��� ��� �κ� ���� ���� ���� �� �߸��̴� �Ф�
	static std::vector <WORD> s_vDSGrid(DRAGON_SOUL_INVENTORY_MAX_NUM);

	// �ϴ� ��ȥ���� ��ȯ���� ���� ���ɼ��� ũ�Ƿ�, ��ȥ�� �κ� ����� ��ȥ���� ���� �� �ϵ��� �Ѵ�.
	bool bDSInitialized = false;

	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!(item = m_apItems[i]))
			continue;

		if (item->IsDragonSoul())
		{
			if (!victim->DragonSoul_IsQualified())
				return false;

			if (!bDSInitialized)
			{
				bDSInitialized = true;
				victim->CopyDragonSoulItemGrid(s_vDSGrid);
			}

			bool bExistEmptySpace = false;
			WORD wBasePos = DSManager::instance().GetBasePosition(item);
			if (wBasePos >= DRAGON_SOUL_INVENTORY_MAX_NUM)
				return false;

			for (int i = 0; i < DRAGON_SOUL_BOX_SIZE; i++)
			{
				WORD wPos = wBasePos + i;
				if (0 == s_vDSGrid[wPos])
				{
					bool bEmpty = true;
					for (int j = 1; j < item->GetSize(); j++)
					{
						if (s_vDSGrid[wPos + j * DRAGON_SOUL_BOX_COLUMN_NUM])
						{
							bEmpty = false;
							break;
						}
					}
					if (bEmpty)
					{
						for (int j = 0; j < item->GetSize(); j++)
						{
							s_vDSGrid[wPos + j * DRAGON_SOUL_BOX_COLUMN_NUM] = wPos + 1;
						}
						bExistEmptySpace = true;
						break;
					}
				}
				if (bExistEmptySpace)
					break;
			}
			if (!bExistEmptySpace)
				return false;
		}
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
		else if (item->IsSkillBook())
		{
			int iPos;

			if ((iPos = s_grid5.FindBlank(1, item->GetSize())) >= 0)
				s_grid5.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid6.FindBlank(1, item->GetSize())) >= 0)
				s_grid6.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid7.FindBlank(1, item->GetSize())) >= 0)
				s_grid7.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid8.FindBlank(1, item->GetSize())) >= 0)
				s_grid8.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid9.FindBlank(1, item->GetSize())) >= 0)
				s_grid9.Put(iPos, 1, item->GetSize());
			else
				return false; // No space left in inventory
		}
		else if (item->IsUpgradeItem())
		{
			int iPos;

			if ((iPos = s_grid10.FindBlank(1, item->GetSize())) >= 0)
				s_grid10.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid11.FindBlank(1, item->GetSize())) >= 0)
				s_grid11.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid12.FindBlank(1, item->GetSize())) >= 0)
				s_grid12.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid13.FindBlank(1, item->GetSize())) >= 0)
				s_grid13.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid14.FindBlank(1, item->GetSize())) >= 0)
				s_grid14.Put(iPos, 1, item->GetSize());
			else
				return false; // No space left in inventory
		}
		else if (item->IsStone())
		{
			int iPos;

			if ((iPos = s_grid15.FindBlank(1, item->GetSize())) >= 0)
				s_grid15.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid16.FindBlank(1, item->GetSize())) >= 0)
				s_grid16.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid17.FindBlank(1, item->GetSize())) >= 0)
				s_grid17.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid18.FindBlank(1, item->GetSize())) >= 0)
				s_grid18.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid19.FindBlank(1, item->GetSize())) >= 0)
				s_grid19.Put(iPos, 1, item->GetSize());
			else
				return false; // No space left in inventory
		}
		else if (item->IsGiftBox())
		{
			int iPos;

			if ((iPos = s_grid20.FindBlank(1, item->GetSize())) >= 0)
				s_grid20.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid21.FindBlank(1, item->GetSize())) >= 0)
				s_grid21.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid22.FindBlank(1, item->GetSize())) >= 0)
				s_grid22.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid23.FindBlank(1, item->GetSize())) >= 0)
				s_grid23.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid24.FindBlank(1, item->GetSize())) >= 0)
				s_grid24.Put(iPos, 1, item->GetSize());
			else
				return false; // No space left in inventory
		}
#endif
		else
		{
			int iPos;

			if ((iPos = s_grid1.FindBlank(1, item->GetSize())) >= 0)
				s_grid1.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid2.FindBlank(1, item->GetSize())) >= 0)
				s_grid2.Put(iPos, 1, item->GetSize());
#if defined(__INVENTORY_4PAGES__)
			else if ((iPos = s_grid3.FindBlank(1, item->GetSize())) >= 0)
				s_grid3.Put(iPos, 1, item->GetSize());
			else if ((iPos = s_grid4.FindBlank(1, item->GetSize())) >= 0)
				s_grid4.Put(iPos, 1, item->GetSize());
#endif
			else
				return false; // No space left in inventory
		}
	}

	return true;
}

// ��ȯ �� (�����۰� �� ���� ������ �ű��)
bool CExchange::Done()
{
	int empty_pos, i;
	LPITEM item;

	LPCHARACTER victim = GetCompany()->GetOwner();

#if defined(__CHEQUE_SYSTEM__)
	if (m_lCheque)
	{
		long vic_cheque = victim->GetCheque() + m_lCheque;
		if (vic_cheque > CHEQUE_MAX)
		{
			// exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_LESS_CHEQUE, 0, 0, NPOS, 0);
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The has reached cheque limit."));
			return false;
		}
	}
#endif

	for (i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (!(item = m_apItems[i]))
			continue;

		if (item->IsDragonSoul())
			empty_pos = victim->GetEmptyDragonSoulInventory(item);
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
		else if (item->IsSkillBook())
			empty_pos = victim->GetEmptySkillBookInventory(item->GetSize());
		else if (item->IsUpgradeItem())
			empty_pos = victim->GetEmptyUpgradeItemsInventory(item->GetSize());
		else if (item->IsStone())
			empty_pos = victim->GetEmptyStoneInventory(item->GetSize());
		else if (item->IsGiftBox())
			empty_pos = victim->GetEmptyGiftBoxInventory(item->GetSize());
#endif
		else
			empty_pos = victim->GetEmptyInventory(item->GetSize());

		if (empty_pos < 0)
		{
			sys_err("Exchange::Done : Cannot find blank position in inventory %s <-> %s item %s",
				m_pOwner->GetName(), victim->GetName(), item->GetName());
			continue;
		}

		if (empty_pos < 0)
		{
			sys_err("Exchange::Done : Cannot find blank position in inventory %s <-> %s item %s",
					m_pOwner->GetName(), victim->GetName(), item->GetName());
			continue;
		}

		assert(empty_pos >= 0);

		m_pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, item->GetCell(), 255);

		item->RemoveFromCharacter();
		if (item->IsDragonSoul())
			item->AddToCharacter(victim, TItemPos(DRAGON_SOUL_INVENTORY, empty_pos));
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
		else if (item->IsSkillBook())
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
		else if (item->IsUpgradeItem())
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
		else if (item->IsStone())
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
		else if (item->IsGiftBox())
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
#endif
		else
			item->AddToCharacter(victim, TItemPos(INVENTORY, empty_pos));
		ITEM_MANAGER::instance().FlushDelayedSave(item);

		item->SetExchanging(false);
		{
			char exchange_buf[51];

			snprintf(exchange_buf, sizeof(exchange_buf), "%s %u %u", item->GetName(), GetOwner()->GetPlayerID(), item->GetCount());
			LogManager::instance().ItemLog(victim, item, "EXCHANGE_TAKE", exchange_buf);

			snprintf(exchange_buf, sizeof(exchange_buf), "%s %u %u", item->GetName(), victim->GetPlayerID(), item->GetCount());
			LogManager::instance().ItemLog(GetOwner(), item, "EXCHANGE_GIVE", exchange_buf);

			if (item->GetVnum() >= 80003 && item->GetVnum() <= 80007)
			{
				LogManager::instance().GoldBarLog(victim->GetPlayerID(), item->GetID(), EXCHANGE_TAKE, "");
				LogManager::instance().GoldBarLog(GetOwner()->GetPlayerID(), item->GetID(), EXCHANGE_GIVE, "");
			}

			m_pOwner->CreateFly(FLY_SP_MEDIUM, victim);
			for (int i = 0; i < 3; i++)
				m_pOwner->CreateFly(FLY_SP_SMALL, victim);
		}

		m_apItems[i] = NULL;
	}

	if (m_lGold)
	{
		GetOwner()->PointChange(POINT_GOLD, -m_lGold, true);
		victim->PointChange(POINT_GOLD, m_lGold, true);

		if (m_lGold > 1000)
		{
			char exchange_buf[51];
			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", GetOwner()->GetPlayerID(), GetOwner()->GetName());
			LogManager::instance().CharLog(victim, m_lGold, "EXCHANGE_GOLD_TAKE", exchange_buf);

			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", victim->GetPlayerID(), victim->GetName());
			LogManager::instance().CharLog(GetOwner(), m_lGold, "EXCHANGE_GOLD_GIVE", exchange_buf);
		}
	}

#if defined(__CHEQUE_SYSTEM__)
	if (m_lCheque)
	{
		GetOwner()->PointChange(POINT_CHEQUE, -m_lCheque, true);
		victim->PointChange(POINT_CHEQUE, m_lCheque, true);

		if (m_lCheque > 1000)
		{
			char exchange_buf[51];
			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", GetOwner()->GetPlayerID(), GetOwner()->GetName());
			LogManager::instance().CharLog(victim, m_lCheque, "EXCHANGE_GOLD_TAKE", exchange_buf);

			snprintf(exchange_buf, sizeof(exchange_buf), "%u %s", victim->GetPlayerID(), victim->GetName());
			LogManager::instance().CharLog(GetOwner(), m_lCheque, "EXCHANGE_GOLD_GIVE", exchange_buf);
		}
	}
#endif

	m_pGrid->Clear();
	return true;
}

// ��ȯ�� ����
bool CExchange::Accept(bool bAccept)
{
	if (m_bAccept == bAccept)
		return true;

	m_bAccept = bAccept;

	// �� �� ���� �����Ƿ� ��ȯ ����
	if (m_bAccept && GetCompany()->m_bAccept)
	{
		int iItemCount;

		LPCHARACTER victim = GetCompany()->GetOwner();

		//PREVENT_PORTAL_AFTER_EXCHANGE
		GetOwner()->SetExchangeTime();
		victim->SetExchangeTime();
		//END_PREVENT_PORTAL_AFTER_EXCHANGE

		// exchange_check ������ ��ȯ�� �����۵��� ���ڸ��� �ֳ� Ȯ���ϰ�,
		// ��ũ�� ����� �ֳ� Ȯ���Ѵ�, �ι�° ���ڷ� ��ȯ�� ������ ����
		// �� �����Ѵ�.
		if (!Check(&iItemCount))
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �����ϰų� �������� ���ڸ��� �����ϴ�."));
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ���� �����ϰų� �������� ���ڸ��� �����ϴ�."));
			goto EXCHANGE_END;
		}

		// ���� ���� ������ ������ ������ ����ǰ�� ���� �ڸ��� �ֳ� Ȯ���Ѵ�.
		if (!CheckSpace())
		{
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}

		// ���浵 ����������..
		if (!GetCompany()->Check(&iItemCount))
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �����ϰų� �������� ���ڸ��� �����ϴ�."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ���� �����ϰų� �������� ���ڸ��� �����ϴ�."));
			goto EXCHANGE_END;
		}

		if (!GetCompany()->CheckSpace())
		{
			victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����ǰ�� �� ������ �����ϴ�."));
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
			goto EXCHANGE_END;
		}

		if (db_clientdesc->GetSocket() == INVALID_SOCKET)
		{
			sys_err("Cannot use exchange feature while DB cache connection is dead.");
			victim->ChatPacket(CHAT_TYPE_INFO, "Unknown error");
			GetOwner()->ChatPacket(CHAT_TYPE_INFO, "Unknown error");
			goto EXCHANGE_END;
		}

		if (Done())
		{
			if (m_lGold) // ���� ���� ���� ����
				GetOwner()->Save();

#if defined(__CHEQUE_SYSTEM__)
			if (m_lCheque)
				GetOwner()->Save();
#endif

			if (GetCompany()->Done())
			{
				if (GetCompany()->m_lGold) // ���� ���� ���� ����
					victim->Save();

#if defined(__CHEQUE_SYSTEM__)
				if (GetCompany()->m_lCheque)
					victim->Save();
#endif

				// INTERNATIONAL_VERSION
				GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s �԰��� ��ȯ�� ���� �Ǿ����ϴ�."), victim->GetName());
				victim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s �԰��� ��ȯ�� ���� �Ǿ����ϴ�."), GetOwner()->GetName());
				// END_OF_INTERNATIONAL_VERSION
			}
		}

	EXCHANGE_END:
		Cancel();
		return false;
	}
	else
	{
		// �ƴϸ� accept�� ���� ��Ŷ�� ������.
		exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_ACCEPT, true, m_bAccept, NPOS, 0);
		exchange_packet(GetCompany()->GetOwner(), EXCHANGE_SUBHEADER_GC_ACCEPT, false, m_bAccept, NPOS, 0);
		return true;
	}
}

// ��ȯ ���
void CExchange::Cancel()
{
	exchange_packet(GetOwner(), EXCHANGE_SUBHEADER_GC_END, 0, 0, NPOS, 0);
	GetOwner()->SetExchange(NULL);

	for (int i = 0; i < EXCHANGE_ITEM_MAX_NUM; ++i)
	{
		if (m_apItems[i])
			m_apItems[i]->SetExchanging(false);
	}

	if (GetCompany())
	{
		GetCompany()->SetCompany(NULL);
		GetCompany()->Cancel();
	}

	M2_DELETE(this);
}
