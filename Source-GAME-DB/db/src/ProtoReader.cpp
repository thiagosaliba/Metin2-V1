#include "stdafx.h"

#include <math.h>
#include "ProtoReader.h"

#include "CsvReader.h"

#include <sstream>

using namespace std;

inline string trim_left(const string& str)
{
	string::size_type n = str.find_first_not_of(" \t\v\n\r");
	return n == string::npos ? str : str.substr(n, str.length());
}

inline string trim_right(const string& str)
{
	string::size_type n = str.find_last_not_of(" \t\v\n\r");
	return n == string::npos ? str : str.substr(0, n + 1);
}

string trim(const string& str) { return trim_left(trim_right(str)); }

static string* StringSplit(string strOrigin, string strTok)
{
	int cutAt; // 자르는위치
	int index = 0; // 문자열인덱스
	string* strResult = new string[30]; //결과return 할변수

	// strTok을찾을때까지반복
	while ((cutAt = strOrigin.find_first_of(strTok)) != strOrigin.npos)
	{
		if (cutAt > 0) // 자르는위치가0보다크면(성공시)
		{
			strResult[index++] = strOrigin.substr(0, cutAt); // 결과배열에추가
		}
		strOrigin = strOrigin.substr(cutAt + 1); // 원본은자른부분제외한나머지
	}

	if (strOrigin.length() > 0) // 원본이아직남았으면
	{
		strResult[index++] = strOrigin.substr(0, cutAt); // 나머지를결과배열에추가
	}

	for (int i = 0; i < index; i++)
	{
		strResult[i] = trim(strResult[i]);
	}

	return strResult; // 결과return
}

int get_Item_Type_Value(string inputString)
{
	string arType[] = {
		"ITEM_NONE",				// 0 ~ 1
		"ITEM_WEAPON",				// 1 ~ 2
		"ITEM_ARMOR",				// 2 ~ 3
		"ITEM_USE",					// 3 ~ 4
		"ITEM_AUTOUSE",				// 4 ~ 5
		"ITEM_MATERIAL",			// 5 ~ 6
		"ITEM_SPECIAL",				// 6 ~ 7
		"ITEM_TOOL",				// 7 ~ 8
		"ITEM_LOTTERY",				// 8 ~ 9
		"ITEM_ELK",					// 9 ~ 10개
		"ITEM_METIN",				// 10 ~ 11
		"ITEM_CONTAINER",			// 11 ~ 12
		"ITEM_FISH",				// 12 ~ 13
		"ITEM_ROD",					// 13 ~ 14
		"ITEM_RESOURCE",			// 14 ~ 15
		"ITEM_CAMPFIRE",			// 15 ~ 16
		"ITEM_UNIQUE",				// 16 ~ 17
		"ITEM_SKILLBOOK",			// 17 ~ 18
		"ITEM_QUEST",				// 18 ~ 19
		"ITEM_POLYMORPH",			// 19 ~ 20개
		"ITEM_TREASURE_BOX",		// 20 ~ 21
		"ITEM_TREASURE_KEY",		// 21 ~ 22
		"ITEM_SKILLFORGET",			// 22 ~ 23
		"ITEM_GIFTBOX",				// 23 ~ 24
		"ITEM_PICK",				// 24 ~ 25
		"ITEM_HAIR",				// 25 ~ 26
		"ITEM_TOTEM",				// 26 ~ 27
		"ITEM_BLEND",				// 27 ~ 28
		"ITEM_COSTUME",				// 28 ~ 29
		"ITEM_DS",					// 29 ~ 30개
		"ITEM_SPECIAL_DS",			// 30 ~ 31
		"ITEM_EXTRACT",				// 31 ~ 32
		"ITEM_SECONDARY_COIN",		// 32 ~ 33개
		"ITEM_RING",				// 33 ~ 34
		"ITEM_BELT",				// 34 ~ 35개 (EItemTypes 값으로 치면 34)
#if defined(__CHEQUE_SYSTEM__)
		"ITEM_CHEQUE",				// 35 ~ 36 (EItemTypes Value 35)
#endif
#if defined(__GACHA_SYSTEM__)
		"ITEM_GACHA",				// 36 ~ 37 (EItemTypes Value 36)
#endif
		"ITEM_PET",					// 37 ~ 38 (EItemTypes Value 37)
#if defined(__SOUL_SYSTEM__)
		"ITEM_SOUL",				// 38 ~ 39 (EItemTypes Value 38)
#endif
		"ITEM_MEDIUM",				// 39 ~ 40 (EItemTypes Value 39)
		"ITEM_PASSIVE",				// 40 ~ 41 (EItemTypes Value 40)
		"ITEM_MERCENARY"			// 41 ~ 42 (EItemTypes Value 41) 
	};

	int retInt = -1;
	// cout << "Type : " << typeStr << " -> ";
	for (int j = 0; j < sizeof(arType) / sizeof(arType[0]); j++)
	{
		string tempString = arType[j];
		if (inputString.find(tempString) != string::npos && tempString.find(inputString) != string::npos)
		{
			// cout << j << " ";
			retInt = j;
			break;
		}
	}
	// cout << endl;

	return retInt;
}

