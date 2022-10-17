#include "stdafx.h"
#include "QID.h"
#include "DBManager.h"
#include "ItemAwardManager.h"
#include "Peer.h"

#include "Main.h"
#include "ClientManager.h"

#if defined(__EXTENDED_ITEM_AWARD__)
#include <float.h>

inline double uniform_random(const double a, const double b)
{
	return thecore_random() / (RAND_MAX + 1.f) * (b - a) + a;
}

inline float gauss_random(const float fAverage, const float sigma)
{
	static bool bHasNextGaussian = false;
	static float fNextGaussian = 0.0f;

	if (bHasNextGaussian)
	{
		bHasNextGaussian = false;
		return (fNextGaussian * sigma) + fAverage;
	}
	else
	{
		double v1(0), v2(0), s(0);
		do
		{
			v1 = uniform_random(-1.f, 1.f);
			v2 = uniform_random(-1.f, 1.f);
			s = (v1 * v1) + (v2 * v2);
		} while (s >= 1.f || fabs(s) < FLT_EPSILON);

		const double multiplier = sqrtf(-2 * logf(s) / s);
		fNextGaussian = v2 * multiplier;
		bHasNextGaussian = true;
		return (v1 * multiplier) * sigma + fAverage;
	}
}
#endif

DWORD g_dwLastCachedItemAwardID = 0;
ItemAwardManager::ItemAwardManager()
{
}

ItemAwardManager::~ItemAwardManager()
{
}

void ItemAwardManager::RequestLoad()
{
	char szQuery[QUERY_MAX_LEN];
#if defined(__EXTENDED_ITEM_AWARD__)
	snprintf(szQuery, sizeof(szQuery), "SELECT `id`, `login`, `vnum`, `count`, `socket0`, `socket1`, `socket2`, "
#if defined(__ITEM_SOCKET5__)
		"`socket3`, "
		"`socket4`, "
#endif
		"`attrtype0`, `attrvalue0`, "
		"`attrtype1`, `attrvalue1`, "
		"`attrtype2`, `attrvalue2`, "
		"`attrtype3`, `attrvalue3`, "
		"`attrtype4`, `attrvalue4`, "
		"`attrtype5`, `attrvalue5`, "
		"`attrtype6`, `attrvalue6`, "
		"mall, why "
		"FROM item_award%s WHERE `taken_time` IS NULL AND `id` > %d", GetTablePostfix(), g_dwLastCachedItemAwardID);
#else
	snprintf(szQuery, sizeof(szQuery), "SELECT `id`, `login`, `vnum`, `count`, `socket0`, `socket1`, `socket2`, `mall`, `why` FROM item_award%s WHERE `taken_time` IS NULL AND `id` > %d", GetTablePostfix(), g_dwLastCachedItemAwardID);
#endif
	CDBManager::instance().ReturnQuery(szQuery, QID_ITEM_AWARD_LOAD, 0, NULL);
}

