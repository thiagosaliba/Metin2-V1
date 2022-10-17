#include "stdafx.h"

#include <stack>

#include "utils.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "item_manager.h"
#include "desc.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "packet.h"
#include "affect.h"
#include "skill.h"
#include "start_position.h"
#include "mob_manager.h"
#include "db.h"
#include "log.h"
#include "vector.h"
#include "buffer_manager.h"
#include "questmanager.h"
#include "fishing.h"
#include "party.h"
#include "dungeon.h"
#include "refine.h"
#include "unique_item.h"
#include "war_map.h"
#include "xmas_event.h"
#include "marriage.h"
#include "monarch.h"
#include "polymorph.h"
#include "blend_item.h"
#include "castle.h"
#include "BattleArena.h"
#include "arena.h"
#include "dev_log.h"
#include "pcbang.h"
#include "threeway_war.h"

#include "safebox.h"
#include "shop.h"

#include "../../common/VnumHelper.h"
#include "DragonSoul.h"
#include "buff_on_attributes.h"
#include "belt_inventory_helper.h"

#if defined(__GUILD_DRAGONLAIR__)
#include "MeleyLair.h"
#endif

#if defined(__EXPRESSING_EMOTIONS__)
#include <random>
#endif

const int ITEM_BROKEN_METIN_VNUM = 28960;

// CHANGE_ITEM_ATTRIBUTES
const DWORD CHARACTER::msc_dwDefaultChangeItemAttrCycle = 10;
const char CHARACTER::msc_szLastChangeItemAttrFlag[] = "Item.LastChangeItemAttr";
const char CHARACTER::msc_szChangeItemAttrCycleFlag[] = "change_itemattr_cycle";
// END_OF_CHANGE_ITEM_ATTRIBUTES
const BYTE g_aBuffOnAttrPoints[] = { POINT_ENERGY, POINT_COSTUME_ATTR_BONUS };

struct FFindStone
{
	std::map<DWORD, LPCHARACTER> m_mapStone;

	void operator()(LPENTITY pEnt)
	{
		if (pEnt->IsType(ENTITY_CHARACTER) == true)
		{
			LPCHARACTER pChar = (LPCHARACTER)pEnt;

			if (pChar->IsStone() == true)
			{
				m_mapStone[(DWORD)pChar->GetVID()] = pChar;
			}
		}
	}
};

// ��ȯ��, ��ȯ����, ��ȥ����
bool IS_SUMMON_ITEM(int vnum) //< 2020.08.08.Owsap - Check function externally.
{
	switch (vnum)
	{
	case 22000:
	case 22010:
	case 22011:
	case 22020:
	case ITEM_MARRIAGE_RING:
		return true;
	}

	return false;
}

bool IS_MONKEY_DUNGEON(int map_index)
{
	switch (map_index)
	{
	case 5:
	case 25:
	case 45:
	case 108:
	case 109:
		return true;
	}

	return false;
}

bool IS_MAZE_DUNGEON(int map_index)
{
	switch (map_index)
	{
	case 382:
	case 383:
	case 384:
		return true;
	}

	return false;
}

bool IS_SUMMONABLE_ZONE(int map_index)
{
	// ��Ű����
	if (IS_MONKEY_DUNGEON(map_index))
		return false;

	if (IS_MAZE_DUNGEON(map_index))
		return false;

	// ��
	if (IS_CASTLE_MAP(map_index))
		return false;

	switch (map_index)
	{
	case 66: // ���Ÿ��
	case 71: // �Ź� ���� 2��
	case 72: // õ�� ����
	case 73: // õ�� ���� 2��
	case 193: // �Ź� ���� 2-1��
#if 0
	case 184: // õ�� ����(�ż�)
	case 185: // õ�� ���� 2��(�ż�)
	case 186: // õ�� ����(õ��)
	case 187: // õ�� ���� 2��(õ��)
	case 188: // õ�� ����(����)
	case 189: // õ�� ���� 2��(����)
#endif
	// case 206: // �Ʊ͵���
	case 216: // �Ʊ͵���
	case 217: // �Ź� ���� 3��
	case 208: // õ�� ���� (���)

	case 113: // OX Event ��
		return false;
	}

	if (CBattleArena::IsBattleArenaMap(map_index)) return false;

	// ��� private ������ ���� �Ұ���
	if (map_index > 10000) return false;

	return true;
}

bool IS_BOTARYABLE_ZONE(int nMapIndex)
{
	if (LC_IsYMIR() == false && LC_IsKorea() == false) return true;

	switch (nMapIndex)
	{
	case 1:
	case 3:
	case 21:
	case 23:
	case 41:
	case 43:
		return true;
	}

	return false;
}

// item socket �� ������Ÿ�԰� ������ üũ -- by mhh
static bool FN_check_item_socket(LPITEM item)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		if (item->GetSocket(i) != item->GetProto()->alSockets[i])
			return false;
	}

	return true;
}

// item socket ���� -- by mhh
static void FN_copy_item_socket(LPITEM dest, LPITEM src)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		dest->SetSocket(i, src->GetSocket(i));
	}
}

static bool FN_check_item_sex(LPCHARACTER ch, LPITEM item)
{
	// ���� ����
	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_MALE))
	{
		if (SEX_MALE == GET_SEX(ch))
			return false;
	}

	// ���ڱ���
	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_FEMALE))
	{
		if (SEX_FEMALE == GET_SEX(ch))
			return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ITEM HANDLING
/////////////////////////////////////////////////////////////////////////////
bool CHARACTER::CanHandleItem(bool bSkipCheckRefine, bool bSkipObserver)
{
	if (!bSkipObserver)
		if (m_bIsObserver)
			return false;

	if (GetMyShop())
		return false;

	if (!bSkipCheckRefine)
		if (IsUnderRefine())
			return false;

	if (IsCubeOpen() || NULL != DragonSoul_RefineWindow_GetOpener())
		return false;

#if defined(__CHANGE_LOOK_SYSTEM__)
	if (GetChangeLook())
		return false;
#endif

#if defined(__MAILBOX__)
	if (GetMailBox())
		return false;
#endif

	if (IsWarping())
		return false;

#if defined(__ACCE_COSTUME_SYSTEM__)
	if ((m_bAcceCombination) || (m_bAcceAbsorption))
		return false;
#endif

	return true;
}

LPITEM CHARACTER::GetInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(INVENTORY, wCell));
}

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
LPITEM CHARACTER::GetSkillBookInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(INVENTORY, wCell));
}

LPITEM CHARACTER::GetUpgradeItemsInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(INVENTORY, wCell));
}

LPITEM CHARACTER::GetStoneInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(INVENTORY, wCell));
}

LPITEM CHARACTER::GetGiftBoxInventoryItem(WORD wCell) const
{
	return GetItem(TItemPos(INVENTORY, wCell));
}
#endif

LPITEM CHARACTER::GetDragonSoulItem(const WORD& wCell) const
{
	return GetItem(TItemPos(DRAGON_SOUL_INVENTORY, wCell));
}

LPITEM CHARACTER::GetItem(TItemPos Cell) const
{
	if (!IsValidItemPosition(Cell))
		return NULL;

	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;

	switch (window_type)
	{
	case INVENTORY:
	case EQUIPMENT:
		if (wCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid item cell %d", wCell);
			return NULL;
		}

		return m_pointsInstant.pItems[wCell];

	case DRAGON_SOUL_INVENTORY:
		if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
		{
			sys_err("CHARACTER::GetInventoryItem: invalid DS item cell %d", wCell);
			return NULL;
		}

		return m_pointsInstant.pDSItems[wCell];

	default:
		return NULL;
	}

	return NULL;
}

#if defined(__SWAP_ITEM_SYSTEM__)
LPITEM CHARACTER::GetItem_NEW(TItemPos Cell) const
{
	LPITEM cellItem = GetItem(Cell);
	if (cellItem)
		return cellItem;

	// There's no item in this cell, but that does not mean there is not an item which currently uses up this cell.
	WORD bCell = Cell.cell;
	BYTE bPage = bCell / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT);

	int j = -2;
	for (int j = -2; j < 0; ++j)
	{
		BYTE p = bCell + (5 * j);

		if (p / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT) != bPage)
			continue;

		if (p >= INVENTORY_MAX_NUM) // Eeh just for the sake of...
			continue;

		LPITEM curCellItem = GetItem(TItemPos(INVENTORY, p));
		if (!curCellItem)
			continue;

		if (p + (curCellItem->GetSize() - 1) * 5 < Cell.cell) // Doesn't reach Cell.cell
			continue;

		return curCellItem;
	}

	return NULL;
}
#endif

#if defined(__WJ_PICKUP_ITEM_EFFECT__)
void CHARACTER::SetItem(TItemPos Cell, LPITEM pItem, bool isHighLight)
#else
void CHARACTER::SetItem(TItemPos Cell, LPITEM pItem)
#endif
{
	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;

	if ((unsigned long)((CItem*)pItem) == 0xff || (unsigned long)((CItem*)pItem) == 0xffffffff)
	{
		sys_err("!!! FATAL ERROR !!! item == 0xff (char: %s cell: %u)", GetName(), wCell);
		core_dump();
		return;
	}

	if (pItem && pItem->GetOwner())
	{
		assert(!"GetOwner exist");
		return;
	}

	// �⺻ �κ��丮
	switch (window_type)
	{
	case INVENTORY:
	case EQUIPMENT:
	{
		if (wCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
		{
			sys_err("CHARACTER::SetItem: invalid item cell %d", wCell);
			return;
		}

		LPITEM pOld = m_pointsInstant.pItems[wCell];

		if (pOld)
		{
			if (wCell < INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= INVENTORY_MAX_NUM)
						continue;

					if (m_pointsInstant.pItems[p] && m_pointsInstant.pItems[p] != pOld)
						continue;

					m_pointsInstant.bItemGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.bItemGrid[wCell] = 0;
		}

		if (pItem)
		{
			if (wCell < INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * 5);

					if (p >= INVENTORY_MAX_NUM)
						continue;

					// wCell + 1 �� �ϴ� ���� ����� üũ�� �� ����
					// �������� ����ó���ϱ� ����
					m_pointsInstant.bItemGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.bItemGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pItems[wCell] = pItem;
	}
	break;

	// ��ȥ�� �κ��丮
	case DRAGON_SOUL_INVENTORY:
	{
		LPITEM pOld = m_pointsInstant.pDSItems[wCell];

		if (pOld)
		{
			if (wCell < DRAGON_SOUL_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pOld->GetSize(); ++i)
				{
					int p = wCell + (i * DRAGON_SOUL_BOX_COLUMN_NUM);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						continue;

					if (m_pointsInstant.pDSItems[p] && m_pointsInstant.pDSItems[p] != pOld)
						continue;

					m_pointsInstant.wDSItemGrid[p] = 0;
				}
			}
			else
				m_pointsInstant.wDSItemGrid[wCell] = 0;
		}

		if (pItem)
		{
			if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
			{
				sys_err("CHARACTER::SetItem: invalid DS item cell %d", wCell);
				return;
			}

			if (wCell < DRAGON_SOUL_INVENTORY_MAX_NUM)
			{
				for (int i = 0; i < pItem->GetSize(); ++i)
				{
					int p = wCell + (i * DRAGON_SOUL_BOX_COLUMN_NUM);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						continue;

					// wCell + 1 �� �ϴ� ���� ����� üũ�� �� ����
					// �������� ����ó���ϱ� ����
					m_pointsInstant.wDSItemGrid[p] = wCell + 1;
				}
			}
			else
				m_pointsInstant.wDSItemGrid[wCell] = wCell + 1;
		}

		m_pointsInstant.pDSItems[wCell] = pItem;
	}
	break;

	default:
		sys_err("Invalid Inventory type %d", window_type);
		return;
	}

	if (GetDesc())
	{
		// Ȯ�� ������: �������� ������ �÷��� ������ ������
		if (pItem)
		{
			TPacketGCItemSet pack;
			pack.header = HEADER_GC_ITEM_SET;
			pack.Cell = Cell;

			pack.count = pItem->GetCount();
#if defined(__SOUL_BIND_SYSTEM__)
			pack.soulbind = pItem->GetSealedTime();
#endif
			pack.vnum = pItem->GetVnum();
			pack.flags = pItem->GetFlag();
			pack.anti_flags = pItem->GetAntiFlag();
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			if (isHighLight)
				pack.highlight = true;
			else
				pack.highlight = (Cell.window_type == DRAGON_SOUL_INVENTORY);
#else
			pack.highlight = (Cell.window_type == DRAGON_SOUL_INVENTORY);
#endif

			thecore_memcpy(pack.alSockets, pItem->GetSockets(), sizeof(pack.alSockets));
#if defined(__ITEM_APPLY_RANDOM__)
			thecore_memcpy(pack.aApplyRandom, pItem->GetRandomApplies(), sizeof(pack.aApplyRandom));
#endif
			thecore_memcpy(pack.aAttr, pItem->GetAttributes(), sizeof(pack.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
			pack.dwTransmutationVnum = pItem->GetTransmutationVnum();
#endif

			GetDesc()->Packet(&pack, sizeof(TPacketGCItemSet));
		}
		else
		{
			TPacketGCItemSetEmpty pack;
			pack.header = HEADER_GC_ITEM_DEL;
			pack.Cell = Cell;
			pack.count = 0;
			pack.vnum = 0;
			memset(pack.alSockets, 0, sizeof(pack.alSockets));
#if defined(__ITEM_APPLY_RANDOM__)
			memset(pack.aApplyRandom, 0, sizeof(pack.aApplyRandom));
#endif
			memset(pack.aAttr, 0, sizeof(pack.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
			pack.dwTransmutationVnum = 0;
#endif

			GetDesc()->Packet(&pack, sizeof(TPacketGCItemSetEmpty));
		}
	}

	if (pItem)
	{
		pItem->SetCell(this, wCell);
		switch (window_type)
		{
		case INVENTORY:
		case EQUIPMENT:
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
			if ((wCell < INVENTORY_MAX_NUM) || (BELT_INVENTORY_SLOT_START <= wCell && BELT_INVENTORY_SLOT_END > wCell) || (SKILL_BOOK_INVENTORY_SLOT_START <= wCell && SKILL_BOOK_INVENTORY_SLOT_END > wCell) || (UPGRADE_ITEMS_INVENTORY_SLOT_START <= wCell && UPGRADE_ITEMS_INVENTORY_SLOT_END > wCell) || (STONE_INVENTORY_SLOT_START <= wCell && STONE_INVENTORY_SLOT_END > wCell) || (GIFT_BOX_INVENTORY_SLOT_START <= wCell && GIFT_BOX_INVENTORY_SLOT_END > wCell))
#else
			if ((wCell < INVENTORY_MAX_NUM) || (BELT_INVENTORY_SLOT_START <= wCell && BELT_INVENTORY_SLOT_END > wCell))
#endif
				pItem->SetWindow(INVENTORY);
			else
				pItem->SetWindow(EQUIPMENT);
			break;

		case DRAGON_SOUL_INVENTORY:
			pItem->SetWindow(DRAGON_SOUL_INVENTORY);
			break;

		default:
			sys_err("Trying to set window %d, non determined behaviour!", window_type);
		}
	}
}

LPITEM CHARACTER::GetWear(UINT bCell) const
{
	// > WEAR_MAX_NUM : ��ȥ�� ���Ե�.
	if (bCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
	{
		sys_err("CHARACTER::GetWear: invalid wear cell %d", bCell);
		return NULL;
	}

	return m_pointsInstant.pItems[INVENTORY_MAX_NUM + bCell];
}

void CHARACTER::SetWear(UINT bCell, LPITEM item)
{
	// > WEAR_MAX_NUM : ��ȥ�� ���Ե�.
	if (bCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
	{
		sys_err("CHARACTER::SetItem: invalid item cell %d", bCell);
		return;
	}

	SetItem(TItemPos(INVENTORY, INVENTORY_MAX_NUM + bCell), item);

	/*
	if (!item && bCell == WEAR_WEAPON)
	{
		// �Ͱ� ��� �� ���� ���̶�� ȿ���� ���־� �Ѵ�.
		if (IsAffectFlag(AFF_GWIGUM))
			RemoveAffect(SKILL_GWIGEOM);

		if (IsAffectFlag(AFF_GEOMGYEONG))
			RemoveAffect(SKILL_GEOMKYUNG);
	}
	*/
}

void CHARACTER::ClearItem()
{
	int i;
	LPITEM item;

	for (i = 0; i < INVENTORY_AND_EQUIP_SLOT_MAX; ++i)
	{
		if ((item = GetInventoryItem(i)))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);

			SyncQuickslot(QUICKSLOT_TYPE_ITEM, i, WORD_MAX);
		}
	}

	for (i = 0; i < DRAGON_SOUL_INVENTORY_MAX_NUM; ++i)
	{
		if ((item = GetItem(TItemPos(DRAGON_SOUL_INVENTORY, i))))
		{
			item->SetSkipSave(true);
			ITEM_MANAGER::instance().FlushDelayedSave(item);

			item->RemoveFromCharacter();
			M2_DESTROY_ITEM(item);
		}
	}
}

bool CHARACTER::IsEmptyItemGrid(TItemPos Cell, BYTE bSize, int iExceptionCell) const
{
	switch (Cell.window_type)
	{
	case INVENTORY:
	{
		UINT bCell = Cell.cell;

		// bItemCell�� 0�� false���� ��Ÿ���� ���� + 1 �ؼ� ó���Ѵ�.
		// ���� iExceptionCell�� 1�� ���� ���Ѵ�.
		++iExceptionCell;

		if (Cell.IsBeltInventoryPosition())
		{
			LPITEM beltItem = GetWear(WEAR_BELT);

			if (NULL == beltItem)
				return false;

			if (false == CBeltInventoryHelper::IsAvailableCell(bCell - BELT_INVENTORY_SLOT_START, beltItem->GetValue(0)))
				return false;

			if (m_pointsInstant.bItemGrid[bCell])
			{
				if (m_pointsInstant.bItemGrid[bCell] == (UINT)iExceptionCell)
					return true;

				return false;
			}

			if (bSize == 1)
				return true;
		}
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
		else if (Cell.IsSkillBookInventoryPosition())
		{
			if (bCell < SKILL_BOOK_INVENTORY_SLOT_START)
				return false;

			if (bCell > SKILL_BOOK_INVENTORY_SLOT_END)
				return false;

			if (m_pointsInstant.bItemGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				BYTE bPage = bCell / (SPECIAL_INVENTORY_MAX_NUM / SPECIAL_INVENTORY_PAGE_COUNT);

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						return false;

					if (p / (SPECIAL_INVENTORY_MAX_NUM / SPECIAL_INVENTORY_PAGE_COUNT) != bPage)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
		}
		else if (Cell.IsUpgradeItemsInventoryPosition())
		{
			if (bCell < UPGRADE_ITEMS_INVENTORY_SLOT_START)
				return false;

			if (bCell > UPGRADE_ITEMS_INVENTORY_SLOT_END)
				return false;

			if (m_pointsInstant.bItemGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				BYTE bPage = bCell / (SPECIAL_INVENTORY_MAX_NUM / SPECIAL_INVENTORY_PAGE_COUNT);

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						return false;

					if (p / (SPECIAL_INVENTORY_MAX_NUM / SPECIAL_INVENTORY_PAGE_COUNT) != bPage)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
		}
		else if (Cell.IsStoneInventoryPosition())
		{
			if (bCell < STONE_INVENTORY_SLOT_START)
				return false;

			if (bCell > STONE_INVENTORY_SLOT_END)
				return false;

			if (m_pointsInstant.bItemGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				BYTE bPage = bCell / (SPECIAL_INVENTORY_MAX_NUM / SPECIAL_INVENTORY_PAGE_COUNT);

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						return false;

					if (p / (SPECIAL_INVENTORY_MAX_NUM / SPECIAL_INVENTORY_PAGE_COUNT) != bPage)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
		}
		else if (Cell.IsGiftBoxInventoryPosition())
		{
			if (bCell < GIFT_BOX_INVENTORY_SLOT_START)
				return false;

			if (bCell > GIFT_BOX_INVENTORY_SLOT_END)
				return false;

			if (m_pointsInstant.bItemGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				BYTE bPage = bCell / (SPECIAL_INVENTORY_MAX_NUM / SPECIAL_INVENTORY_PAGE_COUNT);

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= SPECIAL_INVENTORY_MAX_NUM)
						return false;

					if (p / (SPECIAL_INVENTORY_MAX_NUM / SPECIAL_INVENTORY_PAGE_COUNT) != bPage)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
		}
#endif
		else if (bCell >= INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.bItemGrid[bCell])
		{
			if (m_pointsInstant.bItemGrid[bCell] == (UINT)iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				BYTE bPage = bCell / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT);

				do
				{
					UINT p = bCell + (5 * j);

					if (p >= INVENTORY_MAX_NUM)
						return false;

					if (p / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT) != bPage)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// ũ�Ⱑ 1�̸� ��ĭ�� �����ϴ� ���̹Ƿ� �׳� ����
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			BYTE bPage = bCell / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT);

			do
			{
				UINT p = bCell + (5 * j);

				if (p >= INVENTORY_MAX_NUM)
					return false;

				if (p / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT) != bPage)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;

	case DRAGON_SOUL_INVENTORY:
	{
		WORD wCell = Cell.cell;
		if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
			return false;

		// bItemCell�� 0�� false���� ��Ÿ���� ���� + 1 �ؼ� ó���Ѵ�.
		// ���� iExceptionCell�� 1�� ���� ���Ѵ�.
		iExceptionCell++;

		if (m_pointsInstant.wDSItemGrid[wCell])
		{
			if (m_pointsInstant.wDSItemGrid[wCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					WORD p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.wDSItemGrid[p])
						if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// ũ�Ⱑ 1�̸� ��ĭ�� �����ϴ� ���̹Ƿ� �׳� ����
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				WORD p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

				if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;

	}

	return false;
}

bool CHARACTER::IsEmptyItemGridSpecial(TItemPos Cell, BYTE bSize, int iExceptionCell, std::vector<WORD>& vec) const
{
	if (std::find(vec.begin(), vec.end(), Cell.cell) != vec.end())
	{
		return false;
	}

	switch (Cell.window_type)
	{
	case INVENTORY:
	{
		WORD bCell = (WORD)Cell.cell;

		// bItemCell�� 0�� false���� ��Ÿ���� ���� + 1 �ؼ� ó���Ѵ�.
		// ���� iExceptionCell�� 1�� ���� ���Ѵ�.
		++iExceptionCell;

		if (Cell.IsBeltInventoryPosition())
		{
			LPITEM beltItem = GetWear(WEAR_BELT);

			if (NULL == beltItem)
				return false;

			if (false == CBeltInventoryHelper::IsAvailableCell(bCell - BELT_INVENTORY_SLOT_START, beltItem->GetValue(0)))
				return false;

			if (m_pointsInstant.bItemGrid[bCell])
			{
				if (m_pointsInstant.bItemGrid[bCell] == iExceptionCell)
					return true;

				return false;
			}

			if (bSize == 1)
				return true;

		}
		else if (bCell >= INVENTORY_MAX_NUM)
			return false;

		if (m_pointsInstant.bItemGrid[bCell])
		{
			if (m_pointsInstant.bItemGrid[bCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;
				WORD bPage = bCell / (INVENTORY_PAGE_SIZE);

				do
				{
					WORD p = bCell + (5 * j);

					if (p >= INVENTORY_MAX_NUM)
						return false;

					if (p / (INVENTORY_PAGE_SIZE) != bPage)
						return false;

					if (m_pointsInstant.bItemGrid[p])
						if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// ũ�Ⱑ 1�̸� ��ĭ�� �����ϴ� ���̹Ƿ� �׳� ����
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;
			WORD bPage = bCell / (INVENTORY_PAGE_SIZE);

			do
			{
				WORD p = bCell + (5 * j);

				if (p >= INVENTORY_MAX_NUM)
					return false;

				if (p / (INVENTORY_PAGE_SIZE) != bPage)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.bItemGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	break;

	case DRAGON_SOUL_INVENTORY:
	{
		WORD wCell = Cell.cell;
		if (wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
			return false;

		// bItemCell�� 0�� false���� ��Ÿ���� ���� + 1 �ؼ� ó���Ѵ�.
		// ���� iExceptionCell�� 1�� ���� ���Ѵ�.
		iExceptionCell++;

		if (m_pointsInstant.wDSItemGrid[wCell])
		{
			if (m_pointsInstant.wDSItemGrid[wCell] == iExceptionCell)
			{
				if (bSize == 1)
					return true;

				int j = 1;

				do
				{
					WORD p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

					if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
						return false;

					if (m_pointsInstant.wDSItemGrid[p])
						if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
							return false;
				} while (++j < bSize);

				return true;
			}
			else
				return false;
		}

		// ũ�Ⱑ 1�̸� ��ĭ�� �����ϴ� ���̹Ƿ� �׳� ����
		if (1 == bSize)
			return true;
		else
		{
			int j = 1;

			do
			{
				WORD p = wCell + (DRAGON_SOUL_BOX_COLUMN_NUM * j);

				if (p >= DRAGON_SOUL_INVENTORY_MAX_NUM)
					return false;

				if (m_pointsInstant.bItemGrid[p])
					if (m_pointsInstant.wDSItemGrid[p] != iExceptionCell)
						return false;
			} while (++j < bSize);

			return true;
		}
	}
	}

	return false;
}

int CHARACTER::GetEmptyInventory(BYTE size) const
{
	// NOTE : ���� �� �Լ��� ������ ����, ȹ�� ���� ������ �� �� �κ��丮�� �� ĭ�� ã�� ���� ���ǰ� �ִµ�,
	// ��Ʈ �κ��丮�� Ư�� �κ��丮�̹Ƿ� �˻����� �ʵ��� �Ѵ�. (�⺻ �κ��丮: INVENTORY_MAX_NUM ������ �˻�)
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		if (IsEmptyItemGrid(TItemPos(INVENTORY, i), size))
			return i;
	return -1;
}

int CHARACTER::GetEmptyInventoryCount(BYTE size) const
{
	// NOTE : ���� �� �Լ��� ������ ����, ȹ�� ���� ������ �� �� �κ��丮�� �� ĭ�� ã�� ���� ���ǰ� �ִµ�,
	// ��Ʈ �κ��丮�� Ư�� �κ��丮�̹Ƿ� �˻����� �ʵ��� �Ѵ�. (�⺻ �κ��丮: INVENTORY_MAX_NUM ������ �˻�)
	int emptyCount = 0;
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (IsEmptyItemGrid(TItemPos(INVENTORY, i), size))
			++emptyCount;
	}

	return emptyCount;
}

int CHARACTER::GetEmptyDragonSoulInventory(LPITEM pItem) const
{
	if (NULL == pItem || !pItem->IsDragonSoul())
		return -1;
	if (!DragonSoul_IsQualified())
	{
		return -1;
	}
	BYTE bSize = pItem->GetSize();
	WORD wBaseCell = DSManager::instance().GetBasePosition(pItem);

	if (WORD_MAX == wBaseCell)
		return -1;

	for (int i = 0; i < DRAGON_SOUL_BOX_SIZE; ++i)
		if (IsEmptyItemGrid(TItemPos(DRAGON_SOUL_INVENTORY, i + wBaseCell), bSize))
			return i + wBaseCell;

	return -1;
}

int CHARACTER::GetEmptyDragonSoulInventoryWithExceptions(LPITEM pItem, std::vector<WORD>& vec /*= -1*/) const
{
	if (NULL == pItem || !pItem->IsDragonSoul())
		return -1;

	if (!DragonSoul_IsQualified())
	{
		return -1;
	}

	BYTE bSize = pItem->GetSize();
	WORD wBaseCell = DSManager::instance().GetBasePosition(pItem);

	if (WORD_MAX == wBaseCell)
		return -1;

	for (int i = 0; i < DRAGON_SOUL_BOX_SIZE; ++i)
		if (IsEmptyItemGridSpecial(TItemPos(DRAGON_SOUL_INVENTORY, i + wBaseCell), bSize, -1, vec))
			return i + wBaseCell;

	return -1;
}

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
int CHARACTER::GetEmptySkillBookInventory(BYTE size) const
{
	for (int i = SKILL_BOOK_INVENTORY_SLOT_START; i < SKILL_BOOK_INVENTORY_SLOT_END; ++i)
		if (IsEmptyItemGrid(TItemPos(INVENTORY, i), size))
			return i;

	return -1;
}

int CHARACTER::GetEmptyUpgradeItemsInventory(BYTE size) const
{
	for (int i = UPGRADE_ITEMS_INVENTORY_SLOT_START; i < UPGRADE_ITEMS_INVENTORY_SLOT_END; ++i)
		if (IsEmptyItemGrid(TItemPos(INVENTORY, i), size))
			return i;

	return -1;
}

int CHARACTER::GetEmptyStoneInventory(BYTE size) const
{
	for (int i = STONE_INVENTORY_SLOT_START; i < STONE_INVENTORY_SLOT_END; ++i)
		if (IsEmptyItemGrid(TItemPos(INVENTORY, i), size))
			return i;

	return -1;
}

int CHARACTER::GetEmptyGiftBoxInventory(BYTE size) const
{
	for (int i = GIFT_BOX_INVENTORY_SLOT_START; i < GIFT_BOX_INVENTORY_SLOT_END; ++i)
		if (IsEmptyItemGrid(TItemPos(INVENTORY, i), size))
			return i;

	return -1;
}
#endif

void CHARACTER::CopyDragonSoulItemGrid(std::vector<WORD>& vDragonSoulItemGrid) const
{
	vDragonSoulItemGrid.resize(DRAGON_SOUL_INVENTORY_MAX_NUM);

	std::copy(m_pointsInstant.wDSItemGrid, m_pointsInstant.wDSItemGrid + DRAGON_SOUL_INVENTORY_MAX_NUM, vDragonSoulItemGrid.begin());
}

int CHARACTER::CountEmptyInventory() const
{
	int count = 0;

	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		if (GetInventoryItem(i))
			count += GetInventoryItem(i)->GetSize();

	return (INVENTORY_MAX_NUM - count);
}

void TransformRefineItem(LPITEM pkOldItem, LPITEM pkNewItem)
{
	// ACCESSORY_REFINE
	if (pkOldItem->IsAccessoryForSocket())
	{
		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			pkNewItem->SetSocket(i, pkOldItem->GetSocket(i));
		}
		//pkNewItem->StartAccessorySocketExpireEvent();
	}
	// END_OF_ACCESSORY_REFINE
	else
	{
		// ���⼭ �������� �ڵ������� û�� ��
		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			if (!pkOldItem->GetSocket(i))
				break;
			else
				pkNewItem->SetSocket(i, 1);
		}

		// ���� ����
		int slot = 0;

		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			long socket = pkOldItem->GetSocket(i);

			if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
				pkNewItem->SetSocket(slot++, socket);
		}

	}

	// ���� ������ ����
	pkOldItem->CopyAttributeTo(pkNewItem);
}

void NotifyRefineSuccess(LPCHARACTER ch, LPITEM item, const char* way)
{
	if (NULL != ch && item != NULL)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "RefineSuceeded");

		LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), item->GetRefineLevel(), 1, way);
	}
}

void NotifyRefineFail(LPCHARACTER ch, LPITEM item, const char* way, int success = 0)
{
	if (NULL != ch && NULL != item)
	{
		ch->ChatPacket(CHAT_TYPE_COMMAND, "RefineFailed");

		LogManager::instance().RefineLog(ch->GetPlayerID(), item->GetName(), item->GetID(), item->GetRefineLevel(), success, way);
	}
}

#if defined(__REFINE_FAIL_TYPE__)
void NotifyRefineFailType(const LPCHARACTER pkChr, const LPITEM pkItem, const BYTE bType, const std::string stRefineType, const BYTE bSuccess = 0)
{
	if (pkChr && pkItem)
	{
		pkChr->ChatPacket(CHAT_TYPE_COMMAND, "RefineFailedType %d", bType);
		LogManager::instance().RefineLog(pkChr->GetPlayerID(), pkItem->GetName(), pkItem->GetID(), pkItem->GetRefineLevel(), bSuccess, stRefineType.c_str());
	}
}
#endif

void CHARACTER::SetRefineNPC(LPCHARACTER ch)
{
	if (ch != NULL)
	{
		m_dwRefineNPCVID = ch->GetVID();
	}
	else
	{
		m_dwRefineNPCVID = 0;
	}
}

bool CHARACTER::DoRefine(LPITEM item, bool bMoneyOnly)
{
	if (!CanHandleItem(true))
	{
		ClearRefineMode();
		return false;
	}

	//���� �ð����� : upgrade_refine_scroll.quest ���� ������ 5���̳��� �Ϲ� ������
	//�����Ҽ� ����
	if (quest::CQuestManager::instance().GetEventFlag("update_refine_time") != 0)
	{
		if (get_global_time() < quest::CQuestManager::instance().GetEventFlag("update_refine_time") + (60 * 5))
		{
			sys_log(0, "can't refine %d %s", GetPlayerID(), GetName());
			return false;
		}
	}

	const TRefineTable* prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());

	if (!prt)
		return false;

	DWORD result_vnum = item->GetRefinedVnum();

	// REFINE_COST
	int cost = ComputeRefineFee(prt->cost);

	int RefineChance = GetQuestFlag("main_quest_lv7.refine_chance");

	if (RefineChance > 0)
	{
		if (!item->CheckItemUseLevel(20) || item->GetType() != ITEM_WEAPON)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���� ��ȸ�� 20 ������ ���⸸ �����մϴ�"));
			return false;
		}
		cost = 0;
		SetQuestFlag("main_quest_lv7.refine_chance", RefineChance - 1);
	}
	// END_OF_REFINE_COST

	if (result_vnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �̻� ������ �� �����ϴ�."));
		return false;
	}

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_TUNING)
		return false;

	TItemTable* pProto = ITEM_MANAGER::instance().GetTable(item->GetRefinedVnum());

	if (!pProto)
	{
		sys_err("DoRefine NOT GET ITEM PROTO %d", item->GetRefinedVnum());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� ������ �� �����ϴ�."));
		return false;
	}

	// Check level limit in korea only
	if (!g_iUseLocale)
	{
		for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
		{
			long limit = pProto->aLimits[i].lValue;

			switch (pProto->aLimits[i].bType)
			{
			case LIMIT_LEVEL:
				if (GetLevel() < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �� �������� ���� ���Ѻ��� ������ �����ϴ�."));
					return false;
				}
				break;
			}
		}
	}

	// REFINE_COST
	if (GetGold() < (long long)cost)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ϱ� ���� ���� �����մϴ�."));
		return false;
	}

	if (!bMoneyOnly && !RefineChance)
	{
		for (int i = 0; i < prt->material_count; ++i)
		{
			if (CountSpecifyItem(prt->materials[i].vnum, item->GetCell()) < prt->materials[i].count)
			{
				if (test_server)
				{
					ChatPacket(CHAT_TYPE_INFO, "Find %d, count %d, require %d", prt->materials[i].vnum, CountSpecifyItem(prt->materials[i].vnum), prt->materials[i].count);
				}

				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ϱ� ���� ��ᰡ �����մϴ�."));
				return false;
			}
		}

		for (int i = 0; i < prt->material_count; ++i)
			RemoveSpecifyItem(prt->materials[i].vnum, prt->materials[i].count, item->GetCell());
	}

	int prob = number(1, 100);

	if (IsRefineThroughGuild() || bMoneyOnly)
		prob -= 10;

	// END_OF_REFINE_COST

	if (prob <= prt->prob)
	{
		// ����! ��� �������� �������, ���� �Ӽ��� �ٸ� ������ ȹ��
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_vnum, 1, 0, false);

		if (pkNewItem)
		{
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

			UINT bCell = item->GetCell();

			// DETAIL_REFINE_LOG
			NotifyRefineSuccess(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");
			// END_OF_DETAIL_REFINE_LOG

			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

			sys_log(0, "Refine Success %d", cost);
			pkNewItem->AttrLog();
			//PointChange(POINT_GOLD, -cost);
			sys_log(0, "PayPee %d", cost);
			PayRefineFee(cost);
			sys_log(0, "PayPee End %d", cost);
		}
		else
		{
			// DETAIL_REFINE_LOG
			// ������ ������ ���� -> ���� ���з� ����
			sys_err("cannot create item %u", result_vnum);
#if defined(__REFINE_FAIL_TYPE__)
			NotifyRefineFailType(this, item, REFINE_FAIL_KEEP_GRADE, IsRefineThroughGuild() ? "GUILD" : "POWER");
#else
			NotifyRefineFail(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
#endif
			// END_OF_DETAIL_REFINE_LOG
		}
	}
	else
	{
		// ����! ��� �������� �����.
		DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -cost);
#if defined(__REFINE_FAIL_TYPE__)
		NotifyRefineFailType(this, item, REFINE_FAIL_DEL_ITEM, IsRefineThroughGuild() ? "GUILD" : "POWER");
#else
		NotifyRefineFail(this, item, IsRefineThroughGuild() ? "GUILD" : "POWER");
#endif
		item->AttrLog();
		ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE FAIL)");

		//PointChange(POINT_GOLD, -cost);
		PayRefineFee(cost);
	}

	return true;
}

enum enum_RefineScrolls
{
	CHUKBOK_SCROLL = 0,
	HYUNIRON_CHN = 1, // �߱������� ���
	YONGSIN_SCROLL = 2,
	MUSIN_SCROLL = 3,
	YAGONG_SCROLL = 4,
	MEMO_SCROLL = 5,
	BDRAGON_SCROLL = 6,
#if defined(__STONE_OF_BLESS__)
	STONE_OF_BLESS = 7,
#endif
#if defined(__SOUL_SYSTEM__)
	SOUL_EVOLVE_SCROLL = 8,
	SOUL_AWAKE_SCROLL = 9,
#endif
};

bool CHARACTER::DoRefineWithScroll(LPITEM item)
{
	if (!CanHandleItem(true))
	{
		ClearRefineMode();
		return false;
	}

	ClearRefineMode();

	// ���� �ð����� : upgrade_refine_scroll.quest ���� ������ 5���̳��� �Ϲ� ������
	// �����Ҽ� ����
	if (quest::CQuestManager::instance().GetEventFlag("update_refine_time") != 0)
	{
		if (get_global_time() < quest::CQuestManager::instance().GetEventFlag("update_refine_time") + (60 * 5))
		{
			sys_log(0, "can't refine %d %s", GetPlayerID(), GetName());
			return false;
		}
	}

#if defined(__SOUL_SYSTEM__)
	const TRefineTable* prt;
	if (item->GetType() == ITEM_SOUL)
		prt = CRefineManager::instance().GetRefineRecipe(SOUL_REFINE_SET);
	else
		prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());
#else
	const TRefineTable* prt = CRefineManager::instance().GetRefineRecipe(item->GetRefineSet());
#endif

	if (!prt)
		return false;

	LPITEM pkItemScroll;

	// ������ üũ
	if (m_iRefineAdditionalCell < 0)
		return false;

	pkItemScroll = GetInventoryItem(m_iRefineAdditionalCell);

	if (!pkItemScroll)
		return false;

	if (!(pkItemScroll->GetType() == ITEM_USE && pkItemScroll->GetSubType() == USE_TUNING))
		return false;

	if (pkItemScroll->GetVnum() == item->GetVnum())
		return false;

	DWORD result_vnum = item->GetRefinedVnum();
	DWORD result_fail_vnum = item->GetRefineFromVnum();

	if (result_vnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �̻� ������ �� �����ϴ�."));
		return false;
	}

	// MUSIN_SCROLL
	if (pkItemScroll->GetValue(0) == MUSIN_SCROLL)
	{
		if (item->GetRefineLevel() >= 4)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� �� �̻� ������ �� �����ϴ�."));
			return false;
		}
	}
	// END_OF_MUSIC_SCROLL

	else if (pkItemScroll->GetValue(0) == MEMO_SCROLL)
	{
		if (item->GetRefineLevel() != pkItemScroll->GetValue(1))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� ������ �� �����ϴ�."));
			return false;
		}
	}
	else if (pkItemScroll->GetValue(0) == BDRAGON_SCROLL)
	{
		if (item->GetType() != ITEM_METIN || item->GetRefineLevel() != 4)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� ���������� ������ �� �����ϴ�."));
			return false;
		}
	}

	TItemTable* pProto = ITEM_MANAGER::instance().GetTable(item->GetRefinedVnum());

	if (!pProto)
	{
		sys_err("DoRefineWithScroll NOT GET ITEM PROTO %d", item->GetRefinedVnum());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� ������ �� �����ϴ�."));
		return false;
	}

	// Check level limit in korea only
	if (!g_iUseLocale)
	{
		for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
		{
			long limit = pProto->aLimits[i].lValue;

			switch (pProto->aLimits[i].bType)
			{
			case LIMIT_LEVEL:
				if (GetLevel() < limit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �� �������� ���� ���Ѻ��� ������ �����ϴ�."));
					return false;
				}
				break;
			}
		}
	}

#if defined(__SOUL_SYSTEM__)
	long long llCost = 0;
	if (pkItemScroll->GetValue(0) == SOUL_EVOLVE_SCROLL)
		llCost = SOUL_REFINE_COST;
	else if (pkItemScroll->GetValue(0) == SOUL_AWAKE_SCROLL)
		llCost = SOUL_REFINE_COST;
	else
		llCost = prt->cost;
#else
	long long llCost = prt->cost;
#endif

	if (GetGold() < llCost)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ϱ� ���� ���� �����մϴ�."));
		return false;
	}

	for (int i = 0; i < prt->material_count; ++i)
	{
		if (CountSpecifyItem(prt->materials[i].vnum, item->GetCell()) < prt->materials[i].count)
		{
			if (test_server)
			{
				ChatPacket(CHAT_TYPE_INFO, "Find %d, count %d, require %d", prt->materials[i].vnum, CountSpecifyItem(prt->materials[i].vnum), prt->materials[i].count);
			}

			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ϱ� ���� ��ᰡ �����մϴ�."));
			return false;
		}
	}

	for (int i = 0; i < prt->material_count; ++i)
		RemoveSpecifyItem(prt->materials[i].vnum, prt->materials[i].count, item->GetCell());

	int prob = number(1, 100);
	int success_prob = prt->prob;
	bool bDestroyWhenFail = false;

	const char* szRefineType = "SCROLL";

	if (pkItemScroll->GetValue(0) == HYUNIRON_CHN ||
		pkItemScroll->GetValue(0) == YONGSIN_SCROLL ||
		pkItemScroll->GetValue(0) == YAGONG_SCROLL
#if defined(__STONE_OF_BLESS__)
		|| pkItemScroll->GetValue(0) == STONE_OF_BLESS
#endif
		) // ��ö, ����� �ູ��, �߰��� ������ ó��
	{
		const char hyuniron_prob[9] = { 100, 75, 65, 55, 45, 40, 35, 25, 20 };
		const char hyuniron_prob_euckr[9] = { 100, 75, 65, 55, 45, 40, 35, 30, 25 };

		const char yagong_prob[9] = { 100, 100, 90, 80, 70, 60, 50, 30, 20 };
		const char yagong_prob_euckr[9] = { 100, 100, 90, 80, 70, 60, 50, 40, 30 };

#if defined(__STONE_OF_BLESS__)
		const char stoneofbless[9] = { 100, 100, 95, 90, 85, 80, 80, 60, 40 };
#endif

		if (pkItemScroll->GetValue(0) == YONGSIN_SCROLL)
		{
			if (LC_IsYMIR() == true || LC_IsKorea() == true)
				success_prob = hyuniron_prob_euckr[MINMAX(0, item->GetRefineLevel(), 8)];
			else
				success_prob = hyuniron_prob[MINMAX(0, item->GetRefineLevel(), 8)];
		}
		else if (pkItemScroll->GetValue(0) == YAGONG_SCROLL)
		{
			if (LC_IsYMIR() == true || LC_IsKorea() == true)
				success_prob = yagong_prob_euckr[MINMAX(0, item->GetRefineLevel(), 8)];
			else
				success_prob = yagong_prob[MINMAX(0, item->GetRefineLevel(), 8)];
		}
		else if (pkItemScroll->GetValue(0) == HYUNIRON_CHN) {}
#if defined(__STONE_OF_BLESS__)
		else if (pkItemScroll->GetValue(0) == STONE_OF_BLESS)
		{
			success_prob = stoneofbless[MINMAX(0, item->GetRefineLevel(), 8)];
		}
#endif
		else
		{
			sys_err("REFINE : Unknown refine scroll item. Value0: %d", pkItemScroll->GetValue(0));
		}

		if (test_server)
			ChatPacket(CHAT_TYPE_INFO, "[Only Test] Success_Prob %d, RefineLevel %d ", success_prob, item->GetRefineLevel());

		if (pkItemScroll->GetValue(0) == HYUNIRON_CHN) // ��ö�� �������� �μ����� �Ѵ�.
			bDestroyWhenFail = true;

		// DETAIL_REFINE_LOG
		if (pkItemScroll->GetValue(0) == HYUNIRON_CHN)
		{
			szRefineType = "HYUNIRON";
		}
		else if (pkItemScroll->GetValue(0) == YONGSIN_SCROLL)
		{
			szRefineType = "GOD_SCROLL";
		}
		else if (pkItemScroll->GetValue(0) == YAGONG_SCROLL)
		{
			szRefineType = "YAGONG_SCROLL";
		}
#if defined(__STONE_OF_BLESS__)
		else if (pkItemScroll->GetValue(0) == STONE_OF_BLESS)
		{
			szRefineType = "STONE_OF_BLESS";
		}
#endif
		// END_OF_DETAIL_REFINE_LOG
	}

	// DETAIL_REFINE_LOG
	if (pkItemScroll->GetValue(0) == MUSIN_SCROLL) // ������ �ູ���� 100% ���� (+4������)
	{
		success_prob = 100;
		szRefineType = "MUSIN_SCROLL";
	}
	// END_OF_DETAIL_REFINE_LOG
	else if (pkItemScroll->GetValue(0) == MEMO_SCROLL)
	{
		success_prob = 100;
		szRefineType = "MEMO_SCROLL";
	}
	else if (pkItemScroll->GetValue(0) == BDRAGON_SCROLL)
	{
		success_prob = 80;
		szRefineType = "BDRAGON_SCROLL";
	}
#if defined(__SOUL_SYSTEM__)
	else if (pkItemScroll->GetValue(0) == SOUL_EVOLVE_SCROLL)
	{
		success_prob = SOUL_REFINE_EVOLVE_PROB;
		szRefineType = "SOUL_EVOLVE_SCROLL";
	}
	else if (pkItemScroll->GetValue(0) == SOUL_AWAKE_SCROLL)
	{
		success_prob = SOUL_REFINE_AWAKE_PROB;
		szRefineType = "SOUL_AWAKE_SCROLL";
	}
#endif

	pkItemScroll->SetCount(pkItemScroll->GetCount() - 1);

	if (prob <= success_prob)
	{
		// ����! ��� �������� �������, ���� �Ӽ��� �ٸ� ������ ȹ��
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_vnum, 1, 0, false);

		if (pkNewItem)
		{
#if defined(__SOUL_SYSTEM__)
			if (item->GetType() != ITEM_SOUL)
				ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
#else
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
#endif
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE SUCCESS", pkNewItem->GetName());

			UINT bCell = item->GetCell();

			NotifyRefineSuccess(this, item, szRefineType);
			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -prt->cost);
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE SUCCESS)");

			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);
			pkNewItem->AttrLog();

			//PointChange(POINT_GOLD, -prt->cost);
			PayRefineFee(prt->cost);
		}
		else
		{
			// ������ ������ ���� -> ���� ���з� ����
			sys_err("cannot create item %u", result_vnum);
#if defined(__REFINE_FAIL_TYPE__)
			NotifyRefineFailType(this, item, REFINE_FAIL_KEEP_GRADE, szRefineType);
#else
			NotifyRefineFail(this, item, szRefineType);
#endif
		}
	}
	else if (!bDestroyWhenFail && result_fail_vnum)
	{
		// ����! ��� �������� �������, ���� �Ӽ��� ���� ����� ������ ȹ��
		LPITEM pkNewItem = ITEM_MANAGER::instance().CreateItem(result_fail_vnum, 1, 0, false);

		if (pkNewItem)
		{
#if defined(__SOUL_SYSTEM__)
			if (item->GetType() != ITEM_SOUL)
				ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
#else
			ITEM_MANAGER::CopyAllAttrTo(item, pkNewItem);
#endif
			LogManager::instance().ItemLog(this, pkNewItem, "REFINE FAIL", pkNewItem->GetName());

			UINT bCell = item->GetCell();

			DBManager::instance().SendMoneyLog(MONEY_LOG_REFINE, item->GetVnum(), -prt->cost);
#if defined(__REFINE_FAIL_TYPE__)
			NotifyRefineFailType(this, item, REFINE_FAIL_GRADE_DOWN, szRefineType, -1);
#else
			NotifyRefineFail(this, item, szRefineType, -1);
#endif
			ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (REFINE FAIL)");

			pkNewItem->AddToCharacter(this, TItemPos(INVENTORY, bCell));
			ITEM_MANAGER::instance().FlushDelayedSave(pkNewItem);

			pkNewItem->AttrLog();

			//PointChange(POINT_GOLD, -prt->cost);
			PayRefineFee(prt->cost);
		}
		else
		{
			// ������ ������ ���� -> ���� ���з� ����
			sys_err("cannot create item %u", result_fail_vnum);
#if defined(__REFINE_FAIL_TYPE__)
			NotifyRefineFailType(this, item, REFINE_FAIL_KEEP_GRADE, szRefineType);
#else
			NotifyRefineFail(this, item, szRefineType);
#endif
		}
	}
	else
	{
#if defined(__REFINE_FAIL_TYPE__)
		NotifyRefineFailType(this, item, REFINE_FAIL_KEEP_GRADE, szRefineType);
#else
		NotifyRefineFail(this, item, szRefineType); // ������ ������ ������� ����
#endif
		PayRefineFee(prt->cost);
	}

	return true;
}

