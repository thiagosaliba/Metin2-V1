#include "StdAfx.h"
#include "PythonNetworkStream.h"
#include "Packet.h"
#include "PythonApplication.h"
#include "NetworkActorManager.h"

#include "AbstractPlayer.h"

#include "../EterPack/EterPackManager.h"

#ifdef ENABLE_LOADING_TIP
#include <random>
#endif

void CPythonNetworkStream::EnableChatInsultFilter(bool isEnable)
{
	m_isEnableChatInsultFilter = isEnable;
}

void CPythonNetworkStream::__FilterInsult(char* szLine, UINT uLineLen)
{
	m_kInsultChecker.FilterInsult(szLine, uLineLen);
}

bool CPythonNetworkStream::IsChatInsultIn(const char* c_szMsg)
{
	if (m_isEnableChatInsultFilter)
		return false;

	return IsInsultIn(c_szMsg);
}

bool CPythonNetworkStream::IsInsultIn(const char* c_szMsg)
{
	return m_kInsultChecker.IsInsultIn(c_szMsg, strlen(c_szMsg));
}

bool CPythonNetworkStream::LoadInsultList(const char* c_szInsultListFileName)
{
	CMappedFile file;
	const VOID* pvData;
	if (!CEterPackManager::Instance().Get(file, c_szInsultListFileName, &pvData))
		return false;

	CMemoryTextFileLoader kMemTextFileLoader;
	kMemTextFileLoader.Bind(file.Size(), pvData);

	m_kInsultChecker.Clear();
	for (DWORD dwLineIndex = 0; dwLineIndex < kMemTextFileLoader.GetLineCount(); ++dwLineIndex)
	{
		const std::string& c_rstLine = kMemTextFileLoader.GetLineString(dwLineIndex);
		m_kInsultChecker.AppendInsult(c_rstLine);
	}
	return true;
}

bool CPythonNetworkStream::LoadConvertTable(DWORD dwEmpireID, const char* c_szFileName)
{
	if (dwEmpireID < 1 || dwEmpireID >= 4)
		return false;

	CMappedFile file;
	const VOID* pvData;
	if (!CEterPackManager::Instance().Get(file, c_szFileName, &pvData))
		return false;

	DWORD dwEngCount = 26;
	DWORD dwHanCount = (0xc8 - 0xb0 + 1) * (0xfe - 0xa1 + 1);
	DWORD dwHanSize = dwHanCount * 2;
	DWORD dwFileSize = dwEngCount * 2 + dwHanSize;

	if (file.Size() < dwFileSize)
		return false;

	char* pcData = (char*)pvData;

	STextConvertTable& rkTextConvTable = m_aTextConvTable[dwEmpireID - 1];
	memcpy(rkTextConvTable.acUpper, pcData, dwEngCount); pcData += dwEngCount;
	memcpy(rkTextConvTable.acLower, pcData, dwEngCount); pcData += dwEngCount;
	memcpy(rkTextConvTable.aacHan, pcData, dwHanSize);

	return true;
}

bool CPythonNetworkStream::LoadLocaleQuestVnum(const char* c_szLocaleQuestFileName)
{
	CMappedFile file;
	const VOID* pvData;

	if (!CEterPackManager::Instance().Get(file, c_szLocaleQuestFileName, &pvData))
	{
		Tracef("CPythonNetworkStream::LoadLocaleQuestVnum(c_szLocaleQuestFileName=%s) - Load Error", c_szLocaleQuestFileName);
		return false;
	}

	CMemoryTextFileLoader kMemTextFileLoader;
	kMemTextFileLoader.Bind(file.Size(), pvData);

	m_kMap_dwID_strLocaleQuest.clear();

	CTokenVector TokenVector;
	for (DWORD i = 0; i < kMemTextFileLoader.GetLineCount(); ++i)
	{
		if (!kMemTextFileLoader.SplitLineByTab(i, &TokenVector))
			continue;

		const char* c_szComment = "#";
		if (TokenVector[0].compare(0, 1, c_szComment) == 0 || TokenVector.size() != 2)
			continue;

		m_kMap_dwID_strLocaleQuest.insert(std::make_pair(atoi(TokenVector[0].c_str()), TokenVector[1]));
	}

	return true;
}

const char* CPythonNetworkStream::GetLocaleQuestVnum(DWORD dwVnum)
{
	std::map<DWORD, std::string>::iterator it = m_kMap_dwID_strLocaleQuest.find(dwVnum);

	static char s_szChat[1024 + 1] = "";
	if (it != m_kMap_dwID_strLocaleQuest.end())
	{
		sprintf(s_szChat, "%s", it->second.c_str());
	}

	return s_szChat;
}

