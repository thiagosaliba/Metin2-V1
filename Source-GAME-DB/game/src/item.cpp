#include "stdafx.h"
#include "utils.h"
#include "config.h"
#include "char.h"
#include "desc.h"
#include "sectree_manager.h"
#include "packet.h"
#include "protocol.h"
#include "log.h"
#include "skill.h"
#include "unique_item.h"
#include "profiler.h"
#include "marriage.h"
#include "item_addon.h"
#include "dev_log.h"
#include "locale_service.h"
#include "item.h"
#include "item_manager.h"
#include "affect.h"
#include "DragonSoul.h"
#include "buff_on_attributes.h"
#include "belt_inventory_helper.h"
#include "../../common/VnumHelper.h"
#include "mob_manager.h"
#include "PetSystem.h"

CItem::CItem(DWORD dwVnum)
	: m_dwVnum(dwVnum), m_bWindow(0), m_dwID(0), m_bEquipped(false), m_dwVID(0), m_wCell(0), m_dwCount(0), m_lFlag(0), m_dwLastOwnerPID(0),
	m_bExchanging(false), m_pkDestroyEvent(NULL), m_pkUniqueExpireEvent(NULL), m_pkTimerBasedOnWearExpireEvent(NULL), m_pkRealTimeExpireEvent(NULL),
	m_pkExpireEvent(NULL),
	m_pkAccessorySocketExpireEvent(NULL), m_pkOwnershipEvent(NULL), m_dwOwnershipPID(0), m_bSkipSave(false), m_isLocked(false),
	m_dwMaskVnum(0), m_dwSIGVnum(0)
#if defined(__CHANGE_LOOK_SYSTEM__)
	, m_dwTransmutationVnum(0)
#endif
#if defined(__SOUL_BIND_SYSTEM__)
	, m_lSoulbind(0), m_pkUnbindTimerExpireEvent(NULL)
#endif
#if defined(__SOUL_SYSTEM__)
	, m_pkSoulTimeUseEvent(NULL)
#endif
#if defined(__EXTENDED_BLEND_AFFECT__)
	, m_pkBlendUseEvent(NULL)
#endif
{
	memset(&m_alSockets, 0, sizeof(m_alSockets));
#if defined(__ITEM_APPLY_RANDOM__)
	memset(&m_aApplyRandom, 0, sizeof(m_aApplyRandom));
#endif
	memset(&m_aAttr, 0, sizeof(m_aAttr));
}

CItem::~CItem()
{
	Destroy();
}

void CItem::Initialize()
{
	CEntity::Initialize(ENTITY_ITEM);

	m_bWindow = RESERVED_WINDOW;
	m_pOwner = NULL;
	m_dwID = 0;
	m_bEquipped = false;
	m_dwVID = m_wCell = m_dwCount = m_lFlag = 0;
	m_pProto = NULL;
	m_bExchanging = false;
	memset(&m_alSockets, 0, sizeof(m_alSockets));
#if defined(__ITEM_APPLY_RANDOM__)
	memset(&m_aApplyRandom, 0, sizeof(m_aApplyRandom));
#endif
	memset(&m_aAttr, 0, sizeof(m_aAttr));
#if defined(__SOUL_BIND_SYSTEM__)
	m_lSoulbind = 0;
	m_pkUnbindTimerExpireEvent = NULL;
#endif

	m_pkDestroyEvent = NULL;
	m_pkOwnershipEvent = NULL;
	m_dwOwnershipPID = 0;
	m_pkUniqueExpireEvent = NULL;
	m_pkTimerBasedOnWearExpireEvent = NULL;
	m_pkRealTimeExpireEvent = NULL;
#if defined(__SOUL_SYSTEM__)
	m_pkSoulTimeUseEvent = NULL;
#endif
#if defined(__EXTENDED_BLEND_AFFECT__)
	m_pkBlendUseEvent = NULL;
#endif

	m_pkAccessorySocketExpireEvent = NULL;

	m_bSkipSave = false;
	m_dwLastOwnerPID = 0;
}

void CItem::Destroy()
{
	event_cancel(&m_pkDestroyEvent);
	event_cancel(&m_pkOwnershipEvent);
	event_cancel(&m_pkUniqueExpireEvent);
	event_cancel(&m_pkTimerBasedOnWearExpireEvent);
	event_cancel(&m_pkRealTimeExpireEvent);
	event_cancel(&m_pkAccessorySocketExpireEvent);
#if defined(__SOUL_BIND_SYSTEM__)
	event_cancel(&m_pkUnbindTimerExpireEvent);
#endif
#if defined(__SOUL_SYSTEM__)
	event_cancel(&m_pkSoulTimeUseEvent);
#endif
#if defined(__EXTENDED_BLEND_AFFECT__)
	event_cancel(&m_pkBlendUseEvent);
#endif

	CEntity::Destroy();

	if (GetSectree())
		GetSectree()->RemoveEntity(this);
}

EVENTFUNC(item_destroy_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);

	if (info == NULL)
	{
		sys_err("item_destroy_event> <Factor> Null pointer");
		return 0;
	}

	LPITEM pkItem = info->item;

	if (pkItem->GetOwner())
		sys_err("item_destroy_event: Owner exist. (item %s owner %s)", pkItem->GetName(), pkItem->GetOwner()->GetName());

	pkItem->SetDestroyEvent(NULL);
	M2_DESTROY_ITEM(pkItem);
	return 0;
}

void CItem::SetDestroyEvent(LPEVENT pkEvent)
{
	m_pkDestroyEvent = pkEvent;
}