bool CHARACTER::RefineInformation(BYTE bCell, BYTE bType, int iAdditionalCell)
//bool CHARACTER::RefineInformation(UINT bCell, BYTE bType, int iAdditionalCell)
{
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	if (bCell > INVENTORY_MAX_NUM + SPECIAL_INVENTORY_MAX_PAGE_NUM)
		return false;
#else
	if (bCell > INVENTORY_MAX_NUM)
		return false;
#endif

	LPITEM item = GetInventoryItem(bCell);

	if (!item)
		return false;

	// REFINE_COST
	if (bType == REFINE_TYPE_MONEY_ONLY && !GetQuestFlag("deviltower_zone.can_refine"))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� Ÿ�� �Ϸ� ������ �ѹ����� ��밡���մϴ�."));
		return false;
	}
	// END_OF_REFINE_COST

	TPacketGCRefineInformation p;

	p.header = HEADER_GC_REFINE_INFORMATION;
	p.pos = bCell;
	p.src_vnum = item->GetVnum();
	p.result_vnum = item->GetRefinedVnum();
	p.type = bType;
#if defined(__ITEM_APPLY_RANDOM__)
	thecore_memcpy(&p.aApplyRandom, item->GetNextRandomApplies(), sizeof(p.aApplyRandom));
#endif

	if (p.result_vnum == 0)
	{
		sys_err("RefineInformation p.result_vnum == 0");
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� ������ �� �����ϴ�."));
		return false;
	}

	if (item->GetType() == ITEM_USE && item->GetSubType() == USE_TUNING)
	{
		if (bType == 0)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� �� ������δ� ������ �� �����ϴ�."));
			return false;
		}
		else
		{
			LPITEM itemScroll = GetInventoryItem(iAdditionalCell);
			if (!itemScroll || item->GetVnum() == itemScroll->GetVnum())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �������� ��ĥ ���� �����ϴ�."));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ູ�� ���� ��ö�� ��ĥ �� �ֽ��ϴ�."));
				return false;
			}
		}
	}

	CRefineManager& rm = CRefineManager::instance();

#if defined(__SOUL_SYSTEM__)
	const TRefineTable* prt;
	if (item->GetType() == ITEM_SOUL)
		prt = rm.GetRefineRecipe(SOUL_REFINE_SET);
	else
		prt = rm.GetRefineRecipe(item->GetRefineSet());
#else
	const TRefineTable* prt = rm.GetRefineRecipe(item->GetRefineSet());
#endif

	if (!prt)
	{
		sys_err("RefineInformation NOT GET REFINE SET %d", item->GetRefineSet());
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� ������ �� �����ϴ�."));
		return false;
	}

	// REFINE_COST

	// MAIN_QUEST_LV7
	if (GetQuestFlag("main_quest_lv7.refine_chance") > 0)
	{
		// �Ϻ��� ����
		if (!item->CheckItemUseLevel(20) || item->GetType() != ITEM_WEAPON)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���� ��ȸ�� 20 ������ ���⸸ �����մϴ�"));
			return false;
		}
		p.cost = 0;
	}
	else
		p.cost = ComputeRefineFee(prt->cost);
	// END_MAIN_QUEST_LV7

	p.prob = prt->prob;

	if (bType == REFINE_TYPE_MONEY_ONLY)
	{
		p.material_count = 0;
		memset(p.materials, 0, sizeof(p.materials));
	}
#if defined(__SOUL_SYSTEM__)
	else if (bType == REFINE_TYPE_SOUL_EVOLVE)
	{
		p.cost = SOUL_REFINE_COST;
		p.prob = SOUL_REFINE_EVOLVE_PROB;
		p.material_count = 0;
		memset(p.materials, 0, sizeof(p.materials));
	}
	else if (bType == REFINE_TYPE_SOUL_AWAKE)
	{
		p.cost = SOUL_REFINE_COST;
		p.prob = SOUL_REFINE_AWAKE_PROB;
		p.material_count = 0;
		memset(p.materials, 0, sizeof(p.materials));
	}
#endif
	else
	{
		p.material_count = prt->material_count;
		thecore_memcpy(&p.materials, prt->materials, sizeof(prt->materials));
	}
	// END_OF_REFINE_COST

	GetDesc()->Packet(&p, sizeof(TPacketGCRefineInformation));

	SetRefineMode(iAdditionalCell);
	return true;
}

bool CHARACTER::RefineItem(LPITEM pkItem, LPITEM pkTarget)
{
	if (!CanHandleItem())
		return false;

	if (pkItem->GetSubType() == USE_TUNING)
	{
		// XXX ����, ���� �������� ��������ϴ�...
		// XXX ���ɰ������� �ູ�� ���� �Ǿ���!
		// MUSIN_SCROLL
		if (pkItem->GetValue(0) == MUSIN_SCROLL)
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_MUSIN, pkItem->GetCell());
		// END_OF_MUSIN_SCROLL
		else if (pkItem->GetValue(0) == HYUNIRON_CHN)
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_HYUNIRON, pkItem->GetCell());
		else if (pkItem->GetValue(0) == BDRAGON_SCROLL)
		{
			if (pkTarget->GetRefineSet() != 702) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_BDRAGON, pkItem->GetCell());
		}
#if defined(__STONE_OF_BLESS__)
		else if (pkItem->GetValue(0) == STONE_OF_BLESS)
		{
			if (pkTarget->GetLevelLimit() <= 80) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_STONE_OF_BLESS, pkItem->GetCell());
		}
#endif
#if defined(__SOUL_SYSTEM__)
		else if (pkItem->GetValue(0) == SOUL_EVOLVE_SCROLL)
		{
			if (pkTarget->GetType() != ITEM_SOUL) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SOUL_EVOLVE, pkItem->GetCell());
		}
		else if (pkItem->GetValue(0) == SOUL_AWAKE_SCROLL)
		{
			if (pkTarget->GetType() != ITEM_SOUL) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SOUL_AWAKE, pkItem->GetCell());
		}
#endif
		else
		{
			if (pkTarget->GetRefineSet() == 501) return false;
			RefineInformation(pkTarget->GetCell(), REFINE_TYPE_SCROLL, pkItem->GetCell());
		}
	}
	else if (pkItem->GetSubType() == USE_DETACHMENT && IS_SET(pkTarget->GetFlag(), ITEM_FLAG_REFINEABLE))
	{
		LogManager::instance().ItemLog(this, pkTarget, "USE_DETACHMENT", pkTarget->GetName());

		bool bHasMetinStone = false;

		for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
		{
			long socket = pkTarget->GetSocket(i);
			if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
			{
				bHasMetinStone = true;
				break;
			}
		}

		if (bHasMetinStone)
		{
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			{
				long socket = pkTarget->GetSocket(i);
				if (socket > 2 && socket != ITEM_BROKEN_METIN_VNUM)
				{
					AutoGiveItem(socket);
					//TItemTable* pTable = ITEM_MANAGER::instance().GetTable(pkTarget->GetSocket(i));
					//pkTarget->SetSocket(i, pTable->alValues[2]);
					// �������� ��ü���ش�
					pkTarget->SetSocket(i, ITEM_BROKEN_METIN_VNUM);
				}
			}
			pkItem->SetCount(pkItem->GetCount() - 1);
			return true;
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �� �ִ� ��ƾ���� �����ϴ�."));
			return false;
		}
	}

	return false;
}

EVENTFUNC(kill_campfire_event)
{
	char_event_info* info = dynamic_cast<char_event_info*>(event->info);

	if (info == NULL)
	{
		sys_err("kill_campfire_event> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER ch = info->ch;

	if (ch == NULL) // <Factor>
		return 0;

	ch->m_pkMiningEvent = NULL;
	M2_DESTROY_CHARACTER(ch);
	return 0;
}

bool CHARACTER::GiveRecallItem(LPITEM item)
{
	int idx = GetMapIndex();
	int iEmpireByMapIndex = -1;

	if (idx < 20)
		iEmpireByMapIndex = 1;
	else if (idx < 40)
		iEmpireByMapIndex = 2;
	else if (idx < 60)
		iEmpireByMapIndex = 3;
	else if (idx < 10000)
		iEmpireByMapIndex = 0;

	switch (idx)
	{
	case 66:
	case 216:
		iEmpireByMapIndex = -1;
		break;
	}

	if (iEmpireByMapIndex && GetEmpire() != iEmpireByMapIndex)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����� �� �� ���� ��ġ �Դϴ�."));
		return false;
	}

	int pos;

	if (item->GetCount() == 1) // �������� �ϳ���� �׳� ����.
	{
		item->SetSocket(0, GetX());
		item->SetSocket(1, GetY());
	}
	else if ((pos = GetEmptyInventory(item->GetSize())) != -1) // �׷��� �ʴٸ� �ٸ� �κ��丮 ������ ã�´�.
	{
		LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), 1);

		if (NULL != item2)
		{
			item2->SetSocket(0, GetX());
			item2->SetSocket(1, GetY());
			item2->AddToCharacter(this, TItemPos(INVENTORY, pos));

			item->SetCount(item->GetCount() - 1);
		}
	}
	else
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
		return false;
	}

	return true;
}

void CHARACTER::ProcessRecallItem(LPITEM item)
{
	int idx;

	if ((idx = SECTREE_MANAGER::instance().GetMapIndex(item->GetSocket(0), item->GetSocket(1))) == 0)
		return;

	int iEmpireByMapIndex = -1;

	if (idx < 20)
		iEmpireByMapIndex = 1;
	else if (idx < 40)
		iEmpireByMapIndex = 2;
	else if (idx < 60)
		iEmpireByMapIndex = 3;
	else if (idx < 10000)
		iEmpireByMapIndex = 0;

	switch (idx)
	{
	case 66:
	case 216:
		iEmpireByMapIndex = -1;
		break;
		// �Ƿ決�� �϶�
	case 301:
	case 302:
	case 303:
	case 304:
		if (GetLevel() < 90)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�������� ���� ���Ѻ��� ������ �����ϴ�."));
			return;
		}
		else
			break;
	}

	if (iEmpireByMapIndex && GetEmpire() != iEmpireByMapIndex)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ��ġ�� Ÿ������ ���� �־ ��ȯ�� �� �����ϴ�."));
		item->SetSocket(0, 0);
		item->SetSocket(1, 0);
	}
	else
	{
		sys_log(1, "Recall: %s %d %d -> %d %d", GetName(), GetX(), GetY(), item->GetSocket(0), item->GetSocket(1));
		WarpSet(item->GetSocket(0), item->GetSocket(1));
		item->SetCount(item->GetCount() - 1);
	}
}

void CHARACTER::__OpenPrivateShop()
{
	unsigned bodyPart = GetPart(PART_MAIN);
	switch (bodyPart)
	{
	case 0:
	case 1:
	case 2:
#if defined(__MYSHOP_DECO__)
		ChatPacket(CHAT_TYPE_COMMAND, "MyPrivShopOpen %d %d", 0, 1);
#else
		ChatPacket(CHAT_TYPE_COMMAND, "OpenPrivateShop");
#endif
		break;
	default:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����� ���� ������ �� �� �ֽ��ϴ�."));
		break;
	}
}

// MYSHOP_PRICE_LIST
#if defined(__CHEQUE_SYSTEM__)
void CHARACTER::SendMyShopPriceListCmd(DWORD dwItemVnum, DWORD dwItemPrice, DWORD dwItemPriceCheque)
#else
void CHARACTER::SendMyShopPriceListCmd(DWORD dwItemVnum, DWORD dwItemPrice)
#endif
{
	char szLine[256];
#if defined(__CHEQUE_SYSTEM__)
	snprintf(szLine, sizeof(szLine), "MyShopPriceList %u %u %u", dwItemVnum, dwItemPrice, dwItemPriceCheque);
#else
	snprintf(szLine, sizeof(szLine), "MyShopPriceList %u %u", dwItemVnum, dwItemPrice);
#endif
	ChatPacket(CHAT_TYPE_COMMAND, szLine);
	sys_log(0, szLine);
}

//
// DB ĳ�÷� ���� ���� ����Ʈ�� User ���� �����ϰ� ������ ����� Ŀ�ǵ带 ������.
//
void CHARACTER::UseSilkBotaryReal(const TPacketMyshopPricelistHeader* p)
{
	const TItemPriceInfo* pInfo = (const TItemPriceInfo*)(p + 1);

#if defined(__CHEQUE_SYSTEM__)
	if (!p->byCount)
		SendMyShopPriceListCmd(1, 0, 0);
	else
	{
		for (int idx = 0; idx < p->byCount; idx++)
			SendMyShopPriceListCmd(pInfo[idx].dwVnum, pInfo[idx].dwPrice, pInfo[idx].dwPriceCheque);
	}
#else
	if (!p->byCount)
		// ���� ����Ʈ�� ����. dummy �����͸� ���� Ŀ�ǵ带 �����ش�.
		SendMyShopPriceListCmd(1, 0);
	else {
		for (int idx = 0; idx < p->byCount; idx++)
			SendMyShopPriceListCmd(pInfo[idx].dwVnum, pInfo[idx].dwPrice);
	}
#endif

	__OpenPrivateShop();
}

//
// �̹� ���� �� ó�� ������ Open �ϴ� ��� ����Ʈ�� Load �ϱ� ���� DB ĳ�ÿ� �������� ����Ʈ ��û ��Ŷ�� ������.
// ���ĺ��ʹ� �ٷ� ������ ����� ������ ������.
//
void CHARACTER::UseSilkBotary(void)
{
	if (m_bNoOpenedShop)
	{
		DWORD dwPlayerID = GetPlayerID();
		db_clientdesc->DBPacket(HEADER_GD_MYSHOP_PRICELIST_REQ, GetDesc()->GetHandle(), &dwPlayerID, sizeof(DWORD));
		m_bNoOpenedShop = false;
	}
	else
	{
		__OpenPrivateShop();
	}
}

#if defined(__MYSHOP_DECO__)
void CHARACTER::__OpenMyShopDeco()
{
	unsigned bodyPart = GetPart(PART_MAIN);
	switch (bodyPart)
	{
	case 0:
	case 1:
	case 2:
		ChatPacket(CHAT_TYPE_COMMAND, "OpenMyShopDecoWnd");
		break;
	default:
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����� ���� ������ �� �� �ֽ��ϴ�."));
		break;
	}
}

void CHARACTER::UseDecoBundle(void)
{
	if (m_bNoOpenedShop)
	{
		DWORD dwPlayerID = GetPlayerID();
		db_clientdesc->DBPacket(HEADER_GD_MYSHOP_PRICELIST_REQ, GetDesc()->GetHandle(), &dwPlayerID, sizeof(DWORD));
		m_bNoOpenedShop = false;
	}
	else
	{
		__OpenMyShopDeco();
	}
}
#endif
// END_OF_MYSHOP_PRICE_LIST

#if defined(__EXTENDED_BLEND_AFFECT__)
bool CHARACTER::UseExtendedBlendAffect(LPITEM item, int affect_type, int apply_type, int apply_value, int apply_duration)
{
	apply_duration = apply_duration <= 0 ? INFINITE_AFFECT_DURATION : apply_duration;
	bool bStatus = item->GetSocket(3);

	if (FindAffect(affect_type, apply_type))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
		return false;
	}

	if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, apply_type) || FindAffect(AFFECT_MALL, apply_type))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
		return false;
	}

	switch (item->GetVnum())
	{
		// DEWS
	case 50821:
	case 950821:
	{
		if (FindAffect(AFFECT_BLEND_POTION_1))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_BLEND_POTION_1);
			return false;
		}
		AddAffect(AFFECT_BLEND_POTION_1, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 50822:
	case 950822:
	{
		if (FindAffect(AFFECT_BLEND_POTION_2))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_BLEND_POTION_2);
			return false;
		}
		AddAffect(AFFECT_BLEND_POTION_2, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 50823:
	case 950823:
	{
		if (FindAffect(AFFECT_BLEND_POTION_3))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_BLEND_POTION_3);
			return false;
		}
		AddAffect(AFFECT_BLEND_POTION_3, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 50824:
	case 950824:
	{
		if (FindAffect(AFFECT_BLEND_POTION_4))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_BLEND_POTION_4);
			return false;
		}
		if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, POINT_RESIST_MAGIC))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
			return false;
		}

		AddAffect(AFFECT_BLEND_POTION_4, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 50825:
	case 950825:
	{
		if (FindAffect(AFFECT_BLEND_POTION_5))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_BLEND_POTION_5);
			return false;
		}
		AddAffect(AFFECT_BLEND_POTION_5, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 50826:
	case 950826:
	{
		if (FindAffect(AFFECT_BLEND_POTION_6))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_BLEND_POTION_6);
			return false;
		}
		AddAffect(AFFECT_BLEND_POTION_6, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	// END_OF_DEWS

	// ENERGY_CRISTAL
	case 51002:
	case 951002:
	{
		if (FindAffect(AFFECT_ENERGY))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_ENERGY);
			return false;
		}
		AddAffect(AFFECT_ENERGY, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	// END_OF_ENERGY_CRISTAL

	// DRAGON_GOD_MEDALS
	case 939017:
	{
		if (FindAffect(AFFECT_DRAGON_GOD_1))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_DRAGON_GOD_1);
			return false;
		}

		AddAffect(AFFECT_DRAGON_GOD_1, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 939018:
	{
		if (FindAffect(AFFECT_DRAGON_GOD_2))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_DRAGON_GOD_2);
			return false;
		}
		AddAffect(AFFECT_DRAGON_GOD_2, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 939019:
	{
		if (FindAffect(AFFECT_DRAGON_GOD_3))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_DRAGON_GOD_3);
			return false;
		}
		AddAffect(AFFECT_DRAGON_GOD_3, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 939020:
	{
		if (FindAffect(AFFECT_DRAGON_GOD_4))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_DRAGON_GOD_4);
			return false;
		}
		AddAffect(AFFECT_DRAGON_GOD_4, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	// END_OF_DRAGON_GOD_MEDALS

	// CRITICAL_AND_PENETRATION
	case 939024:
	{
		if (FindAffect(AFFECT_CRITICAL))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_CRITICAL);
			return false;
		}
		AddAffect(AFFECT_CRITICAL, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 939025:
	{
		if (FindAffect(AFFECT_PENETRATE))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_PENETRATE);
			return false;
		}
		AddAffect(AFFECT_PENETRATE, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	// END_OF_CRITICAL_AND_PENETRATION

	// ATTACK_AND_MOVE_SPEED
	case 927209:
	{
		if (FindAffect(AFFECT_ATTACK_SPEED))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_ATTACK_SPEED);
			return false;
		}

		if (FindAffect(AFFECT_ATT_SPEED))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
			return false;
		}

		AddAffect(AFFECT_ATTACK_SPEED, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	case 927212:
	{
		if (FindAffect(AFFECT_MOVE_SPEED))
		{
			if (!bStatus)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				return false;
			}
			RemoveAffect(AFFECT_MOVE_SPEED);
			return false;
		}

		if (FindAffect(AFFECT_MOV_SPEED))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
			return false;
		}

		AddAffect(AFFECT_MOVE_SPEED, apply_type, apply_value, 0, apply_duration, 0, true);
	}
	break;
	// END_OF_ATTACK_AND_MOVE_SPEED
	}

	return true;
}
#endif

