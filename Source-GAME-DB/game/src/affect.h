#ifndef __INC_AFFECT_H__
#define __INC_AFFECT_H__

class CAffect
{
public:
	DWORD dwType;
	BYTE bApplyOn;
	long lApplyValue;
	DWORD dwFlag;
	long lDuration;
	long lSPCost;

	static CAffect* Acquire();
	static void Release(CAffect* p);
};

enum EAffectTypes
{
	AFFECT_NONE,

	AFFECT_MOV_SPEED = 200,
	AFFECT_ATT_SPEED, // 201
	AFFECT_ATT_GRADE, // 202
	AFFECT_INVISIBILITY, // 203
	AFFECT_STR, // 204
	AFFECT_DEX, // 205
	AFFECT_CON, // 206
	AFFECT_INT, // 207
	AFFECT_FISH_MIND_PILL, // 208

	AFFECT_POISON, // 209
	AFFECT_STUN, // 210
	AFFECT_SLOW, // 211
	AFFECT_DUNGEON_READY, // 212
	AFFECT_DUNGEON_UNIQUE, // 213

	AFFECT_BUILDING, // 214
	AFFECT_REVIVE_INVISIBLE, // 215
	AFFECT_FIRE, // 216
	AFFECT_CAST_SPEED, // 217
	AFFECT_HP_RECOVER_CONTINUE, // 218
	AFFECT_SP_RECOVER_CONTINUE, // 219

	AFFECT_POLYMORPH, // 220
	AFFECT_MOUNT, // 221

	AFFECT_WAR_FLAG, // 222

	AFFECT_BLOCK_CHAT, // 223
	AFFECT_CHINA_FIREWORK, // 224

	AFFECT_BOW_DISTANCE, // 225
	AFFECT_DEF_GRADE, // 226

	AFFECT_BLEEDING, // 227

	AFFECT_PREMIUM_START = 500,
	AFFECT_EXP_BONUS = 500, // 경험의 반지
	AFFECT_ITEM_BONUS = 501, // 도둑의 장갑
	AFFECT_SAFEBOX = 502, // PREMIUM_SAFEBOX,
	AFFECT_AUTOLOOT = 503, // PREMIUM_AUTOLOOT,
	AFFECT_FISH_MIND = 504, // PREMIUM_FISH_MIND,
	AFFECT_MARRIAGE_FAST = 505, // 원앙의 깃털
	AFFECT_GOLD_BONUS = 506, // 돈 드롭확률 50%증가

#if defined(__CONQUEROR_LEVEL__)
	AFFECT_SUNGMA_BONUS = 508,
#endif
	AFFECT_PREMIUM_END = 509,

	AFFECT_MALL = 510, // 몰 아이템 에펙트
	AFFECT_NO_DEATH_PENALTY = 511, // 용신의 가호 (경험치가 패널티를 한번 막아준다)
	AFFECT_SKILL_BOOK_BONUS = 512, // 선인의 교훈 (책 수련 성공 확률이 50% 증가)
	AFFECT_SKILL_NO_BOOK_DELAY = 513, // 주안술서

	AFFECT_HAIR = 514, // 헤어 효과
	AFFECT_COLLECT = 515, //수집퀘스트 
	AFFECT_EXP_BONUS_EURO_FREE = 516, // 경험의 반지 (유럽 버전 14 레벨 이하 기본 효과)
	AFFECT_EXP_BONUS_EURO_FREE_UNDER_15 = 517,
	AFFECT_UNIQUE_ABILITY = 518,

	AFFECT_CUBE_1, // 519
	AFFECT_CUBE_2, // 520
	AFFECT_CUBE_3, // 521
	AFFECT_CUBE_4, // 522
	AFFECT_CUBE_5, // 523
	AFFECT_CUBE_6, // 524
	AFFECT_CUBE_7, // 525
	AFFECT_CUBE_8, // 526
	AFFECT_CUBE_9, // 527
	AFFECT_CUBE_10, // 528
	AFFECT_CUBE_11, // 529
	AFFECT_CUBE_12, // 530

	AFFECT_BLEND, // 531

	AFFECT_HORSE_NAME, // 532
	AFFECT_MOUNT_BONUS, // 533

	AFFECT_AUTO_HP_RECOVERY = 534,
	AFFECT_AUTO_SP_RECOVERY = 535,

	//NEW_AFFECT_PREMIUM_PRIVATE_SHOP = 536,
	//AFFECT_NONE = 537,
	//AFFECT_NONE = 538,
	//AFFECT_NONE = 539,

	AFFECT_DRAGON_SOUL_QUALIFIED = 540,
	AFFECT_DRAGON_SOUL_DECK_0 = 541,
	AFFECT_DRAGON_SOUL_DECK_1 = 542,
#if defined(__DS_SET__)
	NEW_AFFECT_DS_SET = 543,
#endif

	//NEW_AFFECT_IMPOSSIBLE_ATTACK = 544,
	//NEW_AFFECT_PVP_ENTER = 545,

	//NEW_AFFECT_SET_ITEM = 550,
	//NEW_AFFECT_EXP_BONUS_EVENT = 552,
	//NEW_AFFECT_PEPSI_EVENT = 553,

	//NEW_AFFECT_BATTLE_FIELD = 555,
	//NEW_AFFECT_BATTLE_POTION = 556,
	//NEW_AFFECT_BATTLE_RANK = 557,

	//NEW_AFFECT_LUCKEY_EVENT_BUFF = 560

	AFFECT_RAMADAN_ABILITY = 300,
	AFFECT_RAMADAN_RING = 301,

