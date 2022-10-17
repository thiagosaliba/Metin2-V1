#include "stdafx.h"
#include "constants.h"
#include "pvp.h"
#include "crc32.h"
#include "packet.h"
#include "desc.h"
#include "desc_manager.h"
#include "char.h"
#include "char_manager.h"
#include "config.h"
#include "sectree_manager.h"
#include "buffer_manager.h"
#include "locale_service.h"

using namespace std;

#if defined(__PVP_COUNTDOWN__)
EVENTINFO(pvp_duel_countdown_info)
{
	DynamicCharacterPtr chA;
	DynamicCharacterPtr chB;
	CPVP* pvp;
	BYTE timer;

	pvp_duel_countdown_info() : chA(), chB(), pvp(0), timer(0) {}
};

EVENTFUNC(pvp_duel_countdown)
{
	pvp_duel_countdown_info* info = dynamic_cast<pvp_duel_countdown_info*>(event->info);

	if (info == NULL)
	{
		sys_err("pvp_duel_countdown> <Factor> Null pointer");
		return 0;
	}

	LPCHARACTER chA = info->chA;
	LPCHARACTER chB = info->chB;

	if (chA == NULL || chB == NULL) // <Factor>
		return 0;

	switch (info->timer)
	{
	case 0:
	{
		info->timer++;
		chA->SpecificEffectPacket("D:/ymir work/ui/game/pvp/3.mse");
		chB->SpecificEffectPacket("D:/ymir work/ui/game/pvp/3.mse");
		return PASSES_PER_SEC(1); break;
	}
	case 1:
	{
		info->timer++;
		chA->SpecificEffectPacket("D:/ymir work/ui/game/pvp/2.mse");
		chB->SpecificEffectPacket("D:/ymir work/ui/game/pvp/2.mse");
		return PASSES_PER_SEC(1); break;
	}
	case 2:
	{
		info->timer++;
		chA->SpecificEffectPacket("D:/ymir work/ui/game/pvp/1.mse");
		chB->SpecificEffectPacket("D:/ymir work/ui/game/pvp/1.mse");
		return PASSES_PER_SEC(1); break;
	}
	case 3:
	{
		info->timer++;
		chA->SpecificEffectPacket("D:/ymir work/ui/game/pvp/go.mse");
		chB->SpecificEffectPacket("D:/ymir work/ui/game/pvp/go.mse");
		return PASSES_PER_SEC(1); break;
	}
	case 4:
	{
		info->pvp->Packet();
		return 0; break;
	}
	default:
	{
		return 0; break;
	}
	}
};
#endif

CPVP::CPVP(DWORD dwPID1, DWORD dwPID2)
{
	if (dwPID1 > dwPID2)
	{
		m_players[0].dwPID = dwPID1;
		m_players[1].dwPID = dwPID2;
		m_players[0].bAgree = true;
	}
	else
	{
		m_players[0].dwPID = dwPID2;
		m_players[1].dwPID = dwPID1;
		m_players[1].bAgree = true;
	}

	DWORD adwID[2];
	adwID[0] = m_players[0].dwPID;
	adwID[1] = m_players[1].dwPID;
	m_dwCRC = GetFastHash((const char*)&adwID, 8);
	m_bRevenge = false;

	SetLastFightTime();
}

CPVP::CPVP(CPVP& k)
{
	m_players[0] = k.m_players[0];
	m_players[1] = k.m_players[1];

	m_dwCRC = k.m_dwCRC;
	m_bRevenge = k.m_bRevenge;

	SetLastFightTime();
}

CPVP::~CPVP()
{
}

