#pragma once

#include "StdAfx.h"

/*
* NPC 데이터 프로토 타잎을 관리 한다.
*/

#if defined(ENABLE_SEND_TARGET_INFO)
struct SMobItemDrop
{
	DWORD dwVnum;
	DWORD wCount;
};
typedef std::map<DWORD, SMobItemDrop> TMobItemDrop;
typedef std::map<DWORD, TMobItemDrop> TMobItemDropMap;
#endif

class CPythonNonPlayer : public CSingleton<CPythonNonPlayer>
{
public:
	enum EClickEvent
	{
		ON_CLICK_EVENT_NONE = 0,
		ON_CLICK_EVENT_BATTLE = 1,
		ON_CLICK_EVENT_SHOP = 2,
		ON_CLICK_EVENT_TALK = 3,
		ON_CLICK_EVENT_VEHICLE = 4,

		ON_CLICK_EVENT_MAX_NUM,
	};

	enum EMobRanks
	{
		MOB_RANK_PAWN,
		MOB_RANK_S_PAWN,
		MOB_RANK_KNIGHT,
		MOB_RANK_S_KNIGHT,
		MOB_RANK_BOSS,
		MOB_RANK_KING,
		MOB_RANK_MAX_NUM
	};

	enum EMobRaceFlags
	{
		MOB_FLAG_ANIMAL = (1 << 0),
		MOB_FLAG_UNDEAD = (1 << 1),
		MOB_FLAG_DEVIL = (1 << 2),
		MOB_FLAG_HUMAN = (1 << 3),
		MOB_FLAG_ORC = (1 << 4),
		MOB_FLAG_MILGYO = (1 << 5),
		MOB_FLAG_INSECT = (1 << 6),
		MOB_FLAG_FIRE = (1 << 7),
		MOB_FLAG_ICE = (1 << 8),
		MOB_FLAG_DESERT = (1 << 9),
		MOB_FLAG_TREE = (1 << 10),
		MOB_FLAG_ATT_ELEC = (1 << 11),
		MOB_FLAG_ATT_FIRE = (1 << 12),
		MOB_FLAG_ATT_ICE = (1 << 13),
		MOB_FLAG_ATT_WIND = (1 << 14),
		MOB_FLAG_ATT_EARTH = (1 << 15),
		MOB_FLAG_ATT_DARK = (1 << 16),
#if defined(ENABLE_ELEMENT_ADD)
		MOB_FLAG_ZODIAC = (1 << 17),
#endif
	};

	enum EMobEnchants
	{
		MOB_ENCHANT_CURSE,
		MOB_ENCHANT_SLOW,
		MOB_ENCHANT_POISON,
		MOB_ENCHANT_STUN,
		MOB_ENCHANT_CRITICAL,
		MOB_ENCHANT_PENETRATE,
		MOB_ENCHANTS_MAX_NUM
	};

	enum EMobResists
	{
		MOB_RESIST_SWORD,
		MOB_RESIST_TWOHAND,
		MOB_RESIST_DAGGER,
		MOB_RESIST_BELL,
		MOB_RESIST_FAN,
		MOB_RESIST_BOW,

		MOB_RESIST_CLAW,
		MOB_RESIST_FIRE,
		MOB_RESIST_ELECT,
		MOB_RESIST_MAGIC,
		MOB_RESIST_WIND,
		MOB_RESIST_POISON,
		MOB_RESIST_BLEEDING,

		MOB_RESISTS_MAX_NUM
	};

#if defined(ENABLE_ELEMENT_ADD)
	enum EMobElements
	{
		MOB_ELEMENT_FIRE,
		MOB_ELEMENT_ICE,
		MOB_ELEMENT_EARTH,
		MOB_ELEMENT_WIND,
		MOB_ELEMENT_DARK,
		MOB_ELEMENT_ELECT,
		MOB_ELEMENT_MAX_NUM
	};
#endif

#if defined(ENABLE_SEND_TARGET_INFO)
	enum EMobItemDrop
	{
		MAX_DROP_INFO_GRID_SLOT_X = 7,
		MAX_DROP_INFO_GRID_SLOT_Y = 3,
		MAX_DROP_INFO_GRID_SIZE = MAX_DROP_INFO_GRID_SLOT_X * MAX_DROP_INFO_GRID_SLOT_Y,
	};
#endif

#define MOB_ATTRIBUTE_MAX_NUM 14 // 12
#define MOB_SKILL_MAX_NUM 5

#pragma pack(push)
#pragma pack(1)
	typedef struct SMobSkillLevel
	{
		DWORD dwVnum;
		BYTE bLevel;
	} TMobSkillLevel;