int get_Item_SubType_Value(int type_value, string inputString)
{
	static string arSub1[] = {
		"WEAPON_SWORD",
		"WEAPON_DAGGER",
		"WEAPON_BOW",
		"WEAPON_TWO_HANDED",
		"WEAPON_BELL",
		"WEAPON_FAN",
		"WEAPON_ARROW",
		"WEAPON_MOUNT_SPEAR",
		"WEAPON_CLAW",
#if defined(__QUIVER_SYSTEM__)
		"WEAPON_QUIVER",
#endif
		"WEAPON_BOUQUET"
	};

	static string arSub2[] = {
		"ARMOR_BODY",
		"ARMOR_HEAD",
		"ARMOR_SHIELD",
		"ARMOR_WRIST",
		"ARMOR_FOOTS",
		"ARMOR_NECK",
		"ARMOR_EAR",
#if defined(__PENDANT_SYSTEM__)
		"ARMOR_PENDANT",
#endif
#if defined(__GLOVE_SYSTEM__)
		"ARMOR_GLOVE",
#endif
		"ARMOR_NUM_TYPES"
	};

	static string arSub3[] = {
		"USE_POTION",						// 0
		"USE_TALISMAN",						// 1
		"USE_TUNING",						// 2
		"USE_MOVE",							// 3
		"USE_TREASURE_BOX",					// 4
		"USE_MONEYBAG",						// 5
		"USE_BAIT",							// 6
		"USE_ABILITY_UP",					// 7
		"USE_AFFECT",						// 8
		"USE_CREATE_STONE",					// 9
		"USE_SPECIAL",						// 10
		"USE_POTION_NODELAY",				// 11
		"USE_CLEAR",						// 12
		"USE_INVISIBILITY",					// 13
		"USE_DETACHMENT",					// 14
		"USE_BUCKET",						// 15
		"USE_POTION_CONTINUE",				// 16
		"USE_CLEAN_SOCKET",					// 17
		"USE_CHANGE_ATTRIBUTE",				// 18
		"USE_ADD_ATTRIBUTE",				// 19
		"USE_ADD_ACCESSORY_SOCKET",			// 20
		"USE_PUT_INTO_ACCESSORY_SOCKET",	// 21
		"USE_ADD_ATTRIBUTE2",				// 22
		"USE_RECIPE",						// 23
		"USE_CHANGE_ATTRIBUTE2",			// 24
		"USE_BIND",							// 25
		"USE_UNBIND",						// 26
		"USE_TIME_CHARGE_PER",				// 27
		"USE_TIME_CHARGE_FIX",				// 28
		"USE_PUT_INTO_BELT_SOCKET",			// 29
		"USE_PUT_INTO_RING_SOCKET",			// 30
#if defined(__COSTUME_ATTR_SYSTEM__)
		"USE_CHANGE_COSTUME_ATTR",			// 31
		"USE_RESET_COSTUME_ATTR",			// 32
#endif
		"USE_CALL",							// 33 (Bravery Cape)
		"USE_REMOVE_AFFECT",				// 34 (Remove Affect)
		"USE_SELECT_ATTRIBUTE",				// 35
		"USE_EMOTION_PACK",					// 36 (Emotion Pack)
		"USE_FLOWER",						// 37 (Flower Power Event)
		"USE_UNK39",						// 38
		"USE_UNK40",						// 39
		"USE_ELEMENT_UPGRADE",				// 40
		"USE_ELEMENT_DOWNGRADE",			// 41
		"USE_ELEMENT_CHANGE",				// 42
		"USE_POTION_TOWER",					// 43 (SungMa Tower)
		"USE_POTION_NODELAY_TOWER",			// 44 (SungMa Tower)
		"USE_EMOTION_TOWER",				// 45 (SungMa Tower)
		"USE_SECRET_DUNGEON_SCROLL"			// 46
	};

	static string arSub4[] = {
		"AUTOUSE_POTION",
		"AUTOUSE_ABILITY_UP",
		"AUTOUSE_BOMB",
		"AUTOUSE_GOLD",
		"AUTOUSE_MONEYBAG",
		"AUTOUSE_TREASURE_BOX"
	};

	static string arSub5[] = {
		"MATERIAL_LEATHER",
		"MATERIAL_BLOOD",
		"MATERIAL_ROOT",
		"MATERIAL_NEEDLE",
		"MATERIAL_JEWEL",
		"MATERIAL_DS_REFINE_NORMAL",
		"MATERIAL_DS_REFINE_BLESSED",
		"MATERIAL_DS_REFINE_HOLLY",
#if defined(__DS_CHANGE_ATTR__)
		"MATERIAL_DS_CHANGE_ATTR",
#endif
		"MATERIAL_PASSIVE_WEAPON",
		"MATERIAL_PASSIVE_ARMOR",
		"MATERIAL_PASSIVE_ACCE",
		"MATERIAL_PASSIVE_ELEMENT"
	};

	static string arSub6[] = {
		"SPECIAL_MAP",
		"SPECIAL_KEY",
		"SPECIAL_DOC",
		"SPECIAL_SPIRIT"
	};

	static string arSub7[] = {
		"TOOL_FISHING_ROD"
	};

	static string arSub8[] = {
		"LOTTERY_TICKET",
		"LOTTERY_INSTANT"
	};

	static string arSub10[] = {
		"METIN_NORMAL",
		"METIN_GOLD",
		"METIN_SUNGMA"
	};

	static string arSub12[] = {
		"FISH_ALIVE",
		"FISH_DEAD"
	};

	static string arSub14[] = {
		"RESOURCE_FISHBONE",
		"RESOURCE_WATERSTONEPIECE",
		"RESOURCE_WATERSTONE",
		"RESOURCE_BLOOD_PEARL",
		"RESOURCE_BLUE_PEARL",
		"RESOURCE_WHITE_PEARL",
		"RESOURCE_BUCKET",
		"RESOURCE_CRYSTAL",
		"RESOURCE_GEM",
		"RESOURCE_STONE",
		"RESOURCE_METIN",
		"RESOURCE_ORE",
#if defined(__AURA_COSTUME_SYSTEM__)
		"RESOURCE_AURA"
#endif
	};

	static string arSub16[] = {
		"UNIQUE_NONE",
		"UNIQUE_BOOK",
		"UNIQUE_SPECIAL_RIDE",
		"UNIQUE_3",
		"UNIQUE_4",
		"UNIQUE_5",
		"UNIQUE_6",
		"UNIQUE_7",
		"UNIQUE_8",
		"UNIQUE_9",
		"USE_SPECIAL"
	};

	static string arSub18[] = {
		"QUEST_NONE",
		"QUEST_WARP"
	};

	static string arSub28[] = {
		"COSTUME_BODY",
		"COSTUME_HAIR",
#if defined(__MOUNT_COSTUME_SYSTEM__)
		"COSTUME_MOUNT",
#endif
#if defined(__ACCE_COSTUME_SYSTEM__)
		"COSTUME_ACCE",
#endif
#if defined(__WEAPON_COSTUME_SYSTEM__)
		"COSTUME_WEAPON",
#endif
#if defined(__AURA_COSTUME_SYSTEM__)
		"COSTUME_AURA"
#endif
	};

	static string arSub29[] = {
		"DS_SLOT1",
		"DS_SLOT2",
		"DS_SLOT3",
		"DS_SLOT4",
		"DS_SLOT5",
		"DS_SLOT6"
	};

	static string arSub31[] = {
		"EXTRACT_DRAGON_SOUL",
		"EXTRACT_DRAGON_HEART"
	};

	static string arSub37[] = {
		"PET_PAY",
		"PET_EGG",
		"PET_UPBRINGING",
		"PET_FEEDSTUFF",
		"PET_BAG",
		"PET_NAME_CHANGE",
		"PET_EXPFOOD",
		"PET_EXPFOOD_PER",
		"PET_PRIMIUM_FEEDSTUFF",
		"PET_SKILL",
		"PET_SKILL_ALL_DEL_BOOK",
		"PET_SKILL_DEL_BOOK",
		"PET_ATTR_DETERMINE",
		"PET_ATTR_CHANGE"
	};

#if defined(__SOUL_SYSTEM__)
	static string arSub38[] = {
		"SOUL_RED",
		"SOUL_BLUE"
	};
#endif

	static string arSub39[] = {
		"MEDIUM_MOVE_COSTUME_ATTR",
		"MEDIUM_MOVE_ACCE_ATTR"
	};

	static string arSub41[] = {
		"MERCENARY_0",
		"MERCENARY_1",
		"MERCENARY_2",
		"MERCENARY_3",
		"MERCENARY_4",
		"MERCENARY_5",
		"MERCENARY_6"
	};

	static string* arSubType[] = {
		0,			// 0
		arSub1,		// 1
		arSub2,		// 2
		arSub3,		// 3
		arSub4,		// 4
		arSub5,		// 5
		arSub6,		// 6
		arSub7,		// 7
		arSub8,		// 8
		0,			// 9
		arSub10,	// 10
		0,			// 11
		arSub12,	// 12
		0,			// 13
		arSub14,	// 14
		0,			// 15
		arSub16,	// 16
		0,			// 17
		arSub18,	// 18 quest
		0,			// 19
		0,			// 20
		0,			// 21
		0,			// 22
		0,			// 23
		0,			// 24
		0,			// 25
		0,			// 26
		0,			// 27
		arSub28,	// 28
		arSub29,	// 29
		arSub29,	// 30
		arSub31,	// 31
		0,			// 32
		0,			// 33 반지
		0,			// 34 벨트
#if defined(__CHEQUE_SYSTEM__)
		0,			// 35 won
#endif
#if defined(__GACHA_SYSTEM__)
		0,			// 36 gacha
#endif
		arSub37,	// 37 pet
#if defined(__SOUL_SYSTEM__)
		arSub38,	// 38 soul
#endif
		arSub39,	// 39 medium
		0,			// 40 passive
		arSub41,	// 41 mercenary
	};

	static int arNumberOfSubtype[_countof(arSubType)] = {
		0,										// 0
		sizeof(arSub1) / sizeof(arSub1[0]),		// 1
		sizeof(arSub2) / sizeof(arSub2[0]),		// 2
		sizeof(arSub3) / sizeof(arSub3[0]),		// 3
		sizeof(arSub4) / sizeof(arSub4[0]),		// 4
		sizeof(arSub5) / sizeof(arSub5[0]),		// 5
		sizeof(arSub6) / sizeof(arSub6[0]),		// 6
		sizeof(arSub7) / sizeof(arSub7[0]),		// 7
		sizeof(arSub8) / sizeof(arSub8[0]),		// 8
		0,										// 9
		sizeof(arSub10) / sizeof(arSub10[0]),	// 10
		0,										// 11
		sizeof(arSub12) / sizeof(arSub12[0]),	// 12
		0,										// 13
		sizeof(arSub14) / sizeof(arSub14[0]),	// 14
		0,										// 15
		sizeof(arSub16) / sizeof(arSub16[0]),	// 16
		0,										// 17
		sizeof(arSub18) / sizeof(arSub18[0]),	// 18 quest
		0,										// 19
		0,										// 20
		0,										// 21
		0,										// 22
		0,										// 23
		0,										// 24
		0,										// 25
		0,										// 26
		0,										// 27
		sizeof(arSub28) / sizeof(arSub28[0]),	// 28
		sizeof(arSub29) / sizeof(arSub29[0]),	// 29
		sizeof(arSub29) / sizeof(arSub29[0]),	// 30
		sizeof(arSub31) / sizeof(arSub31[0]),	// 31
		0,										// 32
		0,										// 33 반지
		0,										// 34 벨트
#if defined(__CHEQUE_SYSTEM__)
		0,										// 35 won
#endif
#if defined(__GACHA_SYSTEM__)
		0,										// 36 gacha
#endif
		sizeof(arSub37) / sizeof(arSub37[0]),	// 37 gacha
#if defined(__SOUL_SYSTEM__)
		sizeof(arSub38) / sizeof(arSub38[0]),	// 38 soul
#endif
		sizeof(arSub39) / sizeof(arSub39[0]),	// 39 medium
		0,										// 40 passive
		sizeof(arSub41) / sizeof(arSub41[0])	// 41 mercenary
	};

	assert(_countof(arSubType) > type_value && "Subtype rule: Out of range!!");

	// assert 안 먹히는 듯..
	if (_countof(arSubType) <= type_value)
	{
		sys_err("SubType : Out of range!! (type_value: %d, count of registered subtype: %d", type_value, _countof(arSubType));
		return -1;
	}

	// 아이템 타입의 서브타입 어레이가 존재하는지 알아보고, 없으면 0 리턴
	if (arSubType[type_value] == 0)
		return 0;
	//

	int retInt = -1;
	// cout << "SubType : " << subTypeStr << " -> ";
	for (int j = 0; j < arNumberOfSubtype[type_value]; j++)
	{
		string tempString = arSubType[type_value][j];
		string tempInputString = trim(inputString);
		if (tempInputString.compare(tempString) == 0)
		{
			// cout << j << " ";
			retInt = j;
			break;
		}
	}
	// cout << endl;

	return retInt;
}

