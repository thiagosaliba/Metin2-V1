#include "stdafx.h"
#include "constants.h"
#include "log.h"
#include "item.h"
#include "char.h"
#include "desc.h"
#include "item_manager.h"
#include "config.h"

const int MAX_NORM_ATTR_NUM = ITEM_MANAGER::MAX_NORM_ATTR_NUM;
const int MAX_RARE_ATTR_NUM = ITEM_MANAGER::MAX_RARE_ATTR_NUM;

int CItem::GetAttributeSetIndex()
{
	if (GetType() == ITEM_WEAPON)
	{
		if (GetSubType() == WEAPON_ARROW)
			return -1;

#if defined(__QUIVER_SYSTEM__)
		if (GetSubType() == WEAPON_QUIVER)
			return -1;
#endif

		return ATTRIBUTE_SET_WEAPON;
	}

#if !defined(__COSTUME_SYSTEM__)
	if (GetType() == ITEM_ARMOR || GetType() == ITEM_COSTUME)
#else
	if (GetType() == ITEM_ARMOR)
#endif
	{
		switch (GetSubType())
		{
		case ARMOR_BODY:
		// case COSTUME_BODY: // 코스츔 갑옷은 일반 갑옷과 동일한 Attribute Set을 이용하여 랜덤속성 붙음 (ARMOR_BODY == COSTUME_BODY)
			return ATTRIBUTE_SET_BODY;

		case ARMOR_WRIST:
			return ATTRIBUTE_SET_WRIST;

		case ARMOR_FOOTS:
			return ATTRIBUTE_SET_FOOTS;

		case ARMOR_NECK:
			return ATTRIBUTE_SET_NECK;

		case ARMOR_HEAD:
		// case COSTUME_HAIR: // 코스츔 헤어는 일반 투구 아이템과 동일한 Attribute Set을 이용하여 랜덤속성 붙음 (ARMOR_HEAD == COSTUME_HAIR)
			return ATTRIBUTE_SET_HEAD;

		case ARMOR_SHIELD:
			return ATTRIBUTE_SET_SHIELD;

		case ARMOR_EAR:
			return ATTRIBUTE_SET_EAR;

#if defined(__PENDANT_SYSTEM__)
		case ARMOR_PENDANT:
			return ATTRIBUTE_SET_PENDANT;
#endif

#if defined(__GLOVE_SYSTEM__)
		case ARMOR_GLOVE:
			return ATTRIBUTE_SET_GLOVE;
#endif
		}
	}

#if defined(__COSTUME_SYSTEM__)
	if (GetType() == ITEM_COSTUME)
	{
		switch (GetSubType())
		{
		case COSTUME_BODY:
			return ATTRIBUTE_SET_COSTUME_BODY;

		case COSTUME_HAIR:
			return ATTRIBUTE_SET_COSTUME_HAIR;

#if defined(__MOUNT_COSTUME_SYSTEM__)
		case COSTUME_MOUNT:
			break;
#endif

#if defined(__ACCE_COSTUME_SYSTEM__)
		case COSTUME_ACCE:
			break;
#endif

#if defined(__WEAPON_COSTUME_SYSTEM__)
		case COSTUME_WEAPON:
			return ATTRIBUTE_SET_COSTUME_WEAPON;
#endif
		}
	}
#endif

	return -1;
}

bool CItem::HasAttr(BYTE bApply)
{
	for (int i = 0; i < ITEM_APPLY_MAX_NUM; ++i)
		if (m_pProto->aApplies[i].bType == bApply)
			return true;

	for (int i = 0; i < MAX_NORM_ATTR_NUM; ++i)
		if (GetAttributeType(i) == bApply)
			return true;

	return false;
}

bool CItem::HasRareAttr(BYTE bApply)
{
	for (int i = 0; i < MAX_RARE_ATTR_NUM; ++i)
		if (GetAttributeType(i + 5) == bApply)
			return true;

	return false;
}