#if defined(__BLEND_AFFECT__)
bool CHARACTER::SetBlendAffect(LPITEM item)
{
	switch (item->GetVnum())
	{
		// DEWS
	case 50821:
	case 950821:
		AddAffect(AFFECT_BLEND_POTION_1, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;

	case 50822:
	case 950822:
		AddAffect(AFFECT_BLEND_POTION_2, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;

	case 50823:
	case 950823:
		AddAffect(AFFECT_BLEND_POTION_3, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;

	case 50824:
	case 950824:
		AddAffect(AFFECT_BLEND_POTION_4, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;

	case 50825:
	case 950825:
		AddAffect(AFFECT_BLEND_POTION_3, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;

	case 50826:
	case 950826:
		AddAffect(AFFECT_BLEND_POTION_6, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
		// END_OF_DEWS

		// ENERGY_CRISTAL
	case 51002:
	case 951002:
		AddAffect(AFFECT_ENERGY, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
		// END_OF_ENERGY_CRISTAL

		// DRAGON_GOD_MEDALS
	case 939017:
		AddAffect(AFFECT_DRAGON_GOD_1, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
	case 939018:
		AddAffect(AFFECT_DRAGON_GOD_2, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
	case 939019:
		AddAffect(AFFECT_DRAGON_GOD_3, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
	case 939020:
		AddAffect(AFFECT_DRAGON_GOD_4, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
		// END_OF_DRAGON_GOD_MEDALS

		// CRITICAL_AND_PENETRATION
	case 939024:
		AddAffect(AFFECT_CRITICAL, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
	case 939025:
		AddAffect(AFFECT_PENETRATE, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
		// END_OF_CRITICAL_AND_PENETRATION

		// ATTACK_AND_MOVE_SPEED
	case 927209:
		AddAffect(AFFECT_ATTACK_SPEED, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
	case 927212:
		AddAffect(AFFECT_MOVE_SPEED, APPLY_NONE, 0, AFF_NONE, item->GetSocket(2), 0, false, false);
		break;
		// END_OF_ATTACK_AND_MOVE_SPEED

	default:
		return false;
	}

	return true;
}
#endif

int CalculateConsume(LPCHARACTER ch)
{
	static const int WARP_NEED_LIFE_PERCENT = 30;
	static const int WARP_MIN_LIFE_PERCENT = 10;
	// CONSUME_LIFE_WHEN_USE_WARP_ITEM
	int consumeLife = 0;
	{
		// CheckNeedLifeForWarp
		const int curLife = ch->GetHP();
		const int needPercent = WARP_NEED_LIFE_PERCENT;
		const int needLife = ch->GetMaxHP() * needPercent / 100;
		if (curLife < needLife)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ����� ���� ���ڶ� ����� �� �����ϴ�."));
			return -1;
		}

		consumeLife = needLife;

		// CheckMinLifeForWarp: ���� ���ؼ� ������ �ȵǹǷ� ����� �ּҷ��� �����ش�
		const int minPercent = WARP_MIN_LIFE_PERCENT;
		const int minLife = ch->GetMaxHP() * minPercent / 100;
		if (curLife - needLife < minLife)
			consumeLife = curLife - minLife;

		if (consumeLife < 0)
			consumeLife = 0;
	}
	// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM
	return consumeLife;
}

int CalculateConsumeSP(LPCHARACTER lpChar)
{
	static const int NEED_WARP_SP_PERCENT = 30;

	const int curSP = lpChar->GetSP();
	const int needSP = lpChar->GetMaxSP() * NEED_WARP_SP_PERCENT / 100;

	if (curSP < needSP)
	{
		lpChar->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���ŷ� ���� ���ڶ� ����� �� �����ϴ�."));
		return -1;
	}

	return needSP;
}

bool CHARACTER::UseItemEx(LPITEM item, TItemPos DestCell)
{
	int iLimitRealtimeStartFirstUseFlagIndex = -1;
	int iLimitTimerBasedOnWearFlagIndex = -1;

	WORD wDestCell = DestCell.cell;
	BYTE bDestInven = DestCell.window_type;

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		long limitValue = item->GetProto()->aLimits[i].lValue;

		switch (item->GetProto()->aLimits[i].bType)
		{
		case LIMIT_LEVEL:
			if (GetLevel() < limitValue)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�������� ���� ���Ѻ��� ������ �����ϴ�."));
				return false;
			}
			break;

		case LIMIT_REAL_TIME_START_FIRST_USE:
			iLimitRealtimeStartFirstUseFlagIndex = i;
			break;

		case LIMIT_TIMER_BASED_ON_WEAR:
			iLimitTimerBasedOnWearFlagIndex = i;
			break;
		}
	}

	if (test_server)
	{
		sys_log(0, "USE_ITEM %s, Inven %d, Cell %d, ItemType %d, SubType %d", item->GetName(), bDestInven, wDestCell, item->GetType(), item->GetSubType());
	}

	if (CArenaManager::instance().IsLimitedItem(GetMapIndex(), item->GetVnum()) == true)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� �߿��� �̿��� �� ���� ��ǰ�Դϴ�."));
		return false;
	}

#if defined(__PET_SYSTEM__)
	if (item->IsPet() && IS_BLOCKED_PET_SUMMON_MAP(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot summon your mount/pet right now."));
		return false;
	}

	if (item->IsPet() && IS_BLOCKED_PET_DUNGEON_MAP(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot summon your mount/pet right now."));
		return false;
	}
#endif

#if defined(__MOUNT_COSTUME_SYSTEM__)
	if (item->IsCostumeMount() && IS_BLOCKED_MOUNT_SUMMON_MAP(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot summon your mount/pet right now."));
		return false;
	}

	if (item->IsCostumeMount() && IS_BLOCKED_MOUNT_DUNGEON_MAP(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot summon your mount/pet right now."));
		return false;
	}
#endif

	// ������ ���� ��� ���ĺ��ʹ� ������� �ʾƵ� �ð��� �����Ǵ� ��� ó��.
	if (-1 != iLimitRealtimeStartFirstUseFlagIndex)
	{
		// �� ���̶� ����� ���������� ���δ� Socket1�� ���� �Ǵ��Ѵ�. (Socket1�� ���Ƚ�� ���)
		if (0 == item->GetSocket(1))
		{
			// ��밡�ɽð��� Default ������ Limit Value ���� ����ϵ�, Socket0�� ���� ������ �� ���� ����ϵ��� �Ѵ�. (������ ��)
			long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[iLimitRealtimeStartFirstUseFlagIndex].lValue;

			if (0 == duration)
				duration = 60 * 60 * 24 * 7;

			item->SetSocket(0, time(0) + duration);
			item->StartRealTimeExpireEvent();
		}

		if (false == item->IsEquipped())
			item->SetSocket(1, item->GetSocket(1) + 1);
	}

	switch (item->GetType())
	{
	case ITEM_HAIR:
		return ItemProcess_Hair(item, wDestCell);

	case ITEM_POLYMORPH:
		return ItemProcess_Polymorph(item);

	case ITEM_QUEST:
	case ITEM_PET:
		if (GetArena() != NULL || IsObserverMode() == true)
		{
			if (item->GetVnum() == 50051 || item->GetVnum() == 50052 || item->GetVnum() == 50053)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� �߿��� �̿��� �� ���� ��ǰ�Դϴ�."));
				return false;
			}
		}

		if (!IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE | ITEM_FLAG_QUEST_USE_MULTIPLE) || item->GetSubType() == PET_PAY)
		{
			if (item->GetSIGVnum() == 0)
			{
				quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
			}
			else
			{
				quest::CQuestManager::instance().SIGUse(GetPlayerID(), item->GetSIGVnum(), item, false);
			}
		}

#if defined(__MAILBOX__)
		if (item->GetVnum() == MOBILE_MAILBOX)
			CMailBox::Open(this);
#endif
		break;

	case ITEM_CAMPFIRE:
	{
		float fx, fy;
		GetDeltaByDegree(GetRotation(), 100.0f, &fx, &fy);

		LPSECTREE tree = SECTREE_MANAGER::instance().Get(GetMapIndex(), (long)(GetX() + fx), (long)(GetY() + fy));

		if (!tree)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ں��� �ǿ� �� ���� �����Դϴ�."));
			return false;
		}

		if (tree->IsAttr((long)(GetX() + fx), (long)(GetY() + fy), ATTR_WATER))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �ӿ� ��ں��� �ǿ� �� �����ϴ�."));
			return false;
		}

		LPCHARACTER campfire = CHARACTER_MANAGER::instance().SpawnMob(fishing::CAMPFIRE_MOB, GetMapIndex(), (long)(GetX() + fx), (long)(GetY() + fy), 0, false, number(0, 359));

		char_event_info* info = AllocEventInfo<char_event_info>();

		info->ch = campfire;

		campfire->m_pkMiningEvent = event_create(kill_campfire_event, info, PASSES_PER_SEC(40));

		item->SetCount(item->GetCount() - 1);
	}
	break;

	case ITEM_UNIQUE:
	{
		switch (item->GetSubType())
		{
		case USE_ABILITY_UP:
		{
			switch (item->GetValue(0))
			{
			case APPLY_MOV_SPEED:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_MOV_SPEED, item->GetValue(2), AFF_MOV_SPEED_POTION, item->GetValue(1), 0, true, true);
				break;

			case APPLY_ATT_SPEED:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ATT_SPEED, item->GetValue(2), AFF_ATT_SPEED_POTION, item->GetValue(1), 0, true, true);
				break;

			case APPLY_STR:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ST, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_DEX:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_DX, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_CON:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_HT, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_INT:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_IQ, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_CAST_SPEED:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_CASTING_SPEED, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_RESIST_MAGIC:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_RESIST_MAGIC, item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_ATT_GRADE_BONUS:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_ATT_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;

			case APPLY_DEF_GRADE_BONUS:
				AddAffect(AFFECT_UNIQUE_ABILITY, POINT_DEF_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true, true);
				break;
			}
		}

		if (GetDungeon())
			GetDungeon()->UsePotion(this);

		if (GetWarMap())
			GetWarMap()->UsePotion(this, item);

		item->SetCount(item->GetCount() - 1);
		break;

		default:
		{
			if (item->GetSubType() == USE_SPECIAL)
			{
				sys_log(0, "ITEM_UNIQUE: USE_SPECIAL %u", item->GetVnum());

				switch (item->GetVnum())
				{
#if defined(__MYSHOP_DECO__)
				case KASHMIR_BUNDLE:
				{
					if (IS_BOTARYABLE_ZONE(GetMapIndex()) == true)
						__OpenMyShopDeco();
					else
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ �� �� ���� �����Դϴ�"));
				}
				break;
#endif

				case 71049: // ��ܺ�����
					if (LC_IsYMIR() == true || LC_IsKorea() == true)
					{
						if (IS_BOTARYABLE_ZONE(GetMapIndex()) == true)
						{
							UseSilkBotary();
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ �� �� ���� �����Դϴ�"));
						}
					}
					else
					{
						UseSilkBotary();
					}
					break;
				}
			}
			else
			{
				if (!item->IsEquipped())
					EquipItem(item);
				else
					UnequipItem(item);
			}
		}
		break;
		}
	}
	break;

	case ITEM_COSTUME:
	case ITEM_WEAPON:
	case ITEM_ARMOR:
	case ITEM_ROD:
	case ITEM_RING: // �ű� ���� ������
	case ITEM_BELT: // �ű� ��Ʈ ������
		// MINING
	case ITEM_PICK:
		// END_OF_MINING
		if (!item->IsEquipped())
			EquipItem(item);
		else
			UnequipItem(item);
		break;
		// �������� ���� ��ȥ���� ����� �� ����.
		// �������� Ŭ����, ��ȥ���� ���Ͽ� item use ��Ŷ�� ���� �� ����.
		// ��ȥ�� ������ item move ��Ŷ���� �Ѵ�.
		// ������ ��ȥ���� �����Ѵ�.
	case ITEM_DS:
	{
		if (!item->IsEquipped())
			return false;
		return DSManager::instance().PullOut(this, NPOS, item);
		break;
	}
	case ITEM_SPECIAL_DS:
		if (!item->IsEquipped())
			EquipItem(item);
		else
			UnequipItem(item);
		break;

	case ITEM_FISH:
	{
		if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� �߿��� �̿��� �� ���� ��ǰ�Դϴ�."));
			return false;
		}

		if (item->GetSubType() == FISH_ALIVE)
			fishing::UseFish(this, item);
	}
	break;

	case ITEM_TREASURE_BOX:
	{
		return false;
		//ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("����� ��� �־ ������ �ʴ°� ����. ���踦 ���غ���."));
	}
	break;

	case ITEM_TREASURE_KEY:
	{
		LPITEM item2;

		if (!GetItem(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging())
			return false;

		if (item2->GetType() != ITEM_TREASURE_BOX)
		{
			ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("����� ���� ������ �ƴѰ� ����."));
			return false;
		}

		if (item->GetValue(0) == item2->GetValue(0))
		{
			//ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("����� ������ ������ �ִ� �κ� ������ �ȵǾ����ϴ�."));

			if (GiveItemFromSpecialItemGroup(item2->GetVnum()))
			{
				ITEM_MANAGER::instance().RemoveItem(item);
				ITEM_MANAGER::instance().RemoveItem(item2);
			}
			else
			{
				ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("���谡 ���� �ʴ� �� ����."));
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("���谡 ���� �ʴ� �� ����."));
			return false;
		}
	}
	break;

	case ITEM_GIFTBOX:
	{
		DWORD dwBoxVnum = item->GetVnum();

		if (dwBoxVnum == 50033 && LC_IsYMIR()) // �˼����� ����
		{
			if (GetLevel() < 15)
			{
				ChatPacket(CHAT_TYPE_INFO, "15���� ���Ͽ����� ����� �� �����ϴ�.");
				return false;
			}
		}

		if ((dwBoxVnum > 51500 && dwBoxVnum < 52000) || (dwBoxVnum >= 50255 && dwBoxVnum <= 50260)) // ��ȥ������
		{
			if (!(this->DragonSoul_IsQualified()))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ��ȥ�� ����Ʈ�� �Ϸ��ϼž� �մϴ�."));
				return false;
			}
		}

		if (!GiveItemFromSpecialItemGroup(dwBoxVnum))
		{
			ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("�ƹ��͵� ���� �� �������ϴ�."));
			return false;
		}

		item->SetCount(item->GetCount() - 1);
	}
	break;

	case ITEM_SKILLFORGET:
	{
		if (!item->GetSocket(0))
		{
			ITEM_MANAGER::instance().RemoveItem(item);
			return false;
		}

		DWORD dwVnum = item->GetSocket(0);

		if (SkillLevelDown(dwVnum))
		{
			ITEM_MANAGER::instance().RemoveItem(item);
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ų ������ �����µ� �����Ͽ����ϴ�."));
		}
		else
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ų ������ ���� �� �����ϴ�."));
	}
	break;

	case ITEM_SKILLBOOK:
	{
		if (IsPolymorphed())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����߿��� å�� ������ �����ϴ�."));
			return false;
		}

		DWORD dwVnum = 0;

		if (item->GetVnum() == ITEM_SKILLBOOK_VNUM)
		{
			dwVnum = item->GetSocket(0);
		}
		else
		{
			// ���ο� ���ü��� value 0 �� ��ų ��ȣ�� �����Ƿ� �װ��� ���.
			dwVnum = item->GetValue(0);
		}

		if (0 == dwVnum)
		{
			ITEM_MANAGER::instance().RemoveItem(item);
			return false;
		}

		if (true == LearnSkillByBook(dwVnum))
		{
			item->SetCount(item->GetCount() - 1);

			int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);

			if (distribution_test_server)
				iReadDelay /= 3;

			// �ѱ� ������ ��쿡�� �ð��� 24�ð� ����
			if (LC_IsKorea())
				iReadDelay = 86400;

			SetSkillNextReadTime(dwVnum, get_global_time() + iReadDelay);
		}
	}
	break;

	case ITEM_USE:
	{
#if defined(__SOUL_BIND_SYSTEM__)
		if (item->IsSealed())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot use a soulbound item."));
			return false;
		}
#endif

#if defined(__ACCE_COSTUME_SYSTEM__)
		if (item->IsAcceReverseScroll())
		{
			LPITEM item2 = GetItem(DestCell);
			if (item2)
			{
				if (item2->IsExchanging())
					return false;

				if (item2->IsEquipped())
					return false;

				//< 2020.12.25.Owsap
				// NOTE : Acce Reverse Scroll
				// Return here while the scroll (pointer) exists.
				if (CleanAcceAttr(item, item2))
					return true;
				//>
			}
		}
#endif

		if (item->GetVnum() > 50800 && item->GetVnum() <= 50820)
		{
			if (test_server)
				sys_log(0, "ADD addtional effect : vnum(%d) subtype(%d)", item->GetOriginalVnum(), item->GetSubType());

			int affect_type = AFFECT_EXP_BONUS_EURO_FREE;
			int apply_type = aApplyInfo[item->GetValue(0)].bPointType;
			int apply_value = item->GetValue(2);
			int apply_duration = item->GetValue(1);

			switch (item->GetSubType())
			{
			case USE_ABILITY_UP:
				if (FindAffect(affect_type, apply_type))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
					return false;
				}

				{
					switch (item->GetValue(0))
					{
					case APPLY_MOV_SPEED:
					{
#if defined(__EXTENDED_BLEND_AFFECT__)
						if (FindAffect(AFFECT_MOVE_SPEED))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}
#endif
						AddAffect(affect_type, apply_type, apply_value, AFF_MOV_SPEED_POTION, apply_duration, 0, true, true);
						break;
					}

					case APPLY_ATT_SPEED:
					{
#if defined(__EXTENDED_BLEND_AFFECT__)
						if (FindAffect(AFFECT_BLEND_POTION_3))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}

						if (FindAffect(AFFECT_ATTACK_SPEED))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}
#endif
						AddAffect(affect_type, apply_type, apply_value, AFF_ATT_SPEED_POTION, apply_duration, 0, true, true);
						break;
					}

					case APPLY_STR:
					case APPLY_DEX:
					case APPLY_CON:
					case APPLY_INT:
					case APPLY_CAST_SPEED:
					{
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
						break;
					}

					case APPLY_CRITICAL_PCT:
					{
#if defined(__EXTENDED_BLEND_AFFECT__)
						if (FindAffect(AFFECT_BLEND_POTION_1))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}

						if (FindAffect(AFFECT_CRITICAL))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}
#endif
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
						break;
					}

					case APPLY_PENETRATE_PCT:
					{
#if defined(__EXTENDED_BLEND_AFFECT__)
						if (FindAffect(AFFECT_BLEND_POTION_2))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}

						if (FindAffect(AFFECT_PENETRATE))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}
#endif
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
						break;
					}

					case APPLY_RESIST_MAGIC:
					{
#if defined(__EXTENDED_BLEND_AFFECT__)
						if (FindAffect(AFFECT_BLEND_POTION_4))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}
#endif
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
						break;
					}

					case APPLY_ATT_GRADE_BONUS:
					{
#if defined(__EXTENDED_BLEND_AFFECT__)
						if (FindAffect(AFFECT_BLEND_POTION_5))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}
#endif
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
						break;
					}

					case APPLY_DEF_GRADE_BONUS:
					{
#if defined(__EXTENDED_BLEND_AFFECT__)
						if (FindAffect(AFFECT_BLEND_POTION_6))
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
							return false;
						}
#endif
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, true, true);
						break;
					}

					}
				}

				if (GetDungeon())
					GetDungeon()->UsePotion(this);

				if (GetWarMap())
					GetWarMap()->UsePotion(this, item);

				item->SetCount(item->GetCount() - 1);
				break;

			case USE_AFFECT:
			{
#if defined(__EXTENDED_BLEND_AFFECT__)
				for (int i = AFFECT_DRAGON_GOD_1; i <= AFFECT_DRAGON_GOD_4; ++i)
				{
					if (FindAffect(i, aApplyInfo[item->GetValue(1)].bPointType))
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
						return false;
					}
				}
#endif

				if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, aApplyInfo[item->GetValue(1)].bPointType))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				}
				else
				{
					// PC_BANG_ITEM_ADD
					if (item->IsPCBangItem() == true)
					{
						// PC������ üũ�ؼ� ó��
						if (CPCBangManager::instance().IsPCBangIP(GetDesc()->GetHostName()) == false)
						{
							// PC���� �ƴ�!
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� PC�濡���� ����� �� �ֽ��ϴ�."));
							return false;
						}
					}
					// END_PC_BANG_ITEM_ADD

					AddAffect(AFFECT_EXP_BONUS_EURO_FREE, aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, item->GetValue(3), 0, false, true);
					item->SetCount(item->GetCount() - 1);
				}
			}
			break;

			case USE_POTION_NODELAY:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����忡�� ����Ͻ� �� �����ϴ�."));
						return false;
					}

					switch (item->GetVnum())
					{
					case 70020:
					case 71018:
					case 71019:
					case 71020:
						if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
						{
							if (m_nPotionLimit <= 0)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� ���ѷ��� �ʰ��Ͽ����ϴ�."));
								return false;
							}
						}
						break;

					default:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����忡�� ����Ͻ� �� �����ϴ�."));
						return false;
						break;
					}
				}

				bool used = false;

				if (item->GetValue(0) != 0) // HP ���밪 ȸ��
				{
					if (GetHP() < GetMaxHP())
					{
						PointChange(POINT_HP, item->GetValue(0) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
						EffectPacket(SE_HPUP_RED);
						used = TRUE;
					}
				}

				if (item->GetValue(1) != 0) // SP ���밪 ȸ��
				{
					if (GetSP() < GetMaxSP())
					{
						PointChange(POINT_SP, item->GetValue(1) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
						EffectPacket(SE_SPUP_BLUE);
						used = TRUE;
					}
				}

				if (item->GetValue(3) != 0) // HP % ȸ��
				{
					if (GetHP() < GetMaxHP())
					{
						PointChange(POINT_HP, item->GetValue(3) * GetMaxHP() / 100);
						EffectPacket(SE_HPUP_RED);
						used = TRUE;
					}
				}

				if (item->GetValue(4) != 0) // SP % ȸ��
				{
					if (GetSP() < GetMaxSP())
					{
						PointChange(POINT_SP, item->GetValue(4) * GetMaxSP() / 100);
						EffectPacket(SE_SPUP_BLUE);
						used = TRUE;
					}
				}

				if (used)
				{
					if (item->GetVnum() == 50085 || item->GetVnum() == 50086)
					{
						if (test_server)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �Ǵ� ���� �� ����Ͽ����ϴ�"));
						SetUseSeedOrMoonBottleTime();
					}
					if (GetDungeon())
						GetDungeon()->UsePotion(this);

					if (GetWarMap())
						GetWarMap()->UsePotion(this, item);

					m_nPotionLimit--;

					// RESTRICT_USE_SEED_OR_MOONBOTTLE
					item->SetCount(item->GetCount() - 1);
					// END_RESTRICT_USE_SEED_OR_MOONBOTTLE
				}
			}
			break;
			}

			return true;
		}

		if (item->GetVnum() >= 27863 && item->GetVnum() <= 27883)
		{
			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� �߿��� �̿��� �� ���� ��ǰ�Դϴ�."));
				return false;
			}
		}

		if (test_server)
		{
			sys_log(0, "USE_ITEM %s Type %d SubType %d vnum %d", item->GetName(), item->GetType(), item->GetSubType(), item->GetOriginalVnum());
		}

		switch (item->GetSubType())
		{
#if defined(__EXPRESSING_EMOTIONS__)
		case USE_EMOTION_PACK:
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(60, 77);
			int iSpecialActionID = dis(gen);

			DWORD dwExpireTime = 60 * 60 * 12; // 12h
			DWORD dwDuration = get_global_time() + dwExpireTime;

			char szQuestFlag[20] = "0";
			snprintf(szQuestFlag, sizeof(szQuestFlag), "emotions.expire_%d", iSpecialActionID);

			DWORD dwActionTime = GetQuestFlag(szQuestFlag);
			if (dwActionTime != 0)
				dwDuration = dwActionTime + dwExpireTime;

			SetQuestFlag(szQuestFlag, dwDuration);

			ChatPacket(CHAT_TYPE_COMMAND, "RegisterSpecialEmotion %d %d 1", iSpecialActionID, dwDuration);
			item->SetCount(item->GetCount() - 1);
		}
		break;
#endif

		case USE_TIME_CHARGE_PER:
		{
			LPITEM pDestItem = GetItem(DestCell);
			if (NULL == pDestItem)
				return false;

			// �켱 ��ȥ���� ���ؼ��� �ϵ��� �Ѵ�.
			if (pDestItem->IsDragonSoul())
			{
				int ret;
				char buf[128];
				if (item->GetVnum() == DRAGON_HEART_VNUM)
				{
					ret = pDestItem->GiveMoreTime_Per((float)item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
				}
				else
				{
					ret = pDestItem->GiveMoreTime_Per((float)item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
				}
				if (ret > 0)
				{
					if (item->GetVnum() == DRAGON_HEART_VNUM)
					{
						sprintf(buf, "Inc %ds by item{VN:%d SOC%d:%d}", ret, item->GetVnum(), ITEM_SOCKET_CHARGING_AMOUNT_IDX, item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
					}
					else
					{
						sprintf(buf, "Inc %ds by item{VN:%d VAL%d:%d}", ret, item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					}

					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d�� ��ŭ �����Ǿ����ϴ�."), ret);
					item->SetCount(item->GetCount() - 1);
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_SUCCESS", buf);
					return true;
				}
				else
				{
					if (item->GetVnum() == DRAGON_HEART_VNUM)
					{
						sprintf(buf, "No change by item{VN:%d SOC%d:%d}", item->GetVnum(), ITEM_SOCKET_CHARGING_AMOUNT_IDX, item->GetSocket(ITEM_SOCKET_CHARGING_AMOUNT_IDX));
					}
					else
					{
						sprintf(buf, "No change by item{VN:%d VAL%d:%d}", item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					}

					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �� �����ϴ�."));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_FAILED", buf);
					return false;
				}
			}
			else
				return false;
		}
		break;

		case USE_TIME_CHARGE_FIX:
		{
			LPITEM pDestItem = GetItem(DestCell);
			if (NULL == pDestItem)
				return false;

			// �켱 ��ȥ���� ���ؼ��� �ϵ��� �Ѵ�.
			if (pDestItem->IsDragonSoul())
			{
#if defined(__EXTENDED_DSS_RECHARGE__)
				int ret = pDestItem->GiveMoreTime_Extend(item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
				char buf[128];
				if (ret)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d days have been added."), (ret / 86400));
					sprintf(buf, "Increase %ds by item{VN:%d VAL%d:%d}", ret, item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_SUCCESS", buf);
					item->SetCount(item->GetCount() - 1);
					return true;
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �� �����ϴ�."));
					sprintf(buf, "No change by item{VN:%d VAL%d:%d}", item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_FAILED", buf);
					return false;
				}
#else
				int ret = pDestItem->GiveMoreTime_Fix(item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
				char buf[128];
				if (ret)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d�� ��ŭ �����Ǿ����ϴ�."), ret);
					sprintf(buf, "Increase %ds by item{VN:%d VAL%d:%d}", ret, item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_SUCCESS", buf);
					item->SetCount(item->GetCount() - 1);
					return true;
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �� �����ϴ�."));
					sprintf(buf, "No change by item{VN:%d VAL%d:%d}", item->GetVnum(), ITEM_VALUE_CHARGING_AMOUNT_IDX, item->GetValue(ITEM_VALUE_CHARGING_AMOUNT_IDX));
					LogManager::instance().ItemLog(this, item, "DS_CHARGING_FAILED", buf);
					return false;
				}
#endif
			}
#if defined(__EXTENDED_COSTUME_RECHARGE__)
			else if (pDestItem->IsCostume() && !pDestItem->IsCostumeAcce())
			{
				LPITEM pDestItem = GetItem(DestCell);
				if (!pDestItem)
					return false;

				if (pDestItem->GiveInfiniteTime())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s is now with infinite time."), pDestItem->GetLocaleName());
					item->SetCount(item->GetCount() - 1);
					return true;
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s already has infinite time."), pDestItem->GetLocaleName());
					return false;
				}
			}
#endif
			else
				return false;
		}
		break;

		case USE_SPECIAL:
			switch (item->GetVnum())
			{
#if defined(__CHANGE_LOOK_SYSTEM__)
			case CHANGE_LOOK_REVERSAL:
				LPITEM item2;
				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging())
					return false;

				if (item2->isLocked())
					return false;

				if (item2->GetTransmutationVnum() == 0)
					return false;

				item2->SetTransmutationVnum(0);
				item2->UpdatePacket();

				item->SetCount(item->GetCount() - 1);
				break;
#endif
				// RESEARCHER_ELIXIR
			case 71035:
			{
				if (FindAffect(AFFECT_RESEARCHER_ELIXIR))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				}
				else
				{
					AddAffect(AFFECT_RESEARCHER_ELIXIR, APPLY_NONE, 1, AFF_NONE, INFINITE_AFFECT_DURATION, 0, true);
					item->SetCount(item->GetCount() - 1);
				}
			}
			break;
			// END_RESEARCHER_ELIXIR

#if defined(__SOUL_BIND_SYSTEM__)
			case 50263:
			{
				if (!g_bSoulBind)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot soulbind items on this server."));
					return false;
				}

				LPITEM item2;
				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsEquipped() || item2->IsExchanging())
					return false;

				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This item is already soulbound."));
					return false;
				}

				if (item2->GetType() != ITEM_WEAPON &&
					item2->GetType() != ITEM_ARMOR &&
					item2->GetType() != ITEM_COSTUME &&
					item2->GetType() != ITEM_RING &&
					item2->GetType() != ITEM_BELT &&
					item2->GetType() != ITEM_DS)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot bind this item to your soul."));
					return false;
				}

				item2->SetSoulBind();
				//ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Item binded."));
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "ITEM_BIND_SUCCESS", buf);
				}
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50264:
			{
				LPITEM item2;
				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->isLocked() || item2->IsEquipped())
					return false;

				if (item2->GetSealedTime() >= 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("The soulbind on this item is already in the process of being removed."));
					return false;
				}

				long duration = 72 * 60 * 60;
				item2->SetSoulBind(time(0) + duration);
				item2->StartUnbindTimerExpireEvent();
				//ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Item unbinded."));
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "ITEM_UNBIND_SUCCESS", buf);
				}
				item->SetCount(item->GetCount() - 1);
			}
			break;
#endif

			// ũ�������� ����
			case ITEM_NOG_POCKET:
			{
				/*
				���ִɷ�ġ : item_proto value �ǹ�
					�̵��ӵ� value 1
					���ݷ� value 2
					����ġ value 3
					���ӽð� value 0 (���� ��)
				*/
				if (FindAffect(AFFECT_NOG_ABILITY))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
					return false;
				}
				long time = item->GetValue(0);
				long moveSpeedPer = item->GetValue(1);
				long attPer = item->GetValue(2);
				long expPer = item->GetValue(3);
				AddAffect(AFFECT_NOG_ABILITY, POINT_MOV_SPEED, moveSpeedPer, AFF_MOV_SPEED_POTION, time, 0, true, true);
				AddAffect(AFFECT_NOG_ABILITY, POINT_MALL_ATTBONUS, attPer, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_NOG_ABILITY, POINT_MALL_EXPBONUS, expPer, AFF_NONE, time, 0, true, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			// �󸶴ܿ� ����
			case ITEM_RAMADAN_CANDY:
			{
				/*
				�����ɷ�ġ : item_proto value �ǹ�
					�̵��ӵ� value 1
					���ݷ� value 2
					����ġ value 3
					���ӽð� value 0 (���� ��)
				*/
				long time = item->GetValue(0);
				long moveSpeedPer = item->GetValue(1);
				long attPer = item->GetValue(2);
				long expPer = item->GetValue(3);
				AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MOV_SPEED, moveSpeedPer, AFF_MOV_SPEED_POTION, time, 0, true, true);
				AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MALL_ATTBONUS, attPer, AFF_NONE, time, 0, true, true);
				AddAffect(AFFECT_RAMADAN_ABILITY, POINT_MALL_EXPBONUS, expPer, AFF_NONE, time, 0, true, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_MARRIAGE_RING:
			{
				marriage::TMarriage* pMarriage = marriage::CManager::instance().Get(GetPlayerID());
				if (pMarriage)
				{
					if (pMarriage->ch1 != NULL)
					{
						if (CArenaManager::instance().IsArenaMap(pMarriage->ch1->GetMapIndex()) == true)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� �߿��� �̿��� �� ���� ��ǰ�Դϴ�."));
							break;
						}
					}

					if (pMarriage->ch2 != NULL)
					{
						if (CArenaManager::instance().IsArenaMap(pMarriage->ch2->GetMapIndex()) == true)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� �߿��� �̿��� �� ���� ��ǰ�Դϴ�."));
							break;
						}
					}

					int consumeSP = CalculateConsumeSP(this);

					if (consumeSP < 0)
						return false;

					PointChange(POINT_SP, -consumeSP, false);

					WarpToPID(pMarriage->GetOther(GetPlayerID()));
				}
				else
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ȥ ���°� �ƴϸ� ��ȥ������ ����� �� �����ϴ�."));
			}
			break;

			// ���� ����� ����
			case UNIQUE_ITEM_CAPE_OF_COURAGE:
				// �󸶴� ����� ����� ����
			case 70057:
			case REWARD_BOX_UNIQUE_ITEM_CAPE_OF_COURAGE:
				AggregateMonster();
				if (!g_bUnlimitedCapeOfCourage)
					item->SetCount(item->GetCount() - 1);
				break;

			case UNIQUE_ITEM_WHITE_FLAG:
				ForgetMyAttacker();
				item->SetCount(item->GetCount() - 1);
				break;

			case UNIQUE_ITEM_TREASURE_BOX:
				break;

#if defined(__PRIVATESHOP_SEARCH_SYSTEM__)
			case PRIVATESHOP_SEARCH_LOOKING_GLASS:
			case PRIVATESHOP_SEARCH_TRADING_GLASS:
				OpenPrivateShopSearch(item->GetVnum());
				break;