void CItem::StartDestroyEvent(int iSec)
{
	if (m_pkDestroyEvent)
		return;

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetDestroyEvent(event_create(item_destroy_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::EncodeInsertPacket(LPENTITY ent)
{
	LPDESC d;

	if (!(d = ent->GetDesc()))
		return;

	const PIXEL_POSITION& c_pos = GetXYZ();

	struct packet_item_ground_add pack;

	pack.bHeader = HEADER_GC_ITEM_GROUND_ADD;
	pack.x = c_pos.x;
	pack.y = c_pos.y;
	pack.z = c_pos.z;
	pack.dwVnum = GetVnum();
	pack.dwVID = m_dwVID;
	// pack.count = m_dwCount;

#if defined(__ITEM_DROP_RENEWAL__)
	for (size_t i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pack.alSockets[i] = GetSocket(i);

	thecore_memcpy(pack.aAttrs, GetAttributes(), sizeof(pack.aAttrs));
#endif

	d->Packet(&pack, sizeof(pack));

	if (m_pkOwnershipEvent != NULL)
	{
		item_event_info* info = dynamic_cast<item_event_info*>(m_pkOwnershipEvent->info);

		if (info == NULL)
		{
			sys_err("CItem::EncodeInsertPacket> <Factor> Null pointer");
			return;
		}

		TPacketGCItemOwnership p;

		p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
		p.dwVID = m_dwVID;
		strlcpy(p.szName, info->szOwnerName, sizeof(p.szName));

		d->Packet(&p, sizeof(TPacketGCItemOwnership));
	}
}

void CItem::EncodeRemovePacket(LPENTITY ent)
{
	LPDESC d;

	if (!(d = ent->GetDesc()))
		return;

	struct packet_item_ground_del pack;

	pack.bHeader = HEADER_GC_ITEM_GROUND_DEL;
	pack.dwVID = m_dwVID;

	d->Packet(&pack, sizeof(pack));
	sys_log(2, "Item::EncodeRemovePacket %s to %s", GetName(), ((LPCHARACTER)ent)->GetName());
}

void CItem::SetProto(const TItemTable* table)
{
	assert(table != NULL);
	m_pProto = table;
	SetFlag(m_pProto->dwFlags);
}

void CItem::UsePacketEncode(LPCHARACTER ch, LPCHARACTER victim, struct packet_item_use* packet)
{
	if (!GetVnum())
		return;

	packet->header = HEADER_GC_ITEM_USE;
	packet->ch_vid = ch->GetVID();
	packet->victim_vid = victim->GetVID();
	packet->Cell = TItemPos(GetWindow(), m_wCell);
	packet->vnum = GetVnum();
}

void CItem::RemoveFlag(long bit)
{
	REMOVE_BIT(m_lFlag, bit);
}

void CItem::AddFlag(long bit)
{
	SET_BIT(m_lFlag, bit);
}

void CItem::UpdatePacket()
{
	if (!m_pOwner || !m_pOwner->GetDesc())
		return;

	TPacketGCItemUpdate pack;

	pack.header = HEADER_GC_ITEM_UPDATE;
	pack.Cell = TItemPos(GetWindow(), m_wCell);
	pack.count = m_dwCount;
#if defined(__SOUL_BIND_SYSTEM__)
	pack.soulbind = m_lSoulbind;
#endif

	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
		pack.alSockets[i] = m_alSockets[i];

#if defined(__ITEM_APPLY_RANDOM__)
	thecore_memcpy(pack.aApplyRandom, GetRandomApplies(), sizeof(pack.aApplyRandom));
#endif
	thecore_memcpy(pack.aAttr, GetAttributes(), sizeof(pack.aAttr));
#if defined(__CHANGE_LOOK_SYSTEM__)
	pack.dwTransmutationVnum = GetTransmutationVnum();
#endif

	sys_log(2, "UpdatePacket %s -> %s", GetName(), m_pOwner->GetName());
	m_pOwner->GetDesc()->Packet(&pack, sizeof(pack));
}

DWORD CItem::GetCount()
{
	if (GetType() == ITEM_ELK) return MIN(m_dwCount, LONG_MAX);
#if defined(__CHEQUE_SYSTEM__)
	else if (GetType() == ITEM_CHEQUE) return MIN(m_dwCount, LONG_MAX);
#endif
	else
	{
		return MIN(m_dwCount, gMaxItemCount);
	}
}

bool CItem::SetCount(DWORD count)
{
	if (GetType() == ITEM_ELK)
	{
		m_dwCount = MIN(count, LONG_MAX);
	}
#if defined(__CHEQUE_SYSTEM__)
	else if (GetType() == ITEM_CHEQUE)
	{
		m_dwCount = MIN(count, LONG_MAX);
	}
#endif
	else
	{
		m_dwCount = MIN(count, gMaxItemCount);
	}

	if (count == 0 && m_pOwner)
	{
		if (GetSubType() == USE_ABILITY_UP || GetSubType() == USE_POTION)
		{
			LPCHARACTER pOwner = GetOwner();
			WORD wCell = GetCell();

			RemoveFromCharacter();

			if (!IsDragonSoul())
			{
				LPITEM pItem = pOwner->FindSpecifyItem(GetVnum());

				if (NULL != pItem)
				{
					pOwner->ChainQuickslotItem(pItem, QUICKSLOT_TYPE_ITEM, wCell);
				}
				else
				{
					pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, wCell, WORD_MAX);
				}
			}

			M2_DESTROY_ITEM(this);
		}
		else
		{
			if (!IsDragonSoul())
			{
				m_pOwner->SyncQuickslot(QUICKSLOT_TYPE_ITEM, m_wCell, WORD_MAX);
			}
			M2_DESTROY_ITEM(RemoveFromCharacter());
		}

		return false;
	}

	UpdatePacket();

	Save();
	return true;
}

#if defined(__SOUL_BIND_SYSTEM__)
void CItem::SetSoulBind(long sp)
{
	m_lSoulbind = sp;
	UpdatePacket();
	Save();
}

EVENTFUNC(unbind_timer_expire_event)
{
	const item_vid_event_info* info = reinterpret_cast<const item_vid_event_info*>(event->info);
	if (!info)
		return 0;

	const LPITEM item = ITEM_MANAGER::instance().FindByVID(info->item_vid);
	if (!item)
		return 0;

	const time_t current = get_global_time();
	if (current > item->GetSealedTime())
	{
		item->SetSoulBind(0);
		return 0;
	}

	return PASSES_PER_SEC(1);
}

void CItem::StartUnbindTimerExpireEvent()
{
	if (m_pkUnbindTimerExpireEvent)
		return;

	item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
	info->item_vid = GetVID();

	m_pkUnbindTimerExpireEvent = event_create(unbind_timer_expire_event, info, PASSES_PER_SEC(1));
	sys_log(0, "UNBIND_TIME_EXPIRE: StartUnbindTimerExpireEvent");
}
#endif

LPITEM CItem::RemoveFromCharacter()
{
	if (!m_pOwner)
	{
		sys_err("Item::RemoveFromCharacter owner null");
		return (this);
	}

	LPCHARACTER pOwner = m_pOwner;

	if (m_bEquipped) // �����Ǿ��°�?
	{
		Unequip();
		//pOwner->UpdatePacket();

		SetWindow(RESERVED_WINDOW);
		Save();
		return (this);
	}
	else
	{
		if (GetWindow() != SAFEBOX && GetWindow() != MALL)
		{
			if (IsDragonSoul())
			{
				if (m_wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
					sys_err("CItem::RemoveFromCharacter: pos >= DRAGON_SOUL_INVENTORY_MAX_NUM");
				else
					pOwner->SetItem(TItemPos(m_bWindow, m_wCell), NULL);
			}
			else
			{
				TItemPos cell(INVENTORY, m_wCell);

				if (false == cell.IsDefaultInventoryPosition() && false == cell.IsBeltInventoryPosition()
#if defined(__SPECIAL_INVENTORY_SYSTEM__)
					&& false == cell.IsSkillBookInventoryPosition() && false == cell.IsUpgradeItemsInventoryPosition() && false == cell.IsStoneInventoryPosition() && false == cell.IsGiftBoxInventoryPosition()
#endif
					) // �ƴϸ� ����ǰ��?
				{
					sys_err("CItem::RemoveFromCharacter: Invalid Item Position");
				}
				else
				{
					pOwner->SetItem(cell, NULL);
				}
			}
		}

		m_pOwner = NULL;
		m_wCell = 0;

		SetWindow(RESERVED_WINDOW);
		Save();
		return (this);
	}
}

#if defined(__WJ_PICKUP_ITEM_EFFECT__)
bool CItem::AddToCharacter(LPCHARACTER ch, const TItemPos& Cell, bool isHighLight)
#else
bool CItem::AddToCharacter(LPCHARACTER ch, TItemPos Cell)
#endif
{
	assert(GetSectree() == NULL);
	assert(m_pOwner == NULL);

	WORD pos = Cell.cell;
	BYTE window_type = Cell.window_type;

	if (INVENTORY == window_type)
	{
		if (m_wCell >= INVENTORY_MAX_NUM && BELT_INVENTORY_SLOT_START > m_wCell)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}
	else if (DRAGON_SOUL_INVENTORY == window_type)
	{
		if (m_wCell >= DRAGON_SOUL_INVENTORY_MAX_NUM)
		{
			sys_err("CItem::AddToCharacter: cell overflow: %s to %s cell %d", m_pProto->szName, ch->GetName(), m_wCell);
			return false;
		}
	}

	if (ch->GetDesc())
		m_dwLastOwnerPID = ch->GetPlayerID();

#if defined(__ACCE_COSTUME_SYSTEM__)
	if ((GetType() == ITEM_COSTUME) && (GetSubType() == COSTUME_ACCE) && (GetSocket(ACCE_ABSORPTION_SOCKET) == 0))
	{
		long lVal = GetValue(ACCE_GRADE_VALUE_FIELD);
		switch (lVal)
		{
		case 2:
		{
			lVal = ACCE_GRADE_2_ABS;
		}
		break;
		case 3:
		{
			lVal = ACCE_GRADE_3_ABS;
		}
		break;
		case 4:
		{
			lVal = number(ACCE_GRADE_4_ABS_MIN, ACCE_GRADE_4_ABS_MAX_COMB);
		}
		break;
		default:
		{
			lVal = ACCE_GRADE_1_ABS;
		}
		break;
		}

		SetSocket(ACCE_ABSORPTION_SOCKET, lVal);
	}
#endif

	event_cancel(&m_pkDestroyEvent);

#if defined(__WJ_PICKUP_ITEM_EFFECT__)
	ch->SetItem(TItemPos(window_type, pos), this, isHighLight);
#else
	ch->SetItem(TItemPos(window_type, pos), this);
#endif
	m_pOwner = ch;

	Save();
	return true;
}

LPITEM CItem::RemoveFromGround()
{
	if (GetSectree())
	{
		SetOwnership(NULL);

		GetSectree()->RemoveEntity(this);

		ViewCleanup();

		Save();
	}

	return (this);
}

bool CItem::AddToGround(long lMapIndex, const PIXEL_POSITION& pos, bool skipOwnerCheck)
{
	if (0 == lMapIndex)
	{
		sys_err("wrong map index argument: %d", lMapIndex);
		return false;
	}

	if (GetSectree())
	{
		sys_err("sectree already assigned");
		return false;
	}

	if (!skipOwnerCheck && m_pOwner)
	{
		sys_err("owner pointer not null");
		return false;
	}

	LPSECTREE tree = SECTREE_MANAGER::instance().Get(lMapIndex, pos.x, pos.y);

	if (!tree)
	{
		sys_err("cannot find sectree by %dx%d", pos.x, pos.y);
		return false;
	}

	//tree->Touch();

	SetWindow(GROUND);
	SetXYZ(pos.x, pos.y, pos.z);
	tree->InsertEntity(this);
	UpdateSectree();
	Save();
	return true;
}

bool CItem::DistanceValid(LPCHARACTER ch)
{
	if (!GetSectree())
		return false;

	int iDist = DISTANCE_APPROX(GetX() - ch->GetX(), GetY() - ch->GetY());

	if (iDist > 300)
		return false;

	return true;
}

bool CItem::CanUsedBy(LPCHARACTER ch)
{
	// Anti flag check
	switch (ch->GetJob())
	{
	case JOB_WARRIOR:
		if (GetAntiFlag() & ITEM_ANTIFLAG_WARRIOR)
			return false;
		break;

	case JOB_ASSASSIN:
		if (GetAntiFlag() & ITEM_ANTIFLAG_ASSASSIN)
			return false;
		break;

	case JOB_SHAMAN:
		if (GetAntiFlag() & ITEM_ANTIFLAG_SHAMAN)
			return false;
		break;

	case JOB_SURA:
		if (GetAntiFlag() & ITEM_ANTIFLAG_SURA)
			return false;
		break;

	case JOB_WOLFMAN:
		if (GetAntiFlag() & ITEM_ANTIFLAG_WOLFMAN)
			return false;
		break;

	}

	return true;
}

int CItem::FindEquipCell(LPCHARACTER ch, int iCandidateCell)
{
	// �ڽ��� ������(ITEM_COSTUME)�� WearFlag ��� ��. (sub type���� ������ġ ����. ������ �� wear flag �� �ʿ䰡 �ֳ�..)
	// ��ȥ��(ITEM_DS, ITEM_SPECIAL_DS)�� SUB_TYPE���� ����. �ű� ����, ��Ʈ�� ITEM_TYPE���� ���� -_-
	if ((0 == GetWearFlag() || ITEM_TOTEM == GetType()) && ITEM_COSTUME != GetType() && ITEM_DS != GetType() && ITEM_SPECIAL_DS != GetType() && ITEM_RING != GetType() && ITEM_BELT != GetType())
		return -1;

	// ��ȥ�� ������ WEAR�� ó���� ���� ���(WEAR�� �ִ� 32������ �����ѵ� ��ȥ���� �߰��ϸ� 32�� �Ѵ´�.)
	// �κ��丮�� Ư�� ��ġ((INVENTORY_MAX_NUM + WEAR_MAX_NUM)���� (INVENTORY_MAX_NUM + WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX - 1)����)��
	// ��ȥ�� �������� ����.
	// return �� ����, INVENTORY_MAX_NUM�� �� ������,
	// ���� WearCell�� INVENTORY_MAX_NUM�� ���� return �ϱ� ����.
	if (GetType() == ITEM_DS || GetType() == ITEM_SPECIAL_DS)
	{
		if (iCandidateCell < 0)
		{
			return WEAR_MAX_NUM + GetSubType();
		}

		for (int i = 0; i < DRAGON_SOUL_DECK_MAX_NUM; i++)
		{
			if (WEAR_MAX_NUM + i * DS_SLOT_MAX + GetSubType() == iCandidateCell)
			{
				return iCandidateCell;
			}
		}
		return -1;
	}

	if (GetType() == ITEM_COSTUME)
	{
		if (GetSubType() == COSTUME_BODY)
			return WEAR_COSTUME_BODY;

		if (GetSubType() == COSTUME_HAIR)
			return WEAR_COSTUME_HAIR;

#if defined(__MOUNT_COSTUME_SYSTEM__)
		if (GetSubType() == COSTUME_MOUNT)
			return WEAR_COSTUME_MOUNT;
#endif

#if defined(__ACCE_COSTUME_SYSTEM__)
		if (GetSubType() == COSTUME_ACCE)
			return WEAR_COSTUME_ACCE;
#endif

#if defined(__WEAPON_COSTUME_SYSTEM__)
		if (GetSubType() == COSTUME_WEAPON)
			return WEAR_COSTUME_WEAPON;
#endif
	}
	else if (GetType() == ITEM_RING)
	{
		if (ch->GetWear(WEAR_RING1))
			return WEAR_RING2;
		else
			return WEAR_RING1;
	}
	else if (GetType() == ITEM_BELT)
		return WEAR_BELT;
	else if (GetWearFlag() & WEARABLE_BODY)
		return WEAR_BODY;
	else if (GetWearFlag() & WEARABLE_HEAD)
		return WEAR_HEAD;
	else if (GetWearFlag() & WEARABLE_FOOTS)
		return WEAR_FOOTS;
	else if (GetWearFlag() & WEARABLE_WRIST)
		return WEAR_WRIST;
	else if (GetWearFlag() & WEARABLE_WEAPON)
		return WEAR_WEAPON;
	else if (GetWearFlag() & WEARABLE_SHIELD)
		return WEAR_SHIELD;
	else if (GetWearFlag() & WEARABLE_NECK)
		return WEAR_NECK;
	else if (GetWearFlag() & WEARABLE_EAR)
		return WEAR_EAR;
	else if (GetWearFlag() & WEARABLE_ARROW)
		return WEAR_ARROW;
	else if (GetWearFlag() & WEARABLE_UNIQUE)
	{
		if (ch->GetWear(WEAR_UNIQUE1))
			return WEAR_UNIQUE2;
		else
			return WEAR_UNIQUE1;
	}
#if defined(__PENDANT_SYSTEM__)
	else if (GetSubType() == ARMOR_PENDANT || GetWearFlag() & WEARABLE_PENDANT)
		return WEAR_PENDANT;
#endif
#if defined(__GLOVE_SYSTEM__)
	else if (GetSubType() == ARMOR_GLOVE || GetWearFlag() & WEARABLE_GLOVE)
		return WEAR_GLOVE;
#endif

	// ���� ����Ʈ�� ���� �������� �����°����� �ѹ� ������ ���� �E�� ����.
	else if (GetWearFlag() & WEARABLE_ABILITY)
	{
		if (!ch->GetWear(WEAR_ABILITY1))
		{
			return WEAR_ABILITY1;
		}
		else if (!ch->GetWear(WEAR_ABILITY2))
		{
			return WEAR_ABILITY2;
		}
		else if (!ch->GetWear(WEAR_ABILITY3))
		{
			return WEAR_ABILITY3;
		}
		else if (!ch->GetWear(WEAR_ABILITY4))
		{
			return WEAR_ABILITY4;
		}
		else if (!ch->GetWear(WEAR_ABILITY5))
		{
			return WEAR_ABILITY5;
		}
		else if (!ch->GetWear(WEAR_ABILITY6))
		{
			return WEAR_ABILITY6;
		}
		else if (!ch->GetWear(WEAR_ABILITY7))
		{
			return WEAR_ABILITY7;
		}
		else if (!ch->GetWear(WEAR_ABILITY8))
		{
			return WEAR_ABILITY8;
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

void CItem::ModifyPoints(bool bAdd)
{
	int accessoryGrade;

	// ����� ���ʸ� ������ �����Ų��.
	if (false == IsAccessoryForSocket())
	{
#if defined(__QUIVER_SYSTEM__)
		if ((m_pProto->bType == ITEM_WEAPON && m_pProto->bSubType != WEAPON_QUIVER) || m_pProto->bType == ITEM_ARMOR)
#else
		if (m_pProto->bType == ITEM_WEAPON || m_pProto->bType == ITEM_ARMOR)
#endif
		{
			for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
			{
				DWORD dwVnum;

				if ((dwVnum = GetSocket(i)) <= 2)
					continue;

				TItemTable* p = ITEM_MANAGER::instance().GetTable(dwVnum);
				if (!p)
				{
					sys_err("cannot find table by vnum %lu", dwVnum);
					continue;
				}

				if (ITEM_METIN == p->bType)
				{
					// m_pOwner->ApplyPoint(p->alValues[0], bAdd ? p->alValues[1] : -p->alValues[1]);
					for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
					{
						if (p->aApplies[i].bType == APPLY_NONE)
							continue;

						if (p->aApplies[i].bType == APPLY_SKILL)
							m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? p->aApplies[i].lValue : p->aApplies[i].lValue ^ 0x00800000);
						else
							m_pOwner->ApplyPoint(p->aApplies[i].bType, bAdd ? p->aApplies[i].lValue : -p->aApplies[i].lValue);
					}
				}
			}
		}

		accessoryGrade = 0;
	}
	else
	{
		accessoryGrade = MIN(GetAccessorySocketGrade(), ITEM_ACCESSORY_SOCKET_MAX_NUM);
	}

#if defined(__ACCE_COSTUME_SYSTEM__)
	if (IsCostumeAcce() && GetSocket(ACCE_ABSORBED_SOCKET))
	{
		TItemTable* pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(ACCE_ABSORBED_SOCKET));
		if (pkItemAbsorbed)
		{
			if ((pkItemAbsorbed->bType == ITEM_ARMOR) && (pkItemAbsorbed->bSubType == ARMOR_BODY))
			{
				long lDefGrade = pkItemAbsorbed->alValues[1] + long(pkItemAbsorbed->alValues[5] * 2);
				double dValue = lDefGrade * GetSocket(ACCE_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefGrade = (long)dValue;
				if ((pkItemAbsorbed->alValues[1] > 0 && (lDefGrade <= 0)) || (pkItemAbsorbed->alValues[5] > 0 && (lDefGrade < 1)))
					lDefGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[5] > 0))
					lDefGrade += 1;

				m_pOwner->ApplyPoint(APPLY_DEF_GRADE_BONUS, bAdd ? lDefGrade : -lDefGrade);

				long lDefMagicBonus = pkItemAbsorbed->alValues[0];
				dValue = lDefMagicBonus * GetSocket(ACCE_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lDefMagicBonus = (long)dValue;
				if ((pkItemAbsorbed->alValues[0] > 0) && (lDefMagicBonus < 1))
					lDefMagicBonus += 1;
				else if (pkItemAbsorbed->alValues[0] > 0)
					lDefMagicBonus += 1;

				m_pOwner->ApplyPoint(APPLY_MAGIC_DEF_GRADE, bAdd ? lDefMagicBonus : -lDefMagicBonus);
			}
			else if (pkItemAbsorbed->bType == ITEM_WEAPON)
			{
				long lAttGrade = pkItemAbsorbed->alValues[4] + pkItemAbsorbed->alValues[5];
				if (pkItemAbsorbed->alValues[3] > pkItemAbsorbed->alValues[4])
					lAttGrade = pkItemAbsorbed->alValues[3] + pkItemAbsorbed->alValues[5];

				double dValue = lAttGrade * GetSocket(ACCE_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lAttGrade = (long)dValue;
				if (((pkItemAbsorbed->alValues[3] > 0) && (lAttGrade < 1)) || ((pkItemAbsorbed->alValues[4] > 0) && (lAttGrade < 1)))
					lAttGrade += 1;
				else if ((pkItemAbsorbed->alValues[3] > 0) || (pkItemAbsorbed->alValues[4] > 0))
					lAttGrade += 1;

				m_pOwner->ApplyPoint(APPLY_ATT_GRADE_BONUS, bAdd ? lAttGrade : -lAttGrade);

				long lAttMagicGrade = pkItemAbsorbed->alValues[2] + pkItemAbsorbed->alValues[5];
				if (pkItemAbsorbed->alValues[1] > pkItemAbsorbed->alValues[2])
					lAttMagicGrade = pkItemAbsorbed->alValues[1] + pkItemAbsorbed->alValues[5];

				dValue = lAttMagicGrade * GetSocket(ACCE_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lAttMagicGrade = (long)dValue;
				if (((pkItemAbsorbed->alValues[1] > 0) && (lAttMagicGrade < 1)) || ((pkItemAbsorbed->alValues[2] > 0) && (lAttMagicGrade < 1)))
					lAttMagicGrade += 1;
				else if ((pkItemAbsorbed->alValues[1] > 0) || (pkItemAbsorbed->alValues[2] > 0))
					lAttMagicGrade += 1;

				m_pOwner->ApplyPoint(APPLY_MAGIC_ATT_GRADE, bAdd ? lAttMagicGrade : -lAttMagicGrade);
			}
		}
	}
#endif

	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
#if defined(__ACCE_COSTUME_SYSTEM__)
		if (m_pProto->aApplies[i].bType == APPLY_NONE && !IsCostumeAcce())
#else
		if (m_pProto->aApplies[i].bType == APPLY_NONE)
#endif
			continue;

		BYTE bType = m_pProto->aApplies[i].bType;
		long lValue = m_pProto->aApplies[i].lValue;

#if defined(__ACCE_COSTUME_SYSTEM__)
		if (IsCostumeAcce())
		{
			TItemTable* pkItemAbsorbed = ITEM_MANAGER::instance().GetTable(GetSocket(ACCE_ABSORBED_SOCKET));
			if (pkItemAbsorbed)
			{
				if (pkItemAbsorbed->aApplies[i].bType == APPLY_NONE)
					continue;

				bType = pkItemAbsorbed->aApplies[i].bType;
				lValue = pkItemAbsorbed->aApplies[i].lValue;
				if (lValue < 0)
					continue;

				double dValue = lValue * GetSocket(ACCE_ABSORPTION_SOCKET);
				dValue = (double)dValue / 100;
				dValue = (double)dValue + .5;
				lValue = (long)dValue;

				if ((pkItemAbsorbed->aApplies[i].lValue > 0) && (lValue <= 0))
					lValue += 1;

				if (m_pProto->aApplies[i].bType == APPLY_SKILL)
				{
					m_pOwner->ApplyPoint(bType, bAdd ? lValue : lValue ^ 0x00800000);
				}
				else
				{
					if (0 != accessoryGrade)
						lValue += MAX(accessoryGrade, lValue * aiAccessorySocketEffectivePct[accessoryGrade] / 100);

					m_pOwner->ApplyPoint(bType, bAdd ? lValue : -lValue);
				}

#if defined(__ITEM_APPLY_RANDOM__)
				if (pkItemAbsorbed->aApplies[i].bType == APPLY_RANDOM)
				{
					BYTE byRandomType = GetRandomApply(i).bType;
					short sRandomValue = GetRandomApply(i).sValue;
					double dRandomValue = sRandomValue * GetSocket(ACCE_ABSORPTION_SOCKET);
					dRandomValue = (double)dRandomValue / 100;
					dRandomValue = (double)dRandomValue + .5;
					sRandomValue = (int)dRandomValue;

					if (bType == APPLY_SKILL)
					{
						m_pOwner->ApplyPoint(byRandomType, bAdd ? sRandomValue : sRandomValue ^ 0x00800000);
					}
					else
					{
						m_pOwner->ApplyPoint(byRandomType, bAdd ? sRandomValue : -sRandomValue);
					}
				}
#endif
			}
			else
				continue;
		}
#endif

		if (m_pProto->aApplies[i].bType == APPLY_SKILL)
		{
			m_pOwner->ApplyPoint(m_pProto->aApplies[i].bType, bAdd ? lValue : lValue ^ 0x00800000);
		}
		else
		{
			if (0 != accessoryGrade)
				lValue += MAX(accessoryGrade, lValue * aiAccessorySocketEffectivePct[accessoryGrade] / 100);

			m_pOwner->ApplyPoint(m_pProto->aApplies[i].bType, bAdd ? lValue : -lValue);
		}
	}

#if defined(__ITEM_APPLY_RANDOM__)
	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		if (m_pProto->aApplies[i].bType == APPLY_RANDOM)
		{
			BYTE bType = GetRandomApply(i).bType;
			short sValue = GetRandomApply(i).sValue;

				if (bType == APPLY_SKILL)
				{
					m_pOwner->ApplyPoint(bType, bAdd ? sValue : sValue ^ 0x00800000);
				}
				else
				{
					if (0 != accessoryGrade)
						sValue += MAX(accessoryGrade, sValue * aiAccessorySocketEffectivePct[accessoryGrade] / 100);

					m_pOwner->ApplyPoint(bType, bAdd ? sValue : -sValue);
				}
		}
	}
#endif

	// �ʽ´��� ����, �ҷ��� ����, �ູ�� ����, ������ ����� ���Ʈ�� ���
	// ������ �ϵ� �ڵ����� ������ �Ӽ��� �ο�������,
	// �� �κ��� �����ϰ� special item group ���̺��� �Ӽ��� �ο��ϵ��� �����Ͽ���.
	// ������ �ϵ� �ڵ��Ǿ����� �� ������ �������� �������� ���� �־ Ư��ó�� �س��´�.
	// �� �����۵��� ���, �ؿ� ITEM_UNIQUE�� ���� ó���� �Ӽ��� �ο��Ǳ� ������,
	// �����ۿ� �����ִ� attribute�� �������� �ʰ� �Ѿ��.
	//if (true == CItemVnumHelper::IsRamadanMoonRing(GetVnum()) || true == CItemVnumHelper::IsHalloweenCandy(GetVnum())
		//|| true == CItemVnumHelper::IsHappinessRing(GetVnum()) || true == CItemVnumHelper::IsLovePendant(GetVnum()))
	if (CItemVnumHelper::IsUniqueItem(GetVnum()))
	{
		// Do not anything.
	}
	else
	{
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			if (GetAttributeType(i))
			{
				const TPlayerItemAttribute& ia = GetAttribute(i);
#if defined(__ACCE_COSTUME_SYSTEM__)
				long sValue = ia.sValue;

				if (IsCostumeAcce())
				{
					double dValue = sValue * GetSocket(ACCE_ABSORPTION_SOCKET);
					dValue = (double)dValue / 100;
					dValue = (double)dValue + .5;
					sValue = (long)dValue;
					if ((ia.sValue > 0) && (sValue <= 0))
						sValue += 1;
				}

				if (ia.bType == APPLY_SKILL)
					m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : sValue ^ 0x00800000);
				else
					m_pOwner->ApplyPoint(ia.bType, bAdd ? sValue : -sValue);
#else
				if (ia.bType == APPLY_SKILL)
					m_pOwner->ApplyPoint(ia.bType, bAdd ? ia.sValue : ia.sValue ^ 0x00800000);
				else
					m_pOwner->ApplyPoint(ia.bType, bAdd ? ia.sValue : -ia.sValue);
#endif
			}
		}
	}

	switch (m_pProto->bType)
	{
	case ITEM_PICK:
	case ITEM_ROD:
	{
		if (bAdd)
		{
			if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
				m_pOwner->SetPart(PART_WEAPON, GetVnum());
		}
		else
		{
			if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
				m_pOwner->SetPart(PART_WEAPON, m_pOwner->GetOriginalPart(PART_WEAPON));
		}
	}
	break;

	case ITEM_WEAPON:
	{
#if defined(__WEAPON_COSTUME_SYSTEM__)
		// �ڽ��� body�� �԰��ִٸ� armor�� ���� �Դ� ��� ���� ���־� ������ �ָ� �� ��.
		if (0 != m_pOwner->GetWear(WEAR_COSTUME_WEAPON))
			return;
#endif

		if (bAdd)
		{
#if defined(__CHANGE_LOOK_SYSTEM__)
			if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
				m_pOwner->SetPart(PART_WEAPON, GetTransmutationVnum() != 0 ? GetTransmutationVnum() : GetVnum());
#else
			if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
				m_pOwner->SetPart(PART_WEAPON, GetVnum());
#endif
		}
		else
		{
			if (m_wCell == INVENTORY_MAX_NUM + WEAR_WEAPON)
				m_pOwner->SetPart(PART_WEAPON, m_pOwner->GetOriginalPart(PART_WEAPON));
		}
	}
	break;

	case ITEM_ARMOR:
	{
#if defined(__COSTUME_SYSTEM__)
		// �ڽ��� body�� �԰��ִٸ� armor�� ���� �Դ� ��� ���� ���־� ������ �ָ� �� ��.
		if (0 != m_pOwner->GetWear(WEAR_COSTUME_BODY))
			break;
#endif

		if (GetSubType() == ARMOR_BODY || GetSubType() == ARMOR_HEAD || GetSubType() == ARMOR_FOOTS || GetSubType() == ARMOR_SHIELD)
		{
			if (bAdd)
			{
#if defined(__CHANGE_LOOK_SYSTEM__)
				if (GetProto()->bSubType == ARMOR_BODY)
					m_pOwner->SetPart(PART_MAIN, GetTransmutationVnum() != 0 ? GetTransmutationVnum() : GetVnum());
#else
				if (GetProto()->bSubType == ARMOR_BODY)
					m_pOwner->SetPart(PART_MAIN, GetVnum());
#endif
			}
			else
			{
				if (GetProto()->bSubType == ARMOR_BODY)
					m_pOwner->SetPart(PART_MAIN, m_pOwner->GetOriginalPart(PART_MAIN));
			}
		}
	}
	break;

#if defined(__COSTUME_SYSTEM__)
	// �ڽ��� ������ �Ծ��� �� ĳ���� parts ���� ����. ���� ��Ÿ�ϴ�� �߰���..
	case ITEM_COSTUME:
	{
		DWORD toSetValue = this->GetVnum();
		EParts toSetPart = PART_MAX_NUM;

		switch (GetSubType())
		{
		// ���� �ڽ���
		case COSTUME_BODY:
		{
			toSetPart = PART_MAIN;

			if (false == bAdd)
			{
				// �ڽ��� ������ ������ �� ���� ������ �԰� �־��ٸ� �� �������� look ����, ���� �ʾҴٸ� default look
				const CItem* pArmor = m_pOwner->GetWear(WEAR_BODY);
#if defined(__CHANGE_LOOK_SYSTEM__)
				if (pArmor)
					toSetValue = pArmor->GetTransmutationVnum() != 0 ? pArmor->GetTransmutationVnum() : pArmor->GetVnum();
				else
					toSetValue = m_pOwner->GetOriginalPart(PART_MAIN);
#else
				toSetValue = (NULL != pArmor) ? pArmor->GetVnum() : m_pOwner->GetOriginalPart(PART_MAIN);
#endif
			}
#if defined(__CHANGE_LOOK_SYSTEM__)
			else
				toSetValue = GetTransmutationVnum() != 0 ? GetTransmutationVnum() : GetVnum();
#endif
		}
		break;

		// ��� �ڽ���
		case COSTUME_HAIR:
		{
			toSetPart = PART_HAIR;

			// �ڽ��� ���� shape���� item proto�� value3�� �����ϵ��� ��. Ư���� ������ ���� ���� ����(ARMOR_BODY)�� shape���� �������� value3�� �־ �� ���� value3���� ��.
			// [NOTE] ������ ������ vnum�� ������ ���� shape(value3)���� ������ ������.. ���� �ý����� �׷��� �Ǿ�����...
#if defined(__CHANGE_LOOK_SYSTEM__)
			const DWORD c_dwTransmutationVnum = GetTransmutationVnum();
			if (c_dwTransmutationVnum != 0)
			{
				TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(c_dwTransmutationVnum);
				toSetValue = (pItemTable != NULL) ? pItemTable->alValues[3] : GetValue(3);
			}
			else
				toSetValue = (true == bAdd) ? this->GetValue(3) : 0;
#else
			toSetValue = (true == bAdd) ? this->GetValue(3) : 0;
#endif
		}
		break;

#if defined(__ACCE_COSTUME_SYSTEM__)
		case COSTUME_ACCE:
		{
			toSetPart = PART_ACCE;

			toSetValue -= 85000;
			if (GetSocket(ACCE_ABSORPTION_SOCKET) >= ACCE_EFFECT_FROM_ABS)
				toSetValue += 1500;

			toSetValue = (bAdd == true) ? toSetValue : 0;
		}
		break;
#endif

#if defined(__WEAPON_COSTUME_SYSTEM__)
		case COSTUME_WEAPON:
		{
			toSetPart = PART_WEAPON;

			if (false == bAdd)
			{
				// �ڽ��� ������ ������ �� ���� ������ �԰� �־��ٸ� �� �������� look ����, ���� �ʾҴٸ� default look
				const CItem* pWeapon = m_pOwner->GetWear(WEAR_WEAPON);
#if defined(__CHANGE_LOOK_SYSTEM__)
				if (pWeapon)
					toSetValue = pWeapon->GetTransmutationVnum() != 0 ? pWeapon->GetTransmutationVnum() : pWeapon->GetVnum();
				else
					toSetValue = m_pOwner->GetOriginalPart(PART_WEAPON);
#else
				toSetValue = (NULL != pWeapon) ? pWeapon->GetVnum() : m_pOwner->GetOriginalPart(PART_WEAPON);
#endif
			}
#if defined(__CHANGE_LOOK_SYSTEM__)
			else
				toSetValue = GetTransmutationVnum() != 0 ? GetTransmutationVnum() : GetVnum();
#endif
		}
		break;
#endif
		
		} // end switch

		if (PART_MAX_NUM != toSetPart)
		{
			m_pOwner->SetPart((BYTE)toSetPart, toSetValue);
			m_pOwner->UpdatePacket();
		}
	}
	break;
#endif

	case ITEM_UNIQUE:
	{
		sys_log(0, "ITEM_UNIQUE Process");
		if (0 != GetSIGVnum())
		{
			sys_log(0, "ITEM_UNIQUE Contains SIGVnum %d", GetSIGVnum());

			const CSpecialItemGroup* pItemGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(GetSIGVnum());
			if (NULL == pItemGroup)
			{
				sys_log(0, "ITEM_UNIQUE No pointer for item group?");
				break;
			}

			DWORD dwAttrVnum = pItemGroup->GetAttrVnum(GetVnum());
			const CSpecialAttrGroup* pAttrGroup = ITEM_MANAGER::instance().GetSpecialAttrGroup(dwAttrVnum);
			if (NULL == pAttrGroup)
			{
				sys_log(0, "ITEM_UNIQUE No pointer for attr group?");
				break;
			}

			for (itertype(pAttrGroup->m_vecAttrs) it = pAttrGroup->m_vecAttrs.begin(); it != pAttrGroup->m_vecAttrs.end(); it++)
			{
				m_pOwner->ApplyPoint(it->apply_type, bAdd ? it->apply_value : -it->apply_value);
			}
		}
	}
	break;

	}
}

bool CItem::IsEquipable() const
{
	switch (this->GetType())
	{
	case ITEM_COSTUME:
	case ITEM_ARMOR:
	case ITEM_WEAPON:
	case ITEM_ROD:
	case ITEM_PICK:
	case ITEM_UNIQUE:
	case ITEM_DS:
	case ITEM_SPECIAL_DS:
	case ITEM_RING:
	case ITEM_BELT:
		return true;
	}

	return false;
}

// return false on error state
bool CItem::EquipTo(LPCHARACTER ch, BYTE bWearCell)
{
	if (!ch)
	{
		sys_err("EquipTo: nil character");
		return false;
	}

	// ��ȥ�� ���� index�� WEAR_MAX_NUM ���� ŭ.
	if (IsDragonSoul())
	{
		if (bWearCell < WEAR_MAX_NUM || bWearCell >= WEAR_MAX_NUM + DRAGON_SOUL_DECK_MAX_NUM * DS_SLOT_MAX)
		{
			sys_err("EquipTo: invalid dragon soul cell (this: #%d %s wearflag: %d cell: %d)", GetOriginalVnum(), GetName(), GetSubType(), bWearCell - WEAR_MAX_NUM);
			return false;
		}
	}
	else
	{
		if (bWearCell >= WEAR_MAX_NUM)
		{
			sys_err("EquipTo: invalid wear cell (this: #%d %s wearflag: %d cell: %d)", GetOriginalVnum(), GetName(), GetWearFlag(), bWearCell);
			return false;
		}
	}

	if (ch->GetWear(bWearCell))
	{
		sys_err("EquipTo: item already exist (this: #%d %s cell: %d %s)", GetOriginalVnum(), GetName(), bWearCell, ch->GetWear(bWearCell)->GetName());
		return false;
	}

	if (GetOwner())
		RemoveFromCharacter();

	ch->SetWear(bWearCell, this); // ���⼭ ��Ŷ ����

	m_pOwner = ch;
	m_bEquipped = true;
	m_wCell = INVENTORY_MAX_NUM + bWearCell;

	DWORD dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; ++i)
	{
		if (m_pOwner->GetWear(i))
		{
			SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->GetRealImmuneFlag());
		}
	}

	m_pOwner->SetImmuneFlag(dwImmuneFlag);

	if (IsDragonSoul())
	{
		DSManager::instance().ActivateDragonSoul(this);
#if defined(__DS_SET__)
		ch->DragonSoul_HandleSetBonus();
#endif
	}
	else
	{
		ModifyPoints(true);
		StartUniqueExpireEvent();
		if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
			StartTimerBasedOnWearExpireEvent();

		// ACCESSORY_REFINE
		StartAccessorySocketExpireEvent();
		// END_OF_ACCESSORY_REFINE
	}

	ch->BuffOnAttr_AddBuffsFromItem(this);

	m_pOwner->ComputeBattlePoints();

	m_pOwner->UpdatePacket();

	Save();

	return (true);
}

bool CItem::Unequip()
{
	if (!m_pOwner || GetCell() < INVENTORY_MAX_NUM)
	{
		// ITEM_OWNER_INVALID_PTR_BUG
		sys_err("%s %u m_pOwner %p, GetCell %d",
			GetName(), GetID(), get_pointer(m_pOwner), GetCell());
		// END_OF_ITEM_OWNER_INVALID_PTR_BUG
		return false;
	}

	if (this != m_pOwner->GetWear(GetCell() - INVENTORY_MAX_NUM))
	{
		sys_err("m_pOwner->GetWear() != this");
		return false;
	}

	// �ű� �� ������ ���Ž� ó��
	if (IsRideItem())
		ClearMountAttributeAndAffect();

	if (IsDragonSoul())
	{
		DSManager::instance().DeactivateDragonSoul(this);
#if defined(__DS_SET__)
		m_pOwner->DragonSoul_HandleSetBonus();
#endif
	}
	else
	{
		ModifyPoints(false);
	}

	StopUniqueExpireEvent();

	if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
		StopTimerBasedOnWearExpireEvent();

	// ACCESSORY_REFINE
	StopAccessorySocketExpireEvent();
	// END_OF_ACCESSORY_REFINE

	m_pOwner->BuffOnAttr_RemoveBuffsFromItem(this);

	m_pOwner->SetWear(GetCell() - INVENTORY_MAX_NUM, NULL);

	DWORD dwImmuneFlag = 0;

	for (int i = 0; i < WEAR_MAX_NUM; ++i)
	{
		if (m_pOwner->GetWear(i))
		{
			SET_BIT(dwImmuneFlag, m_pOwner->GetWear(i)->GetRealImmuneFlag());
		}
	}

	m_pOwner->SetImmuneFlag(dwImmuneFlag);

	m_pOwner->ComputeBattlePoints();

	m_pOwner->UpdatePacket();

	m_pOwner = NULL;
	m_wCell = 0;
	m_bEquipped = false;

	return true;
}

long CItem::GetValue(DWORD idx)
{
	assert(idx < ITEM_VALUES_MAX_NUM);
	return GetProto()->alValues[idx];
}

void CItem::SetExchanging(bool bOn)
{
	m_bExchanging = bOn;
}

void CItem::Save()
{
	if (m_bSkipSave)
		return;

	ITEM_MANAGER::instance().DelayedSave(this);
}

bool CItem::CreateSocket(BYTE bSlot, BYTE bGold)
{
	assert(bSlot < ITEM_SOCKET_MAX_NUM);

	if (m_alSockets[bSlot] != 0)
	{
		sys_err("Item::CreateSocket : socket already exist %s %d", GetName(), bSlot);
		return false;
	}

	if (bGold)
		m_alSockets[bSlot] = 2;
	else
		m_alSockets[bSlot] = 1;

	UpdatePacket();

	Save();
	return true;
}

void CItem::SetSockets(const long* c_al)
{
	thecore_memcpy(m_alSockets, c_al, sizeof(m_alSockets));
	Save();
}

void CItem::SetSocket(int i, long v, bool bLog)
{
	assert(i < ITEM_SOCKET_MAX_NUM);
	m_alSockets[i] = v;
	UpdatePacket();
	Save();
	if (bLog)
		LogManager::instance().ItemLog(i, v, 0, GetID(), "SET_SOCKET", "", "", GetOriginalVnum());
}

int CItem::GetGold()
{
	if (IS_SET(GetFlag(), ITEM_FLAG_COUNT_PER_1GOLD))
	{
		if (GetProto()->dwGold == 0)
			return GetCount();
		else
			return GetCount() / GetProto()->dwGold;
	}
	else
		return GetProto()->dwGold;
}

int CItem::GetShopBuyPrice()
{
	return GetProto()->dwShopBuyPrice;
}

bool CItem::IsOwnership(LPCHARACTER ch)
{
	if (!m_pkOwnershipEvent)
		return true;

	return m_dwOwnershipPID == ch->GetPlayerID() ? true : false;
}

EVENTFUNC(ownership_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);

	if (info == NULL)
	{
		sys_err("ownership_event> <Factor> Null pointer");
		return 0;
	}

	LPITEM pkItem = info->item;

	pkItem->SetOwnershipEvent(NULL);

	TPacketGCItemOwnership p;

	p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
	p.dwVID = pkItem->GetVID();
	p.szName[0] = '\0';

	pkItem->PacketAround(&p, sizeof(p));
	return 0;
}