int get_Item_AntiFlag_Value(string inputString)
{
	string arAntiFlag[] = {
		"ANTI_FEMALE",
		"ANTI_MALE",
		"ANTI_MUSA",
		"ANTI_ASSASSIN",
		"ANTI_SURA",
		"ANTI_MUDANG",
		"ANTI_GET",
		"ANTI_DROP",
		"ANTI_SELL",
		"ANTI_EMPIRE_A",
		"ANTI_EMPIRE_B",
		"ANTI_EMPIRE_C",
		"ANTI_SAVE",
		"ANTI_GIVE",
		"ANTI_PKDROP",
		"ANTI_STACK",
		"ANTI_MYSHOP",
		"ANTI_SAFEBOX",
		"ANTI_WOLFMAN",
		"ANTI_RT_REMOVE",
		"ANTI_QUICKSLOT",
#if defined(__CHANGE_LOOK_SYSTEM__)
		"ANTI_CHANGELOOK",
#endif
		"ANTI_REINFORCE",
		"ANTI_ENCHANT",
		"ANTI_ENERGY",
		"ANTI_PETFEED",
		"ANTI_APPLY",
#if defined(__ACCE_COSTUME_SYSTEM__)
		"ANTI_ACCE",
#endif
		"ANTI_MAIL",
		"ANTI_DESTROY"
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, "|"); // 프로토 정보 내용을 단어별로 쪼갠 배열.
	for (int i = 0; i < sizeof(arAntiFlag) / sizeof(arAntiFlag[0]); i++)
	{
		string tempString = arAntiFlag[i];
		for (int j = 0; j < 30; j++) // 최대 30개 단어까지. (하드코딩)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString) == 0) // 일치하는지 확인.
			{
				retValue = retValue + static_cast<int>(pow((float)2, (float)i));
			}

			if (tempString2.compare("") == 0)
				break;
		}
	}
	delete[]arInputString;
	// cout << "AntiFlag : " << antiFlagStr << " -> " << retValue << endl;

	return retValue;
}

