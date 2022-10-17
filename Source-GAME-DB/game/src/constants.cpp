#include "stdafx.h"
#include "char.h"

TJobInitialPoints JobInitialPoints[JOB_MAX_NUM] =
/*
{
	int st, ht, dx, iq;
	int max_hp, max_sp;
	int hp_per_ht, sp_per_iq;
	int hp_per_lv_begin, hp_per_lv_end;
	int sp_per_lv_begin, sp_per_lv_end;
	int max_stamina;
	int stamina_per_con;
	int stamina_per_lv_begin, stamina_per_lv_end;
}
*/
#if defined(__RANDOM_STATUS_PER_LEVEL__)
{ // Random HP & SP
	// str con dex int 초기HP 초기SP CON/HP INT/SP HP랜덤/lv MP랜덤/lv 초기stam stam/con stam/lv
	{ 6, 4, 3, 3, 600, 200, 40, 20, 36, 44, 18, 22, 800, 5, 1, 3 }, // JOB_WARRIOR 16
	{ 4, 3, 6, 3, 650, 200, 40, 20, 36, 44, 18, 22, 800, 5, 1, 3 }, // JOB_ASSASSIN 16
	{ 5, 3, 3, 5, 650, 200, 40, 20, 36, 44, 18, 22, 800, 5, 1, 3 }, // JOB_SURA 16
	{ 3, 4, 3, 6, 700, 200, 40, 20, 36, 44, 18, 22, 800, 5, 1, 3 }, // JOB_SHAMAN 16
	{ 2, 6, 6, 2, 600, 200, 40, 20, 36, 44, 18, 22, 800, 5, 1, 3 }, // JOB_WOLFMAN 16
};
#else
{
	// str con dex int 초기HP 초기SP CON/HP INT/SP HP랜덤/lv MP랜덤/lv 초기stam stam/con stam/lv
	{ 6, 4, 3, 3, 600, 200, 40, 20, 0, 0, 0, 0, 800, 5, 1, 3 }, // JOB_WARRIOR 16
	{ 4, 3, 6, 3, 650, 200, 40, 20, 0, 0, 0, 0, 800, 5, 1, 3 }, // JOB_ASSASSIN 16
	{ 5, 3, 3, 5, 650, 200, 40, 20, 0, 0, 0, 0, 800, 5, 1, 3 }, // JOB_SURA 16
	{ 3, 4, 3, 6, 700, 200, 40, 20, 0, 0, 0, 0, 800, 5, 1, 3 }, // JOB_SHAMAN 16
	{ 2, 6, 6, 2, 600, 200, 40, 20, 0, 0, 0, 0, 800, 5, 1, 3 }, // JOB_WOLFMAN 16
};
#endif

const TMobRankStat MobRankStats[MOB_RANK_MAX_NUM] =
/*
{
	int iGoldPercent;
}
*/
{
	{ 20, }, // MOB_RANK_PAWN,
	{ 20, }, // MOB_RANK_S_PAWN,
	{ 25, }, // MOB_RANK_KNIGHT,
	{ 30, }, // MOB_RANK_S_KNIGHT,
	{ 50, }, // MOB_RANK_BOSS,
	{ 100, } // MOB_RANK_KING,
};

TBattleTypeStat BattleTypeStats[BATTLE_TYPE_MAX_NUM] =
/*
{
	int AttGradeBias;
	int DefGradeBias;
	int MagicAttGradeBias;
	int MagicDefGradeBias;
}
*/
{
	{ 0, 0, 0, -10 }, // BATTLE_TYPE_MELEE,
	{ 10, -20, -10, -15 }, // BATTLE_TYPE_RANGE,
	{ -5, -5, 10, 10 }, // BATTLE_TYPE_MAGIC,
	{ 0, 0, 0, 0 }, // BATTLE_TYPE_SPECIAL,
	{ 10, -10, 0, -15 }, // BATTLE_TYPE_POWER,
	{ -10, 10, -10, 0 }, // BATTLE_TYPE_TANKER,
	{ 20, -20, 0, -10 }, // BATTLE_TYPE_SUPER_POWER,
	{ -20, 20, -10, 0 }, // BATTLE_TYPE_SUPER_TANKER,
};

const DWORD* exp_table = NULL;

const DWORD exp_table_common[PLAYER_EXP_TABLE_MAX + 1] =
{
	0, // 0
	300,
	800,
	1500,
	2500,
	4300,
	7200,
	11000,
	17000,
	24000,
	33000, // 10
	43000,
	58000,
	76000,
	100000,
	130000,
	169000,
	219000,
	283000,
	365000,
	472000, // 20
	610000,
	705000,
	813000,
	937000,
	1077000,
	1237000,
	1418000,
	1624000,
	1857000,
	2122000, // 30
	2421000,
	2761000,
	3145000,
	3580000,
	4073000,
	4632000,
	5194000,
	5717000,
	6264000,
	6837000, // 40
	7600000,
	8274000,
	8990000,
	9753000,
	10560000,
	11410000,
	12320000,
	13270000,
	14280000,
	15340000, // 50
	16870000,
	18960000,
	19980000,
	21420000,
	22930000,
	24530000,
	26200000,
	27960000,
	29800000,
	32780000, // 60
	36060000,
	39670000,
	43640000,
	48000000,
	52800000,
	58080000,
	63890000,
	70280000,
	77310000,
	85040000, // 70
	93540000,
	102900000,
	113500000,
	121000000,
	137000000,
	150700000,
	165700000,
	236990000,
	260650000,
	286780000, // 80
	315000000,
	346970000,
	381680000,
	419770000,
	461760000,
	508040000,
	558740000,
	614640000,
	676130000,
	743730000, // 90
	1041222000,
	1145344200,
	1259878620,
	1385866482,
	1524453130,
	1676898443,
	1844588288,
	2029047116,
	2050000000, // 99레벨 일 때 필요경험치 (100레벨이 되기 위한)
	2150000000, // 100
	2210000000,
	2250000000,
	2280000000,
	2310000000,
	2340000000,
	2370000000,
	2400000000,
	2430000000,
	2460000000,
	2490000000, // 110
	2520000000,
	2550000000,
	2580000000,
	2610000000,
	2640000000,
	2670000000,
	2700000000,
	2730000000,
	2760000000,
	2790000000, // 120
};

#if defined(__CONQUEROR_LEVEL__)
const DWORD conqueror_exp_table[PLAYER_CONQUEROR_EXP_TABLE_MAX + 1] =
{
	0, // 0
	433400,
	1300204,
	2600413,
	4334029,
	6501054,
	9101490,
	12135339,
	15602603,
	19503284,
	23837384, // 10
	28604905,
	33805849,
	39440218,
	45508014,
	52009239,
	58943895,
	66311984,
	74113508,
	82348469,
	91016869, // 20
	100118710,
	109653994,
	119622723,
	130024899,
	140860524,
	152129600,
	163832129,
	175968113,
	188537554,
	188537554, // 30
};
#endif

const int* aiPercentByDeltaLev = NULL;
const int* aiPercentByDeltaLevForBoss = NULL;

