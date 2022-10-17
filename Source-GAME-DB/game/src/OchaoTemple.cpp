#include "stdafx.h"
#include "utils.h"
#include "char.h"
#include "char_manager.h"
#include "sectree_manager.h"
#include "config.h"
#include "TempleOchao.h"

namespace TempleOchao
{
	typedef struct SRooms
	{
		SRooms(int i2X = 0, int i2Y = 0, int i2Z = 0) : iX(i2X), iY(i2Y), iZ(i2Z) {}
		int iX, iY, iZ;
	};

	static std::map<BYTE, SRooms> m_rooms;

	void CMgr::Initialize()
	{
		iTempleRoom = 0;
		dwGuardianVID = 0;
		dwPortalVID = 0;
		bOnGuardianKilled = false;
		s_pkGuardianKilledEvent = NULL;
		s_pkGuardianActivityEvent = NULL;
	}

	void CMgr::Destroy()
	{
		iTempleRoom = 0;
		dwGuardianVID = 0;
		dwPortalVID = 0;
		bOnGuardianKilled = false;

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

		LPSECTREE_MAP pMap = SECTREE_MANAGER::instance().GetMap(TEMPLE_OCHAO_MAP_INDEX);
		if (!pMap)
			return;

		int pos[][2] = {
			{ pMap->m_setting.iBaseX + 193 * 100, pMap->m_setting.iBaseY + 145 * 100 },
			{ pMap->m_setting.iBaseX + 123 * 100, pMap->m_setting.iBaseY + 216 * 100 },
			{ pMap->m_setting.iBaseX + 224 * 100, pMap->m_setting.iBaseY + 383 * 100 },
			{ pMap->m_setting.iBaseX + 348 * 100, pMap->m_setting.iBaseY + 708 * 100 },
			{ pMap->m_setting.iBaseX + 375 * 100, pMap->m_setting.iBaseY + 608 * 100 },
			{ pMap->m_setting.iBaseX + 430 * 100, pMap->m_setting.iBaseY + 516 * 100 },
			{ pMap->m_setting.iBaseX + 444 * 100, pMap->m_setting.iBaseY + 382 * 100 },
			{ pMap->m_setting.iBaseX + 388 * 100, pMap->m_setting.iBaseY + 195 * 100 },
			{ pMap->m_setting.iBaseX + 446 * 100, pMap->m_setting.iBaseY + 247 * 100 },
			{ pMap->m_setting.iBaseX + 592 * 100, pMap->m_setting.iBaseY + 139 * 100 },
			{ pMap->m_setting.iBaseX + 646 * 100, pMap->m_setting.iBaseY + 152 * 100 }
		};

		for (BYTE i = 0; i < TEMPLE_OCHAO_ROOMS; i++)
		{
			int iX = pos[i][0], iY = pos[i][1];
			m_rooms.insert(std::make_pair(i + 1, SRooms(iX, iY)));
			sys_log(0, "<Temple Ochao> Initialize: room [id: %d, x: %d, y: %d].", i + 1, iX, iY);
		}

		SetRoom(number(1, TEMPLE_OCHAO_ROOMS));
		SetGuardianVID(0);
		SetPortalVID(0);
		ClearPrepare();
	}

	EVENTINFO(guardian_event_info)
	{
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
			pInfo->next_change_time = dwTime + TEMPLE_OCHAO_NO_ACTIVITY;
			CMgr::instance().ChangeRoom();
		}

