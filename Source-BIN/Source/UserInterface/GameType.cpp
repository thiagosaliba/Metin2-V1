#include "StdAfx.h"
#include "GameType.h"
#include "Packet.h"

std::string g_strResourcePath = "d:/ymir work/";
std::string g_strImagePath = "d:/ymir work/ui/";

std::string g_strGuildSymbolPathName = "mark/10/";

// DEFAULT_FONT
static std::string gs_strDefaultFontName = "굴림체:12.fnt";
static std::string gs_strDefaultItalicFontName = "굴림체:12i.fnt";
static CResource* gs_pkDefaultFont = NULL;
static CResource* gs_pkDefaultItalicFont = NULL;

static bool gs_isReloadDefaultFont = false;

void DefaultFont_Startup()
{
	gs_pkDefaultFont = NULL;
}

void DefaultFont_Cleanup()
{
	if (gs_pkDefaultFont)
		gs_pkDefaultFont->Release();
}

void DefaultFont_SetName(const char* c_szFontName)
{
	gs_strDefaultFontName = c_szFontName;
	gs_strDefaultFontName += ".fnt";

	gs_strDefaultItalicFontName = c_szFontName;
	if (strchr(c_szFontName, ':'))
		gs_strDefaultItalicFontName += "i";
	gs_strDefaultItalicFontName += ".fnt";

	gs_isReloadDefaultFont = true;
}

bool ReloadDefaultFonts()
{
	CResourceManager& rkResMgr = CResourceManager::Instance();

	gs_isReloadDefaultFont = false;

	CResource* pkNewFont = rkResMgr.GetResourcePointer(gs_strDefaultFontName.c_str());
	pkNewFont->AddReference();
	if (gs_pkDefaultFont)
		gs_pkDefaultFont->Release();
	gs_pkDefaultFont = pkNewFont;

	CResource* pkNewItalicFont = rkResMgr.GetResourcePointer(gs_strDefaultItalicFontName.c_str());
	pkNewItalicFont->AddReference();
	if (gs_pkDefaultItalicFont)
		gs_pkDefaultItalicFont->Release();
	gs_pkDefaultItalicFont = pkNewItalicFont;

	return true;
}

CResource* DefaultFont_GetResource()
{
	if (!gs_pkDefaultFont || gs_isReloadDefaultFont)
		ReloadDefaultFonts();
	return gs_pkDefaultFont;
}

CResource* DefaultItalicFont_GetResource()
{
	if (!gs_pkDefaultItalicFont || gs_isReloadDefaultFont)
		ReloadDefaultFonts();
	return gs_pkDefaultItalicFont;
}

// END_OF_DEFAULT_FONT

void SetGuildSymbolPath(const char* c_szPathName)
{
	g_strGuildSymbolPathName = "mark/";
	g_strGuildSymbolPathName += c_szPathName;
	g_strGuildSymbolPathName += "/";
}

const char* GetGuildSymbolFileName(DWORD dwGuildID)
{
	return _getf("%s%03d.jpg", g_strGuildSymbolPathName.c_str(), dwGuildID);
}