#ifdef ENABLE_LOADING_TIP
bool CPythonNetworkStream::LoadLoadingTipList(const char* c_szLoadingTipListFileName)
{
	CTextFileLoader kTextFileLoader;
	if (!kTextFileLoader.Load(c_szLoadingTipListFileName))
		return false;

	m_kMap_dwMapIndex_vecTipList.clear();

	for (DWORD i = 0; i < kTextFileLoader.GetChildNodeCount(); ++i)
	{
		CTextFileLoader::CGotoChild GotoChild(&kTextFileLoader, i);

		CTokenVector* pTipTokens;
		std::vector<DWORD> vecTipList;
		vecTipList.clear();

		if (kTextFileLoader.GetTokenVector("tip_vnum", &pTipTokens))
		{
			for (size_t j = 0; j < pTipTokens->size(); ++j)
			{
				vecTipList.push_back(atoi(pTipTokens->at(j).c_str()));
			}
		}

		CTokenVector* pMapTokens;
		std::vector<DWORD> vecMapList;
		vecMapList.clear();

		if (kTextFileLoader.GetTokenVector("map_index", &pMapTokens))
		{
			for (size_t k = 0; k < pMapTokens->size(); ++k)
			{
				vecMapList.push_back(atoi(pMapTokens->at(k).c_str()));
			}
		}

		if (vecTipList.size() > 0)
		{
			if (vecMapList.size() == 0)
			{
				m_kMap_dwMapIndex_vecTipList.insert(std::make_pair(0, vecTipList));
			}
			else
			{
				for (size_t l = 0; l < vecMapList.size(); ++l)
				{
					m_kMap_dwMapIndex_vecTipList.insert(std::make_pair(vecMapList.at(l), vecTipList));
				}
			}
		}
	}

	return true;
}

bool CPythonNetworkStream::LoadLoadingTipVnum(const char* c_szLoadingTipVnumFileName)
{
	CMappedFile file;
	const VOID* pvData;

	if (!CEterPackManager::Instance().Get(file, c_szLoadingTipVnumFileName, &pvData))
	{
		Tracef("CPythonNetworkStream::LoadLoadingTipVnum(c_szLoadingTipVnumFileName=%s) - Load Error", c_szLoadingTipVnumFileName);
		return false;
	}

	CMemoryTextFileLoader kMemTextFileLoader;
	kMemTextFileLoader.Bind(file.Size(), pvData);

	m_kMap_dwID_strTipString.clear();

	CTokenVector TokenVector;
	for (DWORD i = 0; i < kMemTextFileLoader.GetLineCount(); ++i)
	{
		if (!kMemTextFileLoader.SplitLineByTab(i, &TokenVector))
			continue;

		const char* c_szComment = "#";
		if (TokenVector[0].compare(0, 1, c_szComment) == 0 || TokenVector.size() != 2)
			continue;

		m_kMap_dwID_strTipString.insert(std::make_pair(atoi(TokenVector[0].c_str()), TokenVector[1]));
	}

	return true;
}

const char* CPythonNetworkStream::GetLoadingTipVnum(LONG lMapIndex)
{
	if (lMapIndex > 10000)
		lMapIndex /= 10000;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::map<DWORD, std::vector<DWORD>>::iterator it = m_kMap_dwMapIndex_vecTipList.find(lMapIndex);

	static char s_szTip[512] = "";

	if (it == m_kMap_dwMapIndex_vecTipList.end())
	{
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, m_kMap_dwMapIndex_vecTipList.begin()->second.size() - 1);
		sprintf(s_szTip, "%s", m_kMap_dwID_strTipString.find(m_kMap_dwMapIndex_vecTipList.begin()->second.at(dist(rd)))->second.c_str());
	}
	else
	{
		std::uniform_int_distribution<std::mt19937::result_type> dist(0, it->second.size() - 1);
		sprintf(s_szTip, "%s", m_kMap_dwID_strTipString.find(it->second.at(dist(rd)))->second.c_str());
	}

	return s_szTip;
}
#endif

