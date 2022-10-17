#ifndef __INC_PACKET_H__
#define __INC_PACKET_H__

enum CG_HEADERS
{
	HEADER_CG_LOGIN = 1,
	HEADER_CG_ATTACK = 2,
	HEADER_CG_CHAT = 3,
	HEADER_CG_CHARACTER_CREATE = 4,
	HEADER_CG_CHARACTER_DELETE = 5,
	HEADER_CG_CHARACTER_SELECT = 6,
	HEADER_CG_MOVE = 7,
	HEADER_CG_SYNC_POSITION = 8,
	//HEADER_CG_DIRECT_ENTER = 9,
	HEADER_CG_ENTERGAME = 10,

	HEADER_CG_ITEM_USE = 11,
	HEADER_CG_ITEM_DROP = 12,
	HEADER_CG_ITEM_MOVE = 13,
	//HEADER_CG_UNUSED = 14,
	HEADER_CG_ITEM_PICKUP = 15,

	HEADER_CG_QUICKSLOT_ADD = 16,
	HEADER_CG_QUICKSLOT_DEL = 17,
	HEADER_CG_QUICKSLOT_SWAP = 18,
	HEADER_CG_WHISPER = 19,
	HEADER_CG_ITEM_DROP2 = 20,
#if defined(__NEW_DROP_DIALOG__)
	HEADER_CG_ITEM_DESTROY = 21,
#endif

	//HEADER_CG_UNUSED = 22,
	HEADER_CG_PLAYER_PIN_CODE = 23,
	//HEADER_CG_UNUSED = 24,
	//HEADER_CG_UNUSED = 25,

	HEADER_CG_ON_CLICK = 26,
	HEADER_CG_EXCHANGE = 27,
	HEADER_CG_CHARACTER_POSITION = 28,
	HEADER_CG_SCRIPT_ANSWER = 29,
	HEADER_CG_QUEST_INPUT_STRING = 30,
	HEADER_CG_QUEST_CONFIRM = 31,

	//HEADER_CG_UNUSED = 32,
	//HEADER_CG_UNUSED = 33,
	//HEADER_CG_UNUSED = 34,
	//HEADER_CG_UNUSED = 35,
	//HEADER_CG_UNUSED = 36,
	//HEADER_CG_UNUSED = 37,
	//HEADER_CG_UNUSED = 38,
	//HEADER_CG_UNUSED = 39,
	//HEADER_CG_UNUSED = 40,
	//HEADER_CG_UNUSED = 41,
	//HEADER_CG_UNUSED = 42,
	//HEADER_CG_UNUSED = 43,
	//HEADER_CG_UNUSED = 44,
	//HEADER_CG_UNUSED = 45,
	//HEADER_CG_UNUSED = 46,
	//HEADER_CG_UNUSED = 47,
	//HEADER_CG_UNUSED = 48,
	//HEADER_CG_UNUSED = 49,

	HEADER_CG_SHOP = 50,
	HEADER_CG_FLY_TARGETING = 51,
	HEADER_CG_USE_SKILL = 52,
	HEADER_CG_ADD_FLY_TARGETING = 53,
	HEADER_CG_SHOOT = 54,
	HEADER_CG_MYSHOP = 55,
#if defined(__MYSHOP_DECO__)
	HEADER_CG_MYSHOP_DECO = 56,
#endif
#if defined(__SKILL_COLOR_SYSTEM__)
	HEADER_CG_SKILL_COLOR = 57,
#endif
	//HEADER_CG_UNUSED = 58,

#if defined(__SEND_TARGET_INFO__)
	HEADER_CG_TARGET_INFO_LOAD = 59,
#endif
	HEADER_CG_ITEM_USE_TO_ITEM = 60,
	HEADER_CG_TARGET = 61,

	//HEADER_CG_UNUSED = 62,
	//HEADER_CG_UNUSED = 63,

	HEADER_CG_TEXT = 64, // @ 로 시작되면 텍스트를 파싱한다.
	HEADER_CG_WARP = 65,
	HEADER_CG_SCRIPT_BUTTON = 66,
	HEADER_CG_MESSENGER = 67,

	//HEADER_CG_UNUSED = 68,

	HEADER_CG_MALL_CHECKOUT = 69,
	HEADER_CG_SAFEBOX_CHECKIN = 70, // 아이템을 창고에 넣넎는다.
	HEADER_CG_SAFEBOX_CHECKOUT = 71, // 아이템을 창고로 부터 빼온다.

	HEADER_CG_PARTY_INVITE = 72,
	HEADER_CG_PARTY_INVITE_ANSWER = 73,
	HEADER_CG_PARTY_REMOVE = 74,
	HEADER_CG_PARTY_SET_STATE = 75,
	HEADER_CG_PARTY_USE_SKILL = 76,
	HEADER_CG_SAFEBOX_ITEM_MOVE = 77,
	HEADER_CG_PARTY_PARAMETER = 78,

	//HEADER_CG_UNUSED = 79,

	HEADER_CG_GUILD = 80,
	HEADER_CG_ANSWER_MAKE_GUILD = 81,
	HEADER_CG_FISHING = 82,
	HEADER_CG_ITEM_GIVE = 83,

	//HEADER_CG_UNUSED = 84,
	//HEADER_CG_UNUSED = 85,
	//HEADER_CG_UNUSED = 86,
	//HEADER_CG_UNUSED = 87,
	//HEADER_CG_UNUSED = 88,
	//HEADER_CG_UNUSED = 89,

	HEADER_CG_EMPIRE = 90,

	//HEADER_CG_UNUSED = 91,
	//HEADER_CG_UNUSED = 92,
	//HEADER_CG_UNUSED = 93,
	//HEADER_CG_UNUSED = 94,
	//HEADER_CG_UNUSED = 95,

	HEADER_CG_REFINE = 96,

	//HEADER_CG_UNUSED = 97,
	//HEADER_CG_UNUSED = 98,
	//HEADER_CG_UNUSED = 99,

	HEADER_CG_MARK_LOGIN = 100,
	HEADER_CG_MARK_CRCLIST = 101,
	HEADER_CG_MARK_UPLOAD = 102,
	//HEADER_CG_CRC_REPORT = 103,
	HEADER_CG_MARK_IDXLIST = 104,

	HEADER_CG_HACK = 105,
	HEADER_CG_CHANGE_NAME = 106,
	//HEADER_CG_UNUSED = 107,
	//HEADER_CG_UNUSED = 108,
	HEADER_CG_LOGIN2 = 109,
	HEADER_CG_DUNGEON = 110,
	HEADER_CG_LOGIN3 = 111,

	HEADER_CG_GUILD_SYMBOL_UPLOAD = 112,
	HEADER_CG_SYMBOL_CRC = 113,

	// SCRIPT_SELECT_ITEM
	HEADER_CG_SCRIPT_SELECT_ITEM = 114,
	// END_OF_SCRIPT_SELECT_ITEM

	//HEADER_CG_UNUSED = 115,

	//HEADER_CG_UNUSED = 116,

	//HEADER_CG_UNUSED = 117,
	//HEADER_CG_UNUSED = 118,
	//HEADER_CG_UNUSED = 119,
	HEADER_CG_WHISPER_DETAILS = 120,
	//HEADER_CG_UNUSED = 121,
	//HEADER_CG_UNUSED = 122,
	//HEADER_CG_UNUSED = 123,
	//HEADER_CG_UNUSED = 124,
	//HEADER_CG_UNUSED = 125,
	//HEADER_CG_UNUSED = 126,
	//HEADER_CG_UNUSED = 127,
	//HEADER_CG_UNUSED = 128,
	//HEADER_CG_UNUSED = 129,
	//HEADER_CG_UNUSED = 130,
	//HEADER_CG_UNUSED = 131,
	//HEADER_CG_UNUSED = 132,
	//HEADER_CG_UNUSED = 133,
	//HEADER_CG_UNUSED = 134,
	//HEADER_CG_UNUSED = 135,
	//HEADER_CG_UNUSED = 136,
	//HEADER_CG_UNUSED = 137,
	//HEADER_CG_UNUSED = 138,
	//HEADER_CG_UNUSED = 139,
	//HEADER_CG_UNUSED = 140,
	//HEADER_CG_UNUSED = 141,
	//HEADER_CG_UNUSED = 142,
	//HEADER_CG_UNUSED = 143,
	//HEADER_CG_UNUSED = 144,
	//HEADER_CG_UNUSED = 145,
	//HEADER_CG_UNUSED = 146,
	//HEADER_CG_UNUSED = 147,
	//HEADER_CG_UNUSED = 148,
	//HEADER_CG_UNUSED = 149,
	//HEADER_CG_UNUSED = 150,
#if defined(__SKILLBOOK_COMB_SYSTEM__)
	HEADER_CG_SKILLBOOK_COMB = 151,
#endif
	//HEADER_CG_UNUSED = 152,
	//HEADER_CG_UNUSED = 153,
	//HEADER_CG_UNUSED = 154,
	//HEADER_CG_UNUSED = 155,
	//HEADER_CG_UNUSED = 156,
	//HEADER_CG_UNUSED = 157,
	//HEADER_CG_UNUSED = 158,
	//HEADER_CG_UNUSED = 159,
	//HEADER_CG_UNUSED = 160,
	//HEADER_CG_UNUSED = 161,
	//HEADER_CG_UNUSED = 162,
	//HEADER_CG_UNUSED = 163,
	//HEADER_CG_UNUSED = 164,
	//HEADER_CG_UNUSED = 165,
	//HEADER_CG_UNUSED = 166,
	//HEADER_CG_UNUSED = 167,
	//HEADER_CG_UNUSED = 168,
	//HEADER_CG_UNUSED = 169,
	//HEADER_CG_UNUSED = 170,
	//HEADER_CG_UNUSED = 171,
	//HEADER_CG_UNUSED = 172,
	//HEADER_CG_UNUSED = 173,
	//HEADER_CG_UNUSED = 174,
	//HEADER_CG_UNUSED = 175,
	//HEADER_CG_UNUSED = 176,
	//HEADER_CG_UNUSED = 177,
	//HEADER_CG_UNUSED = 178,
	//HEADER_CG_UNUSED = 179,
	//HEADER_CG_UNUSED = 180,
	//HEADER_CG_UNUSED = 181,
	//HEADER_CG_UNUSED = 182,
	//HEADER_CG_UNUSED = 183,
	//HEADER_CG_UNUSED = 184,
	//HEADER_CG_UNUSED = 185,
	//HEADER_CG_UNUSED = 186,
	//HEADER_CG_UNUSED = 187,
	//HEADER_CG_UNUSED = 188,
	//HEADER_CG_UNUSED = 189,
	//HEADER_CG_UNUSED = 190,
	//HEADER_CG_UNUSED = 191,
	//HEADER_CG_UNUSED = 192,
	//HEADER_CG_UNUSED = 193,
	//HEADER_CG_UNUSED = 194,
	//HEADER_CG_UNUSED = 195,
	//HEADER_CG_UNUSED = 196,
	//HEADER_CG_UNUSED = 197,
	//HEADER_CG_UNUSED = 198,
	//HEADER_CG_UNUSED = 199,

	//HEADER_CG_ROULETTE = 200,
	//HEADER_CG_UNUSED = 201,

	//
	//HEADER_CG_UNUSED = 202,

	// NOTE : 이런 개XXX 정말 이거 Packet설계한 사람은 누구냐. 이렇게 코딩하고 밥이 넘어가나.
	// enum을 별도로 구별을 하던가. 아님 namepsace로 구별을 하던가..
	// 정말 packet generator까지는 바라지도 않는다. 이런 씨XX
	// 이러다가 숫자 겹치면 누가 책임지는데???
	//HEADER_CG_HS_ACK = 203,
	//HEADER_CG_XTRAP_ACK = 204,

	HEADER_CG_DRAGON_SOUL_REFINE = 205,
	HEADER_CG_STATE_CHECKER = 206,