		return PASSES_PER_SEC(TEMPLE_OCHAO_CHECK_ACTIVITY);
	}

	void CMgr::Spawn()
	{
		int iRoom = 0, iX = 0, iY = 0, iZ = 0;
		RandomRoom(iRoom, iX, iY, iZ);

		LPCHARACTER newGuardian = CHARACTER_MANAGER::instance().SpawnMob(TEMPLE_OCHAO_GUARDIAN, TEMPLE_OCHAO_MAP_INDEX, iX, iY, iZ, false, -1, true);
		if (!newGuardian)
		{
			sys_err("<Temple Ochao> Spawn: cannot spawn %d at X [%d], Y [%d], Z [%d].", TEMPLE_OCHAO_GUARDIAN, iX, iY, iZ);
			return;
		}

		LPCHARACTER oldGuardian = CHARACTER_MANAGER::instance().Find(GetGuardianVID());
		if ((oldGuardian != NULL) && (oldGuardian->GetMapIndex() == TEMPLE_OCHAO_MAP_INDEX))
			M2_DESTROY_CHARACTER(oldGuardian);

		SetGuardianVID(newGuardian->GetVID());

		if (s_pkGuardianActivityEvent != NULL)
			event_cancel(&s_pkGuardianActivityEvent);

		guardian_event_info* pEventInfo = AllocEventInfo<guardian_event_info>();
		s_pkGuardianActivityEvent = event_create(guardian_activity_event, pEventInfo, PASSES_PER_SEC(TEMPLE_OCHAO_CHECK_ACTIVITY));
		pEventInfo->next_change_time = get_global_time() + TEMPLE_OCHAO_NO_ACTIVITY;
		pEventInfo->bAttacked = false;
	}

	void CMgr::RandomRoom(int& iRoom, int& iX, int& iY, int& iZ)
	{
		int iGenerated;
		while ((iGenerated = number(1, TEMPLE_OCHAO_ROOMS)) != GetRoom())
		{
			SetRoom(iGenerated);
			sys_log(0, "<Temple Ochao> RandomRoom: the new room for En-Tai Guardian is %d.", iGenerated);
			break;
		}

		itertype(m_rooms) iter = m_rooms.begin();
		for (; iter != m_rooms.end(); ++iter)
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
		BYTE bStep;
	};

	EVENTFUNC(guardian_event)
	{
		ochao_event_info* pInfo = dynamic_cast<ochao_event_info*>(event->info);
		if (pInfo == NULL)
			return 0;

		if (pInfo->bStep == 1)
		{
			CMgr::instance().ClearPrepare();
			pInfo->bStep = 0;
		}

		return 0;
	}

	void CMgr::OnGuardianKilled(int iX, int iY, int iZ)
	{
		LPCHARACTER newPortal = CHARACTER_MANAGER::instance().SpawnMob(TempleOchao::PORTAL, TEMPLE_OCHAO_MAP_INDEX, iX, iY, iZ, false, -1, true);
		if (!newPortal)
		{
			sys_err("<Temple Ochao> Spawn: can't spawn %d at X [%d], Y [%d], Z [%d].", TempleOchao::PORTAL, iX, iY, iZ);
			return;
		}

		SetPortalVID(newPortal->GetVID());
		if (s_pkGuardianKilledEvent != NULL)
			event_cancel(&s_pkGuardianKilledEvent);

		ochao_event_info* pEventInfo = AllocEventInfo<ochao_event_info>();
		s_pkGuardianKilledEvent = event_create(guardian_event, pEventInfo, PASSES_PER_SEC(TempleOchao::PORTAL_VANISH_TIME));
		pEventInfo->bStep = 1;

		bOnGuardianKilled = true;
	}

	void CMgr::ClearPrepare()
	{
		if (s_pkGuardianKilledEvent != NULL)
		{
			event_cancel(&s_pkGuardianKilledEvent);
			s_pkGuardianKilledEvent = NULL;
		}

		Spawn();
		LPCHARACTER oldPortal = CHARACTER_MANAGER::instance().Find(GetPortalVID());
		if ((oldPortal != NULL) && (oldPortal->GetMapIndex() == TEMPLE_OCHAO_MAP_INDEX))
			M2_DESTROY_CHARACTER(oldPortal);

		SetPortalVID(0);
	}

	void CMgr::ChangeRoom()
	{
		LPCHARACTER oldGuardian = CHARACTER_MANAGER::instance().Find(GetGuardianVID());
		if ((oldGuardian != NULL) && (oldGuardian->GetVictim()) && (oldGuardian->GetMapIndex() == TEMPLE_OCHAO_MAP_INDEX))
			return;

		if (s_pkGuardianActivityEvent != NULL)
		{
			event_cancel(&s_pkGuardianActivityEvent);
			s_pkGuardianActivityEvent = NULL;
		}

		Spawn();
	}

	void CMgr::AttackedGuardian()
	{
		if (s_pkGuardianActivityEvent == NULL)
			return;

		guardian_event_info* pInfo = dynamic_cast<guardian_event_info*>(s_pkGuardianActivityEvent->info);
		if (pInfo == NULL)
			return;

		if (pInfo->bAttacked != true)
		{
			DWORD time = get_global_time();
			pInfo->next_change_time = time + TEMPLE_OCHAO_ATTACKED;
			pInfo->bAttacked = true;
		}
	}
}