void CItem::SetOwnershipEvent(LPEVENT pkEvent)
{
	m_pkOwnershipEvent = pkEvent;
}

void CItem::SetOwnership(LPCHARACTER ch, int iSec)
{
	if (!ch)
	{
		if (m_pkOwnershipEvent)
		{
			event_cancel(&m_pkOwnershipEvent);
			m_dwOwnershipPID = 0;

			TPacketGCItemOwnership p;

			p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
			p.dwVID = m_dwVID;
			p.szName[0] = '\0';

			PacketAround(&p, sizeof(p));
		}
		return;
	}

	if (m_pkOwnershipEvent)
		return;

	if (true == LC_IsEurope())
	{
		if (iSec <= 10)
			iSec = 30;
	}

	m_dwOwnershipPID = ch->GetPlayerID();

	item_event_info* info = AllocEventInfo<item_event_info>();
	strlcpy(info->szOwnerName, ch->GetName(), sizeof(info->szOwnerName));
	info->item = this;

	SetOwnershipEvent(event_create(ownership_event, info, PASSES_PER_SEC(iSec)));

	TPacketGCItemOwnership p;

	p.bHeader = HEADER_GC_ITEM_OWNERSHIP;
	p.dwVID = m_dwVID;
	strlcpy(p.szName, ch->GetName(), sizeof(p.szName));

	PacketAround(&p, sizeof(p));
}