	HEADER_CG_PHASE = 207,
	//HEADER_CG_UNUSED = 208,
	//HEADER_CG_UNUSED = 209,
	//HEADER_CG_UNUSED = 210,
#if defined(__ACCE_COSTUME_SYSTEM__)
	HEADER_CG_ACCE = 211,
#endif
	//HEADER_CG_UNUSED = 212,
	//HEADER_CG_UNUSED = 213,
	//HEADER_CG_UNUSED = 214,
#if defined(__MAILBOX__)
	HEADER_CG_MAILBOX_WRITE = 215,
	HEADER_CG_MAILBOX_WRITE_CONFIRM = 216,
	HEADER_CG_MAILBOX_PROCESS = 217,
#endif
	//HEADER_CG_UNUSED = 218,
	//HEADER_CG_UNUSED = 219,
#if defined(__PRIVATESHOP_SEARCH_SYSTEM__)
	HEADER_CG_PRIVATESHOP_SEARCH = 220,
	HEADER_CG_PRIVATESHOP_SEARCH_CLOSE = 221,
	HEADER_CG_PRIVATESHOP_SEARCH_BUY_ITEM = 222,
#endif
	//HEADER_CG_UNUSED = 223,
	//HEADER_CG_UNUSED = 224,
	//HEADER_CG_UNUSED = 225,
#if defined(__MINI_GAME_CATCH_KING__)
	HEADER_CG_MINI_GAME_CATCH_KING = 226,
#endif
	//HEADER_CG_UNUSED = 227,
#if defined(__CHANGE_LOOK_SYSTEM__)
	HEADER_CG_CHANGE_LOOK = 228,
#endif
	//HEADER_CG_UNUSED = 229,
	HEADER_CG_CHANGE_LANGUAGE = 230,
	//HEADER_CG_UNUSED = 231,
	//HEADER_CG_UNUSED = 232,
	//HEADER_CG_UNUSED = 233,
	//HEADER_CG_UNUSED = 234,
	//HEADER_CG_UNUSED = 235,
	//HEADER_CG_UNUSED = 236,
	//HEADER_CG_UNUSED = 237,
	//HEADER_CG_UNUSED = 238,
	//HEADER_CG_UNUSED = 239,
	//HEADER_CG_UNUSED = 240,
	HEADER_CG_CLIENT_VERSION2 = 0xf1, // 241
	//HEADER_CG_UNUSED = 242,
	//HEADER_CG_UNUSED = 243,
	//HEADER_CG_UNUSED = 244,
	//HEADER_CG_UNUSED = 245,
	//HEADER_CG_UNUSED = 246,
	//HEADER_CG_UNUSED = 247,
	//HEADER_CG_UNUSED = 248,
	//HEADER_CG_UNUSED = 249,
	//HEADER_CG_UNUSED = 250,
#if defined(__IMPROVED_PACKET_ENCRYPTION__)
	HEADER_CG_KEY_AGREEMENT = 0xfb, // 251
#endif
	HEADER_CG_TIME_SYNC = 0xfc, // 252
	HEADER_CG_CLIENT_VERSION = 0xfd, // 253
	HEADER_CG_PONG = 0xfe, // 254
	HEADER_CG_HANDSHAKE = 0xff, // 255
};

enum GC_HEADERS
{
	HEADER_GC_CHARACTER_ADD = 1,
	HEADER_GC_CHARACTER_DEL = 2,
	HEADER_GC_MOVE = 3,
	HEADER_GC_CHAT = 4,
	HEADER_GC_SYNC_POSITION = 5,

	HEADER_GC_LOGIN_SUCCESS = 6,
	HEADER_GC_LOGIN_FAILURE = 7,

	HEADER_GC_CHARACTER_CREATE_SUCCESS = 8,
	HEADER_GC_CHARACTER_CREATE_FAILURE = 9,
	HEADER_GC_CHARACTER_DELETE_SUCCESS = 10,
	HEADER_GC_CHARACTER_DELETE_WRONG_SOCIAL_ID = 11,

	//HEADER_CG_UNUSED = 12,
	HEADER_GC_STUN = 13,
	HEADER_GC_DEAD = 14,

	HEADER_GC_MAIN_CHARACTER_OLD = 15,
	HEADER_GC_CHARACTER_POINTS = 16,
	HEADER_GC_CHARACTER_POINT_CHANGE = 17,
	HEADER_GC_CHANGE_SPEED = 18,
	HEADER_GC_CHARACTER_UPDATE = 19,

	HEADER_GC_ITEM_DEL = 20,
	HEADER_GC_ITEM_SET = 21,
	HEADER_GC_ITEM_USE = 22,
	HEADER_GC_ITEM_DROP = 23,

	//HEADER_CG_UNUSED = 24,
	HEADER_GC_ITEM_UPDATE = 25,
	HEADER_GC_ITEM_GROUND_ADD = 26,
	HEADER_GC_ITEM_GROUND_DEL = 27,

	HEADER_GC_QUICKSLOT_ADD = 28,
	HEADER_GC_QUICKSLOT_DEL = 29,
	HEADER_GC_QUICKSLOT_SWAP = 30,

	HEADER_GC_ITEM_OWNERSHIP = 31,

	HEADER_GC_LOGIN_SUCCESS_NEWSLOT = 32,

	//HEADER_GC_UNUSED = 33,
	HEADER_GC_WHISPER = 34,
	//HEADER_GC_UNUSED = 35,

	HEADER_GC_MOTION = 36,
	//HEADER_GC_UNUSED = 37,

	HEADER_GC_SHOP = 38,
	HEADER_GC_SHOP_SIGN = 39,

	HEADER_GC_DUEL_START = 40,
	HEADER_GC_PVP = 41,
	HEADER_GC_EXCHANGE = 42,
	HEADER_GC_CHARACTER_POSITION = 43,

	HEADER_GC_PING = 44,
	HEADER_GC_SCRIPT = 45,
	HEADER_GC_QUEST_CONFIRM = 46,

	//HEADER_GC_UNUSED = 47,
	//HEADER_GC_UNUSED = 48,
	//HEADER_GC_UNUSED = 49,
	//HEADER_GC_UNUSED = 50,
	//HEADER_GC_UNUSED = 51,
	//HEADER_GC_UNUSED = 52,
	//HEADER_GC_UNUSED = 53,
	//HEADER_GC_UNUSED = 54,
	//HEADER_GC_UNUSED = 55,
	//HEADER_GC_UNUSED = 56,
	//HEADER_GC_UNUSED = 57,
#if defined(__SEND_TARGET_INFO__)
	HEADER_GC_TARGET_INFO = 58,
#endif
	//HEADER_GC_UNUSED = 59,
	//HEADER_GC_UNUSED = 60,
	HEADER_GC_MOUNT = 61,
	HEADER_GC_OWNERSHIP = 62,
	HEADER_GC_TARGET = 63,
	//HEADER_GC_UNUSED = 64,
	HEADER_GC_WARP = 65,
	//HEADER_GC_UNUSED = 66,
	//HEADER_GC_UNUSED = 67,
	//HEADER_GC_UNUSED = 68,
	HEADER_GC_ADD_FLY_TARGETING = 69,

	HEADER_GC_CREATE_FLY = 70,
	HEADER_GC_FLY_TARGETING = 71,
	HEADER_GC_SKILL_LEVEL_OLD = 72,
	//HEADER_GC_UNUSED = 73,
	HEADER_GC_MESSENGER = 74,
	HEADER_GC_GUILD = 75,
	HEADER_GC_SKILL_LEVEL = 76,

	HEADER_GC_PARTY_INVITE = 77,
	HEADER_GC_PARTY_ADD = 78,
	HEADER_GC_PARTY_UPDATE = 79,
	HEADER_GC_PARTY_REMOVE = 80,

	HEADER_GC_QUEST_INFO = 81,
	HEADER_GC_REQUEST_MAKE_GUILD = 82,
	HEADER_GC_PARTY_PARAMETER = 83,
#if defined(__WJ_SHOW_PARTY_ON_MINIMAP__)
	HEADER_GC_PARTY_POSITION_INFO = 84,
#endif

	HEADER_GC_SAFEBOX_SET = 85,
	HEADER_GC_SAFEBOX_DEL = 86,
	HEADER_GC_SAFEBOX_WRONG_PASSWORD = 87,
	HEADER_GC_SAFEBOX_SIZE = 88,

	HEADER_GC_FISHING = 89,

	HEADER_GC_EMPIRE = 90,

	HEADER_GC_PARTY_LINK = 91,
	HEADER_GC_PARTY_UNLINK = 92,
	//HEADER_GC_UNUSED = 93,
	//HEADER_GC_UNUSED = 94,
	HEADER_GC_REFINE_INFORMATION_OLD = 95,
	//HEADER_GC_OBSERVER_ADD = 96,
	//HEADER_GC_OBSERVER_REMOVE = 97,
	//HEADER_GC_OBSERVER_MOVE = 98,
	HEADER_GC_VIEW_EQUIP = 99,

	HEADER_GC_MARK_BLOCK = 100,
	//HEADER_GC_MARK_DIFF_DATA = 101,
	HEADER_GC_MARK_IDXLIST = 102,
	//HEADER_GC_UNUSED = 103,
	//HEADER_GC_UNUSED = 104,
	//HEADER_GC_SLOW_TIMER = 105,
	HEADER_GC_TIME = 106,
	HEADER_GC_CHANGE_NAME = 107,
	HEADER_GC_PLAYER_PIN_CODE = 108,
	//HEADER_GC_UNUSED = 109,

	HEADER_GC_DUNGEON = 110,

	HEADER_GC_WALK_MODE = 111,
	HEADER_GC_SKILL_GROUP = 112,
	HEADER_GC_MAIN_CHARACTER = 113,

	HEADER_GC_SEPCIAL_EFFECT = 114,
	HEADER_GC_NPC_POSITION = 115,

	//HEADER_CG_UNUSED = 116,
	//HEADER_GC_UNUSED = 117,
	HEADER_GC_LOGIN_KEY = 118,

	HEADER_GC_REFINE_INFORMATION = 119,
	//HEADER_GC_CHARACTER_ADD2 = 120,
	HEADER_GC_CHANNEL = 121,

	HEADER_GC_MALL_OPEN = 122,

	HEADER_GC_TARGET_UPDATE = 123,
	HEADER_GC_TARGET_DELETE = 124,
	HEADER_GC_TARGET_CREATE = 125,

	HEADER_GC_AFFECT_ADD = 126,
	HEADER_GC_AFFECT_REMOVE = 127,

	HEADER_GC_MALL_SET = 128,
	HEADER_GC_MALL_DEL = 129,

	HEADER_GC_LAND_LIST = 130,
	HEADER_GC_LOVER_INFO = 131,
	HEADER_GC_LOVE_POINT_UPDATE = 132,

	HEADER_GC_SYMBOL_DATA = 133,

	// MINING
	HEADER_GC_DIG_MOTION = 134,
	// END_OF_MINING

	HEADER_GC_DAMAGE_INFO = 135,
	HEADER_GC_CHAR_ADDITIONAL_INFO = 136,

	// SUPPORT_BGM
	HEADER_GC_MAIN_CHARACTER3_BGM = 137,
	HEADER_GC_MAIN_CHARACTER4_BGM_VOL = 138,
	// END_OF_SUPPORT_BGM

	//HEADER_GC_UNUSED = 139,
	//HEADER_GC_UNUSED = 140,
	//HEADER_GC_UNUSED = 141,
	//HEADER_GC_UNUSED = 142,
	//HEADER_GC_UNUSED = 143,
	//HEADER_GC_UNUSED = 144,
	//HEADER_GC_UNUSED = 145,
	//HEADER_GC_UNUSED = 146,
	//HEADER_GC_UNUSED = 147,
	//HEADER_GC_UNUSED = 148,
	//HEADER_GC_UNUSED = 149,

	HEADER_GC_AUTH_SUCCESS = 150,
	HEADER_GC_PANAMA_PACK = 151,

	// HYBRID CRYPT
	HEADER_GC_HYBRIDCRYPT_KEYS = 152,
	HEADER_GC_HYBRIDCRYPT_SDB = 153, // SDB means Supplmentary Data Blocks
	// HYBRID CRYPT

