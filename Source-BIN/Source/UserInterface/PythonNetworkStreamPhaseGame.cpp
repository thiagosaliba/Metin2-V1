#include "StdAfx.h"
#include "PythonNetworkStream.h"
#include "Packet.h"

#include "PythonGuild.h"
#include "PythonCharacterManager.h"
#include "PythonPlayer.h"
#include "PythonBackground.h"
#include "PythonMiniMap.h"
#include "PythonTextTail.h"
#include "PythonItem.h"
#include "PythonChat.h"
#include "PythonShop.h"
#include "PythonExchange.h"
#include "PythonQuest.h"
#include "PythonEventManager.h"
#include "PythonMessenger.h"
#include "PythonApplication.h"
#if defined(ENABLE_MAILBOX)
#	include "PythonMailBox.h"
#endif

#include "../EterPack/EterPackManager.h"
#include "../GameLib/ItemManager.h"

#include "AbstractApplication.h"
#include "AbstractCharacterManager.h"
#include "InstanceBase.h"

#include "ProcessCRC.h"
#include "../GameLib/ItemData.h"

#if defined(ENABLE_DISCORD_RPC)
#include "discord_rpc.h"
#endif

BOOL gs_bEmpireLanuageEnable = TRUE;

static int64_t StartTime;
static constexpr auto DiscordClientID = "754135131656290425"; // Change

#if defined(ENABLE_DISCORD_RPC)
void CPythonNetworkStream::Discord_Start()
{
	StartTime = time(0);
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize(DiscordClientID, &handlers, 1, nullptr);
	Discord_Update(false /* bGameState */);
}

void CPythonNetworkStream::Discord_Update(const bool bGameState)
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	if (bGameState)
	{
		discordPresence.details = CPythonPlayer::Instance().GetName();
		discordPresence.state = CPythonBackground::Instance().GetWarpMapName();

		discordPresence.startTimestamp = StartTime;

		discordPresence.largeImageKey = "logo";
		discordPresence.largeImageText = "OSF Project";

		switch (CPythonPlayer::Instance().GetRace())
		{
		case MAIN_RACE_WARRIOR_M:
			discordPresence.smallImageKey = "warrior_m";
			discordPresence.smallImageText = "Warrior";
			break;

		case MAIN_RACE_WARRIOR_W:
			discordPresence.smallImageKey = "warrior_w";
			discordPresence.smallImageText = "Warrior";
			break;

		case MAIN_RACE_ASSASSIN_M:
			discordPresence.smallImageKey = "assassin_m";
			discordPresence.smallImageText = "Assassin";
			break;

		case MAIN_RACE_ASSASSIN_W:
			discordPresence.smallImageKey = "assassin_w";
			discordPresence.smallImageText = "Assassin";
			break;

		case MAIN_RACE_SURA_M:
			discordPresence.smallImageKey = "sura_m";
			discordPresence.smallImageText = "Sura";
			break;

		case MAIN_RACE_SURA_W:
			discordPresence.smallImageKey = "sura_w";
			discordPresence.smallImageText = "Sura";
			break;

		case MAIN_RACE_SHAMAN_M:
			discordPresence.smallImageKey = "shaman_m";
			discordPresence.smallImageText = "Shaman";
			break;

		case MAIN_RACE_SHAMAN_W:
			discordPresence.smallImageKey = "shaman_w";
			discordPresence.smallImageText = "Shaman";
			break;

		case MAIN_RACE_WOLFMAN_M:
			discordPresence.smallImageKey = "wolfman_w";
			discordPresence.smallImageText = "Wolfman";
			break;

		}
	}
	Discord_UpdatePresence(&discordPresence);
}

void CPythonNetworkStream::Discord_Close()
{
	Discord_Shutdown();
}
#endif

void CPythonNetworkStream::__RefreshAlignmentWindow()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshAlignment", Py_BuildValue("()"));
}

void CPythonNetworkStream::__RefreshTargetBoardByVID(DWORD dwVID)
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshTargetBoardByVID", Py_BuildValue("(i)", dwVID));
}

void CPythonNetworkStream::__RefreshTargetBoardByName(const char* c_szName)
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshTargetBoardByName", Py_BuildValue("(s)", c_szName));
}

void CPythonNetworkStream::__RefreshTargetBoard()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshTargetBoard", Py_BuildValue("()"));
}

