#include "stdafx.h"
#include "Cache.h"

#include "QID.h"
#include "ClientManager.h"
#include "Main.h"

extern CPacketInfo g_item_info;
extern int g_iPlayerCacheFlushSeconds;
extern int g_iItemCacheFlushSeconds;
extern int g_test_server;
// MYSHOP_PRICE_LIST
extern int g_iItemPriceListTableCacheFlushSeconds;
// END_OF_MYSHOP_PRICE_LIST

extern int g_item_count;

CItemCache::CItemCache()
{
	m_expireTime = MIN(1800, g_iItemCacheFlushSeconds);
}

CItemCache::~CItemCache()
{
}

// 이거 이상한데...
// Delete를 했으면, Cache도 해제해야 하는것 아닌가???
// 근데 Cache를 해제하는 부분이 없어.
// 못 찾은 건가?
// 이렇게 해놓으면, 계속 시간이 될 때마다 아이템을 계속 지워...
// 이미 사라진 아이템인데... 확인사살??????
// fixme
// by rtsummit
void CItemCache::Delete()
{
	if (m_data.vnum == 0)
		return;

	// char szQuery[QUERY_MAX_LEN];
	// szQuery[QUERY_MAX_LEN] = '\0';
	if (g_test_server)
		sys_log(0, "ItemCache::Delete : DELETE %u", m_data.id);

	m_data.vnum = 0;
	m_bNeedQuery = true;
	m_lastUpdateTime = time(0);
	OnFlush();

	// m_bNeedQuery = false;
	// m_lastUpdateTime = time(0) - m_expireTime;
}

