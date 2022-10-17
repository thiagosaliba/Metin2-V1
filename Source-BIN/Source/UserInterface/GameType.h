#pragma once
#include "../GameLib/ItemData.h"
#include "../GameLib//ItemUtil.h"

struct SAffects
{
	enum
	{
		AFFECT_MAX_NUM = 32,
	};

	SAffects() : dwAffects(0) {}
	SAffects(const DWORD& c_rAffects)
	{
		__SetAffects(c_rAffects);
	}
	int operator = (const DWORD& c_rAffects)
	{
		__SetAffects(c_rAffects);
	}

	BOOL IsAffect(BYTE byIndex)
	{
		return dwAffects & (1 << byIndex);
	}

	void __SetAffects(const DWORD& c_rAffects)
	{
		dwAffects = c_rAffects;
	}

	DWORD dwAffects;

};

extern std::string g_strGuildSymbolPathName;

const DWORD c_Name_Max_Length = 64;
const DWORD c_FileName_Max_Length = 128;
const DWORD c_Short_Name_Max_Length = 32;

const DWORD c_Inventory_Page_Size = 5 * 9; // x*y
#ifdef ENABLE_EXTEND_INVEN_SYSTEM
const DWORD c_Inventory_Page_Count = 4;
#else
const DWORD c_Inventory_Page_Count = 2;
#endif
#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
const DWORD c_Special_Inventory_Page_Size = 5 * 9; // x*y
const DWORD c_Special_Inventory_Page_Count = 5;
const DWORD c_Special_ItemSlot_Count = c_Special_Inventory_Page_Size * c_Special_Inventory_Page_Count;
#endif
const DWORD c_ItemSlot_Count = c_Inventory_Page_Size * c_Inventory_Page_Count;
const DWORD c_Equipment_Count = 12;

const DWORD c_Equipment_Start = c_ItemSlot_Count;

const DWORD c_Equipment_Body = c_Equipment_Start + 0;
const DWORD c_Equipment_Head = c_Equipment_Start + 1;
const DWORD c_Equipment_Shoes = c_Equipment_Start + 2;
const DWORD c_Equipment_Wrist = c_Equipment_Start + 3;
const DWORD c_Equipment_Weapon = c_Equipment_Start + 4;
const DWORD c_Equipment_Neck = c_Equipment_Start + 5;
const DWORD c_Equipment_Ear = c_Equipment_Start + 6;
const DWORD c_Equipment_Unique1 = c_Equipment_Start + 7;
const DWORD c_Equipment_Unique2 = c_Equipment_Start + 8;
const DWORD c_Equipment_Arrow = c_Equipment_Start + 9;
const DWORD c_Equipment_Shield = c_Equipment_Start + 10;

// 새로 추가된 신규 반지 & 벨트
// 장착형 아이템에 할당할 수 있는 위치가 기존 장비, 채기랍 퀘스트 보상, 코스튬 시스템 등으로 인해서 공간이 잘려있다.
// 이게 다 채기랍 보상 버프를 장착아이템처럼 구현한 ㅅㄲ 때문에 난리났따... ㅆㅂ
//
// 정리하면, 기존 장비창들은 서버DB상 아이템 포지션이 90 ~ 102 이고,
// 2013년 초에 새로 추가되는 슬롯들은 111 ~ 부터 시작한다. 착용 장비에서 최대로 사용할 수 있는 값은 121 까지이고, 122부터는 용혼석에서 사용한다.
#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
///#if defined(ENABLE_MOUNT_COSTUME_SYSTEM) && defined(ENABLE_ACCE_COSTUME_SYSTEM) && defined(ENABLE_WEAPON_COSTUME_SYSTEM)
const DWORD c_New_Equipment_Start = c_Equipment_Start + 24;
#if defined(ENABLE_PENDANT) && defined(ENABLE_GLOVE_SYSTEM)
const DWORD c_New_Equipment_Count = 5;
#elif defined(ENABLE_PENDANT) || defined(ENABLE_GLOVE_SYSTEM)
const DWORD c_New_Equipment_Count = 4;
#else
const DWORD c_New_Equipment_Count = 3;
#endif
const DWORD c_Equipment_Ring1 = c_New_Equipment_Start + 0;
const DWORD c_Equipment_Ring2 = c_New_Equipment_Start + 1;
const DWORD c_Equipment_Belt = c_New_Equipment_Start + 2;
#if defined(ENABLE_PENDANT)
const DWORD c_Equipment_Pendant = c_New_Equipment_Start + 3;
#endif
#if defined(ENABLE_GLOVE_SYSTEM)
const DWORD c_Equipment_Glove = c_New_Equipment_Start + 4;
#endif
#endif