void CPythonNetworkStream::__RefreshGuildWindowGradePage()
{
	m_isRefreshGuildWndGradePage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowSkillPage()
{
	m_isRefreshGuildWndSkillPage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowMemberPageGradeComboBox()
{
	m_isRefreshGuildWndMemberPageGradeComboBox = true;
}

void CPythonNetworkStream::__RefreshGuildWindowMemberPage()
{
	m_isRefreshGuildWndMemberPage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowBoardPage()
{
	m_isRefreshGuildWndBoardPage = true;
}

void CPythonNetworkStream::__RefreshGuildWindowInfoPage()
{
	m_isRefreshGuildWndInfoPage = true;
}

void CPythonNetworkStream::__RefreshMessengerWindow()
{
	m_isRefreshMessengerWnd = true;
}

void CPythonNetworkStream::__RefreshSafeboxWindow()
{
	m_isRefreshSafeboxWnd = true;
}

void CPythonNetworkStream::__RefreshMallWindow()
{
	m_isRefreshMallWnd = true;
}

void CPythonNetworkStream::__RefreshSkillWindow()
{
	m_isRefreshSkillWnd = true;
}

void CPythonNetworkStream::__RefreshExchangeWindow()
{
	m_isRefreshExchangeWnd = true;
}

void CPythonNetworkStream::__RefreshStatus()
{
	m_isRefreshStatus = true;
}

void CPythonNetworkStream::__RefreshCharacterWindow()
{
	m_isRefreshCharacterWnd = true;
}

void CPythonNetworkStream::__RefreshInventoryWindow()
{
	m_isRefreshInventoryWnd = true;
}

void CPythonNetworkStream::__RefreshEquipmentWindow()
{
	m_isRefreshEquipmentWnd = true;
}

void CPythonNetworkStream::__SetGuildID(DWORD id)
{
	if (m_dwGuildID != id)
	{
		m_dwGuildID = id;
		IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();

		for (int i = 0; i < PLAYER_PER_ACCOUNT4; ++i)
			if (!strncmp(m_akSimplePlayerInfo[i].szName, rkPlayer.GetName(), CHARACTER_NAME_MAX_LEN))
			{
				m_adwGuildID[i] = id;

				std::string guildName;
				if (CPythonGuild::Instance().GetGuildName(id, &guildName))
				{
					m_astrGuildName[i] = guildName;
				}
				else
				{
					m_astrGuildName[i] = "";
				}
			}
	}
}

struct PERF_PacketInfo
{
	DWORD dwCount;
	DWORD dwTime;

	PERF_PacketInfo()
	{
		dwCount = 0;
		dwTime = 0;
	}
};

#ifdef __PERFORMANCE_CHECK__

class PERF_PacketTimeAnalyzer
{
public:
	~PERF_PacketTimeAnalyzer()
	{
		FILE* fp = fopen("perf_dispatch_packet_result.txt", "w");

		for (std::map<DWORD, PERF_PacketInfo>::iterator i = m_kMap_kPacketInfo.begin(); i != m_kMap_kPacketInfo.end(); ++i)
		{
			if (i->second.dwTime > 0)
				fprintf(fp, "header %d: count %d, time %d, tpc %d\n", i->first, i->second.dwCount, i->second.dwTime, i->second.dwTime / i->second.dwCount);
		}
		fclose(fp);
	}

public:
	std::map<DWORD, PERF_PacketInfo> m_kMap_kPacketInfo;
};

PERF_PacketTimeAnalyzer gs_kPacketTimeAnalyzer;

#endif

// Game Phase ---------------------------------------------------------------------------
void CPythonNetworkStream::GamePhase()
{
	if (!m_kQue_stHack.empty())
	{
		__SendHack(m_kQue_stHack.front().c_str());
		m_kQue_stHack.pop_front();
	}

	TPacketHeader header = 0;
	bool ret = true;

#ifdef __PERFORMANCE_CHECK__
	DWORD timeBeginDispatch = timeGetTime();

	static std::map<DWORD, PERF_PacketInfo> kMap_kPacketInfo;
	kMap_kPacketInfo.clear();
#endif

#ifdef __DOUBLE_RECV_BUFFER__
	const DWORD MAX_RECV_COUNT = 4 * 2;
	const DWORD SAFE_RECV_BUFSIZE = 8192 * 2;
#else
	const DWORD MAX_RECV_COUNT = 4;
	const DWORD SAFE_RECV_BUFSIZE = 8192;
#endif
	DWORD dwRecvCount = 0;

	while (ret)
	{
		if (dwRecvCount++ >= MAX_RECV_COUNT - 1 && GetRecvBufferSize() < SAFE_RECV_BUFSIZE
			&& m_strPhase == "Game") // phase_game 이 아니어도 여기로 들어오는 경우가 있다.
			break;

		if (!CheckPacket(&header))
			break;

#ifdef __PERFORMANCE_CHECK__
		DWORD timeBeginPacket = timeGetTime();
#endif

		switch (header)
		{
		case HEADER_GC_OBSERVER_ADD:
			ret = RecvObserverAddPacket();
			break;

		case HEADER_GC_OBSERVER_REMOVE:
			ret = RecvObserverRemovePacket();
			break;

		case HEADER_GC_OBSERVER_MOVE:
			ret = RecvObserverMovePacket();
			break;

		case HEADER_GC_WARP:
			ret = RecvWarpPacket();
			break;

		case HEADER_GC_PHASE:
			ret = RecvPhasePacket();
			return; // 도중에 Phase 가 바뀌면 일단 무조건 GamePhase 탈출 - [levites]

		case HEADER_GC_PVP:
			ret = RecvPVPPacket();
			break;

		case HEADER_GC_DUEL_START:
			ret = RecvDuelStartPacket();
			break;

		case HEADER_GC_CHARACTER_ADD:
			ret = RecvCharacterAppendPacket();
			break;

		case HEADER_GC_CHAR_ADDITIONAL_INFO:
			ret = RecvCharacterAdditionalInfo();
			break;

		case HEADER_GC_CHARACTER_ADD2:
			ret = RecvCharacterAppendPacketNew();
			break;

		case HEADER_GC_CHARACTER_UPDATE:
			ret = RecvCharacterUpdatePacket();
			break;

		case HEADER_GC_CHARACTER_UPDATE2:
			ret = RecvCharacterUpdatePacketNew();
			break;

		case HEADER_GC_CHARACTER_DEL:
			ret = RecvCharacterDeletePacket();
			break;

		case HEADER_GC_CHAT:
			ret = RecvChatPacket();
			break;

		case HEADER_GC_SYNC_POSITION:
			ret = RecvSyncPositionPacket();
			break;

		case HEADER_GC_OWNERSHIP:
			ret = RecvOwnerShipPacket();
			break;

		case HEADER_GC_WHISPER:
			ret = RecvWhisperPacket();
			break;

		case HEADER_GC_CHARACTER_MOVE:
			ret = RecvCharacterMovePacket();
			break;

			// Position
		case HEADER_GC_CHARACTER_POSITION:
			ret = RecvCharacterPositionPacket();
			break;

			// Battle Packet
		case HEADER_GC_STUN:
			ret = RecvStunPacket();
			break;

		case HEADER_GC_DEAD:
			ret = RecvDeadPacket();
			break;

		case HEADER_GC_PLAYER_POINT_CHANGE:
			ret = RecvPointChange();
			break;

			// Item packet
		case HEADER_GC_ITEM_SET_EMPTY:
			ret = RecvItemSetEmptyPacket();
			break;

		case HEADER_GC_ITEM_SET:
			ret = RecvItemSetPacket();
			break;

		case HEADER_GC_ITEM_USE:
			ret = RecvItemUsePacket();
			break;

		case HEADER_GC_ITEM_UPDATE:
			ret = RecvItemUpdatePacket();
			break;

		case HEADER_GC_ITEM_GROUND_ADD:
			ret = RecvItemGroundAddPacket();
			break;

		case HEADER_GC_ITEM_GROUND_DEL:
			ret = RecvItemGroundDelPacket();
			break;

		case HEADER_GC_ITEM_OWNERSHIP:
			ret = RecvItemOwnership();
			break;

		case HEADER_GC_QUICKSLOT_ADD:
			ret = RecvQuickSlotAddPacket();
			break;

		case HEADER_GC_QUICKSLOT_DEL:
			ret = RecvQuickSlotDelPacket();
			break;

		case HEADER_GC_QUICKSLOT_SWAP:
			ret = RecvQuickSlotMovePacket();
			break;

		case HEADER_GC_MOTION:
			ret = RecvMotionPacket();
			break;

		case HEADER_GC_SHOP:
			ret = RecvShopPacket();
			break;

		case HEADER_GC_SHOP_SIGN:
			ret = RecvShopSignPacket();
			break;

		case HEADER_GC_EXCHANGE:
			ret = RecvExchangePacket();
			break;

		case HEADER_GC_QUEST_INFO:
			ret = RecvQuestInfoPacket();
			break;

		case HEADER_GC_REQUEST_MAKE_GUILD:
			ret = RecvRequestMakeGuild();
			break;

		case HEADER_GC_PING:
			ret = RecvPingPacket();
			break;

		case HEADER_GC_SCRIPT:
			ret = RecvScriptPacket();
			break;

		case HEADER_GC_QUEST_CONFIRM:
			ret = RecvQuestConfirmPacket();
			break;

		case HEADER_GC_TARGET:
			ret = RecvTargetPacket();
			break;

		case HEADER_GC_DAMAGE_INFO:
			ret = RecvDamageInfoPacket();
			break;

		case HEADER_GC_MOUNT:
			ret = RecvMountPacket();
			break;

		case HEADER_GC_CHANGE_SPEED:
			ret = RecvChangeSpeedPacket();
			break;

		case HEADER_GC_PLAYER_POINTS:
			ret = __RecvPlayerPoints();
			break;

		case HEADER_GC_CREATE_FLY:
			ret = RecvCreateFlyPacket();
			break;

		case HEADER_GC_FLY_TARGETING:
			ret = RecvFlyTargetingPacket();
			break;

		case HEADER_GC_ADD_FLY_TARGETING:
			ret = RecvAddFlyTargetingPacket();
			break;

		case HEADER_GC_SKILL_LEVEL:
			ret = RecvSkillLevel();
			break;

		case HEADER_GC_SKILL_LEVEL_NEW:
			ret = RecvSkillLevelNew();
			break;

		case HEADER_GC_MESSENGER:
			ret = RecvMessenger();
			break;

		case HEADER_GC_GUILD:
			ret = RecvGuild();
			break;

		case HEADER_GC_PARTY_INVITE:
			ret = RecvPartyInvite();
			break;

		case HEADER_GC_PARTY_ADD:
			ret = RecvPartyAdd();
			break;

		case HEADER_GC_PARTY_UPDATE:
			ret = RecvPartyUpdate();
			break;

		case HEADER_GC_PARTY_REMOVE:
			ret = RecvPartyRemove();
			break;

		case HEADER_GC_PARTY_LINK:
			ret = RecvPartyLink();
			break;

		case HEADER_GC_PARTY_UNLINK:
			ret = RecvPartyUnlink();
			break;

		case HEADER_GC_PARTY_PARAMETER:
			ret = RecvPartyParameter();
			break;

#if defined(WJ_SHOW_PARTY_ON_MINIMAP)
		case HEADER_GC_PARTY_POSITION_INFO:
			ret = RecvPartyPositionInfo();
			break;
#endif

		case HEADER_GC_SAFEBOX_SET:
			ret = RecvSafeBoxSetPacket();
			break;

		case HEADER_GC_SAFEBOX_DEL:
			ret = RecvSafeBoxDelPacket();
			break;

		case HEADER_GC_SAFEBOX_WRONG_PASSWORD:
			ret = RecvSafeBoxWrongPasswordPacket();
			break;

		case HEADER_GC_SAFEBOX_SIZE:
			ret = RecvSafeBoxSizePacket();
			break;

		case HEADER_GC_FISHING:
			ret = RecvFishing();
			break;

		case HEADER_GC_DUNGEON:
			ret = RecvDungeon();
			break;

		case HEADER_GC_TIME:
			ret = RecvTimePacket();
			break;

		case HEADER_GC_WALK_MODE:
			ret = RecvWalkModePacket();
			break;

		case HEADER_GC_CHANGE_SKILL_GROUP:
			ret = RecvChangeSkillGroupPacket();
			break;

		case HEADER_GC_REFINE_INFORMATION:
			ret = RecvRefineInformationPacket();
			break;

		case HEADER_GC_REFINE_INFORMATION_NEW:
			ret = RecvRefineInformationPacketNew();
			break;

		case HEADER_GC_SEPCIAL_EFFECT:
			ret = RecvSpecialEffect();
			break;

		case HEADER_GC_NPC_POSITION:
			ret = RecvNPCList();
			break;

		case HEADER_GC_CHANNEL:
			ret = RecvChannelPacket();
			break;

		case HEADER_GC_VIEW_EQUIP:
			ret = RecvViewEquipPacket();
			break;

		case HEADER_GC_LAND_LIST:
			ret = RecvLandPacket();
			break;

			//case HEADER_GC_TARGET_CREATE:
			//	ret = RecvTargetCreatePacket();
			//	break;

		case HEADER_GC_TARGET_CREATE_NEW:
			ret = RecvTargetCreatePacketNew();
			break;

		case HEADER_GC_TARGET_UPDATE:
			ret = RecvTargetUpdatePacket();
			break;

		case HEADER_GC_TARGET_DELETE:
			ret = RecvTargetDeletePacket();
			break;

		case HEADER_GC_AFFECT_ADD:
			ret = RecvAffectAddPacket();
			break;

		case HEADER_GC_AFFECT_REMOVE:
			ret = RecvAffectRemovePacket();
			break;

		case HEADER_GC_MALL_OPEN:
			ret = RecvMallOpenPacket();
			break;

		case HEADER_GC_MALL_SET:
			ret = RecvMallItemSetPacket();
			break;

		case HEADER_GC_MALL_DEL:
			ret = RecvMallItemDelPacket();
			break;

		case HEADER_GC_LOVER_INFO:
			ret = RecvLoverInfoPacket();
			break;

		case HEADER_GC_LOVE_POINT_UPDATE:
			ret = RecvLovePointUpdatePacket();
			break;

		case HEADER_GC_DIG_MOTION:
			ret = RecvDigMotionPacket();
			break;

		case HEADER_GC_HANDSHAKE:
			RecvHandshakePacket();
			return;

		case HEADER_GC_HANDSHAKE_OK:
			RecvHandshakeOKPacket();
			return;

		case HEADER_GC_HYBRIDCRYPT_KEYS:
			RecvHybridCryptKeyPacket();
			return;

		case HEADER_GC_HYBRIDCRYPT_SDB:
			RecvHybridCryptSDBPacket();
			return;

#if defined(__IMPROVED_PACKET_ENCRYPTION__)
		case HEADER_GC_KEY_AGREEMENT:
			RecvKeyAgreementPacket();
			return;

		case HEADER_GC_KEY_AGREEMENT_COMPLETED:
			RecvKeyAgreementCompletedPacket();
			return;
#endif

		case HEADER_GC_SPECIFIC_EFFECT:
			ret = RecvSpecificEffect();
			break;

		case HEADER_GC_DRAGON_SOUL_REFINE:
			ret = RecvDragonSoulRefine();
			break;

#ifdef ENABLE_SEND_TARGET_INFO
		case HEADER_GC_TARGET_INFO:
			ret = RecvTargetInfoPacket();
			break;
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		case HEADER_GC_ACCE:
			ret = RecvAccePacket();
			break;
#endif

#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
		case HEADER_GC_CHANGE_LOOK_SET:
			ret = RecvChangeLookItemSetPacket();
			break;

		case HEADER_GC_CHANGE_LOOK_DEL:
			ret = RecvChangeLookItemDelPacket();
			break;

		case HEADER_GC_CHANGE_LOOK_FREE_SET:
			ret = RecvChangeLookFreeItemSetPacket();
			break;

		case HEADER_GC_CHANGE_LOOK_FREE_DEL:
			ret = RecvChangeLookFreeItemDelPacket();
			break;
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
		case HEADER_GC_MINI_GAME_CATCH_KING:
			ret = RecvMiniGameCatchKingPacket();
			break;
#endif

#if defined(ENABLE_PRIVATESHOP_SEARCH_SYSTEM)
		case HEADER_GC_PRIVATESHOP_SEARCH:
			ret = RecvPrivateShopSearch();
			break;
		case HEADER_GC_PRIVATESHOP_SEARCH_OPEN:
			ret = RecvPrivateShopSearchOpen();
			break;
#endif

		case HEADER_GC_REQUEST_CHANGE_LANGUAGE:
			ret = RecvRequestChangeLanguage();
			break;

		case HEADER_GC_WHISPER_DETAILS:
			ret = RecvWhisperDetails();
			break;

#if defined(ENABLE_MAILBOX)
		case HEADER_GC_MAILBOX_PROCESS:
			ret = RecvMailboxProcess();
			break;

		case HEADER_GC_MAILBOX:
			ret = RecvMailbox();
			break;

		case HEADER_GC_MAILBOX_ADD_DATA:
			ret = RecvMailboxAddData();
			break;

		case HEADER_GC_MAILBOX_ALL:
			ret = RecvMailboxAll();
			break;

		case HEADER_GC_MAILBOX_UNREAD:
			ret = RecvMailboxUnread();
			break;
#endif

		default:
			ret = RecvDefaultPacket(header);
			break;
		}

#ifdef __PERFORMANCE_CHECK__
		DWORD timeEndPacket = timeGetTime();

		{
			PERF_PacketInfo& rkPacketInfo = kMap_kPacketInfo[header];
			rkPacketInfo.dwCount++;
			rkPacketInfo.dwTime += timeEndPacket - timeBeginPacket;
		}

		{
			PERF_PacketInfo& rkPacketInfo = gs_kPacketTimeAnalyzer.m_kMap_kPacketInfo[header];
			rkPacketInfo.dwCount++;
			rkPacketInfo.dwTime += timeEndPacket - timeBeginPacket;
		}
#endif
	}

#ifdef __PERFORMANCE_CHECK__
	DWORD timeEndDispatch = timeGetTime();

	if (timeEndDispatch - timeBeginDispatch > 2)
	{
		static FILE* fp = fopen("perf_dispatch_packet.txt", "w");

		fprintf(fp, "delay %d\n", timeEndDispatch - timeBeginDispatch);
		for (std::map<DWORD, PERF_PacketInfo>::iterator i = kMap_kPacketInfo.begin(); i != kMap_kPacketInfo.end(); ++i)
		{
			if (i->second.dwTime > 0)
				fprintf(fp, "header %d: count %d, time %d\n", i->first, i->second.dwCount, i->second.dwTime);
		}
		fputs("=====================================================\n", fp);
		fflush(fp);
	}
#endif

	if (!ret)
		RecvErrorPacket(header);

	static DWORD s_nextRefreshTime = ELTimer_GetMSec();

	DWORD curTime = ELTimer_GetMSec();
	if (s_nextRefreshTime > curTime)
		return;

	if (m_isRefreshCharacterWnd)
	{
		m_isRefreshCharacterWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshCharacter", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshEquipmentWnd)
	{
		m_isRefreshEquipmentWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshEquipment", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshInventoryWnd)
	{
		m_isRefreshInventoryWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshInventory", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshExchangeWnd)
	{
		m_isRefreshExchangeWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshExchange", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshSkillWnd)
	{
		m_isRefreshSkillWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSkill", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshSafeboxWnd)
	{
		m_isRefreshSafeboxWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshSafebox", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshMallWnd)
	{
		m_isRefreshMallWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshMall", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshStatus)
	{
		m_isRefreshStatus = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshStatus", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshMessengerWnd)
	{
		m_isRefreshMessengerWnd = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshMessenger", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndInfoPage)
	{
		m_isRefreshGuildWndInfoPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildInfoPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndBoardPage)
	{
		m_isRefreshGuildWndBoardPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildBoardPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndMemberPage)
	{
		m_isRefreshGuildWndMemberPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildMemberPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndMemberPageGradeComboBox)
	{
		m_isRefreshGuildWndMemberPageGradeComboBox = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildMemberPageGradeComboBox", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndSkillPage)
	{
		m_isRefreshGuildWndSkillPage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildSkillPage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}

	if (m_isRefreshGuildWndGradePage)
	{
		m_isRefreshGuildWndGradePage = false;
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildGradePage", Py_BuildValue("()"));
		s_nextRefreshTime = curTime + 300;
	}
}

void CPythonNetworkStream::__InitializeGamePhase()
{
	__ServerTimeSync_Initialize();

	m_isRefreshStatus = false;
	m_isRefreshCharacterWnd = false;
	m_isRefreshEquipmentWnd = false;
	m_isRefreshInventoryWnd = false;
	m_isRefreshExchangeWnd = false;
	m_isRefreshSkillWnd = false;
	m_isRefreshSafeboxWnd = false;
	m_isRefreshMallWnd = false;
	m_isRefreshMessengerWnd = false;
	m_isRefreshGuildWndInfoPage = false;
	m_isRefreshGuildWndBoardPage = false;
	m_isRefreshGuildWndMemberPage = false;
	m_isRefreshGuildWndMemberPageGradeComboBox = false;
	m_isRefreshGuildWndSkillPage = false;
	m_isRefreshGuildWndGradePage = false;

	m_EmoticonStringVector.clear();

	m_pInstTarget = NULL;
}

void CPythonNetworkStream::Warp(LONG lGlobalX, LONG lGlobalY)
{
	CPythonBackground& rkBgMgr = CPythonBackground::Instance();
	rkBgMgr.SetShadowLevel(CPythonBackground::SHADOW_NONE);
	rkBgMgr.Destroy();

	rkBgMgr.Create();
	rkBgMgr.Warp(lGlobalX, lGlobalY);
	rkBgMgr.RefreshShadowLevel();

	// NOTE : Warp 했을때 CenterPosition의 Height가 0이기 때문에 카메라가 땅바닥에 박혀있게 됨
	//        움직일때마다 Height가 갱신 되기 때문이므로 맵을 이동하면 Position을 강제로 한번
	//        셋팅해준다 - [levites]
	LONG lLocalX = lGlobalX;
	LONG lLocalY = lGlobalY;
	__GlobalPositionToLocalPosition(lLocalX, lLocalY);
	float fHeight = CPythonBackground::Instance().GetHeight(float(lLocalX), float(lLocalY));

	IAbstractApplication& rkApp = IAbstractApplication::GetSingleton();
	rkApp.SetCenterPosition(float(lLocalX), float(lLocalY), fHeight);

	__ShowMapName(lLocalX, lLocalY);
}

void CPythonNetworkStream::__ShowMapName(LONG lLocalX, LONG lLocalY)
{
	const std::string& c_rstrMapFileName = CPythonBackground::Instance().GetWarpMapName();
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ShowMapName", Py_BuildValue("(sii)", c_rstrMapFileName.c_str(), lLocalX, lLocalY));
}

void CPythonNetworkStream::__LeaveGamePhase()
{
	CInstanceBase::ClearPVPKeySystem();

	__ClearNetworkActorManager();

	m_bComboSkillFlag = FALSE;

	IAbstractCharacterManager& rkChrMgr = IAbstractCharacterManager::GetSingleton();
	rkChrMgr.Destroy();

	CPythonItem& rkItemMgr = CPythonItem::Instance();
	rkItemMgr.Destroy();
}

void CPythonNetworkStream::SetGamePhase()
{
	if ("Game" != m_strPhase)
		m_phaseLeaveFunc.Run();

	Tracen("");
	Tracen("## Network - Game Phase ##");
	Tracen("");

	m_strPhase = "Game";

	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::GamePhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveGamePhase);

	// Main Character 등록O

	IAbstractPlayer& rkPlayer = IAbstractPlayer::GetSingleton();
	rkPlayer.SetMainCharacterIndex(GetMainActorVID());

	__RefreshStatus();
}

bool CPythonNetworkStream::RecvObserverAddPacket()
{
	TPacketGCObserverAdd kObserverAddPacket;
	if (!Recv(sizeof(kObserverAddPacket), &kObserverAddPacket))
		return false;

	CPythonMiniMap::Instance().AddObserver(
		kObserverAddPacket.vid,
		kObserverAddPacket.x * 100.0f,
		kObserverAddPacket.y * 100.0f);

	return true;
}

#ifdef ENABLE_SEND_TARGET_INFO
bool CPythonNetworkStream::SendTargetInfoLoadPacket(DWORD dwVID)
{
	TPacketCGTargetInfoLoad TargetInfoLoadPacket;
	TargetInfoLoadPacket.header = HEADER_CG_TARGET_INFO_LOAD;
	TargetInfoLoadPacket.dwVID = dwVID;

	if (!Send(sizeof(TargetInfoLoadPacket), &TargetInfoLoadPacket))
		return false;

	return SendSequence();
}
#endif

bool CPythonNetworkStream::RecvObserverRemovePacket()
{
	TPacketGCObserverAdd kObserverRemovePacket;
	if (!Recv(sizeof(kObserverRemovePacket), &kObserverRemovePacket))
		return false;

	CPythonMiniMap::Instance().RemoveObserver(
		kObserverRemovePacket.vid
	);

	return true;
}

bool CPythonNetworkStream::RecvObserverMovePacket()
{
	TPacketGCObserverMove kObserverMovePacket;
	if (!Recv(sizeof(kObserverMovePacket), &kObserverMovePacket))
		return false;

	CPythonMiniMap::Instance().MoveObserver(
		kObserverMovePacket.vid,
		kObserverMovePacket.x * 100.0f,
		kObserverMovePacket.y * 100.0f);

	return true;
}

bool CPythonNetworkStream::RecvWarpPacket()
{
	TPacketGCWarp kWarpPacket;

	if (!Recv(sizeof(kWarpPacket), &kWarpPacket))
		return false;

	__DirectEnterMode_Set(m_dwSelectedCharacterIndex);

	CNetworkStream::Connect((DWORD)kWarpPacket.lAddr, kWarpPacket.wPort);

	return true;
}

bool CPythonNetworkStream::RecvDuelStartPacket()
{
	TPacketGCDuelStart kDuelStartPacket;
	if (!Recv(sizeof(kDuelStartPacket), &kDuelStartPacket))
		return false;

	DWORD count = (kDuelStartPacket.wSize - sizeof(kDuelStartPacket)) / sizeof(DWORD);

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();

	CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
	if (!pkInstMain)
	{
		TraceError("CPythonNetworkStream::RecvDuelStartPacket - MainCharacter is NULL");
		return false;
	}
	DWORD dwVIDSrc = pkInstMain->GetVirtualID();
	DWORD dwVIDDest;

	for (DWORD i = 0; i < count; i++)
	{
		Recv(sizeof(dwVIDDest), &dwVIDDest);
		CInstanceBase::InsertDUELKey(dwVIDSrc, dwVIDDest);
	}

	if (count == 0)
		pkInstMain->SetDuelMode(CInstanceBase::DUEL_CANNOTATTACK);
	else
		pkInstMain->SetDuelMode(CInstanceBase::DUEL_START);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));

	rkChrMgr.RefreshAllPCTextTail();

	return true;
}

bool CPythonNetworkStream::RecvPVPPacket()
{
	TPacketGCPVP kPVPPacket;
	if (!Recv(sizeof(kPVPPacket), &kPVPPacket))
		return false;

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();

	switch (kPVPPacket.bMode)
	{
	case PVP_MODE_AGREE:
		rkChrMgr.RemovePVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);

		// 상대가 나(Dst)에게 동의를 구했을때
		if (rkPlayer.IsMainCharacterIndex(kPVPPacket.dwVIDDst))
			rkPlayer.RememberChallengeInstance(kPVPPacket.dwVIDSrc);

		// 상대에게 동의를 구한 동안에는 대결 불능
		if (rkPlayer.IsMainCharacterIndex(kPVPPacket.dwVIDSrc))
			rkPlayer.RememberCantFightInstance(kPVPPacket.dwVIDDst);
		break;
	case PVP_MODE_REVENGE:
	{
		rkChrMgr.RemovePVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);

		DWORD dwKiller = kPVPPacket.dwVIDSrc;
		DWORD dwVictim = kPVPPacket.dwVIDDst;

		// 내(victim)가 상대에게 복수할 수 있을때
		if (rkPlayer.IsMainCharacterIndex(dwVictim))
			rkPlayer.RememberRevengeInstance(dwKiller);

		// 상대(victim)가 나에게 복수하는 동안에는 대결 불능
		if (rkPlayer.IsMainCharacterIndex(dwKiller))
			rkPlayer.RememberCantFightInstance(dwVictim);
		break;
	}

	case PVP_MODE_FIGHT:
		rkChrMgr.InsertPVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDSrc);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDDst);
		break;
	case PVP_MODE_NONE:
		rkChrMgr.RemovePVPKey(kPVPPacket.dwVIDSrc, kPVPPacket.dwVIDDst);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDSrc);
		rkPlayer.ForgetInstance(kPVPPacket.dwVIDDst);
		break;
	}

	// NOTE : PVP 토글시 TargetBoard 를 업데이트 합니다.
	__RefreshTargetBoardByVID(kPVPPacket.dwVIDSrc);
	__RefreshTargetBoardByVID(kPVPPacket.dwVIDDst);

	return true;
}

// DELETEME
/*
void CPythonNetworkStream::__SendWarpPacket()
{
	TPacketCGWarp kWarpPacket;
	kWarpPacket.bHeader=HEADER_GC_WARP;
	if (!Send(sizeof(kWarpPacket), &kWarpPacket))
	{
		return;
	}
}
*/
void CPythonNetworkStream::NotifyHack(const char* c_szMsg)
{
	if (!m_kQue_stHack.empty())
		if (c_szMsg == m_kQue_stHack.back())
			return;

	m_kQue_stHack.push_back(c_szMsg);
}

bool CPythonNetworkStream::__SendHack(const char* c_szMsg)
{
	Tracen(c_szMsg);

	TPacketCGHack kPacketHack;
	kPacketHack.bHeader = HEADER_CG_HACK;
	strncpy(kPacketHack.szBuf, c_szMsg, sizeof(kPacketHack.szBuf) - 1);

	if (!Send(sizeof(kPacketHack), &kPacketHack))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerAddByVIDPacket(DWORD vid)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_ADD_BY_VID;
	if (!Send(sizeof(packet), &packet))
		return false;
	if (!Send(sizeof(vid), &vid))
		return false;
	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerAddByNamePacket(const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_ADD_BY_NAME;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szName[CHARACTER_NAME_MAX_LEN];
	strncpy(szName, c_szName, CHARACTER_NAME_MAX_LEN - 1);
	szName[CHARACTER_NAME_MAX_LEN - 1] = '\0'; // #720: 메신저 이름 관련 버퍼 오버플로우 버그 수정

	if (!Send(sizeof(szName), &szName))
		return false;
	Tracef(" SendMessengerAddByNamePacket : %s\n", c_szName);
	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerRemovePacket(const char* c_szKey, const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_REMOVE;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szKey[CHARACTER_NAME_MAX_LEN];
	strncpy(szKey, c_szKey, CHARACTER_NAME_MAX_LEN - 1);
	if (!Send(sizeof(szKey), &szKey))
		return false;
	__RefreshTargetBoardByName(c_szName);
	return SendSequence();
}

#ifdef ENABLE_MESSENGER_BLOCK
bool CPythonNetworkStream::SendMessengerBlockAddByVIDPacket(DWORD vid)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_VID;
	if (!Send(sizeof(packet), &packet))
		return false;
	if (!Send(sizeof(vid), &vid))
		return false;
	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerBlockAddByNamePacket(const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_BLOCK_ADD_BY_NAME;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szName[CHARACTER_NAME_MAX_LEN];
	strncpy(szName, c_szName, CHARACTER_NAME_MAX_LEN - 1);
	szName[CHARACTER_NAME_MAX_LEN - 1] = '\0';

	if (!Send(sizeof(szName), &szName))
		return false;
	Tracef(" SendMessengerBlockAddByNamePacket : %s\n", c_szName);
	return SendSequence();
}

bool CPythonNetworkStream::SendMessengerBlockRemoveByVIDPacket(const char* c_szKey, const char* c_szName)
{
	TPacketCGMessenger packet;
	packet.header = HEADER_CG_MESSENGER;
	packet.subheader = MESSENGER_SUBHEADER_CG_BLOCK_REMOVE;
	if (!Send(sizeof(packet), &packet))
		return false;
	char szKey[CHARACTER_NAME_MAX_LEN];
	strncpy(szKey, c_szKey, CHARACTER_NAME_MAX_LEN - 1);
	if (!Send(sizeof(szKey), &szKey))
		return false;
	__RefreshTargetBoardByName(c_szName);
	return SendSequence();
}
#endif

bool CPythonNetworkStream::SendCharacterStatePacket(const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
	NANOBEGIN
		if (!__CanActMainInstance())
			return true;

	if (fDstRot < 0.0f)
		fDstRot = 360 + fDstRot;
	else if (fDstRot > 360.0f)
		fDstRot = fmodf(fDstRot, 360.0f);

	// TODO: 나중에 패킷이름을 바꾸자
	TPacketCGMove kStatePacket;
	kStatePacket.bHeader = HEADER_CG_CHARACTER_MOVE;
	kStatePacket.bFunc = eFunc;
	kStatePacket.bArg = uArg;
	kStatePacket.bRot = fDstRot / 5.0f;
	kStatePacket.lX = long(c_rkPPosDst.x);
	kStatePacket.lY = long(c_rkPPosDst.y);
	kStatePacket.dwTime = ELTimer_GetServerMSec();

	assert(kStatePacket.lX >= 0 && kStatePacket.lX < 204800);

	__LocalPositionToGlobalPosition(kStatePacket.lX, kStatePacket.lY);

	if (!Send(sizeof(kStatePacket), &kStatePacket))
	{
		Tracenf("CPythonNetworkStream::SendCharacterStatePacket(dwCmdTime=%u, fDstPos=(%f, %f), fDstRot=%f, eFunc=%d uArg=%d) - PACKET SEND ERROR",
			kStatePacket.dwTime,
			float(kStatePacket.lX),
			float(kStatePacket.lY),
			fDstRot,
			kStatePacket.bFunc,
			kStatePacket.bArg);
		return false;
	}
	NANOEND
		return SendSequence();
}

// NOTE : SlotIndex는 임시
bool CPythonNetworkStream::SendUseSkillPacket(DWORD dwSkillIndex, DWORD dwTargetVID)
{
	TPacketCGUseSkill UseSkillPacket;
	UseSkillPacket.bHeader = HEADER_CG_USE_SKILL;
	UseSkillPacket.dwVnum = dwSkillIndex;
	UseSkillPacket.dwTargetVID = dwTargetVID;

	if (!Send(sizeof(TPacketCGUseSkill), &UseSkillPacket))
	{
		Tracen("CPythonNetworkStream::SendUseSkillPacket - SEND PACKET ERROR");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendChatPacket(const char* c_szChat, BYTE byType)
{
	if (strlen(c_szChat) == 0)
		return true;

	if (strlen(c_szChat) >= 512)
		return true;

	if (c_szChat[0] == '/')
	{
		if (1 == strlen(c_szChat))
		{
			if (!m_strLastCommand.empty())
				c_szChat = m_strLastCommand.c_str();
		}
		else
		{
			m_strLastCommand = c_szChat;
		}
	}

	if (ClientCommand(c_szChat))
		return true;

	int iTextLen = strlen(c_szChat) + 1;
	TPacketCGChat ChatPacket;
	ChatPacket.header = HEADER_CG_CHAT;
	ChatPacket.length = sizeof(ChatPacket) + iTextLen;
	ChatPacket.type = byType;

	if (!Send(sizeof(ChatPacket), &ChatPacket))
		return false;

	if (!Send(iTextLen, c_szChat))
		return false;

	return SendSequence();
}

//////////////////////////////////////////////////////////////////////////
// Emoticon
void CPythonNetworkStream::RegisterEmoticonString(const char* pcEmoticonString)
{
	if (m_EmoticonStringVector.size() >= CInstanceBase::EMOTICON_NUM)
	{
		TraceError("Can't register emoticon string... vector is full (size:%d)", m_EmoticonStringVector.size());
		return;
	}
	m_EmoticonStringVector.push_back(pcEmoticonString);
}

bool CPythonNetworkStream::ParseEmoticon(const char* pChatMsg, DWORD* pdwEmoticon)
{
	for (DWORD dwEmoticonIndex = 0; dwEmoticonIndex < m_EmoticonStringVector.size(); ++dwEmoticonIndex)
	{
		if (strlen(pChatMsg) > m_EmoticonStringVector[dwEmoticonIndex].size())
			continue;

		const char* pcFind = strstr(pChatMsg, m_EmoticonStringVector[dwEmoticonIndex].c_str());

		if (pcFind != pChatMsg)
			continue;

		*pdwEmoticon = dwEmoticonIndex;

		return true;
	}

	return false;
}
// Emoticon
//////////////////////////////////////////////////////////////////////////

void CPythonNetworkStream::__ConvertEmpireText(DWORD dwEmpireID, char* szText)
{
	if (dwEmpireID < 1 || dwEmpireID>3)
		return;

	UINT uHanPos;

	STextConvertTable& rkTextConvTable = m_aTextConvTable[dwEmpireID - 1];

	BYTE* pbText = (BYTE*)szText;
	while (*pbText)
	{
		if (*pbText & 0x80)
		{
			if (pbText[0] >= 0xb0 && pbText[0] <= 0xc8 && pbText[1] >= 0xa1 && pbText[1] <= 0xfe)
			{
				uHanPos = (pbText[0] - 0xb0) * (0xfe - 0xa1 + 1) + (pbText[1] - 0xa1);
				pbText[0] = rkTextConvTable.aacHan[uHanPos][0];
				pbText[1] = rkTextConvTable.aacHan[uHanPos][1];
			}
			pbText += 2;
		}
		else
		{
			if (*pbText >= 'a' && *pbText <= 'z')
			{
				*pbText = rkTextConvTable.acLower[*pbText - 'a'];
			}
			else if (*pbText >= 'A' && *pbText <= 'Z')
			{
				*pbText = rkTextConvTable.acUpper[*pbText - 'A'];
			}
			pbText++;
		}
	}
}

static std::string SBS(const std::string& c_str, const std::string& c_strStartDelim, const std::string& c_strEndDelim)
{
	unsigned u_strFirstDelimPos = c_str.find(c_strStartDelim);
	unsigned u_strEndPosFirstDelim = u_strFirstDelimPos + c_strStartDelim.length();
	unsigned u_strLastDelimPos = c_str.find_first_of(c_strEndDelim, u_strEndPosFirstDelim);
	return c_str.substr(u_strEndPosFirstDelim, u_strLastDelimPos - u_strEndPosFirstDelim);
}

const char* FilterChat(const char* c_pszBuffer)
{
	std::string strChatBuf = c_pszBuffer;

	std::string strHyperItemLink = "|cffffc700|H";
	std::string strNHyperItemLink = "|cfff1e6c0|H";

	/// HyperLink w/ Attr
	std::string::size_type xPos1 = 0;
	while ((xPos1 = strChatBuf.find(strHyperItemLink, xPos1)) != std::string::npos)
	{
		xPos1 += strHyperItemLink.length();

		std::string strElement = strChatBuf.substr(xPos1, strChatBuf.length());

		if (strElement.find("[") != std::string::npos && strElement.find("]") != std::string::npos)
		{
			std::string strItemLink = SBS(strElement, "", "|h"); // ITEM
			std::string strItemVnum = SBS(strElement, "item:", ":");
			DWORD dwItemVnum = htoi(strItemVnum.c_str()); // VNUM
			std::string strItemName = SBS(strElement, "[", "]").c_str(); // ITEM NAME

			CItemManager::Instance().SelectItemData(dwItemVnum);
			CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

			const char* c_pszItemName = pItemData->GetName();

			{
				char szSearchBuf[1024 + 1];
				snprintf(szSearchBuf, sizeof(szSearchBuf), "[%s]", strItemName.c_str());
				std::string strReplace(szSearchBuf);

				char szReplaceBuf[1024 + 1];
				snprintf(szReplaceBuf, sizeof(szReplaceBuf), "[%s]", c_pszItemName); // Replace VNUM w/ NAME

				std::string::size_type zReplacePos = strChatBuf.find(strReplace);
				strChatBuf.replace(zReplacePos, strReplace.length(), szReplaceBuf);
			}
		}

	}

	// Normal HyperLink
	std::string::size_type xPos2 = 0;
	while ((xPos2 = strChatBuf.find(strNHyperItemLink, xPos2)) != std::string::npos)
	{
		xPos2 += strNHyperItemLink.length();

		std::string strElement = strChatBuf.substr(xPos2, strChatBuf.length());

		if (strElement.find("[") != std::string::npos && strElement.find("]") != std::string::npos)
		{
			std::string strItemLink = SBS(strElement, "", "|h"); // ITEM
			std::string strItemVnum = SBS(strElement, "item:", ":");
			int32_t dwItemVnum = htoi(strItemVnum.c_str()); // VNUM
			std::string strItemName = SBS(strElement, "[", "]").c_str(); // ITEM NAME

			CItemManager::Instance().SelectItemData(dwItemVnum);
			CItemData* pItemData = CItemManager::Instance().GetSelectedItemDataPointer();

			const char* c_pszItemName = pItemData->GetName();

			{
				char szSearchBuf[1024 + 1];
				snprintf(szSearchBuf, sizeof(szSearchBuf), "[%s]", strItemName.c_str());
				std::string strReplace(szSearchBuf);

				char szReplaceBuf[1024 + 1];
				snprintf(szReplaceBuf, sizeof(szReplaceBuf), "[%s]", c_pszItemName); // Replace VNUM w/ NAME

				std::string::size_type zReplacePos = strChatBuf.find(strReplace);
				strChatBuf.replace(zReplacePos, strReplace.length(), szReplaceBuf);
			}
		}
	}

	static char szTmpBuf[1024 + 1] = {};
	const char* c_szpFilterBuf = strChatBuf.c_str();
	snprintf(szTmpBuf, sizeof(szTmpBuf), "%s", c_szpFilterBuf);

	return szTmpBuf;
}

bool CPythonNetworkStream::RecvChatPacket()
{
	TPacketGCChat kChat;
	char buf[1024 + 1];
	char line[1024 + 1];

	if (!Recv(sizeof(kChat), &kChat))
		return false;

	UINT uChatSize = kChat.size - sizeof(kChat);

	if (!Recv(uChatSize, buf))
		return false;

	buf[uChatSize] = '\0';

	// 유럽 아랍 버전 처리
	// "이름: 내용" 입력을 "내용: 이름" 순서로 출력하기 위해 탭(0x08)을 넣음
	// 탭을 아랍어 기호로 처리해 (영어1) : (영어2) 로 입력되어도 (영어2) : (영어1) 로 출력하게 만든다
	if (LocaleService_IsEUROPE() && GetDefaultCodePage() == 1256)
	{
		char* p = strchr(buf, ':');
		if (p && p[1] == ' ')
			p[1] = 0x08;
	}

	if (kChat.type >= CHAT_TYPE_MAX_NUM)
		return true;

	if (CHAT_TYPE_COMMAND == kChat.type)
	{
		ServerCommand(buf);
		return true;
	}
#ifdef ENABLE_12ZI
	else if (CHAT_TYPE_MISSION == kChat.type)
	{
		if (uChatSize)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SetMissionMessage", Py_BuildValue("(s)", buf));
		else
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CleanMissionMessage", Py_BuildValue("()"));

		return true;
	}
	else if (CHAT_TYPE_SUB_MISSION == kChat.type)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SetSubMissionMessage", Py_BuildValue("(s)", buf));
		return true;
	}
#endif

	if (kChat.dwVID != 0)
	{
		CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
		CInstanceBase* pkInstChatter = rkChrMgr.GetInstancePtr(kChat.dwVID);

		if (NULL == pkInstChatter)
			return true;

		switch (kChat.type)
		{
		case CHAT_TYPE_TALKING: /* 그냥 채팅 */
		case CHAT_TYPE_PARTY: /* 파티말 */
		case CHAT_TYPE_GUILD: /* 길드말 */
		case CHAT_TYPE_SHOUT: /* 외치기 */
		case CHAT_TYPE_WHISPER: // 서버와는 연동되지 않는 Only Client Enum
		{
			char* p = strchr(buf, ':');

			if (p)
				p += 2;
			else
				p = buf;

			DWORD dwEmoticon;

#if defined(ENABLE_MESSENGER_BLOCK)
			if (CPythonMessenger::Instance().IsBlockFriendByName(pkInstChatter->GetNameString()))
				return true;
#endif

			if (ParseEmoticon(p, &dwEmoticon))
			{
				pkInstChatter->SetEmoticon(dwEmoticon);
				return true;
			}
			else
			{
				if (gs_bEmpireLanuageEnable)
				{
					CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
					if (pkInstMain)
						if (!pkInstMain->IsSameEmpire(*pkInstChatter))
							__ConvertEmpireText(pkInstChatter->GetEmpireID(), p);
				}

				if (m_isEnableChatInsultFilter)
				{
					if (false == pkInstChatter->IsNPC() && false == pkInstChatter->IsEnemy())
					{
						__FilterInsult(p, strlen(p));
					}
				}

				if (pkInstChatter->IsNPC() || pkInstChatter->IsEnemy())
				{
					std::string strChat(buf, sizeof(buf));

					unsigned uFirstDelim = strChat.find("[");
					unsigned uLastDelim = strChat.find("]");

					std::string strFilter = strChat.substr(uFirstDelim + 1, uLastDelim - uFirstDelim);
					DWORD dwVnum = atoi(strFilter.c_str());

					const char* c_szMobChat = GetLocaleQuestVnum(dwVnum);

					_snprintf(line, sizeof(line), "%s", c_szMobChat);
					break; return true;
				}

				_snprintf(line, sizeof(line), "%s", p);
			}
		}
		break;

		case CHAT_TYPE_COMMAND: /* 명령 */
		case CHAT_TYPE_INFO: /* 정보 (아이템을 집었다, 경험치를 얻었다. 등) */
		case CHAT_TYPE_NOTICE: /* 공지사항 */
		case CHAT_TYPE_BIG_NOTICE:
		case CHAT_TYPE_BIG_CONTROL_NOTICE:
#if defined(ENABLE_DICE_SYSTEM)
		case CHAT_TYPE_DICE_INFO:
#endif
#if defined(ENABLE_CHATTING_WINDOW_RENEWAL)
		case CHAT_TYPE_EXP_INFO:
		case CHAT_TYPE_ITEM_INFO:
		case CHAT_TYPE_MONEY_INFO:
#endif
		case CHAT_TYPE_MAX_NUM:
		default:
			_snprintf(line, sizeof(line), "%s", buf);
			break;
		}

		if (CHAT_TYPE_SHOUT != kChat.type)
		{
			CPythonTextTail::Instance().RegisterChatTail(kChat.dwVID, FilterChat(line));
		}

		if (pkInstChatter->IsPC())
		{
#if defined(ENABLE_MESSENGER_BLOCK)
			if (CPythonMessenger::Instance().IsBlockFriendByName(pkInstChatter->GetNameString()))
				return true;
#endif

			CPythonChat::Instance().AppendChat(kChat.type, FilterChat(buf));
		}
	}
	else
	{
		if (CHAT_TYPE_NOTICE == kChat.type)
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SetTipMessage", Py_BuildValue("(s)", buf));
		}
		else if (CHAT_TYPE_BIG_NOTICE == kChat.type)
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_SetBigMessage", Py_BuildValue("(s)", buf));
		}
		else if (CHAT_TYPE_SHOUT == kChat.type)
		{
			char* p = strchr(buf, ':');

			if (p)
			{
				if (m_isEnableChatInsultFilter)
					__FilterInsult(p, strlen(p));
			}
		}

		CPythonChat::Instance().AppendChat(kChat.type, FilterChat(buf));
	}

	return true;
}

bool CPythonNetworkStream::RecvWhisperPacket()
{
	TPacketGCWhisper whisperPacket;
	char buf[512 + 1];

	if (!Recv(sizeof(whisperPacket), &whisperPacket))
		return false;

	assert(whisperPacket.wSize - sizeof(whisperPacket) < 512);

	if (!Recv(whisperPacket.wSize - sizeof(whisperPacket), &buf))
		return false;

	buf[whisperPacket.wSize - sizeof(whisperPacket)] = '\0';

	static char line[256];
	if (CPythonChat::WHISPER_TYPE_CHAT == whisperPacket.bType || CPythonChat::WHISPER_TYPE_GM == whisperPacket.bType)
	{
		_snprintf(line, sizeof(line), "%s : %s", whisperPacket.szNameFrom, FilterChat(buf));
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisper", Py_BuildValue("(iss)", (int)whisperPacket.bType, whisperPacket.szNameFrom, line));
	}
	else if (CPythonChat::WHISPER_TYPE_SYSTEM == whisperPacket.bType || CPythonChat::WHISPER_TYPE_ERROR == whisperPacket.bType)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisperSystemMessage", Py_BuildValue("(iss)", (int)whisperPacket.bType, whisperPacket.szNameFrom, buf));
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnRecvWhisperError", Py_BuildValue("(iss)", (int)whisperPacket.bType, whisperPacket.szNameFrom, buf));
	}

	return true;
}

bool CPythonNetworkStream::SendWhisperPacket(const char* name, const char* c_szChat)
{
	if (strlen(c_szChat) >= 255)
		return true;

	int iTextLen = strlen(c_szChat) + 1;
	TPacketCGWhisper WhisperPacket;
	WhisperPacket.bHeader = HEADER_CG_WHISPER;
	WhisperPacket.wSize = sizeof(WhisperPacket) + iTextLen;

	strncpy(WhisperPacket.szNameTo, name, sizeof(WhisperPacket.szNameTo) - 1);

	if (!Send(sizeof(WhisperPacket), &WhisperPacket))
		return false;

	if (!Send(iTextLen, c_szChat))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::RecvPointChange()
{
	TPacketGCPointChange PointChange;

	if (!Recv(sizeof(TPacketGCPointChange), &PointChange))
	{
		Tracen("Recv Point Change Packet Error");
		return false;
	}

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.ShowPointEffect(PointChange.Type, PointChange.dwVID);

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();

	// 자신의 Point가 변경되었을 경우..
	if (pInstance)
	{
		if (PointChange.dwVID == pInstance->GetVirtualID())
		{
			CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
			rkPlayer.SetStatus(PointChange.Type, PointChange.value);

			switch (PointChange.Type)
			{
			case POINT_STAT_RESET_COUNT:
				__RefreshStatus();
				break;
#ifdef ENABLE_IMPROVED_LOGOUT_POINTS
			case POINT_PLAYTIME:
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].dwPlayMinutes = PointChange.value;
				break;
#endif
			case POINT_LEVEL:
#ifdef ENABLE_IMPROVED_LOGOUT_POINTS
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byLevel = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
				break;
#endif
			case POINT_ST:
#ifdef ENABLE_IMPROVED_LOGOUT_POINTS
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byST = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
				break;
#endif
			case POINT_DX:
#ifdef ENABLE_IMPROVED_LOGOUT_POINTS
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byDX = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
				break;
#endif
			case POINT_HT:
#ifdef ENABLE_IMPROVED_LOGOUT_POINTS
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byHT = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
				break;
#endif
			case POINT_IQ:
#ifdef ENABLE_IMPROVED_LOGOUT_POINTS
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byIQ = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
				break;
#else
				__RefreshStatus();
				__RefreshSkillWindow();
				break;
#endif

#if defined(ENABLE_CONQUEROR_LEVEL)
			case POINT_CONQUEROR_LEVEL:
	#ifdef ENABLE_IMPROVED_LOGOUT_POINTS
			{
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].byConquerorLevel = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
			} break;
#	endif
			case POINT_SUNGMA_STR:
#	ifdef ENABLE_IMPROVED_LOGOUT_POINTS
			{
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].bySungmaStr = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
			} break;
#	endif
			case POINT_SUNGMA_HP:
#	ifdef ENABLE_IMPROVED_LOGOUT_POINTS
			{
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].bySungmaHp = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
			} break;
#	endif
			case POINT_SUNGMA_MOVE:
#	ifdef ENABLE_IMPROVED_LOGOUT_POINTS
			{
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].bySungmaMove = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
			} break;
#	endif
			case POINT_SUNGMA_IMMUNE:
#	ifdef ENABLE_IMPROVED_LOGOUT_POINTS
			{
				m_akSimplePlayerInfo[m_dwSelectedCharacterIndex].bySungmaImmune = PointChange.value;
				__RefreshStatus();
				__RefreshSkillWindow();
			} break;
#	else
			{
				__RefreshStatus();
				__RefreshSkillWindow();
			} break;
#	endif
#endif

			case POINT_SKILL:
			case POINT_SUB_SKILL:
			case POINT_HORSE_SKILL:
				__RefreshSkillWindow();
				break;
			case POINT_ENERGY:
				if (PointChange.value == 0)
				{
					rkPlayer.SetStatus(POINT_ENERGY_END_TIME, 0);
				}
				__RefreshStatus();
				break;
			default:
				__RefreshStatus();
				break;
			}

			if (POINT_GOLD == PointChange.Type)
			{
				if (PointChange.amount > 0)
				{
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnPickMoney", Py_BuildValue("(i)", PointChange.amount));
				}
			}

#if defined(ENABLE_CHEQUE_SYSTEM)
			if (POINT_CHEQUE == PointChange.Type)
			{
				if (PointChange.amount > 0)
				{
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnPickCheque", Py_BuildValue("(i)", PointChange.amount));
				}
			}
#endif

#if defined(ENABLE_GEM_SYSTEM)
			if (POINT_GEM == PointChange.Type)
			{
				if (PointChange.amount > 0)
				{
					PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnPickGem", Py_BuildValue("(i)", PointChange.amount));
				}
			}
#endif
		}
		else
		{
			pInstance = CPythonCharacterManager::Instance().GetInstancePtr(PointChange.dwVID);
			if (pInstance && PointChange.Type == POINT_LEVEL)
			{
				pInstance->SetLevel(PointChange.value);
				pInstance->UpdateTextTailLevel(PointChange.value);
			}
		}
	}

	return true;
}

bool CPythonNetworkStream::RecvStunPacket()
{
	TPacketGCStun StunPacket;

	if (!Recv(sizeof(StunPacket), &StunPacket))
	{
		Tracen("CPythonNetworkStream::RecvStunPacket Error");
		return false;
	}

	//Tracef("RecvStunPacket %d\n", StunPacket.vid);

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkInstSel = rkChrMgr.GetInstancePtr(StunPacket.vid);

	if (pkInstSel)
	{
		if (CPythonCharacterManager::Instance().GetMainInstancePtr() == pkInstSel)
			pkInstSel->Die();
		else
			pkInstSel->Stun();
	}

	return true;
}

bool CPythonNetworkStream::RecvDeadPacket()
{
	TPacketGCDead DeadPacket;
	if (!Recv(sizeof(DeadPacket), &DeadPacket))
	{
		Tracen("CPythonNetworkStream::RecvDeadPacket Error");
		return false;
	}

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkChrInstSel = rkChrMgr.GetInstancePtr(DeadPacket.vid);
	if (pkChrInstSel)
	{
		CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();
		if (pkInstMain == pkChrInstSel)
		{
			Tracenf("주인공 사망");
			if (false == pkInstMain->GetDuelMode())
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnGameOver", Py_BuildValue("()"));
			}
			CPythonPlayer::Instance().NotifyDeadMainCharacter();
#if defined(ENABLE_SKILL_COOLTIME_UPDATE)
			CPythonPlayer::Instance().ResetSkillCoolTimes();
#endif
		}

		pkChrInstSel->Die();
	}

	return true;
}