#endif

			case 30093:
			case 30094:
			case 30095:
			case 30096:
				// ���ָӴ�
			{
				const int MAX_BAG_INFO = 26;
				static struct LuckyBagInfo
				{
					DWORD count;
					int prob;
					DWORD vnum;
				} b1[MAX_BAG_INFO] =
				{
					{ 1000, 302, 1 },
					{ 10, 150, 27002 },
					{ 10, 75, 27003 },
					{ 10, 100, 27005 },
					{ 10, 50, 27006 },
					{ 10, 80, 27001 },
					{ 10, 50, 27002 },
					{ 10, 80, 27004 },
					{ 10, 50, 27005 },
					{ 1, 10, 50300 },
					{ 1, 6, 92 },
					{ 1, 2, 132 },
					{ 1, 6, 1052 },
					{ 1, 2, 1092 },
					{ 1, 6, 2082 },
					{ 1, 2, 2122 },
					{ 1, 6, 3082 },
					{ 1, 2, 3122 },
					{ 1, 6, 5052 },
					{ 1, 2, 5082 },
					{ 1, 6, 7082 },
					{ 1, 2, 7122 },
					{ 1, 1, 11282 },
					{ 1, 1, 11482 },
					{ 1, 1, 11682 },
					{ 1, 1, 11882 },
				};

				struct LuckyBagInfo b2[MAX_BAG_INFO] =
				{
					{ 1000, 302, 1 },
					{ 10, 150, 27002 },
					{ 10, 75, 27002 },
					{ 10, 100, 27005 },
					{ 10, 50, 27005 },
					{ 10, 80, 27001 },
					{ 10, 50, 27002 },
					{ 10, 80, 27004 },
					{ 10, 50, 27005 },
					{ 1, 10, 50300 },
					{ 1, 6, 92 },
					{ 1, 2, 132 },
					{ 1, 6, 1052 },
					{ 1, 2, 1092 },
					{ 1, 6, 2082 },
					{ 1, 2, 2122 },
					{ 1, 6, 3082 },
					{ 1, 2, 3122 },
					{ 1, 6, 5052 },
					{ 1, 2, 5082 },
					{ 1, 6, 7082 },
					{ 1, 2, 7122 },
					{ 1, 1, 11282 },
					{ 1, 1, 11482 },
					{ 1, 1, 11682 },
					{ 1, 1, 11882 },
				};

				LuckyBagInfo* bi = NULL;
				if (LC_IsHongKong())
					bi = b2;
				else
					bi = b1;

				int pct = number(1, 1000);

				int i;
				for (i = 0; i < MAX_BAG_INFO; i++)
				{
					if (pct <= bi[i].prob)
						break;
					pct -= bi[i].prob;
				}
				if (i >= MAX_BAG_INFO)
					return false;

				if (bi[i].vnum == ITEM_SKILLBOOK_VNUM)
				{
					// ��ų���ü��� Ư���ϰ� �ش�.
					GiveRandomSkillBook();
				}
				else if (bi[i].vnum == 1)
				{
					PointChange(POINT_GOLD, 1000, true);
				}
				else
				{
					AutoGiveItem(bi[i].vnum, bi[i].count);
				}
				ITEM_MANAGER::instance().RemoveItem(item);
			}
			break;

			case 50004: // �̺�Ʈ�� ������
			{
				if (item->GetSocket(0))
				{
					item->SetSocket(0, item->GetSocket(0) + 1);
				}
				else
				{
					// ó�� ����
					int iMapIndex = GetMapIndex();

					PIXEL_POSITION pos;

					if (SECTREE_MANAGER::instance().GetRandomLocation(iMapIndex, pos, 700))
					{
						item->SetSocket(0, 1);
						item->SetSocket(1, pos.x);
						item->SetSocket(2, pos.y);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� ������ �̺�Ʈ�� �����Ⱑ �������� �ʴ°� �����ϴ�."));
						return false;
					}
				}

				int dist = 0;
				float distance = (DISTANCE_SQRT(GetX() - item->GetSocket(1), GetY() - item->GetSocket(2)));

				if (distance < 1000.0f)
				{
					// �߰�!
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̺�Ʈ�� �����Ⱑ �ź�ο� ���� ���� ������ϴ�."));

					// ���Ƚ���� ���� �ִ� �������� �ٸ��� �Ѵ�.
					struct TEventStoneInfo
					{
						DWORD dwVnum;
						int count;
						int prob;
					};
					const int EVENT_STONE_MAX_INFO = 15;
					TEventStoneInfo info_10[EVENT_STONE_MAX_INFO] =
					{
						{ 27001, 10, 8 },
						{ 27004, 10, 6 },
						{ 27002, 10, 12 },
						{ 27005, 10, 12 },
						{ 27100, 1, 9 },
						{ 27103, 1, 9 },
						{ 27101, 1, 10 },
						{ 27104, 1, 10 },
						{ 27999, 1, 12 },

						{ 25040, 1, 4 },

						{ 27410, 1, 0 },
						{ 27600, 1, 0 },
						{ 25100, 1, 0 },

						{ 50001, 1, 0 },
						{ 50003, 1, 1 },
					};
					TEventStoneInfo info_7[EVENT_STONE_MAX_INFO] =
					{
						{ 27001, 10, 1 },
						{ 27004, 10, 1 },
						{ 27004, 10, 9 },
						{ 27005, 10, 9 },
						{ 27100, 1, 5 },
						{ 27103, 1, 5 },
						{ 27101, 1, 10 },
						{ 27104, 1, 10 },
						{ 27999, 1, 14 },

						{ 25040, 1, 5 },

						{ 27410, 1, 5 },
						{ 27600, 1, 5 },
						{ 25100, 1, 5 },

						{ 50001, 1, 0 },
						{ 50003, 1, 5 },

					};
					TEventStoneInfo info_4[EVENT_STONE_MAX_INFO] =
					{
						{ 27001, 10, 0 },
						{ 27004, 10, 0 },
						{ 27002, 10, 0 },
						{ 27005, 10, 0 },
						{ 27100, 1, 0 },
						{ 27103, 1, 0 },
						{ 27101, 1, 0 },
						{ 27104, 1, 0 },
						{ 27999, 1, 25 },

						{ 25040, 1, 0 },

						{ 27410, 1, 0 },
						{ 27600, 1, 0 },
						{ 25100, 1, 15 },

						{ 50001, 1, 10 },
						{ 50003, 1, 50 },

					};

					{
						TEventStoneInfo* info;
						if (item->GetSocket(0) <= 4)
							info = info_4;
						else if (item->GetSocket(0) <= 7)
							info = info_7;
						else
							info = info_10;

						int prob = number(1, 100);

						for (int i = 0; i < EVENT_STONE_MAX_INFO; ++i)
						{
							if (!info[i].prob)
								continue;

							if (prob <= info[i].prob)
							{
								if (info[i].dwVnum == 50001)
								{
									DWORD* pdw = M2_NEW DWORD[2];

									pdw[0] = info[i].dwVnum;
									pdw[1] = info[i].count;

									// ��÷���� ������ �����Ѵ�
									DBManager::instance().ReturnQuery(QID_LOTTO, GetPlayerID(), pdw,
										"INSERT INTO lotto_list VALUES(0, 'server%s', %u, NOW())",
										get_table_postfix(), GetPlayerID());
								}
								else
									AutoGiveItem(info[i].dwVnum, info[i].count);

								break;
							}
							prob -= info[i].prob;
						}
					}

					char chatbuf[CHAT_MAX_LEN + 1];
					int len = snprintf(chatbuf, sizeof(chatbuf), "StoneDetect %u 0 0", (DWORD)GetVID());

					if (len < 0 || len >= (int)sizeof(chatbuf))
						len = sizeof(chatbuf) - 1;

					++len; // \0 ���ڱ��� ������

					TPacketGCChat pack_chat;
					pack_chat.header = HEADER_GC_CHAT;
					pack_chat.size = sizeof(TPacketGCChat) + len;
					pack_chat.type = CHAT_TYPE_COMMAND;
					pack_chat.id = 0;
					pack_chat.bEmpire = GetDesc()->GetEmpire();
					//pack_chat.id = vid;

					TEMP_BUFFER buf;
					buf.write(&pack_chat, sizeof(TPacketGCChat));
					buf.write(chatbuf, len);

					PacketAround(buf.read_peek(), buf.size());

					ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (DETECT_EVENT_STONE) 1");
					return true;
				}
				else if (distance < 20000)
					dist = 1;
				else if (distance < 70000)
					dist = 2;
				else
					dist = 3;

				// ���� ��������� �������.
				const int STONE_DETECT_MAX_TRY = 10;
				if (item->GetSocket(0) >= STONE_DETECT_MAX_TRY)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̺�Ʈ�� �����Ⱑ ������ ���� ������ϴ�."));
					ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (DETECT_EVENT_STONE) 0");
					AutoGiveItem(27002);
					return true;
				}

				if (dist)
				{
					char chatbuf[CHAT_MAX_LEN + 1];
					int len = snprintf(chatbuf, sizeof(chatbuf),
						"StoneDetect %u %d %d",
						(DWORD)GetVID(), dist, (int)GetDegreeFromPositionXY(GetX(), item->GetSocket(2), item->GetSocket(1), GetY()));

					if (len < 0 || len >= (int)sizeof(chatbuf))
						len = sizeof(chatbuf) - 1;

					++len; // \0 ���ڱ��� ������

					TPacketGCChat pack_chat;
					pack_chat.header = HEADER_GC_CHAT;
					pack_chat.size = sizeof(TPacketGCChat) + len;
					pack_chat.type = CHAT_TYPE_COMMAND;
					pack_chat.id = 0;
					pack_chat.bEmpire = GetDesc()->GetEmpire();
					//pack_chat.id = vid;

					TEMP_BUFFER buf;
					buf.write(&pack_chat, sizeof(TPacketGCChat));
					buf.write(chatbuf, len);

					PacketAround(buf.read_peek(), buf.size());
				}
			}
			break;

			case 27989: // ����������
			case 76006: // ������ ����������
			{
				LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(GetMapIndex());

				if (pMap != NULL)
				{
					item->SetSocket(0, item->GetSocket(0) + 1);

					FFindStone f;

					// <Factor> SECTREE::for_each -> SECTREE::for_each_entity
					pMap->for_each(f);

					if (f.m_mapStone.size() > 0)
					{
						std::map<DWORD, LPCHARACTER>::iterator stone = f.m_mapStone.begin();

						DWORD max = UINT_MAX;
						LPCHARACTER pTarget = stone->second;

						while (stone != f.m_mapStone.end())
						{
							DWORD dist = (DWORD)DISTANCE_SQRT(GetX() - stone->second->GetX(), GetY() - stone->second->GetY());

							if (dist != 0 && max > dist)
							{
								max = dist;
								pTarget = stone->second;
							}
							stone++;
						}

						if (pTarget != NULL)
						{
							int val = 3;

							if (max < 10000) val = 2;
							else if (max < 70000) val = 1;

							ChatPacket(CHAT_TYPE_COMMAND, "StoneDetect %u %d %d", (DWORD)GetVID(), val,
								(int)GetDegreeFromPositionXY(GetX(), pTarget->GetY(), pTarget->GetX(), GetY()));
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����⸦ �ۿ��Ͽ����� �����Ǵ� ������ �����ϴ�."));
						}
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����⸦ �ۿ��Ͽ����� �����Ǵ� ������ �����ϴ�."));
					}

					if (item->GetSocket(0) >= 6)
					{
						ChatPacket(CHAT_TYPE_COMMAND, "StoneDetect %u 0 0", (DWORD)GetVID());
						ITEM_MANAGER::instance().RemoveItem(item);
					}
				}
				break;
			}
			break;

			case 27996: // ����
				item->SetCount(item->GetCount() - 1);
				/*
				if (GetSkillLevel(SKILL_CREATE_POISON))
					AddAffect(AFFECT_ATT_GRADE, POINT_ATT_GRADE, 3, AFF_DRINK_POISON, 15 * 60, 0, true);
				else
				{
					// ���ٷ�Ⱑ ������ 50% ��� 50% ���ݷ� +2
					if (number(0, 1))
					{
						if (GetHP() > 100)
							PointChange(POINT_HP, -(GetHP() - 1));
						else
							Dead();
					}
					else
						AddAffect(AFFECT_ATT_GRADE, POINT_ATT_GRADE, 2, AFF_DRINK_POISON, 15 * 60, 0, true);
				}
				*/
				break;

			case 27987: // ����
				// 50 ������ 47990
				// 30 ��
				// 10 ������ 47992
				// 7 û���� 47993
				// 3 ������ 47994
			{
				item->SetCount(item->GetCount() - 1);

				int r = number(1, 100);

				if (r <= 50)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�������� �������� ���Խ��ϴ�."));
					AutoGiveItem(27990);
				}
				else
				{
					const int prob_table_euckr[] =
					{
						80, 90, 97
					};

					const int prob_table_gb2312[] =
					{
						95, 97, 99
					};

					const int* prob_table = !g_iUseLocale ? prob_table_euckr : prob_table_gb2312;

					if (r <= prob_table[0])
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ������ ���� ������ϴ�."));
					}
					else if (r <= prob_table[1])
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�������� �����ְ� ���Խ��ϴ�."));
						AutoGiveItem(27992);
					}
					else if (r <= prob_table[2])
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�������� û���ְ� ���Խ��ϴ�."));
						AutoGiveItem(27993);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�������� �����ְ� ���Խ��ϴ�."));
						AutoGiveItem(27994);
					}
				}
			}
			break;

			case 71013: // ����������
				CreateFly(number(FLY_FIREWORK1, FLY_FIREWORK6), this);
				item->SetCount(item->GetCount() - 1);
				break;

			case 50100: // ����
			case 50101:
			case 50102:
			case 50103:
			case 50104:
			case 50105:
			case 50106:
				CreateFly(item->GetVnum() - 50100 + FLY_FIREWORK1, this);
				item->SetCount(item->GetCount() - 1);
				break;

			case 50200: // ������
				if (LC_IsYMIR() == true || LC_IsKorea() == true)
				{
					if (IS_BOTARYABLE_ZONE(GetMapIndex()) == true)
					{
						__OpenPrivateShop();
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ �� �� ���� �����Դϴ�"));
					}
				}
				else
				{
					__OpenPrivateShop();
				}
				break;

			case fishing::FISH_MIND_PILL_VNUM:
				AddAffect(AFFECT_FISH_MIND_PILL, POINT_NONE, 0, AFF_FISH_MIND, 20 * 60, 0, true);
				item->SetCount(item->GetCount() - 1);
				break;

			case 50301: // ��ַ� ���ü�
			case 50302:
			case 50303:
			{
				if (IsPolymorphed() == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�а� �߿��� �ɷ��� �ø� �� �����ϴ�."));
					return false;
				}

				int lv = GetSkillLevel(SKILL_LEADERSHIP);

				if (lv < item->GetValue(0))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� å�� �ʹ� ����� �����ϱⰡ ����ϴ�."));
					return false;
				}

				if (lv >= item->GetValue(1))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� å�� �ƹ��� ���� ������ �� �� ���� �ʽ��ϴ�."));
					return false;
				}

				if (LearnSkillByBook(SKILL_LEADERSHIP))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(SKILL_LEADERSHIP, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50304: // ����� ���ü�
			case 50305:
			case 50306:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����߿��� å�� ������ �����ϴ�."));
					return false;
				}

				if (GetSkillLevel(SKILL_COMBO) == 0 && GetLevel() < 30)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� 30�� �Ǳ� ������ ������ �� ���� �� ���� �ʽ��ϴ�."));
					return false;
				}

				if (GetSkillLevel(SKILL_COMBO) == 1 && GetLevel() < 50)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� 50�� �Ǳ� ������ ������ �� ���� �� ���� �ʽ��ϴ�."));
					return false;
				}

				if (GetSkillLevel(SKILL_COMBO) >= 2)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ���̻� ������ �� �����ϴ�."));
					return false;
				}

				int iPct = item->GetValue(0);

				if (LearnSkillByBook(SKILL_COMBO, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(SKILL_COMBO, get_global_time() + iReadDelay);
				}
			}
			break;
			case 50311: // ��� ���ü�
			case 50312:
			case 50313:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����߿��� å�� ������ �����ϴ�."));
					return false;
				}

				DWORD dwSkillVnum = item->GetValue(0);
				int iPct = MINMAX(0, item->GetValue(1), 100);
				if (GetSkillLevel(dwSkillVnum) >= 20 || dwSkillVnum - SKILL_LANGUAGE1 + 1 == GetEmpire())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� �Ϻ��ϰ� �˾Ƶ��� �� �ִ� ����̴�."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50061: // �Ϻ� �� ��ȯ ��ų ���ü�
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����߿��� å�� ������ �����ϴ�."));
					return false;
				}

				DWORD dwSkillVnum = item->GetValue(0);
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetSkillLevel(dwSkillVnum) >= 10)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �̻� ������ �� �����ϴ�."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50314: case 50315: case 50316: // ���� ���ü�
			case 50323: case 50324: // ���� ���ü�
			case 50325: case 50326: // ö�� ���ü�
			{
				if (IsPolymorphed() == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�а� �߿��� �ɷ��� �ø� �� �����ϴ�."));
					return false;
				}

				int iSkillLevelLowLimit = item->GetValue(0);
				int iSkillLevelHighLimit = item->GetValue(1);
				int iPct = MINMAX(0, item->GetValue(2), 100);
				int iLevelLimit = item->GetValue(3);
				DWORD dwSkillVnum = 0;

				switch (item->GetVnum())
				{
				case 50314: case 50315: case 50316:
					dwSkillVnum = SKILL_POLYMORPH;
					break;

				case 50323: case 50324:
					dwSkillVnum = SKILL_ADD_HP;
					break;

				case 50325: case 50326:
					dwSkillVnum = SKILL_RESIST_PENETRATE;
					break;

				default:
					return false;
				}

				if (0 == dwSkillVnum)
					return false;

				if (GetLevel() < iLevelLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� å�� �������� ������ �� �÷��� �մϴ�."));
					return false;
				}

				if (GetSkillLevel(dwSkillVnum) >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �̻� ������ �� �����ϴ�."));
					return false;
				}

				if (GetSkillLevel(dwSkillVnum) < iSkillLevelLowLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� å�� �ʹ� ����� �����ϱⰡ ����ϴ�."));
					return false;
				}

				if (GetSkillLevel(dwSkillVnum) >= iSkillLevelHighLimit)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� å���δ� �� �̻� ������ �� �����ϴ�."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;

			case 50902:
			case 50903:
			case 50904:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����߿��� å�� ������ �����ϴ�."));
					return false;
				}

				DWORD dwSkillVnum = SKILL_CREATE;
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetSkillLevel(dwSkillVnum) >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �̻� ������ �� �����ϴ�."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);

					if (test_server)
					{
						ChatPacket(CHAT_TYPE_INFO, "[TEST_SERVER] Success to learn skill ");
					}
				}
				else
				{
					if (test_server)
					{
						ChatPacket(CHAT_TYPE_INFO, "[TEST_SERVER] Failed to learn skill ");
					}
				}
			}
			break;

			// MINING
			case ITEM_MINING_SKILL_TRAIN_BOOK:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����߿��� å�� ������ �����ϴ�."));
					return false;
				}

				DWORD dwSkillVnum = SKILL_MINING;
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetSkillLevel(dwSkillVnum) >= 40)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �̻� ������ �� �����ϴ�."));
					return false;
				}

				if (LearnSkillByBook(dwSkillVnum, iPct))
				{
					item->SetCount(item->GetCount() - 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
			}
			break;
			// END_OF_MINING

			case ITEM_HORSE_SKILL_TRAIN_BOOK:
			{
				if (IsPolymorphed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����߿��� å�� ������ �����ϴ�."));
					return false;
				}

				DWORD dwSkillVnum = SKILL_HORSE;
				int iPct = MINMAX(0, item->GetValue(1), 100);

				if (GetLevel() < 50)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �¸� ��ų�� ������ �� �ִ� ������ �ƴմϴ�."));
					return false;
				}

				if (!test_server && get_global_time() < GetSkillNextReadTime(dwSkillVnum))
				{
					if (FindAffect(AFFECT_SKILL_NO_BOOK_DELAY))
					{
						// �־ȼ��� ����߿��� �ð� ���� ����
						RemoveAffect(AFFECT_SKILL_NO_BOOK_DELAY);
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�־ȼ����� ���� ��ȭ�Ը����� �������Խ��ϴ�."));
					}
					else
					{
						SkillLearnWaitMoreTimeMessage(GetSkillNextReadTime(dwSkillVnum) - get_global_time());
						return false;
					}
				}

				if (GetPoint(POINT_HORSE_SKILL) >= 20 ||
					GetSkillLevel(SKILL_HORSE_WILDATTACK) + GetSkillLevel(SKILL_HORSE_CHARGE) + GetSkillLevel(SKILL_HORSE_ESCAPE) >= 60 ||
					GetSkillLevel(SKILL_HORSE_WILDATTACK_RANGE) + GetSkillLevel(SKILL_HORSE_CHARGE) + GetSkillLevel(SKILL_HORSE_ESCAPE) >= 60)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �̻� �¸� ���ü��� ���� �� �����ϴ�."));
					return false;
				}

				if (number(1, 100) <= iPct)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�¸� ���ü��� �о� �¸� ��ų ����Ʈ�� ������ϴ�."));
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ����Ʈ�δ� �¸� ��ų�� ������ �ø� �� �ֽ��ϴ�."));
					PointChange(POINT_HORSE_SKILL, 1);

					int iReadDelay = number(SKILLBOOK_DELAY_MIN, SKILLBOOK_DELAY_MAX);
					if (distribution_test_server) iReadDelay /= 3;

					if (!test_server)
						SetSkillNextReadTime(dwSkillVnum, get_global_time() + iReadDelay);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�¸� ���ü� ���ؿ� �����Ͽ����ϴ�."));
				}

				ITEM_MANAGER::instance().RemoveItem(item);
			}
			break;

			case 70102: // ����
			case 70103: // ����
			{
				if (GetAlignment() >= 0)
					return false;

				int delta = MIN(-GetAlignment(), item->GetValue(0));

				sys_log(0, "%s ALIGNMENT ITEM %d", GetName(), delta);

				UpdateAlignment(delta);
				item->SetCount(item->GetCount() - 1);

				if (delta / 10 > 0)
				{
					ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("������ �������±�. ������ �������� ���𰡰� �� �������� �����̾�."));
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ġ�� %d �����Ͽ����ϴ�."), delta / 10);
				}
			}
			break;

			case 71107: // õ��������
			{
				int val = item->GetValue(0);
				int interval = item->GetValue(1);
				quest::PC* pPC = quest::CQuestManager::instance().GetPC(GetPlayerID());
				int last_use_time = pPC->GetFlag("mythical_peach.last_use_time");

				if (get_global_time() - last_use_time < interval * 60 * 60)
				{
					if (test_server == false)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ����� �� �����ϴ�."));
						return false;
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�׽�Ʈ ���� �ð����� ���"));
					}
				}

				if (GetAlignment() == 200000)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ġ�� �� �̻� �ø� �� �����ϴ�."));
					return false;
				}

				if (200000 - GetAlignment() < val * 10)
				{
					val = (200000 - GetAlignment()) / 10;
				}

				int old_alignment = GetAlignment() / 10;

				UpdateAlignment(val * 10);

				item->SetCount(item->GetCount() - 1);
				pPC->SetFlag("mythical_peach.last_use_time", get_global_time());

				ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("������ �������±�. ������ �������� ���𰡰� �� �������� �����̾�."));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ġ�� %d �����Ͽ����ϴ�."), val);

				char buf[256 + 1];
				snprintf(buf, sizeof(buf), "%d %d", old_alignment, GetAlignment() / 10);
				LogManager::instance().CharLog(this, val, "MYTHICAL_PEACH", buf);
			}
			break;

			case 71109: // Ż����
			case 72719:
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsExchanging() == true)
					return false;

				if (item2->GetSocketCount() == 0)
					return false;

				if (item2->IsEquipped())
					return false;

				switch (item2->GetType())
				{
				case ITEM_WEAPON:
					break;
				case ITEM_ARMOR:
					switch (item2->GetSubType())
					{
					case ARMOR_EAR:
					case ARMOR_WRIST:
					case ARMOR_NECK:
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ �����ϴ�"));
						return false;
					}
					break;

				default:
					return false;
				}

				std::stack<long> socket;

				for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
					socket.push(item2->GetSocket(i));

				int idx = ITEM_SOCKET_MAX_NUM - 1;

				while (socket.size() > 0)
				{
					if (socket.top() > 2 && socket.top() != ITEM_BROKEN_METIN_VNUM)
						break;

					idx--;
					socket.pop();
				}

				if (socket.size() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ �����ϴ�"));
					return false;
				}

				LPITEM pItemReward = AutoGiveItem(socket.top());

				if (pItemReward != NULL)
				{
					item2->SetSocket(idx, 1);

					char buf[256 + 1];
					snprintf(buf, sizeof(buf), "%s(%u) %s(%u)",
						item2->GetName(), item2->GetID(), pItemReward->GetName(), pItemReward->GetID());
					LogManager::instance().ItemLog(this, item, "USE_DETACHMENT_ONE", buf);

					item->SetCount(item->GetCount() - 1);
				}
			}
			break;

			case 70201: // Ż����
			case 70202: // ������(���)
			case 70203: // ������(�ݻ�)
			case 70204: // ������(������)
			case 70205: // ������(����)
			case 70206: // ������(������)
			{
				// NEW_HAIR_STYLE_ADD
				if (GetPart(PART_HAIR) >= 1001)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ��Ÿ�Ͽ����� ������ Ż���� �Ұ����մϴ�."));
				}
				// END_NEW_HAIR_STYLE_ADD
				else
				{
					quest::CQuestManager& q = quest::CQuestManager::instance();
					quest::PC* pPC = q.GetPC(GetPlayerID());

					if (pPC)
					{
						int last_dye_level = pPC->GetFlag("dyeing_hair.last_dye_level");

						if (last_dye_level == 0 ||
							last_dye_level + 3 <= GetLevel() ||
							item->GetVnum() == 70201)
						{
							SetPart(PART_HAIR, item->GetVnum() - 70201);

							if (item->GetVnum() == 70201)
								pPC->SetFlag("dyeing_hair.last_dye_level", 0);
							else
								pPC->SetFlag("dyeing_hair.last_dye_level", GetLevel());

							item->SetCount(item->GetCount() - 1);
							UpdatePacket();
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d ������ �Ǿ�� �ٽ� �����Ͻ� �� �ֽ��ϴ�."), last_dye_level + 3);
						}
					}
				}
			}
			break;

			case ITEM_NEW_YEAR_GREETING_VNUM:
			{
				if (GiveItemFromSpecialItemGroup(ITEM_NEW_YEAR_GREETING_VNUM))
					item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_VALENTINE_ROSE:
			case ITEM_VALENTINE_CHOCOLATE:
			{
				if (item->GetVnum() == ITEM_VALENTINE_ROSE && SEX_MALE == GET_SEX(this) ||
					item->GetVnum() == ITEM_VALENTINE_CHOCOLATE && SEX_FEMALE == GET_SEX(this))
				{
					// ������ �����ʾ� �� �� ����.
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �����ʾ� �� �������� �� �� �����ϴ�."));
					return false;
				}

				if (GiveItemFromSpecialItemGroup(item->GetVnum()))
					item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_WHITEDAY_CANDY:
			case ITEM_WHITEDAY_ROSE:
			{
				if (item->GetVnum() == ITEM_WHITEDAY_CANDY && SEX_MALE == GET_SEX(this) ||
					item->GetVnum() == ITEM_WHITEDAY_ROSE && SEX_FEMALE == GET_SEX(this))
				{
					// ������ �����ʾ� �� �� ����.
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �����ʾ� �� �������� �� �� �����ϴ�."));
					return false;
				}

				if (GiveItemFromSpecialItemGroup(item->GetVnum()))
					item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50011: // ��������
			{
				if (!GiveItemFromSpecialItemGroup(50011))
				{
					ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("�ƹ��͵� ���� �� �������ϴ�."));
					return false;
				}

				item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_GIVE_STAT_RESET_COUNT_VNUM:
			{
				//PointChange(POINT_GOLD, -iCost);
				PointChange(POINT_STAT_RESET_COUNT, 1);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50107:
			{
				EffectPacket(SE_CHINA_FIREWORK);
				// ���� ������ �÷��ش�
				AddAffect(AFFECT_CHINA_FIREWORK, POINT_STUN_PCT, 30, AFF_CHINA_FIREWORK, 5 * 60, 0, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case 50108:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� �߿��� �̿��� �� ���� ��ǰ�Դϴ�."));
					return false;
				}

				EffectPacket(SE_SPIN_TOP);
				// ���� ������ �÷��ش�
				AddAffect(AFFECT_CHINA_FIREWORK, POINT_STUN_PCT, 30, AFF_CHINA_FIREWORK, 5 * 60, 0, true);
				item->SetCount(item->GetCount() - 1);
			}
			break;

			case ITEM_WONSO_BEAN_VNUM:
				PointChange(POINT_HP, GetMaxHP() - GetHP());
				item->SetCount(item->GetCount() - 1);
				break;

			case ITEM_WONSO_SUGAR_VNUM:
				PointChange(POINT_SP, GetMaxSP() - GetSP());
				item->SetCount(item->GetCount() - 1);
				break;

			case ITEM_WONSO_FRUIT_VNUM:
				PointChange(POINT_STAMINA, GetMaxStamina() - GetStamina());
				item->SetCount(item->GetCount() - 1);
				break;

			case ITEM_ELK_VNUM: // ���ٷ���
			{
				int iGold = item->GetSocket(0);
				ITEM_MANAGER::instance().RemoveItem(item);
#if defined(__CHATTING_WINDOW_RENEWAL__)
				ChatPacket(CHAT_TYPE_MONEY_INFO, LC_TEXT("�� %d ���� ȹ���߽��ϴ�."), iGold);
#else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� %d ���� ȹ���߽��ϴ�."), iGold);
#endif
				PointChange(POINT_GOLD, iGold);
			}
			break;

			// ������ ��ǥ
			case 70021:
			{
				int HealPrice = quest::CQuestManager::instance().GetEventFlag("MonarchHealGold");
				if (HealPrice == 0)
					HealPrice = 2000000;

				if (CMonarch::instance().HealMyEmpire(this, HealPrice))
				{
					SendNoticeMap("������ �ູ���� ������ %s ������ HP,SP�� ��� ä�����ϴ�.", GetMapIndex(), false, EMPIRE_NAME(GetEmpire()));
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ູ�� ����Ͽ����ϴ�."));
				}
			}
			break;

			case 27995:
			{
			}
			break;

			case 71092: // ���� ��ü�� �ӽ�
			{
				if (m_pkChrTarget != NULL)
				{
					if (m_pkChrTarget->IsPolymorphed())
					{
						m_pkChrTarget->SetPolymorph(0);
						m_pkChrTarget->RemoveAffect(AFFECT_POLYMORPH);
					}
				}
				else
				{
					if (IsPolymorphed())
					{
						SetPolymorph(0);
						RemoveAffect(AFFECT_POLYMORPH);
					}
				}
			}
			break;

			case 71051: // ���簡
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetInventoryItem(wDestCell)))
					return false;

				if (item2->IsExchanging() == true)
					return false;

				if (item2->IsEquipped())
					return false;

				if (item2->GetType() == ITEM_COSTUME)
					return false;

				//if (item2->GetAttributeCount() < 5)
				//	return false;

				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� ������ �� ���� �������Դϴ�."));
					return false;
				}

				if (item2->AddRareAttribute() == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ� �߰��� �����Ͽ����ϴ�."));

					int iAddedIdx = item2->GetRareAttrCount() + 4;
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					LogManager::instance().ItemLog(
						GetPlayerID(),
						item2->GetAttributeType(iAddedIdx),
						item2->GetAttributeValue(iAddedIdx),
						item->GetID(),
						"ADD_RARE_ATTR",
						buf,
						GetDesc()->GetHostName(),
						item->GetOriginalVnum());

					if (!g_bUnlimitedAddRareAttributes)
						item->SetCount(item->GetCount() - 1);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ� �߰��� �����Ͽ����ϴ�."));
				}
			}
			break;

			case 71052: // �����
			{
				LPITEM item2;

				if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
					return false;

				if (item2->IsEquipped())
					return false;

				if (item2->GetType() == ITEM_COSTUME)
					return false;

				if (item2->IsExchanging() == true)
					return false;

				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� ������ �� ���� �������Դϴ�."));
					return false;
				}

				if (item2->ChangeRareAttribute() == true)
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CHANGE_RARE_ATTR", buf);

					if (!g_bUnlimitedChangeRareAttributes)
						item->SetCount(item->GetCount() - 1);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� �����Ͽ����ϴ�."));
				}
			}
			break;

			case ITEM_AUTO_HP_RECOVERY_S:
			case ITEM_AUTO_HP_RECOVERY_M:
			case ITEM_AUTO_HP_RECOVERY_L:
			case ITEM_AUTO_HP_RECOVERY_X:
			case ITEM_AUTO_SP_RECOVERY_S:
			case ITEM_AUTO_SP_RECOVERY_M:
			case ITEM_AUTO_SP_RECOVERY_L:
			case ITEM_AUTO_SP_RECOVERY_X:
				// ���ù��������� ������ �ϴ� �� ��ġ��� ������...
				// �׷��� �׳� �ϵ� �ڵ�. ���� ���ڿ� �ڵ����� �����۵�.
			case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_XS:
			case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_S:
			case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_XS:
			case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_S:
			case FUCKING_BRAZIL_ITEM_AUTO_SP_RECOVERY_S:
			case FUCKING_BRAZIL_ITEM_AUTO_HP_RECOVERY_S:
			{
				if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����忡�� ����Ͻ� �� �����ϴ�."));
					return false;
				}

				EAffectTypes type = AFFECT_NONE;
				bool isSpecialPotion = false;

				switch (item->GetVnum())
				{
				case ITEM_AUTO_HP_RECOVERY_X:
					isSpecialPotion = true;

				case ITEM_AUTO_HP_RECOVERY_S:
				case ITEM_AUTO_HP_RECOVERY_M:
				case ITEM_AUTO_HP_RECOVERY_L:
				case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_XS:
				case REWARD_BOX_ITEM_AUTO_HP_RECOVERY_S:
				case FUCKING_BRAZIL_ITEM_AUTO_HP_RECOVERY_S:
					type = AFFECT_AUTO_HP_RECOVERY;
					break;

				case ITEM_AUTO_SP_RECOVERY_X:
					isSpecialPotion = true;

				case ITEM_AUTO_SP_RECOVERY_S:
				case ITEM_AUTO_SP_RECOVERY_M:
				case ITEM_AUTO_SP_RECOVERY_L:
				case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_XS:
				case REWARD_BOX_ITEM_AUTO_SP_RECOVERY_S:
				case FUCKING_BRAZIL_ITEM_AUTO_SP_RECOVERY_S:
					type = AFFECT_AUTO_SP_RECOVERY;
					break;
				}

				if (AFFECT_NONE == type)
					break;

				if (item->GetCount() > 1)
				{
					int pos = GetEmptyInventory(item->GetSize());

					if (-1 == pos)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����ǰ�� �� ������ �����ϴ�."));
						break;
					}

					item->SetCount(item->GetCount() - 1);

					LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), 1);
					item2->AddToCharacter(this, TItemPos(INVENTORY, pos));

					if (item->GetSocket(1) != 0)
					{
						item2->SetSocket(1, item->GetSocket(1));
					}

					item = item2;
				}

				CAffect* pAffect = FindAffect(type);

				if (NULL == pAffect)
				{
					EPointTypes bonus = POINT_NONE;

					if (true == isSpecialPotion)
					{
						if (type == AFFECT_AUTO_HP_RECOVERY)
						{
							bonus = POINT_MAX_HP_PCT;
						}
						else if (type == AFFECT_AUTO_SP_RECOVERY)
						{
							bonus = POINT_MAX_SP_PCT;
						}
					}

					AddAffect(type, bonus, 4, item->GetID(), INFINITE_AFFECT_DURATION, 0, true, false);

					item->Lock(true);
					item->SetSocket(0, true);

					AutoRecoveryItemProcess(type);
				}
				else
				{
					if (item->GetID() == pAffect->dwFlag)
					{
						RemoveAffect(pAffect);

						item->Lock(false);
						item->SetSocket(0, false);
					}
					else
					{
						LPITEM old = FindItemByID(pAffect->dwFlag);

						if (NULL != old)
						{
							old->Lock(false);
							old->SetSocket(0, false);
						}

						RemoveAffect(pAffect);

						EPointTypes bonus = POINT_NONE;

						if (true == isSpecialPotion)
						{
							if (type == AFFECT_AUTO_HP_RECOVERY)
							{
								bonus = POINT_MAX_HP_PCT;
							}
							else if (type == AFFECT_AUTO_SP_RECOVERY)
							{
								bonus = POINT_MAX_SP_PCT;
							}
						}

						AddAffect(type, bonus, 4, item->GetID(), INFINITE_AFFECT_DURATION, 0, true, false);

						item->Lock(true);
						item->SetSocket(0, true);

						AutoRecoveryItemProcess(type);
					}
				}
			}
			break;
			}
			break;

		case USE_CLEAR:
		{
			switch (item->GetVnum())
			{
			case 27124: // Bandage
				RemoveBleeding();
				break;
			case 27874: // Grilled Perch
			default:
				RemoveBadAffect();
				break;
			}
			item->SetCount(item->GetCount() - 1);
		}
		break;

		case USE_INVISIBILITY:
		{
			if (item->GetVnum() == 70026)
			{
				quest::CQuestManager& q = quest::CQuestManager::instance();
				quest::PC* pPC = q.GetPC(GetPlayerID());

				if (pPC != NULL)
				{
					int last_use_time = pPC->GetFlag("mirror_of_disapper.last_use_time");

					if (get_global_time() - last_use_time < 10 * 60)
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ����� �� �����ϴ�."));
						return false;
					}

					pPC->SetFlag("mirror_of_disapper.last_use_time", get_global_time());
				}
			}

			AddAffect(AFFECT_INVISIBILITY, POINT_NONE, 0, AFF_INVISIBILITY, 300, 0, true);
			item->SetCount(item->GetCount() - 1);
		}
		break;

		case USE_POTION_NODELAY:
		{
			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����忡�� ����Ͻ� �� �����ϴ�."));
					return false;
				}

				switch (item->GetVnum())
				{
				case 70020:
				case 71018:
				case 71019:
				case 71020:
					if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
					{
						if (m_nPotionLimit <= 0)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� ���ѷ��� �ʰ��Ͽ����ϴ�."));
							return false;
						}
					}
					break;

				default:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����忡�� ����Ͻ� �� �����ϴ�."));
					return false;
				}
			}

			bool used = false;

			if (item->GetValue(0) != 0) // HP ���밪 ȸ��
			{
				if (GetHP() < GetMaxHP())
				{
					PointChange(POINT_HP, item->GetValue(0) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
					EffectPacket(SE_HPUP_RED);
					used = TRUE;
				}
			}

			if (item->GetValue(1) != 0) // SP ���밪 ȸ��
			{
				if (GetSP() < GetMaxSP())
				{
					PointChange(POINT_SP, item->GetValue(1) * (100 + GetPoint(POINT_POTION_BONUS)) / 100);
					EffectPacket(SE_SPUP_BLUE);
					used = TRUE;
				}
			}

			if (item->GetValue(3) != 0) // HP % ȸ��
			{
				if (GetHP() < GetMaxHP())
				{
					PointChange(POINT_HP, item->GetValue(3) * GetMaxHP() / 100);
					EffectPacket(SE_HPUP_RED);
					used = TRUE;
				}
			}

			if (item->GetValue(4) != 0) // SP % ȸ��
			{
				if (GetSP() < GetMaxSP())
				{
					PointChange(POINT_SP, item->GetValue(4) * GetMaxSP() / 100);
					EffectPacket(SE_SPUP_BLUE);
					used = TRUE;
				}
			}

			if (used)
			{
				if (item->GetVnum() == 50085 || item->GetVnum() == 50086)
				{
					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �Ǵ� ���� �� ����Ͽ����ϴ�"));
					SetUseSeedOrMoonBottleTime();
				}
				if (GetDungeon())
					GetDungeon()->UsePotion(this);

				if (GetWarMap())
					GetWarMap()->UsePotion(this, item);

				m_nPotionLimit--;

				// RESTRICT_USE_SEED_OR_MOONBOTTLE
				item->SetCount(item->GetCount() - 1);
				// END_RESTRICT_USE_SEED_OR_MOONBOTTLE
			}
		}
		break;

		case USE_POTION:
		{
			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit") > 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����忡�� ����Ͻ� �� �����ϴ�."));
					return false;
				}

				switch (item->GetVnum())
				{
				case 27001:
				case 27002:
				case 27003:
				case 27004:
				case 27005:
				case 27006:
					if (quest::CQuestManager::instance().GetEventFlag("arena_potion_limit_count") < 10000)
					{
						if (m_nPotionLimit <= 0)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� ���ѷ��� �ʰ��Ͽ����ϴ�."));
							return false;
						}
					}
					break;

				default:
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����忡�� ����Ͻ� �� �����ϴ�."));
					return false;
				}
			}

			if (item->GetValue(1) != 0)
			{
				if (GetPoint(POINT_SP_RECOVERY) + GetSP() >= GetMaxSP())
					return false;

				PointChange(POINT_SP_RECOVERY, item->GetValue(1) * MIN(200, (100 + GetPoint(POINT_POTION_BONUS))) / 100);
				StartAffectEvent();
				EffectPacket(SE_SPUP_BLUE);
			}

			if (item->GetValue(0) != 0)
			{
				if (GetPoint(POINT_HP_RECOVERY) + GetHP() >= GetMaxHP())
					return false;

				PointChange(POINT_HP_RECOVERY, item->GetValue(0) * MIN(200, (100 + GetPoint(POINT_POTION_BONUS))) / 100);
				StartAffectEvent();
				EffectPacket(SE_HPUP_RED);
			}

			if (GetDungeon())
				GetDungeon()->UsePotion(this);

			if (GetWarMap())
				GetWarMap()->UsePotion(this, item);

			item->SetCount(item->GetCount() - 1);
			m_nPotionLimit--;
		}
		break;

		case USE_POTION_CONTINUE:
		{
			if (item->GetValue(0) != 0)
			{
				AddAffect(AFFECT_HP_RECOVER_CONTINUE, POINT_HP_RECOVER_CONTINUE, item->GetValue(0), 0, item->GetValue(2), 0, true);
			}
			else if (item->GetValue(1) != 0)
			{
				AddAffect(AFFECT_SP_RECOVER_CONTINUE, POINT_SP_RECOVER_CONTINUE, item->GetValue(1), 0, item->GetValue(2), 0, true);
			}
			else
				return false;
		}

		if (GetDungeon())
			GetDungeon()->UsePotion(this);

		if (GetWarMap())
			GetWarMap()->UsePotion(this, item);

		item->SetCount(item->GetCount() - 1);
		break;

		case USE_ABILITY_UP:
		{
			switch (item->GetValue(0))
			{
			case APPLY_MOV_SPEED:
#if defined(__EXTENDED_BLEND_AFFECT__)
				if (FindAffect(AFFECT_MOVE_SPEED))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
					return false;
				}
#endif

				if (FindAffect(AFFECT_MOV_SPEED))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
					return false;
				}

				EffectPacket(SE_DXUP_PURPLE);
				AddAffect(AFFECT_MOV_SPEED, POINT_MOV_SPEED, item->GetValue(2), AFF_MOV_SPEED_POTION, item->GetValue(1), 0, true);
				break;

			case APPLY_ATT_SPEED:
#if defined(__EXTENDED_BLEND_AFFECT__)
				if (FindAffect(AFFECT_ATTACK_SPEED))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
					return false;
				}