	typedef struct SMobTable
	{
		DWORD dwVnum;
		char szName[CHARACTER_NAME_MAX_LEN + 1];
		char szLocaleName[CHARACTER_NAME_MAX_LEN + 1];

		BYTE bType; // Monster, NPC
		BYTE bRank; // PAWN, KNIGHT, KING
		BYTE bBattleType; // MELEE, etc..
		BYTE bLevel; // Level
		BYTE bScale; // Scale
		BYTE bSize;

		DWORD dwGoldMin;
		DWORD dwGoldMax;
		DWORD dwExp;
		uint64_t dwMaxHP;
		BYTE bRegenCycle;
		BYTE bRegenPercent;
		WORD wDef;

		DWORD dwAIFlag;
		DWORD dwRaceFlag;
		DWORD dwImmuneFlag;

		BYTE bStr, bDex, bCon, bInt;
		DWORD dwDamageRange[2];

		short sAttackSpeed;
		short sMovingSpeed;
		BYTE bAggresiveHPPct;
		WORD wAggressiveSight;
		WORD wAttackRange;

		char cEnchants[MOB_ENCHANTS_MAX_NUM];
		char cResists[MOB_RESISTS_MAX_NUM];
#if defined(ENABLE_ELEMENT_ADD)
		char cElements[MOB_ELEMENT_MAX_NUM];
#endif

		DWORD dwResurrectionVnum;
		DWORD dwDropItemVnum;

		BYTE bMountCapacity;
		BYTE bOnClickType;

		BYTE bEmpire;
		char szFolder[64 + 1];
		float fDamMultiply;
		DWORD dwSummonVnum;
		DWORD dwDrainSP;
		DWORD dwMonsterColor;
		DWORD dwPolymorphItemVnum;

		TMobSkillLevel Skills[MOB_SKILL_MAX_NUM];

		BYTE bBerserkPoint;
		BYTE bStoneSkinPoint;
		BYTE bGodSpeedPoint;
		BYTE bDeathBlowPoint;
		BYTE bRevivePoint;
	} TMobTable;
#pragma pack(pop)

	typedef std::list<TMobTable*> TMobTableList;
	typedef std::map<DWORD, TMobTable*> TNonPlayerDataMap;

public:
	CPythonNonPlayer(void);
	virtual ~CPythonNonPlayer(void);

	void Clear();
	void Destroy();

	bool LoadNonPlayerData(const char* c_szFileName);

	const TMobTable* GetTable(DWORD dwVnum);
	bool GetName(DWORD dwVnum, const char** c_pszName);
	bool GetInstanceType(DWORD dwVnum, BYTE* pbType);
	BYTE GetEventType(DWORD dwVnum);
	BYTE GetEventTypeByVID(DWORD dwVID);
	DWORD GetMonsterColor(DWORD dwVnum);
	const char* GetMonsterName(DWORD dwVnum);

#if defined(ENABLE_SEND_TARGET_INFO)
	uint64_t GetMonsterMaxHP(DWORD dwVnum);
	DWORD GetMonsterRaceFlag(DWORD dwVnum);
	bool IsMonsterRaceFlag(DWORD dwVnum, DWORD dwFlag);
	DWORD GetMonsterLevel(DWORD dwVnum);
	DWORD GetMonsterDamage1(DWORD dwVnum);
	DWORD GetMonsterDamage2(DWORD dwVnum);
	DWORD GetMonsterExp(DWORD dwVnum);
	float GetMonsterDamageMultiply(DWORD dwVnum);
	DWORD GetMonsterST(DWORD dwVnum);
	DWORD GetMonsterDX(DWORD dwVnum);
	bool IsMonsterStone(DWORD dwVnum);
	BYTE GetMonsterRegenCycle(DWORD dwVnum);
	BYTE GetMonsterRegenPercent(DWORD dwVnum);
	DWORD GetMonsterGoldMin(DWORD dwVnum);
	DWORD GetMonsterGoldMax(DWORD dwVnum);
	DWORD GetMonsterResist(DWORD dwVnum, BYTE bResistNum);

	void AddItemDropInfo(DWORD dwRaceVnum, BYTE bIndex, DWORD dwVnum, WORD wCount);
	void ClearItemDropInfo();
	size_t GetMonsterDropCount(DWORD dwRaceVnum);
	DWORD GetMonsterDropItemVnum(DWORD dwRaceVnum, BYTE bIndex);
	WORD GetMonsterDropItemCount(DWORD dwRaceVnum, BYTE bIndex);
#endif

	BYTE GetMonsterScalePercent(DWORD dwVnum);

	// Function for outer
	void GetMatchableMobList(int iLevel, int iInterval, TMobTableList* pMobTableList);

protected:
	TNonPlayerDataMap m_NonPlayerDataMap;
#if defined(ENABLE_SEND_TARGET_INFO)
	TMobItemDropMap m_MobItemDropMap;
#endif
};