int get_Item_Flag_Value(string inputString)
{
	string arFlag[] = {
		"ITEM_TUNABLE",
		"ITEM_SAVE",
		"ITEM_STACKABLE",
		"COUNT_PER_1GOLD",
		"ITEM_SLOW_QUERY",
		"ITEM_RARE",
		"ITEM_UNIQUE",
		"ITEM_MAKECOUNT",
		"ITEM_IRREMOVABLE",
		"CONFIRM_WHEN_USE",
		"QUEST_USE",
		"QUEST_USE_MULTIPLE",
		"QUEST_GIVE",
		"LOG",
		"ITEM_APPLICABLE"
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, "|"); // 프로토 정보 내용을 단어별로 쪼갠 배열.
	for (int i = 0; i < sizeof(arFlag) / sizeof(arFlag[0]); i++)
	{
		string tempString = arFlag[i];
		for (int j = 0; j < 30; j++) // 최대 30개 단어까지. (하드코딩)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString) == 0) // 일치하는지 확인.
			{
				retValue = retValue + static_cast<int>(pow((float)2, (float)i));
			}

			if (tempString2.compare("") == 0)
				break;
		}
	}
	delete[]arInputString;
	// cout << "Flag : " << flagStr << " -> " << retValue << endl;

	return retValue;
}

int get_Item_WearFlag_Value(string inputString)
{
	string arWearrFlag[] = {
		"WEAR_BODY",				// 0 @ EItemWearableFlag
		"WEAR_HEAD",				// 1
		"WEAR_FOOTS",				// 2
		"WEAR_WRIST",				// 3
		"WEAR_WEAPON",				// 4
		"WEAR_NECK",				// 5
		"WEAR_EAR",					// 6
		"WEAR_UNIQUE",				// 7
		"WEAR_SHIELD",				// 8
		"WEAR_ARROW",				// 9
		"WEAR_HAIR",				// 10
		"WEAR_ABILITY",				// 11
		"WEAR_COSTUME_BODY",		// 12
		"WEAR_COSTUME_HAIR",		// 13
#if defined(__MOUNT_COSTUME_SYSTEM__)
		"WEAR_COSTUME_MOUNT",		// 14
#endif
#if defined(__ACCE_COSTUME_SYSTEM__)
		"WEAR_COSTUME_ACCE",		// 15
#endif
#if defined(__WEAPON_COSTUME_SYSTEM__)
		"WEAR_COSTUME_WEAPON",		// 16
#endif
#if defined(__AURA_COSTUME_SYSTEM__)
		"WEAR_COSTUME_AURA",		// 17
#endif
#if defined(__PENDANT_SYSTEM__)
		"WEAR_PENDANT",				// 18
#endif
#if defined(__GLOVE_SYSTEM__)
		"WEAR_GLOVE",				// 19
#endif
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, "|"); // 프로토 정보 내용을 단어별로 쪼갠 배열.
	for (int i = 0; i < sizeof(arWearrFlag) / sizeof(arWearrFlag[0]); i++)
	{
		string tempString = arWearrFlag[i];
		for (int j = 0; j < 30; j++) // 최대 30개 단어까지. (하드코딩)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString) == 0) // 일치하는지 확인.
			{
				retValue = retValue + static_cast<int>(pow((float)2, (float)i));
			}

			if (tempString2.compare("") == 0)
				break;
		}
	}
	delete[]arInputString;
	// cout << "WearFlag : " << wearFlagStr << " -> " << retValue << endl;

	return retValue;
}

int get_Item_Immune_Value(string inputString)
{
	string arImmune[] = {
		"PARA",
		"CURSE",
		"STUN",
		"SLEEP",
		"SLOW",
		"POISON",
		"TERROR"
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, "|"); //프로토 정보 내용을 단어별로 쪼갠 배열.
	for (int i = 0; i < sizeof(arImmune) / sizeof(arImmune[0]); i++)
	{
		string tempString = arImmune[i];
		for (int j = 0; j < 30; j++) // 최대 30개 단어까지. (하드코딩)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString) == 0) // 일치하는지 확인.
			{
				retValue = retValue + static_cast<int>(pow((float)2, (float)i));
			}

			if (tempString2.compare("") == 0)
				break;
		}
	}
	delete[]arInputString;
	// cout << "Immune : " << immuneStr << " -> " << retValue << endl;

	return retValue;
}