bool CPythonNetworkStream::SendCharacterPositionPacket(BYTE iPosition)
{
	TPacketCGPosition PositionPacket;

	PositionPacket.header = HEADER_CG_CHARACTER_POSITION;
	PositionPacket.position = iPosition;

	if (!Send(sizeof(TPacketCGPosition), &PositionPacket))
	{
		Tracen("Send Character Position Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendOnClickPacket(DWORD vid)
{
	TPacketCGOnClick OnClickPacket;
	OnClickPacket.header = HEADER_CG_ON_CLICK;
	OnClickPacket.vid = vid;

	if (!Send(sizeof(OnClickPacket), &OnClickPacket))
	{
		Tracen("Send On_Click Packet Error");
		return false;
	}

	Tracef("SendOnClickPacket\n");
	return SendSequence();
}

bool CPythonNetworkStream::RecvCharacterPositionPacket()
{
	TPacketGCPosition PositionPacket;

	if (!Recv(sizeof(TPacketGCPosition), &PositionPacket))
		return false;

	CInstanceBase* pChrInstance = CPythonCharacterManager::Instance().GetInstancePtr(PositionPacket.vid);

	if (!pChrInstance)
		return true;

	//pChrInstance->UpdatePosition(PositionPacket.position);

	return true;
}

bool CPythonNetworkStream::RecvMotionPacket()
{
	TPacketGCMotion MotionPacket;

	if (!Recv(sizeof(TPacketGCMotion), &MotionPacket))
		return false;

	CInstanceBase* pMainInstance = CPythonCharacterManager::Instance().GetInstancePtr(MotionPacket.vid);
	CInstanceBase* pVictimInstance = NULL;

	if (0 != MotionPacket.victim_vid)
		pVictimInstance = CPythonCharacterManager::Instance().GetInstancePtr(MotionPacket.victim_vid);

	if (!pMainInstance)
		return false;

	return true;
}

bool CPythonNetworkStream::RecvShopPacket()
{
	std::vector<char> vecBuffer;
	vecBuffer.clear();

	TPacketGCShop packet_shop;
	if (!Recv(sizeof(packet_shop), &packet_shop))
		return false;

	int iSize = packet_shop.size - sizeof(packet_shop);
	if (iSize > 0)
	{
		vecBuffer.resize(iSize);
		if (!Recv(iSize, &vecBuffer[0]))
			return false;
	}

	switch (packet_shop.subheader)
	{
	case SHOP_SUBHEADER_GC_START:
	{
		CPythonShop::Instance().Clear();

		DWORD dwVID = *(DWORD*)&vecBuffer[0];

		TPacketGCShopStart* pShopStartPacket = (TPacketGCShopStart*)&vecBuffer[4];
#if defined(ENABLE_MYSHOP_DECO)
		BYTE shop_tab_count = pShopStartPacket->shop_tab_count;
		CPythonShop::instance().SetTabCount(shop_tab_count);
#endif
		for (BYTE iItemIndex = 0; iItemIndex < SHOP_HOST_ITEM_MAX_NUM; ++iItemIndex)
		{
			CPythonShop::Instance().SetItemData(iItemIndex, pShopStartPacket->items[iItemIndex]);
		}
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartShop", Py_BuildValue("(i)", dwVID));
	}
	break;

	case SHOP_SUBHEADER_GC_START_EX:
	{
		CPythonShop::Instance().Clear();

		TPacketGCShopStartEx* pShopStartPacket = (TPacketGCShopStartEx*)&vecBuffer[0];
		size_t read_point = sizeof(TPacketGCShopStartEx);

		DWORD dwVID = pShopStartPacket->owner_vid;
		BYTE shop_tab_count = pShopStartPacket->shop_tab_count;

		CPythonShop::instance().SetTabCount(shop_tab_count);

		for (BYTE i = 0; i < shop_tab_count; i++)
		{
			TPacketGCShopStartEx::TSubPacketShopTab* pPackTab = (TPacketGCShopStartEx::TSubPacketShopTab*)&vecBuffer[read_point];
			read_point += sizeof(TPacketGCShopStartEx::TSubPacketShopTab);

			CPythonShop::instance().SetTabCoinType(i, pPackTab->coin_type);
			CPythonShop::instance().SetTabName(i, pPackTab->name);

			struct packet_shop_item* item = &pPackTab->items[0];

			for (BYTE j = 0; j < SHOP_HOST_ITEM_MAX_NUM; j++)
			{
				TShopItemData* itemData = (item + j);
				CPythonShop::Instance().SetItemData(i, j, *itemData);
			}
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartShop", Py_BuildValue("(i)", dwVID));
	}
	break;

	case SHOP_SUBHEADER_GC_END:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "EndShop", Py_BuildValue("()"));
		break;

	case SHOP_SUBHEADER_GC_UPDATE_ITEM:
	{
		TPacketGCShopUpdateItem* pShopUpdateItemPacket = (TPacketGCShopUpdateItem*)&vecBuffer[0];
		CPythonShop::Instance().SetItemData(pShopUpdateItemPacket->pos, pShopUpdateItemPacket->item);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshShop", Py_BuildValue("()"));
	}
	break;

	case SHOP_SUBHEADER_GC_UPDATE_PRICE:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetShopSellingPrice", Py_BuildValue("(i)", *(int*)&vecBuffer[0]));
		break;

	case SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY"));
		break;

	case SHOP_SUBHEADER_GC_NOT_ENOUGH_MONEY_EX:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY_EX"));
		break;

#if defined(ENABLE_CHEQUE_SYSTEM)
	case SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY"));
		break;

	case SHOP_SUBHEADER_GC_NOT_ENOUGH_CHEQUE_MONEY:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_MONEY"));
		break;
#endif

	case SHOP_SUBHEADER_GC_SOLDOUT:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "SOLDOUT"));
		break;

	case SHOP_SUBHEADER_GC_INVENTORY_FULL:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "INVENTORY_FULL"));
		break;

	case SHOP_SUBHEADER_GC_INVALID_POS:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "INVALID_POS"));
		break;

#ifdef ENABLE_SHOPEX_RENEWAL
	case SHOP_SUBHEADER_GC_NOT_ENOUGH_ITEM:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_ITEM"));
		break;

	case SHOP_SUBHEADER_GC_NOT_ENOUGH_EXP:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnShopError", Py_BuildValue("(s)", "NOT_ENOUGH_EXP"));
		break;
#endif

	default:
		TraceError("CPythonNetworkStream::RecvShopPacket: Unknown subheader\n");
		break;
	}

	return true;
}

bool CPythonNetworkStream::RecvExchangePacket()
{
	TPacketGCExchange exchange_packet;

	if (!Recv(sizeof(exchange_packet), &exchange_packet))
		return false;

	switch (exchange_packet.subheader)
	{
	case EXCHANGE_SUBHEADER_GC_START:
		CPythonExchange::Instance().Clear();
		CPythonExchange::Instance().Start();
		CPythonExchange::Instance().SetSelfName(CPythonPlayer::Instance().GetName());

		{
			CInstanceBase* pCharacterInstance = CPythonCharacterManager::Instance().GetInstancePtr(exchange_packet.arg1);

			if (pCharacterInstance)
			{
				CPythonExchange::Instance().SetTargetName(pCharacterInstance->GetNameString());
				CPythonExchange::Instance().SetLevelToTarget(pCharacterInstance->GetLevel());
			}
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartExchange", Py_BuildValue("()"));
		break;

	case EXCHANGE_SUBHEADER_GC_ITEM_ADD:
		if (exchange_packet.is_me)
		{
			int iSlotIndex = exchange_packet.arg2.cell;
			CPythonExchange::Instance().SetItemToSelf(iSlotIndex, exchange_packet.arg1, (WORD)exchange_packet.arg3);
			for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
				CPythonExchange::Instance().SetItemMetinSocketToSelf(iSlotIndex, i, exchange_packet.alValues[i]);
#if defined(ENABLE_APPLY_RANDOM)
			for (int j = 0; j < ITEM_APPLY_RANDOM_SLOT_MAX_NUM; ++j)
				CPythonExchange::Instance().SetItemApplyRandomToSelf(iSlotIndex, j, exchange_packet.aApplyRandom[j].bType, exchange_packet.aApplyRandom[j].sValue);
#endif
			for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
				CPythonExchange::Instance().SetItemAttributeToSelf(iSlotIndex, j, exchange_packet.aAttr[j].bType, exchange_packet.aAttr[j].sValue);

#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
			CPythonExchange::Instance().SetItemChangeLookVnumToSelf(iSlotIndex, exchange_packet.dwTransmutationVnum);
#endif
#ifdef WJ_ENABLE_TRADABLE_ICON
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AddExchangeItemSlotIndex", Py_BuildValue("(i)", exchange_packet.arg4.cell));
#endif
		}
		else
		{
			int iSlotIndex = exchange_packet.arg2.cell;
			CPythonExchange::Instance().SetItemToTarget(iSlotIndex, exchange_packet.arg1, (WORD)exchange_packet.arg3);
			for (int i = 0; i < ITEM_SOCKET_SLOT_MAX_NUM; ++i)
				CPythonExchange::Instance().SetItemMetinSocketToTarget(iSlotIndex, i, exchange_packet.alValues[i]);
#if defined(ENABLE_APPLY_RANDOM)
			for (int j = 0; j < ITEM_APPLY_RANDOM_SLOT_MAX_NUM; ++j)
				CPythonExchange::Instance().SetItemApplyRandomToTarget(iSlotIndex, j, exchange_packet.aApplyRandom[j].bType, exchange_packet.aApplyRandom[j].sValue);
#endif
			for (int j = 0; j < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++j)
				CPythonExchange::Instance().SetItemAttributeToTarget(iSlotIndex, j, exchange_packet.aAttr[j].bType, exchange_packet.aAttr[j].sValue);

#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
			CPythonExchange::Instance().SetItemChangeLookVnumToTarget(iSlotIndex, exchange_packet.dwTransmutationVnum);
#endif
		}

		__RefreshExchangeWindow();
		__RefreshInventoryWindow();
		break;

	case EXCHANGE_SUBHEADER_GC_ITEM_DEL:
		if (exchange_packet.is_me)
		{
			CPythonExchange::Instance().DelItemOfSelf((WORD)exchange_packet.arg1);
		}
		else
		{
			CPythonExchange::Instance().DelItemOfTarget((WORD)exchange_packet.arg1);
		}
		__RefreshExchangeWindow();
		__RefreshInventoryWindow();
		break;

	case EXCHANGE_SUBHEADER_GC_ELK_ADD:
		if (exchange_packet.is_me)
			CPythonExchange::Instance().SetElkToSelf(exchange_packet.arg1);
		else
			CPythonExchange::Instance().SetElkToTarget(exchange_packet.arg1);

		__RefreshExchangeWindow();
		break;

#if defined(ENABLE_CHEQUE_SYSTEM)
	case EXCHANGE_SUBHEADER_GC_CHEQUE_ADD:
		if (exchange_packet.is_me)
			CPythonExchange::Instance().SetChequeToSelf(exchange_packet.arg1);
		else
			CPythonExchange::Instance().SetChequeToTarget(exchange_packet.arg1);

		__RefreshExchangeWindow();
		break;
#endif

	case EXCHANGE_SUBHEADER_GC_ACCEPT:
		if (exchange_packet.is_me)
		{
			CPythonExchange::Instance().SetAcceptToSelf((BYTE)exchange_packet.arg1);
		}
		else
		{
			CPythonExchange::Instance().SetAcceptToTarget((BYTE)exchange_packet.arg1);
		}
		__RefreshExchangeWindow();
		break;

	case EXCHANGE_SUBHEADER_GC_END:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "EndExchange", Py_BuildValue("()"));
		__RefreshInventoryWindow();
		CPythonExchange::Instance().End();
		break;

	case EXCHANGE_SUBHEADER_GC_ALREADY:
		Tracef("trade_already");
		break;

	case EXCHANGE_SUBHEADER_GC_LESS_ELK:
		Tracef("trade_less_elk");
		break;
	};

	return true;
}