#if defined(ENABLE_CONQUEROR_LEVEL)
enum ELevelTypes
{
	LEVEL_TYPE_BASE,
	LEVEL_TYPE_CONQUEROR,
	LEVEL_TYPE_MAX
};
#endif

#ifdef WJ_ENABLE_TRADABLE_ICON
enum ETopWindowTypes
{
	ON_TOP_WND_NONE,					// 0
	ON_TOP_WND_SHOP,					// 1
	ON_TOP_WND_EXCHANGE,				// 1
	ON_TOP_WND_SAFEBOX,					// 3
	ON_TOP_WND_PRIVATE_SHOP,			// 4
	ON_TOP_WND_ITEM_COMB,				// 5
	ON_TOP_WND_PET_FEED,				// 6
	ON_TOP_WND_FISH_EVENT,				// 7
	ON_TOP_WND_MAILBOX,					// 8
	ON_TOP_WND_PET_ATTR_CHANGE,			// 9
	ON_TOP_WND_LUCKY_BOX,				// 10
	ON_TOP_WND_ATTR67,					// 11
	ON_TOP_WND_ACCE_COMBINE,			// 12
	ON_TOP_WND_ACCE_ABSORB,				// 13
	ON_TOP_WND_CHANGE_LOOK,				// 14
	ON_TOP_WND_MAX,
};
#endif

#if defined(ENABLE_KEYCHANGE_SYSTEM)
enum EKeySettings
{
	KEY_NONE,
	KEY_MOVE_UP_1,
	KEY_MOVE_DOWN_1,
	KEY_MOVE_LEFT_1,
	KEY_MOVE_RIGHT_1,
	KEY_MOVE_UP_2,
	KEY_MOVE_DOWN_2,
	KEY_MOVE_LEFT_2,
	KEY_MOVE_RIGHT_2,
	KEY_CAMERA_ROTATE_POSITIVE_1,
	KEY_CAMERA_ROTATE_NEGATIVE_1,
	KEY_CAMERA_ZOOM_POSITIVE_1,
	KEY_CAMERA_ZOOM_NEGATIVE_1,
	KEY_CAMERA_PITCH_POSITIVE_1,
	KEY_CAMERA_PITCH_NEGATIVE_1,
	KEY_CAMERA_ROTATE_POSITIVE_2,
	KEY_CAMERA_ROTATE_NEGATIVE_2,
	KEY_CAMERA_ZOOM_POSITIVE_2,
	KEY_CAMERA_ZOOM_NEGATIVE_2,
	KEY_CAMERA_PITCH_POSITIVE_2,
	KEY_CAMERA_PITCH_NEGATIVE_2,
	KEY_ROOTING_1,
	KEY_ROOTING_2,
	KEY_ATTACK,
	KEY_RIDEMYHORS,
	KEY_FEEDMYHORS,
	KEY_BYEMYHORS,
	KEY_RIDEHORS,
	KEY_EMOTION1,
	KEY_EMOTION2,
	KEY_EMOTION3,
	KEY_EMOTION4,
	KEY_EMOTION5,
	KEY_EMOTION6,
	KEY_EMOTION7,
	KEY_EMOTION8,
	KEY_EMOTION9,
	KEY_SLOT_1,
	KEY_SLOT_2,
	KEY_SLOT_3,
	KEY_SLOT_4,
	KEY_SLOT_5,
	KEY_SLOT_6,
	KEY_SLOT_7,
	KEY_SLOT_8,
	KEY_SLOT_CHANGE_1,
	KEY_SLOT_CHANGE_2,
	KEY_SLOT_CHANGE_3,
	KEY_SLOT_CHANGE_4,
	KEY_OPEN_STATE,
	KEY_OPEN_SKILL,
	KEY_OPEN_QUEST,
	KEY_OPEN_INVENTORY,
	KEY_OPEN_DDS,
	KEY_OPEN_MINIMAP,
	KEY_OPEN_LOGCHAT,
	KEY_OPEN_PET,
	KEY_OPEN_GUILD,
	KEY_OPEN_MESSENGER,
	KEY_OPEN_HELP,
	KEY_OPEN_ACTION,
	KEY_SCROLL_ONOFF,
	KEY_PLUS_MINIMAP,
	KEY_MIN_MINIMAP,
	KEY_SCREENSHOT,
	KEY_SHOW_NAME,
	KEY_OPEN_AUTO,
	KEY_AUTO_RUN,
	KEY_NEXT_TARGET,
	KEY_MONSTER_CARD,
	KEY_PARTY_MATCH,
	KEY_SELECT_DSS_1,
	KEY_SELECT_DSS_2,
	KEY_PASSIVE_ATTR1,
	KEY_PASSIVE_ATTR2,

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	KEY_SPECIAL_INVENTORY,
#endif

