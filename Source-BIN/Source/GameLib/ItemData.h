#pragma once

// NOTE : Item의 통합 관리 클래스다.
// Icon, Model (droped on ground), Game Data

#include "../EterLib/GrpSubImage.h"
#include "../EterGrnLib/Thing.h"
#include "../UserInterface/Locale_inc.h"

class CItemData
{
public:
	enum
	{
		ITEM_NAME_MAX_LEN = 30,
		ITEM_LIMIT_MAX_NUM = 2,
		ITEM_VALUES_MAX_NUM = 6,
		ITEM_SMALL_DESCR_MAX_LEN = 256,
#if defined(ENABLE_ITEM_APPLY4)
		ITEM_APPLY_MAX_NUM = 4,
#else
		ITEM_APPLY_MAX_NUM = 3,
#endif
		ITEM_SOCKET_MAX_NUM = 3,
	};

	enum EItemType
	{
		ITEM_TYPE_NONE,					// 0 ~ 1 무기
		ITEM_TYPE_WEAPON,				// 1 ~ 2 갑옷
		ITEM_TYPE_ARMOR,				// 2 ~ 3 아이템 사용
		ITEM_TYPE_USE,					// 3 ~ 4
		ITEM_TYPE_AUTOUSE,				// 4 ~ 5
		ITEM_TYPE_MATERIAL,				// 5 ~ 6 스페셜 아이템
		ITEM_TYPE_SPECIAL,				// 6 ~ 7
		ITEM_TYPE_TOOL,					// 7 ~ 8 복권
		ITEM_TYPE_LOTTERY,				// 8 ~ 9 돈
		ITEM_TYPE_ELK,					// 9 ~ 10
		ITEM_TYPE_METIN,				// 10 ~ 11
		ITEM_TYPE_CONTAINER,			// 11 ~ 12 낚시
		ITEM_TYPE_FISH,					// 12 ~ 13
		ITEM_TYPE_ROD,					// 13 ~ 14
		ITEM_TYPE_RESOURCE,				// 14 ~ 15
		ITEM_TYPE_CAMPFIRE,				// 15 ~ 16
		ITEM_TYPE_UNIQUE,				// 16 ~ 17
		ITEM_TYPE_SKILLBOOK,			// 17 ~ 18
		ITEM_TYPE_QUEST,				// 18 ~ 19
		ITEM_TYPE_POLYMORPH,			// 19 ~ 20 보물상자
		ITEM_TYPE_TREASURE_BOX,			// 20 ~ 21 보물상자 열쇠
		ITEM_TYPE_TREASURE_KEY,			// 21 ~ 22
		ITEM_TYPE_SKILLFORGET,			// 22 ~ 23
		ITEM_TYPE_GIFTBOX,				// 23 ~ 24
		ITEM_TYPE_PICK,					// 24 ~ 25 머리
		ITEM_TYPE_HAIR,					// 25 ~ 26 토템
		ITEM_TYPE_TOTEM,				// 26 ~ 27 생성될때 랜덤하게 속성이 붙는 약물
		ITEM_TYPE_BLEND,				// 27 ~ 28 코스츔 아이템 (2011년 8월 추가된 코스츔 시스템용 아이템)
		ITEM_TYPE_COSTUME,				// 28 ~ 29 용혼석
		ITEM_TYPE_DS,					// 29 ~ 30 특수한 용혼석 (DS_SLOT에 착용하는 UNIQUE 아이템이라 생각하면 됨)
		ITEM_TYPE_SPECIAL_DS,			// 30 ~ 31 추출도구.
		ITEM_TYPE_EXTRACT,				// 31 ~ 32 ?? 명도전??
		ITEM_TYPE_SECONDARY_COIN,		// 32 ~ 33 반지
		ITEM_TYPE_RING,					// 33 ~ 34
		ITEM_TYPE_BELT,					// 34 ~ 35 벨트
#if defined(ENABLE_CHEQUE_SYSTEM)
		ITEM_TYPE_CHEQUE,				// 35 ~ 36 won
#endif
#ifdef ENABLE_GACHA
		ITEM_TYPE_GACHA,				// 36 ~ 37 gacha
#endif
		ITEM_TYPE_PET,					// 37 ~ 38 pet
#ifdef ENABLE_SOUL_SYSTEM
		ITEM_TYPE_SOUL,					// 38 ~ 39 soul
#endif
		ITEM_TYPE_MEDIUM,				// 39 ~ 40 medium
		ITEM_TYPE_PASSIVE,				// 40 ~ 41 passive
		ITEM_TYPE_MERCENARY,			// 41 ~ 42 mercenary