bool CPythonNetworkStream::RecvQuestInfoPacket()
{
	TPacketGCQuestInfo QuestInfo;

	if (!Peek(sizeof(TPacketGCQuestInfo), &QuestInfo))
	{
		Tracen("Recv Quest Info Packet Error #1");
		return false;
	}

	if (!Peek(QuestInfo.size))
	{
		Tracen("Recv Quest Info Packet Error #2");
		return false;
	}

	Recv(sizeof(TPacketGCQuestInfo));

	const BYTE& c_rFlag = QuestInfo.flag;

	enum
	{
		QUEST_PACKET_TYPE_NONE,
		QUEST_PACKET_TYPE_BEGIN,
		QUEST_PACKET_TYPE_UPDATE,
		QUEST_PACKET_TYPE_END,
	};

	enum EQuestLen
	{
#if defined(ENABLE_QUEST_RENEWAL)
		QUEST_TITLE_NAME_LEN = 50,
#else
		QUEST_TITLE_NAME_LEN = 30,
#endif
		QUEST_CLOCK_NAME_LEN = 16,
		QUEST_COUNTER_NAME_LEN = 16,
		QUEST_ICON_FILE_NAME_LEN = 24,
	};

	BYTE byQuestPacketType = QUEST_PACKET_TYPE_NONE;

	if (0 != (c_rFlag & QUEST_SEND_IS_BEGIN))
	{
		BYTE isBegin;
		if (!Recv(sizeof(isBegin), &isBegin))
			return false;

		if (isBegin)
			byQuestPacketType = QUEST_PACKET_TYPE_BEGIN;
		else
			byQuestPacketType = QUEST_PACKET_TYPE_END;
	}
	else
	{
		byQuestPacketType = QUEST_PACKET_TYPE_UPDATE;
	}

	// Recv Data Start
	char szTitle[QUEST_TITLE_NAME_LEN + 1] = "";
	char szClockName[QUEST_CLOCK_NAME_LEN + 1] = "";
	int iClockValue = 0;
	char szCounterName[QUEST_COUNTER_NAME_LEN + 1] = "";
	int iCounterValue = 0;
	char szIconFileName[QUEST_ICON_FILE_NAME_LEN + 1] = "";

	if (0 != (c_rFlag & QUEST_SEND_TITLE))
	{
		if (!Recv(sizeof(szTitle), &szTitle))
			return false;

		szTitle[QUEST_TITLE_NAME_LEN] = '\0';
	}
	if (0 != (c_rFlag & QUEST_SEND_CLOCK_NAME))
	{
		if (!Recv(sizeof(szClockName), &szClockName))
			return false;

		szClockName[QUEST_CLOCK_NAME_LEN] = '\0';
	}
	if (0 != (c_rFlag & QUEST_SEND_CLOCK_VALUE))
	{
		if (!Recv(sizeof(iClockValue), &iClockValue))
			return false;
	}
	if (0 != (c_rFlag & QUEST_SEND_COUNTER_NAME))
	{
		if (!Recv(sizeof(szCounterName), &szCounterName))
			return false;

		szCounterName[QUEST_COUNTER_NAME_LEN] = '\0';
	}
	if (0 != (c_rFlag & QUEST_SEND_COUNTER_VALUE))
	{
		if (!Recv(sizeof(iCounterValue), &iCounterValue))
			return false;
	}
	if (0 != (c_rFlag & QUEST_SEND_ICON_FILE))
	{
		if (!Recv(sizeof(szIconFileName), &szIconFileName))
			return false;

		szIconFileName[QUEST_ICON_FILE_NAME_LEN] = '\0';
	}
	// Recv Data End

	CPythonQuest& rkQuest = CPythonQuest::Instance();

	// Process Start
	if (QUEST_PACKET_TYPE_END == byQuestPacketType)
	{
		rkQuest.DeleteQuestInstance(QuestInfo.index);
	}
	else if (QUEST_PACKET_TYPE_UPDATE == byQuestPacketType)
	{
		if (!rkQuest.IsQuest(QuestInfo.index))
		{
#if defined(ENABLE_QUEST_RENEWAL)
			rkQuest.MakeQuest(QuestInfo.index, QuestInfo.c_index);
#else
			rkQuest.MakeQuest(QuestInfo.index);
#endif
		}

		if (strlen(szTitle) > 0)
			rkQuest.SetQuestTitle(QuestInfo.index, szTitle);
		if (strlen(szClockName) > 0)
			rkQuest.SetQuestClockName(QuestInfo.index, szClockName);
		if (strlen(szCounterName) > 0)
			rkQuest.SetQuestCounterName(QuestInfo.index, szCounterName);
		if (strlen(szIconFileName) > 0)
			rkQuest.SetQuestIconFileName(QuestInfo.index, szIconFileName);

		if (c_rFlag & QUEST_SEND_CLOCK_VALUE)
			rkQuest.SetQuestClockValue(QuestInfo.index, iClockValue);
		if (c_rFlag & QUEST_SEND_COUNTER_VALUE)
			rkQuest.SetQuestCounterValue(QuestInfo.index, iCounterValue);
	}
	else if (QUEST_PACKET_TYPE_BEGIN == byQuestPacketType)
	{
		CPythonQuest::SQuestInstance QuestInstance;
		QuestInstance.dwIndex = QuestInfo.index;
		QuestInstance.strTitle = szTitle;
		QuestInstance.strClockName = szClockName;
		QuestInstance.iClockValue = iClockValue;
		QuestInstance.strCounterName = szCounterName;
		QuestInstance.iCounterValue = iCounterValue;
		QuestInstance.strIconFileName = szIconFileName;
		//CPythonQuest::Instance().RegisterQuestInstance(QuestInstance);
		rkQuest.RegisterQuestInstance(QuestInstance);
	}
	// Process Start End

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshQuest", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::RecvQuestConfirmPacket()
{
	TPacketGCQuestConfirm kQuestConfirmPacket;
	if (!Recv(sizeof(kQuestConfirmPacket), &kQuestConfirmPacket))
	{
		Tracen("RecvQuestConfirmPacket Error");
		return false;
	}

	PyObject* poArg = Py_BuildValue("(sii)", kQuestConfirmPacket.msg, kQuestConfirmPacket.timeout, kQuestConfirmPacket.requestPID);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OnQuestConfirm", poArg);
	return true;
}

bool CPythonNetworkStream::RecvRequestMakeGuild()
{
	TPacketGCBlank blank;
	if (!Recv(sizeof(blank), &blank))
	{
		Tracen("RecvRequestMakeGuild Packet Error");
		return false;
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AskGuildName", Py_BuildValue("()"));

	return true;
}

void CPythonNetworkStream::ToggleGameDebugInfo()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ToggleDebugInfo", Py_BuildValue("()"));
}

bool CPythonNetworkStream::SendExchangeStartPacket(DWORD vid)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_START;
	packet.arg1 = vid;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_start_packet Error\n");
		return false;
	}

	Tracef("send_trade_start_packet vid %d \n", vid);
	return SendSequence();
}

bool CPythonNetworkStream::SendExchangeElkAddPacket(DWORD elk)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_ELK_ADD;
	packet.arg1 = elk;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_elk_add_packet Error\n");
		return false;
	}

	return SendSequence();
}

#if defined(ENABLE_CHEQUE_SYSTEM)
bool CPythonNetworkStream::SendExchangeChequeAddPacket(DWORD cheque)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_CHEQUE_ADD;
	packet.arg1 = cheque;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_cheque_add_packet Error\n");
		return false;
	}

	return SendSequence();
}
#endif

bool CPythonNetworkStream::SendExchangeItemAddPacket(TItemPos ItemPos, BYTE byDisplayPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_ITEM_ADD;
	packet.Pos = ItemPos;
	packet.arg2 = byDisplayPos;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_item_add_packet Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendExchangeItemDelPacket(BYTE pos)
{
	assert(!"Can't be called function - CPythonNetworkStream::SendExchangeItemDelPacket");
	return true;

	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_ITEM_DEL;
	packet.arg1 = pos;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_item_del_packet Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendExchangeAcceptPacket()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_ACCEPT;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_accept_packet Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendExchangeExitPacket()
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGExchange	packet;

	packet.header = HEADER_CG_EXCHANGE;
	packet.subheader = EXCHANGE_SUBHEADER_CG_CANCEL;

	if (!Send(sizeof(packet), &packet))
	{
		Tracef("send_trade_exit_packet Error\n");
		return false;
	}

	return SendSequence();
}

// PointReset 개임시
bool CPythonNetworkStream::SendPointResetPacket()
{
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "StartPointReset", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::__IsPlayerAttacking()
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();

	if (!pkInstMain)
		return false;

	if (!pkInstMain->IsAttacking())
		return false;

	return true;
}

bool CPythonNetworkStream::RecvScriptPacket()
{
	TPacketGCScript ScriptPacket;

	if (!Recv(sizeof(TPacketGCScript), &ScriptPacket))
	{
		TraceError("RecvScriptPacket_RecvError");
		return false;
	}

	if (ScriptPacket.size < sizeof(TPacketGCScript))
	{
		TraceError("RecvScriptPacket_SizeError");
		return false;
	}

	ScriptPacket.size -= sizeof(TPacketGCScript);

	static std::string str;
	str = "";
	str.resize(ScriptPacket.size + 1);

	if (!Recv(ScriptPacket.size, &str[0]))
		return false;

	str[str.size() - 1] = '\0';

	int iIndex = CPythonEventManager::Instance().RegisterEventSetFromString(str);

	if (-1 != iIndex)
	{
		CPythonEventManager::Instance().SetVisibleLineCount(iIndex, 30);
		CPythonNetworkStream::Instance().OnScriptEventStart(ScriptPacket.skin, iIndex);
	}

	return true;
}

bool CPythonNetworkStream::SendScriptAnswerPacket(int iAnswer)
{
	TPacketCGScriptAnswer ScriptAnswer;

	ScriptAnswer.header = HEADER_CG_SCRIPT_ANSWER;
	ScriptAnswer.answer = (BYTE)iAnswer;
	if (!Send(sizeof(TPacketCGScriptAnswer), &ScriptAnswer))
	{
		Tracen("Send Script Answer Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendScriptButtonPacket(unsigned int iIndex)
{
	TPacketCGScriptButton ScriptButton;

	ScriptButton.header = HEADER_CG_SCRIPT_BUTTON;
	ScriptButton.idx = iIndex;
	if (!Send(sizeof(TPacketCGScriptButton), &ScriptButton))
	{
		Tracen("Send Script Button Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendAnswerMakeGuildPacket(const char* c_szName)
{
	TPacketCGAnswerMakeGuild Packet;

	Packet.header = HEADER_CG_ANSWER_MAKE_GUILD;
	strncpy(Packet.guild_name, c_szName, GUILD_NAME_MAX_LEN);
	Packet.guild_name[GUILD_NAME_MAX_LEN] = '\0';

	if (!Send(sizeof(Packet), &Packet))
	{
		Tracen("SendAnswerMakeGuild Packet Error");
		return false;
	}

	//Tracef("SendAnswerMakeGuildPacket : %s", c_szName);
	return SendSequence();
}

bool CPythonNetworkStream::SendQuestInputStringPacket(const char* c_szString)
{
	TPacketCGQuestInputString Packet;
	Packet.bHeader = HEADER_CG_QUEST_INPUT_STRING;
	strncpy(Packet.szString, c_szString, QUEST_INPUT_STRING_MAX_NUM);

	if (!Send(sizeof(Packet), &Packet))
	{
		Tracen("SendQuestInputStringPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendQuestConfirmPacket(BYTE byAnswer, DWORD dwPID)
{
	TPacketCGQuestConfirm kPacket;
	kPacket.header = HEADER_CG_QUEST_CONFIRM;
	kPacket.answer = byAnswer;
	kPacket.requestPID = dwPID;

	if (!Send(sizeof(kPacket), &kPacket))
	{
		Tracen("SendQuestConfirmPacket Error");
		return false;
	}

	Tracenf("SendQuestConfirmPacket : %d, %d", byAnswer, dwPID);
	return SendSequence();
}

bool CPythonNetworkStream::RecvSkillCoolTimeEnd()
{
	TPacketGCSkillCoolTimeEnd kPacketSkillCoolTimeEnd;
	if (!Recv(sizeof(kPacketSkillCoolTimeEnd), &kPacketSkillCoolTimeEnd))
	{
		Tracen("CPythonNetworkStream::RecvSkillCoolTimeEnd - RecvError");
		return false;
	}

	CPythonPlayer::Instance().EndSkillCoolTime(kPacketSkillCoolTimeEnd.bSkill);

	return true;
}

bool CPythonNetworkStream::RecvSkillLevel()
{
	assert(!"CPythonNetworkStream::RecvSkillLevel - 사용하지 않는 함수");
	TPacketGCSkillLevel packet;
	if (!Recv(sizeof(TPacketGCSkillLevel), &packet))
	{
		Tracen("CPythonNetworkStream::RecvSkillLevel - RecvError");
		return false;
	}

	DWORD dwSlotIndex;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
	for (int i = 0; i < SKILL_MAX_NUM; ++i)
	{
		if (rkPlayer.GetSkillSlotIndex(i, &dwSlotIndex))
			rkPlayer.SetSkillLevel(dwSlotIndex, packet.abSkillLevels[i]);
	}

	__RefreshSkillWindow();
	__RefreshStatus();
	Tracef(" >> RecvSkillLevel\n");
	return true;
}

bool CPythonNetworkStream::RecvSkillLevelNew()
{
	TPacketGCSkillLevelNew packet;

	if (!Recv(sizeof(TPacketGCSkillLevelNew), &packet))
	{
		Tracen("CPythonNetworkStream::RecvSkillLevelNew - RecvError");
		return false;
	}

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();

	rkPlayer.SetSkill(7, 0);
	rkPlayer.SetSkill(8, 0);

	for (int i = 0; i < SKILL_MAX_NUM; ++i)
	{
		TPlayerSkill& rPlayerSkill = packet.skills[i];

		if (i >= 112 && i <= 115 && rPlayerSkill.bLevel)
			rkPlayer.SetSkill(7, i);

		if (i >= 116 && i <= 119 && rPlayerSkill.bLevel)
			rkPlayer.SetSkill(8, i);

#ifdef ENABLE_678TH_SKILL
		if (i >= 221 && i <= 229 && rPlayerSkill.bLevel)
			rkPlayer.SetSkill(7, i);

		if (i >= 236 && i <= 244 && rPlayerSkill.bLevel)
			rkPlayer.SetSkill(8, i);
#endif

		rkPlayer.SetSkillLevel_(i, rPlayerSkill.bMasterType, rPlayerSkill.bLevel);
	}

	__RefreshSkillWindow();
	__RefreshStatus();
	//Tracef(" >> RecvSkillLevelNew\n");
	return true;
}

bool CPythonNetworkStream::RecvDamageInfoPacket()
{
	TPacketGCDamageInfo DamageInfoPacket;

	if (!Recv(sizeof(TPacketGCDamageInfo), &DamageInfoPacket))
	{
		Tracen("Recv Target Packet Error");
		return false;
	}

	CInstanceBase* pInstTarget = CPythonCharacterManager::Instance().GetInstancePtr(DamageInfoPacket.dwVID);
	bool bSelf = (pInstTarget == CPythonCharacterManager::Instance().GetMainInstancePtr());
	bool bTarget = (pInstTarget == m_pInstTarget);
	if (pInstTarget)
	{
		if (DamageInfoPacket.damage >= 0)
			pInstTarget->AddDamageEffect(DamageInfoPacket.damage, DamageInfoPacket.flag, bSelf, bTarget);
		else
			TraceError("Damage is equal or below 0.");
	}

	return true;
}

bool CPythonNetworkStream::RecvTargetPacket()
{
	TPacketGCTarget TargetPacket;

	if (!Recv(sizeof(TPacketGCTarget), &TargetPacket))
	{
		Tracen("Recv Target Packet Error");
		return false;
	}

#if defined(ENABLE_SHIP_DEFENSE)
	if (TargetPacket.bAlliance)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetHPAllianceTargetBoard", Py_BuildValue("(iLL)", TargetPacket.dwVID, TargetPacket.iMinHP, TargetPacket.iMaxHP));
		return true;
	}
#endif

	CInstanceBase* pInstPlayer = CPythonCharacterManager::Instance().GetMainInstancePtr();
	CInstanceBase* pInstTarget = CPythonCharacterManager::Instance().GetInstancePtr(TargetPacket.dwVID);
	if (pInstPlayer && pInstTarget)
	{
		if (!pInstTarget->IsDead())
		{
#ifdef ENABLE_VIEW_TARGET_PLAYER_HP
			if (pInstTarget->IsBuilding())
#else
			if (pInstTarget->IsPC() || pInstTarget->IsBuilding())
#endif
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoardIfDifferent", Py_BuildValue("(i)", TargetPacket.dwVID));
			}
			else if (pInstPlayer->CanViewTargetHP(*pInstTarget))
			{
#if defined(ENABLE_VIEW_TARGET_DECIMAL_HP)
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetHPTargetBoard", Py_BuildValue("(iiLL)", TargetPacket.dwVID, TargetPacket.bHPPercent, TargetPacket.iMinHP, TargetPacket.iMaxHP));
#else
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetHPTargetBoard", Py_BuildValue("(ii)", TargetPacket.dwVID, TargetPacket.bHPPercent));
#endif

#if defined(ENABLE_ELEMENT_ADD)
				PyObject* poDict = PyDict_New();
				for (uint8_t uiElementIndex = 0; uiElementIndex < CPythonNonPlayer::MOB_ELEMENT_MAX_NUM; ++uiElementIndex)
					PyDict_SetItem(poDict, Py_BuildValue("i", uiElementIndex), Py_BuildValue("i", TargetPacket.bElement[uiElementIndex]));
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ShowTargetElementEnchant", Py_BuildValue("(O)", poDict));
#endif
			}
			else
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));
			}

			m_pInstTarget = pInstTarget;
		}
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));
	}

	return true;
}

#ifdef ENABLE_SEND_TARGET_INFO
bool CPythonNetworkStream::RecvTargetInfoPacket()
{
	TPacketGCTargetInfo pInfoTargetPacket;

	if (!Recv(sizeof(TPacketGCTargetInfo), &pInfoTargetPacket))
	{
		Tracen("Recv Info Target Packet Error");
		return false;
	}

	CPythonCharacterManager& rkIns = CPythonCharacterManager::Instance();
	CInstanceBase* pInstPlayer = rkIns.GetMainInstancePtr();
	CInstanceBase* pInstTarget = rkIns.GetInstancePtr(pInfoTargetPacket.dwVID);

	if (pInstPlayer && pInstTarget)
	{
		if (!pInstTarget->IsDead())
		{
			if (pInstTarget->IsEnemy() || pInstTarget->IsStone())
			{
				CPythonNonPlayer::instance().AddItemDropInfo(pInfoTargetPacket.dwRaceVnum, pInfoTargetPacket.bIndex, pInfoTargetPacket.dwVnum, pInfoTargetPacket.wCount);
			}
			else
			{
				PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));
				CPythonNonPlayer::instance().ClearItemDropInfo();
			}
		}
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "CloseTargetBoard", Py_BuildValue("()"));
		CPythonNonPlayer::instance().ClearItemDropInfo();
	}

	return true;
}
#endif

bool CPythonNetworkStream::RecvMountPacket()
{
	TPacketGCMount MountPacket;

	if (!Recv(sizeof(TPacketGCMount), &MountPacket))
	{
		Tracen("Recv Mount Packet Error");
		return false;
	}

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(MountPacket.vid);

	if (pInstance)
	{
		// Mount
		if (0 != MountPacket.mount_vid)
		{
			//pInstance->Ride(MountPacket.pos, MountPacket.mount_vid);
		}
		// Unmount
		else
		{
			//pInstance->Unride(MountPacket.pos, MountPacket.x, MountPacket.y);
		}
	}

	if (CPythonPlayer::Instance().IsMainCharacterIndex(MountPacket.vid))
	{
		//CPythonPlayer::Instance().SetRidingVehicleIndex(MountPacket.mount_vid);
	}

	return true;
}