#endif

				if (FindAffect(AFFECT_ATT_SPEED))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
					return false;
				}

				EffectPacket(SE_SPEEDUP_GREEN);
				AddAffect(AFFECT_ATT_SPEED, POINT_ATT_SPEED, item->GetValue(2), AFF_ATT_SPEED_POTION, item->GetValue(1), 0, true);
				break;

			case APPLY_STR:
				AddAffect(AFFECT_STR, POINT_ST, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_DEX:
				AddAffect(AFFECT_DEX, POINT_DX, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_CON:
				AddAffect(AFFECT_CON, POINT_HT, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_INT:
				AddAffect(AFFECT_INT, POINT_IQ, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_CAST_SPEED:
				AddAffect(AFFECT_CAST_SPEED, POINT_CASTING_SPEED, item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_ATT_GRADE_BONUS:
				AddAffect(AFFECT_ATT_GRADE, POINT_ATT_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;

			case APPLY_DEF_GRADE_BONUS:
				AddAffect(AFFECT_DEF_GRADE, POINT_DEF_GRADE_BONUS,
					item->GetValue(2), 0, item->GetValue(1), 0, true);
				break;
			}
		}

		if (GetDungeon())
			GetDungeon()->UsePotion(this);

		if (GetWarMap())
			GetWarMap()->UsePotion(this, item);

		item->SetCount(item->GetCount() - 1);
		break;

		case USE_TALISMAN:
		{
			const int TOWN_PORTAL = 1;
			const int MEMORY_PORTAL = 2;

			// gm_guild_build, oxevent �ʿ��� ��ȯ�� ��ȯ���� �� �����ϰ� ����
			if (GetMapIndex() == 200 || GetMapIndex() == 113)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ��ġ���� ����� �� �����ϴ�."));
				return false;
			}

			if (CArenaManager::instance().IsArenaMap(GetMapIndex()) == true)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��� �߿��� �̿��� �� ���� ��ǰ�Դϴ�."));
				return false;
			}

			if (m_pkWarpEvent)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̵��� �غ� �Ǿ��������� ��ȯ�θ� ����Ҽ� �����ϴ�"));
				return false;
			}

			// CONSUME_LIFE_WHEN_USE_WARP_ITEM
			int consumeLife = CalculateConsume(this);

			if (consumeLife < 0)
				return false;
			// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

			if (item->GetValue(0) == TOWN_PORTAL) // ��ȯ��
			{
				if (item->GetSocket(0) == 0)
				{
					if (!GetDungeon())
						if (!GiveRecallItem(item))
							return false;

					PIXEL_POSITION posWarp;

					if (SECTREE_MANAGER::instance().GetRecallPositionByEmpire(GetMapIndex(), GetEmpire(), posWarp))
					{
						// CONSUME_LIFE_WHEN_USE_WARP_ITEM
						PointChange(POINT_HP, -consumeLife, false);
						// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

						WarpSet(posWarp.x, posWarp.y);
					}
					else
					{
						sys_err("CHARACTER::UseItem : cannot find spawn position (name %s, %d x %d)", GetName(), GetX(), GetY());
					}
				}
				else
				{
					if (test_server)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ��ġ�� ����"));

					ProcessRecallItem(item);
				}
			}
			else if (item->GetValue(0) == MEMORY_PORTAL) // ��ȯ����
			{
				if (item->GetSocket(0) == 0)
				{
					if (GetDungeon())
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �ȿ����� %s%s ����� �� �����ϴ�."), item->GetLocaleName(),
							g_iUseLocale ? "" : (under_han(item->GetName()) ? LC_TEXT("��") : LC_TEXT("��")));
						return false;
					}

					if (!GiveRecallItem(item))
						return false;
				}
				else
				{
					// CONSUME_LIFE_WHEN_USE_WARP_ITEM
					PointChange(POINT_HP, -consumeLife, false);
					// END_OF_CONSUME_LIFE_WHEN_USE_WARP_ITEM

					ProcessRecallItem(item);
				}
			}
		}
		break;

		case USE_TUNING:
		case USE_DETACHMENT:
		{
			LPITEM item2;

			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsEquipped())
				return false;

			if (item2->IsExchanging())
				return false;

			if (PreventTradeWindow(WND_ALL))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("â��,�ŷ�â���� ���� ���¿����� ������ �Ҽ��� �����ϴ�"));
				return false;
			}

#if defined(__STONE_OF_BLESS__)
			if (item->GetValue(0) == STONE_OF_BLESS)
			{
				if (item2->GetLevelLimit() <= 80)
					return false;
			}
#endif

#if defined(__SOUL_SYSTEM__)
			if (item->GetValue(0) == SOUL_EVOLVE_SCROLL || item->GetValue(0) == SOUL_AWAKE_SCROLL)
			{
				if (item2->GetType() != ITEM_SOUL)
					return false;
			}
#endif

			if (item2->GetVnum() >= 28330 && item2->GetVnum() <= 28343) // ����+3
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("+3 ������ �� ���������� ������ �� �����ϴ�"));
				return false;
			}

			if (item2->GetVnum() >= 28430 && item2->GetVnum() <= 28443) // ����+4
			{
				if (item->GetVnum() == 71056) // û���Ǽ���
				{
					RefineItem(item, item2);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �� ���������� ������ �� �����ϴ�"));
				}
			}
			else
			{
				RefineItem(item, item2);
			}
		}
		break;

		// ACCESSORY_REFINE & ADD/CHANGE_ATTRIBUTES
		case USE_PUT_INTO_BELT_SOCKET:
		case USE_PUT_INTO_RING_SOCKET:
		case USE_PUT_INTO_ACCESSORY_SOCKET:
		case USE_ADD_ACCESSORY_SOCKET:
		case USE_CLEAN_SOCKET:
		case USE_CHANGE_ATTRIBUTE:
		case USE_CHANGE_ATTRIBUTE2:
		case USE_ADD_ATTRIBUTE:
		case USE_ADD_ATTRIBUTE2:
		{
			LPITEM item2;
			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsEquipped())
			{
				// BuffOnAttr_RemoveBuffsFromItem(item2);
				return false;
			}

			// [NOTE] �ڽ�Ƭ �����ۿ��� ������ ���� ������ ���� �Ӽ��� �ο��ϵ�, ����簡 ����� ���ƴ޶�� ��û�� �־���.
			// ���� ANTI_CHANGE_ATTRIBUTE ���� ������ Flag�� �߰��Ͽ� ��ȹ �������� �����ϰ� ��Ʈ�� �� �� �ֵ��� �� �����̾�����
			// �׵��� �ʿ������ ��ġ�� ���� �ش޷��� �׳� ���⼭ ����... -_-
			/*
			if (IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_CHANGE_ATTRIBUTE) && !item->IsSocketModifyingItem())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot change the upgrade of this item."));
				return false;
			}
			*/

			if (ITEM_COSTUME == item2->GetType())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� ������ �� ���� �������Դϴ�."));
				return false;
			}

			if (item2->IsOldHair() && item->IsCostumeModifyItem())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot change the upgrade of this item."));
				return false;
			}

#if defined(__SOUL_BIND_SYSTEM__)
			if (item2->IsSealed())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot change the attributes of a soulbound item."));
				return false;
			}
#endif

			if (item2->IsExchanging())
				return false;

			switch (item->GetSubType())
			{
			case USE_CLEAN_SOCKET:
			{
				int i;
				for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
				{
					if (item2->GetSocket(i) == ITEM_BROKEN_METIN_VNUM)
						break;
				}

				if (i == ITEM_SOCKET_MAX_NUM)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("û���� ���� �������� �ʽ��ϴ�."));
					return false;
				}

				int j = 0;

				for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
				{
					if (item2->GetSocket(i) != ITEM_BROKEN_METIN_VNUM && item2->GetSocket(i) != 0)
						item2->SetSocket(j++, item2->GetSocket(i));
				}

				for (; j < ITEM_SOCKET_MAX_NUM; ++j)
				{
					if (item2->GetSocket(j) > 0)
						item2->SetSocket(j, 1);
				}

				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CLEAN_SOCKET", buf);
				}

				if (!g_bUnlimitedCleanSocket)
					item->SetCount(item->GetCount() - 1);
			}
			break;

			case USE_CHANGE_ATTRIBUTE:
			case USE_CHANGE_ATTRIBUTE2:
			{
#if defined(__SOUL_BIND_SYSTEM__)
				if (item2->IsSealed())
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot change the attributes of a soulbound item."));
					return false;
				}
#endif

				//< 2020.08.07.Owsap - Block item attribute enchant.
				if (IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_ENCHANT))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Enchant Item cannot be used on this item."));
					return false;
				}
				//>

				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� ������ �� ���� �������Դϴ�."));
					return false;
				}

				if (item2->GetAttributeCount() == 0)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �Ӽ��� �����ϴ�."));
					return false;
				}

				if (g_bChangeItemAttrCycle)
				{
					if (GM_PLAYER == GetGMLevel() && false == test_server)
					{
						//
						// Event Flag �� ���� ������ ������ �Ӽ� ������ �� �ð����� ���� ����� �ð��� �귶���� �˻��ϰ�
						// �ð��� ����� �귶�ٸ� ���� �Ӽ����濡 ���� �ð��� ������ �ش�.
						//

						DWORD dwChangeItemAttrCycle = quest::CQuestManager::instance().GetEventFlag(msc_szChangeItemAttrCycleFlag);
						if (dwChangeItemAttrCycle < msc_dwDefaultChangeItemAttrCycle)
							dwChangeItemAttrCycle = msc_dwDefaultChangeItemAttrCycle;

						quest::PC* pPC = quest::CQuestManager::instance().GetPC(GetPlayerID());

						if (pPC)
						{
							DWORD dwNowMin = get_global_time() / 60;

							DWORD dwLastChangeItemAttrMin = pPC->GetFlag(msc_szLastChangeItemAttrFlag);

							if (dwLastChangeItemAttrMin + dwChangeItemAttrCycle > dwNowMin)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� �ٲ��� %d�� �̳����� �ٽ� ������ �� �����ϴ�.(%d �� ����)"),
									dwChangeItemAttrCycle, dwChangeItemAttrCycle - (dwNowMin - dwLastChangeItemAttrMin));
								return false;
							}

							pPC->SetFlag(msc_szLastChangeItemAttrFlag, dwNowMin);
						}
					}
				}

				if (item->GetSubType() == USE_CHANGE_ATTRIBUTE2)
				{
					int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
					{
						0, 0, 30, 40, 3
					};

					item2->ChangeAttribute(aiChangeProb);
				}
				else if (item->GetVnum() == 76014)
				{
					int aiChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
					{
						0, 10, 50, 39, 1
					};

					item2->ChangeAttribute(aiChangeProb);
				}
				else
				{
					// ����� Ư��ó��
					// ����� ���簡 �߰� �ȵɰŶ� �Ͽ� �ϵ� �ڵ���.
					if (item->GetVnum() == 71151 || item->GetVnum() == 76023)
					{
						if ((item2->GetType() == ITEM_WEAPON) || (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY))
						{
							bool bCanUse = true;
							for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
							{
								if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 40)
								{
									bCanUse = false;
									break;
								}
							}
							if (false == bCanUse)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �������� ���� ����� �Ұ����մϴ�."));
								break;
							}
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����� ���ʿ��� ��� �����մϴ�."));
							break;
						}
					}
					item2->ChangeAttribute();
				}

				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� �����Ͽ����ϴ�."));
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CHANGE_ATTRIBUTE", buf);
				}

				if (!g_bUnlimitedChangeAttributes)
					item->SetCount(item->GetCount() - 1);
			}
			break;

			case USE_ADD_ATTRIBUTE:
			{
				//< 2020.08.07.Owsap - Block item attribute reinforce.
				if (IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_REINFORCE))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Reinforce Item cannot be used on this item."));
					return false;
				}
				//>

				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� ������ �� ���� �������Դϴ�."));
					return false;
				}

				if (item2->GetAttributeCount() < 4)
				{
					// ���簡 Ư��ó��
					// ����� ���簡 �߰� �ȵɰŶ� �Ͽ� �ϵ� �ڵ���.
					if (item->GetVnum() == 71152 || item->GetVnum() == 76024)
					{
						if ((item2->GetType() == ITEM_WEAPON) || (item2->GetType() == ITEM_ARMOR && item2->GetSubType() == ARMOR_BODY))
						{
							bool bCanUse = true;
							for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
							{
								if (item2->GetLimitType(i) == LIMIT_LEVEL && item2->GetLimitValue(i) > 40)
								{
									bCanUse = false;
									break;
								}
							}
							if (false == bCanUse)
							{
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �������� ���� ����� �Ұ����մϴ�."));
								break;
							}
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����� ���ʿ��� ��� �����մϴ�."));
							break;
						}
					}

					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					if (number(1, 100) <= aiItemAttributeAddPercent[item2->GetAttributeCount()] || g_bNeverFailAddAttributes == true)
					{
						item2->AddAttribute();
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ� �߰��� �����Ͽ����ϴ�."));

						int iAddedIdx = item2->GetAttributeCount() - 1;
						LogManager::instance().ItemLog(
							GetPlayerID(),
							item2->GetAttributeType(iAddedIdx),
							item2->GetAttributeValue(iAddedIdx),
							item->GetID(),
							"ADD_ATTRIBUTE_SUCCESS",
							buf,
							GetDesc()->GetHostName(),
							item->GetOriginalVnum()
						);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ� �߰��� �����Ͽ����ϴ�."));
						LogManager::instance().ItemLog(this, item, "ADD_ATTRIBUTE_FAIL", buf);
					}

					if (!g_bUnlimitedAddAttributes)
						item->SetCount(item->GetCount() - 1);
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���̻� �� �������� �̿��Ͽ� �Ӽ��� �߰��� �� �����ϴ�."));
				}
			}
			break;

			case USE_ADD_ATTRIBUTE2:
			{
				//< 2020.08.07.Owsap - Block item attribute reinforce.
				if (IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_REINFORCE))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Reinforce Item cannot be used on this item."));
					return false;
				}
				//>

				// �ູ�� ����
				// �簡�񼭸� ���� �Ӽ��� 4�� �߰� ��Ų �����ۿ� ���ؼ� �ϳ��� �Ӽ��� �� �ٿ��ش�.
				if (item2->GetAttributeSetIndex() == -1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� ������ �� ���� �������Դϴ�."));
					return false;
				}

				// �Ӽ��� �̹� 4�� �߰� �Ǿ��� ���� �Ӽ��� �߰� �����ϴ�.
				if (item2->GetAttributeCount() == 4)
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					if (number(1, 100) <= aiItemAttributeAddPercent[item2->GetAttributeCount()] || g_bNeverFailAddAttributes == true)
					{
						item2->AddAttribute();
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ� �߰��� �����Ͽ����ϴ�."));

						int iAddedIdx = item2->GetAttributeCount() - 1;
						LogManager::instance().ItemLog(
							GetPlayerID(),
							item2->GetAttributeType(iAddedIdx),
							item2->GetAttributeValue(iAddedIdx),
							item->GetID(),
							"ADD_ATTRIBUTE2_SUCCESS",
							buf,
							GetDesc()->GetHostName(),
							item->GetOriginalVnum()
						);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ� �߰��� �����Ͽ����ϴ�."));
						LogManager::instance().ItemLog(this, item, "ADD_ATTRIBUTE2_FAIL", buf);
					}

					if (!g_bUnlimitedAddAttributes)
						item->SetCount(item->GetCount() - 1);
				}
				else if (item2->GetAttributeCount() == 5)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �̻� �� �������� �̿��Ͽ� �Ӽ��� �߰��� �� �����ϴ�."));
				}
				else if (item2->GetAttributeCount() < 4)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �簡�񼭸� �̿��Ͽ� �Ӽ��� �߰����� �ּ���."));
				}
				else
				{
					// wtf ?!
					sys_err("ADD_ATTRIBUTE2 : Item has wrong AttributeCount(%d)", item2->GetAttributeCount());
				}
			}
			break;

			case USE_ADD_ACCESSORY_SOCKET:
			{
				char buf[21];
				snprintf(buf, sizeof(buf), "%u", item2->GetID());

				if (item2->IsAccessoryForSocket())
				{
					if (item2->GetAccessorySocketMaxGrade() < ITEM_ACCESSORY_SOCKET_MAX_NUM)
					{
						if (number(1, 100) <= 50 || g_bNeverFailAccessorySocket == true)
						{
							item2->SetAccessorySocketMaxGrade(item2->GetAccessorySocketMaxGrade() + 1);
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ���������� �߰��Ǿ����ϴ�."));
							LogManager::instance().ItemLog(this, item, "ADD_SOCKET_SUCCESS", buf);
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �߰��� �����Ͽ����ϴ�."));
							LogManager::instance().ItemLog(this, item, "ADD_SOCKET_FAIL", buf);
						}

						if (!g_bUnlimitedAddAccessorySocket)
							item->SetCount(item->GetCount() - 1);
					}
					else
					{
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �׼��������� ���̻� ������ �߰��� ������ �����ϴ�."));
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� ���������� ������ �߰��� �� ���� �������Դϴ�."));
				}
			}
			break;

			case USE_PUT_INTO_BELT_SOCKET:
			case USE_PUT_INTO_ACCESSORY_SOCKET:
			{
				if (item2->IsAccessoryForSocket() && item->CanPutInto(item2))
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());

					if (item2->GetAccessorySocketGrade() < item2->GetAccessorySocketMaxGrade())
					{
						if (number(1, 100) <= aiAccessorySocketPutPct[item2->GetAccessorySocketGrade()] || g_bNeverFailAccessory == true)
						{
							item2->SetAccessorySocketGrade(item2->GetAccessorySocketGrade() + 1);
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �����Ͽ����ϴ�."));
							LogManager::instance().ItemLog(this, item, "PUT_SOCKET_SUCCESS", buf);
						}
						else
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �����Ͽ����ϴ�."));
							LogManager::instance().ItemLog(this, item, "PUT_SOCKET_FAIL", buf);
						}

						if (!g_bUnlimitedAddAccessory)
							item->SetCount(item->GetCount() - 1);
					}
					else
					{
						if (item2->GetAccessorySocketMaxGrade() == 0)
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���̾Ƹ��� �Ǽ������� ������ �߰��ؾ��մϴ�."));
						else if (item2->GetAccessorySocketMaxGrade() < ITEM_ACCESSORY_SOCKET_MAX_NUM)
						{
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �׼��������� ���̻� ������ ������ �����ϴ�."));
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���̾Ƹ��� ������ �߰��ؾ��մϴ�."));
						}
						else
							ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �׼��������� ���̻� ������ ������ �� �����ϴ�."));
					}
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� ������ �� �����ϴ�."));
				}
			}
			break;
			}

			if (item2->IsEquipped())
			{
				BuffOnAttr_AddBuffsFromItem(item2);
			}
		}
		break;

#if defined(__COSTUME_ATTR_SYSTEM__)
		case USE_CHANGE_COSTUME_ATTR:
		case USE_RESET_COSTUME_ATTR:
		{
			LPITEM item2;
			if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
				return false;

			if (item2->IsEquipped())
			{
				BuffOnAttr_RemoveBuffsFromItem(item2);
			}

			if (ITEM_COSTUME != item2->GetType())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� ������ �� ���� �������Դϴ�."));
				return false;
			}

			if (item2->IsExchanging())
				return false;

			if (item2->GetAttributeSetIndex() == -1)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� ������ �� ���� �������Դϴ�."));
				return false;
			}

			if (item2->GetAttributeCount() == 0)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �Ӽ��� �����ϴ�."));
				return false;
			}

			switch (item->GetSubType())
			{
			case USE_CHANGE_COSTUME_ATTR:
			{
				item2->ChangeAttribute();
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "CHANGE_COSTUME_ATTR", buf);
				}

				if (!g_bUnlimitedChangeCostumeAttributes)
					item->SetCount(item->GetCount() - 1);
			}
			break;
			case USE_RESET_COSTUME_ATTR:
			{
				item2->ClearAttribute();
				item2->AlterToMagicItem();
				//item2->AlterToMagicItem(number(40, 50), number(10, 15));
				{
					char buf[21];
					snprintf(buf, sizeof(buf), "%u", item2->GetID());
					LogManager::instance().ItemLog(this, item, "RESET_COSTUME_ATTR", buf);
				}

				if (!g_bUnlimitedResetCostumeAttributes)
					item->SetCount(item->GetCount() - 1);
			}
			break;
			}

			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�Ӽ��� �����Ͽ����ϴ�."));
		}
		break;
#endif
		// END_OF_ACCESSORY_REFINE & END_OF_ADD_ATTRIBUTES & END_OF_CHANGE_ATTRIBUTES

		case USE_CALL:
			AggregateMonster();
			if (!g_bUnlimitedCapeOfCourage)
				item->SetCount(item->GetCount() - 1);
			break;

		case USE_BAIT:
		{
			if (m_pkFishingEvent)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �߿� �̳��� ���Ƴ��� �� �����ϴ�."));
				return false;
			}

			LPITEM weapon = GetWear(WEAR_WEAPON);

			if (!weapon || weapon->GetType() != ITEM_ROD)
				return false;

			if (weapon->GetSocket(2))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� �����ִ� �̳��� ���� %s�� ����ϴ�."), item->GetLocaleName());
			}
			else
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ô뿡 %s�� �̳��� ����ϴ�."), item->GetLocaleName());
			}

			weapon->SetSocket(2, item->GetValue(0));
			item->SetCount(item->GetCount() - 1);
		}
		break;

		case USE_MOVE:
		case USE_TREASURE_BOX:
		case USE_MONEYBAG:
			break;

		case USE_AFFECT:
		{
#if defined(__ANTI_EXP_RING__)
			if (g_bAntiExpRing)
			{
				if (item->GetVnum() == ITEM_ANTI_EXP_RING)
				{
					if (!HasFrozenEXP())
					{
						FreezeEXP();
						item->Lock(true);
						item->SetSocket(1, true);
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have frozen your experience."));
					}
					else
					{
						UnfreezeEXP();
						item->Lock(false);
						item->SetSocket(1, false);
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have unfrozen your experience."));
					}
					break;
				}
			}
#endif

#if defined(__EXTENDED_BLEND_AFFECT__)
			for (int i = AFFECT_ENERGY; i <= AFFECT_PENETRATE; ++i)
			{
				if (FindAffect(i, aApplyInfo[item->GetValue(1)].bPointType))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
					return false;
				}
			}
#endif

			if (FindAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
			}
			else
			{
				// PC_BANG_ITEM_ADD
				if (item->IsPCBangItem() == true)
				{
					// PC ������ üũ�ؼ� ó��
					if (CPCBangManager::instance().IsPCBangIP(GetDesc()->GetHostName()) == false)
					{
						// PC ���� �ƴ�!
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� PC�濡���� ����� �� �ֽ��ϴ�."));
						return false;
					}
				}
				// END_PC_BANG_ITEM_ADD

				AddAffect(item->GetValue(0), aApplyInfo[item->GetValue(1)].bPointType, item->GetValue(2), 0, item->GetValue(3), 0, false);
				item->SetCount(item->GetCount() - 1);
			}
		}
		break;

		case USE_CREATE_STONE:
		{
			AutoGiveItem(number(28000, 28012));
			item->SetCount(item->GetCount() - 1);
		}
		break;

		// ���� ���� ��ų�� ������ ó��
		case USE_RECIPE:
		{
			LPITEM pSource1 = FindSpecifyItem(item->GetValue(1));
			DWORD dwSourceCount1 = item->GetValue(2);

			LPITEM pSource2 = FindSpecifyItem(item->GetValue(3));
			DWORD dwSourceCount2 = item->GetValue(4);

			if (dwSourceCount1 != 0)
			{
				if (pSource1 == NULL)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ ���� ��ᰡ �����մϴ�."));
					return false;
				}
			}

			if (dwSourceCount2 != 0)
			{
				if (pSource2 == NULL)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ ���� ��ᰡ �����մϴ�."));
					return false;
				}
			}

			if (pSource1 != NULL)
			{
				if (pSource1->GetCount() < dwSourceCount1)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���(%s)�� �����մϴ�."), pSource1->GetLocaleName());
					return false;
				}

				pSource1->SetCount(pSource1->GetCount() - dwSourceCount1);
			}

			if (pSource2 != NULL)
			{
				if (pSource2->GetCount() < dwSourceCount2)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���(%s)�� �����մϴ�."), pSource2->GetLocaleName());
					return false;
				}

				pSource2->SetCount(pSource2->GetCount() - dwSourceCount2);
			}

			LPITEM pBottle = FindSpecifyItem(50901);

			if (!pBottle || pBottle->GetCount() < 1)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� ���� ���ڸ��ϴ�."));
				return false;
			}

			pBottle->SetCount(pBottle->GetCount() - 1);

			if (number(1, 100) > item->GetValue(5))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ �����߽��ϴ�."));
				return false;
			}

			AutoGiveItem(item->GetValue(0));
		}
		break;
		}
	}
	break;

	case ITEM_METIN:
	{
		LPITEM item2;

		if (!IsValidItemPosition(DestCell) || !(item2 = GetItem(DestCell)))
			return false;

		if (item2->IsExchanging())
			return false;

		if (item2->IsEquipped())
			return false;

		//< 2020.08.07.Owsap - Block item add metin sock.
		if (IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_APPLY))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This Spirit Stone cannot be attached to this type of item."));
			return false;
		}
		//>

		if (item2->GetType() == ITEM_PICK) return false;
		if (item2->GetType() == ITEM_ROD) return false;

		int i;

		for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		{
			DWORD dwVnum;

			if ((dwVnum = item2->GetSocket(i)) <= 2)
				continue;

			TItemTable* p = ITEM_MANAGER::instance().GetTable(dwVnum);

			if (!p)
				continue;

			if (item->GetValue(5) == p->alValues[5])
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ ��ƾ���� ������ ������ �� �����ϴ�."));
				return false;
			}
		}

		if (item2->GetType() == ITEM_ARMOR)
		{
			if (!IS_SET(item->GetWearFlag(), WEARABLE_BODY) || !IS_SET(item2->GetWearFlag(), WEARABLE_BODY))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� ��ƾ���� ��� ������ �� �����ϴ�."));
				return false;
			}
		}
		else if (item2->GetType() == ITEM_WEAPON)
		{
			if (!IS_SET(item->GetWearFlag(), WEARABLE_WEAPON))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� ��ƾ���� ���⿡ ������ �� �����ϴ�."));
				return false;
			}
		}
		else
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �� �ִ� ������ �����ϴ�."));
			return false;
		}

		for (i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			if (item2->GetSocket(i) >= 1 && item2->GetSocket(i) <= 2 && item2->GetSocket(i) >= item->GetValue(2))
			{
				// �� Ȯ��
				if (number(1, 100) <= 30 || g_bNeverFailMetin == true)
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ƾ�� ������ �����Ͽ����ϴ�."));
					item2->SetSocket(i, item->GetVnum());
				}
				else
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ƾ�� ������ �����Ͽ����ϴ�."));
					item2->SetSocket(i, ITEM_BROKEN_METIN_VNUM);
				}

				LogManager::instance().ItemLog(this, item2, "SOCKET", item->GetName());
				ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (METIN)");
				break;
			}

		if (i == ITEM_SOCKET_MAX_NUM)
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �� �ִ� ������ �����ϴ�."));
	}
	break;

	case ITEM_AUTOUSE:
	case ITEM_MATERIAL:
	case ITEM_SPECIAL:
	case ITEM_TOOL:
	case ITEM_LOTTERY:
		break;

	case ITEM_TOTEM:
	{
		if (!item->IsEquipped())
			EquipItem(item);
	}
	break;

	case ITEM_BLEND:
	{
		// ���ο� ���ʵ�
		sys_log(0, "ITEM_BLEND!!");

		if (Blend_Item_find(item->GetVnum()))
		{
			int affect_type = AFFECT_BLEND;
			int apply_type = aApplyInfo[item->GetSocket(0)].bPointType;
			int apply_value = item->GetSocket(1);
			int apply_duration = item->GetSocket(2);

#if defined(__EXTENDED_BLEND_AFFECT__) && defined(__ITEM_SOCKET5__)
			if ((apply_duration != 0) && UseExtendedBlendAffect(item, affect_type, apply_type, apply_value, apply_duration))
			{
				item->Lock(true);
				item->SetSocket(3, true);
				item->StartBlendExpireEvent();
			}
			else
			{
				item->Lock(false);
				item->SetSocket(3, false);
				item->StopBlendExpireEvent();
			}
			break;
#endif

			if (FindAffect(affect_type, apply_type))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
			}
			else
			{
				if (FindAffect(AFFECT_EXP_BONUS_EURO_FREE, POINT_RESIST_MAGIC))
				{
					ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ȿ���� �ɷ� �ֽ��ϴ�."));
				}
				else
				{
#if defined(__BLEND_AFFECT__)
					if (SetBlendAffect(item))
					{
						AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, false);
						item->SetCount(item->GetCount() - 1);
					}
#else
					AddAffect(affect_type, apply_type, apply_value, 0, apply_duration, 0, false);
					item->SetCount(item->GetCount() - 1);
#endif
				}
			}
		}
	}
	break;

	case ITEM_EXTRACT:
	{
		LPITEM pDestItem = GetItem(DestCell);
		if (NULL == pDestItem)
			return false;

		switch (item->GetSubType())
		{
		case EXTRACT_DRAGON_SOUL:
		{
			if (pDestItem->IsDragonSoul())
				return DSManager::instance().PullOut(this, NPOS, pDestItem, item);
			return false;
		}
		case EXTRACT_DRAGON_HEART:
		{
			if (pDestItem->IsDragonSoul())
				return DSManager::instance().ExtractDragonHeart(this, pDestItem, item);
			return false;
		}
		default:
			return false;
		}
	}
	break;

#if defined(__GACHA_SYSTEM__)
	case ITEM_GACHA:
	{
		if (GiveItemFromSpecialItemGroup(item->GetVnum()))
		{
			if (item->GetSocket(0) > 1)
				item->SetSocket(0, item->GetSocket(0) - 1);
			else
				ITEM_MANAGER::instance().RemoveItem(item, "REMOVE (ITEM_GACHA)");
		}
	}
	break;
#endif

#if defined(__SOUL_SYSTEM__)
	case ITEM_SOUL:
	{
		BYTE bSoulGrade = static_cast<BYTE>(item->GetValue(1));
		DWORD dwPlayTimeBonus = 0;
		int iSoulAttacks = item->GetValue(2);
		DWORD dwBonusDuration = item->GetSocket(0);

		DWORD dwPlayTime = item->GetSocket(3);

		if (bSoulGrade == BASIC_SOUL)
		{
			if (dwPlayTime >= 60)
				dwPlayTimeBonus = item->GetValue(3);
		}
		else if (bSoulGrade == GLEAMING_SOUL)
		{
			if (dwPlayTime >= 60)
				dwPlayTimeBonus = item->GetValue(3);
		}
		else if (bSoulGrade == LUSTROUS_SOUL)
		{
			if (dwPlayTime >= 60 && dwPlayTime < 120)
				dwPlayTimeBonus = item->GetValue(3);
			else if (dwPlayTime >= 120)
				dwPlayTimeBonus = item->GetValue(4);
		}
		else if (bSoulGrade == PRISMATIC_SOUL)
		{
			if (dwPlayTime >= 60 && dwPlayTime < 120)
				dwPlayTimeBonus = item->GetValue(3);
			else if (dwPlayTime >= 120 && dwPlayTime < 180)
				dwPlayTimeBonus = item->GetValue(4);
			else if (dwPlayTime >= 180)
				dwPlayTimeBonus = item->GetValue(5);
		}
		else if (bSoulGrade == ILUMINED_SOUL)
		{
			if (dwPlayTime >= 60 && dwPlayTime < 120)
				dwPlayTimeBonus = item->GetValue(3);
			else if (dwPlayTime >= 120 && dwPlayTime < 180)
				dwPlayTimeBonus = item->GetValue(4);
			else if (dwPlayTime >= 180)
				dwPlayTimeBonus = item->GetValue(5);
		}
		else
		{
			break;
		}

		EAffectTypes type = AFFECT_NONE;
		EAffectBits flag = AFF_NONE;

		switch (item->GetSubType())
		{
		case RED_SOUL:
			type = AFFECT_SOUL_RED;
			flag = AFF_SOUL_RED;
			break;

		case BLUE_SOUL:
			type = AFFECT_SOUL_BLUE;
			flag = AFF_SOUL_BLUE;
			break;
		}

		if (AFFECT_NONE == type)
			break;

		CAffect* pAffect = FindAffect(type);
		CAffect* pAffectMixed = FindAffect(AFFECT_SOUL_MIX);

		if (pAffect == NULL || pAffectMixed == NULL)
		{
			EPointTypes bonus = POINT_NONE;

			if (type == AFFECT_SOUL_RED)
			{
				bonus = POINT_NORMAL_HIT_DAMAGE_BONUS;
			}
			else if (type == AFFECT_SOUL_BLUE)
			{
				bonus = POINT_SKILL_DAMAGE_BONUS;
			}

			AddAffect(type, bonus, dwPlayTimeBonus, item->GetID(), dwBonusDuration, 0, true, false);
			if (FindAffect(AFFECT_SOUL_RED) && FindAffect(AFFECT_SOUL_BLUE))
				AddAffect(AFFECT_SOUL_MIX, POINT_NONE, 0, AFF_SOUL_MIX, INFINITE_AFFECT_DURATION, 0, true, false);
			else
				AddAffect(AFFECT_SOUL_MIX, POINT_NONE, 0, flag, INFINITE_AFFECT_DURATION, 0, true, false);

			item->Lock(true);
			item->SetSocket(1, true);
		}
		else
		{
			if (item->GetID() == pAffect->dwFlag && flag == pAffectMixed->dwFlag)
			{
				RemoveAffect(pAffect);
				RemoveAffect(pAffectMixed);

				item->Lock(false);
				item->SetSocket(1, false);
			}
			else
			{
				LPITEM old = FindItemByID(pAffect->dwFlag);

				if (NULL != old)
				{
					old->Lock(false);
					old->SetSocket(1, false);
				}

				RemoveAffect(pAffect);

				if (FindAffect(AFFECT_SOUL_RED))
					AddAffect(AFFECT_SOUL_MIX, POINT_NONE, 0, AFF_SOUL_RED, INFINITE_AFFECT_DURATION, 0, true, false);
				else if (FindAffect(AFFECT_SOUL_BLUE))
					AddAffect(AFFECT_SOUL_MIX, POINT_NONE, 0, AFF_SOUL_BLUE, INFINITE_AFFECT_DURATION, 0, true, false);
				else
					RemoveAffect(pAffectMixed);

				item->Lock(false);
				item->SetSocket(1, false);
			}
		}
	}
	break;
#endif

	case ITEM_NONE:
		sys_err("Item type NONE %s", item->GetName());
		break;

	default:
		sys_log(0, "UseItemEx: Unknown type %s %d", item->GetName(), item->GetType());
		return false;
	}

	return true;
}

int g_nPortalLimitTime = 10;

bool CHARACTER::UseItem(TItemPos Cell, TItemPos DestCell)
{
	WORD wCell = Cell.cell;
	BYTE window_type = Cell.window_type;

	//WORD wDestCell = DestCell.cell;
	//BYTE bDestInven = DestCell.window_type;

	LPITEM item;

	if (!CanHandleItem())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	// We don't want to use it if we are dragging it over another item of the same type...
	LPITEM destItem = GetItem(DestCell);
	if (destItem && item != destItem && destItem->IsStackable() && !IS_SET(destItem->GetAntiFlag(), ITEM_ANTIFLAG_STACK) && destItem->GetVnum() == item->GetVnum())
	{
		if (MoveItem(Cell, DestCell, 0))
			return false;
	}

	sys_log(0, "%s: USE_ITEM %s (inven %d, cell: %d)", GetName(), item->GetName(), window_type, wCell);

	if (item->IsExchanging())
		return false;

	if (!item->CanUsedBy(this))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �����ʾ� �� �������� ����� �� �����ϴ�."));
		return false;
	}

	if (IsStun())
		return false;

	if (false == FN_check_item_sex(this, item))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �����ʾ� �� �������� ����� �� �����ϴ�."));
		return false;
	}

#if defined(__CHANGE_LOOK_SYSTEM__)
	DWORD dwTransmutationVnum = item->GetTransmutationVnum();
	if (dwTransmutationVnum != 0)
	{
		TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(dwTransmutationVnum);

		if (!pItemTable->CanUseByJob(GetJob()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You cannot equip the transmuted item as it does not match your class."));
			return false;
		}

#	if defined(__COSTUME_SYSTEM__)
		if (pItemTable && pItemTable->IsCostume())
		{
			if ((IS_SET(pItemTable->GetAntiFlags(), ITEM_ANTIFLAG_MALE) && SEX_MALE == GET_SEX(this)) ||
				(IS_SET(pItemTable->GetAntiFlags(), ITEM_ANTIFLAG_FEMALE) && SEX_FEMALE == GET_SEX(this)))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You cannot equip the transmuted item as it does not match your gender."));
				return false;
			}
		}
	}