int get_Item_LimitType_Value(string inputString)
{
	string arLimitType[] = {
		"LIMIT_NONE",
		"LEVEL",
		"STR",
		"DEX",
		"INT",
		"CON",
		"PC_BANG",
		"REAL_TIME",
		"REAL_TIME_FIRST_USE",
		"TIMER_BASED_ON_WEAR",
		"NEWWORLD_LEVEL",
		"DURATION",
	};

	int retInt = -1;
	// cout << "LimitType : " << limitTypeStr << " -> ";
	for (int j = 0; j < sizeof(arLimitType) / sizeof(arLimitType[0]); j++)
	{
		string tempString = arLimitType[j];
		string tempInputString = trim(inputString);
		if (tempInputString.compare(tempString) == 0)
		{
			// cout << j << " ";
			retInt = j;
			break;
		}
	}
	// cout << endl;

	return retInt;
}

int get_Item_ApplyType_Value(string inputString)
{
	string arApplyType[] = {
		"APPLY_NONE",
		"APPLY_MAX_HP",
		"APPLY_MAX_SP",
		"APPLY_CON",
		"APPLY_INT",
		"APPLY_STR",
		"APPLY_DEX",
		"APPLY_ATT_SPEED",
		"APPLY_MOV_SPEED",
		"APPLY_CAST_SPEED",
		"APPLY_HP_REGEN",
		"APPLY_SP_REGEN",
		"APPLY_POISON_PCT",
		"APPLY_STUN_PCT",
		"APPLY_SLOW_PCT",
		"APPLY_CRITICAL_PCT",
		"APPLY_PENETRATE_PCT",
		"APPLY_ATTBONUS_HUMAN",
		"APPLY_ATTBONUS_ANIMAL",
		"APPLY_ATTBONUS_ORC",
		"APPLY_ATTBONUS_MILGYO",
		"APPLY_ATTBONUS_UNDEAD",
		"APPLY_ATTBONUS_DEVIL",
		"APPLY_STEAL_HP",
		"APPLY_STEAL_SP",
		"APPLY_MANA_BURN_PCT",
		"APPLY_DAMAGE_SP_RECOVER",
		"APPLY_BLOCK",
		"APPLY_DODGE",
		"APPLY_RESIST_SWORD",
		"APPLY_RESIST_TWOHAND",
		"APPLY_RESIST_DAGGER",
		"APPLY_RESIST_BELL",
		"APPLY_RESIST_FAN",
		"APPLY_RESIST_BOW",
		"APPLY_RESIST_FIRE",
		"APPLY_RESIST_ELEC",
		"APPLY_RESIST_MAGIC",
		"APPLY_RESIST_WIND",
		"APPLY_REFLECT_MELEE",
		"APPLY_REFLECT_CURSE",
		"APPLY_POISON_REDUCE",
		"APPLY_KILL_SP_RECOVER",
		"APPLY_EXP_DOUBLE_BONUS",
		"APPLY_GOLD_DOUBLE_BONUS",
		"APPLY_ITEM_DROP_BONUS",
		"APPLY_POTION_BONUS",
		"APPLY_KILL_HP_RECOVER",
		"APPLY_IMMUNE_STUN",
		"APPLY_IMMUNE_SLOW",
		"APPLY_IMMUNE_FALL",
		"APPLY_SKILL",
		"APPLY_BOW_DISTANCE",
		"APPLY_ATT_GRADE_BONUS",
		"APPLY_DEF_GRADE_BONUS",
		"APPLY_MAGIC_ATT_GRADE",
		"APPLY_MAGIC_DEF_GRADE",
		"APPLY_CURSE_PCT",
		"APPLY_MAX_STAMINA",
		"APPLY_ATTBONUS_WARRIOR",
		"APPLY_ATTBONUS_ASSASSIN",
		"APPLY_ATTBONUS_SURA",
		"APPLY_ATTBONUS_SHAMAN",
		"APPLY_ATTBONUS_MONSTER",
		"APPLY_MALL_ATTBONUS",
		"APPLY_MALL_DEFBONUS",
		"APPLY_MALL_EXPBONUS",
		"APPLY_MALL_ITEMBONUS",
		"APPLY_MALL_GOLDBONUS",
		"APPLY_MAX_HP_PCT",
		"APPLY_MAX_SP_PCT",
		"APPLY_SKILL_DAMAGE_BONUS",
		"APPLY_NORMAL_HIT_DAMAGE_BONUS",
		"APPLY_SKILL_DEFEND_BONUS",
		"APPLY_NORMAL_HIT_DEFEND_BONUS",
		"APPLY_PC_BANG_EXP_BONUS",
		"APPLY_PC_BANG_DROP_BONUS",
		"APPLY_EXTRACT_HP_PCT",
		"APPLY_RESIST_WARRIOR",
		"APPLY_RESIST_ASSASSIN",
		"APPLY_RESIST_SURA",
		"APPLY_RESIST_SHAMAN",
		"APPLY_ENERGY",
		"APPLY_DEF_GRADE",
		"APPLY_COSTUME_ATTR_BONUS",
		"APPLY_MAGIC_ATTBONUS_PER",
		"APPLY_MELEE_MAGIC_ATTBONUS_PER",
		"APPLY_RESIST_ICE",
		"APPLY_RESIST_EARTH",
		"APPLY_RESIST_DARK",
		"APPLY_ANTI_CRITICAL_PCT",
		"APPLY_ANTI_PENETRATE_PCT",
		"APPLY_BLEEDING_REDUCE",
		"APPLY_BLEEDING_PCT",
		"APPLY_ATTBONUS_WOLFMAN",
		"APPLY_RESIST_WOLFMAN",
		"APPLY_RESIST_CLAW",
#if defined(__MOUNT_COSTUME_SYSTEM__)
		"APPLY_MOUNT",
#endif
#if defined(__ACCE_COSTUME_SYSTEM__)
		"APPLY_ACCEDRAIN_RATE",
#endif
#if defined(__MAGIC_REDUCTION__)
		"APPLY_RESIST_MAGIC_REDUCTION",
#endif
#if defined(__ELEMENT_SYSTEM__)
		"APPLY_ENCHANT_ELECT",
		"APPLY_ENCHANT_FIRE",
		"APPLY_ENCHANT_ICE",
		"APPLY_ENCHANT_WIND",
		"APPLY_ENCHANT_EARTH",
		"APPLY_ENCHANT_DARK",
		"APPLY_ATTBONUS_CZ",
		"APPLY_ATTBONUS_INSECT",
		"APPLY_ATTBONUS_DESERT",
		"APPLY_RESIST_SWORD_REDUCTION",
		"APPLY_RESIST_TWOHAND_REDUCTION",
		"APPLY_RESIST_DAGGER_REDUCTION",
		"APPLY_RESIST_BELL_REDUCTION",
		"APPLY_RESIST_FAN_REDUCTION",
		"APPLY_RESIST_BOW_REDUCTION",
		"APPLY_RESIST_CLAW_REDUCTION",
		"APPLY_RESIST_HUMAN",
#endif
		"APPLY_ATTBONUS_STONE",
#if defined(__CONQUEROR_LEVEL__)
		"APPLY_SUNGMA_STR",
		"APPLY_SUNGMA_HP",
		"APPLY_SUNGMA_MOVE",
		"APPLY_SUNGMA_IMMUNE",
#endif
#if defined(__ITEM_APPLY_RANDOM__)
		"APPLY_RANDOM",
#endif
	};

	int retInt = -1;
	// cout << "ApplyType : " << applyTypeStr << " -> ";
	for (int j = 0; j < sizeof(arApplyType) / sizeof(arApplyType[0]); j++)
	{
		string tempString = arApplyType[j];
		string tempInputString = trim(inputString);
		if (tempInputString.compare(tempString) == 0)
		{
			// cout << j << " ";
			retInt = j;
			break;
		}
	}
	// cout << endl;

	return retInt;
}

