#include "stdafx.h"
#include "../EterPack/EterPackManager.h"
#include "PythoNnonPlayer.h"
#include "InstanceBase.h"
#include "PythonCharacterManager.h"

bool CPythonNonPlayer::LoadNonPlayerData(const char* c_szFileName)
{
	CMappedFile file;
	LPCVOID pvData;

	Tracef("CPythonNonPlayer::LoadNonPlayerData: %s, sizeof(TMobTable)=%u\n", c_szFileName, sizeof(TMobTable));

	if (!CEterPackManager::Instance().Get(file, c_szFileName, &pvData))
		return false;

	DWORD dwFourCC, dwElements, dwDataSize;

	file.Read(&dwFourCC, sizeof(DWORD));

	if (dwFourCC != MAKEFOURCC('M', 'M', 'P', 'T'))
	{
		TraceError("CPythonNonPlayer::LoadNonPlayerData: invalid Mob proto type %s", c_szFileName);
		return false;
	}

	file.Read(&dwElements, sizeof(DWORD));
	file.Read(&dwDataSize, sizeof(DWORD));

	BYTE* pbData = new BYTE[dwDataSize];
	file.Read(pbData, dwDataSize);

	CLZObject zObj;

	if (!CLZO::Instance().Decompress(zObj, pbData, COMPRESS_MOB))
	{
		delete[] pbData;
		return false;
	}

	if ((zObj.GetSize() % sizeof(TMobTable)) != 0)
	{
		TraceError("CPythonNonPlayer::LoadNonPlayerData: invalid size %u check data format.", zObj.GetSize());
		return false;
	}

	TMobTable* pTable = (TMobTable*)zObj.GetBuffer();
	for (DWORD i = 0; i < dwElements; ++i, ++pTable)
	{
		TMobTable* pNonPlayerData = new TMobTable;

		memcpy(pNonPlayerData, pTable, sizeof(TMobTable));

		//TraceError("%d : %s type[%d] color[%d]", pNonPlayerData->dwVnum, pNonPlayerData->szLocaleName, pNonPlayerData->bType, pNonPlayerData->dwMonsterColor);
		m_NonPlayerDataMap.insert(TNonPlayerDataMap::value_type(pNonPlayerData->dwVnum, pNonPlayerData));
	}

	delete[] pbData;
	return true;
}

bool CPythonNonPlayer::GetName(DWORD dwVnum, const char** c_pszName)
{
	const TMobTable* p = GetTable(dwVnum);

	if (!p)
		return false;

	*c_pszName = p->szLocaleName;

	return true;
}

bool CPythonNonPlayer::GetInstanceType(DWORD dwVnum, BYTE* pbType)
{
	const TMobTable* p = GetTable(dwVnum);

	// dwVnum를 찾을 수 없으면 플레이어 캐릭터로 간주 한다. 문제성 코드 -_- [cronan]
	if (!p)
		return false;

	*pbType = p->bType;

	return true;
}

const CPythonNonPlayer::TMobTable* CPythonNonPlayer::GetTable(DWORD dwVnum)
{
	TNonPlayerDataMap::iterator itor = m_NonPlayerDataMap.find(dwVnum);

	if (itor == m_NonPlayerDataMap.end())
		return NULL;

	return itor->second;
}

BYTE CPythonNonPlayer::GetEventType(DWORD dwVnum)
{
	const TMobTable* p = GetTable(dwVnum);

	if (!p)
	{
		//Tracef("CPythonNonPlayer::GetEventType - Failed to find virtual number\n");
		return ON_CLICK_EVENT_NONE;
	}

	return p->bOnClickType;
}

BYTE CPythonNonPlayer::GetEventTypeByVID(DWORD dwVID)
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(dwVID);

	if (NULL == pInstance)
	{
		//Tracef("CPythonNonPlayer::GetEventTypeByVID - There is no Virtual Number\n");
		return ON_CLICK_EVENT_NONE;
	}

	WORD dwVnum = pInstance->GetVirtualNumber();
	return GetEventType(dwVnum);
}

const char* CPythonNonPlayer::GetMonsterName(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
	{
		static const char* sc_szEmpty = "";
		return sc_szEmpty;
	}

	return c_pTable->szLocaleName;
}

DWORD CPythonNonPlayer::GetMonsterColor(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;

	return c_pTable->dwMonsterColor;
}

void CPythonNonPlayer::GetMatchableMobList(int iLevel, int iInterval, TMobTableList* pMobTableList)
{
	/*
	pMobTableList->clear();

	TNonPlayerDataMap::iterator itor = m_NonPlayerDataMap.begin();
	for (; itor != m_NonPlayerDataMap.end(); ++itor)
	{
		TMobTable* pMobTable = itor->second;

		int iLowerLevelLimit = iLevel - iInterval;
		int iUpperLevelLimit = iLevel + iInterval;

		if ((pMobTable->abLevelRange[0] >= iLowerLevelLimit && pMobTable->abLevelRange[0] <= iUpperLevelLimit) ||
			(pMobTable->abLevelRange[1] >= iLowerLevelLimit && pMobTable->abLevelRange[1] <= iUpperLevelLimit))
		{
			pMobTableList->push_back(pMobTable);
		}
	}
	*/
}