		ITEM_TYPE_MAX_NUM,
	};

	enum EWeaponSubTypes
	{
		WEAPON_SWORD,
		WEAPON_DAGGER, // 이도류
		WEAPON_BOW,
		WEAPON_TWO_HANDED,
		WEAPON_BELL,
		WEAPON_FAN,
		WEAPON_ARROW,
		WEAPON_MOUNT_SPEAR,
		WEAPON_CLAW,
#ifdef ENABLE_QUIVER_SYSTEM
		WEAPON_QUIVER,
#endif
		WEAPON_BOUQUET,

		WEAPON_NUM_TYPES,

		WEAPON_NONE = WEAPON_NUM_TYPES + 1,
	};

	enum EExtractSubTypes
	{
		EXTRACT_DRAGON_SOUL,
		EXTRACT_DRAGON_HEART,
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
#if defined(ENABLE_DS_CHANGE_ATTR)
		MATERIAL_DS_CHANGE_ATTR,
#endif
		MATERIAL_PASSIVE_WEAPON,
		MATERIAL_PASSIVE_ARMOR,
		MATERIAL_PASSIVE_ACCE,
		MATERIAL_PASSIVE_ELEMENT,
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
#if defined(ENABLE_PENDANT)
		ARMOR_PENDANT,
#endif
#if defined(ENABLE_GLOVE_SYSTEM)
		ARMOR_GLOVE,
#endif
		ARMOR_NUM_TYPES
	};

	enum ECostumeSubTypes
	{
		COSTUME_BODY, // 0 갑옷(main look)
		COSTUME_HAIR, // 1 헤어(탈착가능)
#if defined(ENABLE_MOUNT_COSTUME_SYSTEM)
		COSTUME_MOUNT,
#endif
#if defined(ENABLE_ACCE_COSTUME_SYSTEM)
		COSTUME_ACCE,
#endif
#if defined(ENABLE_WEAPON_COSTUME_SYSTEM)
		COSTUME_WEAPON,
#endif
		COSTUME_NUM_TYPES,
	};

#if defined(ENABLE_ACCE_COSTUME_SYSTEM)
	enum EAcceMisc
	{
		ACCE_GRADE_MAX_NUM = 5,
	};
#endif

#if defined(ENABLE_AURA_COSTUME_SYSTEM)
	enum EAuraMisc
	{
		AURA_GRADE_MAX_NUM = 6,
	};
#endif

	enum EUseSubTypes
	{
		USE_POTION,						// 0
		USE_TALISMAN,					// 1
		USE_TUNING,						// 2
		USE_MOVE,						// 3
		USE_TREASURE_BOX,				// 4
		USE_MONEYBAG,					// 5
		USE_BAIT,						// 6
		USE_ABILITY_UP,					// 7
		USE_AFFECT,						// 8
		USE_CREATE_STONE,				// 9
		USE_SPECIAL,					// 10
		USE_POTION_NODELAY,				// 11
		USE_CLEAR,						// 12
		USE_INVISIBILITY,				// 13
		USE_DETACHMENT,					// 14
		USE_BUCKET,						// 15
		USE_POTION_CONTINUE,			// 16
		USE_CLEAN_SOCKET,				// 17
		USE_CHANGE_ATTRIBUTE,			// 18
		USE_ADD_ATTRIBUTE,				// 19
		USE_ADD_ACCESSORY_SOCKET,		// 20
		USE_PUT_INTO_ACCESSORY_SOCKET,	// 21
		USE_ADD_ATTRIBUTE2,				// 22
		USE_RECIPE,						// 23
		USE_CHANGE_ATTRIBUTE2,			// 24
		USE_BIND,						// 25
		USE_UNBIND,						// 26
		USE_TIME_CHARGE_PER,			// 27
		USE_TIME_CHARGE_FIX,			// 28
		USE_PUT_INTO_BELT_SOCKET,		// 29 벨트 소켓에 사용할 수 있는 아이템 
		USE_PUT_INTO_RING_SOCKET,		// 30 반지 소켓에 사용할 수 있는 아이템 (유니크 반지 말고, 새로 추가된 반지 슬롯)
#ifdef ENABLE_COSTUME_ATTR_SYSTEM
		USE_CHANGE_COSTUME_ATTR,		// 31
		USE_RESET_COSTUME_ATTR,			// 32
#endif
		USE_CALL,						// 33 (Bravery Cape)
		USE_REMOVE_AFFECT,				// 34 (Remove Affect)
		USE_SELECT_ATTRIBUTE,			// 35
#ifdef ENABLE_EXPRESSING_EMOTION
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

#ifdef ENABLE_SOUL_BIND_SYSTEM
	enum ESealDate
	{
		SEAL_DATE_DEFAULT_TIMESTAMP = 0,
		SEAL_DATE_UNLIMITED_TIMESTAMP = -1,
	};