//몬스터 프로토도 읽는다.
int get_Mob_Rank_Value(string inputString)
{
	string arRank[] = {
		"PAWN",
		"S_PAWN",
		"KNIGHT",
		"S_KNIGHT",
		"BOSS",
		"KING"
	};

	int retInt = -1;
	// cout << "Rank : " << rankStr << " -> ";
	for (int j = 0; j < sizeof(arRank) / sizeof(arRank[0]); j++)
	{
		string tempString = arRank[j];
		string tempInputString = trim(inputString);
		if (tempInputString.compare(tempString) == 0)
		{
			// cout << j << " ";
			retInt = j;
			break;
		}
	}
	// cout << endl;

	return retInt;
}

int get_Mob_Type_Value(string inputString)
{
	string arType[] = {
		"MONSTER",
		"NPC",
		"STONE",
		"WARP",
		"DOOR",
		"BUILDING",
		"PC",
		"POLYMORPH_PC",
		"HORSE",
		"GOTO",
		"PET",
		"PET_PAY"
	};

	int retInt = -1;
	// cout << "Type : " << typeStr << " -> ";
	for (int j = 0; j < sizeof(arType) / sizeof(arType[0]); j++)
	{
		string tempString = arType[j];
		string tempInputString = trim(inputString);
		if (tempInputString.compare(tempString) == 0)
		{
			// cout << j << " ";
			retInt = j;
			break;
		}
	}
	// cout << endl;

	return retInt;
}

int get_Mob_BattleType_Value(string inputString)
{
	string arBattleType[] = {
		"MELEE",
		"RANGE",
		"MAGIC",
		"SPECIAL",
		"POWER",
		"TANKER",
		"SUPER_POWER",
		"SUPER_TANKER"
	};

	int retInt = -1;
	// cout << "Battle Type : " << battleTypeStr << " -> ";
	for (int j = 0; j < sizeof(arBattleType) / sizeof(arBattleType[0]); j++)
	{
		string tempString = arBattleType[j];
		string tempInputString = trim(inputString);
		if (tempInputString.compare(tempString) == 0)
		{
			// cout << j << " ";
			retInt = j;
			break;
		}
	}
	// cout << endl;

	return retInt;
}

int get_Mob_Size_Value(string inputString)
{
	string arSize[] = {
		"SMALL",
		"MEDIUM",
		"BIG"
	};

	int retInt = 0;
	// cout << "Size : " << sizeStr << " -> ";
	for (int j = 0; j < sizeof(arSize) / sizeof(arSize[0]); j++)
	{
		string tempString = arSize[j];
		string tempInputString = trim(inputString);
		if (tempInputString.compare(tempString) == 0)
		{
			// cout << j << " ";
			retInt = j + 1;
			break;
		}
	}
	// cout << endl;

	return retInt;
}

int get_Mob_AIFlag_Value(string inputString)
{
	string arAIFlag[] = {
		"AGGR",
		"NOMOVE",
		"COWARD",
		"NOATTSHINSU",
		"NOATTCHUNJO",
		"NOATTJINNO",
		"ATTMOB",
		"BERSERK",
		"STONESKIN",
		"GODSPEED",
		"DEATHBLOW",
		"REVIVE",
		"HEALER",
		"COUNT",
		"NORECOVERY",
		"REFLECT",
		"FALL",
		"VIT",
		"RATTSPEED",
		"RCASTSPEED",
		"RHP_REGEN",
		"TIMEVIT",
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, ","); // 프로토 정보 내용을 단어별로 쪼갠 배열.
	for (int i = 0; i < sizeof(arAIFlag) / sizeof(arAIFlag[0]); i++)
	{
		string tempString = arAIFlag[i];
		for (int j = 0; j < 30; j++) // 최대 30개 단어까지. (하드코딩)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString) == 0) // 일치하는지 확인.
			{
				retValue = retValue + static_cast<int>(pow((float)2, (float)i));
			}

			if (tempString2.compare("") == 0)
				break;
		}
	}
	delete[]arInputString;
	// cout << "AIFlag : " << aiFlagStr << " -> " << retValue << endl;

	return retValue;
}

int get_Mob_RaceFlag_Value(string inputString)
{
	string arRaceFlag[] = {
		"ANIMAL",		// 0
		"UNDEAD",		// 1
		"DEVIL",		// 2
		"HUMAN",		// 3
		"ORC",			// 4
		"MILGYO",		// 5
		"INSECT",		// 6
		"FIRE",			// 7
		"ICE",			// 8
		"DESERT",		// 9
		"TREE",			// 10
		"ATT_ELEC",		// 11
		"ATT_FIRE",		// 12
		"ATT_ICE",		// 13
		"ATT_WIND",		// 14
		"ATT_EARTH",	// 15
		"ATT_DARK",		// 16
#if defined(__ELEMENT_SYSTEM__)
		"ZODIAC",		// 17
#endif
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, ","); // 프로토 정보 내용을 단어별로 쪼갠 배열.
	for (int i = 0; i < sizeof(arRaceFlag) / sizeof(arRaceFlag[0]); i++)
	{
		string tempString = arRaceFlag[i];
		for (int j = 0; j < 30; j++) // 최대 30개 단어까지. (하드코딩)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString) == 0) // 일치하는지 확인.
			{
				retValue = retValue + static_cast<int>(pow((float)2, (float)i));
			}

			if (tempString2.compare("") == 0)
				break;
		}
	}
	delete[]arInputString;
	// cout << "Race Flag : " << raceFlagStr << " -> " << retValue << endl;

	return retValue;
}

