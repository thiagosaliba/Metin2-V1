#ifndef __INC_COMMON_ITEM_LENGTH_H__
#define __INC_COMMON_ITEM_LENGTH_H__

enum EItemMisc
{
	ITEM_NAME_MAX_LEN = 30,
	ITEM_VALUES_MAX_NUM = 6,
	ITEM_SMALL_DESCR_MAX_LEN = 256,
	ITEM_LIMIT_MAX_NUM = 2,
#if defined(__ITEM_APPLY4__)
	ITEM_APPLY_MAX_NUM = 4,
#else
	ITEM_APPLY_MAX_NUM = 3,
#endif
#if defined(__ITEM_SOCKET5__)
	ITEM_SOCKET_MAX_NUM = 5,
#else
	ITEM_SOCKET_MAX_NUM = 3,
#endif
	ITEM_MAX_COUNT = 200, // MAX 32767
	ITEM_ATTRIBUTE_NORM_NUM = 5,
	ITEM_ATTRIBUTE_RARE_NUM = 2,

	ITEM_ATTRIBUTE_NORM_START = 0,
	ITEM_ATTRIBUTE_NORM_END = ITEM_ATTRIBUTE_NORM_START + ITEM_ATTRIBUTE_NORM_NUM,

	ITEM_ATTRIBUTE_RARE_START = ITEM_ATTRIBUTE_NORM_END,
	ITEM_ATTRIBUTE_RARE_END = ITEM_ATTRIBUTE_RARE_START + ITEM_ATTRIBUTE_RARE_NUM,

	ITEM_ATTRIBUTE_MAX_NUM = ITEM_ATTRIBUTE_RARE_END, // 7
	ITEM_ATTRIBUTE_MAX_LEVEL = 5,
	ITEM_AWARD_WHY_MAX_LEN = 50,

	REFINE_MATERIAL_MAX_NUM = 5,

	ITEM_ELK_VNUM = 50026,

#if defined(__EXTENDED_DSS_RECHARGE__) || defined(__EXTENDED_COSTUME_RECHARGE__)
	MIN_INFINITE_DURATION = 100 * 24 * 60 * 60, // 100d
	MAX_INFINITE_DURATION = 60 * 365 * 24 * 60 * 60, // 60y
#endif
};

const BYTE ITEM_SOCKET_REMAIN_SEC = 0;
enum EItemValueIdice
{
	ITEM_VALUE_DRAGON_SOUL_POLL_OUT_BONUS_IDX = 0,
	ITEM_VALUE_CHARGING_AMOUNT_IDX = 0,
	ITEM_VALUE_SECONDARY_COIN_UNIT_IDX = 0,
};
enum EItemDragonSoulSockets
{
	ITEM_SOCKET_DRAGON_SOUL_ACTIVE_IDX = 2,
	ITEM_SOCKET_CHARGING_AMOUNT_IDX = 2,
};

// 헐 이거 미친거 아니야?
// 나중에 소켓 확장하면 어쩌려고 이지랄 -_-;;;
enum EItemUniqueSockets
{
	ITEM_SOCKET_UNIQUE_SAVE_TIME = ITEM_SOCKET_MAX_NUM - 2,
	ITEM_SOCKET_UNIQUE_REMAIN_TIME = ITEM_SOCKET_MAX_NUM - 1
};