	enum ESealItem
	{
		SEAL_ITEM_BINDING_VNUM = 50263,
		SEAL_ITEM_UNBINDING_VNUM = 50264,
	};
#endif

#if defined(ENABLE_DSS_EXTENDED_RECHARGE) || defined(ENABLE_EXTENDED_BLEND_AFFECT) || defined(ENABLE_COSTUME_EXTENDED_RECHARGE)
	enum EDSSDuration
	{
		MIN_INFINITE_DURATION = 100 * 24 * 60 * 60,
		MAX_INFINITE_DURATION = 60 * 365 * 24 * 60 * 60,
	};
#endif

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

	enum EQuestType
	{
		QUEST_NONE,
		QUEST_WARP
	};

#ifdef ENABLE_SOUL_SYSTEM
	enum ESoulSubType
	{
		RED_SOUL,
		BLUE_SOUL,
	};
#endif

	enum EMediumSubType
	{
		MEDIUM_MOVE_COSTUME_ATTR,
		MEDIUM_MOVE_ACCE_ATTR,
	};

	enum EMercenarySubType
	{
		MERCENARY_0,
		MERCENARY_1,
		MERCENARY_2,
		MERCENARY_3,
		MERCENARY_4,
		MERCENARY_5,
		MERCENARY_6
	};

#ifdef ENABLE_67TH_BONUS
	enum ERareAttribute
	{
		ADD_RARE_ATTRIBUTE_VNUM = 71051,
		CHANGE_RARE_ATTRIBUTE_VNUM = 71052,
	};
#endif

	enum EDragonSoulSubType
	{
		DS_SLOT1,
		DS_SLOT2,
		DS_SLOT3,
		DS_SLOT4,
		DS_SLOT5,
		DS_SLOT6,
		DS_SLOT_NUM_TYPES = 6,
	};

	enum EMetinSubTypes
	{
		METIN_NORMAL,
		METIN_GOLD,
		METIN_SUNGMA
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

		// Duration
		LIMIT_DURATION,

		LIMIT_MAX_NUM
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
		ITEM_ANTIFLAG_SAFEBOX = (1 << 17),
		ITEM_ANTIFLAG_WOLFMAN = (1 << 18),
		ITEM_ANTIFLAG_RT_REMOVE = (1 << 19),
		ITEM_ANTIFLAG_QUICKSLOT = (1 << 20),
#ifdef ENABLE_CHANGE_LOOK_SYSTEM
		ITEM_ANTIFLAG_CHANGELOOK = (1 << 21),
#endif
		ITEM_ANTIFLAG_REINFORCE = (1 << 22),
		ITEM_ANTIFLAG_ENCHANT = (1 << 23),
		ITEM_ANTIFLAG_ENERGY = (1 << 24),
		ITEM_ANTIFLAG_PETFEED = (1 << 25),
		ITEM_ANTIFLAG_APPLY = (1 << 26),
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		ITEM_ANTIFLAG_ACCE = (1 << 27),
#endif
		ITEM_ANTIFLAG_MAIL = (1 << 28),
		ITEM_ANTIFLAG_DESTROY = (1 << 29),
	};

	enum EItemFlag
	{
		ITEM_FLAG_REFINEABLE = (1 << 0), // 개량 가능
		ITEM_FLAG_SAVE = (1 << 1),
		ITEM_FLAG_STACKABLE = (1 << 2), // 여러개 합칠 수 있음
		ITEM_FLAG_COUNT_PER_1GOLD = (1 << 3), // 가격이 개수 / 가격으로 변함
		ITEM_FLAG_SLOW_QUERY = (1 << 4), // 게임 종료시에만 SQL에 쿼리함
		ITEM_FLAG_RARE = (1 << 5),
		ITEM_FLAG_UNIQUE = (1 << 6),
		ITEM_FLAG_MAKECOUNT = (1 << 7),
		ITEM_FLAG_IRREMOVABLE = (1 << 8),
		ITEM_FLAG_CONFIRM_WHEN_USE = (1 << 9),
		ITEM_FLAG_QUEST_USE = (1 << 10), // 퀘스트 스크립트 돌리는지?
		ITEM_FLAG_QUEST_USE_MULTIPLE = (1 << 11), // 퀘스트 스크립트 돌리는지?
		ITEM_FLAG_UNUSED03 = (1 << 12), // UNUSED03
		ITEM_FLAG_LOG = (1 << 13), // 사용시 로그를 남기는 아이템인가?
		ITEM_FLAG_APPLICABLE = (1 << 14),
	};