int get_Mob_ImmuneFlag_Value(string inputString)
{
	string arImmuneFlag[] = {
		"STUN",
		"SLOW",
		"FALL",
		"CURSE",
		"POISON",
		"TERROR",
		"REFLECT"
	};

	int retValue = 0;
	string* arInputString = StringSplit(inputString, ","); // 프로토 정보 내용을 단어별로 쪼갠 배열.
	for (int i = 0; i < sizeof(arImmuneFlag) / sizeof(arImmuneFlag[0]); i++)
	{
		string tempString = arImmuneFlag[i];
		for (int j = 0; j < 30; j++) // 최대 30개 단어까지. (하드코딩)
		{
			string tempString2 = arInputString[j];
			if (tempString2.compare(tempString) == 0) // 일치하는지 확인.
			{
				retValue = retValue + static_cast<int>(pow((float)2, (float)i));
			}

			if (tempString2.compare("") == 0)
				break;
		}
	}
	delete[]arInputString;
	// cout << "Immune Flag : " << immuneFlagStr << " -> " << retValue << endl;

	return retValue;
}

#ifndef __DUMP_PROTO__
// 몹 테이블을 셋팅해준다.
bool Set_Proto_Mob_Table(TMobTable* mobTable, cCsvTable& csvTable, std::map<int, const char*>& nameMap)
{
	int col = 0;
	str_to_number(mobTable->dwVnum, csvTable.AsStringByIndex(col++));
	strlcpy(mobTable->szName, csvTable.AsStringByIndex(col++), sizeof(mobTable->szName));

	// 3. 지역별 이름 넣어주기.
	map<int, const char*>::iterator it;
	it = nameMap.find(mobTable->dwVnum);
	if (it != nameMap.end())
	{
		const char* localeName = it->second;
		strlcpy(mobTable->szLocaleName, localeName, sizeof(mobTable->szLocaleName));
	}
	else
	{
		strlcpy(mobTable->szLocaleName, mobTable->szName, sizeof(mobTable->szLocaleName));
	}

	// RANK
	int rankValue = get_Mob_Rank_Value(csvTable.AsStringByIndex(col++));
	mobTable->bRank = rankValue;
	// TYPE
	int typeValue = get_Mob_Type_Value(csvTable.AsStringByIndex(col++));
	mobTable->bType = typeValue;
	// BATTLE_TYPE
	int battleTypeValue = get_Mob_BattleType_Value(csvTable.AsStringByIndex(col++));
	mobTable->bBattleType = battleTypeValue;

	str_to_number(mobTable->bLevel, csvTable.AsStringByIndex(col++));
	// SCALEPCT
	col++;
	// SIZE
	int sizeValue = get_Mob_Size_Value(csvTable.AsStringByIndex(col++));
	mobTable->bSize = sizeValue;
	// AI_FLAG
	int aiFlagValue = get_Mob_AIFlag_Value(csvTable.AsStringByIndex(col++));
	mobTable->dwAIFlag = aiFlagValue;
	// MOUNT_CAPACITY;
	col++;
	// RACE_FLAG
	int raceFlagValue = get_Mob_RaceFlag_Value(csvTable.AsStringByIndex(col++));
	mobTable->dwRaceFlag = raceFlagValue;
	// IMMUNE_FLAG
	int immuneFlagValue = get_Mob_ImmuneFlag_Value(csvTable.AsStringByIndex(col++));
	mobTable->dwImmuneFlag = immuneFlagValue;

	str_to_number(mobTable->bEmpire, csvTable.AsStringByIndex(col++)); // col = 11

	strlcpy(mobTable->szFolder, csvTable.AsStringByIndex(col++), sizeof(mobTable->szFolder));

	str_to_number(mobTable->bOnClickType, csvTable.AsStringByIndex(col++));

	str_to_number(mobTable->bStr, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bDex, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bCon, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bInt, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->dwDamageRange[0], csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->dwDamageRange[1], csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->dwMaxHP, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bRegenCycle, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bRegenPercent, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->dwGoldMin, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->dwGoldMax, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->dwExp, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->wDef, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->sAttackSpeed, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->sMovingSpeed, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bAggresiveHPPct, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->wAggressiveSight, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->wAttackRange, csvTable.AsStringByIndex(col++));

	str_to_number(mobTable->dwDropItemVnum, csvTable.AsStringByIndex(col++)); // 32
	str_to_number(mobTable->dwResurrectionVnum, csvTable.AsStringByIndex(col++));
	for (int i = 0; i < MOB_ENCHANTS_MAX_NUM; ++i)
		str_to_number(mobTable->cEnchants[i], csvTable.AsStringByIndex(col++));

	for (int i = 0; i < MOB_RESISTS_MAX_NUM; ++i)
		str_to_number(mobTable->cResists[i], csvTable.AsStringByIndex(col++));

#if defined(__ELEMENT_SYSTEM__)
	for (int i = 0; i < MOB_ELEMENT_MAX_NUM; ++i)
		str_to_number(mobTable->cElements[i], csvTable.AsStringByIndex(col++));
#endif

	str_to_number(mobTable->fDamMultiply, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->dwSummonVnum, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->dwDrainSP, csvTable.AsStringByIndex(col++));

	// MOB_COLOR
	++col;

	str_to_number(mobTable->dwPolymorphItemVnum, csvTable.AsStringByIndex(col++));

	str_to_number(mobTable->Skills[0].bLevel, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->Skills[0].dwVnum, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->Skills[1].bLevel, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->Skills[1].dwVnum, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->Skills[2].bLevel, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->Skills[2].dwVnum, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->Skills[3].bLevel, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->Skills[3].dwVnum, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->Skills[4].bLevel, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->Skills[4].dwVnum, csvTable.AsStringByIndex(col++));

	str_to_number(mobTable->bBerserkPoint, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bStoneSkinPoint, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bGodSpeedPoint, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bDeathBlowPoint, csvTable.AsStringByIndex(col++));
	str_to_number(mobTable->bRevivePoint, csvTable.AsStringByIndex(col++));

	sys_log(0, "MOB #%-5d %-24s level: %-3u rank: %u empire: %d", mobTable->dwVnum, mobTable->szLocaleName, mobTable->bLevel, mobTable->bRank, mobTable->bEmpire);

	return true;
}