// 적과 나와의 레벨차이에 의한 계산에 사용되는 테이블
// MIN(MAX_EXP_DELTA_OF_LEV - 1, (적렙 + 15) - 내렙))
const int aiPercentByDeltaLevForBoss_euckr[MAX_EXP_DELTA_OF_LEV] =
{
	1,		// -15	0
	3,		// -14	1
	5,		// -13	2
	7,		// -12	3
	15,		// -11	4
	30,		// -10	5
	60,		// -9	6
	90,		// -8	7
	91,		// -7	8
	92,		// -6	9
	93,		// -5	10
	94,		// -4	11
	95,		// -3	12
	97,		// -2	13
	99,		// -1	14
	100,	// 0	15
	105,	// 1	16
	110,	// 2	17
	115,	// 3	18
	120,	// 4	19
	125,	// 5	20
	130,	// 6	21
	135,	// 7	22
	140,	// 8	23
	145,	// 9	24
	150,	// 10	25
	155,	// 11	26
	160,	// 12	27
	165,	// 13	28
	170,	// 14	29
	180,	// 15	30
};

const int aiPercentByDeltaLev_euckr[MAX_EXP_DELTA_OF_LEV] =
{
	1,		// -15	0
	5,		// -14	1
	10,		// -13	2
	20,		// -12	3
	30,		// -11	4
	50,		// -10	5
	70,		// -9	6
	80,		// -8	7
	85,		// -7	8
	90,		// -6	9
	92,		// -5	10
	94,		// -4	11
	96,		// -3	12
	98,		// -2	13
	100,	// -1	14
	100,	// 0	15
	105,	// 1	16
	110,	// 2	17
	115,	// 3	18
	120,	// 4	19
	125,	// 5	20
	130,	// 6	21
	135,	// 7	22
	140,	// 8	23
	145,	// 9	24
	150,	// 10	25
	155,	// 11	26
	160,	// 12	27
	165,	// 13	28
	170,	// 14	29
	180,	// 15	30
};

const DWORD party_exp_distribute_table[PLAYER_MAX_LEVEL_CONST + 1] =
{
	0,
	10,		10,		10,		10,		15,		15,		20,		25,		30,		40,		// 1 - 10
	50,		60,		80,		100,	120,	140,	160,	184,	210,	240,	// 11 - 20
	270,	300,	330,	360,	390,	420,	450,	480,	510,	550,	// 21 - 30
	600,	640,	700,	760,	820,	880,	940,	1000,	1100,	1180,	// 31 - 40
	1260,	1320,	1380,	1440,	1500,	1560,	1620,	1680,	1740,	1800,	// 41 - 50
	1860,	1920,	2000,	2100,	2200,	2300,	2450,	2600,	2750,	2900,	// 51 - 60
	3050,	3200,	3350,	3500,	3650,	3800,	3950,	4100,	4250,	4400,	// 61 - 70
	4600,	4800,	5000,	5200,	5400,	5600,	5800,	6000,	6200,	6400,	// 71 - 80
	6600,	6900,	7100,	7300,	7600,	7800,	8000,	8300,	8500,	8800,	// 81 - 90
	9000,	9000,	9000,	9000,	9000,	9000,	9000,	9000,	9000,	9000,	// 91 - 100
	10000,	10000,	10000,	10000,	10000,	10000,	10000,	10000,	10000,	10000,	// 101 - 110
	12000,	12000,	12000,	12000,	12000,	12000,	12000,	12000,	12000,	12000,	// 111 - 120
};

Coord aArroundCoords[ARROUND_COORD_MAX_NUM] =
{
	{ 0, 0 },
	{ 0, 50 },
	{ 35, 35 },
	{ 50, -0 },
	{ 35, -35 },
	{ 0, -50 },
	{ -35, -35 },
	{ -50, 0 },
	{ -35, 35 },
	{ 0, 100 },
	{ 71, 71 },
	{ 100, -0 },
	{ 71, -71 },
	{ 0, -100 },
	{ -71, -71 },
	{ -100, 0 },
	{ -71, 71 },
	{ 0, 150 },
	{ 106, 106 },
	{ 150, -0 },
	{ 106, -106 },
	{ 0, -150 },
	{ -106, -106 },
	{ -150, 0 },
	{ -106, 106 },
	{ 0, 200 },
	{ 141, 141 },
	{ 200, -0 },
	{ 141, -141 },
	{ 0, -200 },
	{ -141, -141 },
	{ -200, 0 },
	{ -141, 141 },
	{ 0, 250 },
	{ 177, 177 },
	{ 250, -0 },
	{ 177, -177 },
	{ 0, -250 },
	{ -177, -177 },
	{ -250, 0 },
	{ -177, 177 },
	{ 0, 300 },
	{ 212, 212 },
	{ 300, -0 },
	{ 212, -212 },
	{ 0, -300 },
	{ -212, -212 },
	{ -300, 0 },
	{ -212, 212 },
	{ 0, 350 },
	{ 247, 247 },
	{ 350, -0 },
	{ 247, -247 },
	{ 0, -350 },
	{ -247, -247 },
	{ -350, 0 },
	{ -247, 247 },
	{ 0, 400 },
	{ 283, 283 },
	{ 400, -0 },
	{ 283, -283 },
	{ 0, -400 },
	{ -283, -283 },
	{ -400, 0 },
	{ -283, 283 },
	{ 0, 450 },
	{ 318, 318 },
	{ 450, -0 },
	{ 318, -318 },
	{ 0, -450 },
	{ -318, -318 },
	{ -450, 0 },
	{ -318, 318 },
	{ 0, 500 },
	{ 354, 354 },
	{ 500, -0 },
	{ 354, -354 },
	{ 0, -500 },
	{ -354, -354 },
	{ -500, 0 },
	{ -354, 354 },
	{ 0, 550 },
	{ 389, 389 },
	{ 550, -0 },
	{ 389, -389 },
	{ 0, -550 },
	{ -389, -389 },
	{ -550, 0 },
	{ -389, 389 },
	{ 0, 600 },
	{ 424, 424 },
	{ 600, -0 },
	{ 424, -424 },
	{ 0, -600 },
	{ -424, -424 },
	{ -600, 0 },
	{ -424, 424 },
	{ 0, 650 },
	{ 460, 460 },
	{ 650, -0 },
	{ 460, -460 },
	{ 0, -650 },
	{ -460, -460 },
	{ -650, 0 },
	{ -460, 460 },
	{ 0, 700 },
	{ 495, 495 },
	{ 700, -0 },
	{ 495, -495 },
	{ 0, -700 },
	{ -495, -495 },
	{ -700, 0 },
	{ -495, 495 },
	{ 0, 750 },
	{ 530, 530 },
	{ 750, -0 },
	{ 530, -530 },
	{ 0, -750 },
	{ -530, -530 },
	{ -750, 0 },
	{ -530, 530 },
	{ 0, 800 },
	{ 566, 566 },
	{ 800, -0 },
	{ 566, -566 },
	{ 0, -800 },
	{ -566, -566 },
	{ -800, 0 },
	{ -566, 566 },
	{ 0, 850 },
	{ 601, 601 },
	{ 850, -0 },
	{ 601, -601 },
	{ 0, -850 },
	{ -601, -601 },
	{ -850, 0 },
	{ -601, 601 },
	{ 0, 900 },
	{ 636, 636 },
	{ 900, -0 },
	{ 636, -636 },
	{ 0, -900 },
	{ -636, -636 },
	{ -900, 0 },
	{ -636, 636 },
	{ 0, 950 },
	{ 672, 672 },
	{ 950, -0 },
	{ 672, -672 },
	{ 0, -950 },
	{ -672, -672 },
	{ -950, 0 },
	{ -672, 672 },
	{ 0, 1000 },
	{ 707, 707 },
	{ 1000, -0 },
	{ 707, -707 },
	{ 0, -1000 },
	{ -707, -707 },
	{ -1000, 0 },
	{ -707, 707 },
};