	AFFECT_NOG_ABILITY = 302,
	AFFECT_HOLLY_STONE_POWER = 303,

#if defined(__EXTENDED_BLEND_AFFECT__) || defined(__BLEND_AFFECT__)
	AFFECT_BLEND_POTION_1 = 304, // Critical
	AFFECT_BLEND_POTION_2 = 305, // Penetration
	AFFECT_BLEND_POTION_3 = 306, // Attack Speed
	AFFECT_BLEND_POTION_4 = 307, // Magic Resist
	AFFECT_BLEND_POTION_5 = 308, // Attack Value
	AFFECT_BLEND_POTION_6 = 309, // Defense
	AFFECT_ENERGY = 310,
	AFFECT_DRAGON_GOD_1 = 311, // Max HP.
	AFFECT_DRAGON_GOD_2 = 312, // Max SP.
	AFFECT_DRAGON_GOD_3 = 313, // Attack Value
	AFFECT_DRAGON_GOD_4 = 314, // Defense
	AFFECT_CRITICAL = 315,
	AFFECT_PENETRATE = 316,
	AFFECT_ATTACK_SPEED = 317,
	AFFECT_MOVE_SPEED = 318,
#endif

	AFFECT_RESEARCHER_ELIXIR = 400,
#if defined(__SHIP_DEFENSE__)
	AFFECT_DEFENSEWAVE_LASER = 573,
#endif
	AFFECT_SAFE_BOX_BUFF = 580,

#if defined(__CONQUEROR_LEVEL__)
	AFFECT_SUNGMA_STR = 623,
	AFFECT_SUNGMA_HP = 624,
	AFFECT_SUNGMA_MOVE = 625,
	AFFECT_SUNGMA_IMMUNE = 626,
#endif

#if defined(__GUILD_DRAGONLAIR__)
	AFFECT_STATUE = 700,
#endif

#if defined(__SOUL_SYSTEM__)
	AFFECT_SOUL_RED = 720,
	AFFECT_SOUL_BLUE = 721,
	AFFECT_SOUL_MIX = 723,
#endif

	AFFECT_QUEST_START_IDX = 1000
};

enum EAffectBits
{
	AFF_NONE,

	AFF_YMIR,
	AFF_INVISIBILITY,
	AFF_SPAWN,

	AFF_POISON,
	AFF_SLOW,
	AFF_STUN,

	AFF_DUNGEON_READY, // 던전에서 준비 상태
	AFF_DUNGEON_UNIQUE, // 던전 유니크 (클라이언트에서 컬링되지않음)

	AFF_BUILDING_CONSTRUCTION_SMALL,
	AFF_BUILDING_CONSTRUCTION_LARGE,
	AFF_BUILDING_UPGRADE,

	AFF_MOV_SPEED_POTION,
	AFF_ATT_SPEED_POTION,

	AFF_FISH_MIND,

	AFF_JEONGWIHON, // 전귀혼
	AFF_GEOMGYEONG, // 검경
	AFF_CHEONGEUN, // 천근추
	AFF_GYEONGGONG, // 경공술
	AFF_EUNHYUNG, // 은형법
	AFF_GWIGUM, // 귀검
	AFF_TERROR, // 공포
	AFF_JUMAGAP, // 주마갑
	AFF_HOSIN, // 호신
	AFF_BOHO, // 보호
	AFF_KWAESOK, // 쾌속
	AFF_MANASHIELD, // 마나쉴드
	AFF_MUYEONG, // 무영진 affect
	AFF_REVIVE_INVISIBLE, // 부활시 잠시동안 무적
	AFF_FIRE, // 지속 불 데미지
	AFF_GICHEON, // 기천대공
	AFF_JEUNGRYEOK, // 증력술
	AFF_TANHWAN_DASH, // 탄환격용 달리기어펙트
	AFF_PABEOP, // 파법술
	AFF_CHEONGEUN_WITH_FALL, // 천근추
	AFF_POLYMORPH,
	AFF_WAR_FLAG1,
	AFF_WAR_FLAG2,
	AFF_WAR_FLAG3,

	AFF_CHINA_FIREWORK,
	AFF_HAIR, // 헤어
	AFF_GERMANY, // 독일

	AFF_DS,

	AFF_BLEEDING, // 42
	AFF_RED_POSSESSION, // 43
	AFF_BLUE_POSSESSION, // 44

#if defined(__GUILD_DRAGONLAIR__)
	AFF_STATUE1, // 45
	AFF_STATUE2, // 46
	AFF_STATUE3, // 47
	AFF_STATUE4, // 48
#endif

#if defined(__SOUL_SYSTEM__)
	AFF_SOUL_RED, // 49
	AFF_SOUL_BLUE, // 50
	AFF_SOUL_MIX, // 51
#endif

#if defined(__9TH_SKILL__)
	AFF_CHEONUN, // 52
	AFF_CHUNWOON_MOOJUK, // 53
#endif

#if defined(__SHIP_DEFENSE__)
	AFF_DEFENSE_WAVE_LASER, // 54
#endif

	AFF_BITS_MAX
};

extern void SendAffectAddPacket(LPDESC d, CAffect* pkAff);

// AFFECT_DURATION_BUG_FIX
enum AffectVariable
{
	// Affect가 무한대로 들어가 있어야 할 경우 사용.
	// 시간을 계속 줄이기 때문에 매우 큰값으로 무한대를 에뮬레이션함.
	//// 24비트는 적으므로 25비트를 사용.
	// ... 25비트 사용한다고 해놓고선 29bit 사용하고 있는 엄청난 주석이란...
	// collect quest에서 무한 시간을 60년으로 사용하고 있으므로, 여기도 60년으로 하자.

	INFINITE_AFFECT_DURATION = 60 * 365 * 24 * 60 * 60
};
// END_AFFECT_DURATION_BUG_FIX

#endif // __INC_AFFECT_H__