void CItemCache::OnFlush()
{
	if (m_data.vnum == 0) // vnum 이 0이면 삭제하라고 표시된 것이다.
	{
		char szQuery[QUERY_MAX_LEN];
		snprintf(szQuery, sizeof(szQuery), "DELETE FROM item%s WHERE `id` = %u", GetTablePostfix(), m_data.id);
		CDBManager::instance().ReturnQuery(szQuery, QID_ITEM_DESTROY, 0, NULL);

		if (g_test_server)
			sys_log(0, "ItemCache::Flush : DELETE %u %s", m_data.id, szQuery);
	}
	else
	{
		long alSockets[ITEM_SOCKET_MAX_NUM];
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
#if defined(__ITEM_APPLY_RANDOM__)
		TPlayerItemAttribute aApplyRandom[ITEM_APPLY_MAX_NUM];
		bool isApplyRandom = false;
#endif
		bool isSocket = false, isAttr = false;

		memset(&alSockets, 0, sizeof(long) * ITEM_SOCKET_MAX_NUM);
		memset(&aAttr, 0, sizeof(TPlayerItemAttribute) * ITEM_ATTRIBUTE_MAX_NUM);
#if defined(__ITEM_APPLY_RANDOM__)
		memset(&aApplyRandom, 0, sizeof(TPlayerItemAttribute) * ITEM_APPLY_MAX_NUM);
#endif

		TPlayerItem* p = &m_data;

		if (memcmp(alSockets, p->alSockets, sizeof(long) * ITEM_SOCKET_MAX_NUM))
			isSocket = true;

		if (memcmp(aAttr, p->aAttr, sizeof(TPlayerItemAttribute) * ITEM_ATTRIBUTE_MAX_NUM))
			isAttr = true;

#if defined(__ITEM_APPLY_RANDOM__)
		if (memcmp(aApplyRandom, p->aApplyRandom, sizeof(TPlayerItemAttribute) * ITEM_APPLY_MAX_NUM))
			isApplyRandom = true;
#endif

		char szColumns[QUERY_MAX_LEN];
		char szValues[QUERY_MAX_LEN];
		char szUpdate[QUERY_MAX_LEN];

		int iLen = snprintf(szColumns, sizeof(szColumns),
			"`id`, `owner_id`, `window`, `pos`, `count`, `vnum`"
#if defined(__SOUL_BIND_SYSTEM__)
			", `soulbind`"
#endif
#if defined(__CHANGE_LOOK_SYSTEM__)
			", `transmutation`"
#endif
		);

		int iValueLen = snprintf(szValues, sizeof(szValues),
			"%u, %u, %d, %d, %u, %u"
#if defined(__SOUL_BIND_SYSTEM__)
			", %ld"
#endif
#if defined(__CHANGE_LOOK_SYSTEM__)
			", %u"
#endif
			, p->id, p->owner, p->window, p->pos, p->count, p->vnum
#if defined(__SOUL_BIND_SYSTEM__)
			, p->soulbind
#endif
#if defined(__CHANGE_LOOK_SYSTEM__)
			, p->dwTransmutationVnum
#endif
		);

		int iUpdateLen = snprintf(szUpdate, sizeof(szUpdate),
			"`owner_id` = %u, `window` = %d, `pos` = %d, `count` = %u, `vnum` = %u"
#if defined(__SOUL_BIND_SYSTEM__)
			", `soulbind` = %ld"
#endif
#if defined(__CHANGE_LOOK_SYSTEM__)
			", `transmutation` = %u"
#endif
			, p->owner, p->window, p->pos, p->count, p->vnum
#if defined(__SOUL_BIND_SYSTEM__)
			, p->soulbind
#endif
#if defined(__CHANGE_LOOK_SYSTEM__)
			, p->dwTransmutationVnum
#endif
		);

		if (isSocket)
		{
			iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen,
				", `socket0`"
				", `socket1`"
				", `socket2`"
#if defined(__ITEM_SOCKET5__)
				", `socket3`"
				", `socket4`"
#endif
			);
			iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,
				", %lu"
				", %lu"
				", %lu"
#if defined(__ITEM_SOCKET5__)
				", %lu"
				", %lu"
#endif
				, p->alSockets[0]
				, p->alSockets[1]
				, p->alSockets[2]
#if defined(__ITEM_SOCKET5__)
				, p->alSockets[3]
				, p->alSockets[4]
#endif
			);
			iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,
				", `socket0` = %lu"
				", `socket1` = %lu"
				", `socket2` = %lu"
#if defined(__ITEM_SOCKET5__)
				", `socket3` = %lu"
				", `socket4` = %lu"
#endif
				, p->alSockets[0]
				, p->alSockets[1]
				, p->alSockets[2]
#if defined(__ITEM_SOCKET5__)
				, p->alSockets[3]
				, p->alSockets[4]
#endif
			);
		}

#if defined(__ITEM_APPLY_RANDOM__)
		if (isApplyRandom)
		{
			iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen,
				", `apply_type0`, `apply_value0`, `apply_path0`"
				", `apply_type1`, `apply_value1`, `apply_path1`"
				", `apply_type2`, `apply_value2`, `apply_path2`"
				", `apply_type3`, `apply_value3`, `apply_path3`"
			);

			iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,
				", %d, %d, %d"
				", %d, %d, %d"
				", %d, %d, %d"
				", %d, %d, %d"
				, p->aApplyRandom[0].bType, p->aApplyRandom[0].sValue, p->aApplyRandom[0].bPath
				, p->aApplyRandom[1].bType, p->aApplyRandom[1].sValue, p->aApplyRandom[1].bPath
				, p->aApplyRandom[2].bType, p->aApplyRandom[2].sValue, p->aApplyRandom[2].bPath
				, p->aApplyRandom[3].bType, p->aApplyRandom[3].sValue, p->aApplyRandom[3].bPath
			);

			iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,
				", `apply_type0` = %d, `apply_value0` = %d, `apply_path0` = %d"
				", `apply_type1` = %d, `apply_value1` = %d, `apply_path1` = %d"
				", `apply_type2` = %d, `apply_value2` = %d, `apply_path2` = %d"
				", `apply_type3` = %d, `apply_value3` = %d, `apply_path3` = %d"
				, p->aApplyRandom[0].bType, p->aApplyRandom[0].sValue, p->aApplyRandom[3].bPath
				, p->aApplyRandom[1].bType, p->aApplyRandom[1].sValue, p->aApplyRandom[3].bPath
				, p->aApplyRandom[2].bType, p->aApplyRandom[2].sValue, p->aApplyRandom[3].bPath
				, p->aApplyRandom[3].bType, p->aApplyRandom[3].sValue, p->aApplyRandom[3].bPath
			);
		}