void CItem::AddAttribute(BYTE bApply, short sValue)
{
	if (HasAttr(bApply))
		return;

	int i = GetAttributeCount();

	if (i >= MAX_NORM_ATTR_NUM)
		sys_err("item attribute overflow!");
	else
	{
		if (sValue)
			SetAttribute(i, bApply, sValue);
	}
}

void CItem::AddAttr(BYTE bApply, BYTE bLevel)
{
	if (HasAttr(bApply))
		return;

	if (bLevel <= 0)
		return;

	int i = GetAttributeCount();

	if (i == MAX_NORM_ATTR_NUM)
		sys_err("item attribute overflow!");
	else
	{
		const TItemAttrTable& r = g_map_itemAttr[bApply];
		long lVal = r.lValues[MIN(4, bLevel - 1)];

		if (lVal)
			SetAttribute(i, bApply, lVal);
	}
}

void CItem::PutAttributeWithLevel(BYTE bLevel)
{
	int iAttributeSet = GetAttributeSetIndex();

	if (iAttributeSet < 0)
		return;

	if (bLevel > ITEM_ATTRIBUTE_MAX_LEVEL)
		return;

	std::vector<int> avail;

	int total = 0;

	// 붙일 수 있는 속성 배열을 구축
	for (int i = 0; i < MAX_APPLY_NUM; ++i)
	{
		const TItemAttrTable& r = g_map_itemAttr[i];

		if (r.bMaxLevelBySet[iAttributeSet] && !HasAttr(i))
		{
			avail.push_back(i);
			total += r.dwProb;
		}
	}

	// 구축된 배열로 확률 계산을 통해 붙일 속성 선정
	unsigned int prob = number(1, total);
	int attr_idx = APPLY_NONE;

	for (DWORD i = 0; i < avail.size(); ++i)
	{
		const TItemAttrTable& r = g_map_itemAttr[avail[i]];

		if (prob <= r.dwProb)
		{
			attr_idx = avail[i];
			break;
		}

		prob -= r.dwProb;
	}

	if (!attr_idx)
	{
		sys_err("Cannot put item attribute %d %d", iAttributeSet, bLevel);
		return;
	}

	const TItemAttrTable& r = g_map_itemAttr[attr_idx];

	// 종류별 속성 레벨 최대값 제한
	if (bLevel > r.bMaxLevelBySet[iAttributeSet])
		bLevel = r.bMaxLevelBySet[iAttributeSet];

	AddAttr(attr_idx, bLevel);
}

void CItem::PutAttribute(const int* aiAttrPercentTable)
{
	int iAttrLevelPercent = number(1, 100);
	int i;

	for (i = 0; i < ITEM_ATTRIBUTE_MAX_LEVEL; ++i)
	{
		if (iAttrLevelPercent <= aiAttrPercentTable[i])
			break;

		iAttrLevelPercent -= aiAttrPercentTable[i];
	}

	PutAttributeWithLevel(i + 1);
}

void CItem::ChangeAttribute(const int* aiChangeProb)
{
	int iAttributeCount = GetAttributeCount();

	ClearAttribute();

	if (iAttributeCount == 0)
		return;

	TItemTable const* pProto = GetProto();

	if (pProto && pProto->sAddonType)
	{
		ApplyAddon(pProto->sAddonType);
	}

	static const int tmpChangeProb[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		0, 10, 40, 35, 15,
	};

	for (int i = GetAttributeCount(); i < iAttributeCount; ++i)
	{
		if (aiChangeProb == NULL)
		{
			PutAttribute(tmpChangeProb);
		}
		else
		{
			PutAttribute(aiChangeProb);
		}
	}
}

void CItem::AddAttribute()
{
	static const int aiItemAddAttributePercent[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		40, 50, 10, 0, 0
	};

	if (GetAttributeCount() < MAX_NORM_ATTR_NUM)
		PutAttribute(aiItemAddAttributePercent);
}

void CItem::ClearAttribute()
{
	for (int i = 0; i < MAX_NORM_ATTR_NUM; ++i)
	{
		m_aAttr[i].bType = 0;
		m_aAttr[i].sValue = 0;
	}
}

