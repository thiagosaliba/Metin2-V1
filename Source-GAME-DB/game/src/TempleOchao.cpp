#include "stdafx.h"
#include "TempleOchao.h"

#if defined(__TEMPLE_OCHAO__)
#include "utils.h"
#include "char.h"
#include "char_manager.h"
#include "sectree_manager.h"
#include "config.h"

namespace TempleOchao
{
	typedef struct SRooms
	{
		SRooms(int i2X = 0, int i2Y = 0, int i2Z = 0) : iX(i2X), iY(i2Y), iZ(i2Z) {}
		int iX, iY, iZ;
	};

	typedef std::map<BYTE, SRooms> RoomMapInfo;
	static RoomMapInfo m_rooms;

	void CMgr::Initialize()
	{
		iTempleRoom = 0;
		dwGuardianVID = 0;
		dwPortalVID = 0;
		s_pkGuardianKilledEvent = NULL;
		s_pkGuardianActivityEvent = NULL;
	}

	void CMgr::Destroy()
	{
		iTempleRoom = 0;
		dwGuardianVID = 0;
		dwPortalVID = 0;

		if (s_pkGuardianKilledEvent != NULL)
			event_cancel(&s_pkGuardianKilledEvent);

		if (s_pkGuardianActivityEvent != NULL)
			event_cancel(&s_pkGuardianActivityEvent);

		s_pkGuardianKilledEvent = NULL;
		s_pkGuardianActivityEvent = NULL;
	}

	void CMgr::Prepare()
	{
		if (std::distance(m_rooms.begin(), m_rooms.end()) != 0)
			return;

		LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(MAP_INDEX);
		if (!pMap)
			return;

		int pos[][2] = {
			{ pMap->m_setting.iBaseX + 193 * 100, pMap->m_setting.iBaseY + 145 * 100 }, // 1
			{ pMap->m_setting.iBaseX + 123 * 100, pMap->m_setting.iBaseY + 216 * 100 }, // 2
			{ pMap->m_setting.iBaseX + 224 * 100, pMap->m_setting.iBaseY + 383 * 100 }, // 3
			{ pMap->m_setting.iBaseX + 348 * 100, pMap->m_setting.iBaseY + 708 * 100 }, // 4
			{ pMap->m_setting.iBaseX + 375 * 100, pMap->m_setting.iBaseY + 608 * 100 }, // 5
			{ pMap->m_setting.iBaseX + 430 * 100, pMap->m_setting.iBaseY + 516 * 100 }, // 6
			{ pMap->m_setting.iBaseX + 444 * 100, pMap->m_setting.iBaseY + 382 * 100 }, // 7
			{ pMap->m_setting.iBaseX + 388 * 100, pMap->m_setting.iBaseY + 195 * 100 }, // 8
			{ pMap->m_setting.iBaseX + 446 * 100, pMap->m_setting.iBaseY + 247 * 100 }, // 9
			{ pMap->m_setting.iBaseX + 592 * 100, pMap->m_setting.iBaseY + 139 * 100 }, // 10
			{ pMap->m_setting.iBaseX + 646 * 100, pMap->m_setting.iBaseY + 152 * 100 } // 11
		};

		for (BYTE i = 0; i < ROOMS; i++)
		{
			int iX = pos[i][0], iY = pos[i][1];
			m_rooms.insert(std::make_pair(i + 1, SRooms(iX, iY)));
			if (test_server)
				sys_log(0, "<Temple Ochao> Initialize: room [id: %d, x: %d, y: %d].", i + 1, iX, iY);
		}

		SetRoom(number(1, ROOMS));
		SetGuardianVID(0);
		SetPortalVID(0);
		ClearPrepare();
	}

	EVENTINFO(guardian_event_info)
	{
		DynamicCharacterPtr ch;
		DWORD next_change_time;
		bool bAttacked;
	};

	EVENTFUNC(guardian_activity_event)
	{
		guardian_event_info* pInfo = dynamic_cast<guardian_event_info*>(event->info);

		if (pInfo == NULL)
			return 0;

		DWORD dwTime = get_global_time();

		if (pInfo->next_change_time <= dwTime)
		{
			LPCHARACTER ch = pInfo->ch;
			if (ch && ch->GetMapIndex() == MAP_INDEX)
				M2_DESTROY_CHARACTER(ch);

			pInfo->next_change_time = dwTime + NO_ACTIVITY;
			CMgr::instance().ChangeRoom();
		}

		return PASSES_PER_SEC(CHECK_ACTIVITY);
	}