	KEY_ADDKEYBUFFERCONTROL = 100,
	KEY_ADDKEYBUFFERALT = 300,
	KEY_ADDKEYBUFFERSHIFT = 500,
};
#endif

#ifdef ENABLE_SKILL_COLOR_SYSTEM
enum ESkillColorLength
{
#if defined(ENABLE_CONQUEROR_LEVEL)
	MAX_SKILL_COUNT = 10,
#else
	MAX_SKILL_COUNT = 6,
#endif
	MAX_EFFECT_COUNT = 5,
	BUFF_BEGIN = MAX_SKILL_COUNT,
	MAX_BUFF_COUNT = 6,
	MAX_COLOR_SLOTS = MAX_SKILL_COUNT + MAX_BUFF_COUNT,
};
#endif

#if defined(ENABLE_SKILLBOOK_COMB_SYSTEM)
const DWORD c_SkillBook_Comb_Size = 10;
enum ESkillBookComb
{
	SKILLBOOK_COMB_SLOT_MAX = c_SkillBook_Comb_Size
};
#endif

#ifdef ENABLE_COSTUME_SYSTEM
const DWORD c_Costume_Slot_Start = c_Equipment_Start + 19; // [주의] 숫자(19) 하드코딩 주의. 현재 서버에서 코스츔 슬롯은 19부터임. 서버 common/length.h 파일의 EWearPositions 열거형 참고.
const DWORD c_Costume_Slot_Body = c_Costume_Slot_Start + 0;
const DWORD c_Costume_Slot_Hair = c_Costume_Slot_Start + 1;
#define COSTUME_SLOT_COUNT 2

#ifdef ENABLE_MOUNT_COSTUME_SYSTEM
const DWORD c_Costume_Slot_Mount = c_Costume_Slot_Start + 2;
#define COSTUME_SLOT_COUNT 3
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
const DWORD c_Costume_Slot_Acce = c_Costume_Slot_Start + 3;
#define COSTUME_SLOT_COUNT 4
#endif

#ifdef ENABLE_WEAPON_COSTUME_SYSTEM
const DWORD c_Costume_Slot_Weapon = c_Costume_Slot_Start + 4;
#define COSTUME_SLOT_COUNT 5
#endif

#ifdef ENABLE_AURA_COSTUME_SYSTEM
const DWORD c_Costume_Slot_Aura = c_Costume_Slot_Start + 5;
#define COSTUME_SLOT_COUNT 6
#endif

const DWORD c_Costume_Slot_Count = COSTUME_SLOT_COUNT;
const DWORD c_Costume_Slot_End = c_Costume_Slot_Start + c_Costume_Slot_Count;
#endif

// [주의] 숫자(32) 하드코딩 주의. 현재 서버에서 용혼석 슬롯은 32부터임. 
// 서버 common/length.h 파일의 EWearPositions 열거형이 32까지 확장될 것을 염두하고(32 이상은 확장 하기 힘들게 되어있음.), 
// 그 이후부터를 용혼석 장착 슬롯으로 사용.
const DWORD c_Wear_Max = 32;
const DWORD c_DragonSoul_Equip_Start = c_ItemSlot_Count + c_Wear_Max;
const DWORD c_DragonSoul_Equip_Slot_Max = 6;
const DWORD c_DragonSoul_Equip_End = c_DragonSoul_Equip_Start + c_DragonSoul_Equip_Slot_Max * DS_DECK_MAX_NUM;