void ItemAwardManager::Load(SQLMsg* pMsg)
{
	MYSQL_RES* pRes = pMsg->Get()->pSQLResult;

	for (uint i = 0; i < pMsg->Get()->uiNumRows; ++i)
	{
		MYSQL_ROW row = mysql_fetch_row(pRes);
		int col = 0;

		DWORD dwID = 0;
		str_to_number(dwID, row[col++]);

		if (m_map_award.find(dwID) != m_map_award.end())
			continue;

		TItemAward* kData = new TItemAward;
		memset(kData, 0, sizeof(TItemAward));

		kData->dwID = dwID;
		trim_and_lower(row[col++], kData->szLogin, sizeof(kData->szLogin));
		str_to_number(kData->dwVnum, row[col++]);
		str_to_number(kData->dwCount, row[col++]);
		str_to_number(kData->dwSocket0, row[col++]);
		str_to_number(kData->dwSocket1, row[col++]);
		str_to_number(kData->dwSocket2, row[col++]);
#if defined(__ITEM_SOCKET5__)
		str_to_number(kData->dwSocket3, row[col++]);
		str_to_number(kData->dwSocket4, row[col++]);
#endif
#if defined(__EXTENDED_ITEM_AWARD__)
		for (size_t j = 0; j < ITEM_ATTRIBUTE_MAX_NUM; j++)
		{
			str_to_number(kData->aAttr[j].bType, row[col++]);
			str_to_number(kData->aAttr[j].sValue, row[col++]);
		}
#endif
		str_to_number(kData->bMall, row[col++]);

		if (row[col])
		{
			strlcpy(kData->szWhy, row[col], sizeof(kData->szWhy));
			// ���� �߿� why�ݷ뿡 ������ �����
			char* whyStr = kData->szWhy; // why �ݷ� �б�
			char cmdStr[100] = ""; // why�ݷ뿡�� ���� ���� �ӽ� ���ڿ��� �����ص�
			strcpy(cmdStr, whyStr); // ��ɾ� ��� �������� ��ū���� ������ ��ūȭ �Ǳ� ����
			char command[20] = "";
			CClientManager::instance().GetCommand(cmdStr, command); // command ���
			//sys_err("%d, %s", pItemAward->dwID, command);
			if (!(strcmp(cmdStr, "GIFT"))) // command �� GIFT�̸�
			{
				TPacketItemAwardInfromer giftData;
				strcpy(giftData.login, kData->szLogin); // �α��� ���̵� ����
				strcpy(giftData.command, command); // ��ɾ� ����
				giftData.vnum = kData->dwVnum; // ������ vnum�� ����
				CClientManager::instance().ForwardPacket(HEADER_DG_ITEMAWARD_INFORMER, &giftData, sizeof(TPacketItemAwardInfromer));
			}
		}

		m_map_award.insert(std::make_pair(dwID, kData));

		printf("ITEM_AWARD load id %u bMall %d \n", kData->dwID, kData->bMall);
		sys_log(0, "ITEM_AWARD: load id %lu login %s vnum %lu count %u socket %lu", kData->dwID, kData->szLogin, kData->dwVnum, kData->dwCount, kData->dwSocket0);
		ItemAwardSet& kSet = m_map_kSetAwardByLogin[kData->szLogin];
		kSet.insert(kData);

		if (dwID > g_dwLastCachedItemAwardID)
			g_dwLastCachedItemAwardID = dwID;
	}
}

ItemAwardSet* ItemAwardManager::GetByLogin(const char* c_pszLogin)
{
	//kSetAwardMap::iterator it = m_map_kSetAwardByLogin.find(c_pszLogin);
	itertype(m_map_kSetAwardByLogin) it = m_map_kSetAwardByLogin.find(c_pszLogin);

	if (it == m_map_kSetAwardByLogin.end())
		return NULL;

	return &it->second;
}

void ItemAwardManager::Taken(DWORD dwAwardID, DWORD dwItemID)
{
	//SetAwardMap::const_iterator it = m_map_award.find(dwAwardID);
	itertype(m_map_award) it = m_map_award.find(dwAwardID);

	if (it == m_map_award.end())
	{
		sys_log(0, "ITEM_AWARD: Taken ID not exist %lu", dwAwardID);
		return;
	}

	TItemAward* k = it->second;
	k->bTaken = true;

	//
	// Update taken_time in database to prevent not to give him again.
	//
	char szQuery[QUERY_MAX_LEN];

	snprintf(szQuery, sizeof(szQuery), "UPDATE item_award%s SET `taken_time` = NOW(), `item_id` = %u WHERE `id` = %u AND `taken_time` IS NULL",
		GetTablePostfix(), dwItemID, dwAwardID);

	CDBManager::instance().ReturnQuery(szQuery, QID_ITEM_AWARD_TAKEN, 0, NULL);
}

std::map<DWORD, TItemAward*>& ItemAwardManager::GetMapAward()
{
	return m_map_award;
}

std::map<std::string, ItemAwardSet>& ItemAwardManager::GetMapkSetAwardByLogin()
{
	return m_map_kSetAwardByLogin;
}