	enum EWearPositions
	{
		WEAR_BODY,				// 0
		WEAR_HEAD,				// 1
		WEAR_FOOTS,				// 2
		WEAR_WRIST,				// 3
		WEAR_WEAPON,			// 4
		WEAR_NECK,				// 5
		WEAR_EAR,				// 6
		WEAR_UNIQUE1,			// 7
		WEAR_UNIQUE2,			// 8
		WEAR_ARROW,				// 9
		WEAR_SHIELD,			// 10
		WEAR_ABILITY1,			// 11
		WEAR_ABILITY2,			// 12
		WEAR_ABILITY3,			// 13
		WEAR_ABILITY4,			// 14
		WEAR_ABILITY5,			// 15
		WEAR_ABILITY6,			// 16
		WEAR_ABILITY7,			// 17
		WEAR_ABILITY8,			// 18
		WEAR_COSTUME_BODY,		// 19
		WEAR_COSTUME_HAIR,		// 20
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		WEAR_COSTUME_MOUNT,		// 21
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		WEAR_COSTUME_ACCE,		// 22
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		WEAR_COSTUME_WEAPON,	// 23
#endif
		WEAR_RING1,				// 24 \ 21
		WEAR_RING2,				// 25 \ 22
		WEAR_BELT,				// 26 \ 23
#ifdef ENABLE_PENDANT
		WEAR_PENDANT,			// 27
#endif
#if defined(ENABLE_GLOVE_SYSTEM)
		WEAR_GLOVE,				// 28
#endif

		WEAR_MAX_NUM,
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
#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
		WEARABLE_COSTUME_MOUNT = (1 << 14),
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		WEARABLE_COSTUME_ACCE = (1 << 15),
#endif
#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
		WEARABLE_COSTUME_WEAPON = (1 << 16),
#endif
#ifdef ENABLE_PENDANT
		WEARABLE_PENDANT = (1 << 17),
#endif
#if defined(ENABLE_GLOVE_SYSTEM)
		WEARABLE_GLOVE = (1 << 18),
#endif
	};

