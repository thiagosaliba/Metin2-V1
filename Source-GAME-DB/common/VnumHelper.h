#ifndef __INC_COMMON_VNUM_HELPER_H__
#define __INC_COMMON_VNUM_HELPER_H__

#include "service.h"

/**
	�̹� �����ϰų� ������ �߰��� ������, �� ���� �ҽ����� �ĺ��� �� ����� ���
	�ĺ���(����=VNum)�� �ϵ��ڵ��ϴ� ������� �Ǿ��־ �������� �ſ� �������µ�

	�����δ� �ҽ��� ���� � ������(Ȥ�� ��)���� �� �� �ְ� ���ڴ� ��ö���� �������� �߰�.

		* �� ������ ������ ���������� ����Ǵµ� PCH�� ������ �ٲ� ������ ��ü ������ �ؾ��ϴ�
		�ϴ��� �ʿ��� cpp���Ͽ��� include �ؼ� ������ ����.

		* cpp���� �����ϸ� ������ ~ ��ũ�ؾ��ϴ� �׳� common�� ����� �־���. (game, db������Ʈ �� �� ��� ����)

	@date 2011.8.29
**/

class CItemVnumHelper
{
public:
	/// ���� DVD�� �һ��� ��ȯ��
	static const bool IsPhoenix(DWORD vnum) { return 53001 == vnum; } // NOTE: �һ��� ��ȯ �������� 53001 ������ mob-vnum�� 34001 �Դϴ�.

	/// �󸶴� �̺�Ʈ �ʽ´��� ���� (������ �󸶴� �̺�Ʈ�� Ư�� �������̾����� ������ ���� �������� ��Ȱ���ؼ� ��� ���ٰ� ��)
	static const bool IsRamadanMoonRing(DWORD vnum) { return 71135 == vnum; }

	/// �ҷ��� ���� (������ �ʽ´��� ������ ����)
	static const bool IsHalloweenCandy(DWORD vnum) { return 71136 == vnum; }

	/// ũ�������� �ູ�� ����
	static const bool IsHappinessRing(DWORD vnum) { return 71143 == vnum; }

	/// �߷�Ÿ�� ����� �Ҵ�Ʈ 
	static const bool IsLovePendant(DWORD vnum) { return 71145 == vnum; }

	/// Magic Ring
	static const bool IsMagicRing(DWORD vnum) { return 71148 == vnum || 71149 == vnum; }

	/// Easter Candy
	static const bool IsEasterCandy(DWORD vnum) { return 71188 == vnum; }

	/// Chocolate Pendant
	static const bool IsChocolatePendant(DWORD vnum) { return 71199 == vnum; }

	/// Nazar Pendant
	static const bool IsNazarPendant(DWORD vnum) { return 71202 == vnum; }

	/// Guardian Pendant
	static const bool IsGuardianPendant(DWORD vnum) { return 72054 == vnum; }

	/// Gem Pendant
	static const bool IsGemCandy(DWORD vnum) { return 76030 == vnum || 76047 == vnum; }

	// Unique Items
	static const bool IsUniqueItem(DWORD vnum)
	{
		switch (vnum)
		{
		case 71135: // IsRamadanMoonRing
		case 71136: // IsHalloweenCandy
		case 71143: // IsHappinessRing
		case 71145: // IsLovePendant

		case 71148: // IsMagicRing
		case 71149: // IsMagicRing

		case 71188: // IsEasterCandy
		case 71199: // IsChocolatePendant
		case 71202: // IsNazarPendant
		case 72054: // IsGuardianPendant

		case 76030: // IsGemCandy
		case 76047: // IsGemCandy
			return true;
		}
		return false;
	}

#if defined(__EXTENDED_BLEND_AFFECT__)
	/// Extended Blend
	static const bool IsExtendedBlend(DWORD vnum)
	{
		switch (vnum)
		{
			// INFINITE_DEWS
		case 950821:
		case 950822:
		case 950823:
		case 950824:
		case 950825:
		case 950826:
		case 951002: // Energy Cristal
		// END_OF_INFINITE_DEWS

		// DRAGON_GOD_MEDALS
		case 939017:
		case 939018:
		case 939019:
		case 939020:
			// END_OF_DRAGON_GOD_MEDALS

			// CRITICAL_AND_PENETRATION
		case 939024:
		case 939025:
			// END_OF_CRITICAL_AND_PENETRATION

			// ATTACK_AND_MOVE_SPEED
		case 927209:
		case 927212:
			// END_OF_ATTACK_AND_MOVE_SPEED
			return true;
		default:
			return false;
		}
	}
#endif

	static const bool IsDragonSoul(DWORD vnum)
	{
		return (vnum >= 110000 && vnum <= 165400);
	}
};

class CMobVnumHelper
{
public:
	/// ���� DVD�� �һ��� �� ��ȣ
	static const bool IsPhoenix(DWORD vnum) { return 34001 == vnum; }
	static const bool IsIcePhoenix(DWORD vnum) { return 34003 == vnum; }
	/// PetSystem�� �����ϴ� ���ΰ�?
	static const bool IsPetUsingPetSystem(DWORD vnum) { return (IsPhoenix(vnum) || IsReindeerYoung(vnum)) || IsIcePhoenix(vnum); }

	/// 2011�� ũ�������� �̺�Ʈ�� �� (�Ʊ� ����)
	static const bool IsReindeerYoung(DWORD vnum) { return 34002 == vnum; }

	/// �󸶴� �̺�Ʈ ����� �渶(20119) .. �ҷ��� �̺�Ʈ�� �󸶴� �渶 Ŭ��(������ ����, 20219)
	static const bool IsRamadanBlackHorse(DWORD vnum) { return 20119 == vnum || 20219 == vnum || 22022 == vnum; }

	/// YMD.2020.06.29.Owsap - NPC Types
	static const bool IsNPCType(BYTE type)
	{
		switch (type)
		{
		case CHAR_TYPE_NPC:
		case CHAR_TYPE_HORSE:
		case CHAR_TYPE_PET:
		case CHAR_TYPE_PET_PAY:
			return true;
		default:
			return false;
		}
	}
};

class CVnumHelper
{
};

#endif // __INC_COMMON_VNUM_HELPER_H__
