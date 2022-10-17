#include "stdafx.h"
#include "config.h"

#include "minigame.h"

#include "../../common/length.h"
#include "../../common/tables.h"
#include "p2p.h"
#include "locale_service.h"
#include "char.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "questmanager.h"
#include "questlua.h"
#include "start_position.h"
#include "char_manager.h"
#include "item_manager.h"
#include "sectree_manager.h"
#include "regen.h"
#include "log.h"
#include "db.h"
#include "target.h"
#include "party.h"

CMiniGameManager::CMiniGameManager()
{
#if defined(__MINI_GAME_CATCH_KING__)
	iCatchKingEndTime = 0;
#endif
#if defined(__EVENT_BANNER_FLAG__)
	m_bIsLoadedBanners = false;
#endif
	//Initialize();
}

CMiniGameManager::~CMiniGameManager()
{
	//Destroy();
}

bool CMiniGameManager::Initialize()
{
	return true;
}

void CMiniGameManager::Destroy()
{
	return;
}

void CMiniGameManager::SpawnEventNPC(DWORD dwVnum)
{
	CharacterVectorInteractor i;
	bool isNpcA1 = false;
	bool isNpcB1 = false;
	bool isNpcC1 = false;
	BYTE bVnumC = 0;

	// { red_x, red_y, yellow_x, yellow_y, blue_x, blue_y }
	DWORD spawnPos[][6] = {
		{ 59000, 47100, 51700, 74200, 30600, 82800 }, // 30129
		{ 60800, 61700, 59600, 61000, 35700, 74300 }, // 30131
#if defined(__MINI_GAME_CATCH_KING__)
		{ 60800, 62300, 59600, 61400, 35000, 73800 }, // 20506
#endif
	};

	switch (dwVnum)
	{
	case 30129:
		bVnumC = 0;
		break;

	case 30131:
		bVnumC = 1;
		break;

#if defined(__MINI_GAME_CATCH_KING__)
	case 20506:
		bVnumC = 2;
		break;
#endif
	}

	if (CHARACTER_MANAGER::instance().GetCharactersByRaceNum(dwVnum, i))
	{
		CharacterVectorInteractor::iterator it = i.begin();

		while (it != i.end())
		{
			LPCHARACTER tch = *(it++);

			if (tch->GetMapIndex() == EMPIRE_A)
			{
				isNpcA1 = true;
			}
			else if (tch->GetMapIndex() == EMPIRE_B)
			{
				isNpcB1 = true;
			}
			else if (tch->GetMapIndex() == EMPIRE_C)
			{
				isNpcC1 = true;
			}
		}
	}

	if (map_allow_find(EMPIRE_A) && !isNpcA1)
	{
		if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(EMPIRE_A))
		{
			CHARACTER_MANAGER::instance().SpawnMob(dwVnum, EMPIRE_A, pkMap->m_setting.iBaseX + spawnPos[bVnumC][0], pkMap->m_setting.iBaseY + spawnPos[bVnumC][1], 0, false, 90, true);
		}
	}

	if (map_allow_find(EMPIRE_B) && !isNpcB1)
	{
		if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(EMPIRE_B))
		{
			CHARACTER_MANAGER::instance().SpawnMob(dwVnum, EMPIRE_B, pkMap->m_setting.iBaseX + spawnPos[bVnumC][2], pkMap->m_setting.iBaseY + spawnPos[bVnumC][3], 0, false, 110, true);
		}
	}

	if (map_allow_find(EMPIRE_C) && !isNpcC1)
	{
		if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(EMPIRE_C))
		{
			CHARACTER_MANAGER::instance().SpawnMob(dwVnum, EMPIRE_C, pkMap->m_setting.iBaseX + spawnPos[bVnumC][4], pkMap->m_setting.iBaseY + spawnPos[bVnumC][5], 0, false, 140, true);
		}
	}
}