// Loading ---------------------------------------------------------------------------
void CPythonNetworkStream::LoadingPhase()
{
	TPacketHeader header;
	if (!CheckPacket(&header))
		return;

	switch (header)
	{
	case HEADER_GC_PHASE:
		if (RecvPhasePacket())
			return;
		break;

	case HEADER_GC_MAIN_CHARACTER:
		if (RecvMainCharacter())
			return;
		break;

		// SUPPORT_BGM
	case HEADER_GC_MAIN_CHARACTER2_EMPIRE:
		if (RecvMainCharacter2_EMPIRE())
			return;
		break;

	case HEADER_GC_MAIN_CHARACTER3_BGM:
		if (RecvMainCharacter3_BGM())
			return;
		break;

	case HEADER_GC_MAIN_CHARACTER4_BGM_VOL:
		if (RecvMainCharacter4_BGM_VOL())
			return;
		break;

		// END_OF_SUPPORT_BGM

	case HEADER_GC_CHARACTER_UPDATE:
		if (RecvCharacterUpdatePacket())
			return;
		break;

	case HEADER_GC_PLAYER_POINTS:
		if (__RecvPlayerPoints())
			return;
		break;

	case HEADER_GC_PLAYER_POINT_CHANGE:
		if (RecvPointChange())
			return;
		break;

	case HEADER_GC_ITEM_SET_EMPTY:
		if (RecvItemSetEmptyPacket())
			return;
		break;

	case HEADER_GC_PING:
		if (RecvPingPacket())
			return;
		break;

	case HEADER_GC_QUICKSLOT_ADD:
		if (RecvQuickSlotAddPacket())
			return;
		break;

	case HEADER_GC_HYBRIDCRYPT_KEYS:
		RecvHybridCryptKeyPacket();
		return;
		break;

	case HEADER_GC_HYBRIDCRYPT_SDB:
		RecvHybridCryptSDBPacket();
		return;
		break;


	default:
		GamePhase();
		return;
		break;
	}

	RecvErrorPacket(header);
}

void CPythonNetworkStream::SetLoadingPhase()
{
	if ("Loading" != m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Loading Phase ##");
	Tracen("");

	m_strPhase = "Loading";

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::LoadingPhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveLoadingPhase);

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.Clear();

	CFlyingManager::Instance().DeleteAllInstances();
	CEffectManager::Instance().DeleteAllInstances();

	__DirectEnterMode_Initialize();
}

bool CPythonNetworkStream::RecvMainCharacter()
{
	TPacketGCMainCharacter MainChrPacket;
	if (!Recv(sizeof(TPacketGCMainCharacter), &MainChrPacket))
		return false;

	m_dwMainActorVID = MainChrPacket.dwVID;
	m_dwMainActorRace = MainChrPacket.wRaceNum;
	m_dwMainActorEmpire = 0;
	m_dwMainActorSkillGroup = MainChrPacket.bySkillGroup;

	m_rokNetActorMgr->SetMainActorVID(m_dwMainActorVID);

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetName(MainChrPacket.szName);
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", MainChrPacket.lX, MainChrPacket.lY));

	// Tracef(" >> RecvMainCharacter\n");

	Warp(MainChrPacket.lX, MainChrPacket.lY);
	SendClientVersionPacket();
	return true;
}

// SUPPORT_BGM
bool CPythonNetworkStream::RecvMainCharacter2_EMPIRE()
{
	TPacketGCMainCharacter2_EMPIRE mainChrPacket;
	if (!Recv(sizeof(mainChrPacket), &mainChrPacket))
		return false;

	m_dwMainActorVID = mainChrPacket.dwVID;
	m_dwMainActorRace = mainChrPacket.wRaceNum;
	m_dwMainActorEmpire = mainChrPacket.byEmpire;
	m_dwMainActorSkillGroup = mainChrPacket.bySkillGroup;

	m_rokNetActorMgr->SetMainActorVID(m_dwMainActorVID);

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetName(mainChrPacket.szName);
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", mainChrPacket.lX, mainChrPacket.lY));

	// Tracef(" >> RecvMainCharacterNew : %d\n", m_dwMainActorEmpire);

	Warp(mainChrPacket.lX, mainChrPacket.lY);
	SendClientVersionPacket();
	return true;
}

bool CPythonNetworkStream::RecvMainCharacter3_BGM()
{
	TPacketGCMainCharacter3_BGM mainChrPacket;
	if (!Recv(sizeof(mainChrPacket), &mainChrPacket))
		return false;

	m_dwMainActorVID = mainChrPacket.dwVID;
	m_dwMainActorRace = mainChrPacket.wRaceNum;
	m_dwMainActorEmpire = mainChrPacket.byEmpire;
	m_dwMainActorSkillGroup = mainChrPacket.bySkillGroup;

	m_rokNetActorMgr->SetMainActorVID(m_dwMainActorVID);

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetName(mainChrPacket.szUserName);
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

	__SetFieldMusicFileName(mainChrPacket.szBGMName);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", mainChrPacket.lX, mainChrPacket.lY));

	//Tracef(" >> RecvMainCharacterNew : %d\n", m_dwMainActorEmpire);

	Warp(mainChrPacket.lX, mainChrPacket.lY);
	SendClientVersionPacket();
	return true;
}