void CItem::ClearAllAttribute()
{
	for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
	{
		m_aAttr[i].bType = 0;
		m_aAttr[i].sValue = 0;
	}
}

int CItem::GetAttributeCount()
{
	int i;

	for (i = 0; i < MAX_NORM_ATTR_NUM; ++i)
	{
		if (GetAttributeType(i) == 0)
			break;
	}

	return i;
}

int CItem::FindAttribute(BYTE bType)
{
	for (int i = 0; i < MAX_NORM_ATTR_NUM; ++i)
	{
		if (GetAttributeType(i) == bType)
			return i;
	}

	return -1;
}

bool CItem::RemoveAttributeAt(int index)
{
	if (GetAttributeCount() <= index)
		return false;

	for (int i = index; i < MAX_NORM_ATTR_NUM - 1; ++i)
	{
		SetAttribute(i, GetAttributeType(i + 1), GetAttributeValue(i + 1));
	}

	SetAttribute(MAX_NORM_ATTR_NUM - 1, APPLY_NONE, 0);
	return true;
}

bool CItem::RemoveAttributeType(BYTE bType)
{
	int index = FindAttribute(bType);
	return index != -1 && RemoveAttributeType(index);
}

void CItem::SetAttributes(const TPlayerItemAttribute* c_pAttribute)
{
	thecore_memcpy(m_aAttr, c_pAttribute, sizeof(m_aAttr));
	Save();
}

void CItem::SetAttribute(int i, BYTE bType, short sValue)
{
	assert(i < MAX_NORM_ATTR_NUM);

	m_aAttr[i].bType = bType;
	m_aAttr[i].sValue = sValue;
	UpdatePacket();
	Save();
}

void CItem::SetForceAttribute(int i, BYTE bType, short sValue)
{
	assert(i < ITEM_ATTRIBUTE_MAX_NUM);

	m_aAttr[i].bType = bType;
	m_aAttr[i].sValue = sValue;
	UpdatePacket();
	Save();
}

void CItem::CopyAttributeTo(LPITEM pItem)
{
	pItem->SetAttributes(m_aAttr);
}

int CItem::GetRareAttrCount()
{
	int ret = 0;

	if (m_aAttr[5].bType != 0)
		ret++;

	if (m_aAttr[6].bType != 0)
		ret++;

	return ret;
}

bool CItem::ChangeRareAttribute()
{
	if (GetRareAttrCount() == 0)
		return false;

	int cnt = GetRareAttrCount();

	for (int i = 0; i < cnt; ++i)
	{
		m_aAttr[i + 5].bType = 0;
		m_aAttr[i + 5].sValue = 0;
	}

	if (g_bAttrLog)
	{
		if (GetOwner() && GetOwner()->GetDesc())
			LogManager::instance().ItemLog(GetOwner(), this, "SET_RARE_CHANGE", "");
		else
			LogManager::instance().ItemLog(0, 0, 0, GetID(), "SET_RARE_CHANGE", "", "", GetOriginalVnum());
	}

	for (int i = 0; i < cnt; ++i)
		AddRareAttribute();

	return true;
}