void CPVP::Packet(bool bDelete)
{
	if (!m_players[0].dwVID || !m_players[1].dwVID)
	{
		if (bDelete)
			sys_err("null vid when removing %u %u", m_players[0].dwVID, m_players[0].dwVID);

		return;
	}

	TPacketGCPVP pack;

	pack.bHeader = HEADER_GC_PVP;

	if (bDelete)
	{
		pack.bMode = PVP_MODE_NONE;
		pack.dwVIDSrc = m_players[0].dwVID;
		pack.dwVIDDst = m_players[1].dwVID;
	}
	else if (IsFight())
	{
		pack.bMode = PVP_MODE_FIGHT;
		pack.dwVIDSrc = m_players[0].dwVID;
		pack.dwVIDDst = m_players[1].dwVID;
	}
	else
	{
		pack.bMode = m_bRevenge ? PVP_MODE_REVENGE : PVP_MODE_AGREE;

		if (m_players[0].bAgree)
		{
			pack.dwVIDSrc = m_players[0].dwVID;
			pack.dwVIDDst = m_players[1].dwVID;
		}
		else
		{
			pack.dwVIDSrc = m_players[1].dwVID;
			pack.dwVIDDst = m_players[0].dwVID;
		}
	}

	const DESC_MANAGER::DESC_SET& c_rSet = DESC_MANAGER::instance().GetClientSet();
	DESC_MANAGER::DESC_SET::const_iterator it = c_rSet.begin();

	while (it != c_rSet.end())
	{
		LPDESC d = *it++;

		if (d->IsPhase(PHASE_GAME) || d->IsPhase(PHASE_DEAD))
			d->Packet(&pack, sizeof(pack));
	}
}

bool CPVP::Agree(DWORD dwPID)
{
	m_players[m_players[0].dwPID != dwPID ? 1 : 0].bAgree = true;

	if (IsFight())
	{
#if defined(__PVP_COUNTDOWN__)
		if (m_pCountdown != NULL)
			event_cancel(&m_pCountdown);

		LPCHARACTER chA = CHARACTER_MANAGER::Instance().FindByPID(dwPID);
		LPCHARACTER chB = CHARACTER_MANAGER::Instance().FindByPID(m_players[m_players[0].dwPID != dwPID ? 0 : 1].dwPID);

		if (chA == NULL || chB == NULL)
			return false;

		{
			pvp_duel_countdown_info* info = AllocEventInfo<pvp_duel_countdown_info>();
			info->chA = chA;
			info->chB = chB;
			info->pvp = this;
			info->timer = 0;
			m_pCountdown = event_create(pvp_duel_countdown, info, PASSES_PER_SEC(1));
		}

		return true;
#else
		Packet();
		return true;
#endif
	}

	return false;
}

bool CPVP::IsFight()
{
	return (m_players[0].bAgree == m_players[1].bAgree) && m_players[0].bAgree;
}

void CPVP::Win(DWORD dwPID)
{
	int iSlot = m_players[0].dwPID != dwPID ? 1 : 0;

	m_bRevenge = true;

	m_players[iSlot].bAgree = true; // �ڵ����� ����
	m_players[!iSlot].bCanRevenge = true;
	m_players[!iSlot].bAgree = false;

	Packet();
}

bool CPVP::CanRevenge(DWORD dwPID)
{
	return m_players[m_players[0].dwPID != dwPID ? 1 : 0].bCanRevenge;
}

void CPVP::SetVID(DWORD dwPID, DWORD dwVID)
{
	if (m_players[0].dwPID == dwPID)
		m_players[0].dwVID = dwVID;
	else
		m_players[1].dwVID = dwVID;
}

void CPVP::SetLastFightTime()
{
	m_dwLastFightTime = get_dword_time();
}

DWORD CPVP::GetLastFightTime()
{
	return m_dwLastFightTime;
}

CPVPManager::CPVPManager()
{
}

CPVPManager::~CPVPManager()
{
}