#endif

		if (isAttr)
		{
			iLen += snprintf(szColumns + iLen, sizeof(szColumns) - iLen,
				", `attrtype0`, `attrvalue0`"
				", `attrtype1`, `attrvalue1`"
				", `attrtype2`, `attrvalue2`"
				", `attrtype3`, `attrvalue3`"
				", `attrtype4`, `attrvalue4`"
				", `attrtype5`, `attrvalue5`"
				", `attrtype6`, `attrvalue6`"
			);

			iValueLen += snprintf(szValues + iValueLen, sizeof(szValues) - iValueLen,
				", %d, %d"
				", %d, %d"
				", %d, %d"
				", %d, %d"
				", %d, %d"
				", %d, %d"
				", %d, %d"
				, p->aAttr[0].bType, p->aAttr[0].sValue
				, p->aAttr[1].bType, p->aAttr[1].sValue
				, p->aAttr[2].bType, p->aAttr[2].sValue
				, p->aAttr[3].bType, p->aAttr[3].sValue
				, p->aAttr[4].bType, p->aAttr[4].sValue
				, p->aAttr[5].bType, p->aAttr[5].sValue
				, p->aAttr[6].bType, p->aAttr[6].sValue
			);

			iUpdateLen += snprintf(szUpdate + iUpdateLen, sizeof(szUpdate) - iUpdateLen,
				", `attrtype0` = %d, `attrvalue0` = %d"
				", `attrtype1` = %d, `attrvalue1` = %d"
				", `attrtype2` = %d, `attrvalue2` = %d"
				", `attrtype3` = %d, `attrvalue3` = %d"
				", `attrtype4` = %d, `attrvalue4` = %d"
				", `attrtype5` = %d, `attrvalue5` = %d"
				", `attrtype6` = %d, `attrvalue6` = %d"
				, p->aAttr[0].bType, p->aAttr[0].sValue
				, p->aAttr[1].bType, p->aAttr[1].sValue
				, p->aAttr[2].bType, p->aAttr[2].sValue
				, p->aAttr[3].bType, p->aAttr[3].sValue
				, p->aAttr[4].bType, p->aAttr[4].sValue
				, p->aAttr[5].bType, p->aAttr[5].sValue
				, p->aAttr[6].bType, p->aAttr[6].sValue
			);
		}

		char szItemQuery[QUERY_MAX_LEN + QUERY_MAX_LEN];
		snprintf(szItemQuery, sizeof(szItemQuery), "REPLACE INTO item%s (%s) VALUES(%s)", GetTablePostfix(), szColumns, szValues);

		if (g_test_server)
			sys_log(0, "ItemCache::Flush :REPLACE (%s)", szItemQuery);

		CDBManager::instance().ReturnQuery(szItemQuery, QID_ITEM_SAVE, 0, NULL);

		// g_item_info.Add(p->vnum);
		++g_item_count;
	}

	m_bNeedQuery = false;
}

#if defined(__SKILL_COLOR_SYSTEM__)
//
// CSKillColorCache
//

extern int g_iSkillColorCacheFlushSeconds;

CSKillColorCache::CSKillColorCache()
{
	m_expireTime = MIN(1800, g_iSkillColorCacheFlushSeconds);
}

CSKillColorCache::~CSKillColorCache()
{
}