bool CItem::AddRareAttribute()
{
	int count = GetRareAttrCount();

	if (count >= 2)
		return false;

	int pos = count + 5;
	TPlayerItemAttribute& attr = m_aAttr[pos];

	int nAttrSet = GetAttributeSetIndex();
	std::vector<int> avail;

	for (int i = 0; i < MAX_APPLY_NUM; ++i)
	{
		const TItemAttrTable& r = g_map_itemRare[i];

		if (r.dwApplyIndex != 0 && r.bMaxLevelBySet[nAttrSet] > 0 && HasRareAttr(i) != true)
		{
			avail.push_back(i);
		}
	}

	const TItemAttrTable& r = g_map_itemRare[avail[number(0, avail.size() - 1)]];
	static const int aiItemAddRareAttributePercent[ITEM_ATTRIBUTE_MAX_LEVEL] =
	{
		//40, 30, 15, 10, 5, // Very Hard
		//30, 25, 20, 15, 10 // Hard
		25, 25, 20, 15, 15 // Average
		//0, 0, 0, 0, 100 // Always max level
	};

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(1, 100);
	int iAttrLevelPercent = dis(gen);

	int nAttrLevel;
	for (nAttrLevel = 1; nAttrLevel < ITEM_ATTRIBUTE_MAX_LEVEL; ++nAttrLevel)
	{
		if (iAttrLevelPercent <= aiItemAddRareAttributePercent[nAttrLevel - 1])
			break;

		iAttrLevelPercent -= aiItemAddRareAttributePercent[nAttrLevel - 1];
	}

	if (nAttrLevel > r.bMaxLevelBySet[nAttrSet])
		nAttrLevel = r.bMaxLevelBySet[nAttrSet];

	attr.bType = r.dwApplyIndex;
	attr.sValue = r.lValues[MIN(4, nAttrLevel - 1)];

	UpdatePacket();

	Save();

	const char* pszIP = NULL;

	if (GetOwner() && GetOwner()->GetDesc())
		pszIP = GetOwner()->GetDesc()->GetHostName();

	if (g_bAttrLog)
		LogManager::instance().ItemLog(pos, attr.bType, attr.sValue, GetID(), "SET_RARE", "", pszIP ? pszIP : "", GetOriginalVnum());

	return true;
}

#if defined(__ITEM_APPLY_RANDOM__)
void CItem::SetRandomApply(uint8_t slot, uint8_t type, int32_t value, uint8_t path)
{
	assert(slot < ITEM_APPLY_MAX_NUM);

	m_aApplyRandom[slot].bType = type;
	m_aApplyRandom[slot].sValue = value;
	m_aApplyRandom[slot].bPath = path;
	UpdatePacket();
	Save();
}

void CItem::SetForceRandomApply(uint8_t slot, uint8_t type, int32_t value, uint8_t path)
{
	assert(slot < ITEM_APPLY_MAX_NUM);

	m_aApplyRandom[slot].bType = type;
	m_aApplyRandom[slot].sValue = value;
	m_aApplyRandom[slot].bPath = path;
	UpdatePacket();
	Save();
}

void CItem::SetRandomApplies(const TPlayerItemAttribute* c_pApplyRandom)
{
	thecore_memcpy(m_aApplyRandom, c_pApplyRandom, sizeof(m_aApplyRandom));
	Save();
}

TPlayerItemAttribute* CItem::GetNextRandomApplies()
{
	int iRefineLevel = GetRefineLevel();
	const TItemTable* c_pItemProto = GetProto();
	if (c_pItemProto == nullptr)
		return nullptr;

	if (iRefineLevel > ITEM_REFINE_MAX_LEVEL)
		iRefineLevel = ITEM_REFINE_MAX_LEVEL;
	else
		++iRefineLevel;

	TPlayerItemAttribute* ApplyRandomTable = m_aApplyRandom;
	for (uint8_t uiApplySlot = 0; uiApplySlot < ITEM_APPLY_MAX_NUM; ++uiApplySlot)
	{
		auto ApplyRandomType = c_pItemProto->aApplies[uiApplySlot].bType;
		auto ApplyRandomValue = c_pItemProto->aApplies[uiApplySlot].lValue;
		if (ApplyRandomType == APPLY_RANDOM)
		{
			ApplyRandomTable[uiApplySlot].bType = m_aApplyRandom[uiApplySlot].bType;
			ApplyRandomTable[uiApplySlot].sValue = ITEM_MANAGER::instance().GetNextApplyRandomValue(ApplyRandomValue, iRefineLevel, m_aApplyRandom[uiApplySlot].bPath, m_aApplyRandom[uiApplySlot].bType);
			ApplyRandomTable[uiApplySlot].bPath = m_aApplyRandom[uiApplySlot].bPath;
		}
	}
	return ApplyRandomTable;
}
#endif