#	endif
#endif

	// PREVENT_TRADE_WINDOW
	if (IS_SUMMON_ITEM(item->GetVnum()))
	{
		if (false == IS_SUMMONABLE_ZONE(GetMapIndex()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("����Ҽ� �����ϴ�."));
			return false;
		}

		// ��ȥ���� ����� ������ SUMMONABLE_ZONE�� �ִ°��� WarpToPC()���� üũ

		// ��Ÿ� ���� �ʿ����� ��ȯ�θ� ���ƹ�����.
		if (CThreeWayWar::instance().IsThreeWayWarMapIndex(GetMapIndex()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��Ÿ� ���� �����߿��� ��ȯ��,��ȯ���θ� ����Ҽ� �����ϴ�."));
			return false;
		}

		int iPulse = thecore_pulse();

		// â�� ���� üũ
		if (iPulse - GetSafeboxLoadTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("â�� ���� %d�� �̳����� ��ȯ��,��ȯ���θ� ����� �� �����ϴ�."), g_nPortalLimitTime);

			if (test_server)
				ChatPacket(CHAT_TYPE_INFO, "[TestOnly]Pulse %d LoadTime %d PASS %d", iPulse, GetSafeboxLoadTime(), PASSES_PER_SEC(g_nPortalLimitTime));
			return false;
		}

		// �ŷ����� â üũ
		if (PreventTradeWindow(WND_ALL))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ŷ�â,â�� ���� �� ���¿����� ��ȯ��,��ȯ���� �� ����Ҽ� �����ϴ�."));
			return false;
		}

		// PREVENT_REFINE_HACK
		// ������ �ð�üũ
		{
			if (iPulse - GetRefineTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ������ %d�� �̳����� ��ȯ��,��ȯ���θ� ����� �� �����ϴ�."), g_nPortalLimitTime);
				return false;
			}
		}
		// END_PREVENT_REFINE_HACK

		// PREVENT_ITEM_COPY
		{
			if (iPulse - GetMyShopTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���λ��� ����� %d�� �̳����� ��ȯ��,��ȯ���θ� ����� �� �����ϴ�."), g_nPortalLimitTime);
				return false;
			}

#if defined(__MAILBOX__)
			if (iPulse - GetMyMailBoxTime() < PASSES_PER_SEC(g_nPortalLimitTime))
			{
				ChatPacket(CHAT_TYPE_INFO, "You cannot use a Return Scroll %d seconds after opening a mailbox.", g_nPortalLimitTime);
				return false;
			}
#endif
		}
		// END_PREVENT_ITEM_COPY

		// ��ȯ�� �Ÿ�üũ
		if (item->GetVnum() != 70302)
		{
			PIXEL_POSITION posWarp;

			int x = 0;
			int y = 0;

			double nDist = 0;
			const double nDistant = 5000.0;

			// ��ȯ����
			if (item->GetVnum() == 22010)
			{
				x = item->GetSocket(0) - GetX();
				y = item->GetSocket(1) - GetY();
			}
			// ��ȯ��
			else if (item->GetVnum() == 22000)
			{
				SECTREE_MANAGER::instance().GetRecallPositionByEmpire(GetMapIndex(), GetEmpire(), posWarp);

				if (item->GetSocket(0) == 0)
				{
					x = posWarp.x - GetX();
					y = posWarp.y - GetY();
				}
				else
				{
					x = item->GetSocket(0) - GetX();
					y = item->GetSocket(1) - GetY();
				}
			}

			nDist = sqrt(pow((float)x, 2) + pow((float)y, 2));

			if (nDistant > nDist)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̵� �Ǿ��� ��ġ�� �ʹ� ����� ��ȯ�θ� ����Ҽ� �����ϴ�."));
				if (test_server)
					ChatPacket(CHAT_TYPE_INFO, "PossibleDistant %f nNowDist %f", nDistant, nDist);
				return false;
			}
		}

		// PREVENT_PORTAL_AFTER_EXCHANGE
		// ��ȯ �� �ð�üũ
		if (iPulse - GetExchangeTime() < PASSES_PER_SEC(g_nPortalLimitTime))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ŷ� �� %d�� �̳����� ��ȯ��,��ȯ���ε��� ����� �� �����ϴ�."), g_nPortalLimitTime);
			return false;
		}
		// END_PREVENT_PORTAL_AFTER_EXCHANGE
	}

	// ������ ��� ���� �ŷ�â ���� üũ
	if ((item->GetVnum() == 50200) || (item->GetVnum() == 71049))
	{
		if (PreventTradeWindow(WND_ALL))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ŷ�â,â�� ���� �� ���¿����� ������,��ܺ������� ����Ҽ� �����ϴ�."));
			return false;
		}
	}
	// END_PREVENT_TRADE_WINDOW

	//< 2020.08.08.Owsap - Prevent item usage while running quest.
	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

	if (IS_SET(item->GetFlag(), ITEM_FLAG_LOG)) // ��� �α׸� ����� ������ ó��
	{
		DWORD vid = item->GetVID();
		DWORD oldCount = item->GetCount();
		DWORD vnum = item->GetVnum();

		char hint[ITEM_NAME_MAX_LEN + 32 + 1];
		int len = snprintf(hint, sizeof(hint) - 32, "%s", item->GetName());

		if (len < 0 || len >= (int)sizeof(hint) - 32)
			len = (sizeof(hint) - 32) - 1;

		bool ret = UseItemEx(item, DestCell);

		if (NULL == ITEM_MANAGER::instance().FindByVID(vid)) // UseItemEx���� �������� ���� �Ǿ���. ���� �α׸� ����
		{
			LogManager::instance().ItemLog(this, vid, vnum, "REMOVE", hint);
		}
		else if (oldCount != item->GetCount())
		{
			snprintf(hint + len, sizeof(hint) - len, " %u", oldCount - 1);
			LogManager::instance().ItemLog(this, vid, vnum, "USE_ITEM", hint);
		}
		return (ret);
	}
	else
		return UseItemEx(item, DestCell);
}

bool CHARACTER::DropItem(TItemPos Cell, WORD wCount)
{
	LPITEM item = NULL;

	if (!CanHandleItem())
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ȭâ�� �� ���¿����� �������� �ű� �� �����ϴ�."));
		return false;
	}

	if (IsDead())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	if (item->IsExchanging())
		return false;

	if (true == item->isLocked())
		return false;

#if defined(__SOUL_BIND_SYSTEM__)
	if (item->IsSealed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot drop a soulbound item."));
		return false;
	}
#endif

	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DROP | ITEM_ANTIFLAG_GIVE))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �� ���� �������Դϴ�."));
		return false;
	}

	if (wCount == 0 || wCount > item->GetCount())
		wCount = item->GetCount();

	SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, WORD_MAX); // Quickslot ���� ����

	LPITEM pkItemToDrop;

	if (wCount == item->GetCount())
	{
		item->RemoveFromCharacter();
		pkItemToDrop = item;
	}
	else
	{
		if (wCount == 0)
		{
			if (test_server)
				sys_log(0, "[DROP_ITEM] drop item count == 0");
			return false;
		}

		item->SetCount(item->GetCount() - wCount);
		ITEM_MANAGER::instance().FlushDelayedSave(item);

		pkItemToDrop = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), wCount);

		// Copy item socket -- by mhh
		FN_copy_item_socket(pkItemToDrop, item);

		char szBuf[51 + 1];
		snprintf(szBuf, sizeof(szBuf), "%u %u", pkItemToDrop->GetID(), pkItemToDrop->GetCount());
		LogManager::instance().ItemLog(this, item, "ITEM_SPLIT", szBuf);
	}

	PIXEL_POSITION pxPos = GetXYZ();

	if (pkItemToDrop->AddToGround(GetMapIndex(), pxPos))
	{
		// �ѱ����� �������� ������ �����ش޶�� ������������ ���Ƽ�
		// �������� �ٴڿ� ���� �� �Ӽ��α׸� �����.
		if (LC_IsYMIR())
			item->AttrLog();

		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �������� 3�� �� ������ϴ�."));
		pkItemToDrop->StartDestroyEvent();

		ITEM_MANAGER::instance().FlushDelayedSave(pkItemToDrop);

		char szHint[32 + 1];
		snprintf(szHint, sizeof(szHint), "%s %u %u", pkItemToDrop->GetName(), pkItemToDrop->GetCount(), pkItemToDrop->GetOriginalVnum());
		LogManager::instance().ItemLog(this, pkItemToDrop, "DROP", szHint);
		//Motion(MOTION_PICKUP);
	}

	return true;
}

#if defined(__NEW_DROP_DIALOG__)
bool CHARACTER::DestroyItem(TItemPos Cell)
{
	LPITEM item = NULL;

	if (!CanHandleItem())
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ȭâ�� �� ���¿����� �������� �ű� �� �����ϴ�."));

		return false;
	}

	if (IsDead())
		return false;

	if (!IsValidItemPosition(Cell) || !(item = GetItem(Cell)))
		return false;

	if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DESTROY))
		return false;

	if (item->IsExchanging())
		return false;

	if (true == item->isLocked())
		return false;

#if defined(__SOUL_BIND_SYSTEM__)
	if (item->IsSealed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot destroy a soulbound item."));
		return false;
	}
#endif

	if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
		return false;

	if (item->GetCount() <= 0)
		return false;

	SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, WORD_MAX);
	ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You have deleted %s."), item->GetLocaleName());
	ITEM_MANAGER::instance().RemoveItem(item, "DESTROYED BY PLAYER");

	return true;
}
#endif

bool CHARACTER::DropGold(int gold)
{
	if (gold <= 0 || (long long)gold > GetGold())
		return false;

	if (!CanHandleItem())
		return false;

	if (0 != g_GoldDropTimeLimitValue)
	{
		if (get_dword_time() < m_dwLastGoldDropTime + g_GoldDropTimeLimitValue)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ��带 ���� �� �����ϴ�."));
			return false;
		}
	}

	m_dwLastGoldDropTime = get_dword_time();

	LPITEM item = ITEM_MANAGER::instance().CreateItem(1, gold);

	if (item)
	{
		PIXEL_POSITION pos = GetXYZ();

		if (item->AddToGround(GetMapIndex(), pos))
		{
			//Motion(MOTION_PICKUP);
			PointChange(POINT_GOLD, -gold, true);

			// ������� ���� �������ٴ� ���װ� �ִµ�,
			// ������ �ó����� �߿� �ϳ���,
			// ��ũ�γ�, ���� �Ἥ 1000�� ������ ���� ��� ���� ��带 0���� �����,
			// ���� �������ٰ� ���� ��û�ϴ� ���� ���� �ִ�.
			// ���� �׷� ��츦 ��� ���� ���� ��ġ�� ��忡 ���ؼ��� �α׸� ����.
			if (LC_IsBrazil() == true)
			{
				if (gold >= 213)
					LogManager::instance().CharLog(this, gold, "DROP_GOLD", "");
			}
			else
			{
				if (gold > 1000) // õ�� �̻� ����Ѵ�.
					LogManager::instance().CharLog(this, gold, "DROP_GOLD", "");
			}

			if (false == LC_IsBrazil())
			{
				item->StartDestroyEvent(150);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �������� %d�� �� ������ϴ�."), 150 / 60);
			}
			else
			{
				item->StartDestroyEvent(60);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �������� %d�� �� ������ϴ�."), 1);
			}
		}

		Save();
		return true;
	}

	return false;
}

#if defined(__CHEQUE_SYSTEM__)
bool CHARACTER::DropCheque(int cheque)
{
	if (cheque <= 0 || (long long)cheque > GetCheque())
		return false;

	if (!CanHandleItem())
		return false;

	if (0 != g_ChequeDropTimeLimitValue)
	{
		if (get_dword_time() < m_dwLastChequeDropTime + g_ChequeDropTimeLimitValue)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ��带 ���� �� �����ϴ�."));
			return false;
		}
	}

	m_dwLastChequeDropTime = get_dword_time();

	LPITEM item = ITEM_MANAGER::instance().CreateItem(80020, cheque);

	if (item)
	{
		PIXEL_POSITION pos = GetXYZ();

		if (item->AddToGround(GetMapIndex(), pos))
		{
			PointChange(POINT_CHEQUE, -cheque, true);

			if (cheque > 1000)
				LogManager::instance().CharLog(this, cheque, "DROP_CHEQUE", "");

			if (false == LC_IsBrazil())
			{
				item->StartDestroyEvent(150);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �������� %d�� �� ������ϴ�."), 150 / 60);
			}
			else
			{
				item->StartDestroyEvent(60);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �������� %d�� �� ������ϴ�."), 1);
			}
		}

		Save();
		return true;
	}

	return false;
}
#endif

bool CHARACTER::MoveItem(TItemPos Cell, TItemPos DestCell, WORD count)
{
	LPITEM item = NULL;

	// NOTE : Prevent the item from being moved to the original slot.
	if (Cell.cell == DestCell.cell)
		return false;

	if (!IsValidItemPosition(Cell))
		return false;

	if (!(item = GetItem(Cell)))
		return false;

	if (item->IsExchanging())
		return false;

	if (item->GetCount() < count)
		return false;

	if (INVENTORY == Cell.window_type && Cell.cell >= INVENTORY_MAX_NUM && IS_SET(item->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		return false;

	if (true == item->isLocked())
		return false;

	if (!IsValidItemPosition(DestCell))
		return false;

	if (!CanHandleItem())
	{
		if (NULL != DragonSoul_RefineWindow_GetOpener())
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ȭâ�� �� ���¿����� �������� �ű� �� �����ϴ�."));

		return false;
	}

	// ��ȹ���� ��û���� ��Ʈ �κ��丮���� Ư�� Ÿ���� �����۸� ���� �� �ִ�.
	if (DestCell.window_type != DRAGON_SOUL_INVENTORY && DestCell.IsBeltInventoryPosition() && !CBeltInventoryHelper::CanMoveIntoBeltInventory(item))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� ��Ʈ �κ��丮�� �ű� �� �����ϴ�."));
		return false;
	}

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	if (DestCell.IsSkillBookInventoryPosition() && (item->IsEquipped() || Cell.IsBeltInventoryPosition()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't move this item into this inventory."));
		return false;
	}

	if (DestCell.IsUpgradeItemsInventoryPosition() && (item->IsEquipped() || Cell.IsBeltInventoryPosition()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't move this item into this inventory."));
		return false;
	}

	if (DestCell.IsStoneInventoryPosition() && (item->IsEquipped() || Cell.IsBeltInventoryPosition()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't move this item into this inventory."));
		return false;
	}

	if (DestCell.IsGiftBoxInventoryPosition() && (item->IsEquipped() || Cell.IsBeltInventoryPosition()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't move this item into this inventory."));
		return false;
	}

	if ((Cell.IsSkillBookInventoryPosition() && !DestCell.IsSkillBookInventoryPosition() && !DestCell.IsDefaultInventoryPosition()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't move this item into this inventory."));
		return false;
	}

	if (Cell.IsUpgradeItemsInventoryPosition() && !DestCell.IsUpgradeItemsInventoryPosition() && !DestCell.IsDefaultInventoryPosition())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't move this item into this inventory."));
		return false;
	}

	if (Cell.IsStoneInventoryPosition() && !DestCell.IsStoneInventoryPosition() && !DestCell.IsDefaultInventoryPosition())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't move this item into this inventory."));
		return false;
	}

	if (Cell.IsGiftBoxInventoryPosition() && !DestCell.IsGiftBoxInventoryPosition() && !DestCell.IsDefaultInventoryPosition())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can't move this item into this inventory."));
		return false;
	}

	if (Cell.IsDefaultInventoryPosition() && DestCell.IsSkillBookInventoryPosition())
	{
		if (!item->IsSkillBook())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can only move skill books into this inventory."));
			return false;
		}
	}

	if (Cell.IsDefaultInventoryPosition() && DestCell.IsUpgradeItemsInventoryPosition())
	{
		if (!item->IsUpgradeItem())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can only move upgrade items into this inventory."));
			return false;
		}
	}

	if (Cell.IsDefaultInventoryPosition() && DestCell.IsStoneInventoryPosition())
	{
		if (!item->IsStone())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can only move stones into this inventory."));
			return false;
		}
	}

	if (Cell.IsDefaultInventoryPosition() && DestCell.IsGiftBoxInventoryPosition())
	{
		if (!item->IsGiftBox())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can only move chests into this inventory."));
			return false;
		}
	}
#endif

	// �̹� �������� �������� �ٸ� ������ �ű�� ���, '��å ����' ������ �� Ȯ���ϰ� �ű�
	if (Cell.IsEquipPosition())
	{
		if (!CanUnequipNow(item))
			return false;

#if defined(__WEAPON_COSTUME_SYSTEM__)
		int iWearCell = item->FindEquipCell(this);
		if (iWearCell == WEAR_WEAPON)
		{
			LPITEM pkCostumeWeapon = GetWear(WEAR_COSTUME_WEAPON);
			if (pkCostumeWeapon)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("If you want to change weapons, you must remove the weapon skin first."));
				return false;
			}
		}
#endif
	}

	if (item->IsBelt() && item->IsEquipped() && CBeltInventoryHelper::IsExistItemInBeltInventory(this))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You can only discard the belt when there are no longer any items in its inventory."));
		return false;

		/*
		if (item->IsEquipped())
			UnequipItem(item);

		if (!CBeltInventoryHelper::ClearBelt(this))
			return false;
		*/
	}

	if (DestCell.IsEquipPosition())
	{
		if (GetItem(DestCell)) // ����� ��� �� ���� �˻��ص� �ȴ�.
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� ��� �����ϰ� �ֽ��ϴ�."));
			return false;
		}

		EquipItem(item, DestCell.cell - INVENTORY_MAX_NUM);
	}
	else
	{
		if (item->IsDragonSoul())
		{
			if (item->IsEquipped())
			{
				return DSManager::instance().PullOut(this, DestCell, item);
			}
			else
			{
				if (DestCell.window_type != DRAGON_SOUL_INVENTORY)
				{
					return false;
				}

				if (!DSManager::instance().IsValidCellForThisItem(item, DestCell))
					return false;
			}
		}
		// ��ȥ���� �ƴ� �������� ��ȥ�� �κ��� �� �� ����.
		else if (DRAGON_SOUL_INVENTORY == DestCell.window_type)
			return false;

		LPITEM item2;

		if ((item2 = GetItem(DestCell)) && item != item2 && item2->IsStackable() &&
			!IS_SET(item2->GetAntiFlag(), ITEM_ANTIFLAG_STACK) &&
			item2->GetVnum() == item->GetVnum() && !item2->IsExchanging()) // ��ĥ �� �ִ� �������� ���
		{
#if defined(__EXTENDED_BLEND_AFFECT__)
			if (item->IsBlendItem() && item2->IsBlendItem())
			{
				if (count == 0)
					count = item->GetCount();

				for (int i = 0; i < 2; ++i)
					// 0 - apply_type
					// 1 - apply_value
					if (item2->GetSocket(i) != item->GetSocket(i))
						return false;

				item2->SetSocket(2, item2->GetSocket(2) + item->GetSocket(2));
				item->SetCount(item->GetCount() - count);
				return true;
			}
#endif

			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
				if (item2->GetSocket(i) != item->GetSocket(i))
					return false;

			if (count == 0)
				count = item->GetCount();

			sys_log(0, "%s: ITEM_STACK %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell,
				DestCell.window_type, DestCell.cell, count);

			count = MIN(gMaxItemCount - item2->GetCount(), count);

			item->SetCount(item->GetCount() - count);
			item2->SetCount(item2->GetCount() + count);
			return true;
		}

		if (!IsEmptyItemGrid(DestCell, item->GetSize(), Cell.cell))
		{
#if defined(__SWAP_ITEM_SYSTEM__)
			if (count != 0 && count != item->GetCount())
				return false;

			if (!DestCell.IsDefaultInventoryPosition() || !Cell.IsDefaultInventoryPosition())
				return false;

			LPITEM targetItem = GetItem_NEW(DestCell);
			if (targetItem && targetItem->GetVID() == item->GetVID())
				return false;

			if (targetItem)
			{
				DestCell = TItemPos(INVENTORY, targetItem->GetCell());
			}

			if (item->IsExchanging() || (targetItem && targetItem->IsExchanging()))
				return false;

			BYTE basePage = DestCell.cell / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT);
			std::map<WORD, LPITEM> moveItemMap;
			BYTE sizeLeft = item->GetSize();

			for (WORD i = 0; i < item->GetSize(); ++i)
			{
				WORD cellNumber = DestCell.cell + i * 5;

				BYTE cPage = cellNumber / (INVENTORY_MAX_NUM / INVENTORY_PAGE_COUNT);
				if (basePage != cPage)
					return false;

				LPITEM mvItem = GetItem(TItemPos(INVENTORY, cellNumber));
				if (mvItem)
				{
					if (mvItem->GetSize() > item->GetSize())
						return false;

					if (mvItem->IsExchanging())
						return false;

					moveItemMap.insert({ Cell.cell + i * 5, mvItem });
					sizeLeft -= mvItem->GetSize();

					if (mvItem->GetSize() > 1)
						i += mvItem->GetSize() - 1;
				}
				else
				{
					sizeLeft -= 1;
				}
			}

			if (sizeLeft != 0)
				return false;

			std::map<WORD, WORD> syncCells;

			syncCells.insert({ GetQuickslotPosition(QUICKSLOT_TYPE_ITEM, item->GetCell()), DestCell.cell });
			item->RemoveFromCharacter();

			for (auto it = moveItemMap.begin(); it != moveItemMap.end(); ++it)
			{
				WORD toCellNumber = it->first;
				LPITEM mvItem = it->second;

				syncCells.insert({ GetQuickslotPosition(QUICKSLOT_TYPE_ITEM, mvItem->GetCell()), toCellNumber });
				mvItem->RemoveFromCharacter();

				SetItem(TItemPos(INVENTORY, toCellNumber), mvItem);
			}

			SetItem(DestCell, item);

			for (auto& sCell : syncCells)
			{
				TQuickslot qs;
				qs.type = QUICKSLOT_TYPE_ITEM;
				qs.pos = sCell.second;

				SetQuickslot(sCell.first, qs);
			}

			return true;
#else
			return false;
#endif
		}

		if (count == 0 || count >= item->GetCount() || !item->IsStackable() || IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
		{
			sys_log(0, "%s: ITEM_MOVE %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell,
				DestCell.window_type, DestCell.cell, count);

			item->RemoveFromCharacter();
			SetItem(DestCell, item);

			if (INVENTORY == Cell.window_type && INVENTORY == DestCell.window_type)
				SyncQuickslot(QUICKSLOT_TYPE_ITEM, Cell.cell, DestCell.cell);
		}
		else if (count < item->GetCount())
		{
			sys_log(0, "%s: ITEM_SPLIT %s (window: %d, cell : %d) -> (window:%d, cell %d) count %d", GetName(), item->GetName(), Cell.window_type, Cell.cell, DestCell.window_type, DestCell.cell, count);

			item->SetCount(item->GetCount() - count);
			LPITEM item2 = ITEM_MANAGER::instance().CreateItem(item->GetVnum(), count);

			// Copy socket -- by mhh
			FN_copy_item_socket(item2, item);

			item2->AddToCharacter(this, DestCell);

			char szBuf[51 + 1];
			snprintf(szBuf, sizeof(szBuf), "%u %u %u %u ", item2->GetID(), item2->GetCount(), item->GetCount(), item->GetCount() + item2->GetCount());
			LogManager::instance().ItemLog(this, item, "ITEM_SPLIT", szBuf);
		}
	}

	return true;
}

namespace NPartyPickupDistribute
{
	struct FFindOwnership
	{
		LPITEM item;
		LPCHARACTER owner;

		FFindOwnership(LPITEM item)
			: item(item), owner(NULL)
		{
		}

		void operator () (LPCHARACTER ch)
		{
			if (item->IsOwnership(ch))
				owner = ch;
		}
	};

	struct FCountNearMember
	{
		int total;
		int x, y;

		FCountNearMember(LPCHARACTER center)
			: total(0), x(center->GetX()), y(center->GetY())
		{
		}

		void operator () (LPCHARACTER ch)
		{
			if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				total += 1;
		}
	};

	struct FMoneyDistributor
	{
		int total;
		LPCHARACTER c;
		int x, y;
		long long llMoney;

		FMoneyDistributor(LPCHARACTER center, long long llMoney)
			: total(0), c(center), x(center->GetX()), y(center->GetY()), llMoney(llMoney)
		{
		}

		void operator ()(LPCHARACTER ch)
		{
			if (ch != c)
				if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				{
					ch->PointChange(POINT_GOLD, llMoney, true);

					if (llMoney > 1000) // õ�� �̻� ����Ѵ�.
						LogManager::instance().CharLog(ch, llMoney, "GET_GOLD", "");
				}
		}
	};

#if defined(__CHEQUE_SYSTEM__)
	struct FChequeDistributor
	{
		int total;
		LPCHARACTER c;
		int x, y;
		int iCheque;

		FChequeDistributor(LPCHARACTER center, int iCheque)
			: total(0), c(center), x(center->GetX()), y(center->GetY()), iCheque(iCheque)
		{
		}

		void operator ()(LPCHARACTER ch)
		{
			if (ch != c)
				if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				{
					ch->PointChange(POINT_CHEQUE, iCheque, true);

					if (iCheque > 1000)
						LogManager::instance().CharLog(ch, iCheque, "GET_CHEQUE", "");
				}
		}
	};
#endif

#if defined(__GEM_SYSTEM__)
	struct FGemDistributor
	{
		int total;
		LPCHARACTER c;
		int x, y;
		int iGem;

		FGemDistributor(LPCHARACTER center, int iGem)
			: total(0), c(center), x(center->GetX()), y(center->GetY()), iGem(iGem)
		{
		}

		void operator ()(LPCHARACTER ch)
		{
			if (ch != c)
				if (DISTANCE_APPROX(ch->GetX() - x, ch->GetY() - y) <= PARTY_DEFAULT_RANGE)
				{
					ch->PointChange(POINT_GEM, iGem, true);

					if (iGem > 1000)
						LogManager::instance().CharLog(ch, iGem, "GET_GEM", "");
				}
		}
	};
#endif
}

void CHARACTER::GiveGold(long long llAmount)
{
	if (llAmount <= 0)
		return;

	sys_log(0, "GIVE_GOLD: %s %lld", GetName(), llAmount);

	if (GetParty())
	{
		LPPARTY pParty = GetParty();

		// ��Ƽ�� �ִ� ��� ������ ������.
		long long llTotal = llAmount;
		long long llMyAmount = llTotal;

		NPartyPickupDistribute::FCountNearMember funcCountNearMember(this);
		pParty->ForEachOnlineMember(funcCountNearMember);

		if (funcCountNearMember.total > 1)
		{
			long long llShare = llTotal / funcCountNearMember.total;
			llMyAmount -= llShare * (funcCountNearMember.total - 1);

			NPartyPickupDistribute::FMoneyDistributor funcMoneyDist(this, llShare);

			pParty->ForEachOnlineMember(funcMoneyDist);
		}

		PointChange(POINT_GOLD, llMyAmount, true);

		if (llMyAmount > 1000) // õ�� �̻� ����Ѵ�.
			LogManager::instance().CharLog(this, llMyAmount, "GET_GOLD", "");
	}
	else
	{
		PointChange(POINT_GOLD, llAmount, true);

		// ������� ���� �������ٴ� ���װ� �ִµ�,
		// ������ �ó����� �߿� �ϳ���,
		// ��ũ�γ�, ���� �Ἥ 1000�� ������ ���� ��� ���� ��带 0���� �����,
		// ���� �������ٰ� ���� ��û�ϴ� ���� ���� �ִ�.
		// ���� �׷� ��츦 ��� ���� ���� ��ġ�� ��忡 ���ؼ��� �α׸� ����.
		if (LC_IsBrazil() == true)
		{
			if (llAmount >= 213)
				LogManager::instance().CharLog(this, llAmount, "GET_GOLD", "");
		}
		else
		{
			if (llAmount > 1000) // õ�� �̻� ����Ѵ�.
				LogManager::instance().CharLog(this, llAmount, "GET_GOLD", "");
		}
	}
}

#if defined(__CHEQUE_SYSTEM__)
void CHARACTER::GiveCheque(int iAmount)
{
	if (iAmount <= 0)
		return;

	sys_log(0, "GIVE_CHEQUE: %s %lld", GetName(), iAmount);

	if (GetParty())
	{
		LPPARTY pParty = GetParty();

		DWORD dwTotal = iAmount;
		DWORD dwMyAmount = dwTotal;

		NPartyPickupDistribute::FCountNearMember funcCountNearMember(this);
		pParty->ForEachOnlineMember(funcCountNearMember);

		if (funcCountNearMember.total > 1)
		{
			DWORD dwShare = dwTotal / funcCountNearMember.total;
			dwMyAmount -= dwShare * (funcCountNearMember.total - 1);

			NPartyPickupDistribute::FChequeDistributor funcChequeDist(this, dwShare);

			pParty->ForEachOnlineMember(funcChequeDist);
		}

		PointChange(POINT_CHEQUE, dwMyAmount, true);

		if (dwMyAmount > 1000)
			LogManager::instance().CharLog(this, dwMyAmount, "GET_CHEQUE", "");
	}
	else
	{
		PointChange(POINT_CHEQUE, iAmount, true);

		if (LC_IsBrazil() == true)
		{
			if (iAmount >= 213)
				LogManager::instance().CharLog(this, iAmount, "GET_CHEQUE", "");
		}
		else
		{
			if (iAmount > 1000)
				LogManager::instance().CharLog(this, iAmount, "GET_CHEQUE", "");
		}
	}
}
#endif

#if defined(__GEM_SYSTEM__)
void CHARACTER::GiveGem(int iAmount)
{
	if (iAmount <= 0)
		return;

	sys_log(0, "GIVE_GEM: %s %lld", GetName(), iAmount);

	if (GetParty())
	{
		LPPARTY pParty = GetParty();

		DWORD dwTotal = iAmount;
		DWORD dwMyAmount = dwTotal;

		NPartyPickupDistribute::FCountNearMember funcCountNearMember(this);
		pParty->ForEachOnlineMember(funcCountNearMember);

		if (funcCountNearMember.total > 1)
		{
			DWORD dwShare = dwTotal / funcCountNearMember.total;
			dwMyAmount -= dwShare * (funcCountNearMember.total - 1);

			NPartyPickupDistribute::FGemDistributor funcGemDist(this, dwShare);

			pParty->ForEachOnlineMember(funcGemDist);
		}

		PointChange(POINT_GEM, dwMyAmount, true);

		if (dwMyAmount > 1000)
			LogManager::instance().CharLog(this, dwMyAmount, "GET_GEM", "");
	}
	else
	{
		PointChange(POINT_GEM, iAmount, true);

		if (LC_IsBrazil() == true)
		{
			if (iAmount >= 213)
				LogManager::instance().CharLog(this, iAmount, "GET_GEM", "");
		}
		else
		{
			if (iAmount > 1000)
				LogManager::instance().CharLog(this, iAmount, "GET_GEM", "");
		}
	}
}
#endif