enum EItemTypes
{
	ITEM_NONE,				// 0
	ITEM_WEAPON,			// 1 // 무기
	ITEM_ARMOR,				// 2 // 갑옷
	ITEM_USE,				// 3 // 아이템 사용
	ITEM_AUTOUSE,			// 4
	ITEM_MATERIAL,			// 5
	ITEM_SPECIAL,			// 6 // 스페셜 아이템
	ITEM_TOOL,				// 7
	ITEM_LOTTERY,			// 8 // 복권
	ITEM_ELK,				// 9 // 돈
	ITEM_METIN,				// 10
	ITEM_CONTAINER,			// 11
	ITEM_FISH,				// 12 // 낚시
	ITEM_ROD,				// 13
	ITEM_RESOURCE,			// 14
	ITEM_CAMPFIRE,			// 15
	ITEM_UNIQUE,			// 16
	ITEM_SKILLBOOK,			// 17
	ITEM_QUEST,				// 18
	ITEM_POLYMORPH,			// 19
	ITEM_TREASURE_BOX,		// 20 // 보물상자
	ITEM_TREASURE_KEY,		// 21 // 보물상자 열쇠
	ITEM_SKILLFORGET,		// 22
	ITEM_GIFTBOX,			// 23
	ITEM_PICK,				// 24
	ITEM_HAIR,				// 25 // 머리
	ITEM_TOTEM,				// 26 // 토템
	ITEM_BLEND,				// 27 // 생성될때 랜덤하게 속성이 붙는 약물
	ITEM_COSTUME,			// 28 // 코스츔 아이템 (2011년 8월 추가된 코스츔 시스템용 아이템)
	ITEM_DS,				// 29 // 용혼석
	ITEM_SPECIAL_DS,		// 30 // 특수한 용혼석 (DS_SLOT에 착용하는 UNIQUE 아이템이라 생각하면 됨)
	ITEM_EXTRACT,			// 31 추출도구.
	ITEM_SECONDARY_COIN,	// 32 ?? 명도전??
	ITEM_RING,				// 33 반지
	ITEM_BELT,				// 34 벨트
#if defined(__CHEQUE_SYSTEM__)
	ITEM_CHEQUE,			// 35 won
#endif
#if defined(__GACHA_SYSTEM__)
	ITEM_GACHA,				// 36 gacha
#endif
	ITEM_PET,				// 37 gacha
#if defined(__SOUL_SYSTEM__)
	ITEM_SOUL,				// 38 soul
#endif
	ITEM_MEDIUM,			// 39 medium
	ITEM_PASSIVE,			// 40 passive
	ITEM_MERCENARY			// 41 mercenary
};

enum EMetinSubTypes
{
	METIN_NORMAL,
	METIN_GOLD,
	METIN_SUNGMA,
};

enum EWeaponSubTypes
{
	WEAPON_SWORD,		// 0
	WEAPON_DAGGER,		// 1
	WEAPON_BOW,			// 2
	WEAPON_TWO_HANDED,	// 3
	WEAPON_BELL,		// 4
	WEAPON_FAN,			// 5
	WEAPON_ARROW,		// 6
	WEAPON_MOUNT_SPEAR,	// 7
	WEAPON_CLAW,		// 8
#if defined(__QUIVER_SYSTEM__)
	WEAPON_QUIVER,		// 9
#endif
	WEAPON_NUM_TYPES,
};

enum EArmorSubTypes
{
	ARMOR_BODY,
	ARMOR_HEAD,
	ARMOR_SHIELD,
	ARMOR_WRIST,
	ARMOR_FOOTS,
	ARMOR_NECK,
	ARMOR_EAR,
#if defined(__PENDANT_SYSTEM__)
	ARMOR_PENDANT,
#endif
#if defined(__GLOVE_SYSTEM__)
	ARMOR_GLOVE,
#endif
	ARMOR_NUM_TYPES
};

enum ECostumeSubTypes
{
	COSTUME_BODY = ARMOR_BODY, // [중요!!] ECostumeSubTypes enum value는 종류별로 EArmorSubTypes의 그것과 같아야 함.
	COSTUME_HAIR = ARMOR_HEAD, // 이는 코스츔 아이템에 추가 속성을 붙이겠다는 사업부의 요청에 따라서 기존 로직을 활용하기 위함임.
#if defined(__MOUNT_COSTUME_SYSTEM__)
	COSTUME_MOUNT,
#endif
#if defined(__ACCE_COSTUME_SYSTEM__)
	COSTUME_ACCE,
#endif
#if defined(__WEAPON_COSTUME_SYSTEM__)
	COSTUME_WEAPON,
#endif
	COSTUME_NUM_TYPES,
};

enum EDragonSoulSubType
{
	DS_SLOT1,
	DS_SLOT2,
	DS_SLOT3,
	DS_SLOT4,
	DS_SLOT5,
	DS_SLOT6,
	DS_SLOT_MAX,
};