// NOTE: 2013년 2월 5일 현재... 용혼석 데크는 2개가 존재하는데, 향후 확장 가능성이 있어서 3개 데크 여유분을 할당 해 둠. 그 뒤 공간은 벨트 인벤토리로 사용
const DWORD c_DragonSoul_Equip_Reserved_Count = c_DragonSoul_Equip_Slot_Max * 3;

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
// 벨트 아이템이 제공하는 인벤토리
const DWORD c_Belt_Inventory_Slot_Start = c_DragonSoul_Equip_End + c_DragonSoul_Equip_Reserved_Count;
const DWORD c_Belt_Inventory_Width = 4;
const DWORD c_Belt_Inventory_Height = 4;
const DWORD c_Belt_Inventory_Slot_Count = c_Belt_Inventory_Width * c_Belt_Inventory_Height;
const DWORD c_Belt_Inventory_Slot_End = c_Belt_Inventory_Slot_Start + c_Belt_Inventory_Slot_Count;

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
const DWORD c_Skill_Book_Inventory_Slot_Start = c_Belt_Inventory_Slot_End;
const DWORD c_Skill_Book_Inventory_Slot_Count = c_Special_ItemSlot_Count;
const DWORD c_Skill_Book_Inventory_Slot_End = c_Skill_Book_Inventory_Slot_Start + c_Skill_Book_Inventory_Slot_Count;

const DWORD c_Upgrade_Items_Inventory_Slot_Start = c_Skill_Book_Inventory_Slot_End;
const DWORD c_Upgrade_Items_Inventory_Slot_Count = c_Special_ItemSlot_Count;
const DWORD c_Upgrade_Items_Inventory_Slot_End = c_Upgrade_Items_Inventory_Slot_Start + c_Upgrade_Items_Inventory_Slot_Count;

const DWORD c_Stone_Inventory_Slot_Start = c_Upgrade_Items_Inventory_Slot_End;
const DWORD c_Stone_Inventory_Slot_Count = c_Special_ItemSlot_Count;
const DWORD c_Stone_Inventory_Slot_End = c_Stone_Inventory_Slot_Start + c_Stone_Inventory_Slot_Count;

const DWORD c_GiftBox_Inventory_Slot_Start = c_Stone_Inventory_Slot_End;
const DWORD c_GiftBox_Inventory_Slot_Count = c_Special_ItemSlot_Count;
const DWORD c_GiftBox_Inventory_Slot_End = c_GiftBox_Inventory_Slot_Start + c_GiftBox_Inventory_Slot_Count;
#endif

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
const DWORD c_Inventory_Count = c_GiftBox_Inventory_Slot_End;
#else
const DWORD c_Inventory_Count = c_Belt_Inventory_Slot_End;
#endif
#else
const DWORD c_Inventory_Count = c_DragonSoul_Equip_End;
#endif

// 용혼석 전용 인벤토리
const DWORD c_DragonSoul_Inventory_Start = 0;
const DWORD c_DragonSoul_Inventory_Box_Size = 32;
const DWORD c_DragonSoul_Inventory_Count = CItemData::DS_SLOT_NUM_TYPES * DRAGON_SOUL_GRADE_MAX * c_DragonSoul_Inventory_Box_Size;
const DWORD c_DragonSoul_Inventory_End = c_DragonSoul_Inventory_Start + c_DragonSoul_Inventory_Count;