BYTE c_aSlotTypeToInvenType[SLOT_TYPE_MAX] =
{
	RESERVED_WINDOW,		// SLOT_TYPE_NONE
	INVENTORY,				// SLOT_TYPE_INVENTORY
	RESERVED_WINDOW,		// SLOT_TYPE_SKILL
	RESERVED_WINDOW,		// SLOT_TYPE_EMOTION
	RESERVED_WINDOW,		// SLOT_TYPE_SHOP
	RESERVED_WINDOW,		// SLOT_TYPE_EXCHANGE_OWNER
	RESERVED_WINDOW,		// SLOT_TYPE_EXCHANGE_TARGET
	RESERVED_WINDOW,		// SLOT_TYPE_QUICK_SLOT
	RESERVED_WINDOW,		// SLOT_TYPE_SAFEBOX <- SAFEBOX, MALL의 경우 하드 코딩되어있는 LEGACY 코드를 유지함.
#if defined(ENABLE_GUILDRENEWAL_SYSTEM)
	GUILDBANK,				// SLOT_TYPE_GUILDBANK
#endif
#if defined(ENABLE_ACCE_COSTUME_SYSTEM)
	SLOT_TYPE_ACCE,			// ACCEREFINE
#endif
	RESERVED_WINDOW,		// SLOT_TYPE_PRIVATE_SHOP
	RESERVED_WINDOW,		// SLOT_TYPE_MALL <- SAFEBOX, MALL의 경우 하드 코딩되어있는 LEGACY 코드를 유지함.
	DRAGON_SOUL_INVENTORY,	// SLOT_TYPE_DRAGON_SOUL_INVENTORY
#if defined(ENABLE_GROWTH_PET_SYSTEM)
	PET_FEED,				// SLOT_TYPE_PET_FEED_WINDOW
#endif
	EQUIPMENT,				// SLOT_TYPE_EQUIPMENT
	BELT_INVENTORY,			// SLOT_TYPE_BELT_INVENTORY
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
	RESERVED_WINDOW,		// SLOT_TYPE_AUTO
#endif
#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
	CHANGELOOK,				// SLOT_TYPE_CHANGE_LOOK
#endif
#if defined(ENABLE_FISH_EVENT)
	RESERVED_WINDOW,		// SLOT_TYPE_FISH_EVENT
#endif
#if defined(ENABLE_AURA_SYSTEM)
	AURA_REFINE,			// SLOT_TYPE_AURA
#endif
#if defined(ENABLE_PREMIUM_PRIVATE_SYSTEM)
	PREMIUM_PRIVATE_SHOP,	// SLOT_TYPE_PREMIUM_PRIVATE_SHOP
#endif
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	INVENTORY,				// SLOT_TYPE_SKILL_BOOK_INVENTORY
	INVENTORY,				// SLOT_TYPE_MATERIAL_INVENTORY
	INVENTORY,				// SLOT_TYPE_STONE_INVENTORY
	INVENTORY,				// SLOT_TYPE_GIFT_BOX_INVENTORY
#endif
};

BYTE SlotTypeToInvenType(BYTE bSlotType)
{
	if (bSlotType >= SLOT_TYPE_MAX)
		return RESERVED_WINDOW;
	else
		return c_aSlotTypeToInvenType[bSlotType];
}

BYTE c_aWndTypeToSlotType[WINDOW_TYPE_MAX] =
{
	SLOT_TYPE_NONE,						// RESERVED_WINDOW
	SLOT_TYPE_INVENTORY,				// INVENTORY
	SLOT_TYPE_EQUIPMENT,				// EQUIPMENT
	SLOT_TYPE_SAFEBOX,					// SAFEBOX
	SLOT_TYPE_MALL,						// MALL
	SLOT_TYPE_DRAGON_SOUL_INVENTORY,	// DRAGON_SOUL_INVENTORY
	SLOT_TYPE_BELT_INVENTORY,			// BELT_INVENTORY
#if defined(ENABLE_GUILDRENEWAL_SYSTEM)
	SLOT_TYPE_GUILDBANK,				// GUILDBANK
#endif
#if defined(ENABLE_MAILBOX)
	SLOT_TYPE_MALL,						// MAIL
#endif
	SLOT_TYPE_NONE,						// NPC_STORAGE
#if defined(ENABLE_PREMIUM_PRIVATE_SHOP)
	SLOT_TYPE_PREMIUM_PRIVATE_SHOP,		// PREMIUM_PRIVATE_SHOP
#endif
#if defined(ENABLE_ACCE_COSTUME_SYSTEM)
	SLOT_TYPE_ACCE,						// ACCEREFINE
#endif
	SLOT_TYPE_NONE,						// GROUND
#if defined(ENABLE_GROWTH_PET_SYSTEM)
	SLOT_TYPE_PET_FEED_WINDOW,			// PET_FEED
#endif
#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
	SLOT_TYPE_CHANGE_LOOK,				// CHANGELOOK
#endif
#if defined(ENABLE_AURA_SYSTEM)
	SLOT_TYPE_AURA,						// AURA_REFINE
#endif
#if defined(ENABLE_CUBE_RENEWAL)
	SLOT_TYPE_NONE,						// CUBE_WINDOW
#endif
#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
	SLOT_TYPE_SKILL_BOOK_INVENTORY,		// INVENTORY
	SLOT_TYPE_UPGRADE_ITEMS_INVENTORY,	// INVENTORY
	SLOT_TYPE_STONE_INVENTORY,			// INVENTORY
	SLOT_TYPE_GIFT_BOX_INVENTORY,		// INVENTORY
#endif
};

BYTE WindowTypeToSlotType(BYTE bWindowType)
{
	if (bWindowType >= WINDOW_TYPE_MAX)
		return RESERVED_WINDOW;

	return c_aWndTypeToSlotType[bWindowType];
}