	//HEADER_CG_UNUSED = 154,
	//HEADER_GC_UNUSED = 155,
#if defined(__PRIVATESHOP_SEARCH_SYSTEM__)
	HEADER_GC_PRIVATESHOP_SEARCH = 156,
	HEADER_GC_PRIVATESHOP_SEARCH_OPEN = 157,
#endif
	//HEADER_GC_UNUSED = 158,
	//HEADER_GC_UNUSED = 159,
	HEADER_GC_WHISPER_DETAILS = 160,
	//HEADER_GC_UNUSED = 161,
	//HEADER_GC_UNUSED = 162,
	//HEADER_GC_UNUSED = 163,
	//HEADER_GC_UNUSED = 164,
	//HEADER_GC_UNUSED = 165,
	//HEADER_GC_UNUSED = 166,
	//HEADER_GC_UNUSED = 167,
	//HEADER_GC_UNUSED = 168,
	//HEADER_GC_UNUSED = 169,
	//HEADER_GC_UNUSED = 170,
	//HEADER_GC_UNUSED = 171,
	//HEADER_GC_UNUSED = 172,
	//HEADER_GC_UNUSED = 173,
	//HEADER_GC_UNUSED = 174,
	//HEADER_GC_UNUSED = 175,
	//HEADER_GC_UNUSED = 176,
	//HEADER_GC_UNUSED = 177,
	//HEADER_GC_UNUSED = 178,
	//HEADER_GC_UNUSED = 179,
	//HEADER_GC_UNUSED = 180,
	//HEADER_GC_UNUSED = 181,
	//HEADER_GC_UNUSED = 182,
	//HEADER_GC_UNUSED = 183,
	//HEADER_GC_UNUSED = 184,
	//HEADER_GC_UNUSED = 185,
	//HEADER_GC_UNUSED = 186,
	//HEADER_GC_UNUSED = 187,
	//HEADER_GC_UNUSED = 188,
	//HEADER_GC_UNUSED = 189,
	HEADER_GC_HWID = 190,
	//HEADER_GC_UNUSED = 191,
	//HEADER_GC_UNUSED = 192,
	//HEADER_GC_UNUSED = 193,
	//HEADER_GC_UNUSED = 194,
	//HEADER_GC_UNUSED = 195,
	//HEADER_GC_UNUSED = 196,
	//HEADER_GC_UNUSED = 197,
	//HEADER_GC_UNUSED = 198,
	//HEADER_GC_UNUSED = 199,

	// ROULETTE
	HEADER_GC_ROULETTE = 200,
	// END_ROULETTE

	//HEADER_GC_UNUSED = 201,
	//HEADER_GC_UNUSED = 202,
	//HEADER_GC_UNUSED = 203,

	//HEADER_GC_UNUSED = 204,
	//HEADER_GC_UNUSED = 205,
	//HEADER_GC_UNUSED = 206,
	//HEADER_GC_UNUSED = 207,

	HEADER_GC_SPECIFIC_EFFECT = 208,

	HEADER_GC_DRAGON_SOUL_REFINE = 209,
	HEADER_GC_RESPOND_CHANNELSTATUS = 210,

	//HEADER_GC_UNUSED = 211,
	//HEADER_GC_UNUSED = 212,
	//HEADER_GC_UNUSED = 213,
	//HEADER_GC_UNUSED = 214,
#if defined(__ACCE_COSTUME_SYSTEM__)
	HEADER_GC_ACCE = 215,
#endif
	HEADER_GC_REQUEST_CHANGE_LANGUAGE = 216,
	//HEADER_GC_UNUSED = 217,
	//HEADER_GC_UNUSED = 218,
	//HEADER_GC_UNUSED = 219,
	//HEADER_GC_UNUSED = 220,
#if defined(__MAILBOX__)
	HEADER_GC_MAILBOX_PROCESS = 221,
	HEADER_GC_MAILBOX = 222,
	HEADER_GC_MAILBOX_ADD_DATA = 223,
	HEADER_GC_MAILBOX_ALL = 224,
	HEADER_GC_MAILBOX_UNREAD = 225,
#endif
	//HEADER_GC_UNUSED = 226,
	//HEADER_GC_UNUSED = 227,
#if defined(__CHANGE_LOOK_SYSTEM__)
	HEADER_GC_CHANGE_LOOK_SET = 228,
	HEADER_GC_CHANGE_LOOK_DEL = 229,
	HEADER_GC_CHANGE_LOOK_FREE_SET = 230,
	HEADER_GC_CHANGE_LOOK_FREE_DEL = 231,
#endif
	//HEADER_GC_UNUSED = 232,
	//HEADER_GC_UNUSED = 233,
	//HEADER_GC_UNUSED = 234,
	//HEADER_GC_UNUSED = 235,
	//HEADER_GC_UNUSED = 236,
	//HEADER_GC_UNUSED = 237,
#if defined(__MINI_GAME_CATCH_KING__)
	HEADER_GC_MINI_GAME_CATCH_KING = 238,
#endif

	//HEADER_GC_UNUSED = 239,
	//HEADER_GC_UNUSED = 240,
	//HEADER_GC_UNUSED = 241,
	//HEADER_GC_UNUSED = 242,
	//HEADER_GC_UNUSED = 243,
	//HEADER_GC_UNUSED = 244,
	//HEADER_GC_UNUSED = 245,
	//HEADER_GC_UNUSED = 246,
	//HEADER_GC_UNUSED = 247,
	//HEADER_GC_UNUSED = 248,
	//HEADER_GC_UNUSED = 249,
#if defined(__IMPROVED_PACKET_ENCRYPTION__)
	HEADER_GC_KEY_AGREEMENT_COMPLETED = 250, // 250
	HEADER_GC_KEY_AGREEMENT = 251, // 251
#endif
	HEADER_GC_TIME_SYNC = 252, // 252
	HEADER_GC_PHASE = 253, // 253
	HEADER_GC_BINDUDP = 254, // 254
	HEADER_GC_HANDSHAKE = 255, // 255
};

enum GG_HEADERS
{
	HEADER_GG_LOGIN = 1,
	HEADER_GG_LOGOUT = 2,
	HEADER_GG_RELAY = 3,
	HEADER_GG_NOTICE = 4,
	HEADER_GG_SHUTDOWN = 5,
	HEADER_GG_GUILD = 6,
	HEADER_GG_DISCONNECT = 7, // 누군가의 접속을 강제로 끊을 때
	HEADER_GG_SHOUT = 8,
	HEADER_GG_SETUP = 9,
	HEADER_GG_MESSENGER_ADD = 10,
	HEADER_GG_MESSENGER_REMOVE = 11,
	HEADER_GG_FIND_POSITION = 12,
	HEADER_GG_WARP_CHARACTER = 13,
	//HEADER_GG_UNUSED = 14,
	HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX = 15,
	HEADER_GG_TRANSFER = 16,
	HEADER_GG_XMAS_WARP_SANTA = 17,
	HEADER_GG_XMAS_WARP_SANTA_REPLY = 18,
	HEADER_GG_RELOAD_CRC_LIST = 19,
	HEADER_GG_LOGIN_PING = 20,
	HEADER_GG_CHECK_CLIENT_VERSION = 21,
	HEADER_GG_BLOCK_CHAT = 22,
	HEADER_GG_BIG_NOTICE = 23,
	HEADER_GG_BLOCK_EXCEPTION = 24,
	HEADER_GG_SIEGE = 25,
	HEADER_GG_MONARCH_NOTICE = 26,
	HEADER_GG_MONARCH_TRANSFER = 27,
	HEADER_GG_PCBANG_UPDATE = 28,
	HEADER_GG_CHECK_AWAKENESS = 29,
#if defined(__MESSENGER_BLOCK_SYSTEM__)
	HEADER_GG_MESSENGER_BLOCK_ADD = 30,
	HEADER_GG_MESSENGER_BLOCK_REMOVE = 31,
#endif
	HEADER_GG_LOCALE_NOTICE = 32,
};

#pragma pack(1)
typedef struct SPacketGGSetup
{
	BYTE bHeader;
	WORD wPort;
	BYTE bChannel;
} TPacketGGSetup;

typedef struct SPacketGGLogin
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD dwPID;
	BYTE bEmpire;
	long lMapIndex;
	BYTE bChannel;
	BYTE bLanguage;
} TPacketGGLogin;

typedef struct SPacketGGLogout
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGLogout;

typedef struct SPacketGGRelay
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	long lSize;
} TPacketGGRelay;

typedef struct SPacketGGNotice
{
	BYTE bHeader;
	long lSize;
	char szArg[CHAT_MAX_LEN + 1];
} TPacketGGNotice;

typedef struct SPacketGGBigNotice
{
	BYTE bHeader;
	long lSize;
} TPacketGGBigNotice;

typedef struct SPacketGGMonarchNotice
{
	BYTE bHeader;
	BYTE bEmpire;
	long lSize;
} TPacketGGMonarchNotice;

// FORKED_ROAD
typedef struct SPacketGGForkedMapInfo
{
	BYTE bHeader;
	BYTE bPass;
	BYTE bSungzi;
} TPacketGGForkedMapInfo;
// END_FORKED_ROAD

typedef struct SPacketGGShutdown
{
	BYTE bHeader;
} TPacketGGShutdown;

typedef struct SPacketGGGuild
{
	BYTE bHeader;
	BYTE bSubHeader;
	DWORD dwGuild;
} TPacketGGGuild;

enum
{
	GUILD_SUBHEADER_GG_CHAT,
	GUILD_SUBHEADER_GG_SET_MEMBER_COUNT_BONUS,
};

typedef struct SPacketGGGuildChat
{
	BYTE bHeader;
	BYTE bSubHeader;
	DWORD dwGuild;
	char szText[CHAT_MAX_LEN + 1];
} TPacketGGGuildChat;

typedef struct SPacketGGParty
{
	BYTE header;
	BYTE subheader;
	DWORD pid;
	DWORD leaderpid;
} TPacketGGParty;

enum
{
	PARTY_SUBHEADER_GG_CREATE,
	PARTY_SUBHEADER_GG_DESTROY,
	PARTY_SUBHEADER_GG_JOIN,
	PARTY_SUBHEADER_GG_QUIT,
};