#if defined(__EVENT_BANNER_FLAG__)
#include "CsvReader.h"
bool CMiniGameManager::InitializeBanners()
{
	if (m_bIsLoadedBanners)
		return false;

	const char* c_szFileName = "data/banner/list.txt";

	cCsvTable nameData;
	if (!nameData.Load(c_szFileName, '\t'))
	{
		sys_log(0, "%s couldn't be loaded or its format is incorrect.", c_szFileName);
		return false;
	}
	else
	{
		nameData.Next();
		while (nameData.Next())
		{
			if (nameData.ColCount() < 2)
				continue;

			BannerMap.insert(std::make_pair(atoi(nameData.AsStringByIndex(0)), nameData.AsStringByIndex(1)));
		}
	}
	nameData.Destroy();

	m_bIsLoadedBanners = true;

	DWORD dwFlagVnum = quest::CQuestManager::instance().GetEventFlag("banner");
	if (dwFlagVnum > 0)
		SpawnBanners(dwFlagVnum);

	return true;
}

bool CMiniGameManager::SpawnBanners(int iEnable, const char* c_szBannerName)
{
	if (!m_bIsLoadedBanners)
		InitializeBanners();

	bool bDestroy = true;
	bool bSpawn = false;

	DWORD dwBannerVnum = 0;
	std::string strBannerName;

	if (!c_szBannerName)
	{
		BannerMapType::const_iterator it = BannerMap.find(iEnable);
		if (it == BannerMap.end())
			return false;

		dwBannerVnum = it->first;
		strBannerName = it->second;
	}
	else
	{
		for (BannerMapType::const_iterator it = BannerMap.begin(); BannerMap.end() != it; ++it)
		{
			if (!strcmp(it->second.c_str(), c_szBannerName))
			{
				dwBannerVnum = it->first;
				strBannerName = it->second;
				break;
			}
		}
	}

	if (dwBannerVnum == 0 || strBannerName.empty())
		return false;

	if (iEnable > 0)
		bSpawn = true;

	if (bDestroy)
	{
		quest::CQuestManager::instance().RequestSetEventFlag("banner", 0);

		CharacterVectorInteractor i;
		CHARACTER_MANAGER::instance().GetCharactersByRaceNum(dwBannerVnum, i);

		for (CharacterVectorInteractor::iterator it = i.begin(); it != i.end(); it++)
		{
			M2_DESTROY_CHARACTER(*it);
		}
	}

	if (bSpawn)
	{
		//if (quest::CQuestManager::instance().GetEventFlag("banner") > 0)
		//	return false;

		quest::CQuestManager::instance().RequestSetEventFlag("banner", dwBannerVnum);

		if (map_allow_find(EMapIndex::EMPIRE_A))
		{
			std::string strBannerFile = "data/banner/a/" + strBannerName + ".txt";

			if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(EMapIndex::EMPIRE_A))
			{
				regen_do(strBannerFile.c_str(), EMapIndex::EMPIRE_A, pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY, NULL, false);
			}
		}

		if (map_allow_find(EMapIndex::EMPIRE_B))
		{
			std::string strBannerFile = "data/banner/b/" + strBannerName + ".txt";

			if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(EMapIndex::EMPIRE_B))
			{
				regen_do(strBannerFile.c_str(), EMapIndex::EMPIRE_B, pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY, NULL, false);
			}
		}

		if (map_allow_find(EMapIndex::EMPIRE_C))
		{
			std::string strBannerFile = "data/banner/c/" + strBannerName + ".txt";

			if (LPSECTREE_MAP pkMap = SECTREE_MANAGER::instance().GetMap(EMapIndex::EMPIRE_C))
			{
				regen_do(strBannerFile.c_str(), EMapIndex::EMPIRE_C, pkMap->m_setting.iBaseX, pkMap->m_setting.iBaseY, NULL, false);
			}
		}
	}

	return true;
}
#endif