void CPVPManager::Insert(LPCHARACTER pkChr, LPCHARACTER pkVictim)
{
	if (pkChr->IsDead() || pkVictim->IsDead())
		return;

	CPVP kPVP(pkChr->GetPlayerID(), pkVictim->GetPlayerID());

	CPVP* pkPVP;

	if ((pkPVP = Find(kPVP.m_dwCRC)))
	{
		// ������ �� ������ �ٷ� �ο�!
		if (pkPVP->Agree(pkChr->GetPlayerID()))
		{
			pkVictim->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s�԰��� ��� ����!"), pkChr->GetName());
			pkChr->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s�԰��� ��� ����!"), pkVictim->GetName());
		}
		return;
	}

	pkPVP = M2_NEW CPVP(kPVP);

	pkPVP->SetVID(pkChr->GetPlayerID(), pkChr->GetVID());
	pkPVP->SetVID(pkVictim->GetPlayerID(), pkVictim->GetVID());

	m_map_pkPVP.insert(map<DWORD, CPVP*>::value_type(pkPVP->m_dwCRC, pkPVP));

	m_map_pkPVPSetByID[pkChr->GetPlayerID()].insert(pkPVP);
	m_map_pkPVPSetByID[pkVictim->GetPlayerID()].insert(pkPVP);

	pkPVP->Packet();

	char msg[CHAT_MAX_LEN + 1];
	snprintf(msg, sizeof(msg), LC_STRING("%s���� ����û�� �߽��ϴ�. �³��Ϸ��� ��ᵿ�Ǹ� �ϼ���.", pkVictim->GetLanguage()), pkChr->GetName());

	pkVictim->ChatPacket(CHAT_TYPE_INFO, msg);
	pkChr->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s���� ����û�� �߽��ϴ�."), pkVictim->GetName());

	// NOTIFY_PVP_MESSAGE
	LPDESC pkVictimDesc = pkVictim->GetDesc();
	if (pkVictimDesc)
	{
		TPacketGCWhisper pack;

		int len = MIN(CHAT_MAX_LEN, strlen(msg) + 1);

		pack.bHeader = HEADER_GC_WHISPER;
		pack.wSize = sizeof(TPacketGCWhisper) + len;
		pack.bType = WHISPER_TYPE_SYSTEM;
		strlcpy(pack.szNameFrom, pkChr->GetName(), sizeof(pack.szNameFrom));

		TEMP_BUFFER buf;

		buf.write(&pack, sizeof(TPacketGCWhisper));
		buf.write(msg, len);

		pkVictimDesc->Packet(buf.read_peek(), buf.size());
	}
	// END_OF_NOTIFY_PVP_MESSAGE
}

void CPVPManager::ConnectEx(LPCHARACTER pkChr, bool bDisconnect)
{
	CPVPSetMap::iterator it = m_map_pkPVPSetByID.find(pkChr->GetPlayerID());

	if (it == m_map_pkPVPSetByID.end())
		return;

	DWORD dwVID = bDisconnect ? 0 : pkChr->GetVID();

	TR1_NS::unordered_set<CPVP*>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
		CPVP* pkPVP = *it2++;
		pkPVP->SetVID(pkChr->GetPlayerID(), dwVID);
	}
}

void CPVPManager::Connect(LPCHARACTER pkChr)
{
	ConnectEx(pkChr, false);
}

void CPVPManager::Disconnect(LPCHARACTER pkChr)
{
	//ConnectEx(pkChr, true);
}

void CPVPManager::GiveUp(LPCHARACTER pkChr, DWORD dwKillerPID) // This method is calling from no where yet.
{
	CPVPSetMap::iterator it = m_map_pkPVPSetByID.find(pkChr->GetPlayerID());

	if (it == m_map_pkPVPSetByID.end())
		return;

	sys_log(1, "PVPManager::Dead %d", pkChr->GetPlayerID());
	TR1_NS::unordered_set<CPVP*>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
		CPVP* pkPVP = *it2++;

		DWORD dwCompanionPID;

		if (pkPVP->m_players[0].dwPID == pkChr->GetPlayerID())
			dwCompanionPID = pkPVP->m_players[1].dwPID;
		else
			dwCompanionPID = pkPVP->m_players[0].dwPID;

		if (dwCompanionPID != dwKillerPID)
			continue;

		pkPVP->SetVID(pkChr->GetPlayerID(), 0);

		m_map_pkPVPSetByID.erase(dwCompanionPID);

		it->second.erase(pkPVP);

		if (it->second.empty())
			m_map_pkPVPSetByID.erase(it);

		m_map_pkPVP.erase(pkPVP->m_dwCRC);

		pkPVP->Packet(true);
		M2_DELETE(pkPVP);
		break;
	}
}