#ifdef ENABLE_SEND_TARGET_INFO
uint64_t CPythonNonPlayer::GetMonsterMaxHP(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->dwMaxHP;
}

DWORD CPythonNonPlayer::GetMonsterRaceFlag(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->dwRaceFlag;
}

bool CPythonNonPlayer::IsMonsterRaceFlag(DWORD dwVnum, DWORD dwFlag)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return false;

	if (c_pTable->dwVnum == dwVnum && IS_SET(c_pTable->dwRaceFlag, dwFlag))
		return true;

	return false;
}

DWORD CPythonNonPlayer::GetMonsterLevel(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->bLevel;
}

DWORD CPythonNonPlayer::GetMonsterDamage1(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->dwDamageRange[0];
}

DWORD CPythonNonPlayer::GetMonsterDamage2(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->dwDamageRange[1];
}

DWORD CPythonNonPlayer::GetMonsterExp(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->dwExp;
}

float CPythonNonPlayer::GetMonsterDamageMultiply(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0.0f;
	return c_pTable->fDamMultiply;
}

DWORD CPythonNonPlayer::GetMonsterST(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->bStr;
}

DWORD CPythonNonPlayer::GetMonsterDX(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->bDex;
}

bool CPythonNonPlayer::IsMonsterStone(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (c_pTable && c_pTable->bType == 2)
		return true;

	return false;
}

BYTE CPythonNonPlayer::GetMonsterRegenCycle(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->bRegenCycle;
}

BYTE CPythonNonPlayer::GetMonsterRegenPercent(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->bRegenPercent;
}

DWORD CPythonNonPlayer::GetMonsterGoldMin(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->dwGoldMin;
}

DWORD CPythonNonPlayer::GetMonsterGoldMax(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;
	return c_pTable->dwGoldMax;
}

DWORD CPythonNonPlayer::GetMonsterResist(DWORD dwVnum, BYTE bResistNum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 0;

	if (bResistNum >= MOB_RESISTS_MAX_NUM)
		return 0;

	return c_pTable->cResists[bResistNum];
}

void CPythonNonPlayer::AddItemDropInfo(DWORD dwRaceVnum, BYTE bIndex, DWORD dwVnum, WORD wCount)
{
	TMobItemDrop::iterator it = m_MobItemDropMap[dwRaceVnum].find(bIndex);
	if (it != m_MobItemDropMap[dwRaceVnum].end())
		return;

	SMobItemDrop MobItemDrop;
	MobItemDrop.dwVnum = dwVnum;
	MobItemDrop.wCount = wCount;
	m_MobItemDropMap[dwRaceVnum].insert(TMobItemDrop::value_type(bIndex, MobItemDrop));
}

void CPythonNonPlayer::ClearItemDropInfo()
{
	if (!m_MobItemDropMap.empty())
		m_MobItemDropMap.clear();
}

size_t CPythonNonPlayer::GetMonsterDropCount(DWORD dwRaceVnum)
{
	TMobItemDropMap::const_iterator it = m_MobItemDropMap.find(dwRaceVnum);
	if (it != m_MobItemDropMap.end())
		return it->second.size();
	return 0;
}

DWORD CPythonNonPlayer::GetMonsterDropItemVnum(DWORD dwRaceVnum, BYTE bIndex)
{
	TMobItemDropMap::const_iterator it = m_MobItemDropMap.find(dwRaceVnum);
	if (it != m_MobItemDropMap.end())
	{
		TMobItemDrop::iterator it = m_MobItemDropMap[dwRaceVnum].find(bIndex);
		if (it != m_MobItemDropMap[dwRaceVnum].end())
			return it->second.dwVnum;
	}
	return 0;
}

WORD CPythonNonPlayer::GetMonsterDropItemCount(DWORD dwRaceVnum, BYTE bIndex)
{
	TMobItemDropMap::const_iterator it = m_MobItemDropMap.find(dwRaceVnum);
	if (it != m_MobItemDropMap.end())
	{
		TMobItemDrop::iterator it = m_MobItemDropMap[dwRaceVnum].find(bIndex);
		if (it != m_MobItemDropMap[dwRaceVnum].end())
			return it->second.wCount;
	}
	return 0;
}
#endif

BYTE CPythonNonPlayer::GetMonsterScalePercent(DWORD dwVnum)
{
	const CPythonNonPlayer::TMobTable* c_pTable = GetTable(dwVnum);
	if (!c_pTable)
		return 100;

	return MINMAX(50, c_pTable->bScale, 200);
}

void CPythonNonPlayer::Clear()
{
}

void CPythonNonPlayer::Destroy()
{
	stl_wipe_second(m_NonPlayerDataMap);
	m_NonPlayerDataMap.clear();
#if defined(ENABLE_SEND_TARGET_INFO)
	m_MobItemDropMap.clear();
#endif
}

CPythonNonPlayer::CPythonNonPlayer()
{
	Clear();
}

CPythonNonPlayer::~CPythonNonPlayer(void)
{
	Destroy();
}