void CSKillColorCache::OnFlush()
{
	char szQuery[QUERY_MAX_LEN];
	snprintf(szQuery, sizeof(szQuery),
		"REPLACE INTO skill_color%s (`player_id`"
		// Skill Slots
		", `skillSlot1_Col1`, `skillSlot1_Col2`, `skillSlot1_Col3`, `skillSlot1_Col4`, `skillSlot1_Col5`"
		", `skillSlot2_Col1`, `skillSlot2_Col2`, `skillSlot2_Col3`, `skillSlot2_Col4`, `skillSlot2_Col5`"
		", `skillSlot3_Col1`, `skillSlot3_Col2`, `skillSlot3_Col3`, `skillSlot3_Col4`, `skillSlot3_Col5`"
		", `skillSlot4_Col1`, `skillSlot4_Col2`, `skillSlot4_Col3`, `skillSlot4_Col4`, `skillSlot4_Col5`"
		", `skillSlot5_Col1`, `skillSlot5_Col2`, `skillSlot5_Col3`, `skillSlot5_Col4`, `skillSlot5_Col5`"
		", `skillSlot6_Col1`, `skillSlot6_Col2`, `skillSlot6_Col3`, `skillSlot6_Col4`, `skillSlot6_Col5`"
#if defined(__9TH_SKILL__)
		", `skillSlot7_Col1`, `skillSlot7_Col2`, `skillSlot7_Col3`, `skillSlot7_Col4`, `skillSlot7_Col5`"
		", `skillSlot8_Col1`, `skillSlot8_Col2`, `skillSlot8_Col3`, `skillSlot8_Col4`, `skillSlot8_Col5`"
		", `skillSlot9_Col1`, `skillSlot9_Col2`, `skillSlot9_Col3`, `skillSlot9_Col4`, `skillSlot9_Col5`"
		", `skillSlot10_Col1`, `skillSlot10_Col2`, `skillSlot10_Col3`, `skillSlot10_Col4`, `skillSlot10_Col5`"
		// Buff Skills
		", `buffSkill1_Col1`, `buffSkill1_Col2`, `buffSkill1_Col3`, `buffSkill1_Col4`, `buffSkill1_Col5`"
		", `buffSkill2_Col1`, `buffSkill2_Col2`, `buffSkill2_Col3`, `buffSkill2_Col4`, `buffSkill2_Col5`"
		", `buffSkill3_Col1`, `buffSkill3_Col2`, `buffSkill3_Col3`, `buffSkill3_Col4`, `buffSkill3_Col5`"
		", `buffSkill4_Col1`, `buffSkill4_Col2`, `buffSkill4_Col3`, `buffSkill4_Col4`, `buffSkill4_Col5`"
		", `buffSkill5_Col1`, `buffSkill5_Col2`, `buffSkill5_Col3`, `buffSkill5_Col4`, `buffSkill5_Col5`"
		", `buffSkill6_Col1`, `buffSkill6_Col2`, `buffSkill6_Col3`, `buffSkill6_Col4`, `buffSkill6_Col5`"
#else
		// Buff Skills
		", `buffSkill1_Col1`, `buffSkill1_Col2`, `buffSkill1_Col3`, `buffSkill1_Col4`, `buffSkill1_Col5`"
		", `buffSkill2_Col1`, `buffSkill2_Col2`, `buffSkill2_Col3`, `buffSkill2_Col4`, `buffSkill2_Col5`"
		", `buffSkill3_Col1`, `buffSkill3_Col2`, `buffSkill3_Col3`, `buffSkill3_Col4`, `buffSkill3_Col5`"
		", `buffSkill4_Col1`, `buffSkill4_Col2`, `buffSkill4_Col3`, `buffSkill4_Col4`, `buffSkill4_Col5`"
		", `buffSkill5_Col1`, `buffSkill5_Col2`, `buffSkill5_Col3`, `buffSkill5_Col4`, `buffSkill5_Col5`"
		", `buffSkill6_Col1`, `buffSkill6_Col2`, `buffSkill6_Col3`, `buffSkill6_Col4`, `buffSkill6_Col5`"
#endif
		") "
		"VALUES (%d"
		// Skill Slots <Slot, Col1, Col2, Col3, Col4, Col5>
		", %d, %d, %d, %d, %d" // Skill Slot 1
		", %d, %d, %d, %d, %d" // Skill Slot 2
		", %d, %d, %d, %d, %d" // Skill Slot 3
		", %d, %d, %d, %d, %d" // Skill Slot 4
		", %d, %d, %d, %d, %d" // Skill Slot 5
		", %d, %d, %d, %d, %d" // Skill Slot 6 (End of Skills)
#if defined(__9TH_SKILL__)
		", %d, %d, %d, %d, %d" // Skill Slot 7 (Unused)
		", %d, %d, %d, %d, %d" // Skill Slot 8 (Unused)
		", %d, %d, %d, %d, %d" // Skill Slot 9 (Conqueror)
		", %d, %d, %d, %d, %d" // Skill Slot 10 (End of Skills)
		// Buff Skills
		", %d, %d, %d, %d, %d" // Buff Skill 11 (Shaman Group 1)
		", %d, %d, %d, %d, %d" // Buff Skill 12 (Shaman Group 1)
		", %d, %d, %d, %d, %d" // Buff Skill 13 (Shaman Group 1)
		", %d, %d, %d, %d, %d" // Buff Skill 14 (Shaman Group 2)
		", %d, %d, %d, %d, %d" // Buff Skill 15 (Shaman Group 2)
		", %d, %d, %d, %d, %d" // Buff Skill 16 (Wolfman)
#else
		// Buff Skills
		", %d, %d, %d, %d, %d" // Buff Skill 7 (Shaman Group 1)
		", %d, %d, %d, %d, %d" // Buff Skill 8 (Shaman Group 1)
		", %d, %d, %d, %d, %d" // Buff Skill 9 (Shaman Group 1)
		", %d, %d, %d, %d, %d" // Buff Skill 10 (Shaman Group 2)
		", %d, %d, %d, %d, %d" // Buff Skill 11 (Shaman Group 2)
		", %d, %d, %d, %d, %d" // Buff Skill 12 (Wolfman)
#endif
		")", GetTablePostfix(), m_data.dwPlayerID
		, m_data.dwSkillColor[0][0], m_data.dwSkillColor[0][1], m_data.dwSkillColor[0][2], m_data.dwSkillColor[0][3], m_data.dwSkillColor[0][4]
		, m_data.dwSkillColor[1][0], m_data.dwSkillColor[1][1], m_data.dwSkillColor[1][2], m_data.dwSkillColor[1][3], m_data.dwSkillColor[1][4]
		, m_data.dwSkillColor[2][0], m_data.dwSkillColor[2][1], m_data.dwSkillColor[2][2], m_data.dwSkillColor[2][3], m_data.dwSkillColor[2][4]
		, m_data.dwSkillColor[3][0], m_data.dwSkillColor[3][1], m_data.dwSkillColor[3][2], m_data.dwSkillColor[3][3], m_data.dwSkillColor[3][4]
		, m_data.dwSkillColor[4][0], m_data.dwSkillColor[4][1], m_data.dwSkillColor[4][2], m_data.dwSkillColor[4][3], m_data.dwSkillColor[4][4]
		, m_data.dwSkillColor[5][0], m_data.dwSkillColor[5][1], m_data.dwSkillColor[5][2], m_data.dwSkillColor[5][3], m_data.dwSkillColor[5][4]
#if defined(__9TH_SKILL__)
		, m_data.dwSkillColor[6][0], m_data.dwSkillColor[6][1], m_data.dwSkillColor[6][2], m_data.dwSkillColor[6][3], m_data.dwSkillColor[6][4]
		, m_data.dwSkillColor[7][0], m_data.dwSkillColor[7][1], m_data.dwSkillColor[7][2], m_data.dwSkillColor[7][3], m_data.dwSkillColor[7][4]
		, m_data.dwSkillColor[8][0], m_data.dwSkillColor[8][1], m_data.dwSkillColor[8][2], m_data.dwSkillColor[8][3], m_data.dwSkillColor[8][4]
		, m_data.dwSkillColor[9][0], m_data.dwSkillColor[9][1], m_data.dwSkillColor[9][2], m_data.dwSkillColor[9][3], m_data.dwSkillColor[9][4]
		, m_data.dwSkillColor[10][0], m_data.dwSkillColor[10][1], m_data.dwSkillColor[10][2], m_data.dwSkillColor[10][3], m_data.dwSkillColor[10][4]
		, m_data.dwSkillColor[11][0], m_data.dwSkillColor[11][1], m_data.dwSkillColor[11][2], m_data.dwSkillColor[11][3], m_data.dwSkillColor[11][4]
		, m_data.dwSkillColor[12][0], m_data.dwSkillColor[12][1], m_data.dwSkillColor[12][2], m_data.dwSkillColor[12][3], m_data.dwSkillColor[12][4]
		, m_data.dwSkillColor[13][0], m_data.dwSkillColor[13][1], m_data.dwSkillColor[13][2], m_data.dwSkillColor[13][3], m_data.dwSkillColor[13][4]
		, m_data.dwSkillColor[14][0], m_data.dwSkillColor[14][1], m_data.dwSkillColor[14][2], m_data.dwSkillColor[14][3], m_data.dwSkillColor[14][4]
		, m_data.dwSkillColor[15][0], m_data.dwSkillColor[15][1], m_data.dwSkillColor[15][2], m_data.dwSkillColor[15][3], m_data.dwSkillColor[15][4]
#else
		, m_data.dwSkillColor[6][0], m_data.dwSkillColor[6][1], m_data.dwSkillColor[6][2], m_data.dwSkillColor[6][3], m_data.dwSkillColor[6][4],
		, m_data.dwSkillColor[7][0], m_data.dwSkillColor[7][1], m_data.dwSkillColor[7][2], m_data.dwSkillColor[7][3], m_data.dwSkillColor[7][4],
		, m_data.dwSkillColor[8][0], m_data.dwSkillColor[8][1], m_data.dwSkillColor[8][2], m_data.dwSkillColor[8][3], m_data.dwSkillColor[8][4],
		, m_data.dwSkillColor[9][0], m_data.dwSkillColor[9][1], m_data.dwSkillColor[9][2], m_data.dwSkillColor[9][3], m_data.dwSkillColor[9][4],
		, m_data.dwSkillColor[10][0], m_data.dwSkillColor[10][1], m_data.dwSkillColor[10][2], m_data.dwSkillColor[10][3], m_data.dwSkillColor[10][4]
		, m_data.dwSkillColor[11][0], m_data.dwSkillColor[11][1], m_data.dwSkillColor[11][2], m_data.dwSkillColor[11][3], m_data.dwSkillColor[11][4]
#endif
	);

	CDBManager::instance().ReturnQuery(szQuery, QID_SKILL_COLOR_SAVE, 0, NULL);

	if (g_test_server)
		sys_log(0, "SkillColorCache::Flush :REPLACE %u (%s)", m_data.dwPlayerID, szQuery);

	m_bNeedQuery = false;
}
#endif