bool CHARACTER::PickupItem(DWORD dwVID)
{
	LPITEM item = ITEM_MANAGER::instance().FindByVID(dwVID);

	if (IsObserverMode())
		return false;

	if (!item || !item->GetSectree())
		return false;

	if (item->DistanceValid(this))
	{
		if (item->GetType() == ITEM_QUEST)
			if (quest::CQuestManager::instance().GetPCForce(GetPlayerID())->IsRunning() == true)
				return false;

		if (item->IsOwnership(this))
		{
			// ���� ������ �ϴ� �������� ��ũ���
			if (item->GetType() == ITEM_ELK)
			{
				GiveGold(item->GetCount());
				item->RemoveFromGround();

				M2_DESTROY_ITEM(item);

				Save();
			}
#if defined(__CHEQUE_SYSTEM__)
			else if (item->GetType() == ITEM_CHEQUE)
			{
				GiveCheque(item->GetCount());
				item->RemoveFromGround();

				M2_DESTROY_ITEM(item);

				Save();
			}
#endif
			// ����� �������̶��
			else
			{
#if defined(__EXTENDED_BLEND_AFFECT__)
				if (item->IsBlendItem() && !item->IsExtendedBlend(item->GetVnum()))
				{
					WORD wCount = item->GetCount();

					for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetInventoryItem(i);

						if (!item2)
							continue;

						if (item2->IsBlendItem() && item->IsBlendItem())
						{
							bool bSameBlend = true;
							for (int j = 0; j < 2; ++j)
							{
								if (item2->GetSocket(j) != item->GetSocket(j))
								{
									bSameBlend = false;
									break;
								}
							}

							if (bSameBlend)
							{
								item2->SetSocket(2, item2->GetSocket(2) + item->GetSocket(2));
								item->SetCount(item->GetCount() - wCount);
								item->RemoveFromGround();

								M2_DESTROY_ITEM(item);
								return true;
							}
						}
					}
				}
				else if (item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
#else
				if (item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
#endif
				{
					WORD wCount = item->GetCount();

					for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
					{
						LPITEM item2 = GetInventoryItem(i);

						if (!item2)
							continue;

						if (item2->GetVnum() == item->GetVnum())
						{
							int j;

							for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
							{
								if (item2->GetSocket(j) != item->GetSocket(j))
									break;
							}

							if (j != ITEM_SOCKET_MAX_NUM)
								continue;

							WORD wCount2 = MIN(gMaxItemCount - item2->GetCount(), wCount);
							wCount -= wCount2;

							item2->SetCount(item2->GetCount() + wCount2);

							if (wCount == 0)
							{
#if defined(__CHATTING_WINDOW_RENEWAL__)
								ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item2->GetLocaleName());
#else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item2->GetLocaleName());
#endif
								M2_DESTROY_ITEM(item);

								//if (item2->GetType() == ITEM_QUEST)
								quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
								return true;
							}
						}
					}

					item->SetCount(wCount);
				}

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
				if ((item->IsSkillBook() || item->IsUpgradeItem() || item->IsStone() || item->IsGiftBox()) && item->IsStackable() && !IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_STACK))
				{
					WORD wCount = item->GetCount();

					for (int i = SKILL_BOOK_INVENTORY_SLOT_START; i < GIFT_BOX_INVENTORY_SLOT_END; ++i)
					{
						LPITEM item2 = GetInventoryItem(i);

						if (!item2)
							continue;

						if (item2->GetVnum() == item->GetVnum())
						{
							int j;

							for (j = 0; j < ITEM_SOCKET_MAX_NUM; ++j)
								if (item2->GetSocket(j) != item->GetSocket(j))
									break;

							if (j != ITEM_SOCKET_MAX_NUM)
								continue;

							WORD wCount2 = MIN(gMaxItemCount - item2->GetCount(), wCount);
							wCount -= wCount2;

							item2->SetCount(item2->GetCount() + wCount2);

							if (wCount == 0)
							{
#if defined(__CHATTING_WINDOW_RENEWAL__)
								ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item2->GetLocaleName());
#else
								ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item2->GetLocaleName());
#endif
								M2_DESTROY_ITEM(item);

								//if (item2->GetType() == ITEM_QUEST)
								quest::CQuestManager::instance().PickupItem(GetPlayerID(), item2);
								return true;
							}
						}
					}

					item->SetCount(wCount);
				}
#endif

				int iEmptyCell;
				if (item->IsDragonSoul())
				{
					if ((iEmptyCell = GetEmptyDragonSoulInventory(item)) == -1)
					{
						sys_log(0, "No empty ds inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
				else if (item->IsSkillBook())
				{
					if ((iEmptyCell = GetEmptySkillBookInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty skill book inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
				else if (item->IsUpgradeItem())
				{
					if ((iEmptyCell = GetEmptyUpgradeItemsInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty refine inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
				else if (item->IsStone())
				{
					if ((iEmptyCell = GetEmptyStoneInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty stone inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
				else if (item->IsGiftBox())
				{
					if ((iEmptyCell = GetEmptyGiftBoxInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty gift box inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
#endif
				else
				{
					if ((iEmptyCell = GetEmptyInventory(item->GetSize())) == -1)
					{
						sys_log(0, "No empty inventory pid %u size %ud itemid %u", GetPlayerID(), item->GetSize(), item->GetID());
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}

				item->RemoveFromGround();

				if (item->IsDragonSoul())
					item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
				else if (item->IsSkillBook())
				{
					int iOpenCount = GetQuestFlag("special_inventory.open_count");
					if (iOpenCount < 3)
					{
						ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("%s was added to your special inventory."), item->GetLocaleName());
						ChatPacket(CHAT_TYPE_COMMAND, "OpenSpecialInventoryWindow 0");
						SetQuestFlag("special_inventory.open_count", iOpenCount + 1);
					}
					item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
				}
				else if (item->IsUpgradeItem())
				{
					int iOpenCount = GetQuestFlag("special_inventory.open_count");
					if (iOpenCount < 3)
					{
						ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("%s was added to your special inventory."), item->GetLocaleName());
						ChatPacket(CHAT_TYPE_COMMAND, "OpenSpecialInventoryWindow 1");
						SetQuestFlag("special_inventory.open_count", iOpenCount + 1);
					}
					item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
				}
				else if (item->IsStone())
				{
					int iOpenCount = GetQuestFlag("special_inventory.open_count");
					if (iOpenCount < 3)
					{
						ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("%s was added to your special inventory."), item->GetLocaleName());
						ChatPacket(CHAT_TYPE_COMMAND, "OpenSpecialInventoryWindow 2");
						SetQuestFlag("special_inventory.open_count", iOpenCount + 1);
					}
					item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
				}
				else if (item->IsGiftBox())
				{
					int iOpenCount = GetQuestFlag("special_inventory.open_count");
					if (iOpenCount < 3)
					{
						ChatPacket(CHAT_TYPE_NOTICE, LC_TEXT("%s was added to your special inventory."), item->GetLocaleName());
						ChatPacket(CHAT_TYPE_COMMAND, "OpenSpecialInventoryWindow 3");
						SetQuestFlag("special_inventory.open_count", iOpenCount + 1);
					}
					item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
				}
#endif
				else
					item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));

				char szHint[32 + 1];
				snprintf(szHint, sizeof(szHint), "%s %u %u", item->GetName(), item->GetCount(), item->GetOriginalVnum());
				LogManager::instance().ItemLog(this, item, "GET", szHint);
#if defined(__CHATTING_WINDOW_RENEWAL__)
				ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#endif

				//if (item->GetType() == ITEM_QUEST)
				quest::CQuestManager::instance().PickupItem(GetPlayerID(), item);
			}

			//Motion(MOTION_PICKUP);
			return true;
		}
		//< 2020.06.26.Owsap - Suppressed the inability to pickup items from the party that are within the anti flags.
		else if (/*!IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_GIVE | ITEM_ANTIFLAG_DROP) &&*/ GetParty()) //< 2020.06.26.Owsap - />
		{
			// �ٸ� ��Ƽ�� ������ �������� �������� �Ѵٸ�
			NPartyPickupDistribute::FFindOwnership funcFindOwnership(item);

			GetParty()->ForEachOnlineMember(funcFindOwnership);

			LPCHARACTER owner = funcFindOwnership.owner;
			if (!owner)
				return false;

			int iEmptyCell;

			if (item->IsDragonSoul())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyDragonSoulInventory(item)) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyDragonSoulInventory(item)) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
			}
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
			else if (item->IsSkillBook())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptySkillBookInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptySkillBookInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
			}
			else if (item->IsUpgradeItem())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyUpgradeItemsInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyUpgradeItemsInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
			}
			else if (item->IsStone())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyStoneInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyStoneInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
			}
			else if (item->IsGiftBox())
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyGiftBoxInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyGiftBoxInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
			}
#endif
			else
			{
				if (!(owner && (iEmptyCell = owner->GetEmptyInventory(item->GetSize())) != -1))
				{
					owner = this;

					if ((iEmptyCell = GetEmptyInventory(item->GetSize())) == -1)
					{
						owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�����ϰ� �ִ� �������� �ʹ� �����ϴ�."));
						return false;
					}
				}
			}

			item->RemoveFromGround();

			if (item->IsDragonSoul())
				item->AddToCharacter(owner, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
			else if (item->IsSkillBook())
				item->AddToCharacter(owner, TItemPos(INVENTORY, iEmptyCell));
			else if (item->IsUpgradeItem())
				item->AddToCharacter(owner, TItemPos(INVENTORY, iEmptyCell));
			else if (item->IsStone())
				item->AddToCharacter(owner, TItemPos(INVENTORY, iEmptyCell));
			else if (item->IsGiftBox())
				item->AddToCharacter(owner, TItemPos(INVENTORY, iEmptyCell));
#endif
			else
				item->AddToCharacter(owner, TItemPos(INVENTORY, iEmptyCell));

			char szHint[32 + 1];
			snprintf(szHint, sizeof(szHint), "%s %u %u", item->GetName(), item->GetCount(), item->GetOriginalVnum());
			LogManager::instance().ItemLog(owner, item, "GET", szHint);

			if (owner == this)
#if defined(__CHATTING_WINDOW_RENEWAL__)
				ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#else
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#endif
			else
			{
#if defined(__CHATTING_WINDOW_RENEWAL__)
				owner->ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s �����κ��� %s"), GetName(), item->GetLocaleName());
				ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ����: %s �Կ��� %s"), owner->GetName(), item->GetLocaleName());
#else
				owner->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s �����κ��� %s"), GetName(), item->GetLocaleName());
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ����: %s �Կ��� %s"), owner->GetName(), item->GetLocaleName());
#endif
			}

			//if (item->GetType() == ITEM_QUEST)
			quest::CQuestManager::instance().PickupItem(owner->GetPlayerID(), item);

			return true;
		}
	}

	return false;
}

// �����Ƽ� ���� ��ũ��.. exp�� true�� msg�� ����ϰ� return false �ϴ� ��ũ�� (�Ϲ����� verify �뵵���� return ������ �ణ �ݴ�� �̸������� �򰥸� ���� �ְڴ�..)
#define VERIFY_MSG(exp, msg) \
	if (true == (exp)) \
	{ \
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT(msg)); \
		return false; \
	}

bool CHARACTER::SwapItem(UINT bCell, UINT bDestCell)
{
	if (!CanHandleItem())
		return false;

	TItemPos srcCell(INVENTORY, bCell), destCell(INVENTORY, bDestCell);

	// �ùٸ� Cell ���� �˻�
	// ��ȥ���� Swap�� �� �����Ƿ�, ���⼭ �ɸ�.
	//if (bCell >= INVENTORY_MAX_NUM + WEAR_MAX_NUM || bDestCell >= INVENTORY_MAX_NUM + WEAR_MAX_NUM)
	if (srcCell.IsDragonSoulEquipPosition() || destCell.IsDragonSoulEquipPosition())
		return false;

	// ���� CELL ���� �˻�
	if (bCell == bDestCell)
		return false;

	// �� �� ���â ��ġ�� Swap �� �� ����.
	if (srcCell.IsEquipPosition() && destCell.IsEquipPosition())
		return false;

	LPITEM item1, item2;

	// item2�� ���â�� �ִ� ���� �ǵ���.
	if (srcCell.IsEquipPosition())
	{
		item1 = GetInventoryItem(bDestCell);
		item2 = GetInventoryItem(bCell);
	}
	else
	{
		item1 = GetInventoryItem(bCell);
		item2 = GetInventoryItem(bDestCell);
	}

	if (!item1 || !item2)
		return false;

	if (item1 == item2)
	{
		sys_log(0, "[WARNING][WARNING][HACK USER!] : %s %d %d", m_stName.c_str(), bCell, bDestCell);
		return false;
	}

	if (ITEM_BELT == item1->GetType() || ITEM_BELT == item2->GetType())
	{
		if (!CBeltInventoryHelper::ClearBelt(this))
			return false;
	}

	// item2�� bCell��ġ�� �� �� �ִ��� Ȯ���Ѵ�.
	if (!IsEmptyItemGrid(TItemPos (INVENTORY, item1->GetCell()), item2->GetSize(), item1->GetCell()))
		return false;

	// �ٲ� �������� ���â�� ������
	if (TItemPos(EQUIPMENT, item2->GetCell()).IsEquipPosition())
	{
		int bEquipCell = item2->GetCell() - INVENTORY_MAX_NUM;
		UINT bInvenCell = item1->GetCell();

		// �������� �������� ���� �� �ְ�, ���� ���� �������� ���� ������ ���¿��߸� ����
		if (!CanUnequipNow(item2, TItemPos(INVENTORY, bInvenCell)) || !CanEquipNow(item1))
			return false;

		if (item2->IsDragonSoul() || item2->GetType() == ITEM_BELT)
		{
			if (false == CanUnequipNow(item2) || false == CanEquipNow(item1))
				return false;
		}

		if (item2->GetType() == ITEM_BELT && item2->IsEquipped() && CBeltInventoryHelper::IsExistItemInBeltInventory(this))
		{
			if (!CBeltInventoryHelper::ClearBelt(this))
				return false;
		}

		if (item1->GetType() == ITEM_BELT && item1->IsEquipped() && CBeltInventoryHelper::IsExistItemInBeltInventory(this))
		{
			if (!CBeltInventoryHelper::ClearBelt(this))
				return false;
		}

		if (bEquipCell != item1->FindEquipCell(this)) // ���� ��ġ�϶��� ���
			return false;

		item2->RemoveFromCharacter();

		if (item1->EquipTo(this, bEquipCell))
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item2->AddToCharacter(this, TItemPos(INVENTORY, bInvenCell), false);
#else
			item2->AddToCharacter(this, TItemPos(INVENTORY, bInvenCell));
#endif
		else
			sys_err("SwapItem cannot equip %s! item1 %s", item2->GetName(), item1->GetName());
	}
	else
	{
		UINT bCell1 = item1->GetCell();
		UINT bCell2 = item2->GetCell();

		item1->RemoveFromCharacter();
		item2->RemoveFromCharacter();

#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		item1->AddToCharacter(this, TItemPos(INVENTORY, bCell2), false);
		item2->AddToCharacter(this, TItemPos(INVENTORY, bCell1), false);
#else
		item1->AddToCharacter(this, TItemPos(INVENTORY, bCell2));
		item2->AddToCharacter(this, TItemPos(INVENTORY, bCell1));
#endif
	}

	return true;
}

bool CHARACTER::UnequipItem(LPITEM item)
{
#if defined(__WEAPON_COSTUME_SYSTEM__)
	if (item->FindEquipCell(this) == WEAR_WEAPON)
	{
		LPITEM pCostumeWeapon = GetWear(WEAR_COSTUME_WEAPON);
		if (pCostumeWeapon)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("If you want to change weapons, you must remove the weapon skin first."));
			return false;
		}
	}
#endif

	if (false == CanUnequipNow(item))
		return false;

	if (item->GetType() == ITEM_BELT && item->IsEquipped() && CBeltInventoryHelper::IsExistItemInBeltInventory(this))
	{
		if (!CBeltInventoryHelper::ClearBelt(this))
			return false;
	}

#if defined(__MOUNT_COSTUME_SYSTEM__)
	if (item->IsCostumeMount() && item->IsEquipped())
		UnMount();
#endif

	int pos;
	if (item->IsDragonSoul())
		pos = GetEmptyDragonSoulInventory(item);
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	else if (item->IsSkillBook())
		pos = GetEmptySkillBookInventory(item->GetSize());
	else if (item->IsUpgradeItem())
		pos = GetEmptyUpgradeItemsInventory(item->GetSize());
	else if (item->IsStone())
		pos = GetEmptyStoneInventory(item->GetSize());
	else if (item->IsGiftBox())
		pos = GetEmptyGiftBoxInventory(item->GetSize());
#endif
	else
		pos = GetEmptyInventory(item->GetSize());

	// HARD CODING
	if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		ShowAlignment(true);

	item->RemoveFromCharacter();
	if (item->IsDragonSoul())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, pos));
#endif
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	else if (item->IsSkillBook())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		item->AddToCharacter(this, TItemPos(INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(INVENTORY, pos));
#endif
	else if (item->IsUpgradeItem())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		item->AddToCharacter(this, TItemPos(INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(INVENTORY, pos));
#endif
	else if (item->IsStone())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		item->AddToCharacter(this, TItemPos(INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(INVENTORY, pos));
#endif
	else if (item->IsGiftBox())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		item->AddToCharacter(this, TItemPos(INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(INVENTORY, pos));
#endif
#endif
	else
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		item->AddToCharacter(this, TItemPos(INVENTORY, pos), false);
#else
		item->AddToCharacter(this, TItemPos(INVENTORY, pos));
#endif

	CheckMaximumPoints();

	return true;
}

//
// @version 05/07/05 Bang2ni - Skill ����� 1.5 �� �̳��� ��� ���� ����
//
bool CHARACTER::EquipItem(LPITEM item, int iCandidateCell)
{
	if (item->IsExchanging())
		return false;

	if (false == item->IsEquipable())
		return false;

	if (false == CanEquipNow(item))
		return false;

	int iWearCell = item->FindEquipCell(this, iCandidateCell);

	if (iWearCell < 0)
		return false;

	// ���𰡸� ź ���¿��� �νõ� �Ա� ����
	if (iWearCell == WEAR_BODY && IsRiding() && (item->GetVnum() >= 11901 && item->GetVnum() <= 11904))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ź ���¿��� ������ ���� �� �����ϴ�."));
		return false;
	}

	if (iWearCell != WEAR_ARROW && IsPolymorphed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�а� �߿��� �������� ��� ������ �� �����ϴ�."));
		return false;
	}

	if (FN_check_item_sex(this, item) == false)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �����ʾ� �� �������� ����� �� �����ϴ�."));
		return false;
	}

#if defined(__CHANGE_LOOK_SYSTEM__)
	DWORD dwTransmutationVnum = item->GetTransmutationVnum();
	if (dwTransmutationVnum != 0)
	{
		TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(dwTransmutationVnum);

		if (!pItemTable->CanUseByJob(GetJob()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You cannot equip the transmuted item as it does not match your class."));
			return false;
		}

#	if defined(__COSTUME_SYSTEM__)
		if (pItemTable && pItemTable->IsCostume())
		{
			if ((IS_SET(pItemTable->GetAntiFlags(), ITEM_ANTIFLAG_MALE) && SEX_MALE == GET_SEX(this)) ||
				(IS_SET(pItemTable->GetAntiFlags(), ITEM_ANTIFLAG_FEMALE) && SEX_FEMALE == GET_SEX(this)))
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("[Transmutation] You cannot equip the transmuted item as it does not match your gender."));
				return false;
			}
		}
	}
#	endif
#endif

	// �ű� Ż�� ���� ���� �� ��뿩�� üũ
	if (item->IsRideItem())
	{
		if (IsRiding())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� Ż���� �̿����Դϴ�."));
			return false;
		}

		if (IsPolymorphed())
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���¿����� ���� Ż �� �����ϴ�."));
			return false;
		}

#if defined(__MOUNT_COSTUME_SYSTEM__)
		if (item->IsRideItem())
		{
			DWORD dwMountVnum = item->GetMountVnum();
			if (dwMountVnum > 0)
				MountVnum(dwMountVnum);
		}
#endif
	}

	// ȭ�� �̿ܿ��� ������ ���� �ð� �Ǵ� ��ų ��� 1.5 �Ŀ� ��� ��ü�� ����
	DWORD dwCurTime = get_dword_time();

#if defined(__MOUNT_COSTUME_SYSTEM__)
	if (iWearCell != WEAR_ARROW && iWearCell != WEAR_COSTUME_MOUNT
		&& (dwCurTime - GetLastAttackTime() <= 1500 || dwCurTime - m_dwLastSkillTime <= 1500))
#else
	if (iWearCell != WEAR_ARROW
		&& (dwCurTime - GetLastAttackTime() <= 1500 || dwCurTime - m_dwLastSkillTime <= 1500))
#endif
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ���� ���� ������ �� �ֽ��ϴ�."));
		return false;
	}

#if defined(__WEAPON_COSTUME_SYSTEM__)
	if (iWearCell == WEAR_WEAPON)
	{
		if (item->GetType() == ITEM_WEAPON)
		{
			LPITEM pkCostumeWeapon = GetWear(WEAR_COSTUME_WEAPON);
			if (pkCostumeWeapon && pkCostumeWeapon->GetValue(3) != item->GetSubType())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("If you want to change weapons, you must remove the weapon skin first."));
				return false;
			}
		}
		else
		{
			LPITEM pkCostumeWeapon = GetWear(WEAR_COSTUME_WEAPON);
			if (pkCostumeWeapon)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("If you want to change weapons, you must remove the weapon skin first."));
				return false;
			}
		}
	}
	else if (iWearCell == WEAR_COSTUME_WEAPON)
	{
		if (item->GetType() == ITEM_COSTUME && item->GetSubType() == COSTUME_WEAPON)
		{
			LPITEM pkWeapon = GetWear(WEAR_WEAPON);
			if (!pkWeapon)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You need to equip a weapon first."));
				return false;
			}
			else if (pkWeapon->GetType() != ITEM_WEAPON || item->GetValue(3) != pkWeapon->GetSubType())
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot use this costume for this weapon."));
				return false;
			}
		}
	}
#endif

	// ��ȥ�� Ư�� ó��
	if (item->IsDragonSoul())
	{
		// ���� Ÿ���� ��ȥ���� �̹� �� �ִٸ� ������ �� ����.
		// ��ȥ���� swap�� �����ϸ� �ȵ�.
		if (GetInventoryItem(INVENTORY_MAX_NUM + iWearCell))
		{
			ChatPacket(CHAT_TYPE_INFO, "�̹� ���� ������ ��ȥ���� �����ϰ� �ֽ��ϴ�.");
			return false;
		}

		if (!item->EquipTo(this, iWearCell))
		{
			return false;
		}
	}
	// ��ȥ���� �ƴ�.
	else
	{
		// ������ ���� �������� �ִٸ�,
		if (GetWear(iWearCell) && !IS_SET(GetWear(iWearCell)->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		{
			// �� �������� �ѹ� ������ ���� �Ұ�. swap ���� ���� �Ұ�
			if (item->GetWearFlag() == WEARABLE_ABILITY)
				return false;

			if (!SwapItem((WORD)item->GetCell(), (WORD)(INVENTORY_MAX_NUM + iWearCell)))
				return false;
		}
		else
		{
			BYTE bOldCell = item->GetCell();

			if (item->EquipTo(this, iWearCell))
			{
				SyncQuickslot(QUICKSLOT_TYPE_ITEM, bOldCell, iWearCell);
			}
		}
	}

	if (true == item->IsEquipped())
	{
		// ������ ���� ��� ���ĺ��ʹ� ������� �ʾƵ� �ð��� �����Ǵ� ��� ó��.
		if (-1 != item->GetProto()->cLimitRealTimeFirstUseIndex)
		{
			// �� ���̶� ����� ���������� ���δ� Socket1�� ���� �Ǵ��Ѵ�. (Socket1�� ���Ƚ�� ���)
			if (0 == item->GetSocket(1))
			{
				// ��밡�ɽð��� Default ������ Limit Value ���� ����ϵ�, Socket0�� ���� ������ �� ���� ����ϵ��� �Ѵ�. (������ ��)
				long duration = (0 != item->GetSocket(0)) ? item->GetSocket(0) : item->GetProto()->aLimits[(BYTE)item->GetProto()->cLimitRealTimeFirstUseIndex].lValue;

				if (0 == duration)
					duration = 60 * 60 * 24 * 7;

				item->SetSocket(0, time(0) + duration);
				item->StartRealTimeExpireEvent();
			}

			item->SetSocket(1, item->GetSocket(1) + 1);
		}

		if (item->GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
			ShowAlignment(false);

		if (item->IsRing() || item->IsCostume() && !item->IsCostumeMount())
			ChatPacket(CHAT_TYPE_COMMAND, "OpenCostumeWindow");

		const DWORD& dwVnum = item->GetVnum();

		// �󸶴� �̺�Ʈ �ʽ´��� ����(71135) ����� ����Ʈ �ߵ�
		if (true == CItemVnumHelper::IsRamadanMoonRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_RAMADAN_RING);
		}
		// �ҷ��� ����(71136) ����� ����Ʈ �ߵ�
		else if (true == CItemVnumHelper::IsHalloweenCandy(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HALLOWEEN_CANDY);
		}
		// �ູ�� ����(71143) ����� ����Ʈ �ߵ�
		else if (true == CItemVnumHelper::IsHappinessRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_HAPPINESS_RING);
		}
		// ����� �Ҵ�Ʈ(71145) ����� ����Ʈ �ߵ�
		else if (true == CItemVnumHelper::IsLovePendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_LOVE_PENDANT);
		}
		// Magic Ring ( 71148 | 71149 )
		else if (true == CItemVnumHelper::IsMagicRing(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_MAGIC_RING);
		}
		// Easter Candy (71188)
		else if (true == CItemVnumHelper::IsEasterCandy(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_EASTER_CANDY);
		}
		// Chocolate Pendant (71199)
		else if (true == CItemVnumHelper::IsChocolatePendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_CHOCOLATE_PENDANT);
		}
		// Nazar Pendant (71202)
		else if (true == CItemVnumHelper::IsNazarPendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_NAZAR_PENDANT);
		}
		// Guardian Pendant (72054)
		else if (true == CItemVnumHelper::IsGuardianPendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_GUARDIAN_PENDANT);
		}
		// Gem Pendant (#) UNUSED
		else if (true == CItemVnumHelper::IsGemCandy(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_GEM_PENDANT);
		}
		// ITEM_UNIQUE�� ���, SpecialItemGroup�� ���ǵǾ� �ְ�, (item->GetSIGVnum() != NULL)
		//
		else if ((ITEM_UNIQUE == item->GetType() || ITEM_RING == item->GetType()) && 0 != item->GetSIGVnum())
		{
			const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(item->GetSIGVnum());
			if (NULL != pGroup)
			{
				const CSpecialAttrGroup* pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(pGroup->GetAttrVnum(item->GetVnum()));
				if (NULL != pAttrGroup)
				{
					const std::string& std = pAttrGroup->m_stEffectFileName;
					SpecificEffectPacket(std.c_str());
				}
			}
		}
#if defined(__ACCE_COSTUME_SYSTEM__)
		else if ((item->GetType() == ITEM_COSTUME) && (item->GetSubType() == COSTUME_ACCE))
			this->EffectPacket(SE_EQUIP_ACCE);
#endif

		if (
			(ITEM_UNIQUE == item->GetType() && UNIQUE_SPECIAL_RIDE == item->GetSubType() && IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE)) ||
			(ITEM_UNIQUE == item->GetType() && UNIQUE_SPECIAL_MOUNT_RIDE == item->GetSubType() && IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_USE))
#if defined(__MOUNT_COSTUME_SYSTEM__)
			|| (ITEM_COSTUME == item->GetType() && COSTUME_MOUNT == item->GetSubType())
#endif
			)
		{
			quest::CQuestManager::instance().UseItem(GetPlayerID(), item, false);
		}
	}

	return true;
}

void CHARACTER::BuffOnAttr_AddBuffsFromItem(LPITEM pItem)
{
	for (int i = 0; i < sizeof(g_aBuffOnAttrPoints) / sizeof(g_aBuffOnAttrPoints[0]); i++)
	{
		TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(g_aBuffOnAttrPoints[i]);
		if (it != m_map_buff_on_attrs.end())
		{
			it->second->AddBuffFromItem(pItem);
		}
	}
}

void CHARACTER::BuffOnAttr_RemoveBuffsFromItem(LPITEM pItem)
{
	for (int i = 0; i < sizeof(g_aBuffOnAttrPoints) / sizeof(g_aBuffOnAttrPoints[0]); i++)
	{
		TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(g_aBuffOnAttrPoints[i]);
		if (it != m_map_buff_on_attrs.end())
		{
			it->second->RemoveBuffFromItem(pItem);
		}
	}
}

void CHARACTER::BuffOnAttr_ClearAll()
{
	for (TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.begin(); it != m_map_buff_on_attrs.end(); it++)
	{
		CBuffOnAttributes* pBuff = it->second;
		if (pBuff)
		{
			pBuff->Initialize();
		}
	}
}

void CHARACTER::BuffOnAttr_ValueChange(BYTE bType, BYTE bOldValue, BYTE bNewValue)
{
	TMapBuffOnAttrs::iterator it = m_map_buff_on_attrs.find(bType);

	if (0 == bNewValue)
	{
		if (m_map_buff_on_attrs.end() == it)
			return;
		else
			it->second->Off();
	}
	else if (0 == bOldValue)
	{
		CBuffOnAttributes* pBuff;
		if (m_map_buff_on_attrs.end() == it)
		{
			switch (bType)
			{
			case POINT_ENERGY:
			{
				static BYTE abSlot[] = {
					WEAR_BODY,
					WEAR_HEAD,
					WEAR_FOOTS,
					WEAR_WRIST,
					WEAR_WEAPON,
					WEAR_NECK,
					WEAR_EAR,
					WEAR_SHIELD
				};
				static std::vector <BYTE> vec_slots(abSlot, abSlot + _countof(abSlot));
				pBuff = M2_NEW CBuffOnAttributes(this, bType, &vec_slots);
			}
			break;

			case POINT_COSTUME_ATTR_BONUS:
			{
				static BYTE abSlot[] = {
					WEAR_COSTUME_BODY,
					WEAR_COSTUME_HAIR,
#if defined(__WEAPON_COSTUME_SYSTEM__)
					WEAR_COSTUME_WEAPON,
#endif
				};
				static std::vector <BYTE> vec_slots(abSlot, abSlot + _countof(abSlot));
				pBuff = M2_NEW CBuffOnAttributes(this, bType, &vec_slots);
			}
			break;

			default:
				break;

			}
			m_map_buff_on_attrs.insert(TMapBuffOnAttrs::value_type(bType, pBuff));

		}
		else
			pBuff = it->second;

		pBuff->On(bNewValue);
	}
	else
	{
		if (m_map_buff_on_attrs.end() == it)
			return;
		else
			it->second->ChangeBuffValue(bNewValue);
	}
}

LPITEM CHARACTER::FindSpecifyItem(DWORD vnum) const
{
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		if (GetInventoryItem(i) && GetInventoryItem(i)->GetVnum() == vnum)
			return GetInventoryItem(i);

	return NULL;
}

LPITEM CHARACTER::FindItemByID(DWORD id) const
{
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (NULL != GetInventoryItem(i) && GetInventoryItem(i)->GetID() == id)
			return GetInventoryItem(i);
	}

	for (int i = BELT_INVENTORY_SLOT_START; i < BELT_INVENTORY_SLOT_END; ++i)
	{
		if (NULL != GetInventoryItem(i) && GetInventoryItem(i)->GetID() == id)
			return GetInventoryItem(i);
	}

	return NULL;
}

int CHARACTER::CountSpecifyItem(DWORD vnum, int iExceptionCell) const
{
	int count = 0;
	LPITEM item;

	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (i == iExceptionCell)
			continue;

		item = GetInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			// ���� ������ ��ϵ� �����̸� �Ѿ��.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}

	for (int i = 0; i < DRAGON_SOUL_INVENTORY_MAX_NUM; ++i)
	{
		item = GetDragonSoulItem(i);
		if (item && item->GetVnum() == vnum)
		{
			count += item->GetCount();
		}
	}

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	for (int i = SKILL_BOOK_INVENTORY_SLOT_START; i < GIFT_BOX_INVENTORY_SLOT_END; ++i)
	{
		item = GetInventoryItem(i);
		if (NULL != item && item->GetVnum() == vnum)
		{
			// ���� ������ ��ϵ� �����̸� �Ѿ��.
			if (m_pkMyShop && m_pkMyShop->IsSellingItem(item->GetID()))
			{
				continue;
			}
			else
			{
				count += item->GetCount();
			}
		}
	}
#endif

	return count;
}

void CHARACTER::RemoveSpecifyItem(DWORD vnum, DWORD count, int iExceptionCell)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (i == iExceptionCell)
			continue;

		if (NULL == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetVnum() != vnum)
			continue;

		//���� ������ ��ϵ� �����̸� �Ѿ��. (���� �������� �Ǹŵɶ� �� �κ����� ���� ��� ����!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}

	for (UINT i = 0; i < DRAGON_SOUL_INVENTORY_MAX_NUM; ++i)
	{
		if (NULL == GetDragonSoulItem(i))
			continue;

		if (GetDragonSoulItem(i)->GetVnum() != vnum)
			continue;

		if (count >= GetDragonSoulItem(i)->GetCount())
		{
			count -= GetDragonSoulItem(i)->GetCount();
			GetDragonSoulItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetDragonSoulItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	for (UINT i = SKILL_BOOK_INVENTORY_SLOT_START; i < GIFT_BOX_INVENTORY_SLOT_END; ++i)
	{
		if (NULL == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetVnum() != vnum)
			continue;

		//���� ������ ��ϵ� �����̸� �Ѿ��. (���� �������� �Ǹŵɶ� �� �κ����� ���� ��� ����!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (vnum >= 80003 && vnum <= 80007)
			LogManager::instance().GoldBarLog(GetPlayerID(), GetInventoryItem(i)->GetID(), QUEST, "RemoveSpecifyItem");

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#endif

	// ����ó���� ���ϴ�.
	if (count)
		sys_log(0, "CHARACTER::RemoveSpecifyItem cannot remove enough item vnum %u, still remain %d", vnum, count);
}

int CHARACTER::CountSpecifyTypeItem(BYTE type) const
{
	int count = 0;

	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pItem = GetInventoryItem(i);
		if (pItem != NULL && pItem->GetType() == type)
		{
			count += pItem->GetCount();
		}
	}

	return count;
}

void CHARACTER::RemoveSpecifyTypeItem(BYTE type, DWORD count)
{
	if (0 == count)
		return;

	for (UINT i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		if (NULL == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetType() != type)
			continue;

		//���� ������ ��ϵ� �����̸� �Ѿ��. (���� �������� �Ǹŵɶ� �� �κ����� ���� ��� ����!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}

	for (UINT i = 0; i < DRAGON_SOUL_INVENTORY_MAX_NUM; ++i)
	{
		if (NULL == GetDragonSoulItem(i))
			continue;

		if (GetDragonSoulItem(i)->GetType() != type)
			continue;

		if (count >= GetDragonSoulItem(i)->GetCount())
		{
			count -= GetDragonSoulItem(i)->GetCount();
			GetDragonSoulItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetDragonSoulItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	for (UINT i = SKILL_BOOK_INVENTORY_SLOT_START; i < GIFT_BOX_INVENTORY_SLOT_END; ++i)
	{
		if (NULL == GetInventoryItem(i))
			continue;

		if (GetInventoryItem(i)->GetType() != type)
			continue;

		// ���� ������ ��ϵ� �����̸� �Ѿ��. (���� �������� �Ǹŵɶ� �� �κ����� ���� ��� ����!)
		if (m_pkMyShop)
		{
			bool isItemSelling = m_pkMyShop->IsSellingItem(GetInventoryItem(i)->GetID());
			if (isItemSelling)
				continue;
		}

		if (count >= GetInventoryItem(i)->GetCount())
		{
			count -= GetInventoryItem(i)->GetCount();
			GetInventoryItem(i)->SetCount(0);

			if (0 == count)
				return;
		}
		else
		{
			GetInventoryItem(i)->SetCount(GetInventoryItem(i)->GetCount() - count);
			return;
		}
	}
#endif
}

#if defined(__WJ_PICKUP_ITEM_EFFECT__)
void CHARACTER::AutoGiveItem(LPITEM item, bool longOwnerShip, bool isHighLight)
#else
void CHARACTER::AutoGiveItem(LPITEM item, bool longOwnerShip)
#endif
{
	if (NULL == item)
	{
		sys_err("NULL point.");
		return;
	}

	if (item->GetOwner())
	{
		sys_err("item %d 's owner exists!", item->GetID());
		return;
	}

	int cell;
	if (item->IsDragonSoul())
	{
		cell = GetEmptyDragonSoulInventory(item);
	}
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	else if (item->IsSkillBook())
	{
		cell = GetEmptySkillBookInventory(item->GetSize());
	}
	else if (item->IsUpgradeItem())
	{
		cell = GetEmptyUpgradeItemsInventory(item->GetSize());
	}
	else if (item->IsStone())
	{
		cell = GetEmptyStoneInventory(item->GetSize());
	}
	else if (item->IsGiftBox())
	{
		cell = GetEmptyGiftBoxInventory(item->GetSize());
	}
#endif
	else
	{
		cell = GetEmptyInventory(item->GetSize());
	}

	if (cell != -1)
	{
		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, cell));
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
		else if (item->IsSkillBook())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, cell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, cell));
#endif
		else if (item->IsUpgradeItem())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, cell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, cell));
#endif
		else if (item->IsStone())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, cell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, cell));
#endif
		else if (item->IsGiftBox())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, cell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, cell));
#endif
#endif
		else
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, cell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, cell));
#endif

		LogManager::instance().ItemLog(this, item, "SYSTEM", item->GetName());

		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION)
		{
			TQuickslot* pSlot;

			if (GetQuickslot(0, &pSlot) && pSlot->type == QUICKSLOT_TYPE_NONE)
			{
				TQuickslot slot;
				slot.type = QUICKSLOT_TYPE_ITEM;
				slot.pos = cell;
				SetQuickslot(0, slot);
			}
		}
	}
	else
	{
		item->AddToGround(GetMapIndex(), GetXYZ());
		item->StartDestroyEvent();

		if (longOwnerShip)
			item->SetOwnership(this, 300);
		else
			item->SetOwnership(this, 60);
		LogManager::instance().ItemLog(this, item, "SYSTEM_DROP", item->GetName());
	}
}

//< 2020.08.08.Owsap - Fix book stacking while sorting.
void CHARACTER::GiveSkillBook(DWORD dwSkillVnum, WORD wCount)
{
	for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
	{
		LPITEM pkItem = GetInventoryItem(i);

		if (!pkItem)
			continue;

		if ((pkItem->GetType() == ITEM_SKILLBOOK || pkItem->GetType() == ITEM_SKILLFORGET) && pkItem->GetSocket(0) == dwSkillVnum)
		{
			WORD wCount2 = MIN(gMaxItemCount - pkItem->GetCount(), wCount);
			wCount -= wCount2;

			pkItem->SetCount(pkItem->GetCount() + wCount2);
			if (wCount == 0)
				return;
		}
	}

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	for (int i = SKILL_BOOK_INVENTORY_SLOT_START; i < SKILL_BOOK_INVENTORY_SLOT_END; ++i)
	{
		LPITEM pkItem = GetSkillBookInventoryItem(i);

		if (!pkItem)
			continue;

		if ((pkItem->GetType() == ITEM_SKILLBOOK || pkItem->GetType() == ITEM_SKILLFORGET) && pkItem->GetSocket(0) == dwSkillVnum)
		{
			WORD wCount2 = MIN(gMaxItemCount - pkItem->GetCount(), wCount);
			wCount -= wCount2;

			pkItem->SetCount(pkItem->GetCount() + wCount2);
			if (wCount == 0)
				return;
		}
	}
#endif

	LPITEM pkBookItem = AutoGiveItem(ITEM_SKILLBOOK_VNUM, wCount, false, false);
	if (NULL != pkBookItem)
		pkBookItem->SetSocket(0, dwSkillVnum);
}
//>

#if defined(__WJ_PICKUP_ITEM_EFFECT__)
LPITEM CHARACTER::AutoGiveItem(DWORD dwItemVnum, WORD wCount, int iRarePct, bool bMsg, bool isHighLight)
#else
LPITEM CHARACTER::AutoGiveItem(DWORD dwItemVnum, WORD bCount, int iRarePct, bool bMsg)
#endif
{
	TItemTable* p = ITEM_MANAGER::instance().GetTable(dwItemVnum);

	if (!p)
		return NULL;

	DBManager::instance().SendMoneyLog(MONEY_LOG_DROP, dwItemVnum, wCount);

	if (p->dwFlags & ITEM_FLAG_STACKABLE/* && p->bType != ITEM_BLEND*/)
	{
		for (int i = 0; i < INVENTORY_MAX_NUM; ++i)
		{
			LPITEM item = GetInventoryItem(i);

			if (!item)
				continue;

			if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (wCount < p->alValues[1])
						wCount = p->alValues[1];
				}

				WORD wCount2 = MIN(gMaxItemCount - item->GetCount(), wCount);
				wCount -= wCount2;

				item->SetCount(item->GetCount() + wCount2);

				if (wCount == 0)
				{
					if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
						ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#else
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#endif

					return item;
				}
			}
		}

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
		for (int i = SKILL_BOOK_INVENTORY_SLOT_START; i < SKILL_BOOK_INVENTORY_SLOT_END; ++i)
		{
			LPITEM item = GetSkillBookInventoryItem(i);

			if (!item)
				continue;

			if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (wCount < p->alValues[1])
						wCount = p->alValues[1];
				}

				WORD wCount2 = MIN(gMaxItemCount - item->GetCount(), wCount);
				wCount -= wCount2;

				item->SetCount(item->GetCount() + wCount2);

				if (wCount == 0)
				{
					if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
						ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#else
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#endif

					return item;
				}
			}
		}

		for (int i = UPGRADE_ITEMS_INVENTORY_SLOT_START; i < UPGRADE_ITEMS_INVENTORY_SLOT_END; ++i)
		{
			LPITEM item = GetUpgradeItemsInventoryItem(i);

			if (!item)
				continue;

			if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (wCount < p->alValues[1])
						wCount = p->alValues[1];
				}

				WORD wCount2 = MIN(gMaxItemCount - item->GetCount(), wCount);
				wCount -= wCount2;

				item->SetCount(item->GetCount() + wCount2);

				if (wCount == 0)
				{
					if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
						ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#else
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#endif

					return item;
				}
			}
		}

		for (int i = STONE_INVENTORY_SLOT_START; i < STONE_INVENTORY_SLOT_END; ++i)
		{
			LPITEM item = GetStoneInventoryItem(i);

			if (!item)
				continue;

			if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (wCount < p->alValues[1])
						wCount = p->alValues[1];
				}

				WORD wCount2 = MIN(gMaxItemCount - item->GetCount(), wCount);
				wCount -= wCount2;

				item->SetCount(item->GetCount() + wCount2);

				if (wCount == 0)
				{
					if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
						ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#else
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#endif

					return item;
				}
			}
		}

		for (int i = GIFT_BOX_INVENTORY_SLOT_START; i < GIFT_BOX_INVENTORY_SLOT_END; ++i)
		{
			LPITEM item = GetGiftBoxInventoryItem(i);

			if (!item)
				continue;

			if (item->GetVnum() == dwItemVnum && FN_check_item_socket(item))
			{
				if (IS_SET(p->dwFlags, ITEM_FLAG_MAKECOUNT))
				{
					if (wCount < p->alValues[1])
						wCount = p->alValues[1];
				}

				WORD wCount2 = MIN(gMaxItemCount - item->GetCount(), wCount);
				wCount -= wCount2;

				item->SetCount(item->GetCount() + wCount2);

				if (wCount == 0)
				{
					if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
						ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#else
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#endif

					return item;
				}
			}
		}