int CItem::GetSocketCount()
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; i++)
	{
		if (GetSocket(i) == 0)
			return i;
	}
	return ITEM_SOCKET_MAX_NUM;
}

bool CItem::AddSocket()
{
	int count = GetSocketCount();
	if (count == ITEM_SOCKET_MAX_NUM)
		return false;
	m_alSockets[count] = 1;
	return true;
}

void CItem::AlterToSocketItem(int iSocketCount)
{
	if (iSocketCount >= ITEM_SOCKET_MAX_NUM)
	{
		sys_log(0, "Invalid Socket Count %d, set to maximum", ITEM_SOCKET_MAX_NUM);
		iSocketCount = ITEM_SOCKET_MAX_NUM;
	}

	for (int i = 0; i < iSocketCount; ++i)
		SetSocket(i, 1);
}

void CItem::AlterToMagicItem(int iSecondPct /*= 0*/, int iThirdPct /*= 0 */)
{
	int idx = GetAttributeSetIndex();

	if (idx < 0)
		return;

	//			Appeariance	Second	Third
	// Weapon	50			20		5
	// Armor	30			10		2
	// Acc		20			10		1

	//int iSecondPct;
	//int iThirdPct;

	switch (GetType())
	{
	case ITEM_WEAPON:
		iSecondPct = 20;
		iThirdPct = 5;
		break;
	case ITEM_COSTUME:
		iSecondPct = 30;
		iThirdPct = 20;
		break;
	case ITEM_ARMOR:
		if (GetSubType() == ARMOR_BODY)
		{
			iSecondPct = 10;
			iThirdPct = 2;
		}
		else
		{
			iSecondPct = 10;
			iThirdPct = 1;
		}
		break;

	default:
		return;
	}

	// 100% Ȯ���� ���� �Ӽ� �ϳ�
	PutAttribute(aiItemMagicAttributePercentHigh);

	if (number(1, 100) <= iSecondPct)
		PutAttribute(aiItemMagicAttributePercentLow);

	if (number(1, 100) <= iThirdPct)
		PutAttribute(aiItemMagicAttributePercentLow);
}