#if defined(__EXTENDED_ITEM_AWARD__)
const int8_t ItemAwardManager::GetItemAttributeSetIndex(const uint8_t bItemType, const uint8_t bItemSubType)
{
	using TAttributeMapT = std::map<uint8_t, std::map<uint8_t, int8_t>>;
	static const TAttributeMapT mapAttrSetFields =
	{
		{ ITEM_WEAPON,
			{
				{ WEAPON_ARROW, ITEM_ATTRIBUTE_NONE },
#if defined(__QUIVER_SYSTEM__)
				{ WEAPON_QUIVER, ITEM_ATTRIBUTE_NONE },
#endif
			}
		},

		{ ITEM_ARMOR,
			{
				{ ARMOR_BODY, ATTRIBUTE_SET_BODY },
				{ ARMOR_WRIST, ATTRIBUTE_SET_WRIST },
				{ ARMOR_FOOTS, ATTRIBUTE_SET_FOOTS },
				{ ARMOR_NECK, ATTRIBUTE_SET_NECK },
				{ ARMOR_HEAD, ATTRIBUTE_SET_HEAD },
				{ ARMOR_SHIELD, ATTRIBUTE_SET_SHIELD },
				{ ARMOR_EAR, ATTRIBUTE_SET_EAR },
#if defined(__PENDANT_SYSTEM__)
				{ ARMOR_PENDANT, ATTRIBUTE_SET_PENDANT },
#endif
#if defined(__GLOVE_SYSTEM__)
				{ ARMOR_GLOVE, ATTRIBUTE_SET_GLOVE },
#endif
			}
		},

		{ ITEM_COSTUME,
			{
				{ COSTUME_BODY, ATTRIBUTE_SET_COSTUME_BODY },
				{ COSTUME_HAIR, ATTRIBUTE_SET_COSTUME_HAIR },
#if defined(__MOUNT_COSTUME_SYSTEM__)
				{ COSTUME_MOUNT, ITEM_ATTRIBUTE_NONE },
#endif
#if defined(__ACCE_COSTUME_SYSTEM__)
				{ COSTUME_ACCE, ITEM_ATTRIBUTE_NONE },
#endif
#if defined(__WEAPON_COSTUME_SYSTEM__)
				{ COSTUME_WEAPON, ATTRIBUTE_SET_COSTUME_WEAPON },
#endif
			}
		},
	};

	const auto c_iter_type = mapAttrSetFields.find(bItemType);
	if (c_iter_type != mapAttrSetFields.end())
	{
		const auto c_iter_sub_type = c_iter_type->second.find(bItemSubType);
		if (c_iter_sub_type != c_iter_type->second.end())
			return c_iter_sub_type->second;
		else if (c_iter_type->first == ITEM_WEAPON)
			return ATTRIBUTE_SET_WEAPON;
	}

	return ITEM_ATTRIBUTE_NONE;
}

void ItemAwardManager::CheckItemAttributes(TItemAward& rkItemAward, const TItemTable& rkItemTable, const std::vector<TItemAttrTable> vec_itemAttrTable)
{
	const uint8_t bItemType = rkItemTable.bType;
	const uint8_t bItemSubType = rkItemTable.bSubType;
	const int8_t iAttributeSet = GetItemAttributeSetIndex(bItemType, bItemSubType);
	if (iAttributeSet != ITEM_ATTRIBUTE_NONE)
	{
		for (size_t i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			const uint8_t bApplyType = rkItemAward.aAttr[i].bType;
			const bool bIsAddonTypeItem = (bApplyType == APPLY_SKILL_DAMAGE_BONUS || bApplyType == APPLY_NORMAL_HIT_DAMAGE_BONUS);

			const TItemAttrTable* pkAttrTable = NULL;
			for (size_t j = 0; j < vec_itemAttrTable.size(); ++j)
			{
				const TItemAttrTable& rkAttrTable = vec_itemAttrTable.at(j);
				if (rkAttrTable.dwApplyIndex == bApplyType && !bIsAddonTypeItem)
				{
					const uint8_t bAttrLevel = rkAttrTable.bMaxLevelBySet[iAttributeSet];
					if (bAttrLevel > 0)
					{
						pkAttrTable = &rkAttrTable;
						break;
					}
				}
			}

			if (pkAttrTable)
			{
				const int16_t sMinValue = static_cast<int16_t>(pkAttrTable->lValues[0]);
				const int16_t sMaxValue = static_cast<int16_t>(pkAttrTable->lValues[(sizeof(pkAttrTable->lValues) / sizeof(pkAttrTable->lValues[0])) - 1]);
				rkItemAward.aAttr[i].sValue = MINMAX(sMinValue, rkItemAward.aAttr[i].sValue, sMaxValue);
			}
			else if (!bIsAddonTypeItem)
			{
				rkItemAward.aAttr[i].bType = 0;
				rkItemAward.aAttr[i].sValue = 0;
			}
		}
	}
}