bool CPythonNetworkStream::RecvChangeSpeedPacket()
{
	TPacketGCChangeSpeed SpeedPacket;

	if (!Recv(sizeof(TPacketGCChangeSpeed), &SpeedPacket))
	{
		Tracen("Recv Speed Packet Error");
		return false;
	}

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(SpeedPacket.vid);

	if (!pInstance)
		return true;

	//pInstance->SetWalkSpeed(SpeedPacket.walking_speed);
	//pInstance->SetRunSpeed(SpeedPacket.running_speed);
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Recv
bool CPythonNetworkStream::SendAttackPacket(UINT uMotAttack, DWORD dwVIDVictim)
{
	if (!__CanActMainInstance())
		return true;

#ifdef ATTACK_TIME_LOG
	static DWORD prevTime = timeGetTime();
	DWORD curTime = timeGetTime();
	TraceError("TIME: %.4f(%.4f) ATTACK_PACKET: %d TARGET: %d", curTime / 1000.0f, (curTime - prevTime) / 1000.0f, uMotAttack, dwVIDVictim);
	prevTime = curTime;
#endif

	TPacketCGAttack kPacketAtk;
	kPacketAtk.header = HEADER_CG_ATTACK;
	kPacketAtk.bType = uMotAttack;
	kPacketAtk.dwVictimVID = dwVIDVictim;

	if (!SendSpecial(sizeof(kPacketAtk), &kPacketAtk))
	{
		Tracen("Send Battle Attack Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendSpecial(int nLen, void* pvBuf)
{
	BYTE bHeader = *(BYTE*)pvBuf;

	switch (bHeader)
	{
	case HEADER_CG_ATTACK:
	{
		TPacketCGAttack* pkPacketAtk = (TPacketCGAttack*)pvBuf;
		pkPacketAtk->bCRCMagicCubeProcPiece = GetProcessCRCMagicCubePiece();
		pkPacketAtk->bCRCMagicCubeFilePiece = GetProcessCRCMagicCubePiece();
		return Send(nLen, pvBuf);
	}
	break;
	}

	return Send(nLen, pvBuf);
}

bool CPythonNetworkStream::RecvAddFlyTargetingPacket()
{
	TPacketGCFlyTargeting kPacket;
	if (!Recv(sizeof(kPacket), &kPacket))
		return false;

	__GlobalPositionToLocalPosition(kPacket.lX, kPacket.lY);

	Tracef("VID [%d]가 타겟을 추가 설정\n", kPacket.dwShooterVID);

	CPythonCharacterManager& rpcm = CPythonCharacterManager::Instance();

	CInstanceBase* pShooter = rpcm.GetInstancePtr(kPacket.dwShooterVID);

	if (!pShooter)
	{
#ifndef _DEBUG
		TraceError("CPythonNetworkStream::RecvFlyTargetingPacket() - dwShooterVID[%d] NOT EXIST", kPacket.dwShooterVID);
#endif
		return true;
	}

	CInstanceBase* pTarget = rpcm.GetInstancePtr(kPacket.dwTargetVID);

	if (kPacket.dwTargetVID && pTarget)
	{
		pShooter->GetGraphicThingInstancePtr()->AddFlyTarget(pTarget->GetGraphicThingInstancePtr());
	}
	else
	{
		float h = CPythonBackground::Instance().GetHeight(kPacket.lX, kPacket.lY) + 60.0f; // TEMPORARY HEIGHT
		pShooter->GetGraphicThingInstancePtr()->AddFlyTarget(D3DXVECTOR3(kPacket.lX, kPacket.lY, h));
		//pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(kPacket.kPPosTarget.x,kPacket.kPPosTarget.y,);
	}

	return true;
}

bool CPythonNetworkStream::RecvFlyTargetingPacket()
{
	TPacketGCFlyTargeting kPacket;
	if (!Recv(sizeof(kPacket), &kPacket))
		return false;

	__GlobalPositionToLocalPosition(kPacket.lX, kPacket.lY);

	//Tracef("CPythonNetworkStream::RecvFlyTargetingPacket - VID [%d]\n",kPacket.dwShooterVID);

	CPythonCharacterManager& rpcm = CPythonCharacterManager::Instance();

	CInstanceBase* pShooter = rpcm.GetInstancePtr(kPacket.dwShooterVID);

	if (!pShooter)
	{
#ifdef _DEBUG
		TraceError("CPythonNetworkStream::RecvFlyTargetingPacket() - dwShooterVID[%d] NOT EXIST", kPacket.dwShooterVID);
#endif
		return true;
	}

	CInstanceBase* pTarget = rpcm.GetInstancePtr(kPacket.dwTargetVID);

	if (kPacket.dwTargetVID && pTarget)
	{
		pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(pTarget->GetGraphicThingInstancePtr());
	}
	else
	{
		float h = CPythonBackground::Instance().GetHeight(kPacket.lX, kPacket.lY) + 60.0f; // TEMPORARY HEIGHT
		pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(D3DXVECTOR3(kPacket.lX, kPacket.lY, h));
		//pShooter->GetGraphicThingInstancePtr()->SetFlyTarget(kPacket.kPPosTarget.x,kPacket.kPPosTarget.y,);
	}

	return true;
}

bool CPythonNetworkStream::SendShootPacket(UINT uSkill)
{
	TPacketCGShoot kPacketShoot;
	kPacketShoot.bHeader = HEADER_CG_SHOOT;
	kPacketShoot.bType = uSkill;

	if (!Send(sizeof(kPacketShoot), &kPacketShoot))
	{
		Tracen("SendShootPacket Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendAddFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget)
{
	TPacketCGFlyTargeting packet;

	//CPythonCharacterManager & rpcm = CPythonCharacterManager::Instance();

	packet.bHeader = HEADER_CG_ADD_FLY_TARGETING;
	packet.dwTargetVID = dwTargetVID;
	packet.lX = kPPosTarget.x;
	packet.lY = kPPosTarget.y;

	__LocalPositionToGlobalPosition(packet.lX, packet.lY);

	if (!Send(sizeof(packet), &packet))
	{
		Tracen("Send FlyTargeting Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendFlyTargetingPacket(DWORD dwTargetVID, const TPixelPosition& kPPosTarget)
{
	TPacketCGFlyTargeting packet;

	//CPythonCharacterManager & rpcm = CPythonCharacterManager::Instance();

	packet.bHeader = HEADER_CG_FLY_TARGETING;
	packet.dwTargetVID = dwTargetVID;
	packet.lX = kPPosTarget.x;
	packet.lY = kPPosTarget.y;

	__LocalPositionToGlobalPosition(packet.lX, packet.lY);

	if (!Send(sizeof(packet), &packet))
	{
		Tracen("Send FlyTargeting Packet Error");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RecvCreateFlyPacket()
{
	TPacketGCCreateFly kPacket;
	if (!Recv(sizeof(TPacketGCCreateFly), &kPacket))
		return false;

	CFlyingManager& rkFlyMgr = CFlyingManager::Instance();
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();

	CInstanceBase* pkStartInst = rkChrMgr.GetInstancePtr(kPacket.dwStartVID);
	CInstanceBase* pkEndInst = rkChrMgr.GetInstancePtr(kPacket.dwEndVID);
	if (!pkStartInst || !pkEndInst)
		return true;

	rkFlyMgr.CreateIndexedFly(kPacket.bType, pkStartInst->GetGraphicThingInstancePtr(), pkEndInst->GetGraphicThingInstancePtr());

	return true;
}

bool CPythonNetworkStream::SendTargetPacket(DWORD dwVID)
{
	TPacketCGTarget packet;
	packet.header = HEADER_CG_TARGET;
	packet.dwVID = dwVID;

	if (!Send(sizeof(packet), &packet))
	{
		Tracen("Send Target Packet Error");
		return false;
	}

	return SendSequence();
}

#ifdef ENABLE_SKILL_COLOR_SYSTEM
bool CPythonNetworkStream::SendSkillColorPacket(BYTE bSkillSlot,
	DWORD dwCol1,
	DWORD dwCol2,
	DWORD dwCol3,
	DWORD dwCol4,
	DWORD dwCol5
)
{
	TPacketCGSkillColor CGPacket;
	CGPacket.bHeader = HEADER_CG_SKILL_COLOR;
	CGPacket.bSkillSlot = bSkillSlot;
	CGPacket.dwCol1 = dwCol1;
	CGPacket.dwCol2 = dwCol2;
	CGPacket.dwCol3 = dwCol3;
	CGPacket.dwCol4 = dwCol4;
	CGPacket.dwCol5 = dwCol5;

	if (!Send(sizeof(CGPacket), &CGPacket))
	{
		Tracen("Send Skill Color Packet Error");
		return false;
	}

	return SendSequence();
}
#endif

bool CPythonNetworkStream::SendSyncPositionElementPacket(DWORD dwVictimVID, DWORD dwVictimX, DWORD dwVictimY)
{
	TPacketCGSyncPositionElement kSyncPos;
	kSyncPos.dwVID = dwVictimVID;
	kSyncPos.lX = dwVictimX;
	kSyncPos.lY = dwVictimY;

	__LocalPositionToGlobalPosition(kSyncPos.lX, kSyncPos.lY);

	if (!Send(sizeof(kSyncPos), &kSyncPos))
	{
		Tracen("CPythonNetworkStream::SendSyncPositionElementPacket - ERROR");
		return false;
	}

	return true;
}

bool CPythonNetworkStream::RecvMessenger()
{
	TPacketGCMessenger p;
	if (!Recv(sizeof(p), &p))
		return false;

	int iSize = p.size - sizeof(p);
	char char_name[CHARACTER_NAME_MAX_LEN + 1];

	switch (p.subheader)
	{
	case MESSENGER_SUBHEADER_GC_LIST:
	{
		TPacketGCMessengerListOnline on;
		while (iSize)
		{
			if (!Recv(sizeof(TPacketGCMessengerListOffline), &on))
				return false;

			if (!Recv(on.length, char_name))
				return false;

			char_name[on.length] = 0;

			if (on.connected & MESSENGER_CONNECTED_STATE_ONLINE)
				CPythonMessenger::Instance().OnFriendLogin(char_name);
			else
				CPythonMessenger::Instance().OnFriendLogout(char_name);

			iSize -= sizeof(TPacketGCMessengerListOffline);
			iSize -= on.length;
		}
		break;
	}

	case MESSENGER_SUBHEADER_GC_LOGIN:
	{
		TPacketGCMessengerLogin p;
		if (!Recv(sizeof(p), &p))
			return false;
		if (!Recv(p.length, char_name))
			return false;
		char_name[p.length] = 0;
		CPythonMessenger::Instance().OnFriendLogin(char_name);
		__RefreshTargetBoardByName(char_name);
		break;
	}

	case MESSENGER_SUBHEADER_GC_LOGOUT:
	{
		TPacketGCMessengerLogout logout;
		if (!Recv(sizeof(logout), &logout))
			return false;
		if (!Recv(logout.length, char_name))
			return false;
		char_name[logout.length] = 0;
		CPythonMessenger::Instance().OnFriendLogout(char_name);
		break;
	}

#ifdef ENABLE_MESSENGER_GM
	case MESSENGER_SUBHEADER_GC_GM_LIST:
	{
		TPacketGCMessengerGMListOnline gm_on;
		while (iSize)
		{
			if (!Recv(sizeof(TPacketGCMessengerGMListOffline), &gm_on))
				return false;

			if (!Recv(gm_on.length, char_name))
				return false;

			char_name[gm_on.length] = 0;

			if (gm_on.connected & MESSENGER_CONNECTED_STATE_ONLINE)
				CPythonMessenger::Instance().OnGMLogin(char_name);
			else
				CPythonMessenger::Instance().OnGMLogout(char_name);

			iSize -= sizeof(TPacketGCMessengerGMListOffline);
			iSize -= gm_on.length;
		}
		break;
	}

	case MESSENGER_SUBHEADER_GC_GM_LOGIN:
	{
		TPacketGCMessengerLogin gm_p;
		if (!Recv(sizeof(gm_p), &gm_p))
			return false;
		if (!Recv(gm_p.length, char_name))
			return false;
		char_name[gm_p.length] = 0;
		CPythonMessenger::Instance().OnGMLogin(char_name);
		__RefreshTargetBoardByName(char_name);
		break;
	}

	case MESSENGER_SUBHEADER_GC_GM_LOGOUT:
	{
		TPacketGCMessengerLogout gm_logout;
		if (!Recv(sizeof(gm_logout), &gm_logout))
			return false;
		if (!Recv(gm_logout.length, char_name))
			return false;
		char_name[gm_logout.length] = 0;
		CPythonMessenger::Instance().OnGMLogout(char_name);
		break;
	}
#endif

#ifdef ENABLE_MESSENGER_BLOCK
	case MESSENGER_SUBHEADER_GC_BLOCK_LIST:
	{
		TPacketGCMessengerBlockListOnline block_on;
		while (iSize)
		{
			if (!Recv(sizeof(TPacketGCMessengerBlockListOffline), &block_on))
				return false;

			if (!Recv(block_on.length, char_name))
				return false;

			char_name[block_on.length] = 0;

			if (block_on.connected & MESSENGER_CONNECTED_STATE_ONLINE)
				CPythonMessenger::Instance().OnBlockLogin(char_name);
			else
				CPythonMessenger::Instance().OnBlockLogout(char_name);

			iSize -= sizeof(TPacketGCMessengerBlockListOffline);
			iSize -= block_on.length;
		}
		break;
	}

	case MESSENGER_SUBHEADER_GC_BLOCK_LOGIN:
	{
		TPacketGCMessengerLogin block_p;
		if (!Recv(sizeof(block_p), &block_p))
			return false;
		if (!Recv(block_p.length, char_name))
			return false;
		char_name[block_p.length] = 0;
		CPythonMessenger::Instance().OnBlockLogin(char_name);
		__RefreshTargetBoardByName(char_name);
		break;
	}

	case MESSENGER_SUBHEADER_GC_BLOCK_LOGOUT:
	{
		TPacketGCMessengerLogout block_logout;
		if (!Recv(sizeof(block_logout), &block_logout))
			return false;
		if (!Recv(block_logout.length, char_name))
			return false;
		char_name[block_logout.length] = 0;
		CPythonMessenger::Instance().OnBlockLogout(char_name);
		break;
	}
#endif
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Party

bool CPythonNetworkStream::SendPartyInvitePacket(DWORD dwVID)
{
	TPacketCGPartyInvite kPartyInvitePacket;
	kPartyInvitePacket.header = HEADER_CG_PARTY_INVITE;
	kPartyInvitePacket.vid = dwVID;

	if (!Send(sizeof(kPartyInvitePacket), &kPartyInvitePacket))
	{
		Tracenf("CPythonNetworkStream::SendPartyInvitePacket [%ud] - PACKET SEND ERROR", dwVID);
		return false;
	}

	Tracef(" << SendPartyInvitePacket : %d\n", dwVID);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartyInviteAnswerPacket(DWORD dwLeaderVID, BYTE byAnswer)
{
	TPacketCGPartyInviteAnswer kPartyInviteAnswerPacket;
	kPartyInviteAnswerPacket.header = HEADER_CG_PARTY_INVITE_ANSWER;
	kPartyInviteAnswerPacket.leader_pid = dwLeaderVID;
	kPartyInviteAnswerPacket.accept = byAnswer;

	if (!Send(sizeof(kPartyInviteAnswerPacket), &kPartyInviteAnswerPacket))
	{
		Tracenf("CPythonNetworkStream::SendPartyInviteAnswerPacket [%ud %ud] - PACKET SEND ERROR", dwLeaderVID, byAnswer);
		return false;
	}

	Tracef(" << SendPartyInviteAnswerPacket : %d, %d\n", dwLeaderVID, byAnswer);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartyRemovePacket(DWORD dwPID)
{
	TPacketCGPartyRemove kPartyInviteRemove;
	kPartyInviteRemove.header = HEADER_CG_PARTY_REMOVE;
	kPartyInviteRemove.pid = dwPID;

	if (!Send(sizeof(kPartyInviteRemove), &kPartyInviteRemove))
	{
		Tracenf("CPythonNetworkStream::SendPartyRemovePacket [%ud] - PACKET SEND ERROR", dwPID);
		return false;
	}

	Tracef(" << SendPartyRemovePacket : %d\n", dwPID);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartySetStatePacket(DWORD dwVID, BYTE byState, BYTE byFlag)
{
	TPacketCGPartySetState kPartySetState;
	kPartySetState.byHeader = HEADER_CG_PARTY_SET_STATE;
	kPartySetState.dwVID = dwVID;
	kPartySetState.byState = byState;
	kPartySetState.byFlag = byFlag;

	if (!Send(sizeof(kPartySetState), &kPartySetState))
	{
		Tracenf("CPythonNetworkStream::SendPartySetStatePacket(%ud, %ud) - PACKET SEND ERROR", dwVID, byState);
		return false;
	}

	Tracef(" << SendPartySetStatePacket : %d, %d, %d\n", dwVID, byState, byFlag);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartyUseSkillPacket(BYTE bySkillIndex, DWORD dwVID)
{
	TPacketCGPartyUseSkill kPartyUseSkill;
	kPartyUseSkill.byHeader = HEADER_CG_PARTY_USE_SKILL;
	kPartyUseSkill.bySkillIndex = bySkillIndex;
	kPartyUseSkill.dwTargetVID = dwVID;

	if (!Send(sizeof(kPartyUseSkill), &kPartyUseSkill))
	{
		Tracenf("CPythonNetworkStream::SendPartyUseSkillPacket(%ud, %ud) - PACKET SEND ERROR", bySkillIndex, dwVID);
		return false;
	}

	Tracef(" << SendPartyUseSkillPacket : %d, %d\n", bySkillIndex, dwVID);
	return SendSequence();
}

bool CPythonNetworkStream::SendPartyParameterPacket(BYTE byDistributeMode)
{
	TPacketCGPartyParameter kPartyParameter;
	kPartyParameter.bHeader = HEADER_CG_PARTY_PARAMETER;
	kPartyParameter.bDistributeMode = byDistributeMode;

	if (!Send(sizeof(kPartyParameter), &kPartyParameter))
	{
		Tracenf("CPythonNetworkStream::SendPartyParameterPacket(%d) - PACKET SEND ERROR", byDistributeMode);
		return false;
	}

	Tracef(" << SendPartyParameterPacket : %d\n", byDistributeMode);
	return SendSequence();
}

bool CPythonNetworkStream::RecvPartyInvite()
{
	TPacketGCPartyInvite kPartyInvitePacket;
	if (!Recv(sizeof(kPartyInvitePacket), &kPartyInvitePacket))
		return false;

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(kPartyInvitePacket.leader_pid);
	if (!pInstance)
	{
		TraceError(" CPythonNetworkStream::RecvPartyInvite - Failed to find leader instance [%d]\n", kPartyInvitePacket.leader_pid);
		return true;
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RecvPartyInviteQuestion", Py_BuildValue("(is)", kPartyInvitePacket.leader_pid, pInstance->GetNameString()));
	Tracef(" >> RecvPartyInvite : %d, %s\n", kPartyInvitePacket.leader_pid, pInstance->GetNameString());

	return true;
}

bool CPythonNetworkStream::RecvPartyAdd()
{
	TPacketGCPartyAdd kPartyAddPacket;
	if (!Recv(sizeof(kPartyAddPacket), &kPartyAddPacket))
		return false;

	CPythonPlayer::Instance().AppendPartyMember(kPartyAddPacket.pid, kPartyAddPacket.name);

#if defined(WJ_SHOW_PARTY_ON_MINIMAP)
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AddPartyMember", Py_BuildValue("(isii)", kPartyAddPacket.pid, kPartyAddPacket.name, kPartyAddPacket.mapIdx, kPartyAddPacket.channel));
	Tracef(" >> RecvPartyAdd : %d, %s, %d, %d\n", kPartyAddPacket.pid, kPartyAddPacket.name, kPartyAddPacket.mapIdx, kPartyAddPacket.channel);
#else
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AddPartyMember", Py_BuildValue("(is)", kPartyAddPacket.pid, kPartyAddPacket.name));
	Tracef(" >> RecvPartyAdd : %d, %s\n", kPartyAddPacket.pid, kPartyAddPacket.name);
#endif

	return true;
}

bool CPythonNetworkStream::RecvPartyUpdate()
{
	TPacketGCPartyUpdate kPartyUpdatePacket;
	if (!Recv(sizeof(kPartyUpdatePacket), &kPartyUpdatePacket))
		return false;

	CPythonPlayer::TPartyMemberInfo* pPartyMemberInfo;
	if (!CPythonPlayer::Instance().GetPartyMemberPtr(kPartyUpdatePacket.pid, &pPartyMemberInfo))
		return true;

	BYTE byOldState = pPartyMemberInfo->byState;

	CPythonPlayer::Instance().UpdatePartyMemberInfo(kPartyUpdatePacket.pid, kPartyUpdatePacket.state, kPartyUpdatePacket.percent_hp);
	for (int i = 0; i < PARTY_AFFECT_SLOT_MAX_NUM; ++i)
	{
		CPythonPlayer::Instance().UpdatePartyMemberAffect(kPartyUpdatePacket.pid, i, kPartyUpdatePacket.affects[i]);
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "UpdatePartyMemberInfo", Py_BuildValue("(i)", kPartyUpdatePacket.pid));

	// 만약 리더가 바뀌었다면, TargetBoard 의 버튼을 업데이트 한다.
	DWORD dwVID;
	if (CPythonPlayer::Instance().PartyMemberPIDToVID(kPartyUpdatePacket.pid, &dwVID))
		if (byOldState != kPartyUpdatePacket.state)
		{
			__RefreshTargetBoardByVID(dwVID);
		}

	// 	Tracef(" >> RecvPartyUpdate : %d, %d, %d\n", kPartyUpdatePacket.pid, kPartyUpdatePacket.state, kPartyUpdatePacket.percent_hp);

	return true;
}

bool CPythonNetworkStream::RecvPartyRemove()
{
	TPacketGCPartyRemove kPartyRemovePacket;
	if (!Recv(sizeof(kPartyRemovePacket), &kPartyRemovePacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RemovePartyMember", Py_BuildValue("(i)", kPartyRemovePacket.pid));
	Tracef(" >> RecvPartyRemove : %d\n", kPartyRemovePacket.pid);

	return true;
}

bool CPythonNetworkStream::RecvPartyLink()
{
	TPacketGCPartyLink kPartyLinkPacket;
	if (!Recv(sizeof(kPartyLinkPacket), &kPartyLinkPacket))
		return false;

	CPythonPlayer::Instance().LinkPartyMember(kPartyLinkPacket.pid, kPartyLinkPacket.vid);

#if defined(WJ_SHOW_PARTY_ON_MINIMAP)
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "LinkPartyMember", Py_BuildValue("(iiii)", kPartyLinkPacket.pid, kPartyLinkPacket.vid, kPartyLinkPacket.mapIdx, kPartyLinkPacket.channel));
	Tracef(" >> RecvPartyLink : %d, %d, %d, %d\n", kPartyLinkPacket.pid, kPartyLinkPacket.vid, kPartyLinkPacket.mapIdx, kPartyLinkPacket.channel);
#else
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "LinkPartyMember", Py_BuildValue("(ii)", kPartyLinkPacket.pid, kPartyLinkPacket.vid));
	Tracef(" >> RecvPartyLink : %d, %d\n", kPartyLinkPacket.pid, kPartyLinkPacket.vid);
#endif

	return true;
}

bool CPythonNetworkStream::RecvPartyUnlink()
{
	TPacketGCPartyUnlink kPartyUnlinkPacket;
	if (!Recv(sizeof(kPartyUnlinkPacket), &kPartyUnlinkPacket))
		return false;

	CPythonPlayer::Instance().UnlinkPartyMember(kPartyUnlinkPacket.pid);

	if (CPythonPlayer::Instance().IsMainCharacterIndex(kPartyUnlinkPacket.vid))
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "UnlinkAllPartyMember", Py_BuildValue("()"));
	}
	else
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "UnlinkPartyMember", Py_BuildValue("(i)", kPartyUnlinkPacket.pid));
	}

	Tracef(" >> RecvPartyUnlink : %d, %d\n", kPartyUnlinkPacket.pid, kPartyUnlinkPacket.vid);

	return true;
}

bool CPythonNetworkStream::RecvPartyParameter()
{
	TPacketGCPartyParameter kPartyParameterPacket;
	if (!Recv(sizeof(kPartyParameterPacket), &kPartyParameterPacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ChangePartyParameter", Py_BuildValue("(i)", kPartyParameterPacket.bDistributeMode));
	Tracef(" >> RecvPartyParameter : %d\n", kPartyParameterPacket.bDistributeMode);

	return true;
}

#if defined(WJ_SHOW_PARTY_ON_MINIMAP)
bool CPythonNetworkStream::RecvPartyPositionInfo()
{
	TPacketGCPartyPosition Packet;
	if (!Recv(sizeof(Packet), &Packet))
	{
		Tracen("RecvPartyPositionInfo Error");
		return false;
	}

	for (auto iPacketSize = Packet.wSize - sizeof(Packet); iPacketSize > 0; iPacketSize -= sizeof(SPartyPosition))
	{
		SPartyPosition PositionInfo;
		if (Recv(sizeof(PositionInfo), &PositionInfo))
			CPythonMiniMap::Instance().AddPartyPositionInfo(PositionInfo);
	}

	return true;
}
#endif
// Party
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Guild
bool CPythonNetworkStream::SendGuildAddMemberPacket(DWORD dwVID)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_ADD_MEMBER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwVID), &dwVID))
		return false;

	Tracef(" SendGuildAddMemberPacket\n", dwVID);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildRemoveMemberPacket(DWORD dwPID)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_REMOVE_MEMBER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwPID), &dwPID))
		return false;

	Tracef(" SendGuildRemoveMemberPacket %d\n", dwPID);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChangeGradeNamePacket(BYTE byGradeNumber, const char* c_szName)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_GRADE_NAME;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(byGradeNumber), &byGradeNumber))
		return false;

	char szName[GUILD_GRADE_NAME_MAX_LEN + 1];
	strncpy(szName, c_szName, GUILD_GRADE_NAME_MAX_LEN);
	szName[GUILD_GRADE_NAME_MAX_LEN] = '\0';

	if (!Send(sizeof(szName), &szName))
		return false;

	Tracef(" SendGuildChangeGradeNamePacket %d, %s\n", byGradeNumber, c_szName);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChangeGradeAuthorityPacket(BYTE byGradeNumber, BYTE byAuthority)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_GRADE_AUTHORITY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(byGradeNumber), &byGradeNumber))
		return false;
	if (!Send(sizeof(byAuthority), &byAuthority))
		return false;

	Tracef(" SendGuildChangeGradeAuthorityPacket %d, %d\n", byGradeNumber, byAuthority);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildOfferPacket(DWORD dwExperience)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_OFFER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwExperience), &dwExperience))
		return false;

	Tracef(" SendGuildOfferPacket %d\n", dwExperience);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildPostCommentPacket(const char* c_szMessage)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_POST_COMMENT;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	BYTE bySize = BYTE(strlen(c_szMessage)) + 1;
	if (!Send(sizeof(bySize), &bySize))
		return false;
	if (!Send(bySize, c_szMessage))
		return false;

	Tracef(" SendGuildPostCommentPacket %d, %s\n", bySize, c_szMessage);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildDeleteCommentPacket(DWORD dwIndex)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_DELETE_COMMENT;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwIndex), &dwIndex))
		return false;

	Tracef(" SendGuildDeleteCommentPacket %d\n", dwIndex);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildRefreshCommentsPacket(DWORD dwHighestIndex)
{
	static DWORD s_LastTime = timeGetTime() - 1001;

	if (timeGetTime() - s_LastTime < 1000)
		return true;
	s_LastTime = timeGetTime();

	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_REFRESH_COMMENT;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	Tracef(" SendGuildRefreshCommentPacket %d\n", dwHighestIndex);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChangeMemberGradePacket(DWORD dwPID, BYTE byGrade)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_MEMBER_GRADE;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwPID), &dwPID))
		return false;
	if (!Send(sizeof(byGrade), &byGrade))
		return false;

	Tracef(" SendGuildChangeMemberGradePacket %d, %d\n", dwPID, byGrade);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildUseSkillPacket(DWORD dwSkillID, DWORD dwTargetVID)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_USE_SKILL;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwSkillID), &dwSkillID))
		return false;
	if (!Send(sizeof(dwTargetVID), &dwTargetVID))
		return false;

	Tracef(" SendGuildUseSkillPacket %d, %d\n", dwSkillID, dwTargetVID);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChangeMemberGeneralPacket(DWORD dwPID, BYTE byFlag)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHANGE_MEMBER_GENERAL;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwPID), &dwPID))
		return false;
	if (!Send(sizeof(byFlag), &byFlag))
		return false;

	Tracef(" SendGuildChangeMemberGeneralFlagPacket %d, %d\n", dwPID, byFlag);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildInviteAnswerPacket(DWORD dwGuildID, BYTE byAnswer)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_GUILD_INVITE_ANSWER;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwGuildID), &dwGuildID))
		return false;
	if (!Send(sizeof(byAnswer), &byAnswer))
		return false;

	Tracef(" SendGuildInviteAnswerPacket %d, %d\n", dwGuildID, byAnswer);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildChargeGSPPacket(DWORD dwMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_CHARGE_GSP;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;

	if (!Send(sizeof(dwMoney), &dwMoney))
		return false;

	Tracef(" SendGuildChargeGSPPacket %d\n", dwMoney);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildDepositMoneyPacket(DWORD dwMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_DEPOSIT_MONEY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwMoney), &dwMoney))
		return false;

	Tracef(" SendGuildDepositMoneyPacket %d\n", dwMoney);
	return SendSequence();
}