const DWORD guild_exp_table[GUILD_MAX_LEVEL + 1] =
{
	0,
	15000UL,
	45000UL,
	90000UL,
	160000UL,
	235000UL,
	325000UL,
	430000UL,
	550000UL,
	685000UL,
	835000UL,
	1000000UL,
	1500000UL,
	2100000UL,
	2800000UL,
	3600000UL,
	4500000UL,
	6500000UL,
	8000000UL,
	10000000UL,
	42000000UL
};

// INTERNATIONAL_VERSION 길드경험치
const DWORD guild_exp_table2[GUILD_MAX_LEVEL + 1] =
{
	0,
	6000UL,
	18000UL,
	36000UL,
	64000UL,
	94000UL,
	130000UL,
	172000UL,
	220000UL,
	274000UL,
	334000UL,
	400000UL,
	600000UL,
	840000UL,
	1120000UL,
	1440000UL,
	1800000UL,
	2600000UL,
	3200000UL,
	4000000UL,
	16800000UL
};
// END_OF_INTERNATIONAL_VERSION 길드경험치

const int aiMobEnchantApplyIdx[MOB_ENCHANTS_MAX_NUM] =
{
	APPLY_CURSE_PCT,
	APPLY_SLOW_PCT,
	APPLY_POISON_PCT,
	APPLY_STUN_PCT,
	APPLY_CRITICAL_PCT,
	APPLY_PENETRATE_PCT,
};

const int aiMobResistsApplyIdx[MOB_RESISTS_MAX_NUM] =
{
	APPLY_RESIST_SWORD,
	APPLY_RESIST_TWOHAND,
	APPLY_RESIST_DAGGER,
	APPLY_RESIST_BELL,
	APPLY_RESIST_FAN,
	APPLY_RESIST_BOW,
	APPLY_RESIST_CLAW,
	APPLY_RESIST_FIRE,
	APPLY_RESIST_ELEC,
	APPLY_RESIST_MAGIC,
	APPLY_RESIST_WIND,
	APPLY_POISON_REDUCE,
	APPLY_BLEEDING_REDUCE,
};

#if defined(__ELEMENT_SYSTEM__)
const int aiMobElementsApplyIdx[MOB_ELEMENT_MAX_NUM] =
{
	APPLY_ENCHANT_ELECT,
	APPLY_ENCHANT_FIRE,
	APPLY_ENCHANT_ICE,
	APPLY_ENCHANT_WIND,
	APPLY_ENCHANT_EARTH,
	APPLY_ENCHANT_DARK
};
#endif

const int aiSocketPercentByQty[5][4] =
{
	{ 0, 0, 0, 0 },
	{ 3, 0, 0, 0 },
	{ 10, 1, 0, 0 },
	{ 15, 10, 1, 0 },
	{ 20, 15, 10, 1 }
};

const int aiWeaponSocketQty[WEAPON_NUM_TYPES] =
{
#if defined(__ITEM_SOCKET5__)
	5, // WEAPON_SWORD,
	5, // WEAPON_DAGGER,
	5, // WEAPON_BOW,
	5, // WEAPON_TWO_HANDED,
	5, // WEAPON_BELL,
	5, // WEAPON_FAN,
#else
	3, // WEAPON_SWORD,
	3, // WEAPON_DAGGER,
	3, // WEAPON_BOW,
	3, // WEAPON_TWO_HANDED,
	3, // WEAPON_BELL,
	3, // WEAPON_FAN,
#endif
	0, // WEAPON_ARROW,
	0, // WEAPON_MOUNT_SPEAR
	3, // WEAPON_CLAW
#if defined(__QUIVER_SYSTEM__)
	0, // WEAPON_QUIVER,
#endif
};

const int aiArmorSocketQty[ARMOR_NUM_TYPES] =
{
#if defined(__ITEM_SOCKET5__)
	5, // ARMOR_BODY,
#else
	3, // ARMOR_BODY,
#endif
	1, // ARMOR_HEAD,
	1, // ARMOR_SHIELD,
	0, // ARMOR_WRIST,
	0, // ARMOR_FOOTS,
	0 // ARMOR_ACCESSORY
};

TItemAttrMap g_map_itemAttr;
TItemAttrMap g_map_itemRare;