enum EDragonSoulGradeTypes
{
	DRAGON_SOUL_GRADE_NORMAL,
	DRAGON_SOUL_GRADE_BRILLIANT,
	DRAGON_SOUL_GRADE_RARE,
	DRAGON_SOUL_GRADE_ANCIENT,
	DRAGON_SOUL_GRADE_LEGENDARY,
#if defined(__DS_GRADE_MYTH__)
	DRAGON_SOUL_GRADE_MYTH,
#endif
	DRAGON_SOUL_GRADE_MAX,
};

enum EDragonSoulStepTypes
{
	DRAGON_SOUL_STEP_LOWEST,
	DRAGON_SOUL_STEP_LOW,
	DRAGON_SOUL_STEP_MID,
	DRAGON_SOUL_STEP_HIGH,
	DRAGON_SOUL_STEP_HIGHEST,
	DRAGON_SOUL_STEP_MAX,
};

#if defined(__DS_CHANGE_ATTR__)
enum EDragonSoulChangeAttrMaterialCount
{
	DRAGON_SOUL_CHANGE_ATTR_STEP_LOWEST = 1,
	DRAGON_SOUL_CHANGE_ATTR_STEP_LOW = 3,
	DRAGON_SOUL_CHANGE_ATTR_STEP_MID = 9,
	DRAGON_SOUL_CHANGE_ATTR_STEP_HIGH = 27,
	DRAGON_SOUL_CHANGE_ATTR_STEP_HIGHEST = 81,
};
#endif

#define DRAGON_SOUL_STRENGTH_MAX 7

enum EDSInventoryMaxNum
{
	DRAGON_SOUL_INVENTORY_MAX_NUM = DS_SLOT_MAX * DRAGON_SOUL_GRADE_MAX * DRAGON_SOUL_BOX_SIZE,
};

enum EFishSubTypes
{
	FISH_ALIVE,
	FISH_DEAD,
};

enum EResourceSubTypes
{
	RESOURCE_FISHBONE,
	RESOURCE_WATERSTONEPIECE,
	RESOURCE_WATERSTONE,
	RESOURCE_BLOOD_PEARL,
	RESOURCE_BLUE_PEARL,
	RESOURCE_WHITE_PEARL,
	RESOURCE_BUCKET,
	RESOURCE_CRYSTAL,
	RESOURCE_GEM,
	RESOURCE_STONE,
	RESOURCE_METIN,
	RESOURCE_ORE,
};

enum EUniqueSubTypes
{
	UNIQUE_NONE,
	UNIQUE_BOOK,
	UNIQUE_SPECIAL_RIDE,
	UNIQUE_SPECIAL_MOUNT_RIDE,
};

enum EUseSubTypes
{
	USE_POTION,						// 0
	USE_TALISMAN,
	USE_TUNING,
	USE_MOVE,
	USE_TREASURE_BOX,
	USE_MONEYBAG,
	USE_BAIT,
	USE_ABILITY_UP,
	USE_AFFECT,
	USE_CREATE_STONE,
	USE_SPECIAL,					// 10
	USE_POTION_NODELAY,
	USE_CLEAR,
	USE_INVISIBILITY,
	USE_DETACHMENT,
	USE_BUCKET,
	USE_POTION_CONTINUE,
	USE_CLEAN_SOCKET,
	USE_CHANGE_ATTRIBUTE,
	USE_ADD_ATTRIBUTE,
	USE_ADD_ACCESSORY_SOCKET,		// 20
	USE_PUT_INTO_ACCESSORY_SOCKET,
	USE_ADD_ATTRIBUTE2,
	USE_RECIPE,
	USE_CHANGE_ATTRIBUTE2,
	USE_BIND,
	USE_UNBIND,
	USE_TIME_CHARGE_PER,
	USE_TIME_CHARGE_FIX,			// 28
	USE_PUT_INTO_BELT_SOCKET,		// 29 벨트 소켓에 사용할 수 있는 아이템 
	USE_PUT_INTO_RING_SOCKET,		// 30 반지 소켓에 사용할 수 있는 아이템 (유니크 반지 말고, 새로 추가된 반지 슬롯)
#if defined(__COSTUME_ATTR_SYSTEM__)
	USE_CHANGE_COSTUME_ATTR,		// 31
	USE_RESET_COSTUME_ATTR,			// 32
#endif
	USE_CALL,						// 33 (Bravery Cape)
	USE_REMOVE_AFFECT,				// 34 (Remove Affect)
	USE_SELECT_ATTRIBUTE,			// 35
#if defined(__EXPRESSING_EMOTIONS__)
	USE_EMOTION_PACK,				// 36 (Emotion Pack)
#endif
	USE_FLOWER,						// 37 (Flower Power Event)
	USE_UNK39,						// 38
	USE_UNK40,						// 39
	USE_ELEMENT_UPGRADE,			// 40
	USE_ELEMENT_DOWNGRADE,			// 41
	USE_ELEMENT_CHANGE,				// 42
	USE_POTION_TOWER,				// 43 (SungMa Tower)
	USE_POTION_NODELAY_TOWER,		// 44 (SungMa Tower)
	USE_EMOTION_TOWER,				// 45 (SungMa Tower)
	USE_SECRET_DUNGEON_SCROLL		// 46
};