	void CMgr::Spawn()
	{
		int iRoom = 0, iX = 0, iY = 0, iZ = 0;
		RandomRoom(iRoom, iX, iY, iZ);

		LPCHARACTER newGuardian = CHARACTER_MANAGER::instance().SpawnMob(GUARDIAN, MAP_INDEX, iX, iY, iZ);
		LPCHARACTER oldGuardian = NULL;

		if (newGuardian)
		{
			SetGuardianVID(newGuardian->GetVID());

			oldGuardian = CHARACTER_MANAGER::instance().Find(GetGuardianVID());
			if ((oldGuardian != NULL) && (oldGuardian->GetMapIndex() == MAP_INDEX))
				M2_DESTROY_CHARACTER(oldGuardian);

			if (s_pkGuardianActivityEvent != NULL)
				event_cancel(&s_pkGuardianActivityEvent);

			guardian_event_info* pEventInfo = AllocEventInfo<guardian_event_info>();
			pEventInfo->ch = newGuardian;
			pEventInfo->next_change_time = get_global_time() + NO_ACTIVITY;
			pEventInfo->bAttacked = false;
			s_pkGuardianActivityEvent = event_create(guardian_activity_event, pEventInfo, PASSES_PER_SEC(CHECK_ACTIVITY));
		}
		else
		{
			sys_err("<Temple Ochao> Cannot spawn %d at X [%d], Y [%d], Z [%d].", GUARDIAN, iX, iY, iZ);
			return;
		}
	}

	void CMgr::RandomRoom(int& iRoom, int& iX, int& iY, int& iZ)
	{
		int iGenerated;
		while ((iGenerated = number(1, ROOMS)) != GetRoom())
		{
			SetRoom(iGenerated);
			if (test_server)
				sys_log(0, "<Temple Ochao> %d spawned at room %d.", GUARDIAN, iGenerated);

			break;
		}

		for (RoomMapInfo::const_iterator iter = m_rooms.begin(); iter != m_rooms.end(); ++iter)
		{
			if (iter->first == GetRoom())
			{
				iRoom = iter->first;
				iX = iter->second.iX;
				iY = iter->second.iY;
				iZ = iter->second.iZ;
				break;
			}
		}
	}

	EVENTINFO(ochao_event_info)
	{
		DynamicCharacterPtr ch;
		BYTE bStep;
	};

	EVENTFUNC(guardian_event)
	{
		ochao_event_info* pInfo = dynamic_cast<ochao_event_info*>(event->info);

		if (!pInfo)
			return 0;

		if (pInfo->bStep == 1)
		{
			LPCHARACTER ch = pInfo->ch;
			if (ch && ch->GetMapIndex() == MAP_INDEX)
				M2_DESTROY_CHARACTER(ch);

			CMgr::instance().ClearPrepare();
			pInfo->ch = NULL;
			pInfo->bStep = 0;
		}

		return 0;
	}

	void CMgr::OnGuardianKilled(int iX, int iY, int iZ)
	{
		LPCHARACTER newPortal = CHARACTER_MANAGER::instance().SpawnMob(PORTAL, MAP_INDEX, iX, iY, iZ);
		if (newPortal)
		{
			SetPortalVID(newPortal->GetVID());

			if (s_pkGuardianKilledEvent != NULL)
				event_cancel(&s_pkGuardianKilledEvent);

			ochao_event_info* pEventInfo = AllocEventInfo<ochao_event_info>();
			pEventInfo->ch = newPortal;
			pEventInfo->bStep = 1;
			s_pkGuardianKilledEvent = event_create(guardian_event, pEventInfo, PASSES_PER_SEC(PORTAL_VANISH_TIME));
		}
		else
		{
			sys_err("<Temple Ochao> Cannot spawn %d at X [%d], Y [%d], Z [%d].", PORTAL, iX, iY, iZ);
			return;
		}
	}

	void CMgr::ClearPrepare()
	{
		LPCHARACTER oldPortal = CHARACTER_MANAGER::instance().Find(GetPortalVID());
		if (oldPortal)
		{
			//SetPortalVID(0);
			if (oldPortal->GetMapIndex() == MAP_INDEX)
				M2_DESTROY_CHARACTER(oldPortal);
		}

		SetPortalVID(0);

		if (s_pkGuardianKilledEvent != NULL)
		{
			event_cancel(&s_pkGuardianKilledEvent);
			s_pkGuardianKilledEvent = NULL;
		}

		Spawn();
	}

	void CMgr::ChangeRoom()
	{
		LPCHARACTER oldGuardian = CHARACTER_MANAGER::instance().Find(GetGuardianVID());
		if ((oldGuardian != NULL) && (oldGuardian->GetVictim()) && (oldGuardian->GetMapIndex() == MAP_INDEX))
			return;

		if (s_pkGuardianActivityEvent != NULL)
		{
			event_cancel(&s_pkGuardianActivityEvent);
			s_pkGuardianActivityEvent = NULL;
		}

		Spawn();
	}

	void CMgr::AttackedGuardian(LPCHARACTER pkVictim)
	{
		if (s_pkGuardianActivityEvent == NULL)
			return;

		guardian_event_info* pInfo = dynamic_cast<guardian_event_info*>(s_pkGuardianActivityEvent->info);
		if (pInfo == NULL)
			return;

		if (pInfo->bAttacked != true)
		{
			DWORD time = get_global_time();
			pInfo->ch = pkVictim ? pkVictim : NULL;
			pInfo->next_change_time = time + ATTACKED;
			pInfo->bAttacked = true;
		}
	}
}
#endif