	enum EApplyTypes
	{
		APPLY_NONE,							// 0
		APPLY_MAX_HP,						// 1
		APPLY_MAX_SP,						// 2
		APPLY_CON,							// 3
		APPLY_INT,							// 4
		APPLY_STR,							// 5
		APPLY_DEX,							// 6
		APPLY_ATT_SPEED,					// 7
		APPLY_MOV_SPEED,					// 8
		APPLY_CAST_SPEED,					// 9
		APPLY_HP_REGEN,						// 10
		APPLY_SP_REGEN,						// 11
		APPLY_POISON_PCT,					// 12
		APPLY_STUN_PCT,						// 13
		APPLY_SLOW_PCT,						// 14
		APPLY_CRITICAL_PCT,					// 15
		APPLY_PENETRATE_PCT,				// 16
		APPLY_ATTBONUS_HUMAN,				// 17
		APPLY_ATTBONUS_ANIMAL,				// 18
		APPLY_ATTBONUS_ORC,					// 19
		APPLY_ATTBONUS_MILGYO,				// 20
		APPLY_ATTBONUS_UNDEAD,				// 21
		APPLY_ATTBONUS_DEVIL,				// 22
		APPLY_STEAL_HP,						// 23
		APPLY_STEAL_SP,						// 24
		APPLY_MANA_BURN_PCT,				// 25
		APPLY_DAMAGE_SP_RECOVER,			// 26
		APPLY_BLOCK,						// 27
		APPLY_DODGE,						// 28
		APPLY_RESIST_SWORD,					// 29
		APPLY_RESIST_TWOHAND,				// 30
		APPLY_RESIST_DAGGER,				// 31
		APPLY_RESIST_BELL,					// 32
		APPLY_RESIST_FAN,					// 33
		APPLY_RESIST_BOW,					// 34
		APPLY_RESIST_FIRE,					// 35
		APPLY_RESIST_ELEC,					// 36
		APPLY_RESIST_MAGIC,					// 37
		APPLY_RESIST_WIND,					// 38
		APPLY_REFLECT_MELEE,				// 39
		APPLY_REFLECT_CURSE,				// 40
		APPLY_POISON_REDUCE,				// 41
		APPLY_KILL_SP_RECOVER,				// 42
		APPLY_EXP_DOUBLE_BONUS,				// 43
		APPLY_GOLD_DOUBLE_BONUS,			// 44
		APPLY_ITEM_DROP_BONUS,				// 45
		APPLY_POTION_BONUS,					// 46
		APPLY_KILL_HP_RECOVER,				// 47
		APPLY_IMMUNE_STUN,					// 48
		APPLY_IMMUNE_SLOW,					// 49
		APPLY_IMMUNE_FALL,					// 50
		APPLY_SKILL,						// 51
		APPLY_BOW_DISTANCE,					// 52
		APPLY_ATT_GRADE_BONUS,				// 53
		APPLY_DEF_GRADE_BONUS,				// 54
		APPLY_MAGIC_ATT_GRADE,				// 55
		APPLY_MAGIC_DEF_GRADE,				// 56
		APPLY_CURSE_PCT,					// 57
		APPLY_MAX_STAMINA,					// 58
		APPLY_ATT_BONUS_TO_WARRIOR,			// 59
		APPLY_ATT_BONUS_TO_ASSASSIN,		// 60
		APPLY_ATT_BONUS_TO_SURA,			// 61
		APPLY_ATT_BONUS_TO_SHAMAN,			// 62
		APPLY_ATT_BONUS_TO_MONSTER,			// 63
		APPLY_MALL_ATTBONUS,				// 64 공격력 +x%
		APPLY_MALL_DEFBONUS,				// 65 방어력 +x%
		APPLY_MALL_EXPBONUS,				// 66 경험치 +x%
		APPLY_MALL_ITEMBONUS,				// 67 아이템 드롭율 x/10배
		APPLY_MALL_GOLDBONUS,				// 68 돈 드롭율 x/10배
		APPLY_MAX_HP_PCT,					// 69 최대 생명력 +x%
		APPLY_MAX_SP_PCT,					// 70 최대 정신력 +x%
		APPLY_SKILL_DAMAGE_BONUS,			// 71 스킬 데미지 * (100+x)%
		APPLY_NORMAL_HIT_DAMAGE_BONUS,		// 72 평타 데미지 * (100+x)%
		APPLY_SKILL_DEFEND_BONUS,			// 73 스킬 데미지 방어 * (100-x)%
		APPLY_NORMAL_HIT_DEFEND_BONUS,		// 74 평타 데미지 방어 * (100-x)%
		APPLY_EXTRACT_HP_PCT,				// 75
		APPLY_PC_BANG_EXP_BONUS,			// 76
		APPLY_PC_BANG_DROP_BONUS,			// 77
		APPLY_RESIST_WARRIOR,				// 78
		APPLY_RESIST_ASSASSIN,				// 79
		APPLY_RESIST_SURA,					// 80
		APPLY_RESIST_SHAMAN,				// 81
		APPLY_ENERGY,						// 82
		APPLY_DEF_GRADE,					// 83 방어력. DEF_GRADE_BONUS는 클라에서 두배로 보여지는 의도된 버그(...)가 있다.
		APPLY_COSTUME_ATTR_BONUS,			// 84 코스튬 아이템에 붙은 속성치 보너스
		APPLY_MAGIC_ATTBONUS_PER,			// 85 마법 공격력 +x%
		APPLY_MELEE_MAGIC_ATTBONUS_PER,		// 86 마법 + 밀리 공격력 +x%

		APPLY_RESIST_ICE,					// 87 냉기 저항
		APPLY_RESIST_EARTH,					// 88 대지 저항
		APPLY_RESIST_DARK,					// 89 어둠 저항

		APPLY_ANTI_CRITICAL_PCT,			// 90 크리티컬 저항
		APPLY_ANTI_PENETRATE_PCT,			// 91 관통타격 저항