const TApplyInfo aApplyInfo[MAX_APPLY_NUM] =
/*
{
	DWORD dwPointType;
}
*/
{
	// Point Type
	{ POINT_NONE, },						// APPLY_NONE,						// 0
	{ POINT_MAX_HP, },						// APPLY_MAX_HP,					// 1
	{ POINT_MAX_SP, },						// APPLY_MAX_SP,					// 2
	{ POINT_HT, },							// APPLY_CON,						// 3
	{ POINT_IQ, },							// APPLY_INT,						// 4
	{ POINT_ST, },							// APPLY_STR,						// 5
	{ POINT_DX, },							// APPLY_DEX,						// 6
	{ POINT_ATT_SPEED, },					// APPLY_ATT_SPEED,					// 7
	{ POINT_MOV_SPEED, },					// APPLY_MOV_SPEED,					// 8
	{ POINT_CASTING_SPEED, },				// APPLY_CAST_SPEED,				// 9
	{ POINT_HP_REGEN, },					// APPLY_HP_REGEN,					// 10
	{ POINT_SP_REGEN, },					// APPLY_SP_REGEN,					// 11
	{ POINT_POISON_PCT, },					// APPLY_POISON_PCT,				// 12
	{ POINT_STUN_PCT, },					// APPLY_STUN_PCT,					// 13
	{ POINT_SLOW_PCT, },					// APPLY_SLOW_PCT,					// 14
	{ POINT_CRITICAL_PCT, },				// APPLY_CRITICAL_PCT,				// 15
	{ POINT_PENETRATE_PCT, },				// APPLY_PENETRATE_PCT,				// 16
	{ POINT_ATTBONUS_HUMAN, },				// APPLY_ATTBONUS_HUMAN,			// 17
	{ POINT_ATTBONUS_ANIMAL, },				// APPLY_ATTBONUS_ANIMAL,			// 18
	{ POINT_ATTBONUS_ORC,},					// APPLY_ATTBONUS_ORC,				// 19
	{ POINT_ATTBONUS_MILGYO, },				// APPLY_ATTBONUS_MILGYO,			// 20
	{ POINT_ATTBONUS_UNDEAD, },				// APPLY_ATTBONUS_UNDEAD,			// 21
	{ POINT_ATTBONUS_DEVIL, },				// APPLY_ATTBONUS_DEVIL,			// 22
	{ POINT_STEAL_HP, },					// APPLY_STEAL_HP,					// 23
	{ POINT_STEAL_SP, },					// APPLY_STEAL_SP,					// 24
	{ POINT_MANA_BURN_PCT, },				// APPLY_MANA_BURN_PCT,				// 25
	{ POINT_DAMAGE_SP_RECOVER, },			// APPLY_DAMAGE_SP_RECOVER,			// 26
	{ POINT_BLOCK, },						// APPLY_BLOCK,						// 27
	{ POINT_DODGE, },						// APPLY_DODGE,						// 28
	{ POINT_RESIST_SWORD, },				// APPLY_RESIST_SWORD,				// 29
	{ POINT_RESIST_TWOHAND, },				// APPLY_RESIST_TWOHAND,			// 30
	{ POINT_RESIST_DAGGER, },				// APPLY_RESIST_DAGGER,				// 31
	{ POINT_RESIST_BELL, },					// APPLY_RESIST_BELL,				// 32
	{ POINT_RESIST_FAN, },					// APPLY_RESIST_FAN,				// 33
	{ POINT_RESIST_BOW, },					// APPLY_RESIST_BOW,				// 34
	{ POINT_RESIST_FIRE, },					// APPLY_RESIST_FIRE,				// 35
	{ POINT_RESIST_ELEC, },					// APPLY_RESIST_ELEC,				// 36
	{ POINT_RESIST_MAGIC, },				// APPLY_RESIST_MAGIC,				// 37
	{ POINT_RESIST_WIND, },					// APPLY_RESIST_WIND,				// 38
	{ POINT_REFLECT_MELEE, },				// APPLY_REFLECT_MELEE,				// 39
	{ POINT_REFLECT_CURSE, },				// APPLY_REFLECT_CURSE,				// 40
	{ POINT_POISON_REDUCE, },				// APPLY_POISON_REDUCE,				// 41
	{ POINT_KILL_SP_RECOVER, },				// APPLY_KILL_SP_RECOVER,			// 42
	{ POINT_EXP_DOUBLE_BONUS, },			// APPLY_EXP_DOUBLE_BONUS,			// 43
	{ POINT_GOLD_DOUBLE_BONUS, },			// APPLY_GOLD_DOUBLE_BONUS,			// 44
	{ POINT_ITEM_DROP_BONUS, },				// APPLY_ITEM_DROP_BONUS,			// 45
	{ POINT_POTION_BONUS, },				// APPLY_POTION_BONUS,				// 46
	{ POINT_KILL_HP_RECOVERY, },			// APPLY_KILL_HP_RECOVER,			// 47
	{ POINT_IMMUNE_STUN, },					// APPLY_IMMUNE_STUN,				// 48
	{ POINT_IMMUNE_SLOW, },					// APPLY_IMMUNE_SLOW,				// 49
	{ POINT_IMMUNE_FALL, },					// APPLY_IMMUNE_FALL,				// 50
	{ POINT_NONE, },						// APPLY_SKILL,						// 51
	{ POINT_BOW_DISTANCE, },				// APPLY_BOW_DISTANCE,				// 52
	{ POINT_ATT_GRADE_BONUS, },				// APPLY_ATT_GRADE,					// 53
	{ POINT_DEF_GRADE_BONUS, },				// APPLY_DEF_GRADE,					// 54
	{ POINT_MAGIC_ATT_GRADE_BONUS, },		// APPLY_MAGIC_ATT_GRADE,			// 55
	{ POINT_MAGIC_DEF_GRADE_BONUS, },		// APPLY_MAGIC_DEF_GRADE,			// 56
	{ POINT_CURSE_PCT, },					// APPLY_CURSE_PCT,					// 57
	{ POINT_MAX_STAMINA, },					// APPLY_MAX_STAMINA,				// 58
	{ POINT_ATTBONUS_WARRIOR, },			// APPLY_ATTBONUS_WARRIOR,			// 59
	{ POINT_ATTBONUS_ASSASSIN, },			// APPLY_ATTBONUS_ASSASSIN,			// 60
	{ POINT_ATTBONUS_SURA, },				// APPLY_ATTBONUS_SURA,				// 61
	{ POINT_ATTBONUS_SHAMAN, },				// APPLY_ATTBONUS_SHAMAN,			// 62
	{ POINT_ATTBONUS_MONSTER, },			// APPLY_ATTBONUS_MONSTER,			// 63
	{ POINT_ATT_BONUS, },					// APPLY_MALL_ATTBONUS,				// 64 공격력 +x%
	{ POINT_MALL_DEFBONUS, },				// APPLY_MALL_DEFBONUS,				// 65 방어력 +x%
	{ POINT_MALL_EXPBONUS, },				// APPLY_MALL_EXPBONUS,				// 66 경픽치 +x%
	{ POINT_MALL_ITEMBONUS, },				// APPLY_MALL_ITEMBONUS,			// 67 아이템 드롭율 x/10배
	{ POINT_MALL_GOLDBONUS, },				// APPLY_MALL_GOLDBONUS,			// 68 돈 드롭율 x/10배
	{ POINT_MAX_HP_PCT, },					// APPLY_MAX_HP_PCT,				// 69 최대 생명력 +x%
	{ POINT_MAX_SP_PCT, },					// APPLY_MAX_SP_PCT,				// 70 최대 정신력 +x%
	{ POINT_SKILL_DAMAGE_BONUS, },			// APPLY_SKILL_DAMAGE_BONUS,		// 71 스킬 데미지 * (100+x)%
	{ POINT_NORMAL_HIT_DAMAGE_BONUS, },		// APPLY_NORMAL_HIT_DAMAGE_BONUS,	// 72 평타 데미지 * (100+x)%

	// DEFEND_BONUS_ATTRIBUTES
	{ POINT_SKILL_DEFEND_BONUS, },			// APPLY_SKILL_DEFEND_BONUS,		// 73 스킬 데미지 방어 * (100-x)%
	{ POINT_NORMAL_HIT_DEFEND_BONUS, },		// APPLY_NORMAL_HIT_DEFEND_BONUS,	// 74 평타 데미지 방어 * (100-x)%
	// END_OF_DEFEND_BONUS_ATTRIBUTES

	// PC_BANG_ITEM_ADD
	{ POINT_PC_BANG_EXP_BONUS, },			// APPLY_PC_BANG_EXP_BONUS,			// 75 PC방 아이템 EXP 보너스
	{ POINT_PC_BANG_DROP_BONUS, },			// APPLY_PC_BANG_DROP_BONUS,		// 76 PC방 아이템 드롭율 보너스
	// END_PC_BANG_ITEM_ADD

	{ POINT_NONE, },						// APPLY_EXTRACT_HP_PCT, UNUSED		// 77 사용시 HP 소모

	{ POINT_RESIST_WARRIOR, },				// APPLY_RESIST_WARRIOR,			// 78 무사에게 저항
	{ POINT_RESIST_ASSASSIN, },				// APPLY_RESIST_ASSASSIN,			// 79 자객에게 저항
	{ POINT_RESIST_SURA, },					// APPLY_RESIST_SURA,				// 80 수라에게 저항
	{ POINT_RESIST_SHAMAN, },				// APPLY_RESIST_SHAMAN,				// 81 무당에게 저항
	{ POINT_ENERGY, },						// APPLY_ENERGY,					// 82 기력
	{ POINT_DEF_GRADE, },					// APPLY_DEF_GRADE,					// 83 방어력. DEF_GRADE_BONUS는 클라에서 두배로 보여지는 의도된 버그(...)가 있다.
	{ POINT_COSTUME_ATTR_BONUS, },			// APPLY_COSTUME_ATTR_BONUS,		// 84 코스튬 아이템에 붙은 속성치 보너스
	{ POINT_MAGIC_ATT_BONUS_PER, },			// APPLY_MAGIC_ATTBONUS_PER,		// 85 마법 공격력 +x%
	{ POINT_MELEE_MAGIC_ATT_BONUS_PER, },	// APPLY_MELEE_MAGIC_ATTBONUS_PER,	// 86 마법 + 밀리 공격력 +x%

	{ POINT_RESIST_ICE, },					// APPLY_RESIST_ICE,				// 87 냉기 저항
	{ POINT_RESIST_EARTH, },				// APPLY_RESIST_EARTH,				// 88 대지 저항
	{ POINT_RESIST_DARK, },					// APPLY_RESIST_DARK,				// 89 어둠 저항

	{ POINT_RESIST_CRITICAL, },				// APPLY_ANTI_CRITICAL_PCT,			// 90 크리티컬 저항
	{ POINT_RESIST_PENETRATE, },			// APPLY_ANTI_PENETRATE_PCT,		// 91 관통타격 저항

	{ POINT_BLEEDING_REDUCE, },				// APPLY_BLEEDING_REDUCE,			// 92
	{ POINT_BLEEDING_PCT, },				// APPLY_BLEEDING_PCT,				// 93
	{ POINT_ATTBONUS_WOLFMAN, },			// APPLY_ATT_BONUS_TO_WOLFMAN,		// 94 수인족에게 강함
	{ POINT_RESIST_WOLFMAN, },				// APPLY_RESIST_WOLFMAN,			// 95 수인족에게 저항
	{ POINT_RESIST_CLAW, },					// APPLY_RESIST_CLAW,				// 96 CLAW에게 저항

#if defined(__MOUNT_COSTUME_SYSTEM__)
	{ POINT_MOUNT, },						// APPLY_MOUNT,						// 97
#endif

#if defined(__ACCE_COSTUME_SYSTEM__)
	{ POINT_ACCEDRAIN_RATE, }, 				// APPLY_ACCEDRAIN_RATE,			// 98
#endif

#if defined(__MAGIC_REDUCTION__)
	{ POINT_RESIST_MAGIC_REDUCTION, },		// APPLY_RESIST_MAGIC_REDUCTION,	// 99
#endif

#if defined(__ELEMENT_SYSTEM__)
	{ POINT_ENCHANT_ELECT, },				// APPLY_ENCHANT_ELECT,				// 100
	{ POINT_ENCHANT_FIRE, },				// APPLY_ENCHANT_FIRE,				// 101
	{ POINT_ENCHANT_ICE, },					// APPLY_ENCHANT_ICE,				// 102
	{ POINT_ENCHANT_WIND, },				// APPLY_ENCHANT_WIND,				// 103
	{ POINT_ENCHANT_EARTH, },				// APPLY_ENCHANT_EARTH,				// 104
	{ POINT_ENCHANT_DARK, },				// APPLY_ENCHANT_DARK,				// 105
	{ POINT_ATTBONUS_CZ, },					// APPLY_ATTBONUS_CZ,				// 106
	{ POINT_ATTBONUS_INSECT, },				// APPLY_ATTBONUS_INSECT,			// 107
	{ POINT_ATTBONUS_DESERT, },				// APPLY_ATTBONUS_DESERT,			// 108
	{ POINT_ATTBONUS_SWORD, },				// APPLY_ATTBONUS_SWORD,			// 109
	{ POINT_ATTBONUS_TWOHAND, },			// APPLY_ATTBONUS_TWOHAND,			// 110
	{ POINT_ATTBONUS_DAGGER, },				// APPLY_ATTBONUS_DAGGER,			// 111
	{ POINT_ATTBONUS_BELL, },				// APPLY_ATTBONUS_BELL,				// 112
	{ POINT_ATTBONUS_FAN, },				// APPLY_ATTBONUS_FAN,				// 113
	{ POINT_ATTBONUS_BOW, },				// APPLY_ATTBONUS_BOW,				// 114
	{ POINT_ATTBONUS_CLAW, },				// APPLY_ATTBONUS_CLAW,				// 115
	{ POINT_RESIST_HUMAN, },				// APPLY_RESIST_HUMAN,				// 116
#endif
	{ POINT_ATTBONUS_STONE, },				// APPLY_ATTBONUS_STONE,			// 117
#if defined(__CONQUEROR_LEVEL__)
	{ POINT_SUNGMA_STR, },					// APPLY_SUNGMA_STR,				// 118
	{ POINT_SUNGMA_HP, },					// APPLY_SUNGMA_HP,					// 119
	{ POINT_SUNGMA_MOVE, },					// APPLY_SUNGMA_MOVE,				// 120
	{ POINT_SUNGMA_IMMUNE, },				// APPLY_SUNGMA_IMMUNE,				// 121
#endif
#if defined(__ITEM_APPLY_RANDOM__)
	{ POINT_NONE, },						// APPLY_RANDOM,					// 122
#endif
};