bool CPythonNetworkStream::SendGuildWithdrawMoneyPacket(DWORD dwMoney)
{
	TPacketCGGuild GuildPacket;
	GuildPacket.byHeader = HEADER_CG_GUILD;
	GuildPacket.bySubHeader = GUILD_SUBHEADER_CG_WITHDRAW_MONEY;
	if (!Send(sizeof(GuildPacket), &GuildPacket))
		return false;
	if (!Send(sizeof(dwMoney), &dwMoney))
		return false;

	Tracef(" SendGuildWithdrawMoneyPacket %d\n", dwMoney);
	return SendSequence();
}

bool CPythonNetworkStream::RecvGuild()
{
	TPacketGCGuild GuildPacket;
	if (!Recv(sizeof(GuildPacket), &GuildPacket))
		return false;

	switch (GuildPacket.subheader)
	{
	case GUILD_SUBHEADER_GC_LOGIN:
	{
		DWORD dwPID;
		if (!Recv(sizeof(DWORD), &dwPID))
			return false;

		// Messenger
		CPythonGuild::TGuildMemberData* pGuildMemberData;
		if (CPythonGuild::Instance().GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
			if (0 != pGuildMemberData->strName.compare(CPythonPlayer::Instance().GetName()))
				CPythonMessenger::Instance().LoginGuildMember(pGuildMemberData->strName.c_str());

		//Tracef(" <Login> %d\n", dwPID);
		break;
	}
	case GUILD_SUBHEADER_GC_LOGOUT:
	{
		DWORD dwPID;
		if (!Recv(sizeof(DWORD), &dwPID))
			return false;

		// Messenger
		CPythonGuild::TGuildMemberData* pGuildMemberData;
		if (CPythonGuild::Instance().GetMemberDataPtrByPID(dwPID, &pGuildMemberData))
			if (0 != pGuildMemberData->strName.compare(CPythonPlayer::Instance().GetName()))
				CPythonMessenger::Instance().LogoutGuildMember(pGuildMemberData->strName.c_str());

		//Tracef(" <Logout> %d\n", dwPID);
		break;
	}
	case GUILD_SUBHEADER_GC_REMOVE:
	{
		DWORD dwPID;
		if (!Recv(sizeof(dwPID), &dwPID))
			return false;

		// Main Player 일 경우 DeleteGuild
		if (CPythonGuild::Instance().IsMainPlayer(dwPID))
		{
			CPythonGuild::Instance().Destroy();
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "DeleteGuild", Py_BuildValue("()"));
			CPythonMessenger::Instance().RemoveAllGuildMember();
			__SetGuildID(0);
			__RefreshMessengerWindow();
			__RefreshTargetBoard();
			__RefreshCharacterWindow();
		}
		else
		{
			// Get Member Name
			std::string strMemberName = "";
			CPythonGuild::TGuildMemberData* pData;
			if (CPythonGuild::Instance().GetMemberDataPtrByPID(dwPID, &pData))
			{
				strMemberName = pData->strName;
				CPythonMessenger::Instance().RemoveGuildMember(pData->strName.c_str());
			}

			CPythonGuild::Instance().RemoveMember(dwPID);

			// Refresh
			__RefreshTargetBoardByName(strMemberName.c_str());
			__RefreshGuildWindowMemberPage();
		}

		Tracef(" <Remove> %d\n", dwPID);
		break;
	}
	case GUILD_SUBHEADER_GC_LIST:
	{
		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);

		for (; iPacketSize > 0;)
		{
			TPacketGCGuildSubMember memberPacket;
			if (!Recv(sizeof(memberPacket), &memberPacket))
				return false;

			char szName[CHARACTER_NAME_MAX_LEN + 1] = "";
			if (memberPacket.byNameFlag)
			{
				if (!Recv(sizeof(szName), &szName))
					return false;

				iPacketSize -= CHARACTER_NAME_MAX_LEN + 1;
			}
			else
			{
				CPythonGuild::TGuildMemberData* pMemberData;
				if (CPythonGuild::Instance().GetMemberDataPtrByPID(memberPacket.pid, &pMemberData))
				{
					strncpy(szName, pMemberData->strName.c_str(), CHARACTER_NAME_MAX_LEN);
				}
			}

			//Tracef(" <List> %d : %s, %d (%d, %d, %d)\n", memberPacket.pid, szName, memberPacket.byGrade, memberPacket.byJob, memberPacket.byLevel, memberPacket.dwOffer);

			CPythonGuild::SGuildMemberData GuildMemberData;
			GuildMemberData.dwPID = memberPacket.pid;
			GuildMemberData.byGrade = memberPacket.byGrade;
			GuildMemberData.strName = szName;
			GuildMemberData.byJob = memberPacket.byJob;
			GuildMemberData.byLevel = memberPacket.byLevel;
			GuildMemberData.dwOffer = memberPacket.dwOffer;
			GuildMemberData.byGeneralFlag = memberPacket.byIsGeneral;
			CPythonGuild::Instance().RegisterMember(GuildMemberData);

			// Messenger
			if (strcmp(szName, CPythonPlayer::Instance().GetName()))
				CPythonMessenger::Instance().AppendGuildMember(szName);

			__RefreshTargetBoardByName(szName);

			iPacketSize -= sizeof(memberPacket);
		}

		__RefreshGuildWindowInfoPage();
		__RefreshGuildWindowMemberPage();
		__RefreshMessengerWindow();
		__RefreshCharacterWindow();
		break;
	}
	case GUILD_SUBHEADER_GC_GRADE:
	{
		BYTE byCount;
		if (!Recv(sizeof(byCount), &byCount))
			return false;

		for (BYTE i = 0; i < byCount; ++i)
		{
			BYTE byIndex;
			if (!Recv(sizeof(byCount), &byIndex))
				return false;
			TPacketGCGuildSubGrade GradePacket;
			if (!Recv(sizeof(GradePacket), &GradePacket))
				return false;

			auto aGradeData = CPythonGuild::SGuildGradeData(GradePacket.auth_flag, GradePacket.grade_name);
			CPythonGuild::Instance().SetGradeData(byIndex, aGradeData);
			//Tracef(" <Grade> [%d/%d] : %s, %d\n", byIndex, byCount, GradePacket.grade_name, GradePacket.auth_flag);
		}
		__RefreshGuildWindowGradePage();
		__RefreshGuildWindowMemberPageGradeComboBox();
		break;
	}
	case GUILD_SUBHEADER_GC_GRADE_NAME:
	{
		BYTE byGradeNumber;
		if (!Recv(sizeof(byGradeNumber), &byGradeNumber))
			return false;

		char szGradeName[GUILD_GRADE_NAME_MAX_LEN + 1] = "";
		if (!Recv(sizeof(szGradeName), &szGradeName))
			return false;

		CPythonGuild::Instance().SetGradeName(byGradeNumber, szGradeName);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildGrade", Py_BuildValue("()"));

		Tracef(" <Change Grade Name> %d, %s\n", byGradeNumber, szGradeName);
		__RefreshGuildWindowGradePage();
		__RefreshGuildWindowMemberPageGradeComboBox();
		break;
	}
	case GUILD_SUBHEADER_GC_GRADE_AUTH:
	{
		BYTE byGradeNumber;
		if (!Recv(sizeof(byGradeNumber), &byGradeNumber))
			return false;
		BYTE byAuthorityFlag;
		if (!Recv(sizeof(byAuthorityFlag), &byAuthorityFlag))
			return false;

		CPythonGuild::Instance().SetGradeAuthority(byGradeNumber, byAuthorityFlag);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshGuildGrade", Py_BuildValue("()"));

		Tracef(" <Change Grade Authority> %d, %d\n", byGradeNumber, byAuthorityFlag);
		__RefreshGuildWindowGradePage();
		break;
	}
	case GUILD_SUBHEADER_GC_INFO:
	{
		TPacketGCGuildInfo GuildInfo;
		if (!Recv(sizeof(GuildInfo), &GuildInfo))
			return false;

		CPythonGuild::Instance().EnableGuild();
		CPythonGuild::TGuildInfo& rGuildInfo = CPythonGuild::Instance().GetGuildInfoRef();
		strncpy(rGuildInfo.szGuildName, GuildInfo.name, GUILD_NAME_MAX_LEN);
		rGuildInfo.szGuildName[GUILD_NAME_MAX_LEN] = '\0';

		rGuildInfo.dwGuildID = GuildInfo.guild_id;
		rGuildInfo.dwMasterPID = GuildInfo.master_pid;
		rGuildInfo.dwGuildLevel = GuildInfo.level;
		rGuildInfo.dwCurrentExperience = GuildInfo.exp;
		rGuildInfo.dwCurrentMemberCount = GuildInfo.member_count;
		rGuildInfo.dwMaxMemberCount = GuildInfo.max_member_count;
		rGuildInfo.dwGuildMoney = GuildInfo.gold;
		rGuildInfo.bHasLand = GuildInfo.hasLand;

		//Tracef(" <Info> %s, %d, %d : %d\n", GuildInfo.name, GuildInfo.master_pid, GuildInfo.level, rGuildInfo.bHasLand);
		__RefreshGuildWindowInfoPage();
		break;
	}
	case GUILD_SUBHEADER_GC_COMMENTS:
	{
		BYTE byCount;
		if (!Recv(sizeof(byCount), &byCount))
			return false;

		CPythonGuild::Instance().ClearComment();
		//Tracef(" >>> Comments Count : %d\n", byCount);

		for (BYTE i = 0; i < byCount; ++i)
		{
			DWORD dwCommentID;
			if (!Recv(sizeof(dwCommentID), &dwCommentID))
				return false;

			char szName[CHARACTER_NAME_MAX_LEN + 1] = "";
			if (!Recv(sizeof(szName), &szName))
				return false;

			char szComment[GULID_COMMENT_MAX_LEN + 1] = "";
			if (!Recv(sizeof(szComment), &szComment))
				return false;

			//Tracef(" [Comment-%d] : %s, %s\n", dwCommentID, szName, szComment);
			CPythonGuild::Instance().RegisterComment(dwCommentID, szName, szComment);
		}

		__RefreshGuildWindowBoardPage();
		break;
	}
	case GUILD_SUBHEADER_GC_CHANGE_EXP:
	{
		BYTE byLevel;
		if (!Recv(sizeof(byLevel), &byLevel))
			return false;
		DWORD dwEXP;
		if (!Recv(sizeof(dwEXP), &dwEXP))
			return false;
		CPythonGuild::Instance().SetGuildEXP(byLevel, dwEXP);
		Tracef(" <ChangeEXP> %d, %d\n", byLevel, dwEXP);
		__RefreshGuildWindowInfoPage();
		break;
	}
	case GUILD_SUBHEADER_GC_CHANGE_MEMBER_GRADE:
	{
		DWORD dwPID;
		if (!Recv(sizeof(dwPID), &dwPID))
			return false;
		BYTE byGrade;
		if (!Recv(sizeof(byGrade), &byGrade))
			return false;
		CPythonGuild::Instance().ChangeGuildMemberGrade(dwPID, byGrade);
		Tracef(" <ChangeMemberGrade> %d, %d\n", dwPID, byGrade);
		__RefreshGuildWindowMemberPage();
		break;
	}
	case GUILD_SUBHEADER_GC_SKILL_INFO:
	{
		CPythonGuild::TGuildSkillData& rSkillData = CPythonGuild::Instance().GetGuildSkillDataRef();
		if (!Recv(sizeof(rSkillData.bySkillPoint), &rSkillData.bySkillPoint))
			return false;
		if (!Recv(sizeof(rSkillData.bySkillLevel), rSkillData.bySkillLevel))
			return false;
		if (!Recv(sizeof(rSkillData.wGuildPoint), &rSkillData.wGuildPoint))
			return false;
		if (!Recv(sizeof(rSkillData.wMaxGuildPoint), &rSkillData.wMaxGuildPoint))
			return false;

		Tracef(" <SkillInfo> %d / %d, %d\n", rSkillData.bySkillPoint, rSkillData.wGuildPoint, rSkillData.wMaxGuildPoint);
		__RefreshGuildWindowSkillPage();
		break;
	}
	case GUILD_SUBHEADER_GC_CHANGE_MEMBER_GENERAL:
	{
		DWORD dwPID;
		if (!Recv(sizeof(dwPID), &dwPID))
			return false;
		BYTE byFlag;
		if (!Recv(sizeof(byFlag), &byFlag))
			return false;

		CPythonGuild::Instance().ChangeGuildMemberGeneralFlag(dwPID, byFlag);
		Tracef(" <ChangeMemberGeneralFlag> %d, %d\n", dwPID, byFlag);
		__RefreshGuildWindowMemberPage();
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_INVITE:
	{
		DWORD dwGuildID;
		if (!Recv(sizeof(dwGuildID), &dwGuildID))
			return false;
		char szGuildName[GUILD_NAME_MAX_LEN + 1];
		if (!Recv(GUILD_NAME_MAX_LEN, &szGuildName))
			return false;

		szGuildName[GUILD_NAME_MAX_LEN] = 0;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RecvGuildInviteQuestion", Py_BuildValue("(is)", dwGuildID, szGuildName));
		Tracef(" <Guild Invite> %d, %s\n", dwGuildID, szGuildName);
		break;
	}
	case GUILD_SUBHEADER_GC_WAR:
	{
		TPacketGCGuildWar kGuildWar;
		if (!Recv(sizeof(kGuildWar), &kGuildWar))
			return false;

		switch (kGuildWar.bWarState)
		{
		case GUILD_WAR_SEND_DECLARE:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_SEND_DECLARE\n");
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnSendDeclare",
				Py_BuildValue("(i)", kGuildWar.dwGuildOpp)
			);
			break;
		case GUILD_WAR_RECV_DECLARE:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_RECV_DECLARE\n");
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnRecvDeclare",
				Py_BuildValue("(ii)", kGuildWar.dwGuildOpp, kGuildWar.bType)
			);
			break;
		case GUILD_WAR_ON_WAR:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_ON_WAR : %d, %d\n", kGuildWar.dwGuildSelf, kGuildWar.dwGuildOpp);
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnStart",
				Py_BuildValue("(ii)", kGuildWar.dwGuildSelf, kGuildWar.dwGuildOpp)
			);
			CPythonGuild::Instance().StartGuildWar(kGuildWar.dwGuildOpp);
			break;
		case GUILD_WAR_END:
			Tracef(" >> GUILD_SUBHEADER_GC_WAR : GUILD_WAR_END\n");
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
				"BINARY_GuildWar_OnEnd",
				Py_BuildValue("(ii)", kGuildWar.dwGuildSelf, kGuildWar.dwGuildOpp)
			);
			CPythonGuild::Instance().EndGuildWar(kGuildWar.dwGuildOpp);
			break;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_NAME:
	{
		DWORD dwID;
		char szGuildName[GUILD_NAME_MAX_LEN + 1];

		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);

		int nItemSize = sizeof(dwID) + GUILD_NAME_MAX_LEN;

		assert(iPacketSize % nItemSize == 0 && "GUILD_SUBHEADER_GC_GUILD_NAME");

		for (; iPacketSize > 0;)
		{
			if (!Recv(sizeof(dwID), &dwID))
				return false;

			if (!Recv(GUILD_NAME_MAX_LEN, &szGuildName))
				return false;

			szGuildName[GUILD_NAME_MAX_LEN] = 0;

			//Tracef(" >> GulidName [%d : %s]\n", dwID, szGuildName);
			CPythonGuild::Instance().RegisterGuildName(dwID, szGuildName);
			iPacketSize -= nItemSize;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_WAR_LIST:
	{
		DWORD dwSrcGuildID;
		DWORD dwDstGuildID;

		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);
		int nItemSize = sizeof(dwSrcGuildID) + sizeof(dwDstGuildID);

		assert(iPacketSize % nItemSize == 0 && "GUILD_SUBHEADER_GC_GUILD_WAR_LIST");

		for (; iPacketSize > 0;)
		{
			if (!Recv(sizeof(dwSrcGuildID), &dwSrcGuildID))
				return false;

			if (!Recv(sizeof(dwDstGuildID), &dwDstGuildID))
				return false;

			Tracef(" >> GulidWarList [%d vs %d]\n", dwSrcGuildID, dwDstGuildID);
			CInstanceBase::InsertGVGKey(dwSrcGuildID, dwDstGuildID);
			CPythonCharacterManager::Instance().ChangeGVG(dwSrcGuildID, dwDstGuildID);
			iPacketSize -= nItemSize;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST:
	{
		DWORD dwSrcGuildID;
		DWORD dwDstGuildID;

		int iPacketSize = int(GuildPacket.size) - sizeof(GuildPacket);
		int nItemSize = sizeof(dwSrcGuildID) + sizeof(dwDstGuildID);

		assert(iPacketSize % nItemSize == 0 && "GUILD_SUBHEADER_GC_GUILD_WAR_END_LIST");

		for (; iPacketSize > 0;)
		{
			if (!Recv(sizeof(dwSrcGuildID), &dwSrcGuildID))
				return false;

			if (!Recv(sizeof(dwDstGuildID), &dwDstGuildID))
				return false;

			Tracef(" >> GulidWarEndList [%d vs %d]\n", dwSrcGuildID, dwDstGuildID);
			CInstanceBase::RemoveGVGKey(dwSrcGuildID, dwDstGuildID);
			CPythonCharacterManager::Instance().ChangeGVG(dwSrcGuildID, dwDstGuildID);
			iPacketSize -= nItemSize;
		}
		break;
	}
	case GUILD_SUBHEADER_GC_WAR_POINT:
	{
		TPacketGuildWarPoint GuildWarPoint;
		if (!Recv(sizeof(GuildWarPoint), &GuildWarPoint))
			return false;

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
			"BINARY_GuildWar_OnRecvPoint",
			Py_BuildValue("(iii)", GuildWarPoint.dwGainGuildID, GuildWarPoint.dwOpponentGuildID, GuildWarPoint.lPoint)
		);
		break;
	}
	case GUILD_SUBHEADER_GC_MONEY_CHANGE:
	{
		DWORD dwMoney;
		if (!Recv(sizeof(dwMoney), &dwMoney))
			return false;

		CPythonGuild::Instance().SetGuildMoney(dwMoney);

		__RefreshGuildWindowInfoPage();
		Tracef(" >> Guild Money Change : %d\n", dwMoney);
		break;
	}
	}

	return true;
}
// Guild
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Fishing
bool CPythonNetworkStream::SendFishingPacket(int iRotation)
{
	BYTE byHeader = HEADER_CG_FISHING;
	if (!Send(sizeof(byHeader), &byHeader))
		return false;
	BYTE byPacketRotation = iRotation / 5;
	if (!Send(sizeof(BYTE), &byPacketRotation))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendGiveItemPacket(DWORD dwTargetVID, TItemPos ItemPos, int iItemCount)
{
	TPacketCGGiveItem GiveItemPacket;
	GiveItemPacket.byHeader = HEADER_CG_GIVE_ITEM;
	GiveItemPacket.dwTargetVID = dwTargetVID;
	GiveItemPacket.ItemPos = ItemPos;
	GiveItemPacket.byItemCount = iItemCount;

	if (!Send(sizeof(GiveItemPacket), &GiveItemPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::RecvFishing()
{
	TPacketGCFishing FishingPacket;
	if (!Recv(sizeof(FishingPacket), &FishingPacket))
		return false;

	CInstanceBase* pFishingInstance = NULL;
	if (FISHING_SUBHEADER_GC_FISH != FishingPacket.subheader)
	{
		pFishingInstance = CPythonCharacterManager::Instance().GetInstancePtr(FishingPacket.info);
		if (!pFishingInstance)
			return true;
	}

	switch (FishingPacket.subheader)
	{
	case FISHING_SUBHEADER_GC_START:
		pFishingInstance->StartFishing(float(FishingPacket.dir) * 5.0f);
		break;
	case FISHING_SUBHEADER_GC_STOP:
		if (pFishingInstance->IsFishing())
			pFishingInstance->StopFishing();
		break;
	case FISHING_SUBHEADER_GC_REACT:
		if (pFishingInstance->IsFishing())
		{
			pFishingInstance->SetFishEmoticon(); // Fish Emoticon
			pFishingInstance->ReactFishing();
		}
		break;
	case FISHING_SUBHEADER_GC_SUCCESS:
		pFishingInstance->CatchSuccess();
		break;
	case FISHING_SUBHEADER_GC_FAIL:
		pFishingInstance->CatchFail();
		if (pFishingInstance == CPythonCharacterManager::Instance().GetMainInstancePtr())
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingFailure", Py_BuildValue("()"));
		}
		break;
	case FISHING_SUBHEADER_GC_FISH:
	{
		DWORD dwFishID = FishingPacket.info;

		if (0 == FishingPacket.info)
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingNotifyUnknown", Py_BuildValue("()"));
			return true;
		}

		CItemData* pItemData;
		if (!CItemManager::Instance().GetItemDataPointer(dwFishID, &pItemData))
			return true;

		CInstanceBase* pMainInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
		if (!pMainInstance)
			return true;

		if (pMainInstance->IsFishing())
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingNotify", Py_BuildValue("(is)", CItemData::ITEM_TYPE_FISH == pItemData->GetType(), pItemData->GetName()));
		}
		else
		{
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OnFishingSuccess", Py_BuildValue("(is)", CItemData::ITEM_TYPE_FISH == pItemData->GetType(), pItemData->GetName()));
		}
		break;
	}
	}

	return true;
}
// Fishing
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Dungeon
bool CPythonNetworkStream::RecvDungeon()
{
	TPacketGCDungeon DungeonPacket;
	if (!Recv(sizeof(DungeonPacket), &DungeonPacket))
		return false;

	switch (DungeonPacket.subheader)
	{
	case DUNGEON_SUBHEADER_GC_TIME_ATTACK_START:
	{
		break;
	}
	case DUNGEON_SUBHEADER_GC_DESTINATION_POSITION:
	{
		unsigned long ulx, uly;
		if (!Recv(sizeof(ulx), &ulx))
			return false;
		if (!Recv(sizeof(uly), &uly))
			return false;

		CPythonPlayer::Instance().SetDungeonDestinationPosition(ulx, uly);
		break;
	}
	}

	return true;
}
// Dungeon
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// MyShop
bool CPythonNetworkStream::SendBuildPrivateShopPacket(const char* c_szName, const std::vector<TShopItemTable>& c_rSellingItemStock)
{
	TPacketCGMyShop packet;
	packet.bHeader = HEADER_CG_MYSHOP;
	strncpy(packet.szSign, c_szName, SHOP_SIGN_MAX_LEN);
	packet.bCount = static_cast<BYTE>(c_rSellingItemStock.size());

	if (!Send(sizeof(packet), &packet))
		return false;

	for (std::vector<TShopItemTable>::const_iterator itor = c_rSellingItemStock.begin(); itor < c_rSellingItemStock.end(); ++itor)
	{
		const TShopItemTable& c_rItem = *itor;
		if (!Send(sizeof(c_rItem), &c_rItem))
			return false;
	}

	return SendSequence();
}

#if defined(ENABLE_MYSHOP_DECO)
bool CPythonNetworkStream::SendMyShopDecoSet(BYTE bType, DWORD dwPolyVnum)
{
	SPacketCGMyShopDeco pkDecoShop;
	pkDecoShop.bHeader = HEADER_CG_MYSHOP_DECO;
	pkDecoShop.bType = bType;
	pkDecoShop.dwPolyVnum = dwPolyVnum;

	if (!Send(sizeof(pkDecoShop), &pkDecoShop))
	{
		Tracef("SendMyShopDecoSet Error\n");
		return false;
	}

	return SendSequence();
}
#endif

bool CPythonNetworkStream::RecvShopSignPacket()
{
	TPacketGCShopSign p;
	if (!Recv(sizeof(TPacketGCShopSign), &p))
		return false;

	CPythonPlayer& rkPlayer = CPythonPlayer::Instance();

	if (0 == strlen(p.szSign))
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_PrivateShop_Disappear", Py_BuildValue("(i)", p.dwVID));

		if (rkPlayer.IsMainCharacterIndex(p.dwVID))
			rkPlayer.ClosePrivateShop();
	}
	else
	{
#if defined(ENABLE_MYSHOP_DECO)
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_PrivateShop_Appear", Py_BuildValue("(isi)", p.dwVID, p.szSign, p.bType));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_PrivateShop_Appear", Py_BuildValue("(is)", p.dwVID, p.szSign));
#endif

		if (rkPlayer.IsMainCharacterIndex(p.dwVID))
			rkPlayer.OpenPrivateShop();
	}

	return true;
}
/////////////////////////////////////////////////////////////////////////