		APPLY_BLEEDING_REDUCE = 92,			// 92
		APPLY_BLEEDING_PCT = 93,			// 93
		APPLY_ATT_BONUS_TO_WOLFMAN = 94,	// 94
		APPLY_RESIST_WOLFMAN = 95,			// 95
		APPLY_RESIST_CLAW = 96,				// 96

#if defined(ENABLE_MOUNT_COSTUME_SYSTEM)
		APPLY_MOUNT,						// 97
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		APPLY_ACCEDRAIN_RATE,				// 98
#endif

#ifdef ENABLE_MAGIC_REDUCTION
		APPLY_RESIST_MAGIC_REDUCTION,		// 99
#endif

#if defined(ENABLE_ELEMENT_ADD)
		APPLY_ENCHANT_ELECT,				// 100
		APPLY_ENCHANT_FIRE,					// 101
		APPLY_ENCHANT_ICE,					// 102
		APPLY_ENCHANT_WIND,					// 103
		APPLY_ENCHANT_EARTH,				// 104
		APPLY_ENCHANT_DARK,					// 105
		APPLY_ATTBONUS_CZ,					// 106
		APPLY_ATTBONUS_INSECT,				// 107
		APPLY_ATTBONUS_DESERT,				// 108

		APPLY_ATTBONUS_SWORD,				// 109
		APPLY_ATTBONUS_TWOHAND,				// 110
		APPLY_ATTBONUS_DAGGER,				// 111
		APPLY_ATTBONUS_BELL,				// 112
		APPLY_ATTBONUS_FAN,					// 113
		APPLY_ATTBONUS_BOW,					// 114
		APPLY_ATTBONUS_CLAW,				// 115

		APPLY_RESIST_HUMAN,					// 116
#endif
		APPLY_ATTBONUS_STONE,				// 117
#if defined(ENABLE_CONQUEROR_LEVEL)
		APPLY_SUNGMA_STR,					// 118
		APPLY_SUNGMA_HP,					// 119
		APPLY_SUNGMA_MOVE,					// 120
		APPLY_SUNGMA_IMMUNE,				// 121
#endif
#if defined(ENABLE_APPLY_RANDOM)
		APPLY_RANDOM,						// 122
#endif

		MAX_APPLY_NUM = 255,				//
	};

	enum EImmuneFlags
	{
		IMMUNE_PARA = (1 << 0),
		IMMUNE_CURSE = (1 << 1),
		IMMUNE_STUN = (1 << 2),
		IMMUNE_SLEEP = (1 << 3),
		IMMUNE_SLOW = (1 << 4),
		IMMUNE_POISON = (1 << 5),
		IMMUNE_TERROR = (1 << 6),
	};

#pragma pack(push)
#pragma pack(1)
	typedef struct SItemLimit
	{
		BYTE bType;
		long lValue;
	} TItemLimit;

	typedef struct SItemApply
	{
		BYTE bType;
		long lValue;
	} TItemApply;

	typedef struct SItemTable
	{
		DWORD dwVnum;
		DWORD dwVnumRange;
		char szName[ITEM_NAME_MAX_LEN + 1];
		char szLocaleName[ITEM_NAME_MAX_LEN + 1];
		BYTE bType;
		BYTE bSubType;

		BYTE bWeight;
		BYTE bSize;

		uint64_t ullAntiFlags;
		DWORD dwFlags;
		DWORD dwWearFlags;
		DWORD dwImmuneFlag;

		DWORD dwIBuyItemPrice;
		DWORD dwISellItemPrice;

		TItemLimit aLimits[ITEM_LIMIT_MAX_NUM];
		TItemApply aApplies[ITEM_APPLY_MAX_NUM];
		long alValues[ITEM_VALUES_MAX_NUM];
		long alSockets[ITEM_SOCKET_MAX_NUM];
		DWORD dwRefinedVnum;
		WORD wRefineSet;
		BYTE bAlterToMagicItemPct;
		BYTE bSpecular;
		BYTE bGainSocketPct;
	} TItemTable;

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	struct SScaleInfo
	{
		float fScaleX, fScaleY, fScaleZ;
		float fPositionX, fPositionY, fPositionZ;
	};

	typedef struct SScaleTable
	{
		SScaleInfo tInfo[10];
	} TScaleTable;
#endif

	/*
	typedef struct SItemTable
	{
		DWORD dwVnum;
		char szItemName[ITEM_NAME_MAX_LEN + 1];
		BYTE bType;
		BYTE bSubType;
		BYTE bSize;
		DWORD dwAntiFlags;
		DWORD dwFlags;
		DWORD dwWearFlags;
		DWORD dwIBuyItemPrice;
		DWORD dwISellItemPrice;
		TItemLimit aLimits[ITEM_LIMIT_MAX_NUM];
		TItemApply aApplies[ITEM_APPLY_MAX_NUM];
		long alValues[ITEM_VALUES_MAX_NUM];
		long alSockets[ITEM_SOCKET_MAX_NUM];
		DWORD dwRefinedVnum;
		BYTE bSpecular;
		DWORD dwIconNumber;
	} TItemTable;
	*/
#pragma pack(pop)

public:
	CItemData();
	virtual ~CItemData();