// ���ϰ�: 0 = PK, 1 = PVP
// PVP�� �����ϸ� ����ġ�� �������� ������ PK�� ������ �ʴ´�.
bool CPVPManager::Dead(LPCHARACTER pkChr, DWORD dwKillerPID)
{
	CPVPSetMap::iterator it = m_map_pkPVPSetByID.find(pkChr->GetPlayerID());

	if (it == m_map_pkPVPSetByID.end())
		return false;

	bool found = false;

	sys_log(1, "PVPManager::Dead %d", pkChr->GetPlayerID());
	TR1_NS::unordered_set<CPVP*>::iterator it2 = it->second.begin();

	while (it2 != it->second.end())
	{
		CPVP* pkPVP = *it2++;

		DWORD dwCompanionPID;

		if (pkPVP->m_players[0].dwPID == pkChr->GetPlayerID())
			dwCompanionPID = pkPVP->m_players[1].dwPID;
		else
			dwCompanionPID = pkPVP->m_players[0].dwPID;

		if (dwCompanionPID == dwKillerPID)
		{
			if (pkPVP->IsFight())
			{
				pkPVP->SetLastFightTime();
				pkPVP->Win(dwKillerPID);
				found = true;
				break;
			}
			else if (get_dword_time() - pkPVP->GetLastFightTime() <= 15000)
			{
				found = true;
				break;
			}
		}
	}

	return found;
}