void ItemAwardManager::CheckItemAddonType(TItemAward& rkItemAward, const TItemTable& rkItemTable)
{
	const bool bIsAddonTypeItem = (rkItemTable.sAddonType == -1);
	if (!bIsAddonTypeItem)
		return;

	bool bHasBonus = false;
	for (size_t i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		const uint8_t bType = rkItemAward.aAttr[i].bType;
		const int16_t sValue = rkItemAward.aAttr[i].sValue;

		if ((bType == APPLY_SKILL_DAMAGE_BONUS || bType == APPLY_NORMAL_HIT_DAMAGE_BONUS) && sValue)
		{
			bHasBonus = true;
			break;
		}
	}

	if (!bHasBonus)
	{
		const int16_t sApplySkillDamageValue = MINMAX(-30, static_cast<int16_t>((gauss_random(0, 5) + 0.5f)), 30);
		const int16_t sApplyNormalHitValue = std::abs(sApplySkillDamageValue) <= 20 ?
			(-2 * sApplySkillDamageValue) + std::abs(number(-8, 8) + number(-8, 8)) + number(1, 4) :
			(-2 * sApplySkillDamageValue) + number(1, 5);

		rkItemAward.aAttr[0].bType = APPLY_NORMAL_HIT_DAMAGE_BONUS;
		rkItemAward.aAttr[0].sValue = sApplyNormalHitValue;
		rkItemAward.aAttr[1].bType = APPLY_SKILL_DAMAGE_BONUS;
		rkItemAward.aAttr[1].sValue = sApplySkillDamageValue;
	}
}

void ItemAwardManager::CheckItemSkillBook(TItemAward& rkItemAward, const std::vector<TSkillTable> vec_skillTable)
{
	const bool bIsBookItem = (rkItemAward.dwVnum == ITEM_SKILL_VNUM || rkItemAward.dwVnum == ITEM_SKILLFORGET_VNUM);
	if (!bIsBookItem)
		return;

	uint32_t dwSocket0SkillVnum = rkItemAward.dwSocket0;
	bool bHasSkill = false;
	for (size_t i = 0; i < vec_skillTable.size(); ++i)
	{
		if (vec_skillTable[i].dwVnum == dwSocket0SkillVnum)
		{
			bHasSkill = true;
			break;
		}
	}

	if (!bHasSkill)
	{
		do
		{
			const TSkillTable* pkSkillTable = &vec_skillTable.at(number(0, vec_skillTable.size() - 1));
			if (!pkSkillTable)
				continue;

			const uint32_t dwSkillVnum = pkSkillTable->dwVnum;
			const uint8_t bSkillType = pkSkillTable->bType;

			const bool bIsPCSkill = (JOB_WARRIOR + 1 <= bSkillType && bSkillType <= JOB_SHAMAN + 1) || bSkillType == 7;
			if (!bIsPCSkill)
				continue;

			dwSocket0SkillVnum = dwSkillVnum;
			break;
		} while (true);

		rkItemAward.dwSocket0 = dwSocket0SkillVnum;
	}
}

void ItemAwardManager::CheckItemCount(TItemAward& rkItemAward, const TItemTable& rkItemTable)
{
	const bool bIsStackableItem = (rkItemTable.dwFlags & ITEM_FLAG_STACKABLE);
	if (rkItemAward.dwCount > 1 && !bIsStackableItem)
		rkItemAward.dwCount = 1;
	else
		rkItemAward.dwCount = MINMAX(1, rkItemAward.dwCount, ITEM_MAX_COUNT);
}

void ItemAwardManager::CheckItemBlend(TItemAward& rkItemAward, const TItemTable& rkItemTable)
{
	const bool bIsBlendItem = (rkItemTable.bType == ITEM_BLEND);
	if (!bIsBlendItem)
		return;

	const uint32_t bApplyType = rkItemAward.dwSocket0;
	const uint32_t bApplyValue = rkItemAward.dwSocket1;
	const uint32_t dwApplyDuration = rkItemAward.dwSocket2;

	if (bApplyType == 0 || bApplyValue == 0 || dwApplyDuration == 0)
		sys_err("ItemAwardManager: Unknown sockets for ITEM_BLEND.");
}
#endif