bool CPythonNetworkStream::RecvTimePacket()
{
	TPacketGCTime TimePacket;
	if (!Recv(sizeof(TimePacket), &TimePacket))
		return false;

	IAbstractApplication& rkApp = IAbstractApplication::GetSingleton();
	rkApp.SetServerTime(TimePacket.time);

	return true;
}

bool CPythonNetworkStream::RecvWalkModePacket()
{
	TPacketGCWalkMode WalkModePacket;
	if (!Recv(sizeof(WalkModePacket), &WalkModePacket))
		return false;

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(WalkModePacket.vid);
	if (pInstance)
	{
		if (WALKMODE_RUN == WalkModePacket.mode)
		{
			pInstance->SetRunMode();
		}
		else
		{
			pInstance->SetWalkMode();
		}
	}

	return true;
}

bool CPythonNetworkStream::RecvChangeSkillGroupPacket()
{
	TPacketGCChangeSkillGroup ChangeSkillGroup;
	if (!Recv(sizeof(ChangeSkillGroup), &ChangeSkillGroup))
		return false;

	m_dwMainActorSkillGroup = ChangeSkillGroup.skill_group;

	CPythonPlayer::Instance().NEW_ClearSkillData();
	__RefreshCharacterWindow();
	return true;
}

void CPythonNetworkStream::__TEST_SetSkillGroupFake(int iIndex)
{
	m_dwMainActorSkillGroup = DWORD(iIndex);

	CPythonPlayer::Instance().NEW_ClearSkillData();
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshCharacter", Py_BuildValue("()"));
}

bool CPythonNetworkStream::SendRefinePacket(BYTE byPos, BYTE byType)
{
	TPacketCGRefine kRefinePacket;
	kRefinePacket.header = HEADER_CG_REFINE;
	kRefinePacket.pos = byPos;
	kRefinePacket.type = byType;

	if (!Send(sizeof(kRefinePacket), &kRefinePacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendSelectItemPacket(DWORD dwItemPos)
{
	TPacketCGScriptSelectItem kScriptSelectItem;
	kScriptSelectItem.header = HEADER_CG_SCRIPT_SELECT_ITEM;
	kScriptSelectItem.selection = dwItemPos;

	if (!Send(sizeof(kScriptSelectItem), &kScriptSelectItem))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::RecvRefineInformationPacket()
{
	TPacketGCRefineInformation kRefineInfoPacket;
	if (!Recv(sizeof(kRefineInfoPacket), &kRefineInfoPacket))
		return false;

	TRefineTable& rkRefineTable = kRefineInfoPacket.refine_table;
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
		"OpenRefineDialog",
		Py_BuildValue("(iiii)",
			kRefineInfoPacket.pos,
			kRefineInfoPacket.refine_table.result_vnum,
			rkRefineTable.cost,
			rkRefineTable.prob));

	for (int i = 0; i < rkRefineTable.material_count; ++i)
	{
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AppendMaterialToRefineDialog", Py_BuildValue("(ii)", rkRefineTable.materials[i].vnum, rkRefineTable.materials[i].count));
	}

#ifdef _DEBUG
	Tracef(" >> RecvRefineInformationPacket(pos=%d, result_vnum=%d, cost=%d, prob=%d)\n",
		kRefineInfoPacket.pos,
		kRefineInfoPacket.refine_table.result_vnum,
		rkRefineTable.cost,
		rkRefineTable.prob);
#endif

	return true;
}

bool CPythonNetworkStream::RecvRefineInformationPacketNew()
{
	TPacketGCRefineInformationNew kRefineInfoPacket;
	if (!Recv(sizeof(kRefineInfoPacket), &kRefineInfoPacket))
		return false;

	TRefineTable& rkRefineTable = kRefineInfoPacket.refine_table;
#if defined(ENABLE_APPLY_RANDOM)
	PyObject* poList = PyList_New(0);
	for (uint8_t uiApplySlot = 0; uiApplySlot < ITEM_APPLY_RANDOM_SLOT_MAX_NUM; ++uiApplySlot)
	{
		PyList_Append(poList, Py_BuildValue("(ii)", kRefineInfoPacket.aApplyRandom[uiApplySlot].bType, kRefineInfoPacket.aApplyRandom[uiApplySlot].sValue));
	}
#endif

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME],
		"OpenRefineDialog",
#if defined(ENABLE_APPLY_RANDOM)
		Py_BuildValue("(iiiiiOi)",
			kRefineInfoPacket.pos,
			kRefineInfoPacket.refine_table.result_vnum,
			rkRefineTable.cost,
			rkRefineTable.prob,
			kRefineInfoPacket.type,
			poList,
			rkRefineTable.src_vnum)
#else
		Py_BuildValue("(iiiii)",
			kRefineInfoPacket.pos,
			kRefineInfoPacket.refine_table.result_vnum,
			rkRefineTable.cost,
			rkRefineTable.prob,
			kRefineInfoPacket.type)
#endif
	);

	for (int i = 0; i < rkRefineTable.material_count; ++i)
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AppendMaterialToRefineDialog", Py_BuildValue("(ii)", rkRefineTable.materials[i].vnum, rkRefineTable.materials[i].count));

#ifdef _DEBUG
	Tracef(" >> RecvRefineInformationPacketNew(pos=%d, result_vnum=%d, cost=%d, prob=%d, type=%d, src_vnum=%d)\n",
		kRefineInfoPacket.pos,
		kRefineInfoPacket.refine_table.result_vnum,
		rkRefineTable.cost,
		rkRefineTable.prob,
		kRefineInfoPacket.type
//#if defined(ENABLE_APPLY_RANDOM)
//		, 0
//		, rkRefineTable.src_vnum
//#endif
	);
#endif

	return true;
}

bool CPythonNetworkStream::RecvNPCList()
{
	TPacketGCNPCPosition kNPCPosition;
	if (!Recv(sizeof(kNPCPosition), &kNPCPosition))
		return false;

	assert(int(kNPCPosition.size) - sizeof(kNPCPosition) == kNPCPosition.count * sizeof(TNPCPosition) && "HEADER_GC_NPC_POSITION");

	CPythonMiniMap::Instance().ClearAtlasMarkInfo();

	for (int i = 0; i < kNPCPosition.count; ++i)
	{
		TNPCPosition NPCPosition;
		if (!Recv(sizeof(TNPCPosition), &NPCPosition))
			return false;

		CPythonMiniMap::Instance().RegisterAtlasMark(NPCPosition.bType, NPCPosition.name, NPCPosition.x, NPCPosition.y);
	}

	return true;
}

bool CPythonNetworkStream::__SendCRCReportPacket()
{
	/*
	DWORD dwProcessCRC = 0;
	DWORD dwFileCRC = 0;
	CFilename exeFileName;
	//LPCVOID c_pvBaseAddress = NULL;

	GetExeCRC(dwProcessCRC, dwFileCRC);

	CFilename strRootPackFileName = CEterPackManager::Instance().GetRootPacketFileName();
	strRootPackFileName.ChangeDosPath();

	TPacketCGCRCReport kReportPacket;

	kReportPacket.header = HEADER_CG_CRC_REPORT;
	kReportPacket.byPackMode = CEterPackManager::Instance().GetSearchMode();
	kReportPacket.dwBinaryCRC32 = dwFileCRC;
	kReportPacket.dwProcessCRC32 = dwProcessCRC;
	kReportPacket.dwRootPackCRC32 = GetFileCRC32(strRootPackFileName.c_str());

	if (!Send(sizeof(kReportPacket), &kReportPacket))
		Tracef("SendClientReportPacket Error");

	return SendSequence();
	*/
	return true;
}

bool CPythonNetworkStream::SendClientVersionPacket()
{
	std::string filename;

	GetExcutedFileName(filename);

	filename = CFileNameHelper::NoPath(filename);
	CFileNameHelper::ChangeDosPath(filename);

	if (LocaleService_IsEUROPE() && false == LocaleService_IsYMIR())
	{
		TPacketCGClientVersion2 kVersionPacket;
		kVersionPacket.header = HEADER_CG_CLIENT_VERSION2;
		strncpy(kVersionPacket.filename, filename.c_str(), sizeof(kVersionPacket.filename) - 1);
		strncpy(kVersionPacket.timestamp, "1215955205", sizeof(kVersionPacket.timestamp) - 1); // # python time.time 앞자리
		//strncpy(kVersionPacket.timestamp, __TIMESTAMP__, sizeof(kVersionPacket.timestamp)-1); // old_string_ver
		//strncpy(kVersionPacket.timestamp, "1218055205", sizeof(kVersionPacket.timestamp)-1); // new_future
		//strncpy(kVersionPacket.timestamp, "1214055205", sizeof(kVersionPacket.timestamp)-1); // old_past

		if (!Send(sizeof(kVersionPacket), &kVersionPacket))
			Tracef("SendClientReportPacket Error");
	}
	else
	{
		TPacketCGClientVersion kVersionPacket;
		kVersionPacket.header = HEADER_CG_CLIENT_VERSION;
		strncpy(kVersionPacket.filename, filename.c_str(), sizeof(kVersionPacket.filename) - 1);
		strncpy(kVersionPacket.timestamp, __TIMESTAMP__, sizeof(kVersionPacket.timestamp) - 1);

		if (!Send(sizeof(kVersionPacket), &kVersionPacket))
			Tracef("SendClientReportPacket Error");
	}
	return SendSequence();
}

bool CPythonNetworkStream::RecvAffectAddPacket()
{
	TPacketGCAffectAdd kAffectAdd;
	if (!Recv(sizeof(kAffectAdd), &kAffectAdd))
		return false;

	TPacketAffectElement& rkElement = kAffectAdd.elem;
	if (rkElement.bPointIdxApplyOn == POINT_ENERGY)
	{
		CPythonPlayer::instance().SetStatus(POINT_ENERGY_END_TIME, CPythonApplication::Instance().GetServerTimeStamp() + rkElement.lDuration);
		__RefreshStatus();
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_NEW_AddAffect", Py_BuildValue("(iiii)", rkElement.dwType, rkElement.bPointIdxApplyOn, rkElement.lApplyValue, rkElement.lDuration));

	CPythonPlayer::instance().AddAffect(rkElement.dwType, kAffectAdd.elem);

	return true;
}

bool CPythonNetworkStream::RecvAffectRemovePacket()
{
	TPacketGCAffectRemove kAffectRemove;
	if (!Recv(sizeof(kAffectRemove), &kAffectRemove))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_NEW_RemoveAffect", Py_BuildValue("(ii)", kAffectRemove.dwType, kAffectRemove.bApplyOn));

	CPythonPlayer::instance().RemoveAffect(kAffectRemove.dwType, kAffectRemove.bApplyOn);

	return true;
}

bool CPythonNetworkStream::RecvChannelPacket()
{
	TPacketGCChannel kChannelPacket;
	if (!Recv(sizeof(kChannelPacket), &kChannelPacket))
		return false;

	// Tracef(" >> CPythonNetworkStream::RecvChannelPacket(channel=%d)\n", kChannelPacket.channel);

	return true;
}

bool CPythonNetworkStream::RecvViewEquipPacket()
{
	TPacketGCViewEquip kViewEquipPacket;
	if (!Recv(sizeof(kViewEquipPacket), &kViewEquipPacket))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenEquipmentDialog", Py_BuildValue("(i)", kViewEquipPacket.dwVID));

	for (int i = 0; i < WEAR_MAX_NUM; ++i)
	{
		TEquipmentItemSet& rItemSet = kViewEquipPacket.equips[i];
#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetEquipmentDialogItem", Py_BuildValue("(iiiii)", kViewEquipPacket.dwVID, i, rItemSet.vnum, rItemSet.count, rItemSet.dwTransmutationVnum));
#else
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetEquipmentDialogItem", Py_BuildValue("(iiii)", kViewEquipPacket.dwVID, i, rItemSet.vnum, rItemSet.count));
#endif

		for (int j = 0; j < ITEM_SOCKET_SLOT_MAX_NUM; ++j)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetEquipmentDialogSocket", Py_BuildValue("(iiii)", kViewEquipPacket.dwVID, i, j, rItemSet.alSockets[j]));

		for (int k = 0; k < ITEM_ATTRIBUTE_SLOT_MAX_NUM; ++k)
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "SetEquipmentDialogAttr", Py_BuildValue("(iiiii)", kViewEquipPacket.dwVID, i, k, rItemSet.aAttr[k].bType, rItemSet.aAttr[k].sValue));
	}

	return true;
}

bool CPythonNetworkStream::RecvLandPacket()
{
	TPacketGCLandList kLandList;
	if (!Recv(sizeof(kLandList), &kLandList))
		return false;

	std::vector<DWORD> kVec_dwGuildID;

	CPythonMiniMap& rkMiniMap = CPythonMiniMap::Instance();
	CPythonBackground& rkBG = CPythonBackground::Instance();
	CInstanceBase* pMainInstance = CPythonPlayer::Instance().NEW_GetMainActorPtr();

	rkMiniMap.ClearGuildArea();
	rkBG.ClearGuildArea();

	int iPacketSize = (kLandList.size - sizeof(TPacketGCLandList));
	for (; iPacketSize > 0; iPacketSize -= sizeof(TLandPacketElement))
	{
		TLandPacketElement kElement;
		if (!Recv(sizeof(TLandPacketElement), &kElement))
			return false;

		rkMiniMap.RegisterGuildArea(kElement.dwID,
			kElement.dwGuildID,
			kElement.x,
			kElement.y,
			kElement.width,
			kElement.height);

		if (pMainInstance)
			if (kElement.dwGuildID == pMainInstance->GetGuildID())
			{
				rkBG.RegisterGuildArea(kElement.x,
					kElement.y,
					kElement.x + kElement.width,
					kElement.y + kElement.height);
			}

		if (0 != kElement.dwGuildID)
			kVec_dwGuildID.push_back(kElement.dwGuildID);
	}

	if (kVec_dwGuildID.size() > 0)
		__DownloadSymbol(kVec_dwGuildID);

	return true;
}

bool CPythonNetworkStream::RecvTargetCreatePacket()
{
	TPacketGCTargetCreate kTargetCreate;
	if (!Recv(sizeof(kTargetCreate), &kTargetCreate))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	rkpyMiniMap.CreateTarget(kTargetCreate.lID, kTargetCreate.szTargetName);

	//#ifdef _DEBUG
	//	char szBuf[256+1];
	//	_snprintf(szBuf, sizeof(szBuf), "타겟이 생성 되었습니다 [%d:%s]", kTargetCreate.lID, kTargetCreate.szTargetName);
	//	CPythonChat::Instance().AppendChat(CHAT_TYPE_NOTICE, szBuf);
	//	Tracef(" >> RecvTargetCreatePacket %d : %s\n", kTargetCreate.lID, kTargetCreate.szTargetName);
	//#endif

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OpenAtlasWindow", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::RecvTargetCreatePacketNew()
{
	TPacketGCTargetCreateNew kTargetCreate;
	if (!Recv(sizeof(kTargetCreate), &kTargetCreate))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	CPythonBackground& rkpyBG = CPythonBackground::Instance();
	if (CREATE_TARGET_TYPE_LOCATION == kTargetCreate.byType)
	{
		rkpyMiniMap.CreateTarget(kTargetCreate.lID, kTargetCreate.szTargetName);
	}
#if defined(ENABLE_PRIVATESHOP_SEARCH_SYSTEM)
	else if (CREATE_TARGET_TYPE_SHOP_SEARCH == kTargetCreate.byType)
	{
		rkpyMiniMap.CreateTarget(kTargetCreate.lID, kTargetCreate.szTargetName, kTargetCreate.dwVID);
		rkpyBG.CreateShopSearchTargetEffect(kTargetCreate.lID, kTargetCreate.dwVID);
	}
#endif
	else
	{
		rkpyMiniMap.CreateTarget(kTargetCreate.lID, kTargetCreate.szTargetName, kTargetCreate.dwVID);
		rkpyBG.CreateTargetEffect(kTargetCreate.lID, kTargetCreate.dwVID);
	}

	//#ifdef _DEBUG
	//	char szBuf[256+1];
	//	_snprintf(szBuf, sizeof(szBuf), "캐릭터 타겟이 생성 되었습니다 [%d:%s:%d]", kTargetCreate.lID, kTargetCreate.szTargetName, kTargetCreate.dwVID);
	//	CPythonChat::Instance().AppendChat(CHAT_TYPE_NOTICE, szBuf);
	//	Tracef(" >> RecvTargetCreatePacketNew %d : %d/%d\n", kTargetCreate.lID, kTargetCreate.byType, kTargetCreate.dwVID);
	//#endif

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OpenAtlasWindow", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::RecvTargetUpdatePacket()
{
	TPacketGCTargetUpdate kTargetUpdate;
	if (!Recv(sizeof(kTargetUpdate), &kTargetUpdate))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	rkpyMiniMap.UpdateTarget(kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);

	CPythonBackground& rkpyBG = CPythonBackground::Instance();
#if defined(ENABLE_PRIVATESHOP_SEARCH_SYSTEM)
	if (kTargetUpdate.bIsShopSearch == true)
		rkpyBG.CreateShopSearchTargetEffect(kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);
	else
		rkpyBG.CreateTargetEffect(kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);
#else
	rkpyBG.CreateTargetEffect(kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);
#endif

	//#ifdef _DEBUG
	//	char szBuf[256+1];
	//	_snprintf(szBuf, sizeof(szBuf), "타겟의 위치가 갱신 되었습니다 [%d:%d/%d]", kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);
	//	CPythonChat::Instance().AppendChat(CHAT_TYPE_NOTICE, szBuf);
	//	Tracef(" >> RecvTargetUpdatePacket %d : %d, %d\n", kTargetUpdate.lID, kTargetUpdate.lX, kTargetUpdate.lY);
	//#endif

	return true;
}

bool CPythonNetworkStream::RecvTargetDeletePacket()
{
	TPacketGCTargetDelete kTargetDelete;
	if (!Recv(sizeof(kTargetDelete), &kTargetDelete))
		return false;

	CPythonMiniMap& rkpyMiniMap = CPythonMiniMap::Instance();
	rkpyMiniMap.DeleteTarget(kTargetDelete.lID);

	CPythonBackground& rkpyBG = CPythonBackground::Instance();
	rkpyBG.DeleteTargetEffect(kTargetDelete.lID);

	//#ifdef _DEBUG
	//	Tracef(" >> RecvTargetDeletePacket %d\n", kTargetDelete.lID);
	//#endif

	return true;
}

bool CPythonNetworkStream::RecvLoverInfoPacket()
{
	TPacketGCLoverInfo kLoverInfo;
	if (!Recv(sizeof(kLoverInfo), &kLoverInfo))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_LoverInfo", Py_BuildValue("(si)", kLoverInfo.szName, kLoverInfo.byLovePoint));
#ifdef _DEBUG
	Tracef("RECV LOVER INFO : %s, %d\n", kLoverInfo.szName, kLoverInfo.byLovePoint);
#endif
	return true;
}

bool CPythonNetworkStream::RecvLovePointUpdatePacket()
{
	TPacketGCLovePointUpdate kLovePointUpdate;
	if (!Recv(sizeof(kLovePointUpdate), &kLovePointUpdate))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_UpdateLovePoint", Py_BuildValue("(i)", kLovePointUpdate.byLovePoint));
#ifdef _DEBUG
	Tracef("RECV LOVE POINT UPDATE : %d\n", kLovePointUpdate.byLovePoint);
#endif
	return true;
}

bool CPythonNetworkStream::RecvDigMotionPacket()
{
	TPacketGCDigMotion kDigMotion;
	if (!Recv(sizeof(kDigMotion), &kDigMotion))
		return false;

#ifdef _DEBUG
	Tracef(" Dig Motion [%d/%d]\n", kDigMotion.vid, kDigMotion.count);
#endif

	IAbstractCharacterManager& rkChrMgr = IAbstractCharacterManager::GetSingleton();
	CInstanceBase* pkInstMain = rkChrMgr.GetInstancePtr(kDigMotion.vid);
	CInstanceBase* pkInstTarget = rkChrMgr.GetInstancePtr(kDigMotion.target_vid);
	if (NULL == pkInstMain)
		return true;

	if (pkInstTarget)
		pkInstMain->NEW_LookAtDestInstance(*pkInstTarget);

	for (int i = 0; i < kDigMotion.count; ++i)
		pkInstMain->PushOnceMotion(CRaceMotionData::NAME_DIG);

	return true;
}

// 용혼석 강화
bool CPythonNetworkStream::SendDragonSoulRefinePacket(BYTE bRefineType, TItemPos* pos)
{
	TPacketCGDragonSoulRefine pk;
	pk.header = HEADER_CG_DRAGON_SOUL_REFINE;
	pk.bSubType = bRefineType;
	memcpy(pk.ItemGrid, pos, sizeof(TItemPos) * DS_REFINE_WINDOW_MAX_NUM);
	if (!Send(sizeof(pk), &pk))
	{
		return false;
	}
	return true;
}

#ifdef ENABLE_SKILLBOOK_COMB_SYSTEM
bool CPythonNetworkStream::SendSkillBookCombinationPacket(TItemPos* pPos, BYTE bAction)
{
	TPacketCGSkillBookCombination SkillBookCombCGPacket;
	SkillBookCombCGPacket.bHeader = HEADER_CG_SKILLBOOK_COMB;
	SkillBookCombCGPacket.bAction = bAction;
	memcpy(SkillBookCombCGPacket.CombItemGrid, pPos, sizeof(TItemPos) * SKILLBOOK_COMB_SLOT_MAX);

	if (!Send(sizeof(SkillBookCombCGPacket), &SkillBookCombCGPacket))
		return false;

	return true;
}
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
bool CPythonNetworkStream::RecvAccePacket(bool bReturn)
{
	TPacketAcce sPacket;
	if (!Recv(sizeof(sPacket), &sPacket))
		return bReturn;

	bReturn = true;
	switch (sPacket.subheader)
	{
	case ACCE_SUBHEADER_GC_OPEN:
	{
		CPythonAcce::Instance().Clear();
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 1, sPacket.bWindow));
	}
	break;
	case ACCE_SUBHEADER_GC_CLOSE:
	{
		CPythonAcce::Instance().Clear();
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 2, sPacket.bWindow));
	}
	break;
	case ACCE_SUBHEADER_GC_ADDED:
	{
		CPythonAcce::Instance().AddMaterial(sPacket.dwPrice, sPacket.bPos, sPacket.tPos);
		if (sPacket.bPos == 1)
		{
			CPythonAcce::Instance().AddResult(sPacket.dwItemVnum, sPacket.dwMinAbs, sPacket.dwMaxAbs);
			PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "AlertAcce", Py_BuildValue("(b)", sPacket.bWindow));
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 3, sPacket.bWindow));
	}
	break;
	case ACCE_SUBHEADER_GC_REMOVED:
	{
		CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, sPacket.bPos);
		if (sPacket.bPos == 0)
			CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, 1);

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 4, sPacket.bWindow));
	}
	break;
	case ACCE_SUBHEADER_CG_REFINED:
	{
		if (sPacket.dwMaxAbs == 0)
			CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, 1);
		else
		{
			CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, 0);
			CPythonAcce::Instance().RemoveMaterial(sPacket.dwPrice, 1);
		}

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "ActAcce", Py_BuildValue("(ib)", 4, sPacket.bWindow));
	}
	break;
	default:
		TraceError("CPythonNetworkStream::RecvAccePacket: unknown subheader %d\n.", sPacket.subheader);
		break;
	}

	return bReturn;
}