enum ESlotType
{
	SLOT_TYPE_NONE,
	SLOT_TYPE_INVENTORY,
	SLOT_TYPE_SKILL,
	SLOT_TYPE_EMOTION,
	SLOT_TYPE_SHOP,
	SLOT_TYPE_EXCHANGE_OWNER,
	SLOT_TYPE_EXCHANGE_TARGET,
	SLOT_TYPE_QUICK_SLOT,
	SLOT_TYPE_SAFEBOX,
#if defined(ENABLE_GUILDRENEWAL_SYSTEM)
	SLOT_TYPE_GUILDBANK,
#endif
#if defined(ENABLE_ACCE_COSTUME_SYSTEM)
	SLOT_TYPE_ACCE,
#endif
	SLOT_TYPE_PRIVATE_SHOP,
	SLOT_TYPE_MALL,
	SLOT_TYPE_DRAGON_SOUL_INVENTORY,
#if defined(ENABLE_GROWTH_PET_SYSTEM)
	SLOT_TYPE_PET_FEED_WINDOW,
#endif
	SLOT_TYPE_EQUIPMENT,
	SLOT_TYPE_BELT_INVENTORY,
#if defined(ENABLE_AUTOMATIC_HUNTING_SYSTEM)
	SLOT_TYPE_AUTO,
#endif
#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
	SLOT_TYPE_CHANGE_LOOK,
#endif
#if defined(ENABLE_FISH_EVENT)
	SLOT_TYPE_FISH_EVENT,
#endif
#if defined(ENABLE_AURA_SYSTEM)
	SLOT_TYPE_AURA,
#endif
#if defined(ENABLE_PREMIUM_PRIVATE_SYSTEM)
	SLOT_TYPE_PREMIUM_PRIVATE_SHOP,
#endif
#if defined(ENABLE_SPECIAL_INVENTORY_SYSTEM)
	SLOT_TYPE_SKILL_BOOK_INVENTORY,
	SLOT_TYPE_UPGRADE_ITEMS_INVENTORY,
	SLOT_TYPE_STONE_INVENTORY,
	SLOT_TYPE_GIFT_BOX_INVENTORY,
#endif
	SLOT_TYPE_MAX,
};

enum EWindows
{
	RESERVED_WINDOW,
	INVENTORY, // 기본 인벤토리. (45칸 짜리가 2페이지 존재 = 90칸)
	EQUIPMENT,
	SAFEBOX,
	MALL,
	DRAGON_SOUL_INVENTORY,
	BELT_INVENTORY, // NOTE: W2.1 버전에 새로 추가되는 벨트 슬롯 아이템이 제공하는 벨트 인벤토리
#if defined(ENABLE_GUILDRENEWAL_SYSTEM)
	GUILDBANK,
#endif
#if defined(ENABLE_MAILBOX)
	MAIL,
#endif
	NPC_STORAGE,
#if defined(ENABLE_PREMIUM_PRIVATE_SHOP)
	PREMIUM_PRIVATE_SHOP,
#endif
#if defined(ENABLE_ACCE_COSTUME_SYSTEM)
	ACCEREFINE,
#endif
	GROUND, // NOTE: 2013년 2월5일 현재까지 unused.. 왜 있는거지???
#if defined(ENABLE_GROWTH_PET_SYSTEM)
	PET_FEED,
#endif
#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
	CHANGELOOK,
#endif
#if defined(ENABLE_AURA_SYSTEM)
	AURA_REFINE,
#endif
#if defined(ENABLE_CUBE_RENEWAL)
	CUBE_WINDOW,
#endif
#if defined(ENABLE_SPECIAL_INVENTORY_SYSTEM)
	SKILL_BOOK_INVENTORY,
	UPGRADE_ITEMS_INVENTORY,
	STONE_INVENTORY,
	GIFT_BOX_INVENTORY,
#endif

	WINDOW_TYPE_MAX,
};

enum EDSInventoryMaxNum
{
	DS_INVENTORY_MAX_NUM = c_DragonSoul_Inventory_Count,
	DS_REFINE_WINDOW_MAX_NUM = 15,
};

#if defined(ENABLE_DS_CHANGE_ATTR)
enum EDSRefineType
{
	DS_REFINE_TYPE_GRADE,
	DS_REFINE_TYPE_STEP,
	DS_REFINE_TYPE_STRENGTH,
	DS_REFINE_TYPE_CHANGE_ATTR,
	DS_REFINE_TYPE_MAX,
};
#endif

#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
enum class EChangeLookType : BYTE
{
	CHANGE_LOOK_TYPE_ITEM,
	CHANGE_LOOK_TYPE_MOUNT
};