enum EPetSubTypes
{
	PET_PAY,
	PET_EGG,
	PET_UPBRINGING,
	PET_FEEDSTUFF,
	PET_BAG,
	PET_NAME_CHANGE,
	PET_EXPFOOD,
	PET_EXPFOOD_PER,
	PET_PRIMIUM_FEEDSTUFF,
	PET_SKILL,
	PET_SKILL_ALL_DEL_BOOK,
	PET_SKILL_DEL_BOOK,
	PET_ATTR_DETERMINE,
	PET_ATTR_CHANGE,
};

#if defined(__SOUL_SYSTEM__)
enum ESoulSubType
{
	RED_SOUL,
	BLUE_SOUL
};

enum ESoulGrade
{
	BASIC_SOUL = 1,
	GLEAMING_SOUL = 2,
	LUSTROUS_SOUL = 3,
	PRISMATIC_SOUL = 4,
	ILUMINED_SOUL = 5,
};

enum ESoulRefine
{
	SOUL_REFINE_SET = 999,
	SOUL_REFINE_EVOLVE_PROB = 100,
	SOUL_REFINE_AWAKE_PROB = 60,
	SOUL_REFINE_COST = 0,
};
#endif

enum EExtractSubTypes
{
	EXTRACT_DRAGON_SOUL,
	EXTRACT_DRAGON_HEART,
};

enum EAutoUseSubTypes
{
	AUTOUSE_POTION,
	AUTOUSE_ABILITY_UP,
	AUTOUSE_BOMB,
	AUTOUSE_GOLD,
	AUTOUSE_MONEYBAG,
	AUTOUSE_TREASURE_BOX
};

enum EMaterialSubTypes
{
	MATERIAL_LEATHER,
	MATERIAL_BLOOD,
	MATERIAL_ROOT,
	MATERIAL_NEEDLE,
	MATERIAL_JEWEL,
	MATERIAL_DS_REFINE_NORMAL,
	MATERIAL_DS_REFINE_BLESSED,
	MATERIAL_DS_REFINE_HOLLY,
#if defined(__DS_CHANGE_ATTR__)
	MATERIAL_DS_CHANGE_ATTR,
#endif
	MATERIAL_PASSIVE_WEAPON,
	MATERIAL_PASSIVE_ARMOR,
	MATERIAL_PASSIVE_ACCE,
	MATERIAL_PASSIVE_ELEMENT,
};

enum ESpecialSubTypes
{
	SPECIAL_MAP,
	SPECIAL_KEY,
	SPECIAL_DOC,
	SPECIAL_SPIRIT,
};

enum EToolSubTypes
{
	TOOL_FISHING_ROD
};

enum ELotterySubTypes
{
	LOTTERY_TICKET,
	LOTTERY_INSTANT
};

enum EItemFlag
{
	ITEM_FLAG_REFINEABLE = (1 << 0),
	ITEM_FLAG_SAVE = (1 << 1),
	ITEM_FLAG_STACKABLE = (1 << 2), // 여러개 합칠 수 있음
	ITEM_FLAG_COUNT_PER_1GOLD = (1 << 3),
	ITEM_FLAG_SLOW_QUERY = (1 << 4),
	ITEM_FLAG_RARE = (1 << 5),
	ITEM_FLAG_UNIQUE = (1 << 6),
	ITEM_FLAG_MAKECOUNT = (1 << 7),
	ITEM_FLAG_IRREMOVABLE = (1 << 8),
	ITEM_FLAG_CONFIRM_WHEN_USE = (1 << 9),
	ITEM_FLAG_QUEST_USE = (1 << 10),
	ITEM_FLAG_QUEST_USE_MULTIPLE = (1 << 11),
	ITEM_FLAG_QUEST_GIVE = (1 << 12),
	ITEM_FLAG_LOG = (1 << 13),
	ITEM_FLAG_APPLICABLE = (1 << 14),
};