bool CPVPManager::CanAttack(LPCHARACTER pkChr, LPCHARACTER pkVictim)
{
	switch (pkVictim->GetCharType())
	{
	case CHAR_TYPE_NPC:
	case CHAR_TYPE_WARP:
	case CHAR_TYPE_GOTO:
	case CHAR_TYPE_HORSE:
	case CHAR_TYPE_PET:
	case CHAR_TYPE_PET_PAY:
		return false;
	}

	if (pkChr == pkVictim) // ���� �� ĥ��� �ϳ� -_-
		return false;

	if (pkVictim->IsNPC() && pkChr->IsNPC() && !pkChr->IsGuardNPC())
		return false;

	if (true == pkChr->IsHorseRiding())
	{
		if (pkChr->GetHorseLevel() > 0 && 1 == pkChr->GetHorseGrade())
			return false;
	}
	else
	{
		switch (pkChr->GetMountVnum())
		{
		case 0:
		case 20030:

		case 20110:
		case 20111:
		case 20112:
		case 20113:

		case 20114:
		case 20115:
		case 20116:
		case 20117:
		case 20118:
		case 20119: // �󸶴� �渶

		case 20120:
		case 20121:
		case 20122:
		case 20123:
		case 20124:
		case 20125:

		//case 20201:
		//case 20202:
		//case 20203:
		//case 20204:

		// �ű� Ż�� ���
		case 20205:
		case 20206:
		case 20207:
		case 20208:

		case 20209:
		case 20210:
		case 20211:
		case 20212:

		//case 20213:
		case 20214: // ������ ����������
		case 20215: // ����� ����������
		//case 20216:
		case 20217: // ������ �����ϼ���
		case 20218: // ����� �����ϼ���

		case 20219: // �󸶴� �渶 Ŭ�� (�ҷ�����)
		case 20220: // ũ�������� Ż��
		case 20221: // ���� ���
		case 20222: // ���� �Ҵ�

		//case 20223:
		case 20224: // ������ ����������
		case 20225: // ����� ����������

		case 20226: // ������
		case 20227:

		//case 20228:
		case 20229:
		case 20230:

		case 20231:
		case 20232:
		case 20233:
		case 20234:
		case 20235:
		case 20236:
		case 20237:
		case 20238:
		case 20239:
		case 20240:
		case 20241:
		case 20242:
		case 20243:
		case 20244:
		case 20245:
		case 20246:
		case 20247:
		case 20248:
		case 20249:
		case 20250:
		case 20251:
		case 20252:
		case 20253:
		case 20254:
		case 20255:
		case 20257:
		case 20258:
		case 20259:
		case 20260:
		case 20261:
		case 20262:
		case 20263:
		case 20264:
		case 20265:
		case 20266:
		case 20267:
		case 20268:
		case 20269:
		case 20270:
		case 20271:
		case 20272:
		case 20273:
		case 20274:
		case 20275:
		case 20276:
		case 20277:
		case 20278:
		case 20279:
		case 20280:
		case 20281:
		case 20282:
		case 20283:
			break;

		default:
			return false;
		}
	}

	if (pkVictim->IsNPC() || pkChr->IsNPC())
		return true;

	if (pkVictim->IsObserverMode() || pkChr->IsObserverMode())
		return false;

	{
		BYTE bMapEmpire = SECTREE_MANAGER::instance().GetEmpireFromMapIndex(pkChr->GetMapIndex());

		if (pkChr->GetPKMode() == PK_MODE_PROTECT && pkChr->GetEmpire() == bMapEmpire ||
			pkVictim->GetPKMode() == PK_MODE_PROTECT && pkVictim->GetEmpire() == bMapEmpire)
		{
			return false;
		}
	}

	if (pkChr->GetEmpire() != pkVictim->GetEmpire())
	{
		if (LC_IsYMIR() == true || LC_IsKorea() == true)
		{
			if (pkChr->GetPKMode() == PK_MODE_PROTECT || pkVictim->GetPKMode() == PK_MODE_PROTECT)
			{
				return false;
			}
		}

		return true;
	}

	bool beKillerMode = false;

	if (pkVictim->GetParty() && pkVictim->GetParty() == pkChr->GetParty())
	{
		return false;
		// Cannot attack same party on any pvp model
	}
	else
	{
		if (pkVictim->IsKillerMode())
		{
			return true;
		}

		if (pkChr->GetAlignment() < 0 && pkVictim->GetAlignment() >= 0)
		{
			if (g_protectNormalPlayer)
			{
				// �����ڴ� ��ȭ����� ���ѻ���� ������ �� ����.
				if (PK_MODE_PEACE == pkVictim->GetPKMode())
					return false;
			}
		}

		switch (pkChr->GetPKMode())
		{
		case PK_MODE_PEACE:
		case PK_MODE_REVENGE:
			// Cannot attack same guild
			if (pkVictim->GetGuild() && pkVictim->GetGuild() == pkChr->GetGuild())
				break;

			if (pkChr->GetPKMode() == PK_MODE_REVENGE)
			{
				//if (!g_iUseLocale)
				if (1)
				{
					if (pkChr->GetAlignment() < 0 && pkVictim->GetAlignment() >= 0)
					{
						pkChr->SetKillerMode(true);
						return true;
					}
					else if (pkChr->GetAlignment() >= 0 && pkVictim->GetAlignment() < 0)
						return true;
				}
				else
				{
					if (pkChr->GetAlignment() < 0 && pkVictim->GetAlignment() < 0)
						break;
					else if (pkChr->GetAlignment() >= 0 && pkVictim->GetAlignment() >= 0)
						break;

					beKillerMode = true;
				}
			}
			break;

		case PK_MODE_GUILD:
			// Same implementation from PK_MODE_FREE except for attacking same guild
			if (!pkChr->GetGuild() || (pkVictim->GetGuild() != pkChr->GetGuild()))
			{
				if (1)
					//if (!g_iUseLocale)
				{
					if (pkVictim->GetAlignment() >= 0)
						pkChr->SetKillerMode(true);
					else if (pkChr->GetAlignment() < 0 && pkVictim->GetAlignment() < 0)
						pkChr->SetKillerMode(true);

					return true;
				}
				else
					beKillerMode = true;
			}
			break;

		case PK_MODE_FREE:
			//if (!g_iUseLocale)
			if (1)
			{
				if (pkVictim->GetAlignment() >= 0)
					pkChr->SetKillerMode(true);
				else if (pkChr->GetAlignment() < 0 && pkVictim->GetAlignment() < 0)
					pkChr->SetKillerMode(true);

				return true;
			}
			else
				beKillerMode = true;
			break;
		}
	}

	CPVP kPVP(pkChr->GetPlayerID(), pkVictim->GetPlayerID());
	CPVP* pkPVP = Find(kPVP.m_dwCRC);

	if (!pkPVP || !pkPVP->IsFight())
	{
		if (beKillerMode)
			pkChr->SetKillerMode(true);

		return (beKillerMode);
	}

	pkPVP->SetLastFightTime();
	return true;
}