enum class EChangeLookSlots : BYTE
{
	CHANGE_LOOK_SLOT_LEFT,
	CHANGE_LOOK_SLOT_RIGHT,
	CHANGE_LOOK_SLOT_MAX
};

enum class EChangeLookPrice : DWORD
{
	CHANGE_LOOK_ITEM_PRICE = 50000000,
	CHANGE_LOOK_MOUNT_PRICE = 30000000,
};

enum class EChangeLookItems : DWORD
{
	CHANGE_LOOK_TICKET_1 = 72326,
	CHANGE_LOOK_TICKET_2 = 72341,
	CHANGE_LOOK_CLEAR_SCROLL = 72325,
};
#endif

#pragma pack (push, 1)
#define WORD_MAX 0xffff

typedef struct SItemPos
{
	BYTE window_type;
	WORD cell;
	SItemPos()
	{
		window_type = INVENTORY;
		cell = WORD_MAX;
	}

	SItemPos(BYTE _window_type, WORD _cell)
	{
		window_type = _window_type;
		cell = _cell;
	}

	// 기존에 cell의 형을 보면 BYTE가 대부분이지만, oi
	// 어떤 부분은 int, 어떤 부분은 WORD로 되어있어,
	// 가장 큰 자료형인 int로 받는다.
	/*
	int operator=(const int _cell)
	{
		window_type = INVENTORY;
		cell = _cell;
		return cell;
	}
	*/
	bool IsValidCell()
	{
		switch (window_type)
		{
		case INVENTORY:
			return cell < c_Inventory_Count;

		case EQUIPMENT:
			return cell < c_DragonSoul_Equip_End;

		case DRAGON_SOUL_INVENTORY:
			return cell < (DS_INVENTORY_MAX_NUM);

		default:
			return false;
		}
	}
	bool IsEquipCell()
	{
		switch (window_type)
		{
		case INVENTORY:
		case EQUIPMENT:
			return (c_Equipment_Start + c_Wear_Max > cell) && (c_Equipment_Start <= cell);

		case BELT_INVENTORY:
		case DRAGON_SOUL_INVENTORY:
			return false;

		default:
			return false;
		}
	}

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	bool IsBeltInventoryCell()
	{
		bool bResult = c_Belt_Inventory_Slot_Start <= cell && c_Belt_Inventory_Slot_End > cell;
		return bResult;
	}
#endif

#ifdef ENABLE_SPECIAL_INVENTORY_SYSTEM
	bool IsSkillBookInventoryCell()
	{
		bool bResult = c_Skill_Book_Inventory_Slot_Start <= cell && c_Skill_Book_Inventory_Slot_End > cell;
		return bResult;
	}

	bool IsUpgradeItemsInventoryCell()
	{
		bool bResult = c_Upgrade_Items_Inventory_Slot_Start <= cell && c_Upgrade_Items_Inventory_Slot_End > cell;
		return bResult;
	}

	bool IsStoneInventoryCell()
	{
		bool bResult = c_Stone_Inventory_Slot_Start <= cell && c_Stone_Inventory_Slot_End > cell;
		return bResult;
	}

	bool IsGiftBoxInventoryCell()
	{
		bool bResult = c_GiftBox_Inventory_Slot_Start <= cell && c_GiftBox_Inventory_Slot_End > cell;
		return bResult;
	}
#endif

	bool operator== (const struct SItemPos& rhs) const
	{
		return (window_type == rhs.window_type) && (cell == rhs.cell);
	}

	bool operator< (const struct SItemPos& rhs) const
	{
		return (window_type < rhs.window_type) || ((window_type == rhs.window_type) && (cell < rhs.cell));
	}
} TItemPos;
#pragma pack(pop)

const DWORD c_QuickBar_Line_Count = 3;
const DWORD c_QuickBar_Slot_Count = 12;

const float c_Idle_WaitTime = 5.0f;

const int c_Monster_Race_Start_Number = 6;
const int c_Monster_Model_Start_Number = 20001;

const float c_fAttack_Delay_Time = 0.2f;
const float c_fHit_Delay_Time = 0.1f;
const float c_fCrash_Wave_Time = 0.2f;
const float c_fCrash_Wave_Distance = 3.0f;

const float c_fHeight_Step_Distance = 50.0f;