typedef struct SPacketGGDisconnect
{
	BYTE bHeader;
	char szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGDisconnect;

typedef struct SPacketGGShout
{
	BYTE bHeader;
	BYTE bEmpire;
	char szText[CHAT_MAX_LEN + 1];
#if defined(__MESSENGER_BLOCK_SYSTEM__)
	char szName[CHARACTER_NAME_MAX_LEN + 1];
#endif
} TPacketGGShout;

typedef struct SPacketGGXmasWarpSanta
{
	BYTE bHeader;
	BYTE bChannel;
	long lMapIndex;
} TPacketGGXmasWarpSanta;

typedef struct SPacketGGXmasWarpSantaReply
{
	BYTE bHeader;
	BYTE bChannel;
} TPacketGGXmasWarpSantaReply;

typedef struct SPacketGGMessenger
{
	BYTE bHeader;
	char szAccount[CHARACTER_NAME_MAX_LEN + 1];
	char szCompanion[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGGMessenger;

typedef struct SPacketGGFindPosition
{
	BYTE header;
	DWORD dwFromPID; // 저 위치로 워프하려는 사람
	DWORD dwTargetPID; // 찾는 사람
} TPacketGGFindPosition;

typedef struct SPacketGGWarpCharacter
{
	BYTE header;
	DWORD pid;
	long x;
	long y;
} TPacketGGWarpCharacter;

//HEADER_GG_GUILD_WAR_ZONE_MAP_INDEX = 15,

typedef struct SPacketGGGuildWarMapIndex
{
	BYTE bHeader;
	DWORD dwGuildID1;
	DWORD dwGuildID2;
	long lMapIndex;
} TPacketGGGuildWarMapIndex;

typedef struct SPacketGGTransfer
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	long lX, lY;
} TPacketGGTransfer;

typedef struct SPacketGGLoginPing
{
	BYTE bHeader;
	char szLogin[LOGIN_MAX_LEN + 1];
} TPacketGGLoginPing;

typedef struct SPacketGGBlockChat
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	long lBlockDuration;
} TPacketGGBlockChat;

/* 클라이언트 측에서 보내는 패킷 */
typedef struct command_text
{
	BYTE bHeader;
} TPacketCGText;

typedef struct command_phase
{
	BYTE bHeader;
} TPacketCGPhase;

/* 로그인 (1) */
typedef struct command_handshake
{
	BYTE bHeader;
	DWORD dwHandshake;
	DWORD dwTime;
	long lDelta;
} TPacketCGHandshake;

typedef struct command_login
{
	BYTE header;
	char login[LOGIN_MAX_LEN + 1];
	char passwd[PASSWD_MAX_LEN + 1];
} TPacketCGLogin;

typedef struct command_login2
{
	BYTE header;
	char login[LOGIN_MAX_LEN + 1];
	DWORD dwLoginKey;
	DWORD adwClientKey[4];
} TPacketCGLogin2;

typedef struct command_login3
{
	BYTE header;
	char login[LOGIN_MAX_LEN + 1];
	char passwd[PASSWD_MAX_LEN + 1];
	DWORD adwClientKey[4];
	char hwid[HWID_MAX_NUM + 1];
	BYTE bLanguage;
} TPacketCGLogin3;

typedef struct packet_login_key
{
	BYTE bHeader;
	DWORD dwLoginKey;
} TPacketGCLoginKey;

typedef struct command_player_select
{
	BYTE header;
	BYTE index;
} TPacketCGPlayerSelect;

typedef struct command_player_delete
{
	BYTE header;
	BYTE index;
	char private_code[8];
} TPacketCGPlayerDelete;

typedef struct command_player_create
{
	BYTE header;
	BYTE index;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	WORD job;
	BYTE shape;
	BYTE Con;
	BYTE Int;
	BYTE Str;
	BYTE Dex;
	char pin[PIN_CODE_LENGTH + 1];
} TPacketCGPlayerCreate;

typedef struct command_player_create_success
{
	BYTE header;
	BYTE bAccountCharacterIndex;
	TSimplePlayer player;
} TPacketGCPlayerCreateSuccess;

// 공격
typedef struct command_attack
{
	BYTE bHeader;
	BYTE bType;
	DWORD dwVID;
	BYTE bCRCMagicCubeProcPiece;
	BYTE bCRCMagicCubeFilePiece;
} TPacketCGAttack;

enum EMoveFuncType
{
	FUNC_WAIT,
	FUNC_MOVE,
	FUNC_ATTACK,
	FUNC_COMBO,
	FUNC_MOB_SKILL,
	_FUNC_SKILL,
	FUNC_MAX_NUM,
	FUNC_SKILL = 0x80,
};

// 이동
typedef struct command_move
{
	BYTE bHeader;
	BYTE bFunc;
	BYTE bArg;
	BYTE bRot;
	long lX;
	long lY;
	DWORD dwTime;
} TPacketCGMove;

typedef struct command_sync_position_element
{
	DWORD dwVID;
	long lX;
	long lY;
} TPacketCGSyncPositionElement;

// 위치 동기화
typedef struct command_sync_position // 가변 패킷
{
	BYTE bHeader;
	WORD wSize;
} TPacketCGSyncPosition;

/* 채팅 (3) */
typedef struct command_chat // 가변 패킷
{
	BYTE header;
	WORD size;
	BYTE type;
} TPacketCGChat;

/* 귓속말 */
typedef struct command_whisper
{
	BYTE bHeader;
	WORD wSize;
	char szNameTo[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisper;

typedef struct command_entergame
{
	BYTE header;
} TPacketCGEnterGame;

typedef struct command_item_use
{
	BYTE header;
	TItemPos Cell;
} TPacketCGItemUse;

typedef struct command_item_use_to_item
{
	BYTE header;
	TItemPos Cell;
	TItemPos TargetCell;
} TPacketCGItemUseToItem;

typedef struct command_item_drop
{
	BYTE header;
	TItemPos Cell;
	DWORD gold;
#if defined(__CHEQUE_SYSTEM__)
	DWORD cheque;
#endif
} TPacketCGItemDrop;

typedef struct command_item_drop2
{
	BYTE header;
	TItemPos Cell;
	DWORD gold;
#if defined(__CHEQUE_SYSTEM__)
	DWORD cheque;
#endif
	WORD count;
} TPacketCGItemDrop2;

#if defined(__NEW_DROP_DIALOG__)
typedef struct command_item_destroy
{
	BYTE header;
	TItemPos Cell;
} TPacketCGItemDestroy;
#endif

typedef struct command_item_move
{
	BYTE header;
	TItemPos Cell;
	TItemPos CellTo;
	WORD count;
} TPacketCGItemMove;

typedef struct command_item_pickup
{
	BYTE header;
	DWORD vid;
} TPacketCGItemPickup;

typedef struct command_quickslot_add
{
	BYTE header;
	BYTE pos;
	TQuickslot slot;
} TPacketCGQuickslotAdd;

typedef struct command_quickslot_del
{
	BYTE header;
	BYTE pos;
} TPacketCGQuickslotDel;

typedef struct command_quickslot_swap
{
	BYTE header;
	BYTE pos;
	BYTE change_pos;
} TPacketCGQuickslotSwap;

enum
{
	SHOP_SUBHEADER_CG_END,
	SHOP_SUBHEADER_CG_BUY,
	SHOP_SUBHEADER_CG_SELL,
	SHOP_SUBHEADER_CG_SELL2,
};

typedef struct command_shop_buy
{
	BYTE count;
} TPacketCGShopBuy;

typedef struct command_shop_sell
{
	WORD wPos;
	WORD wCount;
	BYTE bType;
} TPacketCGShopSell;

typedef struct command_shop
{
	BYTE header;
	WORD subheader;
} TPacketCGShop;

typedef struct command_on_click
{
	BYTE header;
	DWORD vid;
} TPacketCGOnClick;

enum
{
	EXCHANGE_SUBHEADER_CG_START, /* arg1 == vid of target character */
	EXCHANGE_SUBHEADER_CG_ITEM_ADD, /* arg1 == position of item */
	EXCHANGE_SUBHEADER_CG_ITEM_DEL, /* arg1 == position of item */
	EXCHANGE_SUBHEADER_CG_ELK_ADD, /* arg1 == amount of gold */
#if defined(__CHEQUE_SYSTEM__)
	EXCHANGE_SUBHEADER_CG_CHEQUE_ADD, /* arg1 == amount of cheque */
#endif
	EXCHANGE_SUBHEADER_CG_ACCEPT, /* arg1 == not used */
	EXCHANGE_SUBHEADER_CG_CANCEL, /* arg1 == not used */
};

typedef struct command_exchange
{
	BYTE header;
	BYTE sub_header;
	DWORD arg1;
	BYTE arg2;
	TItemPos Pos;
} TPacketCGExchange;

typedef struct command_position
{
	BYTE header;
	BYTE position;
} TPacketCGPosition;

typedef struct command_script_answer
{
	BYTE header;
	BYTE answer;
	//char file[32 + 1];
	//BYTE answer[16 + 1];
} TPacketCGScriptAnswer;

typedef struct command_script_button
{
	BYTE header;
	unsigned int idx;
} TPacketCGScriptButton;

typedef struct command_quest_input_string
{
	BYTE header;
	char msg[64 + 1];
} TPacketCGQuestInputString;

typedef struct command_quest_confirm
{
	BYTE header;
	BYTE answer;
	DWORD requestPID;
} TPacketCGQuestConfirm;

/*
* 서버 측에서 보내는 패킷
*/
typedef struct packet_quest_confirm
{
	BYTE header;
	char msg[128 + 1];
	long timeout;
	DWORD requestPID;
} TPacketGCQuestConfirm;

typedef struct packet_handshake
{
	BYTE bHeader;
	DWORD dwHandshake;
	DWORD dwTime;
	long lDelta;
} TPacketGCHandshake;

enum EPhase
{
	PHASE_CLOSE,
	PHASE_HANDSHAKE,
	PHASE_LOGIN,
	PHASE_SELECT,
	PHASE_LOADING,
	PHASE_GAME,
	PHASE_DEAD,

	PHASE_CLIENT_CONNECTING,
	PHASE_DBCLIENT,
	PHASE_P2P,
	PHASE_AUTH,
};

typedef struct packet_phase
{
	BYTE header;
	BYTE phase;
} TPacketGCPhase;

typedef struct packet_bindudp
{
	BYTE header;
	DWORD addr;
	WORD port;
} TPacketGCBindUDP;

enum
{
	LOGIN_FAILURE_ALREADY = 1,
	LOGIN_FAILURE_ID_NOT_EXIST = 2,
	LOGIN_FAILURE_WRONG_PASS = 3,
	LOGIN_FAILURE_FALSE = 4,
	LOGIN_FAILURE_NOT_TESTOR = 5,
	LOGIN_FAILURE_NOT_TEST_TIME = 6,
	LOGIN_FAILURE_FULL = 7
};

typedef struct packet_login_success
{
	BYTE bHeader;
	TSimplePlayer players[PLAYER_PER_ACCOUNT];
	DWORD guild_id[PLAYER_PER_ACCOUNT];
	char guild_name[PLAYER_PER_ACCOUNT][GUILD_NAME_MAX_LEN + 1];

	DWORD handle;
	DWORD random_key;
} TPacketGCLoginSuccess;

typedef struct packet_auth_success
{
	BYTE bHeader;
	DWORD dwLoginKey;
	BYTE bResult;
	BOOL bState;
} TPacketGCAuthSuccess;

typedef struct packet_login_failure
{
	BYTE header;
	char szStatus[ACCOUNT_STATUS_MAX_LEN + 1];
} TPacketGCLoginFailure;

typedef struct packet_create_failure
{
	BYTE header;
	BYTE bType;
} TPacketGCCreateFailure;

enum
{
	ADD_CHARACTER_STATE_DEAD = (1 << 0),
	ADD_CHARACTER_STATE_SPAWN = (1 << 1),
	ADD_CHARACTER_STATE_GUNGON = (1 << 2),
	ADD_CHARACTER_STATE_KILLER = (1 << 3),
	ADD_CHARACTER_STATE_PARTY = (1 << 4),
};

enum ECharacterEquipmentPart
{
	CHR_EQUIPPART_ARMOR,
	CHR_EQUIPPART_WEAPON,
	CHR_EQUIPPART_HEAD,
	CHR_EQUIPPART_HAIR,
#if defined(__ACCE_COSTUME_SYSTEM__)
	CHR_EQUIPPART_ACCE,
#endif

	CHR_EQUIPPART_NUM,
};

typedef struct packet_add_char
{
	BYTE header;
	DWORD dwVID;

#if defined(__WJ_SHOW_MOB_INFO__)
	DWORD dwLevel;
	DWORD dwAIFlag;
#endif

	float angle;
	long x;
	long y;
	long z;

	BYTE bType;
	WORD wRaceNum;
	BYTE bMovingSpeed;
	BYTE bAttackSpeed;

	BYTE bStateFlag;
	DWORD dwAffectFlag[2]; // 효과
} TPacketGCCharacterAdd;

typedef struct packet_char_additional_info
{
	BYTE header;
	DWORD dwVID;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	WORD awPart[CHR_EQUIPPART_NUM];
	BYTE bEmpire;
	DWORD dwGuildID;
	DWORD dwLevel;
#if defined(__CONQUEROR_LEVEL__)
	DWORD dwConquerorLevel;
#endif
	short sAlignment;
	BYTE bPKMode;
	DWORD dwMountVnum;
#if defined(__QUIVER_SYSTEM__)
	DWORD dwArrow;
#endif
#if defined(__GUILD_LEADER_GRADE_NAME__)
	BYTE bGuildLeaderGrade;
#endif
#if defined(__GENDER_ALIGNMENT__)
	BYTE bJob;
#endif
#if defined(__SKILL_COLOR_SYSTEM__)
	DWORD dwSkillColor[ESkillColorLength::MAX_SKILL_COUNT + ESkillColorLength::MAX_BUFF_COUNT][ESkillColorLength::MAX_EFFECT_COUNT];
#endif
	BYTE bLanguage;
} TPacketGCCharacterAdditionalInfo;

/*
typedef struct packet_update_char_old
{
	BYTE header;
	DWORD dwVID;

	WORD awPart[CHR_EQUIPPART_NUM];
	BYTE bMovingSpeed;
	BYTE bAttackSpeed;

	BYTE bStateFlag;
	DWORD dwAffectFlag[2];

	DWORD dwGuildID;
	short sAlignment;
	BYTE bPKMode;
	DWORD dwMountVnum;
} TPacketGCCharacterUpdateOld;
*/

typedef struct packet_update_char
{
	BYTE header;
	DWORD dwVID;

	WORD awPart[CHR_EQUIPPART_NUM];
	BYTE bMovingSpeed;
	BYTE bAttackSpeed;

	BYTE bStateFlag;
	DWORD dwAffectFlag[2];

	DWORD dwGuildID;
	short sAlignment;
	DWORD dwLevel;
#if defined(__CONQUEROR_LEVEL__)
	DWORD dwConquerorLevel;
#endif
	BYTE bPKMode;
	DWORD dwMountVnum;
#if defined(__QUIVER_SYSTEM__)
	DWORD dwArrow;
#endif
#if defined(__GUILD_LEADER_GRADE_NAME__)
	BYTE bGuildLeaderGrade;
#endif
	//WORD wRaceNum;
#if defined(__SKILL_COLOR_SYSTEM__)
	DWORD dwSkillColor[ESkillColorLength::MAX_SKILL_COUNT + ESkillColorLength::MAX_BUFF_COUNT][ESkillColorLength::MAX_EFFECT_COUNT];
#endif
	BYTE bLanguage;
} TPacketGCCharacterUpdate;

typedef struct packet_del_char
{
	BYTE header;
	DWORD id;
} TPacketGCCharacterDelete;

typedef struct packet_chat // 가변 패킷
{
	BYTE header;
	WORD size;
	BYTE type;
	DWORD id;
	BYTE bEmpire;
} TPacketGCChat;

typedef struct packet_whisper // 가변 패킷
{
	BYTE bHeader;
	WORD wSize;
	BYTE bType;
	char szNameFrom[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCWhisper;

typedef struct packet_main_character
{
	BYTE header;
	DWORD dwVID;
	WORD wRaceNum;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	long lx, ly, lz;
	BYTE empire;
	BYTE skill_group;
} TPacketGCMainCharacter;

// SUPPORT_BGM
typedef struct packet_main_character3_bgm
{
	enum
	{
		MUSIC_NAME_LEN = 24,
	};

	BYTE header;
	DWORD dwVID;
	WORD wRaceNum;
	char szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char szBGMName[MUSIC_NAME_LEN + 1];
	long lx, ly, lz;
	BYTE empire;
	BYTE skill_group;
} TPacketGCMainCharacter3_BGM;

typedef struct packet_main_character4_bgm_vol
{
	enum
	{
		MUSIC_NAME_LEN = 24,
	};

	BYTE header;
	DWORD dwVID;
	WORD wRaceNum;
	char szChrName[CHARACTER_NAME_MAX_LEN + 1];
	char szBGMName[MUSIC_NAME_LEN + 1];
	float fBGMVol;
	long lx, ly, lz;
	BYTE empire;
	BYTE skill_group;
} TPacketGCMainCharacter4_BGM_VOL;
// END_OF_SUPPORT_BGM

typedef struct packet_points
{
	BYTE header;
	long long points[POINT_MAX_NUM];
} TPacketGCPoints;

typedef struct packet_skill_level
{
	BYTE bHeader;
	TPlayerSkill skills[SKILL_MAX_NUM];
} TPacketGCSkillLevel;

typedef struct packet_point_change
{
	int header;
	DWORD dwVID;
	BYTE type;
	long long amount;
	long long value;
} TPacketGCPointChange;

typedef struct packet_stun
{
	BYTE header;
	DWORD vid;
} TPacketGCStun;

typedef struct packet_dead
{
	BYTE header;
	DWORD vid;
} TPacketGCDead;

typedef struct packet_item_set_empty
{
	BYTE header;
	TItemPos Cell;
	DWORD vnum;
	WORD count;
	long alSockets[ITEM_SOCKET_MAX_NUM];
#if defined(__ITEM_APPLY_RANDOM__)
	TPlayerItemAttribute aApplyRandom[ITEM_APPLY_MAX_NUM];
#endif
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#if defined(__CHANGE_LOOK_SYSTEM__)
	DWORD dwTransmutationVnum;
#endif
} TPacketGCItemSetEmpty;

typedef struct packet_item_set
{
	BYTE header;
	TItemPos Cell;
	DWORD vnum;
	WORD count;
#if defined(__SOUL_BIND_SYSTEM__)
	long soulbind;
#endif
	DWORD flags;
	DWORD anti_flags;
	bool highlight;
	long alSockets[ITEM_SOCKET_MAX_NUM];
#if defined(__ITEM_APPLY_RANDOM__)
	TPlayerItemAttribute aApplyRandom[ITEM_APPLY_MAX_NUM];
#endif
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#if defined(__CHANGE_LOOK_SYSTEM__)
	DWORD dwTransmutationVnum;
#endif
} TPacketGCItemSet;

typedef struct packet_item_del
{
	BYTE header;
	BYTE pos;
} TPacketGCItemDel;

struct packet_item_use
{
	BYTE header;
	TItemPos Cell;
	DWORD ch_vid;
	DWORD victim_vid;
	DWORD vnum;
};

struct packet_item_move
{
	BYTE header;
	TItemPos Cell;
	TItemPos CellTo;
};

typedef struct packet_item_update
{
	BYTE header;
	TItemPos Cell;
	WORD count;
#if defined(__SOUL_BIND_SYSTEM__)
	long soulbind;
#endif
	long alSockets[ITEM_SOCKET_MAX_NUM];
#if defined(__ITEM_APPLY_RANDOM__)
	TPlayerItemAttribute aApplyRandom[ITEM_APPLY_MAX_NUM];
#endif
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#if defined(__CHANGE_LOOK_SYSTEM__)
	DWORD dwTransmutationVnum;
#endif
} TPacketGCItemUpdate;

typedef struct packet_item_ground_add
{
#if defined(__ITEM_DROP_RENEWAL__)
	packet_item_ground_add()
	{
		memset(&alSockets, 0, sizeof(alSockets));
		memset(&aAttrs, 0, sizeof(aAttrs));
	}
#endif

	BYTE bHeader;
	long x, y, z;
	DWORD dwVID;
	DWORD dwVnum;
#if defined(__ITEM_DROP_RENEWAL__)
	long alSockets[ITEM_SOCKET_MAX_NUM]; // 3
	TPlayerItemAttribute aAttrs[ITEM_ATTRIBUTE_MAX_NUM]; // 7
#endif
} TPacketGCItemGroundAdd;

typedef struct packet_item_ownership
{
	BYTE bHeader;
	DWORD dwVID;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCItemOwnership;

typedef struct packet_item_ground_del
{
	BYTE bHeader;
	DWORD dwVID;
} TPacketGCItemGroundDel;

struct packet_quickslot_add
{
	BYTE header;
	BYTE pos;
	TQuickslot slot;
};

struct packet_quickslot_del
{
	BYTE header;
	BYTE pos;
};

struct packet_quickslot_swap
{
	BYTE header;
	BYTE pos;
	BYTE pos_to;
};

struct packet_motion
{
	BYTE header;
	DWORD vid;
	DWORD victim_vid;
	WORD motion;
};

enum EPacketShopSubHeaders
{
	SHOP_SUBHEADER_GC_START,
	SHOP_SUBHEADER_GC_END,
	SHOP_SUBHEADER_GC_UPDATE_ITEM,
	SHOP_SUBHEADER_GC_UPDATE_PRICE,
	SHOP_SUBHEADER_GC_OK,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY,
#if defined(__CHEQUE_SYSTEM__)
	SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE,
#endif
	SHOP_SUBHEADER_GC_SOLDOUT,
	SHOP_SUBHEADER_GC_INVENTORY_FULL,
	SHOP_SUBHEADER_GC_INVALID_POS,
	SHOP_SUBHEADER_GC_SOLD_OUT,
	SHOP_SUBHEADER_GC_START_EX,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX,
#if defined(__CHEQUE_SYSTEM__)
	SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE_EX,
#endif
#if defined(__SHOPEX_RENEWAL__)
	SHOP_SUBHEADER_GC_NOT_ENOUGH_ITEM,
	SHOP_SUBHEADER_GC_NOT_ENOUGH_EXP,
#endif
};

struct packet_shop_item
{
	DWORD vnum;
	DWORD price;
#if defined(__CHEQUE_SYSTEM__)
	DWORD price_cheque;
#endif
	WORD count;
	BYTE display_pos;
	long alSockets[ITEM_SOCKET_MAX_NUM];
#if defined(__ITEM_APPLY_RANDOM__)
	TPlayerItemAttribute aApplyRandom[ITEM_APPLY_MAX_NUM];
#endif
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#if defined(__CHANGE_LOOK_SYSTEM__)
	DWORD dwTransmutationVnum;
#endif
#if defined(__SHOPEX_RENEWAL__)
	DWORD price_type = 1;
	DWORD price_vnum = 0;
	packet_shop_item()
	{
		memset(&alSockets, 0, sizeof(alSockets));
#if defined(__ITEM_APPLY_RANDOM__)
		memset(&aApplyRandom, 0, sizeof(aApplyRandom));
#endif
		memset(&aAttr, 0, sizeof(aAttr));
	}
#endif
};

typedef struct packet_shop_start
{
	DWORD owner_vid;
	struct packet_shop_item	items[SHOP_HOST_ITEM_MAX_NUM];
#if defined(__MYSHOP_DECO__)
	BYTE shop_tab_count;
#endif
} TPacketGCShopStart;

typedef struct packet_shop_start_ex // 다음에 TSubPacketShopTab* shop_tabs 이 따라옴.
{
	typedef struct sub_packet_shop_tab
	{
		char name[SHOP_TAB_NAME_MAX];
		BYTE coin_type;
		packet_shop_item items[SHOP_HOST_ITEM_MAX_NUM];
	} TSubPacketShopTab;
	DWORD owner_vid;
	BYTE shop_tab_count;
} TPacketGCShopStartEx;

typedef struct packet_shop_update_item
{
	BYTE pos;
	struct packet_shop_item item;
} TPacketGCShopUpdateItem;

typedef struct packet_shop_update_price
{
	int iPrice;
#if defined(__CHEQUE_SYSTEM__)
	int iPriceCheque;
#endif
} TPacketGCShopUpdatePrice;

typedef struct packet_shop // 가변 패킷
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCShop;

typedef struct packet_exchange
{
	BYTE header;
	BYTE sub_header;
	BYTE is_me;
	DWORD arg1; // vnum
	TItemPos arg2; // cell
	DWORD arg3; // count
	TItemPos arg4;
	long alSockets[ITEM_SOCKET_MAX_NUM];
#if defined(__ITEM_APPLY_RANDOM__)
	TPlayerItemAttribute aApplyRandom[ITEM_APPLY_MAX_NUM];
#endif
	TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#if defined(__CHANGE_LOOK_SYSTEM__)
	DWORD dwTransmutationVnum;
#endif
} TPacketGCExchange;

enum EPacketTradeSubHeaders
{
	EXCHANGE_SUBHEADER_GC_START, /* arg1 == vid */
	EXCHANGE_SUBHEADER_GC_ITEM_ADD, /* arg1 == vnum arg2 == pos arg3 == count */
	EXCHANGE_SUBHEADER_GC_ITEM_DEL,
	EXCHANGE_SUBHEADER_GC_GOLD_ADD, /* arg1 == gold */
#if defined(__CHEQUE_SYSTEM__)
	EXCHANGE_SUBHEADER_GC_CHEQUE_ADD, /* arg1 == cheque */
#endif
	EXCHANGE_SUBHEADER_GC_ACCEPT, /* arg1 == accept */
	EXCHANGE_SUBHEADER_GC_END, /* arg1 == not used */
	EXCHANGE_SUBHEADER_GC_ALREADY, /* arg1 == not used */
	EXCHANGE_SUBHEADER_GC_LESS_GOLD, /* arg1 == not used */
#if defined(__CHEQUE_SYSTEM__)
	EXCHANGE_SUBHEADER_GC_LESS_CHEQUE, /* arg1 == not used */
#endif
};

struct packet_position
{
	BYTE header;
	DWORD vid;
	BYTE position;
};

typedef struct packet_ping
{
	BYTE header;
} TPacketGCPing;

struct packet_script
{
	BYTE header;
	WORD size;
	BYTE skin;
	WORD src_size;
};

typedef struct packet_change_speed
{
	BYTE header;
	DWORD vid;
	WORD moving_speed;
} TPacketGCChangeSpeed;

struct packet_mount
{
	BYTE header;
	DWORD vid;
	DWORD mount_vid;
	BYTE pos;
	DWORD x, y;
};

typedef struct packet_move
{
	BYTE bHeader;
	BYTE bFunc;
	BYTE bArg;
	BYTE bRot;
	DWORD dwVID;
	long lX;
	long lY;
	DWORD dwTime;
	DWORD dwDuration;
} TPacketGCMove;

// 소유권
typedef struct packet_ownership
{
	BYTE bHeader;
	DWORD dwOwnerVID;
	DWORD dwVictimVID;
} TPacketGCOwnership;

// 위치 동기화 패킷의 bCount 만큼 붙는 단위
typedef struct packet_sync_position_element
{
	DWORD dwVID;
	long lX;
	long lY;
} TPacketGCSyncPositionElement;

// 위치 동기화
typedef struct packet_sync_position // 가변 패킷
{
	BYTE bHeader;
	WORD wSize; // 개수 = (wSize - sizeof(TPacketGCSyncPosition)) / sizeof(TPacketGCSyncPositionElement) 
} TPacketGCSyncPosition;

typedef struct packet_fly
{
	BYTE bHeader;
	BYTE bType;
	DWORD dwStartVID;
	DWORD dwEndVID;
} TPacketGCCreateFly;

typedef struct command_fly_targeting
{
	BYTE bHeader;
	DWORD dwTargetVID;
	long x, y;
} TPacketCGFlyTargeting;

typedef struct packet_fly_targeting
{
	BYTE bHeader;
	DWORD dwShooterVID;
	DWORD dwTargetVID;
	long x, y;
} TPacketGCFlyTargeting;

typedef struct packet_shoot
{
	BYTE bHeader;
	BYTE bType;
} TPacketCGShoot;

typedef struct packet_duel_start
{
	BYTE header;
	WORD wSize; // DWORD가 몇개? 개수 = (wSize - sizeof(TPacketGCPVPList)) / 4
} TPacketGCDuelStart;

enum EPVPModes
{
	PVP_MODE_NONE,
	PVP_MODE_AGREE,
	PVP_MODE_FIGHT,
	PVP_MODE_REVENGE
};

typedef struct packet_pvp
{
	BYTE bHeader;
	DWORD dwVIDSrc;
	DWORD dwVIDDst;
	BYTE bMode; // 0 이면 끔, 1이면 켬
} TPacketGCPVP;

typedef struct command_use_skill
{
	BYTE bHeader;
	DWORD dwVnum;
	DWORD dwVID;
} TPacketCGUseSkill;

typedef struct command_target
{
	BYTE header;
	DWORD dwVID;
} TPacketCGTarget;

typedef struct packet_target
{
	BYTE header;
	DWORD dwVID;
	BYTE bHPPercent;
#if defined(__VIEW_TARGET_DECIMAL_HP__)
	int64_t iMinHP;
	int64_t iMaxHP;
#endif
#if defined(__ELEMENT_SYSTEM__)
	BYTE bElement[MOB_ELEMENT_MAX_NUM];
#endif
#if defined(__SHIP_DEFENSE__)
	bool bAlliance;
#endif
} TPacketGCTarget;

#if defined(__SEND_TARGET_INFO__)
typedef struct packet_target_info
{
	BYTE bHeader;
	BYTE bIndex;
	DWORD dwVID;
	DWORD dwRaceVnum;
	DWORD dwVnum;
	WORD wCount;
} TPacketGCTargetInfo;

typedef struct packet_target_info_load
{
	BYTE header;
	DWORD dwVID;
} TPacketCGTargetInfoLoad;
#endif

typedef struct packet_warp
{
	BYTE bHeader;
	long lX;
	long lY;
	long lAddr;
	WORD wPort;
} TPacketGCWarp;

typedef struct command_warp
{
	BYTE bHeader;
} TPacketCGWarp;

struct packet_quest_info
{
	BYTE header;
	WORD size;
	WORD index;
#if defined(__QUEST_RENEWAL__)
	WORD c_index;
#endif
	BYTE flag;
};

enum
{
#if defined(__MESSENGER_GM__)
	MESSENGER_SUBHEADER_GC_GM_LIST,
	MESSENGER_SUBHEADER_GC_GM_LOGIN,
	MESSENGER_SUBHEADER_GC_GM_LOGOUT,
#endif
#if defined(__MESSENGER_BLOCK_SYSTEM__)
	MESSENGER_SUBHEADER_GC_BLOCK_LIST,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGIN,
	MESSENGER_SUBHEADER_GC_BLOCK_LOGOUT,
	MESSENGER_SUBHEADER_GC_BLOCK_INVITE, //not used
#endif
	MESSENGER_SUBHEADER_GC_LIST,
	MESSENGER_SUBHEADER_GC_LOGIN,
	MESSENGER_SUBHEADER_GC_LOGOUT,
	MESSENGER_SUBHEADER_GC_INVITE,
};

typedef struct packet_messenger
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCMessenger;

typedef struct packet_messenger_guild_list
{
	BYTE connected;
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildList;

typedef struct packet_messenger_guild_login
{
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildLogin;

typedef struct packet_messenger_guild_logout
{
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketGCMessengerGuildLogout;

typedef struct packet_messenger_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerListOffline;

typedef struct packet_messenger_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerListOnline;

#if defined(__MESSENGER_GM__)
typedef struct packet_messenger_gm_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerGMListOffline;

typedef struct packet_messenger_gm_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerGMListOnline;
#endif

#if defined(__MESSENGER_BLOCK_SYSTEM__)
typedef struct packet_messenger_block_list_offline
{
	BYTE connected; // always 0
	BYTE length;
} TPacketGCMessengerBlockListOffline;

typedef struct packet_messenger_block_list_online
{
	BYTE connected; // always 1
	BYTE length;
} TPacketGCMessengerBlockListOnline;
#endif

enum
{
#if defined(__MESSENGER_BLOCK_SYSTEM__)
	MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_VID,
	MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_NAME,
	MESSENGER_SUBHEADER_CG_BLOCK_REMOVE,
#endif
	MESSENGER_SUBHEADER_CG_ADD_BY_VID,
	MESSENGER_SUBHEADER_CG_ADD_BY_NAME,
	MESSENGER_SUBHEADER_CG_REMOVE,
	MESSENGER_SUBHEADER_CG_INVITE_ANSWER,
};

typedef struct command_messenger
{
	BYTE header;
	BYTE subheader;
} TPacketCGMessenger;

typedef struct command_messenger_add_by_vid
{
	DWORD vid;
} TPacketCGMessengerAddByVID;

typedef struct command_messenger_add_by_name
{
	BYTE length;
	//char login[LOGIN_MAX_LEN+1];
} TPacketCGMessengerAddByName;

typedef struct command_messenger_remove
{
	char login[LOGIN_MAX_LEN + 1];
	//DWORD account;
} TPacketCGMessengerRemove;

#if defined(__MESSENGER_BLOCK_SYSTEM__)
typedef struct command_messenger_add_block_by_vid
{
	DWORD vid;
} TPacketCGMessengerAddBlockByVID;

typedef struct command_messenger_add_block_by_name
{
	BYTE length;
} TPacketCGMessengerAddBlockByName;

typedef struct command_messenger_remove_block
{
	char login[LOGIN_MAX_LEN + 1];
} TPacketCGMessengerRemoveBlock;
#endif

typedef struct command_safebox_checkout
{
	BYTE bHeader;
	BYTE bSafePos;
	TItemPos ItemPos;
} TPacketCGSafeboxCheckout;

typedef struct command_safebox_checkin
{
	BYTE bHeader;
	BYTE bSafePos;
	TItemPos ItemPos;
} TPacketCGSafeboxCheckin;

///////////////////////////////////////////////////////////////////////////////////
// Party
typedef struct command_party_parameter
{
	BYTE bHeader;
	BYTE bDistributeMode;
} TPacketCGPartyParameter;

typedef struct paryt_parameter
{
	BYTE bHeader;
	BYTE bDistributeMode;
} TPacketGCPartyParameter;

#if defined(__WJ_SHOW_PARTY_ON_MINIMAP__)
struct TPartyPosition
{
	DWORD dwPID;
	long lX;
	long lY;
	float fRot;
};

typedef struct SPacketGCPartyPosition
{
	BYTE bHeader;
	WORD wSize;
} TPacketGCPartyPosition;
#endif

typedef struct packet_party_add
{
	BYTE header;
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN + 1];
#if defined(__WJ_SHOW_PARTY_ON_MINIMAP__)
	long mapIdx;
	BYTE channel;
#endif
} TPacketGCPartyAdd;

typedef struct command_party_invite
{
	BYTE header;
	DWORD vid;
} TPacketCGPartyInvite;

typedef struct packet_party_invite
{
	BYTE header;
	DWORD leader_vid;
} TPacketGCPartyInvite;

typedef struct command_party_invite_answer
{
	BYTE header;
	DWORD leader_vid;
	BYTE accept;
} TPacketCGPartyInviteAnswer;

typedef struct packet_party_update
{
	BYTE header;
	DWORD pid;
	BYTE role;
	BYTE percent_hp;
	short affects[7];
} TPacketGCPartyUpdate;

typedef struct packet_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketGCPartyRemove;

typedef struct packet_party_link
{
	BYTE header;
	DWORD pid;
	DWORD vid;
#if defined(__WJ_SHOW_PARTY_ON_MINIMAP__)
	long mapIdx;
	BYTE channel;
#endif
} TPacketGCPartyLink;

typedef struct packet_party_unlink
{
	BYTE header;
	DWORD pid;
	DWORD vid;
} TPacketGCPartyUnlink;

typedef struct command_party_remove
{
	BYTE header;
	DWORD pid;
} TPacketCGPartyRemove;

typedef struct command_party_set_state
{
	BYTE header;
	DWORD pid;
	BYTE byRole;
	BYTE flag;
} TPacketCGPartySetState;

enum
{
	PARTY_SKILL_HEAL = 1,
	PARTY_SKILL_WARP = 2
};

typedef struct command_party_use_skill
{
	BYTE header;
	BYTE bySkillIndex;
	DWORD vid;
} TPacketCGPartyUseSkill;

typedef struct packet_safebox_size
{
	BYTE bHeader;
	BYTE bSize;
} TPacketCGSafeboxSize;

typedef struct packet_safebox_wrong_password
{
	BYTE bHeader;
} TPacketCGSafeboxWrongPassword;

typedef struct command_empire
{
	BYTE bHeader;
	BYTE bEmpire;
} TPacketCGEmpire;

typedef struct packet_empire
{
	BYTE bHeader;
	BYTE bEmpire;
} TPacketGCEmpire;

enum
{
	SAFEBOX_MONEY_STATE_SAVE,
	SAFEBOX_MONEY_STATE_WITHDRAW,
};

typedef struct command_safebox_money
{
	BYTE bHeader;
	BYTE bState;
	long lMoney;
} TPacketCGSafeboxMoney;

typedef struct packet_safebox_money_change
{
	BYTE bHeader;
	long lMoney;
} TPacketGCSafeboxMoneyChange;

// Guild
enum
{
	GUILD_SUBHEADER_GC_LOGIN,
	GUILD_SUBHEADER_GC_LOGOUT,
	GUILD_SUBHEADER_GC_LIST,
	GUILD_SUBHEADER_GC_GRADE,
	GUILD_SUBHEADER_GC_ADD,
	GUILD_SUBHEADER_GC_REMOVE,
	GUILD_SUBHEADER_GC_GRADE_NAME,
	GUILD_SUBHEADER_GC_GRADE_AUTH,
	GUILD_SUBHEADER_GC_INFO,
	GUILD_SUBHEADER_GC_COMMENTS,
	GUILD_SUBHEADER_GC_CHANGE_EXP,
	GUILD_SUBHEADER_GC_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_GC_SKILL_INFO,
	GUILD_SUBHEADER_GC_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_GC_GUILD_INVITE,
	GUILD_SUBHEADER_GC_WAR,
	GUILD_SUBHEADER_GC_GUILD_NAME,
	GUILD_SUBHEADER_GC_GUILD_WAR_LIST,
	GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST,
	GUILD_SUBHEADER_GC_WAR_SCORE,
	GUILD_SUBHEADER_GC_MONEY_CHANGE,
};

enum GUILD_SUBHEADER_CG
{
	GUILD_SUBHEADER_CG_ADD_MEMBER,
	GUILD_SUBHEADER_CG_REMOVE_MEMBER,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME,
	GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY,
	GUILD_SUBHEADER_CG_OFFER,
	GUILD_SUBHEADER_CG_POST_COMMENT,
	GUILD_SUBHEADER_CG_DELETE_COMMENT,
	GUILD_SUBHEADER_CG_REFRESH_COMMENT,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE,
	GUILD_SUBHEADER_CG_USE_SKILL,
	GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL,
	GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER,
	GUILD_SUBHEADER_CG_CHARGE_GSP,
	GUILD_SUBHEADER_CG_DEPOSIT_MONEY,
	GUILD_SUBHEADER_CG_WITHDRAW_MONEY,
};

typedef struct packet_guild
{
	BYTE header;
	WORD size;
	BYTE subheader;
} TPacketGCGuild;

typedef struct packet_guild_name_t
{
	BYTE header;
	WORD size;
	BYTE subheader;
	DWORD guildID;
	char guildName[GUILD_NAME_MAX_LEN];
} TPacketGCGuildName;

typedef struct packet_guild_war
{
	DWORD dwGuildSelf;
	DWORD dwGuildOpp;
	BYTE bType;
	BYTE bWarState;
} TPacketGCGuildWar;

typedef struct command_guild
{
	BYTE header;
	BYTE subheader;
} TPacketCGGuild;

typedef struct command_guild_answer_make_guild
{
	BYTE header;
	char guild_name[GUILD_NAME_MAX_LEN + 1];
} TPacketCGAnswerMakeGuild;

typedef struct command_guild_use_skill
{
	DWORD dwVnum;
	DWORD dwPID;
} TPacketCGGuildUseSkill;

// Guild Mark
typedef struct command_mark_login
{
	BYTE header;
	DWORD handle;
	DWORD random_key;
} TPacketCGMarkLogin;

typedef struct command_mark_upload
{
	BYTE header;
	DWORD gid;
	BYTE image[16 * 12 * 4];
} TPacketCGMarkUpload;

typedef struct command_mark_idxlist
{
	BYTE header;
} TPacketCGMarkIDXList;

typedef struct command_mark_crclist
{
	BYTE header;
	BYTE imgIdx;
	DWORD crclist[80];
} TPacketCGMarkCRCList;

typedef struct packet_mark_idxlist
{
	BYTE header;
	DWORD bufSize;
	WORD count;
	// 뒤에 size * (WORD + WORD)만큼 데이터 붙음
} TPacketGCMarkIDXList;

typedef struct packet_mark_block
{
	BYTE header;
	DWORD bufSize;
	BYTE imgIdx;
	DWORD count;
	// 뒤에 64 x 48 x 픽셀크기(4바이트) = 12288만큼 데이터 붙음
} TPacketGCMarkBlock;

typedef struct command_symbol_upload
{
	BYTE header;
	WORD size;
	DWORD guild_id;
} TPacketCGGuildSymbolUpload;

typedef struct command_symbol_crc
{
	BYTE header;
	DWORD guild_id;
	DWORD crc;
	DWORD size;
} TPacketCGSymbolCRC;

typedef struct packet_symbol_data
{
	BYTE header;
	WORD size;
	DWORD guild_id;
} TPacketGCGuildSymbolData;

// Fishing
typedef struct command_fishing
{
	BYTE header;
	BYTE dir;
} TPacketCGFishing;

typedef struct packet_fishing
{
	BYTE header;
	BYTE subheader;
	DWORD info;
	BYTE dir;
} TPacketGCFishing;

enum
{
	FISHING_SUBHEADER_GC_START,
	FISHING_SUBHEADER_GC_STOP,
	FISHING_SUBHEADER_GC_REACT,
	FISHING_SUBHEADER_GC_SUCCESS,
	FISHING_SUBHEADER_GC_FAIL,
	FISHING_SUBHEADER_GC_FISH,
};

typedef struct command_give_item
{
	BYTE byHeader;
	DWORD dwTargetVID;
	TItemPos ItemPos;
	WORD wItemCount;
} TPacketCGGiveItem;

typedef struct SPacketCGHack
{
	BYTE bHeader;
	char szBuf[255 + 1];
} TPacketCGHack;

// SubHeader - Dungeon
enum
{
	DUNGEON_SUBHEADER_GC_TIME_ATTACK_START = 0,
	DUNGEON_SUBHEADER_GC_DESTINATION_POSITION = 1,
};

typedef struct packet_dungeon
{
	BYTE bHeader;
	WORD size;
	BYTE subheader;
} TPacketGCDungeon;

typedef struct packet_dungeon_dest_position
{
	long x;
	long y;
} TPacketGCDungeonDestPosition;

typedef struct SPacketGCShopSign
{
	BYTE bHeader;
	DWORD dwVID;
	char szSign[SHOP_SIGN_MAX_LEN + 1];
#if defined(__MYSHOP_DECO__)
	BYTE bType;
#endif
} TPacketGCShopSign;

typedef struct SPacketCGMyShop
{
	BYTE bHeader;
	char szSign[SHOP_SIGN_MAX_LEN + 1];
	WORD wCount;
} TPacketCGMyShop;

#if defined(__MYSHOP_DECO__)
typedef struct SPacketCGMyShopDeco
{
	BYTE bHeader;
	BYTE bType;
	DWORD dwPolyVnum;
} TPacketCGMyShopDeco;
#endif

typedef struct SPacketGCTime
{
	BYTE bHeader;
	time_t time;
} TPacketGCTime;

enum
{
	WALKMODE_RUN,
	WALKMODE_WALK,
};

typedef struct SPacketGCWalkMode
{
	BYTE header;
	DWORD vid;
	BYTE mode;
} TPacketGCWalkMode;

typedef struct SPacketGCChangeSkillGroup
{
	BYTE header;
	BYTE skill_group;
} TPacketGCChangeSkillGroup;

typedef struct SPacketCGRefine
{
	BYTE header;
	BYTE pos;
	BYTE type;
} TPacketCGRefine;

typedef struct SPacketCGRequestRefineInfo
{
	BYTE header;
	BYTE pos;
} TPacketCGRequestRefineInfo;

typedef struct SPacketGCRefineInformaion
{
	BYTE header;
	BYTE type;
	BYTE pos;
	DWORD src_vnum;
	DWORD result_vnum;
	WORD material_count;
	int cost; // 소요 비용
	int prob; // 확률
	TRefineMaterial materials[REFINE_MATERIAL_MAX_NUM];
#if defined(__ITEM_APPLY_RANDOM__)
	TPlayerItemAttribute aApplyRandom[ITEM_APPLY_MAX_NUM];
#endif
} TPacketGCRefineInformation;

struct TNPCPosition
{
	BYTE bType;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	long x;
	long y;
};

typedef struct SPacketGCNPCPosition
{
	BYTE header;
	WORD size;
	WORD count;
	// array of TNPCPosition
} TPacketGCNPCPosition;

typedef struct SPacketGCSpecialEffect
{
	BYTE header;
	BYTE type;
	DWORD vid;
} TPacketGCSpecialEffect;

typedef struct SPacketCGChangeName
{
	BYTE header;
	BYTE index;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGChangeName;

typedef struct SPacketGCChangeName
{
	BYTE header;
	DWORD pid;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketGCChangeName;

typedef struct command_client_version
{
	BYTE header;
	char filename[32 + 1];
	char timestamp[32 + 1];
} TPacketCGClientVersion;

typedef struct command_client_version2
{
	BYTE header;
	char filename[32 + 1];
	char timestamp[32 + 1];
} TPacketCGClientVersion2;

typedef struct packet_channel
{
	BYTE header;
	BYTE channel;
} TPacketGCChannel;

typedef struct pakcet_view_equip
{
	BYTE header;
	DWORD vid;
	struct {
		DWORD vnum;
		WORD count;
		long alSockets[ITEM_SOCKET_MAX_NUM];
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#if defined(__CHANGE_LOOK_SYSTEM__)
		DWORD dwTransmutationVnum;
#endif
	} equips[WEAR_MAX_NUM];
} TPacketViewEquip;

typedef struct
{
	DWORD dwID;
	long x, y;
	long width, height;
	DWORD dwGuildID;
} TLandPacketElement;

typedef struct packet_land_list
{
	BYTE header;
	WORD size;
} TPacketGCLandList;

typedef struct
{
	BYTE bHeader;
	long lID;
	char szName[32 + 1];
	DWORD dwVID;
	BYTE bType;
} TPacketGCTargetCreate;

typedef struct
{
	BYTE bHeader;
	long lID;
	long lX, lY;
#if defined(__PRIVATESHOP_SEARCH_SYSTEM__)
	bool bIsShopSearch;
#endif
} TPacketGCTargetUpdate;

typedef struct
{
	BYTE bHeader;
	long lID;
} TPacketGCTargetDelete;

typedef struct
{
	BYTE bHeader;
	TPacketAffectElement elem;
} TPacketGCAffectAdd;

typedef struct
{
	BYTE bHeader;
	DWORD dwType;
	BYTE bApplyOn;
} TPacketGCAffectRemove;

typedef struct packet_lover_info
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	BYTE love_point;
} TPacketGCLoverInfo;

typedef struct packet_love_point_update
{
	BYTE header;
	BYTE love_point;
} TPacketGCLovePointUpdate;

// MINING
typedef struct packet_dig_motion
{
	BYTE header;
	DWORD vid;
	DWORD target_vid;
	BYTE count;
} TPacketGCDigMotion;
// END_OF_MINING

// SCRIPT_SELECT_ITEM
typedef struct command_script_select_item
{
	BYTE header;
	DWORD selection;
} TPacketCGScriptSelectItem;
// END_OF_SCRIPT_SELECT_ITEM

typedef struct packet_damage_info
{
	BYTE header;
	DWORD dwVID;
	BYTE flag;
	int damage;
} TPacketGCDamageInfo;

typedef struct tag_GGSiege
{
	BYTE bHeader;
	BYTE bEmpire;
	BYTE bTowerCount;
} TPacketGGSiege;

typedef struct SPacketGGMonarchTransfer
{
	BYTE bHeader;
	DWORD dwTargetPID;
	long x;
	long y;
} TPacketMonarchGGTransfer;

typedef struct SPacketGGPCBangUpdate
{
	BYTE bHeader;
	unsigned long ulPCBangID;
} TPacketPCBangUpdate;

typedef struct SPacketGGCheckAwakeness
{
	BYTE bHeader;
} TPacketGGCheckAwakeness;

typedef struct SPacketGCPanamaPack
{
	BYTE bHeader;
	char szPackName[256];
	BYTE abIV[32];
} TPacketGCPanamaPack;

// TODO : 아우 짱나..가변패킷 사이즈 받아들일수 있게 패킷 핸들러 Refactoring 하자. 
typedef struct SPacketGCHybridCryptKeys
{
	SPacketGCHybridCryptKeys() : m_pStream(NULL) {}
	~SPacketGCHybridCryptKeys()
	{
		// GCC 에선 NULL delete 해도 괜찮나? 일단 안전하게 NULL 체크 하자. ( 근데 이거 C++ 표준아니었나 --a )
		if (m_pStream)
		{
			delete[] m_pStream;
			m_pStream = NULL;
		}
	}

	DWORD GetStreamSize()
	{
		return sizeof(bHeader) + sizeof(WORD) + sizeof(int) + KeyStreamLen;
	}

	BYTE* GetStreamData()
	{
		if (m_pStream)
			delete[] m_pStream;

		uDynamicPacketSize = (WORD)GetStreamSize();

		m_pStream = new BYTE[uDynamicPacketSize];

		memcpy(m_pStream, &bHeader, 1);
		memcpy(m_pStream + 1, &uDynamicPacketSize, 2);
		memcpy(m_pStream + 3, &KeyStreamLen, 4);

		if (KeyStreamLen > 0)
			memcpy(m_pStream + 7, pDataKeyStream, KeyStreamLen);

		return m_pStream;
	}

	BYTE bHeader;
	WORD uDynamicPacketSize; // 빌어먹을 클라 DynamicPacketHeader 구조때문에 맞춰줘야한다 -_-;
	int KeyStreamLen;
	BYTE* pDataKeyStream;

private:
	BYTE* m_pStream;

} TPacketGCHybridCryptKeys;

typedef struct SPacketGCPackageSDB
{
	SPacketGCPackageSDB() : m_pDataSDBStream(NULL), m_pStream(NULL) {}
	~SPacketGCPackageSDB()
	{
		if (m_pStream)
		{
			delete[] m_pStream;
			m_pStream = NULL;
		}
	}

	DWORD GetStreamSize()
	{
		return sizeof(bHeader) + sizeof(WORD) + sizeof(int) + iStreamLen;
	}

	BYTE* GetStreamData()
	{
		if (m_pStream)
			delete[] m_pStream;

		uDynamicPacketSize = static_cast<WORD>(GetStreamSize());

		m_pStream = new BYTE[uDynamicPacketSize];

		memcpy(m_pStream, &bHeader, 1);
		memcpy(m_pStream + 1, &uDynamicPacketSize, 2);
		memcpy(m_pStream + 3, &iStreamLen, 4);

		if (iStreamLen > 0)
			memcpy(m_pStream + 7, m_pDataSDBStream, iStreamLen);

		return m_pStream;
	}

	BYTE bHeader;
	WORD uDynamicPacketSize; // 빌어먹을 클라 DynamicPacketHeader 구조때문에 맞춰줘야한다 -_-;
	int iStreamLen;
	BYTE* m_pDataSDBStream;

private:
	BYTE* m_pStream;

} TPacketGCPackageSDB;

#if defined(__IMPROVED_PACKET_ENCRYPTION__)
struct TPacketKeyAgreement
{
	static const int MAX_DATA_LEN = 256;
	BYTE bHeader;
	WORD wAgreedLength;
	WORD wDataLength;
	BYTE data[MAX_DATA_LEN];
};

struct TPacketKeyAgreementCompleted
{
	BYTE bHeader;
	BYTE data[3]; // dummy (not used)
};
#endif

#define MAX_EFFECT_FILE_NAME 128
typedef struct SPacketGCSpecificEffect
{
	BYTE header;
	DWORD vid;
	char effect_file[MAX_EFFECT_FILE_NAME];
} TPacketGCSpecificEffect;

// 용혼석
enum EDragonSoulRefineWindowRefineType
{
	DragonSoulRefineWindow_UPGRADE,
	DragonSoulRefineWindow_IMPROVEMENT,
	DragonSoulRefineWindow_REFINE,
};

enum EPacketCGDragonSoulSubHeaderType
{
	DS_SUB_HEADER_OPEN,
	DS_SUB_HEADER_CLOSE,
	DS_SUB_HEADER_DO_REFINE_GRADE,
	DS_SUB_HEADER_DO_REFINE_STEP,
	DS_SUB_HEADER_DO_REFINE_STRENGTH,
	DS_SUB_HEADER_REFINE_FAIL,
	DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE,
	DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY,
	DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL,
	DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL,
	DS_SUB_HEADER_REFINE_SUCCEED,
#if defined(__DS_CHANGE_ATTR__)
	DS_SUB_HEADER_OPEN_CHANGE_ATTR,
	DS_SUB_HEADER_DO_CHANGE_ATTR,
#endif
};

typedef struct SPacketCGDragonSoulRefine
{
	SPacketCGDragonSoulRefine() : header(HEADER_CG_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos ItemGrid[DRAGON_SOUL_REFINE_GRID_SIZE];
} TPacketCGDragonSoulRefine;

typedef struct SPacketGCDragonSoulRefine
{
	SPacketGCDragonSoulRefine() : header(HEADER_GC_DRAGON_SOUL_REFINE)
	{}
	BYTE header;
	BYTE bSubType;
	TItemPos Pos;
} TPacketGCDragonSoulRefine;

typedef struct SPacketCGStateCheck
{
	BYTE header;
	unsigned long key;
	unsigned long index;
} TPacketCGStateCheck;

typedef struct SPacketGCStateCheck
{
	BYTE header;
	unsigned long key;
	unsigned long index;
	unsigned char state;
} TPacketGCStateCheck;

#if defined(__ACCE_COSTUME_SYSTEM__)
enum
{
	ACCE_SUBHEADER_GC_OPEN = 0,
	ACCE_SUBHEADER_GC_CLOSE,
	ACCE_SUBHEADER_GC_ADDED,
	ACCE_SUBHEADER_GC_REMOVED,
	ACCE_SUBHEADER_CG_REFINED,
	ACCE_SUBHEADER_CG_CLOSE = 0,
	ACCE_SUBHEADER_CG_ADD,
	ACCE_SUBHEADER_CG_REMOVE,
	ACCE_SUBHEADER_CG_REFINE,
};

typedef struct SPacketAcce
{
	BYTE header;
	BYTE subheader;
	bool bWindow;
	DWORD dwPrice;
	BYTE bPos;
	TItemPos tPos;
	DWORD dwItemVnum;
	DWORD dwMinAbs;
	DWORD dwMaxAbs;
} TPacketAcce;
#endif

#if defined(__SKILL_COLOR_SYSTEM__)
typedef struct packet_skill_color
{
	BYTE bHeader;
	BYTE bSkillSlot;
	DWORD dwCol1;
	DWORD dwCol2;
	DWORD dwCol3;
	DWORD dwCol4;
	DWORD dwCol5;
} TPacketCGSkillColor;
#endif

#if defined(__MINI_GAME_CATCH_KING__)
enum
{
	SUBHEADER_GC_CATCH_KING_EVENT_INFO,
	SUBHEADER_GC_CATCH_KING_START,
	SUBHEADER_GC_CATCH_KING_SET_CARD,
	SUBHEADER_GC_CATCH_KING_RESULT_FIELD,
	SUBHEADER_GC_CATCH_KING_SET_END_CARD,
	SUBHEADER_GC_CATCH_KING_REWARD
};

typedef struct SCatchKingCard
{
	SCatchKingCard() { bIndex = 0; bIsExposed = false; }
	SCatchKingCard(BYTE index, bool isExposed)
	{
		bIndex = index;
		bIsExposed = isExposed;
	}

	BYTE bIndex;
	bool bIsExposed;
} TCatchKingCard;

typedef struct SPacketCGMiniGameCatchKing
{
	BYTE bHeader;
	BYTE bSubheader;
	BYTE bSubArgument;
} TPacketCGMiniGameCatchKing;

typedef struct SPacketGCMiniGameCatchKing
{
	BYTE bHeader;
	WORD wSize;
	BYTE bSubheader;
} TPacketGCMiniGameCatchKing;

typedef struct SPacketGCMiniGameCatchKingResult
{
	DWORD dwPoints;
	BYTE bRowType;
	BYTE bCardPos;
	BYTE bCardValue;
	bool bKeepFieldCard;
	bool bDestroyHandCard;
	bool bGetReward;
	bool bIsFiveNearBy;
} TPacketGCMiniGameCatchKingResult;

typedef struct SPacketGCMiniGameCatchKingSetEndCard
{
	BYTE bCardPos;
	BYTE bCardValue;
} TPacketGCMiniGameCatchKingSetEndCard;
#endif

#if defined(__PRIVATESHOP_SEARCH_SYSTEM__)
typedef struct command_privateshop_searchcg
{
	BYTE header;
	BYTE bJob;
	BYTE bMaskType;
	int iMaskSub;
	int iMinRefine;
	int iMaxRefine;
	int iMinLevel;
	int iMaxLevel;
	int iMinGold;
	int iMaxGold;
	char szItemName[ITEM_NAME_MAX_LEN + 1];
#if defined(__CHEQUE_SYSTEM__)
	int iMinCheque;
	int iMaxCheque;
#endif
} TPacketCGPrivateShopSearch;

typedef struct command_privateshopsearch_item
{
	packet_shop_item item;
	char szSellerName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD dwShopPID;
} TPacketGCPrivateShopSearchItem;

typedef struct command_privateshop_searchgc
{
	BYTE header;
	WORD size;
} TPacketGCPrivateShopSearch;

typedef struct command_privateshop_searchopengc
{
	BYTE header;
} TPacketGCPrivateShopSearchOpen;

typedef struct command_privateshop_closecg
{
	BYTE header;
} TPacketCGPrivateShopSearchClose;

typedef struct command_privateshop_buy_item
{
	BYTE header;
	BYTE bPos;
	DWORD dwShopPID;
} TPacketCGPrivateShopSearchBuyItem;
#endif

typedef struct SPacketGGLocaleNotice
{
	BYTE bHeader;
	bool bBigFont;
	char szNotice[MAX_QUEST_NOTICE_ARGS + 1][CHAT_MAX_LEN + 1];
} TPacketGGLocaleNotice;

typedef struct SPacketChangeLanguage
{
	BYTE bHeader;
	BYTE bLanguage;
} TPacketChangeLanguage;

typedef struct SPacketCGWhisperDetails
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGWhisperDetails;

typedef struct SPacketGCWhisperDetails
{
	BYTE header;
	char name[CHARACTER_NAME_MAX_LEN + 1];
	BYTE bLanguage;
} TPacketGCWhisperDetails;

typedef struct SPacketCGCharacterPinCode
{
	BYTE bHeader;
	BYTE bIndex;
	char szPinCode[PIN_CODE_LENGTH + 1];
} TPacketCGCharacterPinCode;

typedef struct SPacketGCCharacterPinCode
{
	BYTE bHeader;
	bool bValid;
} TPacketGCCharacterPinCode;

#if defined(__SKILLBOOK_COMB_SYSTEM__)
typedef struct SPacketCGSkillBookCombination
{
	BYTE bHeader;
	BYTE bAction;
	TItemPos CombItemGrid[SKILLBOOK_COMB_SLOT_MAX];
} TPacketCGSkillBookCombination;
#endif

typedef struct SPacketGCHWID
{
	BYTE bHeader;
	BYTE bStatus;
	BYTE bVerifiedPIN[PLAYER_PER_ACCOUNT];
} TPacketGCHWID;

#if defined(__CHANGE_LOOK_SYSTEM__)
enum class EPacketCGChangeLookSubHeader : BYTE
{
	ITEM_CHECK_IN,
	ITEM_CHECK_OUT,
	FREE_ITEM_CHECK_IN,
	FREE_ITEM_CHECK_OUT,
	ACCEPT,
	CANCEL
};

typedef struct packet_set_changelook
{
	BYTE bHeader;
	WORD wCell;
	BYTE bSlotIndex;
} TPacketGCChangeLookSet;

typedef struct packet_changelook_del
{
	BYTE bHeader;
	WORD wCell;
	BYTE bSlotIndex;
} TPacketGCChangeLookDel;

typedef struct command_changelook
{
	BYTE bHeader;
	BYTE bSubHeader;
	BYTE bSlotIndex;
	TItemPos ItemPos;
} TPacketCGChangeLook;
#endif

#if defined(__MAILBOX__)
typedef struct packet_mailbox_process
{
	BYTE bHeader;
	BYTE bSubHeader;
	BYTE bArg1;
	BYTE bArg2;
} TPacketMailboxProcess;

typedef struct packet_mailbox_process_all
{
	BYTE Index;
} TPacketGCMailboxProcessAll;

typedef struct packet_mailbox
{
	BYTE bHeader;
	WORD wSize;
} TPacketGCMailBox;

typedef struct packet_mailbox_write
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	char szTitle[25 + 1];
	char szMessage[100 + 1];
	TItemPos pos;
	int iYang;
	int iWon;
} TPacketCGMailboxWrite;

typedef struct packet_mailbox_write_confirm
{
	BYTE bHeader;
	char szName[CHARACTER_NAME_MAX_LEN + 1];
} TPacketCGMailboxWriteConfirm;
#endif

#pragma pack()
#endif // __INC_PACKET_H__