DWORD CItem::GetRefineFromVnum()
{
	return ITEM_MANAGER::instance().GetRefineFromVnum(GetVnum());
}

int CItem::GetRefineLevel()
{
	const char* name = GetBaseName();
	char* p = const_cast<char*>(strrchr(name, '+'));

	if (!p)
		return 0;

	int rtn = 0;
	str_to_number(rtn, p + 1);

	const char* locale_name = GetLocaleName();

	p = const_cast<char*>(strrchr(locale_name, '+'));

	if (p)
	{
		int locale_rtn = 0;
		str_to_number(locale_rtn, p + 1);
		if (locale_rtn != rtn)
		{
			sys_err("refine_level_based_on_NAME(%d) is not equal to refine_level_based_on_LOCALE_NAME(%d).", rtn, locale_rtn);
		}
	}

	return rtn;
}

bool CItem::IsPolymorphItem()
{
	return GetType() == ITEM_POLYMORPH;
}

EVENTFUNC(unique_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);

	if (info == NULL)
	{
		sys_err("unique_expire_event> <Factor> Null pointer");
		return 0;
	}

	LPITEM pkItem = info->item;

	if (pkItem->GetValue(2) == 0)
	{
		if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) <= 1)
		{
			sys_log(0, "UNIQUE_ITEM: expire %s %u", pkItem->GetName(), pkItem->GetID());
			pkItem->SetUniqueExpireEvent(NULL);
			ITEM_MANAGER::instance().RemoveItem(pkItem, "UNIQUE_EXPIRE");
			return 0;
		}
		else
		{
			pkItem->SetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME, pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - 1);
			return PASSES_PER_SEC(60);
		}
	}
	else
	{
		time_t cur = get_global_time();

		if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) <= cur)
		{
			pkItem->SetUniqueExpireEvent(NULL);
			ITEM_MANAGER::instance().RemoveItem(pkItem, "UNIQUE_EXPIRE");
			return 0;
		}
		else
		{
			// ���� ���� �ð��� �����۵��� ���������ϰ� ������� �ʴ� ���װ� �־�
			// ����
			// by rtsummit
			if (pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - cur < 600)
				return PASSES_PER_SEC(pkItem->GetSocket(ITEM_SOCKET_UNIQUE_REMAIN_TIME) - cur);
			else
				return PASSES_PER_SEC(600);
		}
	}
}