enum EItemAntiFlag
{
	ITEM_ANTIFLAG_FEMALE = (1 << 0), // 여성 사용 불가
	ITEM_ANTIFLAG_MALE = (1 << 1), // 남성 사용 불가
	ITEM_ANTIFLAG_WARRIOR = (1 << 2), // 무사 사용 불가
	ITEM_ANTIFLAG_ASSASSIN = (1 << 3), // 자객 사용 불가
	ITEM_ANTIFLAG_SURA = (1 << 4), // 수라 사용 불가 
	ITEM_ANTIFLAG_SHAMAN = (1 << 5), // 무당 사용 불가
	ITEM_ANTIFLAG_GET = (1 << 6), // 집을 수 없음
	ITEM_ANTIFLAG_DROP = (1 << 7), // 버릴 수 없음
	ITEM_ANTIFLAG_SELL = (1 << 8), // 팔 수 없음
	ITEM_ANTIFLAG_EMPIRE_A = (1 << 9), // A 제국 사용 불가
	ITEM_ANTIFLAG_EMPIRE_B = (1 << 10), // B 제국 사용 불가
	ITEM_ANTIFLAG_EMPIRE_C = (1 << 11), // C 제국 사용 불가
	ITEM_ANTIFLAG_SAVE = (1 << 12), // 저장되지 않음
	ITEM_ANTIFLAG_GIVE = (1 << 13), // 거래 불가
	ITEM_ANTIFLAG_PKDROP = (1 << 14), // PK시 떨어지지 않음
	ITEM_ANTIFLAG_STACK = (1 << 15), // 합칠 수 없음
	ITEM_ANTIFLAG_MYSHOP = (1 << 16), // 개인 상점에 올릴 수 없음
	ITEM_ANTIFLAG_SAFEBOX = (1 << 17), // 창고에 넣을 수 없음
	ITEM_ANTIFLAG_WOLFMAN = (1 << 18),
	ITEM_ANTIFLAG_RT_REMOVE = (1 << 19),
	ITEM_ANTIFLAG_QUICKSLOT = (1 << 20),
#if defined(__CHANGE_LOOK_SYSTEM__)
	ITEM_ANTIFLAG_CHANGELOOK = (1 << 21),
#endif
	ITEM_ANTIFLAG_REINFORCE = (1 << 22),
	ITEM_ANTIFLAG_ENCHANT = (1 << 23),
	ITEM_ANTIFLAG_ENERGY = (1 << 24),
	ITEM_ANTIFLAG_PETFEED = (1 << 25),
	ITEM_ANTIFLAG_APPLY = (1 << 26),
#if defined(__ACCE_COSTUME_SYSTEM__)
	ITEM_ANTIFLAG_ACCE = (1 << 27),
#endif
	ITEM_ANTIFLAG_MAIL = (1 << 28),
	ITEM_ANTIFLAG_DESTROY = (1 << 29),
};

enum EItemWearableFlag
{
	WEARABLE_BODY = (1 << 0),
	WEARABLE_HEAD = (1 << 1),
	WEARABLE_FOOTS = (1 << 2),
	WEARABLE_WRIST = (1 << 3),
	WEARABLE_WEAPON = (1 << 4),
	WEARABLE_NECK = (1 << 5),
	WEARABLE_EAR = (1 << 6),
	WEARABLE_UNIQUE = (1 << 7),
	WEARABLE_SHIELD = (1 << 8),
	WEARABLE_ARROW = (1 << 9),
	WEARABLE_HAIR = (1 << 10),
	WEARABLE_ABILITY = (1 << 11),
	WEARABLE_COSTUME_BODY = (1 << 12),
	WEARABLE_COSTUME_HAIR = (1 << 13),
#if defined(__MOUNT_COSTUME_SYSTEM__)
	WEARABLE_COSTUME_MOUNT = (1 << 14),
#endif
#if defined(__ACCE_COSTUME_SYSTEM__)
	WEARABLE_COSTUME_ACCE = (1 << 15),
#endif
#if defined(__WEAPON_COSTUME_SYSTEM__)
	WEARABLE_COSTUME_WEAPON = (1 << 16),
#endif
#if defined(__PENDANT_SYSTEM__)
	WEARABLE_PENDANT = (1 << 17),
#endif
#if defined(__GLOVE_SYSTEM__)
	WEARABLE_GLOVE = (1 << 18),
#endif
};