//
// CPlayerTableCache
//
CPlayerTableCache::CPlayerTableCache()
{
	m_expireTime = MIN(1800, g_iPlayerCacheFlushSeconds);
}

CPlayerTableCache::~CPlayerTableCache()
{
}

void CPlayerTableCache::OnFlush()
{
	if (g_test_server)
		sys_log(0, "PlayerTableCache::Flush : %s", m_data.name);

	char szQuery[QUERY_MAX_LEN];
	CreatePlayerSaveQuery(szQuery, sizeof(szQuery), &m_data);
	CDBManager::instance().ReturnQuery(szQuery, QID_PLAYER_SAVE, 0, NULL);
}

// MYSHOP_PRICE_LIST
//
// CItemPriceListTableCache class implementation
//

const int CItemPriceListTableCache::s_nMinFlushSec = 1800;

CItemPriceListTableCache::CItemPriceListTableCache()
{
	m_expireTime = MIN(s_nMinFlushSec, g_iItemPriceListTableCacheFlushSeconds);
}

CItemPriceListTableCache::~CItemPriceListTableCache()
{
}

void CItemPriceListTableCache::UpdateList(const TItemPriceListTable* pUpdateList)
{
	//
	// 이미 캐싱된 아이템과 중복된 아이템을 찾고 중복되지 않는 이전 정보는 tmpvec 에 넣는다.
	//

	std::vector<TItemPriceInfo> tmpvec;

	for (uint idx = 0; idx < m_data.byCount; ++idx)
	{
		const TItemPriceInfo* pos = pUpdateList->aPriceInfo;
		for (; pos != pUpdateList->aPriceInfo + pUpdateList->byCount && m_data.aPriceInfo[idx].dwVnum != pos->dwVnum; ++pos);

		if (pos == pUpdateList->aPriceInfo + pUpdateList->byCount)
			tmpvec.push_back(m_data.aPriceInfo[idx]);
	}

	//
	// pUpdateList 를 m_data 에 복사하고 남은 공간을 tmpvec 의 앞에서 부터 남은 만큼 복사한다.
	//

	if (pUpdateList->byCount > SHOP_PRICELIST_MAX_NUM)
	{
		sys_err("Count overflow!");
		return;
	}

	m_data.byCount = pUpdateList->byCount;

	thecore_memcpy(m_data.aPriceInfo, pUpdateList->aPriceInfo, sizeof(TItemPriceInfo) * pUpdateList->byCount);

	int nDeletedNum; // 삭제된 가격정보의 갯수

	if (pUpdateList->byCount < SHOP_PRICELIST_MAX_NUM)
	{
		size_t sizeAddOldDataSize = SHOP_PRICELIST_MAX_NUM - pUpdateList->byCount;

		if (tmpvec.size() < sizeAddOldDataSize)
			sizeAddOldDataSize = tmpvec.size();

		thecore_memcpy(m_data.aPriceInfo + pUpdateList->byCount, &tmpvec[0], sizeof(TItemPriceInfo) * sizeAddOldDataSize);
		m_data.byCount += static_cast<BYTE>(sizeAddOldDataSize);

		nDeletedNum = static_cast<int>(tmpvec.size() - sizeAddOldDataSize);
	}
	else
		nDeletedNum = tmpvec.size();

	m_bNeedQuery = true;

	sys_log(0, "ItemPriceListTableCache::UpdateList : OwnerID[%u] Update [%u] Items, Delete [%u] Items, Total [%u] Items",
		m_data.dwOwnerID, pUpdateList->byCount, nDeletedNum, m_data.byCount);
}