enum
{
	DISTANCE_TYPE_FOUR_WAY,
	DISTANCE_TYPE_EIGHT_WAY,
	DISTANCE_TYPE_ONE_WAY,
	DISTANCE_TYPE_MAX_NUM,
};

const int c_iShow_UI_Window_Limit_Range = 1000;

const float c_fMagic_Script_Version = 1.0f;
const float c_fSkill_Script_Version = 1.0f;
const float c_fMagicSoundInformation_Version = 1.0f;
const float c_fBattleCommand_Script_Version = 1.0f;
const float c_fEmotionCommand_Script_Version = 1.0f;
const float c_fActive_Script_Version = 1.0f;
const float c_fPassive_Script_Version = 1.0f;

// Used by PushMove
const float c_fWalkDistance = 175.0f;
const float c_fRunDistance = 310.0f;

#define FILE_MAX_LEN 128

enum
{
#ifdef ENABLE_ITEM_SOCKET5
	ITEM_SOCKET_SLOT_MAX_NUM = 5,
#else
	ITEM_SOCKET_SLOT_MAX_NUM = 3,
#endif
	ITEM_ATTRIBUTE_SLOT_MAX_NUM = 7,
#if defined(ENABLE_APPLY_RANDOM)
	ITEM_APPLY_RANDOM_SLOT_MAX_NUM = 4,
#endif
};

#pragma pack(push)
#pragma pack(1)

typedef struct SQuickSlot
{
	BYTE Type;
	WORD Position;
} TQuickSlot;

typedef struct TPlayerItemAttribute
{
	BYTE bType;
	short sValue;
#if defined(ENABLE_APPLY_RANDOM)
	BYTE bPath;
#endif
} TPlayerItemAttribute;

typedef struct packet_item
{
	DWORD vnum;
	WORD count;
	DWORD flags;
	DWORD anti_flags;
	long alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
#if defined(ENABLE_APPLY_RANDOM)
	TPlayerItemAttribute aApplyRandom[ITEM_APPLY_RANDOM_SLOT_MAX_NUM];
#endif
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#ifdef ENABLE_SOUL_BIND_SYSTEM
	long soulbind;
#endif
#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
	DWORD dwTransmutationVnum;
#endif
} TItemData;

typedef struct packet_shop_item
{
	DWORD vnum;
	DWORD price;
#if defined(ENABLE_CHEQUE_SYSTEM)
	DWORD price_cheque;
#endif
	WORD count;
	BYTE display_pos;
	long alSockets[ITEM_SOCKET_SLOT_MAX_NUM];
#if defined(ENABLE_APPLY_RANDOM)
	TPlayerItemAttribute aApplyRandom[ITEM_APPLY_RANDOM_SLOT_MAX_NUM];
#endif
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_SLOT_MAX_NUM];
#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
	DWORD dwTransmutationVnum;
#endif
#ifdef ENABLE_SHOPEX_RENEWAL
	DWORD price_type = 1;
	DWORD price_vnum = 0;
#endif
} TShopItemData;

#if defined(ENABLE_PRIVATESHOP_SEARCH_SYSTEM)
struct ShopSearchData
{
	TShopItemData item;
	std::string name;
	DWORD dwShopPID;
};
#endif

#pragma pack(pop)

inline float GetSqrtDistance(int ix1, int iy1, int ix2, int iy2) // By sqrt
{
	float dx, dy;

	dx = float(ix1 - ix2);
	dy = float(iy1 - iy2);

	return sqrtf(dx * dx + dy * dy);
}

// DEFAULT_FONT
void DefaultFont_Startup();
void DefaultFont_Cleanup();
void DefaultFont_SetName(const char* c_szFontName);
CResource* DefaultFont_GetResource();
CResource* DefaultItalicFont_GetResource();
// END_OF_DEFAULT_FONT

void SetGuildSymbolPath(const char* c_szPathName);
const char* GetGuildSymbolFileName(DWORD dwGuildID);
BYTE SlotTypeToInvenType(BYTE bSlotType);
BYTE WindowTypeToSlotType(BYTE bWndType);
#ifdef ENABLE_DETAILS_UI
BYTE ApplyTypeToPointType(BYTE bApplyType);
#endif
