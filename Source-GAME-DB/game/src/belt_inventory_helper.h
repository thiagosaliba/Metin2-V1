#ifndef __INC_BELT_INVENTORY_HELPER_H__
#define __INC_BELT_INVENTORY_HELPER_H__

#include "char.h"
#include "item.h"

class CBeltInventoryHelper
{
public:
	typedef BYTE TGradeUnit;

	static TGradeUnit GetBeltGradeByRefineLevel(int level)
	{
		static TGradeUnit beltGradeByLevelTable[] =
		{
			0, // ��Ʈ+0
			1, // +1
			1, // +2
			2, // +3
			2, // +4,
			3, // +5
			4, // +6,
			5, // +7,
			6, // +8,
			7, // +9
		};

		int countOf = _countof(beltGradeByLevelTable);
		if (level >= countOf)
		{
			sys_err("CBeltInventoryHelper::GetBeltGradeByRefineLevel - Overflow level (%d", level);
			return 0;
		}

		return beltGradeByLevelTable[level];
	}

	// ���� ��Ʈ ������ ��������, � ������ �̿��� �� �ִ��� ����
	static const TGradeUnit* GetAvailableRuleTableByGrade()
	{
		/**
			��Ʈ�� �� +0 ~ +9 ������ ���� �� ������, ������ ���� 7�ܰ� ������� ���еǾ� �κ��丮�� Ȱ�� ȭ �ȴ�.
			��Ʈ ������ ���� ��� ������ ���� �Ʒ� �׸��� ����. ���� ��� >= Ȱ������ ����̸� ��� ����.
			(��, ���� ������ 0�̸� ������ ��� �Ұ�, ��ȣ ���� ���ڴ� ���)

				2(1)	4(2)	6(4)	8(6)
				5(3)	5(3)	6(4)	8(6)
				7(5)	7(5)	7(5)	8(6)
				9(7)	9(7)	9(7)	9(7)

			��Ʈ �κ��丮�� ũ��� 4x4 (16ĭ)
		**/

		static TGradeUnit availableRuleByGrade[BELT_INVENTORY_SLOT_COUNT] = {
			1, 2, 4, 6,
			3, 3, 4, 6,
			5, 5, 5, 6,
			7, 7, 7, 7
		};

		return availableRuleByGrade;
	}

	static bool IsAvailableCell(WORD cell, int beltGrade /* int beltLevel */)
	{
		// ��ȹ �� �ٲ�.. �Ƴ�...
		//const TGradeUnit beltGrade = GetBeltGradeByRefineLevel(beltLevel);
		const TGradeUnit* ruleTable = GetAvailableRuleTableByGrade();

		return ruleTable[cell] <= beltGrade;
	}

	/// pc�� ��Ʈ �κ��丮�� �������� �ϳ��� �����ϴ� �� �˻��ϴ� �Լ�.
	static bool IsExistItemInBeltInventory(LPCHARACTER pc)
	{
		for (WORD i = BELT_INVENTORY_SLOT_START; i < BELT_INVENTORY_SLOT_END; ++i)
		{
			LPITEM beltInventoryItem = pc->GetInventoryItem(i);

			if (NULL != beltInventoryItem)
				return true;
		}

		return false;
	}

	static int GetItemCount(LPCHARACTER pc)
	{
		DWORD count = 0;
		for (WORD i = BELT_INVENTORY_SLOT_START; i < BELT_INVENTORY_SLOT_END; ++i)
		{
			LPITEM beltInventoryItem = pc->GetInventoryItem(i);
			if (beltInventoryItem)
				count++;
		}
		return count;
	}

	/// pc�� ��Ʈ �κ��丮�� �������� �ϳ��� �����ϴ� �� �˻��ϴ� �Լ�.
	static bool ClearBelt(LPCHARACTER pc)
	{
		int EmptySlotCount = pc->GetEmptyInventoryCount(1);
		int BeltItemCount = GetItemCount(pc);

		if (EmptySlotCount - 1 < BeltItemCount)
		{
			pc->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("NOT_ENOUGH_SPACE_FOR_BELT_ITEMS"));
			return false;
		}

		for (WORD i = BELT_INVENTORY_SLOT_START; i < BELT_INVENTORY_SLOT_END; ++i)
		{
			LPITEM beltInventoryItem = pc->GetInventoryItem(i);

			if (NULL != beltInventoryItem)
			{
				int NewPos = pc->GetEmptyInventory(1);
				if (NewPos != -1)
				{
					beltInventoryItem->RemoveFromCharacter();
					beltInventoryItem->AddToCharacter(pc, TItemPos(INVENTORY, NewPos));
				}
				else
				{
					pc->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("NOT_ENOUGH_SPACE_FOR_BELT_ITEMS"));
					return false;
				}
			}
		}
		return true;
	}

	/// item�� ��Ʈ �κ��丮�� �� �� �ִ� Ÿ������ �˻��ϴ� �Լ�. (�� ��Ģ�� ��ȹ�ڰ� ������)
	static bool CanMoveIntoBeltInventory(LPITEM item)
	{
#if !defined(__EXTENDED_BLEND_AFFECT__)
		if (item->GetType() == ITEM_BLEND)
			return true;
#endif

		if (item->GetType() == ITEM_USE)
		{
			switch (item->GetSubType())
			{
			case USE_POTION:
			case USE_POTION_NODELAY:
			case USE_ABILITY_UP:
			case USE_AFFECT:
			case USE_BAIT:
			case USE_TALISMAN:
				return true;
				break;
			}
		}

		return false;
	}
};

#endif // __INC_BELT_INVENTORY_HELPER_H__