void CItemPriceListTableCache::OnFlush()
{
	char szQuery[QUERY_MAX_LEN];

	//
	// 이 캐시의 소유자에 대한 기존에 DB 에 저장된 아이템 가격정보를 모두 삭제한다.
	//

	snprintf(szQuery, sizeof(szQuery), "DELETE FROM myshop_pricelist%s WHERE `owner_id` = %u", GetTablePostfix(), m_data.dwOwnerID);
	CDBManager::instance().ReturnQuery(szQuery, QID_ITEMPRICE_DESTROY, 0, NULL);

	//
	// 캐시의 내용을 모두 DB 에 쓴다.
	//

	for (int idx = 0; idx < m_data.byCount; ++idx)
	{
		snprintf(szQuery, sizeof(szQuery), "REPLACE myshop_pricelist%s (`owner_id`, `item_vnum`, `price`) VALUES(%u, %u, %u)",
			GetTablePostfix(), m_data.dwOwnerID, m_data.aPriceInfo[idx].dwVnum, m_data.aPriceInfo[idx].dwPrice);
		CDBManager::instance().ReturnQuery(szQuery, QID_ITEMPRICE_SAVE, 0, NULL);
	}

	sys_log(0, "ItemPriceListTableCache::Flush : OwnerID[%u] Update [%u]Items", m_data.dwOwnerID, m_data.byCount);

	m_bNeedQuery = false;
}
// END_OF_MYSHOP_PRICE_LIST