	void Clear();
	void SetSummary(const std::string& c_rstSumm);
	void SetDescription(const std::string& c_rstDesc);

	CGraphicThing* GetModelThing();
	CGraphicThing* GetSubModelThing();
	CGraphicThing* GetDropModelThing();
	CGraphicSubImage* GetIconImage();

	DWORD GetLODModelThingCount();
	BOOL GetLODModelThingPointer(DWORD dwIndex, CGraphicThing** ppModelThing);

	DWORD GetAttachingDataCount();
	BOOL GetCollisionDataPointer(DWORD dwIndex, const NRaceData::TAttachingData** c_ppAttachingData);
	BOOL GetAttachingDataPointer(DWORD dwIndex, const NRaceData::TAttachingData** c_ppAttachingData);

	/////
	const TItemTable* GetTable() const;
	DWORD GetIndex() const;
	const char* GetName() const;
	const char* GetDescription() const;
	const char* GetSummary() const;
	BYTE GetType() const;
	BYTE GetSubType() const;
	uint64_t GetAntiFlags() const;
	UINT GetRefine() const;
	const char* GetUseTypeString() const;
	DWORD GetWeaponType() const;
	BYTE GetSize() const;
	BOOL IsAntiFlag(uint64_t ullFlag) const;
	BOOL IsFlag(DWORD dwFlag) const;
	BOOL IsWearableFlag(DWORD dwFlag) const;
	BOOL HasNextGrade() const;
	DWORD GetWearFlags() const;
	DWORD GetIBuyItemPrice() const;
	DWORD GetISellItemPrice() const;
	int GetLevelLimit() const;
	BOOL GetLimit(BYTE byIndex, TItemLimit* pItemLimit) const;
	BOOL GetApply(BYTE byIndex, TItemApply* pItemApply) const;
	long GetValue(BYTE byIndex) const;
	long GetSocket(BYTE byIndex) const;
	long SetSocket(BYTE byIndex, DWORD value);
	int GetSocketCount() const;
	DWORD GetIconNumber() const;

	UINT GetSpecularPoweru() const;
	float GetSpecularPowerf() const;

	/////

	BOOL IsEquipment() const;
#ifdef ENABLE_SOUL_BIND_SYSTEM
	bool IsSealAbleItem() const;
#endif

	/////

	//BOOL LoadItemData(const char* c_szFileName);
	void SetDefaultItemData(const char* c_szIconFileName, const char* c_szModelFileName = NULL);
	void SetItemTableData(TItemTable* pItemTable);

	/////
	// Mappers
public:
	// Costume
	bool IsCostume() const { return GetType() == ITEM_TYPE_COSTUME; }
	bool IsCostumeBody() const { return GetType() == ITEM_TYPE_COSTUME && GetSubType() == COSTUME_BODY; }
	bool IsCostumeHair() const { return GetType() == ITEM_TYPE_COSTUME && GetSubType() == COSTUME_HAIR; }
#if defined(ENABLE_MOUNT_COSTUME_SYSTEM)
	bool IsCostumeMount() const { return GetType() == ITEM_TYPE_COSTUME && GetSubType() == COSTUME_MOUNT; }
#endif
#if defined(ENABLE_ACCE_COSTUME_SYSTEM)
	bool IsCostumeAcce() const { return GetType() == ITEM_TYPE_COSTUME && GetSubType() == COSTUME_ACCE; }
#endif
#if defined(ENABLE_WEAPON_COSTUME_SYSTEM)
	bool IsCostumeWeapon() const { return GetType() == ITEM_TYPE_COSTUME && GetSubType() == COSTUME_WEAPON; }
#endif

	// Weapon
	bool IsWeapon() const { return GetType() == ITEM_TYPE_WEAPON; }
	bool IsMainWeapon() const
	{
		return GetType() == ITEM_TYPE_WEAPON && (
			GetSubType() == WEAPON_SWORD
			|| GetSubType() == WEAPON_DAGGER
			|| GetSubType() == WEAPON_BOW
			|| GetSubType() == WEAPON_TWO_HANDED
			|| GetSubType() == WEAPON_BELL
			|| GetSubType() == WEAPON_FAN
			|| GetSubType() == WEAPON_CLAW
			);
	}
	bool IsSword() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_SWORD; }
	bool IsDagger() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_DAGGER; }
	bool IsBow() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_BOW; }
	bool IsTwoHandSword() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_TWO_HANDED; }
	bool IsBell() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_BELL; }
	bool IsFan() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_FAN; }
	bool IsArrow() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_ARROW; }
	bool IsMountSpear() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_MOUNT_SPEAR; }