#endif
	}

	LPITEM item = ITEM_MANAGER::instance().CreateItem(dwItemVnum, wCount, 0, true);

	if (!item)
	{
		sys_err("cannot create item by vnum %u (name: %s)", dwItemVnum, GetName());
		return NULL;
	}

	if (item->GetType() == ITEM_BLEND)
	{
		for (int i = 0; i < INVENTORY_MAX_NUM; i++)
		{
			LPITEM inv_item = GetInventoryItem(i);

			if (inv_item == NULL) continue;

#if defined(__EXTENDED_BLEND_AFFECT__)
			if (inv_item->GetType() == ITEM_BLEND && !inv_item->IsExtendedBlend(inv_item->GetVnum()))
#else
			if (inv_item->GetType() == ITEM_BLEND)
#endif
			{
				if (inv_item->GetVnum() == item->GetVnum())
				{
					if (inv_item->GetSocket(0) == item->GetSocket(0) &&
						inv_item->GetSocket(1) == item->GetSocket(1) &&
#if !defined(__EXTENDED_BLEND_AFFECT__)
						inv_item->GetSocket(2) == item->GetSocket(2) &&
#endif
						inv_item->GetCount() < gMaxItemCount)
					{
#if defined(__EXTENDED_BLEND_AFFECT__)
						inv_item->SetSocket(2, inv_item->GetSocket(2) + item->GetSocket(2));
#else
						inv_item->SetCount(inv_item->GetCount() + item->GetCount());
#endif
						M2_DESTROY_ITEM(item);
						return inv_item;
					}
				}
			}
		}
	}

	int iEmptyCell;
	if (item->IsDragonSoul())
		iEmptyCell = GetEmptyDragonSoulInventory(item);
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
	else if (item->IsSkillBook())
		iEmptyCell = GetEmptySkillBookInventory(item->GetSize());
	else if (item->IsUpgradeItem())
		iEmptyCell = GetEmptyUpgradeItemsInventory(item->GetSize());
	else if (item->IsStone())
		iEmptyCell = GetEmptyStoneInventory(item->GetSize());
	else if (item->IsGiftBox())
		iEmptyCell = GetEmptyGiftBoxInventory(item->GetSize());
#endif
	else
		iEmptyCell = GetEmptyInventory(item->GetSize());

	if (iEmptyCell != -1)
	{
		if (item->IsDragonSoul())
			item->AddToCharacter(this, TItemPos(DRAGON_SOUL_INVENTORY, iEmptyCell));
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
		else if (item->IsSkillBook())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
#endif
		else if (item->IsUpgradeItem())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
#endif
		else if (item->IsStone())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
#endif
		else if (item->IsGiftBox())
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
#endif
#endif
		else
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell), isHighLight);
#else
			item->AddToCharacter(this, TItemPos(INVENTORY, iEmptyCell));
#endif

		if (bMsg)
#if defined(__CHATTING_WINDOW_RENEWAL__)
			ChatPacket(CHAT_TYPE_ITEM_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#else
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ȹ��: %s"), item->GetLocaleName());
#endif

		LogManager::instance().ItemLog(this, item, "SYSTEM", item->GetName());

		if (item->GetType() == ITEM_USE && item->GetSubType() == USE_POTION)
		{
			TQuickslot* pSlot;

			if (GetQuickslot(0, &pSlot) && pSlot->type == QUICKSLOT_TYPE_NONE)
			{
				TQuickslot slot;
				slot.type = QUICKSLOT_TYPE_ITEM;
				slot.pos = iEmptyCell;
				SetQuickslot(0, slot);
			}
		}
	}
	else
	{
		item->AddToGround(GetMapIndex(), GetXYZ());
		item->StartDestroyEvent();
		// ��Ƽ ��� flag�� �ɷ��ִ� �������� ���,
		// �κ��� �� ������ ��� ��¿ �� ���� ����Ʈ���� �Ǹ�,
		// ownership�� �������� ����� ������(300��) �����Ѵ�.
		if (IS_SET(item->GetAntiFlag(), ITEM_ANTIFLAG_DROP))
			item->SetOwnership(this, 300);
		else
			item->SetOwnership(this, 60);
		LogManager::instance().ItemLog(this, item, "SYSTEM_DROP", item->GetName());
	}

	sys_log(0, "7: %d %d", dwItemVnum, wCount);
	return item;
}

bool CHARACTER::GiveItem(LPCHARACTER victim, TItemPos Cell)
{
	if (!CanHandleItem())
		return false;

	LPITEM item = GetItem(Cell);

	if (item && !item->IsExchanging())
	{
		if (victim->CanReceiveItem(this, item))
		{
			victim->ReceiveItem(this, item);
			return true;
		}
	}

	return false;
}

bool CHARACTER::CanReceiveItem(LPCHARACTER from, LPITEM item) const
{
	if (IsPC())
		return false;

	// TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX
	if (DISTANCE_APPROX(GetX() - from->GetX(), GetY() - from->GetY()) > 2000)
		return false;
	// END_OF_TOO_LONG_DISTANCE_EXCHANGE_BUG_FIX

#if defined(__SOUL_BIND_SYSTEM__)
	if (item->IsSealed())
	{
		from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot improve a soulbound item."));
		return false;
	}
#endif

	switch (GetRaceNum())
	{
	case fishing::CAMPFIRE_MOB:
		if (item->GetType() == ITEM_FISH &&
			(item->GetSubType() == FISH_ALIVE || item->GetSubType() == FISH_DEAD))
			return true;
		break;

	case fishing::FISHER_MOB:
		if (item->GetType() == ITEM_ROD)
			return true;
		break;

		// BUILDING_NPC
	case BLACKSMITH_WEAPON_MOB:
	case DEVILTOWER_BLACKSMITH_WEAPON_MOB:
		if (item->GetType() == ITEM_WEAPON &&
			item->GetRefinedVnum())
			return true;
		else
			return false;
		break;

	case BLACKSMITH_ARMOR_MOB:
	case DEVILTOWER_BLACKSMITH_ARMOR_MOB:
		if (item->GetType() == ITEM_ARMOR &&
			(item->GetSubType() == ARMOR_BODY || item->GetSubType() == ARMOR_SHIELD || item->GetSubType() == ARMOR_HEAD) &&
			item->GetRefinedVnum())
			return true;
		else
			return false;
		break;

	case BLACKSMITH_ACCESSORY_MOB:
	case DEVILTOWER_BLACKSMITH_ACCESSORY_MOB:
		if (item->GetType() == ITEM_ARMOR &&
			!(item->GetSubType() == ARMOR_BODY || item->GetSubType() == ARMOR_SHIELD || item->GetSubType() == ARMOR_HEAD) &&
			item->GetRefinedVnum())
			return true;
		else
			return false;
		break;
		// END_OF_BUILDING_NPC

	case BLACKSMITH_MOB:
		if (item->GetRefinedVnum() && item->GetRefineSet() < 500)
		{
			return true;
		}
		else
		{
			return false;
		}

	case BLACKSMITH2_MOB:
		if (item->GetRefineSet() >= 500)
		{
			return true;
		}
		else
		{
			return false;
		}

	case ALCHEMIST_MOB:
		if (item->GetRefinedVnum())
			return true;
		break;

	case 20101:
	case 20102:
	case 20103:
		// �ʱ� ��
		if (item->GetVnum() == ITEM_REVIVE_HORSE_1)
		{
			if (!IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���� ������ ���ʸ� ���� �� �����ϴ�."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_1)
		{
			if (IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ ��Ḧ ���� �� �����ϴ�."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_2 || item->GetVnum() == ITEM_HORSE_FOOD_3)
		{
			return false;
		}
		break;
	case 20104:
	case 20105:
	case 20106:
		// �߱� ��
		if (item->GetVnum() == ITEM_REVIVE_HORSE_2)
		{
			if (!IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���� ������ ���ʸ� ���� �� �����ϴ�."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_2)
		{
			if (IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ ��Ḧ ���� �� �����ϴ�."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_1 || item->GetVnum() == ITEM_HORSE_FOOD_3)
		{
			return false;
		}
		break;
	case 20107:
	case 20108:
	case 20109:
		// ��� ��
		if (item->GetVnum() == ITEM_REVIVE_HORSE_3)
		{
			if (!IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ���� ������ ���ʸ� ���� �� �����ϴ�."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_3)
		{
			if (IsDead())
			{
				from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ ��Ḧ ���� �� �����ϴ�."));
				return false;
			}
			return true;
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_1 || item->GetVnum() == ITEM_HORSE_FOOD_2)
		{
			return false;
		}
		break;
	}

	//if (IS_SET(item->GetFlag(), ITEM_FLAG_QUEST_GIVE))
	{
		return true;
	}

	return false;
}

void CHARACTER::ReceiveItem(LPCHARACTER from, LPITEM item)
{
	if (IsPC())
		return;

	switch (GetRaceNum())
	{
	case fishing::CAMPFIRE_MOB:
		if (item->GetType() == ITEM_FISH && (item->GetSubType() == FISH_ALIVE || item->GetSubType() == FISH_DEAD))
			fishing::Grill(from, item);
		else
		{
			// TAKE_ITEM_BUG_FIX
			from->SetQuestNPCID(GetVID());
			// END_OF_TAKE_ITEM_BUG_FIX
			quest::CQuestManager::instance().TakeItem(from->GetPlayerID(), GetRaceNum(), item);
		}
		break;

#if defined(__GUILD_DRAGONLAIR__)
	case MeleyLair::STATUE_VNUM:
	{
		if (MeleyLair::CMgr::instance().IsMeleyMap(from->GetMapIndex()))
			MeleyLair::CMgr::instance().OnKillStatue(item, from, this, from->GetGuild());
	}
	break;
#endif

	// DEVILTOWER_NPC
	case DEVILTOWER_BLACKSMITH_WEAPON_MOB:
	case DEVILTOWER_BLACKSMITH_ARMOR_MOB:
	case DEVILTOWER_BLACKSMITH_ACCESSORY_MOB:
		if (item->GetRefinedVnum() != 0 && item->GetRefineSet() != 0 && item->GetRefineSet() < 500)
		{
#if defined(__SOUL_SYSTEM__)
			if (item->GetType() == ITEM_SOUL)
				return;
#endif
			from->SetRefineNPC(this);
			from->RefineInformation(item->GetCell(), REFINE_TYPE_MONEY_ONLY);
		}
		else
		{
			from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� ������ �� �����ϴ�."));
		}
		break;
		// END_OF_DEVILTOWER_NPC

	case BLACKSMITH_MOB:
	case BLACKSMITH2_MOB:
	case BLACKSMITH_WEAPON_MOB:
	case BLACKSMITH_ARMOR_MOB:
	case BLACKSMITH_ACCESSORY_MOB:
		if (item->GetRefinedVnum())
		{
#if defined(__SOUL_SYSTEM__)
			if (item->GetType() == ITEM_SOUL)
				return;
#endif
			from->SetRefineNPC(this);
			from->RefineInformation(item->GetCell(), REFINE_TYPE_NORMAL);
		}
		else
		{
			from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� �������� ������ �� �����ϴ�."));
		}
		break;

	case 20101:
	case 20102:
	case 20103:
	case 20104:
	case 20105:
	case 20106:
	case 20107:
	case 20108:
	case 20109:
		if (item->GetVnum() == ITEM_REVIVE_HORSE_1 ||
			item->GetVnum() == ITEM_REVIVE_HORSE_2 ||
			item->GetVnum() == ITEM_REVIVE_HORSE_3)
		{
			from->ReviveHorse();
			item->SetCount(item->GetCount() - 1);
			from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ���ʸ� �־����ϴ�."));
		}
		else if (item->GetVnum() == ITEM_HORSE_FOOD_1 ||
			item->GetVnum() == ITEM_HORSE_FOOD_2 ||
			item->GetVnum() == ITEM_HORSE_FOOD_3)
		{
			from->FeedHorse();
			from->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ��Ḧ �־����ϴ�."));
			item->SetCount(item->GetCount() - 1);
			EffectPacket(SE_HPUP_RED);
		}
		break;

	default:
		sys_log(0, "TakeItem %s %d %s", from->GetName(), GetRaceNum(), item->GetName());
		from->SetQuestNPCID(GetVID());
		quest::CQuestManager::instance().TakeItem(from->GetPlayerID(), GetRaceNum(), item);
		break;
	}
}

bool CHARACTER::IsEquipUniqueItem(DWORD dwItemVnum) const
{
	{
		LPITEM u = GetWear(WEAR_UNIQUE1);

		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}

	{
		LPITEM u = GetWear(WEAR_UNIQUE2);

		if (u && u->GetVnum() == dwItemVnum)
			return true;
	}

	// �������� ��� ������(�ߺ�) ������ üũ�Ѵ�.
	if (dwItemVnum == UNIQUE_ITEM_RING_OF_LANGUAGE)
		return IsEquipUniqueItem(UNIQUE_ITEM_RING_OF_LANGUAGE_SAMPLE);

	return false;
}

// CHECK_UNIQUE_GROUP
bool CHARACTER::IsEquipUniqueGroup(DWORD dwGroupVnum) const
{
	{
		LPITEM u = GetWear(WEAR_UNIQUE1);

		if (u && u->GetSpecialGroup() == (int)dwGroupVnum)
			return true;
	}

	{
		LPITEM u = GetWear(WEAR_UNIQUE2);

		if (u && u->GetSpecialGroup() == (int)dwGroupVnum)
			return true;
	}

	return false;
}
// END_OF_CHECK_UNIQUE_GROUP

void CHARACTER::SetRefineMode(int iAdditionalCell)
{
	m_iRefineAdditionalCell = iAdditionalCell;
	SetUnderRefine(true);
}

void CHARACTER::ClearRefineMode()
{
	SetUnderRefine(false);
	SetRefineNPC(NULL);
}

//bool CHARACTER::GiveItemFromSpecialItemGroup(DWORD dwGroupNum, std::vector<DWORD>& dwItemVnums,
//	std::vector<DWORD>& dwItemCounts, std::vector <LPITEM>& item_gets, int& count)
bool CHARACTER::GiveItemFromSpecialItemGroup(DWORD dwGroupNum)
{
	const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(dwGroupNum);

	if (!pGroup)
	{
		sys_err("cannot find special item group %d", dwGroupNum);
		return false;
	}

	std::vector<int> idxes;
	int n = pGroup->GetMultiIndex(idxes);

	bool bSuccess = false;

	for (int i = 0; i < n; i++)
	{
		bSuccess = false;
		int idx = idxes[i];
		DWORD dwVnum = pGroup->GetVnum(idx);
		DWORD dwCount = pGroup->GetCount(idx);
		int iRarePct = pGroup->GetRarePct(idx);
		LPITEM item_get = NULL;
		switch (dwVnum)
		{
		case CSpecialItemGroup::GOLD:
		{
			PointChange(POINT_GOLD, dwCount);
#if defined(__CHATTING_WINDOW_RENEWAL__)
			ChatPacket(CHAT_TYPE_MONEY_INFO, LC_TEXT("�� %d ���� ȹ���߽��ϴ�."), dwCount);
#else
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�� %d ���� ȹ���߽��ϴ�."), dwCount);
#endif
			LogManager::instance().CharLog(this, dwCount, "TREASURE_GOLD", "");
			bSuccess = true;
		}
		break;

		case CSpecialItemGroup::EXP:
		{
			PointChange(POINT_EXP, dwCount);
#if defined(__CHATTING_WINDOW_RENEWAL__)
			ChatPacket(CHAT_TYPE_EXP_INFO, LC_TEXT("%d�� ����ġ�� ȹ���߽��ϴ�."), dwCount);
#else
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%d�� ����ġ�� ȹ���߽��ϴ�."), dwCount);
#endif
			LogManager::instance().CharLog(this, dwCount, "TREASURE_EXP", "");
			bSuccess = true;
		}
		break;

		case CSpecialItemGroup::MOB:
		{
			sys_log(0, "CSpecialItemGroup::MOB %d", dwCount);
			int x = GetX() + number(-500, 500);
			int y = GetY() + number(-500, 500);

			LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnMob(dwCount, GetMapIndex(), x, y, 0, true, -1);
			if (ch)
				ch->SetAggressive();
			else
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This doesn't seem to work here."));
				return false;
			}
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ڿ��� ���Ͱ� ��Ÿ�����ϴ�!"));
			bSuccess = true;
		}
		break;

		case CSpecialItemGroup::SLOW:
		{
			sys_log(0, "CSpecialItemGroup::SLOW %d", -(int)dwCount);
			AddAffect(AFFECT_SLOW, POINT_MOV_SPEED, -(int)dwCount, AFF_SLOW, 300, 0, true);
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ڿ��� ���� ���� ���⸦ ���̸����� �����̴� �ӵ��� ���������ϴ�!"));
			bSuccess = true;
		}
		break;

		case CSpecialItemGroup::DRAIN_HP:
		{
			int iDropHP = GetMaxHP() * dwCount / 100;
			sys_log(0, "CSpecialItemGroup::DRAIN_HP %d", -iDropHP);
			iDropHP = MIN(iDropHP, GetHP() - 1);
			sys_log(0, "CSpecialItemGroup::DRAIN_HP %d", -iDropHP);
			PointChange(POINT_HP, -iDropHP);
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ڰ� ���ڱ� �����Ͽ����ϴ�! ������� �����߽��ϴ�."));
			bSuccess = true;
		}
		break;

		case CSpecialItemGroup::POISON:
		{
			AttackedByPoison(NULL);
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ڿ��� ���� ��� ���⸦ ���̸����� ���� �¸����� �����ϴ�!"));
			bSuccess = true;
		}
		break;

		case CSpecialItemGroup::BLEEDING:
		{
			AttackedByBleeding(NULL);
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ڿ��� ���� ���� ���⸦ ���̸����� �����̴� �ӵ��� ���������ϴ�!"));
			bSuccess = true;
		}
		break;

		case CSpecialItemGroup::MOB_GROUP:
		{
			int sx = GetX() - number(300, 500);
			int sy = GetY() - number(300, 500);
			int ex = GetX() + number(300, 500);
			int ey = GetY() + number(300, 500);
			LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnGroup(dwCount, GetMapIndex(), sx, sy, ex, ey, NULL, true);
			if (!ch)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("This doesn't seem to work here."));
				return false;
			}
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���ڿ��� ���Ͱ� ��Ÿ�����ϴ�!"));
			bSuccess = true;
		}
		break;

		default:
		{
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
			item_get = AutoGiveItem(dwVnum, dwCount, iRarePct, true, true);
#else
			item_get = AutoGiveItem(dwVnum, dwCount, iRarePct);
#endif
			if (item_get)
				bSuccess = true;
		}
		break;
		}

		if (!bSuccess)
		{
			ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("�ƹ��͵� ���� �� �������ϴ�."));
			return false;
		}
	}
	return bSuccess;
}

// NEW_HAIR_STYLE_ADD
bool CHARACTER::ItemProcess_Hair(LPITEM item, int iDestCell)
{
	if (item->CheckItemUseLevel(GetLevel()) == false)
	{
		// ���� ���ѿ� �ɸ�
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� �� �Ӹ��� ����� �� ���� �����Դϴ�."));
		return false;
	}

	DWORD hair = item->GetVnum();

	switch (GetJob())
	{
	case JOB_WARRIOR:
		hair -= 72000; // 73001 - 72000 = 1001 ���� ��� ��ȣ ����
		break;

	case JOB_ASSASSIN:
		hair -= 71250;
		break;

	case JOB_SURA:
		hair -= 70500;
		break;

	case JOB_SHAMAN:
		hair -= 69750;
		break;

	case JOB_WOLFMAN:
		break;

	default:
		return false;
		break;
	}

	if (hair == GetPart(PART_HAIR))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �Ӹ� ��Ÿ�Ϸδ� ��ü�� �� �����ϴ�."));
		return true;
	}

	item->SetCount(item->GetCount() - 1);

	SetPart(PART_HAIR, hair);
	UpdatePacket();

	return true;
}
// END_NEW_HAIR_STYLE_ADD

bool CHARACTER::ItemProcess_Polymorph(LPITEM item)
{
	if (IsPolymorphed())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�̹� �а����� �����Դϴ�."));
		return false;
	}

	if (true == IsRiding())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�а��� �� ���� �����Դϴ�."));
		return false;
	}

	DWORD dwVnum = item->GetSocket(0);

	if (dwVnum == 0)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�߸��� �а� �������Դϴ�."));
		item->SetCount(item->GetCount() - 1);
		return false;
	}

	const CMob* pMob = CMobManager::instance().Get(dwVnum);

	if (pMob == NULL)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�߸��� �а� �������Դϴ�."));
		item->SetCount(item->GetCount() - 1);
		return false;
	}

	switch (item->GetVnum())
	{
	case 70104:
	case 70105:
	case 70106:
	case 70107:
	case 71093:
	{
		// �а��� ó��
		sys_log(0, "USE_POLYMORPH_BALL PID(%d) vnum(%d)", GetPlayerID(), dwVnum);

		// ���� ���� üũ
		int iPolymorphLevelLimit = MAX(0, 20 - GetLevel() * 3 / 10);
		if (pMob->m_table.bLevel >= GetLevel() + iPolymorphLevelLimit)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ �ʹ� ���� ������ ���ͷδ� ���� �� �� �����ϴ�."));
			return false;
		}

		int iDuration = GetSkillLevel(POLYMORPH_SKILL_ID) == 0 ? 5 : (5 + (5 + GetSkillLevel(POLYMORPH_SKILL_ID) / 40 * 25));
		iDuration *= 60;

		DWORD dwBonus = 0;

		if (true == LC_IsYMIR() || true == LC_IsKorea())
		{
			dwBonus = GetSkillLevel(POLYMORPH_SKILL_ID) + 60;
		}
		else
		{
			dwBonus = (2 + GetSkillLevel(POLYMORPH_SKILL_ID) / 40) * 100;
		}

		AddAffect(AFFECT_POLYMORPH, POINT_POLYMORPH, dwVnum, AFF_POLYMORPH, iDuration, 0, true);
		AddAffect(AFFECT_POLYMORPH, POINT_ATT_BONUS, dwBonus, AFF_POLYMORPH, iDuration, 0, false);

		item->SetCount(item->GetCount() - 1);
	}
	break;

	case 50322:
	{
		// ����

		// �а��� ó��
		// ����0 ����1 ����2
		// �а��� ���� ��ȣ �������� �а��� ����
		sys_log(0, "USE_POLYMORPH_BOOK: %s(%u) vnum(%u)", GetName(), GetPlayerID(), dwVnum);

		if (CPolymorphUtils::instance().PolymorphCharacter(this, item, pMob) == true)
		{
			CPolymorphUtils::instance().UpdateBookPracticeGrade(this, item);
		}
		else
		{
		}
	}
	break;

	default:
		sys_err("POLYMORPH invalid item passed PID(%d) vnum(%d)", GetPlayerID(), item->GetOriginalVnum());
		return false;
	}

	return true;
}

bool CHARACTER::CanDoCube() const
{
	if (m_bIsObserver) return false;
	if (GetShop()) return false;
	if (GetMyShop()) return false;
	if (IsUnderRefine()) return false;
	if (IsWarping()) return false;

	return true;
}

bool CHARACTER::UnEquipSpecialRideUniqueItem()
{
	LPITEM Unique1 = GetWear(WEAR_UNIQUE1);
	LPITEM Unique2 = GetWear(WEAR_UNIQUE2);
#if defined(__MOUNT_COSTUME_SYSTEM__)
	LPITEM MountCostume = GetWear(WEAR_COSTUME_MOUNT);
#endif

	if (NULL != Unique1)
	{
		if (UNIQUE_GROUP_SPECIAL_RIDE == Unique1->GetSpecialGroup())
		{
			return UnequipItem(Unique1);
		}
	}

	if (NULL != Unique2)
	{
		if (UNIQUE_GROUP_SPECIAL_RIDE == Unique2->GetSpecialGroup())
		{
			return UnequipItem(Unique2);
		}
	}

#if defined(__MOUNT_COSTUME_SYSTEM__)
	if (MountCostume)
		return UnequipItem(MountCostume);
#endif

	return true;
}

void CHARACTER::AutoRecoveryItemProcess(const EAffectTypes type)
{
	if (true == IsDead() || true == IsStun())
		return;

	if (false == IsPC())
		return;

	if (AFFECT_AUTO_HP_RECOVERY != type && AFFECT_AUTO_SP_RECOVERY != type)
		return;

	if (NULL != FindAffect(AFFECT_STUN))
		return;

	{
		const DWORD stunSkills[] = { SKILL_TANHWAN, SKILL_GEOMPUNG, SKILL_BYEURAK, SKILL_GIGUNG };

		for (size_t i = 0; i < sizeof(stunSkills) / sizeof(DWORD); ++i)
		{
			const CAffect* p = FindAffect(stunSkills[i]);

			if (NULL != p && AFF_STUN == p->dwFlag)
				return;
		}
	}

	const CAffect* pAffect = FindAffect(type);
	const size_t idx_of_amount_of_used = 1;
	const size_t idx_of_amount_of_full = 2;

	if (NULL != pAffect)
	{
		LPITEM pItem = FindItemByID(pAffect->dwFlag);

		if (NULL != pItem && true == pItem->GetSocket(0))
		{
			if (false == CArenaManager::instance().IsArenaMap(GetMapIndex()))
			{
				const long amount_of_used = pItem->GetSocket(idx_of_amount_of_used);
				const long amount_of_full = pItem->GetSocket(idx_of_amount_of_full);

				const int32_t avail = amount_of_full - amount_of_used;

				int32_t amount = 0;

				if (AFFECT_AUTO_HP_RECOVERY == type)
				{
					amount = GetMaxHP() - (GetHP() + GetPoint(POINT_HP_RECOVERY));
				}
				else if (AFFECT_AUTO_SP_RECOVERY == type)
				{
					amount = GetMaxSP() - (GetSP() + GetPoint(POINT_SP_RECOVERY));
				}

				if (amount > 0)
				{
					if (avail > amount)
					{
						const int pct_of_used = amount_of_used * 100 / amount_of_full;
						const int pct_of_will_used = (amount_of_used + amount) * 100 / amount_of_full;

						bool bLog = false;
						// ��뷮�� 10% ������ �α׸� ����
						// (��뷮�� %����, ���� �ڸ��� �ٲ� ������ �α׸� ����.)
						if ((pct_of_will_used / 10) - (pct_of_used / 10) >= 1)
							bLog = true;
						pItem->SetSocket(idx_of_amount_of_used, amount_of_used + amount, bLog);
					}
					else
					{
						amount = avail;

						ITEM_MANAGER::instance().RemoveItem(pItem);
					}

					if (AFFECT_AUTO_HP_RECOVERY == type)
					{
						PointChange(POINT_HP_RECOVERY, amount);
						EffectPacket(SE_AUTO_HPUP);
					}
					else if (AFFECT_AUTO_SP_RECOVERY == type)
					{
						PointChange(POINT_SP_RECOVERY, amount);
						EffectPacket(SE_AUTO_SPUP);
					}
				}
			}
			else
			{
				pItem->Lock(false);
				pItem->SetSocket(0, false);
				RemoveAffect(const_cast<CAffect*>(pAffect));
			}
		}
		else
		{
			RemoveAffect(const_cast<CAffect*>(pAffect));
		}
	}
}

bool CHARACTER::IsValidItemPosition(TItemPos Pos) const
{
	BYTE window_type = Pos.window_type;
	WORD cell = Pos.cell;

	switch (window_type)
	{
	case RESERVED_WINDOW:
		return false;

	case INVENTORY:
	case EQUIPMENT:
		return cell < (INVENTORY_AND_EQUIP_SLOT_MAX);

	case DRAGON_SOUL_INVENTORY:
		return cell < (DRAGON_SOUL_INVENTORY_MAX_NUM);

	case SAFEBOX:
		if (NULL != m_pkSafebox)
			return m_pkSafebox->IsValidPosition(cell);
		else
			return false;

	case MALL:
		if (NULL != m_pkMall)
			return m_pkMall->IsValidPosition(cell);
		else
			return false;

	default:
		return false;
	}
}

/// ���� ĳ������ ���¸� �������� �־��� item�� ������ �� �ִ� �� Ȯ���ϰ�, �Ұ��� �ϴٸ� ĳ���Ϳ��� ������ �˷��ִ� �Լ�
bool CHARACTER::CanEquipNow(const LPITEM item, const TItemPos& srcCell, const TItemPos& destCell) /*const*/
{
	const TItemTable* itemTable = item->GetProto();
	//BYTE itemType = item->GetType();
	//BYTE itemSubType = item->GetSubType();

#if defined(__PET_SYSTEM__)
	if (item->IsPet() && IS_BLOCKED_PET_SUMMON_MAP(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot summon your mount/pet right now."));
		return false;
	}

	if (item->IsPet() && IS_BLOCKED_PET_DUNGEON_MAP(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot summon your mount/pet right now."));
		return false;
	}
#endif

#if defined(__MOUNT_COSTUME_SYSTEM__)
	if (item->IsCostumeMount() && IS_BLOCKED_MOUNT_SUMMON_MAP(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot summon your mount/pet right now."));
		return false;
	}

	if (item->IsCostumeMount() && IS_BLOCKED_MOUNT_DUNGEON_MAP(GetMapIndex()))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You cannot summon your mount/pet right now."));
		return false;
	}
#endif

	switch (GetJob())
	{
	case JOB_WARRIOR:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
			return false;
		break;

	case JOB_ASSASSIN:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
			return false;
		break;

	case JOB_SHAMAN:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
			return false;
		break;

	case JOB_SURA:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_SURA)
			return false;
		break;

	case JOB_WOLFMAN:
		if (item->GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN)
			return false;
		break;

	}

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		long limit = itemTable->aLimits[i].lValue;
		switch (itemTable->aLimits[i].bType)
		{
		case LIMIT_LEVEL:
			if (GetLevel() < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ���� ������ �� �����ϴ�."));
				return false;
			}
			break;

		case LIMIT_STR:
			if (GetPoint(POINT_ST) < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("�ٷ��� ���� ������ �� �����ϴ�."));
				return false;
			}
			break;

		case LIMIT_INT:
			if (GetPoint(POINT_IQ) < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("������ ���� ������ �� �����ϴ�."));
				return false;
			}
			break;

		case LIMIT_DEX:
			if (GetPoint(POINT_DX) < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ø�� ���� ������ �� �����ϴ�."));
				return false;
			}
			break;

		case LIMIT_CON:
			if (GetPoint(POINT_HT) < limit)
			{
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("ü���� ���� ������ �� �����ϴ�."));
				return false;
			}
			break;
		}
	}

	if (item->GetWearFlag() & WEARABLE_UNIQUE)
	{
		if ((GetWear(WEAR_UNIQUE1) && GetWear(WEAR_UNIQUE1)->IsSameSpecialGroup(item)) ||
			(GetWear(WEAR_UNIQUE2) && GetWear(WEAR_UNIQUE2)->IsSameSpecialGroup(item)))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("���� ������ ����ũ ������ �� ���� ���ÿ� ������ �� �����ϴ�."));
			return false;
		}

		if (marriage::CManager::instance().IsMarriageUniqueItem(item->GetVnum()) &&
			!marriage::CManager::instance().IsMarried(GetPlayerID()))
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("��ȥ���� ���� ���¿��� ������ ������ �� �����ϴ�."));
			return false;
		}

	}

	return true;
}

/// ���� ĳ������ ���¸� �������� ���� ���� item�� ���� �� �ִ� �� Ȯ���ϰ�, �Ұ��� �ϴٸ� ĳ���Ϳ��� ������ �˷��ִ� �Լ�
bool CHARACTER::CanUnequipNow(const LPITEM item, const TItemPos& srcCell, const TItemPos& destCell) /*const*/
//bool CHARACTER::CanUnequipNow(const LPITEM item, const TItemPos& swapCell) /*const*/
{
	// ������ ������ �� ���� ������
	if (IS_SET(item->GetFlag(), ITEM_FLAG_IRREMOVABLE))
		return false;

	//< 2019.05.09.Owsap Remove SKILL_GWIGEOM & SKILL_GEOMKYUNG if not using weapon
	{
		if (item->GetType() == ITEM_WEAPON)
		{
			if (IsAffectFlag(AFF_GWIGUM))
				RemoveAffect(SKILL_GWIGEOM);

			if (IsAffectFlag(AFF_GEOMGYEONG))
				RemoveAffect(SKILL_GEOMKYUNG);
		}
	}
	//>

	// ������ unequip�� �κ��丮�� �ű� �� �� �ڸ��� �ִ� �� Ȯ��
	{
		int pos = -1;

		if (item->IsDragonSoul())
			pos = GetEmptyDragonSoulInventory(item);
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
		else if (item->IsSkillBook())
			pos = GetEmptySkillBookInventory(item->GetSize());
		else if (item->IsUpgradeItem())
			pos = GetEmptyUpgradeItemsInventory(item->GetSize());
		else if (item->IsStone())
			pos = GetEmptyStoneInventory(item->GetSize());
		else if (item->IsGiftBox())
			pos = GetEmptyGiftBoxInventory(item->GetSize());
#endif
		else
			pos = GetEmptyInventory(item->GetSize());

		VERIFY_MSG(-1 == pos, "����ǰ�� �� ������ �����ϴ�.");
	}

	return true;
}

#if defined(__SOUL_SYSTEM__)
void CHARACTER::UseSoulAttack(BYTE bSoulType)
{
	EAffectTypes type = AFFECT_NONE;
	EAffectBits flag = AFF_NONE;

	switch (bSoulType)
	{
	case RED_SOUL:
		type = AFFECT_SOUL_RED;
		flag = AFF_SOUL_RED;
		break;

	case BLUE_SOUL:
		type = AFFECT_SOUL_BLUE;
		flag = AFF_SOUL_BLUE;
		break;
	}

	if (AFFECT_NONE == type)
		return;

	const CAffect* pAffect = FindAffect(type);

	if (NULL != pAffect)
	{
		DWORD vnum[][5] = {
			{ 70500, 70501, 70502, 70503, 70504 },
			{ 70505, 70506, 70507, 70508, 70509 },
		};

		for (int i = 0; i < 5; ++i)
		{
			LPITEM item = FindSpecifyItem(vnum[bSoulType][i]);

			if (NULL != item)
			{
				if (item->GetSocket(3) >= item->GetLimitValue(1) && item->isLocked())
				{
					if (item->GetSocket(2) <= (item->GetLimitValue(1) * 10000))
					{
						RemoveAffect(type);
						if (FindAffect(AFFECT_SOUL_MIX))
							RemoveAffect(AFFECT_SOUL_MIX);

						item->SetSocket(1, false);
						item->SetSocket(2, item->GetValue(2));
						item->SetSocket(3, 0);
						item->StartSoulTimeUseEvent();
					}
					else
						item->SetSocket(2, item->GetSocket(2) - 1);
				}
			}
		}
	}
}

bool CHARACTER::CheckSoulItem()
{
	DWORD vnum[2][5] = {
		{ 70500, 70501, 70502, 70503, 70504 },
		{ 70505, 70506, 70507, 70508, 70509 },
	};

	bool hasSoulItem = false;

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			LPITEM item = FindSpecifyItem(vnum[i][j]);
			if (item)
			{
				if (item->isLocked() || item->GetSocket(1) == true)
				{
					item->Lock(true);
					hasSoulItem = true;
					break;
				}
			}
		}
	}

	if (!hasSoulItem)
		return false;

	return true;
}
#endif