bool CPythonNetworkStream::RecvMainCharacter4_BGM_VOL()
{
	TPacketGCMainCharacter4_BGM_VOL mainChrPacket;
	if (!Recv(sizeof(mainChrPacket), &mainChrPacket))
		return false;

	m_dwMainActorVID = mainChrPacket.dwVID;
	m_dwMainActorRace = mainChrPacket.wRaceNum;
	m_dwMainActorEmpire = mainChrPacket.byEmpire;
	m_dwMainActorSkillGroup = mainChrPacket.bySkillGroup;

	m_rokNetActorMgr->SetMainActorVID(m_dwMainActorVID);

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	rkPlayer.SetName(mainChrPacket.szUserName);
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

	__SetFieldMusicFileInfo(mainChrPacket.szBGMName, mainChrPacket.fBGMVol);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_LOAD], "LoadData", Py_BuildValue("(ii)", mainChrPacket.lX, mainChrPacket.lY));

	//Tracef(" >> RecvMainCharacterNew : %d\n", m_dwMainActorEmpire);

	Warp(mainChrPacket.lX, mainChrPacket.lY);
	SendClientVersionPacket();
	return true;
}

static std::string gs_fieldMusic_fileName;
static float gs_fieldMusic_volume = 1.0f / 5.0f * 0.1f;

void CPythonNetworkStream::__SetFieldMusicFileName(const char* musicName)
{
	gs_fieldMusic_fileName = musicName;
}

void CPythonNetworkStream::__SetFieldMusicFileInfo(const char* musicName, float vol)
{
	gs_fieldMusic_fileName = musicName;
	gs_fieldMusic_volume = vol;
}

const char* CPythonNetworkStream::GetFieldMusicFileName()
{
	return gs_fieldMusic_fileName.c_str();
}

float CPythonNetworkStream::GetFieldMusicVolume()
{
	return gs_fieldMusic_volume;
}
// END_OF_SUPPORT_BGM


bool CPythonNetworkStream::__RecvPlayerPoints()
{
	TPacketGCPoints PointsPacket;

	if (!Recv(sizeof(TPacketGCPoints), &PointsPacket))
		return false;

	for (DWORD i = 0; i < POINT_MAX_NUM; ++i)
	{
		CPythonPlayer::Instance().SetStatus(i, PointsPacket.points[i]);

#ifdef ENABLE_IMPROVED_LOGOUT_POINTS
		if (i == POINT_LEVEL)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byLevel = PointsPacket.points[i];
		else if (i == POINT_ST)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byST = PointsPacket.points[i];
		else if (i == POINT_HT)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byHT = PointsPacket.points[i];
		else if (i == POINT_DX)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byDX = PointsPacket.points[i];
		else if (i == POINT_IQ)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byIQ = PointsPacket.points[i];

#	if defined(ENABLE_CONQUEROR_LEVEL)
		if (i == POINT_CONQUEROR_LEVEL)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byConquerorLevel = PointsPacket.points[i];
		else if (i == POINT_SUNGMA_STR)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].bySungmaStr = PointsPacket.points[i];
		else if (i == POINT_SUNGMA_HP)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].bySungmaHp = PointsPacket.points[i];
		else if (i == POINT_SUNGMA_MOVE)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].bySungmaMove = PointsPacket.points[i];
		else if (i == POINT_SUNGMA_IMMUNE)
			m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].bySungmaImmune = PointsPacket.points[i];
#	endif
#endif
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshStatus", Py_BuildValue("()"));
	return true;
}

void CPythonNetworkStream::StartGame()
{
	m_isStartGame = TRUE;
}

bool CPythonNetworkStream::SendEnterGame()
{
	TPacketCGEnterFrontGame EnterFrontGamePacket;

	EnterFrontGamePacket.header = HEADER_CG_ENTERGAME;

	if (!Send(sizeof(EnterFrontGamePacket), &EnterFrontGamePacket))
	{
		Tracen("Send EnterFrontGamePacket");
		return false;
	}

	if (!SendSequence())
		return false;

	m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].dwLastPlay = static_cast<DWORD>(time(0));

	__SendInternalBuffer();
	return true;
}