enum ELimitTypes
{
	LIMIT_NONE,

	LIMIT_LEVEL,
	LIMIT_STR,
	LIMIT_DEX,
	LIMIT_INT,
	LIMIT_CON,
	LIMIT_PCBANG,

	/// 착용 여부와 상관 없이 실시간으로 시간 차감 (socket0에 소멸 시간이 박힘: unix_timestamp 타입)
	LIMIT_REAL_TIME,

	/// 아이템을 맨 처음 사용(혹은 착용) 한 순간부터 리얼타임 타이머 시작 
	/// 최초 사용 전에는 socket0에 사용가능시간(초단위, 0이면 프로토의 limit value값 사용) 값이 쓰여있다가 
	/// 아이템 사용시 socket1에 사용 횟수가 박히고 socket0에 unix_timestamp 타입의 소멸시간이 박힘.
	LIMIT_REAL_TIME_START_FIRST_USE,

	/// 아이템을 착용 중일 때만 사용 시간이 차감되는 아이템
	/// socket0에 남은 시간이 초단위로 박힘. (아이템 최초 사용시 해당 값이 0이면 프로토의 limit value값을 socket0에 복사)
	LIMIT_TIMER_BASED_ON_WEAR,

	/// SungMa Update (New World)
	LIMIT_NEWWORLD_LEVEL,

	LIMIT_MAX_NUM
};

enum EAttrAddonTypes
{
	ATTR_ADDON_NONE,
	// positive values are reserved for set
	ATTR_DAMAGE_ADDON = -1,
};

enum ERefineType
{
	REFINE_TYPE_NORMAL,
	REFINE_TYPE_NOT_USED1,
	REFINE_TYPE_SCROLL,
	REFINE_TYPE_HYUNIRON,
	REFINE_TYPE_MONEY_ONLY,
	REFINE_TYPE_MUSIN,
	REFINE_TYPE_BDRAGON,
#if defined(__STONE_OF_BLESS__)
	REFINE_TYPE_STONE_OF_BLESS,
#endif
#if defined(__SOUL_SYSTEM__)
	REFINE_TYPE_SOUL_EVOLVE,
	REFINE_TYPE_SOUL_AWAKE,
#endif
};

#if defined(__ACCE_COSTUME_SYSTEM__)
enum EAcceInfo
{
	ACCE_GRADE_VALUE_FIELD = 0,
	ACCE_ABSORPTION_SOCKET = 0,
	ACCE_ABSORBED_SOCKET = 1,
	ACCE_GRADE_1_ABS = 1,
	ACCE_GRADE_2_ABS = 5,
	ACCE_GRADE_3_ABS = 10,
	ACCE_GRADE_4_ABS_MIN = 11,
	ACCE_GRADE_4_ABS_MAX = 25,
	ACCE_GRADE_4_ABS_MAX_COMB = 19,
	ACCE_GRADE_4_ABS_RANGE = 5,
	ACCE_EFFECT_FROM_ABS = 19,
	ACCE_WINDOW_MAX_MATERIALS = 2,
	ACCE_GRADE_1_PRICE = 100000,
	ACCE_GRADE_2_PRICE = 200000,
	ACCE_GRADE_3_PRICE = 300000,
	ACCE_GRADE_4_PRICE = 500000,
	ACCE_COMBINE_GRADE_1 = 80,
	ACCE_COMBINE_GRADE_2 = 70,
	ACCE_COMBINE_GRADE_3 = 50,
	ACCE_COMBINE_GRADE_4 = 30,
};
#endif

#endif // __INC_COMMON_ITEM_LENGTH_H__