const int aiItemMagicAttributePercentHigh[ITEM_ATTRIBUTE_MAX_LEVEL] =
{
	//25, 25, 40, 8, 2,
	30, 40, 20, 8, 2
};

const int aiItemMagicAttributePercentLow[ITEM_ATTRIBUTE_MAX_LEVEL] =
{
	//45, 25, 20, 10, 0,
	50, 40, 10, 0, 0
};

// ADD_ITEM_ATTRIBUTE
const int aiItemAttributeAddPercent[ITEM_ATTRIBUTE_MAX_NUM] =
{
	100, 80, 60, 50, 30, 0, 0,
};
// END_OF_ADD_ITEM_ATTRIBUTE

const int aiExpLossPercents[PLAYER_EXP_TABLE_MAX + 1] =
{
	0,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 4, // 1 - 10
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, // 11 - 20
	4, 4, 4, 4, 4, 4, 4, 3, 3, 3, // 21 - 30
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, // 31 - 40
	3, 3, 3, 3, 2, 2, 2, 2, 2, 2, // 41 - 50
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, // 51 - 60
	2, 2, 1, 1, 1, 1, 1, 1, 1, 1, // 61 - 70
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 71 - 80
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 81 - 90
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 91 - 100
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 101 - 110
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 111 - 120
};