#ifdef ENABLE_DETAILS_UI
typedef struct SApplyInfo
{
	BYTE bPointType; // APPLY -> POINT
} TApplyInfo;

const TApplyInfo aApplyInfo[CItemData::MAX_APPLY_NUM] =
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
	{ POINT_ATTBONUS_WOLFMAN, },			// APPLY_ATTBONUS_WOLFMAN,			// 94 수인족에게 강함
	{ POINT_RESIST_WOLFMAN, },				// APPLY_RESIST_WOLFMAN,			// 95 수인족에게 저항
	{ POINT_RESIST_CLAW, },					// APPLY_RESIST_CLAW,				// 96 CLAW에게 저항

#if defined(ENABLE_MOUNT_COSTUME_SYSTEM)
	{ POINT_MOUNT, }, 						// APPLY_MOUNT,						// 97
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	{ POINT_ACCEDRAIN_RATE, }, 				// APPLY_ACCEDRAIN_RATE,			// 98
#endif

#ifdef ENABLE_MAGIC_REDUCTION
	{ POINT_RESIST_MAGIC_REDUCTION, },		// APPLY_RESIST_MAGIC_REDUCTION,	// 99
#endif

#if defined(ENABLE_ELEMENT_ADD)
	{ POINT_ENCHANT_ELEC, },				// APPLY_ENCHANT_ELEC,				// 100
	{ POINT_ENCHANT_FIRE, },				// APPLY_ENCHANT_FIRE,				// 101
	{ POINT_ENCHANT_ICE, }, 				// APPLY_ENCHANT_ICE,				// 102
	{ POINT_ENCHANT_WIND, }, 				// APPLY_ENCHANT_WIND,				// 103
	{ POINT_ENCHANT_EARTH, },				// APPLY_ENCHANT_EARTH,				// 104
	{ POINT_ENCHANT_DARK, },				// APPLY_ENCHANT_DARK,				// 105
	{ POINT_ATTBONUS_CZ, },					// APPLY_ATTBONUS_CZ,				// 106
	{ POINT_ATTBONUS_INSECT, },				// APPLY_ATTBONUS_INSECT,			// 107
	{ POINT_ATTBONUS_DESERT, },				// APPLY_ATTBONUS_DESERT,			// 108
	{ POINT_RESIST_SWORD_REDUCTION, },		// APPLY_RESIST_SWORD_REDUCTION,	// 109
	{ POINT_RESIST_TWOHAND_REDUCTION, },	// APPLY_RESIST_TWOHAND_REDUCTION,	// 110
	{ POINT_RESIST_DAGGER_REDUCTION, },		// APPLY_RESIST_DAGGER_REDUCTION,	// 111
	{ POINT_RESIST_BELL_REDUCTION, },		// APPLY_RESIST_BELL_REDUCTION,		// 112
	{ POINT_RESIST_FAN_REDUCTION, },		// APPLY_RESIST_FAN_REDUCTION,		// 113
	{ POINT_RESIST_BOW_REDUCTION, },		// APPLY_RESIST_BOW_REDUCTION,		// 114
	{ POINT_RESIST_CLAW_REDUCTION, },		// APPLY_RESIST_CLAW_REDUCTION,		// 115
	{ POINT_RESIST_HUMAN, },				// APPLY_RESIST_HUMAN,				// 116
#endif
	{ POINT_ATTBONUS_STONE, },				// APPLY_ATTBONUS_STONE,			// 117
#if defined(ENABLE_CONQUEROR_LEVEL)
	{ POINT_SUNGMA_STR, },					// APPLY_SUNGMA_STR,				// 118
	{ POINT_SUNGMA_HP, },					// APPLY_SUNGMA_HP,					// 119
	{ POINT_SUNGMA_MOVE, },					// APPLY_SUNGMA_MOVE,				// 120
	{ POINT_SUNGMA_IMMUNE, },				// APPLY_SUNGMA_IMMUNE,				// 121
#endif
#if defined(ENABLE_APPLY_RANDOM)
	{ POINT_NONE, },						// APPLY_RANDOM,					// 122
#endif
};

BYTE ApplyTypeToPointType(BYTE bApplyType)
{
	if (bApplyType >= CItemData::MAX_APPLY_NUM)
		return POINT_NONE;
	else
		return aApplyInfo[bApplyType].bPointType;
}
#endif