bool Set_Proto_Item_Table(TItemTable* itemTable, cCsvTable& csvTable, std::map<int, const char*>& nameMap)
{
	int col = 0;

#if defined(__ITEM_APPLY4__)
	int dataArray[35];
#else
	int dataArray[33];
#endif
	for (int i = 0; i < sizeof(dataArray) / sizeof(dataArray[0]); i++)
	{
		int validCheck = 0;
		if (i == 2)
		{
			dataArray[i] = get_Item_Type_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 3)
		{
			dataArray[i] = get_Item_SubType_Value(dataArray[i - 1], csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 5)
		{
			dataArray[i] = get_Item_AntiFlag_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 6)
		{
			dataArray[i] = get_Item_Flag_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 7)
		{
			dataArray[i] = get_Item_WearFlag_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 8)
		{
			dataArray[i] = get_Item_Immune_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 14)
		{
			dataArray[i] = get_Item_LimitType_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 16)
		{
			dataArray[i] = get_Item_LimitType_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 18)
		{
			dataArray[i] = get_Item_ApplyType_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 20)
		{
			dataArray[i] = get_Item_ApplyType_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
		else if (i == 22)
		{
			dataArray[i] = get_Item_ApplyType_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
#if defined(__ITEM_APPLY4__)
		else if (i == 24)
		{
			dataArray[i] = get_Item_ApplyType_Value(csvTable.AsStringByIndex(col));
			validCheck = dataArray[i];
		}
#endif
		else
		{
			str_to_number(dataArray[i], csvTable.AsStringByIndex(col));
		}

		if (validCheck == -1)
		{
			std::ostringstream dataStream;

			for (int j = 0; j < i; ++j)
				dataStream << dataArray[j] << ",";

			// fprintf(stderr, "ItemProto Reading Failed : Invalid value.\n");
			sys_err("ItemProto Reading Failed : Invalid value. (index: %d, col: %d, value: %s)", i, col, csvTable.AsStringByIndex(col));
			sys_err("\t%d ~ %d Values: %s", 0, i, dataStream.str().c_str());

			exit(0);
		}

		col = col + 1;
	}

	// vnum 및 vnum range 읽기.
	{
		std::string s(csvTable.AsStringByIndex(0));
		int pos = s.find("~");
		// vnum 필드에 '~'가 없다면 패스
		if (std::string::npos == pos)
		{
			itemTable->dwVnum = dataArray[0];
			itemTable->dwVnumRange = 0;
		}
		else
		{
			std::string s_start_vnum(s.substr(0, pos));
			std::string s_end_vnum(s.substr(pos + 1));

			int start_vnum = atoi(s_start_vnum.c_str());
			int end_vnum = atoi(s_end_vnum.c_str());
			if (0 == start_vnum || (0 != end_vnum && end_vnum < start_vnum))
			{
				sys_err("INVALID VNUM %s", s.c_str());
				return false;
			}
			itemTable->dwVnum = start_vnum;
			itemTable->dwVnumRange = end_vnum - start_vnum;
		}
	}

	strlcpy(itemTable->szName, csvTable.AsStringByIndex(1), sizeof(itemTable->szName));
	// 지역별 이름 넣어주기.
	map<int, const char*>::iterator it;
	it = nameMap.find(itemTable->dwVnum);
	if (it != nameMap.end())
	{
		const char* localeName = it->second;
		strlcpy(itemTable->szLocaleName, localeName, sizeof(itemTable->szLocaleName));
	}
	else
	{
		strlcpy(itemTable->szLocaleName, itemTable->szName, sizeof(itemTable->szLocaleName));
	}
	itemTable->bType = dataArray[2];
	itemTable->bSubType = dataArray[3];
	itemTable->bSize = dataArray[4];
	itemTable->ullAntiFlags = dataArray[5];
	itemTable->dwFlags = dataArray[6];
	itemTable->dwWearFlags = dataArray[7];
	itemTable->dwImmuneFlag = dataArray[8];
	itemTable->dwGold = dataArray[9];
	itemTable->dwShopBuyPrice = dataArray[10];
	itemTable->dwRefinedVnum = dataArray[11];
	itemTable->wRefineSet = dataArray[12];
	itemTable->bAlterToMagicItemPct = dataArray[13];
	itemTable->cLimitRealTimeFirstUseIndex = -1;
	itemTable->cLimitTimerBasedOnWearIndex = -1;

	int i;

	for (i = 0; i < ITEM_LIMIT_MAX_NUM; ++i)
	{
		itemTable->aLimits[i].bType = dataArray[14 + i * 2];
		itemTable->aLimits[i].lValue = dataArray[15 + i * 2];

		if (LIMIT_REAL_TIME_START_FIRST_USE == itemTable->aLimits[i].bType)
			itemTable->cLimitRealTimeFirstUseIndex = (char)i;

		if (LIMIT_TIMER_BASED_ON_WEAR == itemTable->aLimits[i].bType)
			itemTable->cLimitTimerBasedOnWearIndex = (char)i;
	}

	for (i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
	{
		itemTable->aApplies[i].bType = dataArray[18 + i * 2];
		itemTable->aApplies[i].lValue = dataArray[19 + i * 2];
	}

	for (i = 0; i < ITEM_VALUES_MAX_NUM; ++i)
#if defined(__ITEM_APPLY4__)
		itemTable->alValues[i] = dataArray[26 + i];
#else
		itemTable->alValues[i] = dataArray[24 + i];
#endif

	// column for 'Specular'
#if defined(__ITEM_APPLY4__)
	itemTable->bGainSocketPct = dataArray[33];
	itemTable->sAddonType = dataArray[34];
#else
	itemTable->bGainSocketPct = dataArray[31];
	itemTable->sAddonType = dataArray[32];
#endif

	// test
	str_to_number(itemTable->bWeight, "0");

	return true;
}
#endif