const int aiSkillBookCountForLevelUp[10] =
{
	3, 3, 3, 3, 3, 4, 4, 5, 5, 6
};

// ADD_GRANDMASTER_SKILL
const int aiGrandMasterSkillBookCountForLevelUp[10] =
{
	3, 3, 5, 5, 7, 7, 10, 10, 10, 20,
};

const int aiGrandMasterSkillBookMinCount[10] =
{
	// 1, 1, 3, 5, 10, 15, 20, 30, 40, 50,
	// 3, 3, 5, 5, 10, 10, 15, 15, 20, 30
	1, 1, 1, 2, 2, 3, 3, 4, 5, 6
};

const int aiGrandMasterSkillBookMaxCount[10] =
{
	// 6, 15, 30, 45, 60, 80, 100, 120, 160, 200,
	// 6, 10, 15, 20, 30, 40, 50, 60, 70, 80
	5, 7, 9, 11, 13, 15, 20, 25, 30, 35
};
// END_OF_ADD_GRANDMASTER_SKILL

const int CHN_aiPartyBonusExpPercentByMemberCount[9] =
{
	0, 0, 12, 18, 26, 40, 53, 70, 100
};

// UPGRADE_PARTY_BONUS
const int KOR_aiPartyBonusExpPercentByMemberCount[9] =
{
	0,
	0,
	30, // 66% * 2 - 100
	60, // 53% * 3 - 100
	75, // 44% * 4 - 100
	90, // 38% * 5 - 100
	105, // 34% * 6 - 100
	110, // 30% * 7 - 100
	140, // 30% * 8 - 100
};

const int KOR_aiUniqueItemPartyBonusExpPercentByMemberCount[9] =
{
	0,
	0,
	15 * 2,
	14 * 3,
	13 * 4,
	12 * 5,
	11 * 6,
	10 * 7,
	10 * 8,
};
// END_OF_UPGRADE_PARTY_BONUS

const int* aiChainLightningCountBySkillLevel = NULL;

const int aiChainLightningCountBySkillLevel_euckr[SKILL_MAX_LEVEL + 1] =
{
	0, // 0
	2, // 1
	2, // 2
	2, // 3
	2, // 4
	2, // 5
	2, // 6
	2, // 7
	2, // 8
	3, // 9
	3, // 10
	3, // 11
	3, // 12
	3, // 13
	3, // 14
	3, // 15
	3, // 16
	3, // 17
	3, // 18
	4, // 19
	4, // 20
	4, // 21
	4, // 22
	4, // 23
	5, // 24
	5, // 25
	5, // 26
	5, // 27
	5, // 28
	5, // 29
	5, // 30
	5, // 31
	5, // 32
	5, // 33
	5, // 34
	5, // 35
	5, // 36
	5, // 37
	5, // 38
	5, // 39
	5, // 40
};

const SStoneDropInfo aStoneDrop[STONE_INFO_MAX_NUM] =
{
	// mob pct { +0 +1 +2 +3 +4 }
	{ 8005, 60, { 30, 30, 30, 9, 1 } },
	{ 8006, 60, { 28, 29, 31, 11, 1 } },
	{ 8007, 60, { 24, 29, 32, 13, 2 } },
	{ 8008, 60, { 22, 28, 33, 15, 2 } },
	{ 8009, 60, { 21, 27, 33, 17, 2 } },
	{ 8010, 60, { 18, 26, 34, 20, 2 } },
	{ 8011, 60, { 14, 26, 35, 22, 3 } },
	{ 8012, 60, { 10, 26, 37, 24, 3 } },
	{ 8013, 60, { 2, 26, 40, 29, 3 } },
	{ 8014, 60, { 0, 26, 41, 30, 3 } },
	{ 8018, 60, { 0, 25, 42, 30, 3 } },
	{ 8019, 60, { 0, 23, 42, 32, 3 } },
	{ 8024, 60, { 0, 22, 43, 32, 3 } },
	{ 8025, 60, { 0, 21, 43, 32, 4 } },
	{ 8026, 60, { 0, 19, 44, 33, 4 } },
	{ 8027, 60, { 0, 17, 45, 34, 4 } },
	{ 8051, 60, { 0, 15, 46, 34, 5 } },
	{ 8052, 60, { 0, 15, 46, 34, 5 } },
	{ 8053, 60, { 0, 12, 47, 36, 5 } },
	{ 8054, 60, { 0, 12, 47, 36, 5 } },
	{ 8055, 60, { 0, 9, 48, 37, 6 } },
	{ 8056, 60, { 0, 9, 48, 37, 6 } },
};

const char* c_apszEmpireNames[EMPIRE_MAX_NUM] =
{
	"전제국",
	"신수국",
	"천조국",
	"진노국"
};

const char* c_apszPrivNames[MAX_PRIV_NUM] =
{
	"",
	"아이템이 나올 확률",
	"돈이 나올 확률",
	"돈 대박이 나올 확률",
	"경험치 배율",
};

const int aiPolymorphPowerByLevel[SKILL_MAX_LEVEL + 1] =
{
	10, // 1
	11, // 2
	11, // 3
	12, // 4
	13, // 5
	13, // 6
	14, // 7
	15, // 8
	16, // 9
	17, // 10
	18, // 11
	19, // 12
	20, // 13
	22, // 14
	23, // 15
	24, // 16
	26, // 17
	27, // 18
	29, // 19
	31, // 20
	33, // 21
	35, // 22
	37, // 23
	39, // 24
	41, // 25
	44, // 26
	46, // 27
	49, // 28
	52, // 29
	55, // 30
	59, // 31
	62, // 32
	66, // 33
	70, // 34
	74, // 35
	79, // 36
	84, // 37
	89, // 38
	94, // 39
	100, // 40
};

TGuildWarInfo KOR_aGuildWarInfo[GUILD_WAR_TYPE_MAX_NUM] =
/*
{
	long lMapIndex;
	int iWarPrice;
	int iWinnerPotionRewardPctToWinner;
	int iLoserPotionRewardPctToWinner;
	int iInitialScore;
	int iEndScore;
};
*/
{
	{ 0, 0, 0, 0, 0, 0 },
	{ 110, 0, 100, 50, 0, 100 },
	{ 111, 0, 100, 50, 0, 10 },
};

//
// 악세서리 소켓용 수치들
//

// 다이아몬드로 소켓을 추가할 때 확률
const int aiAccessorySocketAddPct[ITEM_ACCESSORY_SOCKET_MAX_NUM] =
{
	50, 50, 50
};

// 악세서리 수치 값의 몇%만큼의 성능을 추가하는지
const int aiAccessorySocketEffectivePct[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1] =
{
	0, 10, 20, 40
};

// 소켓 지속시간 24, 12, 6
const int aiAccessorySocketDegradeTime[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1] =
{
	0, 3600 * 24, 3600 * 12, 3600 * 6
};

// 소켓 장착 성공률
const int aiAccessorySocketPutPct[ITEM_ACCESSORY_SOCKET_MAX_NUM + 1] =
{
	90, 80, 70, 0
};
// END_OF_ACCESSORY_REFINE