// �ð� �ĺ���
// timer�� ������ ���� �ð� �����ϴ� ���� �ƴ϶�, 
// timer�� ��ȭ�� ���� timer�� ������ �ð� ��ŭ �ð� ������ �Ѵ�.
EVENTFUNC(timer_based_on_wear_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);

	if (info == NULL)
	{
		sys_err("expire_event <Factor> Null pointer");
		return 0;
	}

	LPITEM pkItem = info->item;
	int remain_time = pkItem->GetSocket(ITEM_SOCKET_REMAIN_SEC) - processing_time / passes_per_sec;
	if (remain_time <= 0)
	{
		sys_log(0, "ITEM EXPIRED : expired %s %u", pkItem->GetName(), pkItem->GetID());
		pkItem->SetTimerBasedOnWearExpireEvent(NULL);
		pkItem->SetSocket(ITEM_SOCKET_REMAIN_SEC, 0);

		// �ϴ� timer based on wear ��ȥ���� �ð� �� �Ǿ��ٰ� ������ �ʴ´�.
		if (pkItem->IsDragonSoul())
		{
			DSManager::instance().DeactivateDragonSoul(pkItem);
		}
		else
		{
			ITEM_MANAGER::instance().RemoveItem(pkItem, "TIMER_BASED_ON_WEAR_EXPIRE");
		}
		return 0;
	}
	pkItem->SetSocket(ITEM_SOCKET_REMAIN_SEC, remain_time);
	return PASSES_PER_SEC(MIN(60, remain_time));
}

void CItem::SetUniqueExpireEvent(LPEVENT pkEvent)
{
	m_pkUniqueExpireEvent = pkEvent;
}

void CItem::SetTimerBasedOnWearExpireEvent(LPEVENT pkEvent)
{
	m_pkTimerBasedOnWearExpireEvent = pkEvent;
}

EVENTFUNC(real_time_expire_event)
{
	const item_vid_event_info* info = reinterpret_cast<const item_vid_event_info*>(event->info);

	if (NULL == info)
		return 0;

	const LPITEM item = ITEM_MANAGER::instance().FindByVID(info->item_vid);

	if (NULL == item)
		return 0;

	const time_t current = get_global_time();

	if (current > item->GetSocket(0))
	{
#if defined(__CHANGE_LOOK_SYSTEM__)
		if (LPCHARACTER lpCh = item->GetOwner())
		{
			CChangeLook* pChangeLook = lpCh->GetChangeLook();
			if (pChangeLook)
				pChangeLook->Clear();
		}
#endif
		ITEM_MANAGER::instance().RemoveItem(item, "REAL_TIME_EXPIRE");
		return 0;
	}

	return PASSES_PER_SEC(1);
}

void CItem::StartRealTimeExpireEvent()
{
	if (m_pkRealTimeExpireEvent)
		return;

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType || LIMIT_REAL_TIME_START_FIRST_USE == GetProto()->aLimits[i].bType)
		{
			item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
			info->item_vid = GetVID();

			m_pkRealTimeExpireEvent = event_create(real_time_expire_event, info, PASSES_PER_SEC(1));

			sys_log(0, "REAL_TIME_EXPIRE: StartRealTimeExpireEvent");

			return;
		}
	}
}

#if defined(__SOUL_SYSTEM__)
EVENTFUNC(soul_time_use_event)
{
	const item_vid_event_info* info = reinterpret_cast<const item_vid_event_info*>(event->info);

	if (NULL == info)
	{
		sys_err("expire_event <Factor> Null pointer");
		return 0;
	}

	const LPITEM item = ITEM_MANAGER::instance().FindByVID(info->item_vid);

	if (NULL == item)
		return 0;

	item->SetSoulKeepTime();

	int iSec = test_server ? 1 : 60;
	return PASSES_PER_SEC(iSec);
}

void CItem::StartSoulTimeUseEvent()
{
	if (m_pkSoulTimeUseEvent)
		return;

	if (GetType() == ITEM_SOUL)
	{
		int iSec = test_server ? 1 : 60;
		item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
		info->item_vid = GetVID();
		m_pkSoulTimeUseEvent = event_create(soul_time_use_event, info, PASSES_PER_SEC(iSec));

		sys_log(0, "SOUL_TIME_USE: StartSoulTimeUseEvent");

		return;
	}
}
#endif

#if defined(__EXTENDED_BLEND_AFFECT__)
EVENTFUNC(blend_use_expire_event)
{
	item_event_info* info = dynamic_cast<item_event_info*>(event->info);

	if (info == NULL)
	{
		sys_err("expire_event <Factor> Null pointer");
		return 0;
	}

	LPITEM pkItem = info->item;
	int remain_time = pkItem->GetSocket(2) - processing_time / passes_per_sec;
	if (remain_time <= 0)
	{
		pkItem->SetSocket(2, 0);
		pkItem->SetSocket(3, false);
		pkItem->Lock(false);
		ITEM_MANAGER::instance().RemoveItem(pkItem, "BLEND_USE_EXPIRE");
		return 0;
	}

	pkItem->SetSocket(2, remain_time);
	return PASSES_PER_SEC(MIN(1, remain_time));
}

void CItem::StartBlendExpireEvent()
{
	if (m_pkBlendUseEvent)
		return;

	if (IsRealTimeItem())
		return;

	if (!IsBlendItem())
		return;

	int iSec = GetSocket(2);

	if (iSec >= 8640000)
		return;

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	m_pkBlendUseEvent = event_create(blend_use_expire_event, info, PASSES_PER_SEC(1));
}

void CItem::StopBlendExpireEvent()
{
	if (!m_pkBlendUseEvent)
		return;

	int remain_time = GetSocket(2) - event_processing_time(m_pkBlendUseEvent) / passes_per_sec;

	SetSocket(2, remain_time);
	event_cancel(&m_pkBlendUseEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}
#endif

bool CItem::IsRealTimeItem()
{
	if (!GetProto())
		return false;

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType)
			return true;
	}

	return false;
}

bool CItem::IsUsedTimeItem()
{
	return m_pkRealTimeExpireEvent != NULL;
}

void CItem::StartUniqueExpireEvent()
{
	if (!m_pOwner)
	{
		// sys_err("Item::StartUniqueExpireEvent owner null");
		return;
	}

#if defined(__MOUNT_COSTUME_SYSTEM__)
	if (GetType() != ITEM_UNIQUE && !IsRideItem())
		return;
#else
	if (GetType() != ITEM_UNIQUE)
		return;
#endif

	if (m_pkUniqueExpireEvent)
		return;

	// �Ⱓ�� �������� ��� �ð��� �������� �������� �ʴ´�
	if (IsRealTimeItem())
		return;

	// HARD CODING
	if (GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		m_pOwner->ShowAlignment(false);

	int iSec = GetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME);

	if (iSec == 0)
		iSec = 60;
	else
		iSec = MIN(iSec, 60);

	SetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME, 0);

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetUniqueExpireEvent(event_create(unique_expire_event, info, PASSES_PER_SEC(iSec)));
}