CPVP* CPVPManager::Find(DWORD dwCRC)
{
	map<DWORD, CPVP*>::iterator it = m_map_pkPVP.find(dwCRC);

	if (it == m_map_pkPVP.end())
		return NULL;

	return it->second;
}

void CPVPManager::Delete(CPVP* pkPVP)
{
	map<DWORD, CPVP*>::iterator it = m_map_pkPVP.find(pkPVP->m_dwCRC);

	if (it == m_map_pkPVP.end())
		return;

	m_map_pkPVP.erase(it);
	m_map_pkPVPSetByID[pkPVP->m_players[0].dwPID].erase(pkPVP);
	m_map_pkPVPSetByID[pkPVP->m_players[1].dwPID].erase(pkPVP);

	M2_DELETE(pkPVP);
}

void CPVPManager::SendList(LPDESC d)
{
	map<DWORD, CPVP*>::iterator it = m_map_pkPVP.begin();

	DWORD dwVID = d->GetCharacter()->GetVID();

	TPacketGCPVP pack;

	pack.bHeader = HEADER_GC_PVP;

	while (it != m_map_pkPVP.end())
	{
		CPVP* pkPVP = (it++)->second;

		if (!pkPVP->m_players[0].dwVID || !pkPVP->m_players[1].dwVID)
			continue;

		// VID�� �Ѵ� ���� ��쿡�� ������.
		if (pkPVP->IsFight())
		{
			pack.bMode = PVP_MODE_FIGHT;
			pack.dwVIDSrc = pkPVP->m_players[0].dwVID;
			pack.dwVIDDst = pkPVP->m_players[1].dwVID;
		}
		else
		{
			pack.bMode = pkPVP->m_bRevenge ? PVP_MODE_REVENGE : PVP_MODE_AGREE;

			if (pkPVP->m_players[0].bAgree)
			{
				pack.dwVIDSrc = pkPVP->m_players[0].dwVID;
				pack.dwVIDDst = pkPVP->m_players[1].dwVID;
			}
			else
			{
				pack.dwVIDSrc = pkPVP->m_players[1].dwVID;
				pack.dwVIDDst = pkPVP->m_players[0].dwVID;
			}
		}

		d->Packet(&pack, sizeof(pack));
		sys_log(1, "PVPManager::SendList %d %d", pack.dwVIDSrc, pack.dwVIDDst);

		if (pkPVP->m_players[0].dwVID == dwVID)
		{
			LPCHARACTER ch = CHARACTER_MANAGER::instance().Find(pkPVP->m_players[1].dwVID);
			if (ch && ch->GetDesc())
			{
				LPDESC d = ch->GetDesc();
				d->Packet(&pack, sizeof(pack));
			}
		}
		else if (pkPVP->m_players[1].dwVID == dwVID)
		{
			LPCHARACTER ch = CHARACTER_MANAGER::instance().Find(pkPVP->m_players[0].dwVID);
			if (ch && ch->GetDesc())
			{
				LPDESC d = ch->GetDesc();
				d->Packet(&pack, sizeof(pack));
			}
		}
	}
}

void CPVPManager::Process()
{
	map<DWORD, CPVP*>::iterator it = m_map_pkPVP.begin();

	while (it != m_map_pkPVP.end())
	{
		CPVP* pvp = (it++)->second;

		if (get_dword_time() - pvp->GetLastFightTime() > 600000) // 10�� �̻� �ο��� ��������
		{
			pvp->Packet(true);
			Delete(pvp);
		}
	}
}