#include "../../common/length.h"
// from import_item_proto.c
typedef struct SValueName
{
	const char* c_pszName;
	long lValue;
} TValueName;

TValueName c_aApplyTypeNames[] =
{
	// APPLY_NONE
	{ "MAX_HP", APPLY_MAX_HP },
	{ "MAX_SP", APPLY_MAX_SP },
	{ "CON", APPLY_CON },
	{ "INT", APPLY_INT },
	{ "STR", APPLY_STR },
	{ "DEX", APPLY_DEX },
	{ "ATT_SPEED", APPLY_ATT_SPEED },
	{ "MOV_SPEED", APPLY_MOV_SPEED },
	{ "CAST_SPEED", APPLY_CAST_SPEED },
	{ "HP_REGEN", APPLY_HP_REGEN },
	{ "SP_REGEN", APPLY_SP_REGEN },
	{ "POISON_PCT", APPLY_POISON_PCT },
	{ "STUN_PCT", APPLY_STUN_PCT },
	{ "SLOW_PCT", APPLY_SLOW_PCT },
	{ "CRITICAL_PCT", APPLY_CRITICAL_PCT },
	{ "PENETRATE_PCT", APPLY_PENETRATE_PCT },
	{ "ATTBONUS_HUMAN", APPLY_ATTBONUS_HUMAN },
	{ "ATTBONUS_ANIMAL", APPLY_ATTBONUS_ANIMAL },
	{ "ATTBONUS_ORC", APPLY_ATTBONUS_ORC },
	{ "ATTBONUS_MILGYO", APPLY_ATTBONUS_MILGYO },
	{ "ATTBONUS_UNDEAD", APPLY_ATTBONUS_UNDEAD },
	{ "ATTBONUS_DEVIL", APPLY_ATTBONUS_DEVIL },
	{ "STEAL_HP", APPLY_STEAL_HP },
	{ "STEAL_SP", APPLY_STEAL_SP },
	{ "MANA_BURN_PCT", APPLY_MANA_BURN_PCT },
	{ "DAMAGE_SP_RECOVER", APPLY_DAMAGE_SP_RECOVER },
	{ "BLOCK", APPLY_BLOCK },
	{ "DODGE", APPLY_DODGE },
	{ "RESIST_SWORD", APPLY_RESIST_SWORD },
	{ "RESIST_TWOHAND", APPLY_RESIST_TWOHAND },
	{ "RESIST_DAGGER", APPLY_RESIST_DAGGER },
	{ "RESIST_BELL", APPLY_RESIST_BELL },
	{ "RESIST_FAN", APPLY_RESIST_FAN },
	{ "RESIST_BOW", APPLY_RESIST_BOW },
	{ "RESIST_FIRE", APPLY_RESIST_FIRE },
	{ "RESIST_ELEC", APPLY_RESIST_ELEC },
	{ "RESIST_MAGIC", APPLY_RESIST_MAGIC },
	{ "RESIST_WIND", APPLY_RESIST_WIND },
	{ "REFLECT_MELEE", APPLY_REFLECT_MELEE },
	{ "REFLECT_CURSE", APPLY_REFLECT_CURSE },
	{ "POISON_REDUCE", APPLY_POISON_REDUCE },
	{ "KILL_SP_RECOVER", APPLY_KILL_SP_RECOVER },
	{ "EXP_DOUBLE_BONUS", APPLY_EXP_DOUBLE_BONUS },
	{ "GOLD_DOUBLE_BONUS", APPLY_GOLD_DOUBLE_BONUS },
	{ "ITEM_DROP_BONUS", APPLY_ITEM_DROP_BONUS },
	{ "POTION_BONUS", APPLY_POTION_BONUS },
	{ "KILL_HP_RECOVER", APPLY_KILL_HP_RECOVER },
	{ "IMMUNE_STUN", APPLY_IMMUNE_STUN },
	{ "IMMUNE_SLOW", APPLY_IMMUNE_SLOW },
	{ "IMMUNE_FALL", APPLY_IMMUNE_FALL },
	{ "SKILL", APPLY_SKILL },
	{ "BOW_DISTANCE", APPLY_BOW_DISTANCE },
	{ "ATT_GRADE_BONUS", APPLY_ATT_GRADE_BONUS },
	{ "DEF_GRADE_BONUS", APPLY_DEF_GRADE_BONUS },
	{ "MAGIC_ATT_GRADE", APPLY_MAGIC_ATT_GRADE },
	{ "MAGIC_DEF_GRADE", APPLY_MAGIC_DEF_GRADE },
	{ "CURSE_PCT", APPLY_CURSE_PCT },
	{ "MAX_STAMINA", APPLY_MAX_STAMINA },
	{ "ATTBONUS_WARRIOR", APPLY_ATT_BONUS_TO_WARRIOR },
	{ "ATTBONUS_ASSASSIN", APPLY_ATT_BONUS_TO_ASSASSIN },
	{ "ATTBONUS_SURA", APPLY_ATT_BONUS_TO_SURA },
	{ "ATTBONUS_SHAMAN", APPLY_ATT_BONUS_TO_SHAMAN },
	{ "ATTBONUS_MONSTER", APPLY_ATT_BONUS_TO_MONSTER },
	{ "MALL_ATTBONUS", APPLY_MALL_ATTBONUS },
	{ "MALL_DEFBONUS", APPLY_MALL_DEFBONUS },
	{ "MALL_EXPBONUS", APPLY_MALL_EXPBONUS },
	{ "MALL_ITEMBONUS", APPLY_MALL_ITEMBONUS },
	{ "MALL_GOLDBONUS", APPLY_MALL_GOLDBONUS },
	{ "MAX_HP_PCT", APPLY_MAX_HP_PCT },
	{ "MAX_SP_PCT", APPLY_MAX_SP_PCT },
	{ "SKILL_DAMAGE_BONUS", APPLY_SKILL_DAMAGE_BONUS },
	{ "NORMAL_HIT_DAMAGE_BONUS", APPLY_NORMAL_HIT_DAMAGE_BONUS },
	{ "SKILL_DEFEND_BONUS", APPLY_SKILL_DEFEND_BONUS },
	{ "NORMAL_HIT_DEFEND_BONUS", APPLY_NORMAL_HIT_DEFEND_BONUS },
	{ "PC_BANG_EXP_BONUS", APPLY_PC_BANG_EXP_BONUS },
	{ "PC_BANG_DROP_BONUS", APPLY_PC_BANG_DROP_BONUS },
	{ "EXTRACT_HP_PCT", APPLY_EXTRACT_HP_PCT },
	{ "RESIST_WARRIOR", APPLY_RESIST_WARRIOR },
	{ "RESIST_ASSASSIN", APPLY_RESIST_ASSASSIN },
	{ "RESIST_SURA", APPLY_RESIST_SURA },
	{ "RESIST_SHAMAN", APPLY_RESIST_SHAMAN },
	{ "ENERGY", APPLY_ENERGY },
	{ "DEF_GRADE", APPLY_DEF_GRADE },
	{ "COSTUME_ATTR_BONUS", APPLY_COSTUME_ATTR_BONUS },
	{ "MAGIC_ATTBONUS_PER", APPLY_MAGIC_ATTBONUS_PER },
	{ "MELEE_MAGIC_ATTBONUS_PER", APPLY_MELEE_MAGIC_ATTBONUS_PER },
	{ "RESIST_ICE", APPLY_RESIST_ICE },
	{ "RESIST_EARTH", APPLY_RESIST_EARTH },
	{ "RESIST_DARK", APPLY_RESIST_DARK },
	{ "RESIST_CRITICAL", APPLY_ANTI_CRITICAL_PCT /* APPLY_RESIST_CRITICAL */ },
	{ "RESIST_PENETRATE", APPLY_ANTI_PENETRATE_PCT /* APPLY_RESIST_PENETRATE */ },
	{ "BLEEDING_REDUCE", APPLY_BLEEDING_REDUCE },
	{ "BLEEDING_PCT", APPLY_BLEEDING_PCT },
	{ "ATTBONUS_WOLFMAN", APPLY_ATT_BONUS_TO_WOLFMAN },
	{ "RESIST_WOLFMAN", APPLY_RESIST_WOLFMAN },
	{ "RESIST_CLAW", APPLY_RESIST_CLAW },
#if defined(__MOUNT_COSTUME_SYSTEM__)
	{ "MOUNT", APPLY_MOUNT },
#endif
#if defined(__ACCE_COSTUME_SYSTEM__)
	{ "ACCEDRAIN_RATE", APPLY_ACCEDRAIN_RATE },
#endif
#if defined(__MAGIC_REDUCTION__)
	{ "RESIST_MAGIC_REDUCTION", APPLY_RESIST_MAGIC_REDUCTION },
#endif
#if defined(__ELEMENT_SYSTEM__)
	{ "ENCHANT_ELECT", APPLY_ENCHANT_ELECT },
	{ "ENCHANT_FIRE", APPLY_ENCHANT_FIRE },
	{ "ENCHANT_ICE", APPLY_ENCHANT_ICE },
	{ "ENCHANT_WIND", APPLY_ENCHANT_WIND },
	{ "ENCHANT_EARTH", APPLY_ENCHANT_EARTH },
	{ "ENCHANT_DARK", APPLY_ENCHANT_DARK },
	{ "ATTBONUS_CZ", APPLY_ATTBONUS_CZ },
	{ "ATTBONUS_INSECT", APPLY_ATTBONUS_INSECT },
	{ "ATTBONUS_DESERT", APPLY_ATTBONUS_DESERT },
	{ "ATTBONUS_SWORD", APPLY_ATTBONUS_SWORD },
	{ "ATTBONUS_TWOHAND", APPLY_ATTBONUS_TWOHAND },
	{ "ATTBONUS_DAGGER", APPLY_ATTBONUS_DAGGER },
	{ "ATTBONUS_BELL", APPLY_ATTBONUS_BELL },
	{ "ATTBONUS_FAN", APPLY_ATTBONUS_FAN },
	{ "ATTBONUS_BOW", APPLY_ATTBONUS_BOW },
	{ "ATTBONUS_CLAW", APPLY_ATTBONUS_CLAW },
	{ "RESIST_HUMAN", APPLY_RESIST_HUMAN },
#endif
	{ "ATTBONUS_STONE", APPLY_ATTBONUS_STONE },
#if defined(__CONQUEROR_LEVEL__)
	{ "SUNGMA_STR", APPLY_SUNGMA_STR },
	{ "SUNGMA_HP", APPLY_SUNGMA_HP },
	{ "SUNGMA_MOVE", APPLY_SUNGMA_MOVE },
	{ "SUNGMA_IMMUNE", APPLY_SUNGMA_IMMUNE },
#endif

	// by mhh game/affect.h 정의되어있음. INFINITE_AFFECT_DURATION = 0x1FFFFFFF
	{ "INFINITE_AFFECT_DURATION", 0x1FFFFFFF },

	//< 2020.08.06.Owsap - Old apply values support.
	{ "ATTACK_SPEED", APPLY_ATT_SPEED },
	{ "MOVE_SPEED", APPLY_MOV_SPEED },
	{ "ATT_BONUS", APPLY_ATT_GRADE_BONUS },
	{ "DEF_BONUS", APPLY_DEF_GRADE_BONUS },
	{ "ATT_BONUS_TO_WARRIOR", APPLY_ATT_BONUS_TO_WARRIOR },
	{ "ATT_BONUS_TO_ASSASSIN", APPLY_ATT_BONUS_TO_ASSASSIN },
	{ "ATT_BONUS_TO_SURA", APPLY_ATT_BONUS_TO_SURA },
	{ "ATT_BONUS_TO_SHAMAN", APPLY_ATT_BONUS_TO_SHAMAN },
	{ "ATT_BONUS_TO_WOLFMAN", APPLY_ATT_BONUS_TO_WOLFMAN },
	//>

	{ NULL, 0 }
};
// from import_item_proto.c