// �ð� �ĺ���
// timer_based_on_wear_expire_event ���� ����
void CItem::StartTimerBasedOnWearExpireEvent()
{
	if (m_pkTimerBasedOnWearExpireEvent)
		return;

	//�Ⱓ�� �������� ��� �ð��� �������� �������� �ʴ´�
	if (IsRealTimeItem())
		return;

	if (-1 == GetProto()->cLimitTimerBasedOnWearIndex)
		return;

	int iSec = GetSocket(0);

	if (0 != iSec)
	{
		iSec %= 60;
		if (0 == iSec)
			iSec = 60;
	}

	item_event_info* info = AllocEventInfo<item_event_info>();
	info->item = this;

	SetTimerBasedOnWearExpireEvent(event_create(timer_based_on_wear_expire_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::StopUniqueExpireEvent()
{
	if (!m_pOwner)
		return;

	if (!m_pkUniqueExpireEvent)
		return;

	if (GetValue(2) != 0) // ���ӽð��� �̿��� �������� UniqueExpireEvent�� �ߴ��� �� ����.
		return;

	// HARD CODING
	if (GetVnum() == UNIQUE_ITEM_HIDE_ALIGNMENT_TITLE)
		m_pOwner->ShowAlignment(true);

	SetSocket(ITEM_SOCKET_UNIQUE_SAVE_TIME, event_time(m_pkUniqueExpireEvent) / passes_per_sec);
	event_cancel(&m_pkUniqueExpireEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}

void CItem::StopTimerBasedOnWearExpireEvent()
{
	if (!m_pkTimerBasedOnWearExpireEvent)
		return;

	int remain_time = GetSocket(ITEM_SOCKET_REMAIN_SEC) - event_processing_time(m_pkTimerBasedOnWearExpireEvent) / passes_per_sec;

	SetSocket(ITEM_SOCKET_REMAIN_SEC, remain_time);
	event_cancel(&m_pkTimerBasedOnWearExpireEvent);

	ITEM_MANAGER::instance().SaveSingleItem(this);
}

void CItem::ApplyAddon(int iAddonType)
{
	CItemAddonManager::instance().ApplyAddonTo(iAddonType, this);
}

int CItem::GetSpecialGroup() const
{
	return ITEM_MANAGER::instance().GetSpecialGroupFromItem(GetVnum());
}

//
// �Ǽ����� ���� ó��.
//
bool CItem::IsAccessoryForSocket()
{
	return (m_pProto->bType == ITEM_ARMOR && (m_pProto->bSubType == ARMOR_WRIST || m_pProto->bSubType == ARMOR_NECK || m_pProto->bSubType == ARMOR_EAR)) ||
		(m_pProto->bType == ITEM_BELT); // 2013�� 2�� ���� �߰��� '��Ʈ' �������� ��� ��ȹ������ �Ǽ����� ���� �ý����� �״�� �̿����ڰ� ��.
}

void CItem::SetAccessorySocketGrade(int iGrade)
{
	SetSocket(0, MINMAX(0, iGrade, GetAccessorySocketMaxGrade()));

	int iDownTime = aiAccessorySocketDegradeTime[GetAccessorySocketGrade()];

	//if (test_server)
	//	iDownTime /= 60;

	SetAccessorySocketDownGradeTime(iDownTime);
}

void CItem::SetAccessorySocketMaxGrade(int iMaxGrade)
{
	SetSocket(1, MINMAX(0, iMaxGrade, ITEM_ACCESSORY_SOCKET_MAX_NUM));
}

void CItem::SetAccessorySocketDownGradeTime(DWORD time)
{
	SetSocket(2, time);

	if (test_server && GetOwner())
		GetOwner()->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s���� ���� ���������� ���� �ð� %d"), GetLocaleName(), time);
}

EVENTFUNC(accessory_socket_expire_event)
{
	item_vid_event_info* info = dynamic_cast<item_vid_event_info*>(event->info);

	if (info == NULL)
	{
		sys_err("accessory_socket_expire_event> <Factor> Null pointer");
		return 0;
	}

	LPITEM item = ITEM_MANAGER::instance().FindByVID(info->item_vid);

	if (item->GetAccessorySocketDownGradeTime() <= 1)
	{
	degrade:
		item->SetAccessorySocketExpireEvent(NULL);
		item->AccessorySocketDegrade();
		return 0;
	}
	else
	{
		int iTime = item->GetAccessorySocketDownGradeTime() - 60;

		if (iTime <= 1)
			goto degrade;

		item->SetAccessorySocketDownGradeTime(iTime);

		if (iTime > 60)
			return PASSES_PER_SEC(60);
		else
			return PASSES_PER_SEC(iTime);
	}
}

void CItem::StartAccessorySocketExpireEvent()
{
	if (!IsAccessoryForSocket())
		return;

	if (m_pkAccessorySocketExpireEvent)
		return;

	if (GetAccessorySocketMaxGrade() == 0)
		return;

	if (GetAccessorySocketGrade() == 0)
		return;

	int iSec = GetAccessorySocketDownGradeTime();
	SetAccessorySocketExpireEvent(NULL);

	if (iSec <= 1)
		iSec = 5;
	else
		iSec = MIN(iSec, 60);

	item_vid_event_info* info = AllocEventInfo<item_vid_event_info>();
	info->item_vid = GetVID();

	SetAccessorySocketExpireEvent(event_create(accessory_socket_expire_event, info, PASSES_PER_SEC(iSec)));
}

void CItem::StopAccessorySocketExpireEvent()
{
	if (!m_pkAccessorySocketExpireEvent)
		return;

	if (!IsAccessoryForSocket())
		return;

	int new_time = GetAccessorySocketDownGradeTime() - (60 - event_time(m_pkAccessorySocketExpireEvent) / passes_per_sec);

	event_cancel(&m_pkAccessorySocketExpireEvent);

	if (new_time <= 1)
	{
		AccessorySocketDegrade();
	}
	else
	{
		SetAccessorySocketDownGradeTime(new_time);
	}
}

bool CItem::IsRideItem()
{
	if (ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_RIDE == GetSubType())
		return true;
	if (ITEM_UNIQUE == GetType() && UNIQUE_SPECIAL_MOUNT_RIDE == GetSubType())
		return true;
#if defined(__MOUNT_COSTUME_SYSTEM__)
	if (ITEM_COSTUME == GetType() && COSTUME_MOUNT == GetSubType())
		return true;
#endif

	return false;
}

bool CItem::IsRamadanRing()
{
	if (GetVnum() == UNIQUE_ITEM_RAMADAN_RING)
		return true;
	return false;
}

void CItem::ClearMountAttributeAndAffect()
{
	LPCHARACTER ch = GetOwner();

	ch->RemoveAffect(AFFECT_MOUNT);
	ch->RemoveAffect(AFFECT_MOUNT_BONUS);

	ch->MountVnum(0);

	ch->PointChange(POINT_ST, 0);
	ch->PointChange(POINT_DX, 0);
	ch->PointChange(POINT_HT, 0);
	ch->PointChange(POINT_IQ, 0);
}

#if defined(__SOUL_SYSTEM__)
void CItem::SetSoulKeepTime()
{
	if (GetType() != ITEM_SOUL)
		return;

	LPCHARACTER ch = GetOwner();
	if (!ch)
		return;

	DWORD dwItemPlayTime = GetSocket(3);
	DWORD dwMaxTime = GetLimitValue(1);
	int iSoulAttacks = GetValue(2);

	if (dwItemPlayTime < dwMaxTime)
	{
		dwItemPlayTime += 1;

		DWORD dwUseData = (dwItemPlayTime * 10000) + iSoulAttacks;
		SetSocket(2, dwUseData);
		SetSocket(3, dwItemPlayTime);
	}
	else
	{
		event_cancel(&m_pkSoulTimeUseEvent);
		return;
	}
}
#endif

// fixme
// �̰� ������ �Ⱦ���... �ٵ� Ȥ�ó� �; ���ܵ�.
// by rtsummit
bool CItem::IsNewMountItem()
{
	switch (GetVnum())
	{
	case 76000: case 76001: case 76002: case 76003:
	case 76004: case 76005: case 76006: case 76007:
	case 76008: case 76009: case 76010: case 76011:
	case 76012: case 76013: case 76014:
		return true;
	}
	return false;
}

void CItem::SetAccessorySocketExpireEvent(LPEVENT pkEvent)
{
	m_pkAccessorySocketExpireEvent = pkEvent;
}

void CItem::AccessorySocketDegrade()
{
	if (GetAccessorySocketGrade() > 0)
	{
		LPCHARACTER ch = GetOwner();

		if (ch)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s�� �����ִ� ������ ������ϴ�."), GetLocaleName());
		}

		ModifyPoints(false);
		SetAccessorySocketGrade(GetAccessorySocketGrade() - 1);
		ModifyPoints(true);

		int iDownTime = aiAccessorySocketDegradeTime[GetAccessorySocketGrade()];

		if (test_server)
			iDownTime /= 60;

		SetAccessorySocketDownGradeTime(iDownTime);

		if (iDownTime)
			StartAccessorySocketExpireEvent();
	}
}

// ring�� item�� ���� �� �ִ��� ���θ� üũ�ؼ� ����
static const bool CanPutIntoRing(LPITEM ring, LPITEM item)
{
	const DWORD vnum = item->GetVnum();
	return false;
}

bool CItem::CanPutInto(LPITEM item)
{
	if (item->GetType() == ITEM_BELT)
		return this->GetSubType() == USE_PUT_INTO_BELT_SOCKET;

	else if (item->GetType() == ITEM_RING)
		return CanPutIntoRing(item, this);

	else if (item->GetType() != ITEM_ARMOR)
		return false;

	DWORD vnum = item->GetVnum();

	struct JewelAccessoryInfo
	{
		DWORD jewel;
		DWORD wrist;
		DWORD neck;
		DWORD ear;
	};
	const static JewelAccessoryInfo infos[] = {
		{ 50634, 14420, 16220, 17220 },
		{ 50635, 14500, 16500, 17500 },
		{ 50636, 14520, 16520, 17520 },
		{ 50637, 14540, 16540, 17540 },
		{ 50638, 14560, 16560, 17560 },
		{ 50639, 14570, 16570, 0 },
	};

	DWORD item_type = (item->GetVnum() / 10) * 10;
	for (int i = 0; i < sizeof(infos) / sizeof(infos[0]); i++)
	{
		const JewelAccessoryInfo& info = infos[i];
		switch (item->GetSubType())
		{
		case ARMOR_WRIST:
			if (info.wrist == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_NECK:
			if (info.neck == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		case ARMOR_EAR:
			if (info.ear == item_type)
			{
				if (info.jewel == GetVnum())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
		}
	}
	if (item->GetSubType() == ARMOR_WRIST)
		vnum -= 14000;
	else if (item->GetSubType() == ARMOR_NECK)
		vnum -= 16000;
	else if (item->GetSubType() == ARMOR_EAR)
		vnum -= 17000;
	else
		return false;

	DWORD type = vnum / 20;

	if (type < 0 || type > 11)
	{
		type = (vnum - 170) / 20;

		if (50623 + type != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16210 && item->GetVnum() <= 16219)
	{
		if (50625 != GetVnum())
			return false;
		else
			return true;
	}
	else if (item->GetVnum() >= 16230 && item->GetVnum() <= 16239)
	{
		if (50626 != GetVnum())
			return false;
		else
			return true;
	}

	return 50623 + type == GetVnum();
}

// PC_BANG_ITEM_ADD
bool CItem::IsPCBangItem()
{
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (m_pProto->aLimits[i].bType == LIMIT_PCBANG)
			return true;
	}
	return false;
}
// END_PC_BANG_ITEM_ADD

bool CItem::IsStackable()
{
	return (GetFlag() & ITEM_FLAG_STACKABLE) ? true : false;
}

bool CItem::CheckItemUseLevel(int nLevel)
{
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (this->m_pProto->aLimits[i].bType == LIMIT_LEVEL)
		{
			if (this->m_pProto->aLimits[i].lValue > nLevel) return false;
			else return true;
		}
	}
	return true;
}

long CItem::FindApplyValue(BYTE bApplyType)
{
	if (m_pProto == NULL)
		return 0;

	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		if (m_pProto->aApplies[i].bType == bApplyType)
			return m_pProto->aApplies[i].lValue;
	}

	return 0;
}

DWORD CItem::GetMountVnum()
{
	if (!IsCostumeMount())
		return 0;

	const DWORD dwMountVnum = FindApplyValue(APPLY_MOUNT);
#if defined(__CHANGE_LOOK_SYSTEM__)
	const DWORD c_dwTransmutationVnum = GetTransmutationVnum();
	if (c_dwTransmutationVnum != 0)
	{
		TItemTable* pItemTable = ITEM_MANAGER::instance().GetTable(c_dwTransmutationVnum);
		if (pItemTable)
			return static_cast<DWORD>(pItemTable->FindApplyValue(APPLY_MOUNT));
	}
#endif
	return dwMountVnum;
}

void CItem::CopySocketTo(LPITEM pItem)
{
	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		pItem->m_alSockets[i] = m_alSockets[i];
	}
}

int CItem::GetAccessorySocketGrade()
{
	return MINMAX(0, GetSocket(0), GetAccessorySocketMaxGrade());
}

int CItem::GetAccessorySocketMaxGrade()
{
	return MINMAX(0, GetSocket(1), ITEM_ACCESSORY_SOCKET_MAX_NUM);
}

int CItem::GetAccessorySocketDownGradeTime()
{
	return MINMAX(0, GetSocket(2), aiAccessorySocketDegradeTime[GetAccessorySocketGrade()]);
}

void CItem::AttrLog()
{
	const char* pszIP = NULL;

	if (GetOwner() && GetOwner()->GetDesc())
		pszIP = GetOwner()->GetDesc()->GetHostName();

	for (int i = 0; i < ITEM_SOCKET_MAX_NUM; ++i)
	{
		if (m_alSockets[i])
		{
			LogManager::instance().ItemLog(i, m_alSockets[i], 0, GetID(), "INFO_SOCKET", "", pszIP ? pszIP : "", GetOriginalVnum());
		}
	}

	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		int type = m_aAttr[i].bType;
		int value = m_aAttr[i].sValue;

		if (type)
			LogManager::instance().ItemLog(i, type, value, GetID(), "INFO_ATTR", "", pszIP ? pszIP : "", GetOriginalVnum());
	}
}

int CItem::GetLevelLimit()
{
	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		if (this->m_pProto->aLimits[i].bType == LIMIT_LEVEL)
		{
			return this->m_pProto->aLimits[i].lValue;
		}
	}
	return 0;
}

bool CItem::OnAfterCreatedItem()
{
#if defined(__SOUL_SYSTEM__)
	if (GetType() == ITEM_SOUL)
		StartSoulTimeUseEvent();
#endif

#if defined(__SOUL_BIND_SYSTEM__)
	if (GetSealedTime() > 0)
		StartUnbindTimerExpireEvent();
#endif

#if defined(__EXTENDED_BLEND_AFFECT__) && defined(__ITEM_SOCKET5__)
	if (IsBlendItem())
	{
		if (GetSocket(3) == true)
		{
			Lock(true);
			StartBlendExpireEvent();
		}
	}
#endif

	// �������� �� ���̶� ����ߴٸ�, �� ���Ŀ� ��� ������ �ʾƵ� �ð��� �����Ǵ� ���
	if (-1 != this->GetProto()->cLimitRealTimeFirstUseIndex)
	{
		// Socket1�� �������� ��� Ƚ���� ��ϵǾ� ������, �� ���̶� ����� �������� Ÿ�̸Ӹ� �����Ѵ�.
		if (0 != GetSocket(1))
		{
			StartRealTimeExpireEvent();
		}
	}

	return true;
}

bool CItem::IsDragonSoul()
{
	return GetType() == ITEM_DS;
}

int CItem::GiveMoreTime_Per(float fPercent)
{
	if (IsDragonSoul())
	{
		DWORD duration = DSManager::instance().GetDuration(this);
		DWORD remain_sec = GetSocket(ITEM_SOCKET_REMAIN_SEC);
		DWORD given_time = fPercent * duration / 100u;

		if (remain_sec == duration)
			return false;

		if ((given_time + remain_sec) >= duration)
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, duration);
			return duration - remain_sec;
		}
		else
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, given_time + remain_sec);
			return given_time;
		}
	}
	// �켱 ��ȥ���� ���ؼ��� �ϵ��� �Ѵ�.
	else
		return 0;
}