bool CPythonNetworkStream::SendAcceClosePacket()
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAcce sPacket;
	sPacket.header = HEADER_CG_ACCE;
	sPacket.subheader = ACCE_SUBHEADER_CG_CLOSE;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendAcceAddPacket(TItemPos tPos, BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TPacketAcce sPacket;
	sPacket.header = HEADER_CG_ACCE;
	sPacket.subheader = ACCE_SUBHEADER_CG_ADD;
	sPacket.dwPrice = 0;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendAcceRemovePacket(BYTE bPos)
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAcce sPacket;
	sPacket.header = HEADER_CG_ACCE;
	sPacket.subheader = ACCE_SUBHEADER_CG_REMOVE;
	sPacket.dwPrice = 0;
	sPacket.bPos = bPos;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendAcceRefinePacket()
{
	if (!__CanActMainInstance())
		return true;

	TItemPos tPos;
	tPos.window_type = INVENTORY;
	tPos.cell = 0;

	TPacketAcce sPacket;
	sPacket.header = HEADER_CG_ACCE;
	sPacket.subheader = ACCE_SUBHEADER_CG_REFINE;
	sPacket.dwPrice = 0;
	sPacket.bPos = 0;
	sPacket.tPos = tPos;
	sPacket.dwItemVnum = 0;
	sPacket.dwMinAbs = 0;
	sPacket.dwMaxAbs = 0;
	if (!Send(sizeof(sPacket), &sPacket))
		return false;

	return SendSequence();
}
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
bool CPythonNetworkStream::SendMiniGameCatchKing(BYTE bSubHeader, BYTE bSubArgument)
{
	if (!__CanActMainInstance())
		return true;

	TPacketCGMiniGameCatchKing packet;
	packet.bHeader = HEADER_CG_MINI_GAME_CATCH_KING;
	packet.bSubheader = bSubHeader;
	packet.bSubArgument = bSubArgument;

	if (!Send(sizeof(TPacketCGMiniGameCatchKing), &packet))
	{
		Tracef("SendMiniGameCatchKing Send Packet Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RecvMiniGameCatchKingPacket()
{
	std::vector<char> vecBuffer;
	vecBuffer.clear();

	TPacketGCMiniGameCatchKing packet;
	if (!Recv(sizeof(packet), &packet))
		return false;

	int iSize = packet.wSize - sizeof(packet);
	if (iSize > 0)
	{
		vecBuffer.resize(iSize);
		if (!Recv(iSize, &vecBuffer[0]))
			return false;
	}

	switch (packet.bSubheader)
	{
	case SUBHEADER_GC_CATCH_KING_EVENT_INFO:
	{
		bool bIsEnable = *(bool*)&vecBuffer[0];
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingEvent", Py_BuildValue("(b)", bIsEnable));
	}
	break;

	case SUBHEADER_GC_CATCH_KING_START:
	{
		DWORD dwBigScore = *(DWORD*)&vecBuffer[0];
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingEventStart", Py_BuildValue("(i)", dwBigScore));
	}
	break;

	case SUBHEADER_GC_CATCH_KING_SET_CARD:
	{
		BYTE bCardNumber = *(BYTE*)&vecBuffer[0];
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingSetHandCard", Py_BuildValue("(i)", bCardNumber));
	}
	break;

	case SUBHEADER_GC_CATCH_KING_RESULT_FIELD:
	{
		TPacketGCMiniGameCatchKingResult* packSecond = (TPacketGCMiniGameCatchKingResult*)&vecBuffer[0];

		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingResultField", Py_BuildValue("(iiiibbbb)",
			packSecond->dwPoints, packSecond->bRowType, packSecond->bCardPos, packSecond->bCardValue,
			packSecond->bKeepFieldCard, packSecond->bDestroyHandCard, packSecond->bGetReward, packSecond->bIsFiveNearBy));
	}
	break;

	case SUBHEADER_GC_CATCH_KING_SET_END_CARD:
	{
		TPacketGCMiniGameCatchKingSetEndCard* packSecond = (TPacketGCMiniGameCatchKingSetEndCard*)&vecBuffer[0];
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingSetEndCard", Py_BuildValue("(ii)", packSecond->bCardPos, packSecond->bCardValue));
	}
	break;

	case SUBHEADER_GC_CATCH_KING_REWARD:
	{
		BYTE bReturnCode = *(BYTE*)&vecBuffer[0];
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MiniGameCatchKingReward", Py_BuildValue("(i)", bReturnCode));
	}
	break;

	default:
		TraceError("CPythonNetworkStream::RecvMiniGameCatchKingPacket: Unknown subheader\n");
		break;
	}

	return true;
}
#endif

#if defined(ENABLE_PRIVATESHOP_SEARCH_SYSTEM)
bool CPythonNetworkStream::RecvPrivateShopSearch()
{
	TPacketGCPrivateShopSearch p;
	if (!Recv(sizeof(p), &p))
		return false;

	CPythonShop::Instance().ClearShopSearchData();
	unsigned int iPacketSize = (p.size - sizeof(TPacketGCPrivateShopSearch));
	for (; iPacketSize > 0; iPacketSize -= sizeof(TPacketGCPrivateShopSearchItem))
	{
		TPacketGCPrivateShopSearchItem Item;
		if (!Recv(sizeof(TPacketGCPrivateShopSearchItem), &Item))
			return false;

		ShopSearchData* SShopSearch = new ShopSearchData();
		SShopSearch->item = Item.item;
		SShopSearch->name.assign(Item.szSellerName);
		SShopSearch->dwShopPID = Item.dwShopPID;
		CPythonShop::Instance().SetShopSearchItemData(SShopSearch);
	}

	CPythonShop::Instance().SortShopSearchData();
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshPShopSearchDialog", Py_BuildValue("()"));
	return true;
}

bool CPythonNetworkStream::RecvPrivateShopSearchOpen()
{
	TPacketGCPrivateShopSearchOpen p;
	if (!Recv(sizeof(p), &p))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "OpenPShopSearchDialogCash", Py_BuildValue("()"));
	return true;
}

void CPythonNetworkStream::PrivateShopSearchChangePage(int iPage)
{
	CPythonShop::Instance().ShopSearchChangePage(iPage);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "RefreshPShopSearchDialog", Py_BuildValue("()"));
}

bool CPythonNetworkStream::SendPrivateShopSearchInfoPacket(BYTE bJob, BYTE bMaskType, int iMaskSub, int iMinRefine, int iMaxRefine, int iMinLevel, int iMaxLevel, int iMinGold, int iMaxGold, const char* szItemName
#if defined(ENABLE_CHEQUE_SYSTEM)
	, int iMinCheque, int iMaxCheque
#endif
)
{
	TPacketCGPrivateShopSearch packet;
	packet.header = HEADER_CG_PRIVATESHOP_SEARCH;
	packet.bJob = bJob;
	packet.bMaskType = bMaskType;
	packet.iMaskSub = iMaskSub;
	packet.iMinRefine = iMinRefine;
	packet.iMaxRefine = iMaxRefine;
	packet.iMinLevel = iMinLevel;
	packet.iMaxLevel = iMaxLevel;
	packet.iMinGold = iMinGold;
	packet.iMaxGold = iMaxGold;
	std::strcpy(packet.szItemName, szItemName);
#if defined(ENABLE_CHEQUE_SYSTEM)
	packet.iMinCheque = iMinCheque;
	packet.iMaxCheque = iMaxCheque;
#endif

	if (!Send(sizeof(packet), &packet))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::SendPrivateShopSearchClose()
{
	TPacketCGPrivateShopSearchClose packet;
	packet.header = HEADER_CG_PRIVATESHOP_SEARCH_CLOSE;
	if (!Send(sizeof(packet), &packet))
		return false;

	CPythonShop::Instance().ClearShopSearchData();
	return SendSequence();
}

bool CPythonNetworkStream::SendPrivateShopSearchBuyItem(int iIndex)
{
	auto Item = CPythonShop::Instance().GetShopSearchItemData(iIndex);
	if (Item == nullptr)
		return false;

	TPacketCGPrivateShopSearchBuyItem packet;
	packet.header = HEADER_CG_PRIVATESHOP_SEARCH_BUY_ITEM;
	packet.pos = Item->item.display_pos;
	packet.dwShopPID = Item->dwShopPID;
	if (!Send(sizeof(packet), &packet))
		return false;

	return SendSequence();
}
#endif

bool CPythonNetworkStream::SendChangeLanguagePacket(BYTE bLanguage)
{
	if (!__CanActMainInstance())
		return true;

	if (m_bLanguageSet != FALSE)
		return true;

	TPacketChangeLanguage packet;
	packet.bHeader = HEADER_CG_CHANGE_LANGUAGE;
	packet.bLanguage = bLanguage;

	m_bLanguageSet = TRUE;

	if (!Send(sizeof(packet), &packet))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::RecvRequestChangeLanguage()
{
	TPacketChangeLanguage packet;
	if (!Recv(sizeof(packet), &packet))
		return false;

	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	CInstanceBase* pkInstMain = rkChrMgr.GetMainInstancePtr();

	if (!pkInstMain)
	{
		TraceError("CPythonNetworkStream::RecvRequestChangeLanguage - MainCharacter is NULL");
		return false;
	}

	pkInstMain->SetLanguage(packet.bLanguage);
	pkInstMain->Update();

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_RequestChangeLanguage", Py_BuildValue("(i)", packet.bLanguage));

	return true;
}

bool CPythonNetworkStream::SendWhisperDetails(const char* name)
{
	TPacketCGWhisperDetails packet;
	packet.header = HEADER_CG_WHISPER_DETAILS;
	strncpy(packet.name, name, sizeof(packet.name) - 1);
	if (!Send(sizeof(packet), &packet))
		return false;

	return SendSequence();
}

bool CPythonNetworkStream::RecvWhisperDetails()
{
	TPacketGCWhisperDetails packet;
	if (!Recv(sizeof(packet), &packet))
		return false;

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_RecieveWhisperDetails", Py_BuildValue("(si)", packet.name, packet.bLanguage));
	return true;
}

#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
#define ChangeLookPacket(x) TPacketCGChangeLook Packet(static_cast<std::underlying_type_t<EPacketCGChangeLookSubHeader>>(EPacketCGChangeLookSubHeader::x))

bool CPythonNetworkStream::SendChangeLookCheckInPacket(const TItemPos& c_rItemPos, const BYTE c_bSlotIndex)
{
	ChangeLookPacket(ITEM_CHECK_IN);
	Packet.bSlotIndex = c_bSlotIndex;
	Packet.ItemPos = c_rItemPos;
	if (!Send(sizeof(Packet), &Packet))
	{
		Tracef("CPythonNetworkStream::SendChangeLookCheckInPacket Error\n");
		return false;
	}
	return SendSequence();
}

bool CPythonNetworkStream::SendChangeLookCheckOutPacket(const BYTE c_bSlotIndex)
{
	ChangeLookPacket(ITEM_CHECK_OUT);
	Packet.bSlotIndex = c_bSlotIndex;
	if (!Send(sizeof(Packet), &Packet))
	{
		Tracef("CPythonNetworkStream::SendChangeLookCheckOutPacket Error\n");
		return false;
	}
	return SendSequence();
}

bool CPythonNetworkStream::SendChangeLookCheckInFreeYangItemPacket(const TItemPos& c_rItemPos)
{
	ChangeLookPacket(FREE_ITEM_CHECK_IN);
	Packet.ItemPos = c_rItemPos;
	if (!Send(sizeof(Packet), &Packet))
	{
		Tracef("CPythonNetworkStream::SendChangeLookCheckInFreeYangItemPacket Error\n");
		return false;
	}
	return SendSequence();
}

bool CPythonNetworkStream::SendChangeLookCheckOutFreeYangItemPacket()
{
	ChangeLookPacket(FREE_ITEM_CHECK_OUT);
	if (!Send(sizeof(Packet), &Packet))
	{
		Tracef("CPythonNetworkStream::SendChangeLookCheckOutFreeYangItemPacket Error\n");
		return false;
	}
	return SendSequence();
}

bool CPythonNetworkStream::SendChangeLookAcceptPacket()
{
	ChangeLookPacket(ACCEPT);
	if (!Send(sizeof(Packet), &Packet))
	{
		Tracef("CPythonNetworkStream::SendChangeLookAcceptPacket Error\n");
		return false;
	}
	return SendSequence();
}

bool CPythonNetworkStream::SendChangeLookCancelPacket()
{
	ChangeLookPacket(CANCEL);
	if (!Send(sizeof(Packet), &Packet))
	{
		Tracef("CPythonNetworkStream::SendChangeLookCancelPacket Error\n");
		return false;
	}
	return SendSequence();
}

bool CPythonNetworkStream::RecvChangeLookItemSetPacket()
{
	TPacketGCChangeLookSet kItemSet;
	if (!Recv(sizeof(kItemSet), &kItemSet))
		return false;

	if (kItemSet.bSlotIndex >= static_cast<BYTE>(EChangeLookSlots::CHANGE_LOOK_SLOT_MAX))
		return true;

	CPythonPlayer::Instance().SetChangeLookItemData(kItemSet);
	__RefreshInventoryWindow();

	return true;
}

bool CPythonNetworkStream::RecvChangeLookItemDelPacket()
{
	TPacketGCChangeLookDel kItemDel;
	if (!Recv(sizeof(kItemDel), &kItemDel))
		return false;

	CPythonPlayer::Instance().DelChangeLookItemData(kItemDel.bSlotIndex, true);
	__RefreshInventoryWindow();

	return true;
}

bool CPythonNetworkStream::RecvChangeLookFreeItemSetPacket()
{
	TPacketGCChangeLookSet kItemSet;
	if (!Recv(sizeof(kItemSet), &kItemSet))
		return false;

	CPythonPlayer::Instance().SetChangeLookFreeItemData(kItemSet);
	__RefreshInventoryWindow();

	return true;
}

bool CPythonNetworkStream::RecvChangeLookFreeItemDelPacket()
{
	TPacketGCChangeLookDel kItemDel;
	if (!Recv(sizeof(kItemDel), &kItemDel))
		return false;

	CPythonPlayer::Instance().DelChangeLookFreeItemData(true);
	__RefreshInventoryWindow();

	return true;
}
#endif

#if defined(ENABLE_MAILBOX)
bool CPythonNetworkStream::RecvMailboxProcess()
{
	TPacketMailboxProcess p;

	if (!Recv(sizeof(p), &p))
		return false;

	switch (p.bSubHeader)
	{
	case CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_CLOSE:
		CPythonMailBox::Instance().Destroy();
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MailBoxProcess", Py_BuildValue("(ii)", p.bSubHeader, p.bArg1));
		break;
	case CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_OPEN:
	case CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_POST_WRITE_CONFIRM:
	case CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_POST_WRITE:
	case CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_POST_ALL_DELETE:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MailBoxProcess", Py_BuildValue("(ii)", p.bSubHeader, p.bArg1));
		break;
	case CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_ADD_DATA:
	case CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_POST_DELETE:
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MailBoxProcess", Py_BuildValue("(i(ii))", p.bSubHeader, p.bArg1, p.bArg2));
		break;
	case CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_POST_GET_ITEMS:
		CPythonMailBox::Instance().ResetAddData(p.bArg1);
		PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MailBoxProcess", Py_BuildValue("(i(ii))", p.bSubHeader, p.bArg1, p.bArg2));
		break;
	default:
		TraceError("CPythonNetworkStream::RecvMailboxProcess: Unknown subheader: %d\n", p.bSubHeader);
		break;
	}

	return true;
}

bool CPythonNetworkStream::RecvMailbox()
{
	TPacketGCMailBox p;
	if (!Recv(sizeof(p), &p))
		return false;

	CPythonMailBox::Instance().Destroy();
	
	unsigned int iPacketSize = (p.wSize - sizeof(TPacketGCMailBox));
	for (; iPacketSize > 0; iPacketSize -= sizeof(TPacketGCMailBoxMessage))
	{
		TPacketGCMailBoxMessage Message;
		if (!Recv(sizeof(Message), &Message))
			return false;

		CPythonMailBox::Instance().AddMail(new CPythonMailBox::SMailBox(Message.SendTime, Message.DeleteTime, Message.szTitle, Message.bIsGMPost, Message.bIsItemExist, Message.bIsConfirm));
	}

	return true;
}

bool CPythonNetworkStream::RecvMailboxAddData()
{
	TPacketGCMailBoxAddData p;

	if (!Recv(sizeof(p), &p))
		return false;

	CPythonMailBox::SMailBox* _Data = CPythonMailBox::Instance().GetMail(p.Index);
	if (_Data == nullptr)
	{
		Tracef("RecvMailboxAddData Error: SMailBox is null.\n");
		return true;
	}

	CPythonMailBox::SMailBoxAddData*& _AddData = _Data->AddData;
	if (_AddData != nullptr)
	{
		Tracef("RecvMailboxAddData Error: SMailBoxAddData is not null.\n");
		return true;
	}

	_Data->bIsConfirm = true;
#if defined(ENABLE_APPLY_RANDOM)
	_AddData = new CPythonMailBox::SMailBoxAddData(p.szFrom, p.szMessage, p.iYang, p.iWon, p.ItemVnum, p.ChangeLookVnum, p.ItemCount, p.alSockets, p.aAttr, p.aApplyRandom);
#else
	_AddData = new CPythonMailBox::SMailBoxAddData(p.szFrom, p.szMessage, p.iYang, p.iWon, p.ItemVnum, p.ChangeLookVnum, p.ItemCount, p.alSockets, p.aAttr);
#endif
	return true;
}

bool CPythonNetworkStream::RecvMailboxAll()
{
	TPacketGCMailBox p;
	if (!Recv(sizeof(p), &p))
		return false;

	PyObject* list = PyList_New(0);

	unsigned int iPacketSize = (p.wSize - sizeof(TPacketGCMailBox));
	for (; iPacketSize > 0; iPacketSize -= sizeof(TPacketGCMailboxProcessAll))
	{
		TPacketGCMailboxProcessAll Index;
		if (!Recv(sizeof(Index), &Index))
			return false;

		CPythonMailBox::Instance().ResetAddData(Index.Index);
		PyList_Append(list, Py_BuildValue("i", Index.Index));
	}

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MailBoxProcess", Py_BuildValue("i(iO)",
		CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_POST_ALL_GET_ITEMS, CPythonMailBox::EMAILBOX_POST_ALL_GET_ITEMS::POST_ALL_GET_ITEMS_OK, list));

	Py_DECREF(list);
	return true;
}

bool CPythonNetworkStream::RecvMailboxUnread()
{
	TMailBoxRespondUnreadData p;

	if (!Recv(sizeof(p), &p))
		return false;

	const bool bFlash = p.bItemMessageCount > 0;
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "MailBoxProcess", Py_BuildValue("i(iiiii)", CPythonMailBox::EMAILBOX_GC::MAILBOX_GC_UNREAD_DATA, 
		bFlash, (p.bItemMessageCount + p.bCommonMessageCount), p.bItemMessageCount, p.bCommonMessageCount, p.bGMVisible));

	return true;
}

bool CPythonNetworkStream::SendPostWriteConfirm(const char* szName)
{
	TPacketCGMailboxWriteConfirm p;
	p.bHeader = HEADER_CG_MAILBOX_WRITE_CONFIRM;
	std::strcpy(p.szName, szName);

	if (!Send(sizeof(p), &p))
	{
		Tracef("SendPostWriteConfirm Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendMailBoxClose()
{
	TPacketMailboxProcess p;
	p.bHeader = HEADER_CG_MAILBOX_PROCESS;
	p.bSubHeader = CPythonMailBox::EMAILBOX_CG::MAILBOX_CG_CLOSE;

	if (!Send(sizeof(p), &p))
	{
		Tracef("SendMailBoxClose Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendPostDelete(const BYTE Index)
{
	TPacketMailboxProcess p;
	p.bHeader = HEADER_CG_MAILBOX_PROCESS;
	p.bSubHeader = CPythonMailBox::EMAILBOX_CG::MAILBOX_CG_DELETE;
	p.bArg1 = Index;

	if (!Send(sizeof(p), &p))
	{
		Tracef("SendPostDelete Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendPostAllDelete()
{
	TPacketMailboxProcess p;
	p.bHeader = HEADER_CG_MAILBOX_PROCESS;
	p.bSubHeader = CPythonMailBox::EMAILBOX_CG::MAILBOX_CG_ALL_DELETE;

	if (!Send(sizeof(p), &p))
	{
		Tracef("SendPostAllDelete Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendPostGetItems(const BYTE Index)
{
	TPacketMailboxProcess p;
	p.bHeader = HEADER_CG_MAILBOX_PROCESS;
	p.bSubHeader = CPythonMailBox::EMAILBOX_CG::MAILBOX_CG_GET_ITEMS;
	p.bArg1 = Index;

	if (!Send(sizeof(p), &p))
	{
		Tracef("SendPostGetItems Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendPostAllGetItems()
{
	TPacketMailboxProcess p;
	p.bHeader = HEADER_CG_MAILBOX_PROCESS;
	p.bSubHeader = CPythonMailBox::EMAILBOX_CG::MAILBOX_CG_ALL_GET_ITEMS;

	if (!Send(sizeof(p), &p))
	{
		Tracef("SendPostAllGetItems Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::RequestPostAddData(const BYTE ButtonIndex, const BYTE DataIndex)
{
	TPacketMailboxProcess p;
	p.bHeader = HEADER_CG_MAILBOX_PROCESS;
	p.bSubHeader = CPythonMailBox::EMAILBOX_CG::MAILBOX_CG_ADD_DATA;
	p.bArg1 = ButtonIndex;
	p.bArg2 = DataIndex;

	if (!Send(sizeof(p), &p))
	{
		Tracef("RequestPostAddData Error\n");
		return false;
	}

	return SendSequence();
}

bool CPythonNetworkStream::SendPostWrite(const char* szName, const char* szTitle, const char* szMessage, const TItemPos& pos, const int iYang, const int iWon)
{
	TPacketCGMailboxWrite p;
	p.bHeader = HEADER_CG_MAILBOX_WRITE;
	std::strcpy(p.szName, szName);
	std::strcpy(p.szTitle, szTitle);
	std::strcpy(p.szMessage, szMessage);
	p.pos = pos;
	p.iYang = iYang;
	p.iWon = iWon;

	if (!Send(sizeof(p), &p))
	{
		Tracef("SendPostWrite Error\n");
		return false;
	}

	return SendSequence();
}
#endif