long FN_get_apply_type(const char* apply_type_string)
{
	TValueName* value_name;
	for (value_name = c_aApplyTypeNames; value_name->c_pszName; ++value_name)
	{
		if (0 == strcasecmp(value_name->c_pszName, apply_type_string))
			return value_name->lValue;
	}
	return 0;
}

#if defined(__EREBUS_DUNGEON__)
DWORD dwHealerPos[4/*SUMMON_HEALER_COUNT*/][2] = {
	// X, Y
	{ 776500, 1503700 }, // Healer 1
	{ 773100, 1503700 }, // Healer 2
	{ 773200, 1500400 }, // Healer 3
	{ 776500, 1500400 }, // Healer 4
};

ErebusHealers vErebusHealers = {
	// HP, SP
	{ 75, 4 },
	{ 50, 3 },
	{ 25, 2 },
	{ 2, 1 },
};
#endif

#if defined(__CONQUEROR_LEVEL__)
enum ENewWorlMapIndex
{
	MAP_EMPIRECASTLE = 374,
	MAP_EASTPLAIN_01 = 373,
	MAP_EASTPLAIN_02 = 376,
	MAP_EASTPLAIN_03 = 377,

	MAP_MAZE_DUNGEON_01 = 382,
	MAP_MAZE_DUNGEON_02 = 383,
	MAP_MAZE_DUNGEON_03 = 384
};

TSungMaWillMap SungMaWillMap = {
	{ MAP_EMPIRECASTLE, { 0, 0, 1, 0 } },
	{ MAP_EASTPLAIN_01, { 15, 10, 15, 20 } },
	{ MAP_EASTPLAIN_02, { 20, 10, 20, 25 } },
	{ MAP_EASTPLAIN_03, { 25, 15, 25, 30 } },
	{ MAP_MAZE_DUNGEON_01, { 25, 15, 20, 25 } },
	{ MAP_MAZE_DUNGEON_02, { 55, 35, 45, 45 } },
	{ MAP_MAZE_DUNGEON_03, { 75, 60, 65, 65 } },
};
#endif