int CItem::GiveMoreTime_Fix(DWORD dwTime)
{
	if (IsDragonSoul())
	{
		DWORD duration = DSManager::instance().GetDuration(this);
		DWORD remain_sec = GetSocket(ITEM_SOCKET_REMAIN_SEC);

		if (remain_sec == duration)
			return false;

		if ((dwTime + remain_sec) >= duration)
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, duration);
			return duration - remain_sec;
		}
		else
		{
			SetSocket(ITEM_SOCKET_REMAIN_SEC, dwTime + remain_sec);
			return dwTime;
		}
	}
	// �켱 ��ȥ���� ���ؼ��� �ϵ��� �Ѵ�.
	else
		return 0;
}

#if defined(__EXTENDED_DSS_RECHARGE__)
int CItem::GiveMoreTime_Extend(DWORD dwTime)
{
	if (IsDragonSoul())
	{
		int remain_sec = GetSocket(ITEM_SOCKET_REMAIN_SEC);

		if (remain_sec >= MIN_INFINITE_DURATION)
			return 0;

		if (dwTime < 86400)
			dwTime = 86400;

		if ((dwTime + remain_sec) >= MIN_INFINITE_DURATION)
			SetSocket(ITEM_SOCKET_REMAIN_SEC, MAX_INFINITE_DURATION);
		else
			SetSocket(ITEM_SOCKET_REMAIN_SEC, dwTime + remain_sec);

		return dwTime;
	}
	else
		return 0;
}
#endif

#if defined(__EXTENDED_COSTUME_RECHARGE__)
bool CItem::GiveInfiniteTime()
{
	if (IsCostume())
	{
		bool bExtendable = false;
		for (int i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
		{
			long lLimitValue = GetLimitValue(i);
			switch (GetLimitType(i))
			{
			case LIMIT_REAL_TIME:
				if (lLimitValue < MIN_INFINITE_DURATION)
					bExtendable = true;
				break;
			}
		}

		if (bExtendable)
		{
			int iRemainingSec = GetSocket(ITEM_SOCKET_REMAIN_SEC);

			if (iRemainingSec > (get_global_time() + MIN_INFINITE_DURATION))
				return false;

			SetSocket(ITEM_SOCKET_REMAIN_SEC, MAX_INFINITE_DURATION);
		}

		return true;
	}
	else
		return false;
}
#endif

int CItem::GetDuration()
{
	if (!GetProto())
		return -1;

	for (int i = 0; i < ITEM_LIMIT_MAX_NUM; i++)
	{
		if (LIMIT_REAL_TIME == GetProto()->aLimits[i].bType)
			return GetProto()->aLimits[i].lValue;
	}

	if (-1 != GetProto()->cLimitTimerBasedOnWearIndex)
		return GetProto()->aLimits[GetProto()->cLimitTimerBasedOnWearIndex].lValue;

	return -1;
}

bool CItem::IsSameSpecialGroup(const LPITEM item) const
{
	// ���� VNUM�� ���ٸ� ���� �׷��� ������ ����
	if (this->GetVnum() == item->GetVnum())
		return true;

	if (GetSpecialGroup() && (item->GetSpecialGroup() == GetSpecialGroup()))
		return true;

	return false;
}

DWORD CItem::GetRealImmuneFlag()
{
	DWORD dwImmuneFlag = m_pProto->dwImmuneFlag;
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		if (GetAttributeType(i))
		{
			const TPlayerItemAttribute& ia = GetAttribute(i);

			if (ia.bType == APPLY_IMMUNE_STUN && !IS_SET(dwImmuneFlag, IMMUNE_STUN))
				SET_BIT(dwImmuneFlag, IMMUNE_STUN);
			else if (ia.bType == APPLY_IMMUNE_FALL && !IS_SET(dwImmuneFlag, IMMUNE_FALL))
				SET_BIT(dwImmuneFlag, IMMUNE_FALL);
			else if (ia.bType == APPLY_IMMUNE_SLOW && !IS_SET(dwImmuneFlag, IMMUNE_SLOW))
				SET_BIT(dwImmuneFlag, IMMUNE_SLOW);
		}
	}

	return dwImmuneFlag;
}

const char* CItem::GetLocaleName(BYTE bLocale)
{
	if (bLocale == LOCALE_MAX_NUM)
	{
		if (m_pOwner != NULL)
			bLocale = m_pOwner->GetLanguage();
		else
		{
			if (GetDesc())
				bLocale = GetDesc()->GetLanguage();
			else
				bLocale = LOCALE_DEFAULT;
		}
	}
	return m_pProto ? LC_ITEM_NAME(GetVnum(), bLocale) : NULL;
}

const char* CItem::GetName()
{
	return m_pProto ? LC_ITEM_NAME(GetVnum(), LOCALE_DEFAULT) : NULL;
}

#if defined(__CHANGE_LOOK_SYSTEM__)
void CItem::SetTransmutationVnum(DWORD blVnum)
{
	m_dwTransmutationVnum = blVnum;
	Save();
}

DWORD CItem::GetTransmutationVnum() const
{
	return m_dwTransmutationVnum;
}
#endif

#if defined(__SPECIAL_INVENTORY_SYSTEM__)
bool CItem::IsSkillBook()
{
	return GetType() == ITEM_SKILLBOOK;
}

bool CItem::IsUpgradeItem()
{
	switch (GetVnum())
	{
	case 30003:
	case 30004:
	case 30005:
	case 30006:
	case 30007:
	case 30008:
	case 30009:
	case 30010:
	case 30011:
	case 30014:
	case 30015:
	case 30016:
	case 30017:
	case 30018:
	case 30019:
	case 30021:
	case 30022:
	case 30023:
	case 30025:
	case 30027:
	case 30028:
	case 30030:
	case 30031:
	case 30032:
	case 30033:
	case 30034:
	case 30035:
	case 30037:
	case 30038:
	case 30039:
	case 30040:
	case 30041:
	case 30042:
	case 30045:
	case 30046:
	case 30047:
	case 30048:
	case 30049:
	case 30050:
	case 30051:
	case 30052:
	case 30053:
	case 30055:
	case 30056:
	case 30057:
	case 30058:
	case 30059:
	case 30060:
	case 30061:
	case 30067:
	case 30069:
	case 30070:
	case 30071:
	case 30072:
	case 30073:
	case 30074:
	case 30075:
	case 30076:
	case 30077:
	case 30078:
	case 30079:
	case 30080:
	case 30081:
	case 30082:
	case 30083:
	case 30084:
	case 30085:
	case 30086:
	case 30087:
	case 30088:
	case 30089:
	case 30090:
	case 30091:
	case 30092:
	case 30192:
	case 30193:
	case 30194:
	case 30195:
	case 30196:
	case 30197:
	case 30198:
	case 30199:
	case 30500:
	case 30501:
	case 30502:
	case 30503:
	case 30504:
	case 30505:
	case 30506:
	case 30507:
	case 30508:
	case 30509:
	case 30510:
	case 30511:
	case 30512:
	case 30513:
	case 30514:
	case 30515:
	case 30516:
	case 30517:
	case 30518:
	case 30519:
	case 30520:
	case 30521:
	case 30522:
	case 30523:
	case 30524:
	case 30525:
	case 30600:
	case 30601:
	case 30602:
	case 30603:
	case 30604:
	case 30605:
	case 30606:
	case 30607:
	case 30608:
	case 30609:
	case 30610:
	case 30611:
	case 30612:
	case 30614:
	case 30615:
	case 30616:
	case 30617:
	case 30618:
	case 30619:
	case 27799:
	case 27992:
	case 27993:
	case 27994:
	case 27987:
	case 33031:
	case 33030:
	case 33029:
	case 30630:
	case 30629:
	case 30628:
	case 30627:
	case 30626:
	case 30625:
	case 30624:
	case 30623:
	case 70097:
	case 70096:
	case 70095:
	case 70094:
	case 70093:
	case 70092:
	case 70091:
	case 70090:
	case 70089:
	case 70088:
	case 70087:
	case 70086:
	case 85000:
	case 70254:
	case 70253:
	case 70252:
	case 70251:
	case 51001:
	case 50160:
	case 50161:
	case 50162:
	case 50163:
	case 50164:
	case 50165:
	case 50166:
	case 50167:
	case 50168:
	case 50169:
	case 50170:
	case 50171:
	case 50172:
	case 50173:
	case 50174:
	case 50175:
	case 50176:
	case 50177:
	case 50178:
	case 50179:
	case 30622:
	case 30621:
	case 30620:
	case 30559:
	case 30558:
	case 30557:
	case 30556:
	case 30555:
	case 30554:
	case 30550:
	case 30252:
	case 30251:
	case 30228:
	case 30227:
	case 30226:
	case 30225:
	case 30224:
	case 30223:
	case 30222:
	case 30221:
	case 30220:
	case 30168:
	case 30167:
	case 30166:
	case 30165:
	case 27990:
	case 71123:
	case 71129:
		return true;
	}

	return false;
}

bool CItem::IsStone()
{
	return GetType() == ITEM_METIN;
}

bool CItem::IsGiftBox()
{
#if defined(__GACHA_SYSTEM__)
	return GetType() == ITEM_GIFTBOX || GetType() == ITEM_GACHA;
#else
	return GetType() == ITEM_GIFTBOX;
#endif
}
#endif