#if defined(ENABLE_QUIVER_SYSTEM)
	bool IsQuiver() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_QUIVER; }
#endif
	bool IsClaw() const { return GetType() == ITEM_TYPE_WEAPON && GetSubType() == WEAPON_CLAW; }

	// Armor
	bool IsArmor() const { return GetType() == ITEM_TYPE_ARMOR; }
	bool IsArmorBody() const { return GetType() == ITEM_TYPE_ARMOR && GetSubType() == ARMOR_BODY; }
	bool IsHelmet() const { return GetType() == ITEM_TYPE_ARMOR && GetSubType() == ARMOR_HEAD; }
	bool IsShield() const { return GetType() == ITEM_TYPE_ARMOR && GetSubType() == ARMOR_SHIELD; }
	bool IsWrist() const { return GetType() == ITEM_TYPE_ARMOR && GetSubType() == ARMOR_WRIST; }
	bool IsShoe() const { return GetType() == ITEM_TYPE_ARMOR && GetSubType() == ARMOR_FOOTS; }
	bool IsNecklace() const { return GetType() == ITEM_TYPE_ARMOR && GetSubType() == ARMOR_NECK; }
	bool IsEarRing() const { return GetType() == ITEM_TYPE_ARMOR && GetSubType() == ARMOR_EAR; }
#if defined(ENABLE_PENDANT)
	bool IsPendant() const { return GetType() == ITEM_TYPE_ARMOR && GetSubType() == ARMOR_PENDANT; }
#endif
#if defined(ENABLE_GLOVE_SYSTEM)
	bool IsGlove() const { return GetType() == ITEM_TYPE_ARMOR && GetSubType() == ARMOR_GLOVE; }
#endif

	// Unique
	bool IsBelt() const { return GetType() == ITEM_TYPE_BELT; }
	bool IsRing() const { return GetType() == ITEM_TYPE_RING; }
	/////

protected:
	void __LoadFiles();
	void __SetIconImage(const char* c_szFileName);

protected:
	std::string m_strModelFileName;
	std::string m_strSubModelFileName;
	std::string m_strDropModelFileName;
	std::string m_strIconFileName;
	std::string m_strDescription;
	std::string m_strSummary;
	std::vector<std::string> m_strLODModelFileNameVector;

	CGraphicThing* m_pModelThing;
	CGraphicThing* m_pSubModelThing;
	CGraphicThing* m_pDropModelThing;
	CGraphicSubImage* m_pIconImage;
	std::vector<CGraphicThing*> m_pLODModelThingVector;

	NRaceData::TAttachingDataVector m_AttachingDataVector;
	DWORD m_dwVnum;
	TItemTable m_ItemTable;
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	TScaleTable m_ScaleTable;
#endif

public:
	static void DestroySystem();

	static CItemData* New();
	static void Delete(CItemData* pkItemData);

	static CDynamicPool<CItemData> ms_kPool;

#if defined(ENABLE_ACCE_COSTUME_SYSTEM)
protected:
	typedef struct SItemScaleTable
	{
		D3DXVECTOR3 v3Scale[NRaceData::SEX_MAX_NUM][NRaceData::JOB_MAX_NUM];
		float fScaleParticle[NRaceData::SEX_MAX_NUM][NRaceData::JOB_MAX_NUM];
	} TItemScaleTable;
	TItemScaleTable m_ItemScaleTable;

public:
	float GetItemParticleScale(BYTE bJob, BYTE bSex);
	void SetItemTableScaleData(BYTE bJob, BYTE bSex, float fScaleX, float fScaleY, float fScaleZ, float fScaleParticle);
	D3DXVECTOR3& GetItemScaleVector(BYTE bJob, BYTE bSex);
#endif

#if defined(ENABLE_AURA_COSTUME_SYSTEM)
protected:
	DWORD m_dwAuraEffectID;

public:
	void SetAuraEffectID(const char* szAuraEffectPath);
	DWORD GetAuraEffectID() const { return m_dwAuraEffectID; }
#endif

};
