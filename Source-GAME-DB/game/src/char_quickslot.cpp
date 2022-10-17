#include "stdafx.h"
#include "constants.h"
#include "char.h"
#include "desc.h"
#include "desc_manager.h"
#include "packet.h"
#include "item.h"

/////////////////////////////////////////////////////////////////////////////
// QUICKSLOT HANDLING
/////////////////////////////////////////////////////////////////////////////
void CHARACTER::SyncQuickslot(BYTE bType, WORD wOldPos, WORD wNewPos) // wNewPos == WORD_MAX ¸é DELETE
{
	if (wOldPos == wNewPos)
		return;

	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
	{
		if (m_quickslot[i].type == bType && m_quickslot[i].pos == wOldPos)
		{
			if (wNewPos == WORD_MAX)
				DelQuickslot(i);
			else
			{
				TQuickslot slot;

				slot.type = bType;
				slot.pos = wNewPos;

				SetQuickslot(i, slot);
			}
		}
	}
}

#if defined(__SWAP_ITEM_SYSTEM__)
int CHARACTER::GetQuickslotPosition(BYTE bType, WORD wInventoryPos)
{
	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
	{
		if (m_quickslot[i].type == bType && m_quickslot[i].pos == wInventoryPos)
			return i;
	}

	return -1;
}
#endif

bool CHARACTER::GetQuickslot(BYTE pos, TQuickslot** ppSlot)
{
	if (pos >= QUICKSLOT_MAX_NUM)
		return false;

	*ppSlot = &m_quickslot[pos];
	return true;
}

bool CHARACTER::SetQuickslot(BYTE pos, TQuickslot& rSlot)
{
	struct packet_quickslot_add pack_quickslot_add;

	if (pos >= QUICKSLOT_MAX_NUM)
		return false;

	if (rSlot.type >= QUICKSLOT_TYPE_MAX_NUM)
		return false;

	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
	{
		if (rSlot.type == 0)
			continue;
		else if (m_quickslot[i].type == rSlot.type && m_quickslot[i].pos == rSlot.pos)
			DelQuickslot(i);
	}

	TItemPos srcCell(INVENTORY, rSlot.pos);

	switch (rSlot.type)
	{
	case QUICKSLOT_TYPE_ITEM:
	{
		if (false == srcCell.IsDefaultInventoryPosition() && false == srcCell.IsBeltInventoryPosition())
			return false;

		//< 2020.04.09.Owsap - Prevent unwanted items into quick slot
		if (LPITEM pItem = GetItem(srcCell))
		{
			if (pItem->GetType() != ITEM_USE && pItem->GetType() != ITEM_QUEST && pItem->GetSubType() == PET_PAY)
				return false;
		}
		//>
	}
	break;

	case QUICKSLOT_TYPE_SKILL:
		if ((int)rSlot.pos >= SKILL_MAX_NUM)
			return false;

		break;

	case QUICKSLOT_TYPE_COMMAND:
		break;

	default:
		return false;
	}

	m_quickslot[pos] = rSlot;

	if (GetDesc())
	{
		pack_quickslot_add.header = HEADER_GC_QUICKSLOT_ADD;
		pack_quickslot_add.pos = pos;
		pack_quickslot_add.slot = m_quickslot[pos];

		GetDesc()->Packet(&pack_quickslot_add, sizeof(pack_quickslot_add));
	}

	return true;
}

bool CHARACTER::DelQuickslot(BYTE pos)
{
	struct packet_quickslot_del pack_quickslot_del;

	if (pos >= QUICKSLOT_MAX_NUM)
		return false;

	memset(&m_quickslot[pos], 0, sizeof(TQuickslot));

	pack_quickslot_del.header = HEADER_GC_QUICKSLOT_DEL;
	pack_quickslot_del.pos = pos;

	GetDesc()->Packet(&pack_quickslot_del, sizeof(pack_quickslot_del));
	return true;
}

bool CHARACTER::SwapQuickslot(BYTE a, BYTE b)
{
	struct packet_quickslot_swap pack_quickslot_swap;
	TQuickslot quickslot;

	if (a >= QUICKSLOT_MAX_NUM || b >= QUICKSLOT_MAX_NUM)
		return false;

	// Äü ½½·Ô ÀÚ¸®¸¦ ¼­·Î ¹Ù²Û´Ù.
	quickslot = m_quickslot[a];

	m_quickslot[a] = m_quickslot[b];
	m_quickslot[b] = quickslot;

	pack_quickslot_swap.header = HEADER_GC_QUICKSLOT_SWAP;
	pack_quickslot_swap.pos = a;
	pack_quickslot_swap.pos_to = b;

	GetDesc()->Packet(&pack_quickslot_swap, sizeof(pack_quickslot_swap));
	return true;
}

void CHARACTER::ChainQuickslotItem(LPITEM pItem, BYTE bType, WORD wOldPos)
{
	if (pItem->IsDragonSoul())
		return;

	for (int i = 0; i < QUICKSLOT_MAX_NUM; ++i)
	{
		if (m_quickslot[i].type == bType && m_quickslot[i].pos == wOldPos)
		{
			TQuickslot slot;
			slot.type = bType;
			slot.pos = pItem->GetCell();

			SetQuickslot(i, slot);

			break;
		}
	}
}
