#include "stdafx.h"
#include "constants.h"
#include "gm.h"
#include "messenger_manager.h"
#include "buffer_manager.h"
#include "desc_client.h"
#include "log.h"
#include "config.h"
#include "p2p.h"
#include "crc32.h"
#include "char.h"
#include "char_manager.h"
#include "questmanager.h"

MessengerManager::MessengerManager()
{
}

MessengerManager::~MessengerManager()
{
}

void MessengerManager::Initialize()
{
}

void MessengerManager::Destroy()
{
}

void MessengerManager::P2PLogin(const std::string& account)
{
	Login(account);
}

void MessengerManager::P2PLogout(const std::string& account)
{
	Logout(account);
}

void MessengerManager::Login(const std::string& account)
{
	if (m_set_loginAccount.find(account) != m_set_loginAccount.end())
		return;

	DBManager::instance().FuncQuery(std::bind(&MessengerManager::LoadList, this, std::placeholders::_1),
		"SELECT `account`, `companion` FROM messenger_list%s WHERE `account` = '%s'", get_table_postfix(), account.c_str());

#if defined(__MESSENGER_GM__)
	DBManager::instance().FuncQuery(std::bind(&MessengerManager::LoadGMList, this, std::placeholders::_1),
		"SELECT '%s' AS `account`, `mName` AS `companion`, '0000-00-00 00:00:00' AS `time` FROM common.gmlist%s WHERE `mAuthority` = 'IMPLEMENTOR' OR mAuthority = 'HIGH_WIZARD' OR mAuthority = 'GOD' OR mAuthority = 'LOW_WIZARD'", account.c_str(), get_table_postfix());
#endif

#if defined(__MESSENGER_BLOCK_SYSTEM__)
	DBManager::instance().FuncQuery(std::bind(&MessengerManager::LoadBlockList, this, std::placeholders::_1),
		"SELECT `account`, `companion` FROM messenger_block_list%s WHERE `account` = '%s'", get_table_postfix(), account.c_str());
#endif

	m_set_loginAccount.insert(account);
}

void MessengerManager::Logout(const std::string& account)
{
	if (m_set_loginAccount.find(account) == m_set_loginAccount.end())
		return;

	m_set_loginAccount.erase(account);

	for (KeyTSet::iterator it = m_InverseRelation[account].begin(); it != m_InverseRelation[account].end(); ++it)
		SendLogout(*it, account);

	KeyTRelation::iterator itTRelation = m_Relation.begin();
	while (itTRelation != m_Relation.end())
	{
		itTRelation->second.erase(account);
		++itTRelation;
	}
	m_Relation.erase(account);

#if defined(__MESSENGER_GM__)
	for (KeyGSet::iterator it = m_InverseGMRelation[account].begin(); it != m_InverseGMRelation[account].end(); ++it)
		SendGMLogout(*it, account);

	KeyGRelation::iterator itGRelation = m_GMRelation.begin();
	while (itGRelation != m_GMRelation.end())
	{
		itGRelation->second.erase(account);
		++itGRelation;
	}
	m_GMRelation.erase(account);
#endif

#if defined(__MESSENGER_BLOCK_SYSTEM__)
	for (KeyBSet::iterator it = m_InverseBlockRelation[account].begin(); it != m_InverseBlockRelation[account].end(); ++it)
		SendBlockLogout(*it, account);

	KeyBRelation::iterator itBRelation = m_BlockRelation.begin();
	while (itBRelation != m_BlockRelation.end())
	{
		itBRelation->second.erase(account);
		++itBRelation;
	}
	m_BlockRelation.erase(account);
#endif
}

void MessengerManager::LoadList(SQLMsg* msg)
{
	if (NULL == msg)
		return;

	if (NULL == msg->Get())
		return;

	if (msg->Get()->uiNumRows == 0)
		return;

	std::string account;

	sys_log(1, "Messenger::LoadList");

	for (uint i = 0; i < msg->Get()->uiNumRows; ++i)
	{
		MYSQL_ROW row = mysql_fetch_row(msg->Get()->pSQLResult);

		if (row[0] && row[1])
		{
			if (account.length() == 0)
				account = row[0];

			m_Relation[row[0]].insert(row[1]);
			m_InverseRelation[row[1]].insert(row[0]);
		}
	}

	SendList(account);

	for (KeyTSet::iterator it = m_InverseRelation[account].begin(); it != m_InverseRelation[account].end(); ++it)
		SendLogin(*it, account);
}

void MessengerManager::RequestToAdd(LPCHARACTER ch, LPCHARACTER target)
{
	if (!ch->IsPC() || !target->IsPC())
		return;

	if (quest::CQuestManager::instance().GetPCForce(ch->GetPlayerID())->IsRunning() == true)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상대방이 친구 추가를 받을 수 없는 상태입니다."));
		return;
	}

	if (quest::CQuestManager::instance().GetPCForce(target->GetPlayerID())->IsRunning() == true)
		return;

	DWORD dw1 = GetCRC32(ch->GetName(), strlen(ch->GetName()));
	DWORD dw2 = GetCRC32(target->GetName(), strlen(target->GetName()));

	char buf[64];
	snprintf(buf, sizeof(buf), "%u:%u", dw1, dw2);
	DWORD dwComplex = GetCRC32(buf, strlen(buf));

	m_set_requestToAdd.insert(dwComplex);

	target->ChatPacket(CHAT_TYPE_COMMAND, "messenger_auth %s", ch->GetName());
}

void MessengerManager::AuthToAdd(const std::string& account, const std::string& companion, bool bDeny)
{
	DWORD dw1 = GetCRC32(companion.c_str(), companion.length());
	DWORD dw2 = GetCRC32(account.c_str(), account.length());

	char buf[64];
	snprintf(buf, sizeof(buf), "%u:%u", dw1, dw2);
	DWORD dwComplex = GetCRC32(buf, strlen(buf));

	if (m_set_requestToAdd.find(dwComplex) == m_set_requestToAdd.end())
	{
		sys_log(0, "MessengerManager::AuthToAdd : request not exist %s -> %s", companion.c_str(), account.c_str());
		return;
	}

	m_set_requestToAdd.erase(dwComplex);

	if (!bDeny)
	{
		AddToList(companion, account);
		AddToList(account, companion);
	}
}

void MessengerManager::__AddToList(const std::string& account, const std::string& companion)
{
	m_Relation[account].insert(companion);
	m_InverseRelation[companion].insert(account);

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (d)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<메신져> %s 님을 친구로 추가하였습니다."), companion.c_str());
	}

	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(companion.c_str());

	if (tch)
		SendLogin(account, companion);
	else
		SendLogout(account, companion);
}

void MessengerManager::AddToList(const std::string& account, const std::string& companion)
{
	if (companion.empty())
		return;

	if (m_Relation[account].find(companion) != m_Relation[account].end())
		return;

	sys_log(0, "Messenger Add %s %s", account.c_str(), companion.c_str());
	DBManager::instance().Query("INSERT INTO messenger_list%s VALUES ('%s', '%s')",
		get_table_postfix(), account.c_str(), companion.c_str());

	__AddToList(account, companion);

	TPacketGGMessenger p2ppck;

	p2ppck.bHeader = HEADER_GG_MESSENGER_ADD;
	strlcpy(p2ppck.szAccount, account.c_str(), sizeof(p2ppck.szAccount));
	strlcpy(p2ppck.szCompanion, companion.c_str(), sizeof(p2ppck.szCompanion));
	P2P_MANAGER::instance().Send(&p2ppck, sizeof(TPacketGGMessenger));
}

void MessengerManager::__RemoveFromList(const std::string& account, const std::string& companion)
{
	m_Relation[account].erase(companion);
	m_InverseRelation[companion].erase(account);

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (d)
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<메신져> %s 님을 메신저에서 삭제하였습니다."), companion.c_str());
}

bool MessengerManager::IsInList(const std::string& account, const std::string& companion)
{
	if (m_Relation.find(account) == m_Relation.end())
		return false;

	if (m_Relation[account].empty())
		return false;

	return m_Relation[account].find(companion) != m_Relation[account].end();
}

bool MessengerManager::IsFriend(const char* c_szAccount, const char* c_szName)
{
	if (m_Relation.empty())
		return false;

	std::string strAccount(c_szAccount), strName(c_szName);
	for (KeyBSet::iterator it = m_Relation[strAccount].begin(); it != m_Relation[strAccount].end(); ++it)
	{
		if (it->compare(strName) == 0)
			return true;
	}

	return false;
}

void MessengerManager::RemoveFromList(const std::string& account, const std::string& companion)
{
	if (companion.empty())
		return;

	if (!IsInList(account, companion))
	{
		LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
		LPDESC d = ch ? ch->GetDesc() : NULL;

		if (d)
			ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s 님은 접속되 있지 않습니다."), companion.c_str());

		return;
	}

	char szCompanion[CHARACTER_NAME_MAX_LEN + 1];
	DBManager::instance().EscapeString(szCompanion, sizeof(szCompanion), companion.c_str(), companion.length());
	DBManager::instance().Query("DELETE FROM messenger_list%s WHERE `account` = '%s' AND `companion` = '%s'",
		get_table_postfix(), account.c_str(), szCompanion);

	__RemoveFromList(account, companion);

	sys_log(1, "Messenger Remove %s %s", account.c_str(), companion.c_str());

	TPacketGGMessenger p2ppck;
	p2ppck.bHeader = HEADER_GG_MESSENGER_REMOVE;
	strlcpy(p2ppck.szAccount, account.c_str(), sizeof(p2ppck.szAccount));
	strlcpy(p2ppck.szCompanion, companion.c_str(), sizeof(p2ppck.szCompanion));
	P2P_MANAGER::instance().Send(&p2ppck, sizeof(TPacketGGMessenger));
}

void MessengerManager::RemoveAllList(const std::string& account)
{
	std::set<keyT> company(m_Relation[account]);

	/* SQL Data 삭제 */
	DBManager::instance().Query("DELETE FROM messenger_list%s WHERE account='%s' OR companion='%s'",
		get_table_postfix(), account.c_str(), account.c_str());

	/* 내가 가지고있는 리스트 삭제 */
	for (std::set<keyT>::iterator iter = company.begin();
		iter != company.end();
		iter++)
	{
		this->RemoveFromList(account, *iter);
	}

	/* 복사한 데이타 삭제 */
	for (std::set<keyT>::iterator iter = company.begin();
		iter != company.end();
		)
	{
		company.erase(iter++);
	}

	company.clear();
}

void MessengerManager::SendList(const std::string& account)
{
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());

	if (!ch)
		return;

	LPDESC d = ch->GetDesc();

	if (!d)
		return;

	if (m_Relation.find(account) == m_Relation.end())
		return;

	if (m_Relation[account].empty())
		return;

	TPacketGCMessenger pack;

	pack.header = HEADER_GC_MESSENGER;
	pack.subheader = MESSENGER_SUBHEADER_GC_LIST;
	pack.size = sizeof(TPacketGCMessenger);

	TPacketGCMessengerListOffline pack_offline;
	TPacketGCMessengerListOnline pack_online;

	TEMP_BUFFER buf(128 * 1024); // 128k

	// KeyTSet::const_iterator it = m_Relation[account].begin(), eit = m_Relation[account].end();
	itertype(m_Relation[account]) it = m_Relation[account].begin(), eit = m_Relation[account].end();

	while (it != eit)
	{
		if (m_set_loginAccount.find(*it) != m_set_loginAccount.end())
		{
			pack_online.connected = 1;

			// Online
			pack_online.length = it->size();

			buf.write(&pack_online, sizeof(TPacketGCMessengerListOnline));
			buf.write(it->c_str(), it->size());
		}
		else
		{
			pack_offline.connected = 0;

			// Offline
			pack_offline.length = it->size();

			buf.write(&pack_offline, sizeof(TPacketGCMessengerListOffline));
			buf.write(it->c_str(), it->size());
		}

		++it;
	}

	pack.size += buf.size();

	d->BufferedPacket(&pack, sizeof(TPacketGCMessenger));
	d->Packet(buf.read_peek(), buf.size());
}

void MessengerManager::SendLogin(const std::string& account, const std::string& companion)
{
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (!d)
		return;

	if (!d->GetCharacter())
		return;

	if (ch->GetGMLevel() == GM_PLAYER && gm_get_level(companion.c_str()) != GM_PLAYER)
		return;

	BYTE bLen = companion.size();

	TPacketGCMessenger pack;

	pack.header = HEADER_GC_MESSENGER;
	pack.subheader = MESSENGER_SUBHEADER_GC_LOGIN;
	pack.size = sizeof(TPacketGCMessenger) + sizeof(BYTE) + bLen;

	d->BufferedPacket(&pack, sizeof(TPacketGCMessenger));
	d->BufferedPacket(&bLen, sizeof(BYTE));
	d->Packet(companion.c_str(), companion.size());
}

void MessengerManager::SendLogout(const std::string& account, const std::string& companion)
{
	if (!companion.size())
		return;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (!d)
		return;

	BYTE bLen = companion.size();

	TPacketGCMessenger pack;

	pack.header = HEADER_GC_MESSENGER;
	pack.subheader = MESSENGER_SUBHEADER_GC_LOGOUT;
	pack.size = sizeof(TPacketGCMessenger) + sizeof(BYTE) + bLen;

	d->BufferedPacket(&pack, sizeof(TPacketGCMessenger));
	d->BufferedPacket(&bLen, sizeof(BYTE));
	d->Packet(companion.c_str(), companion.size());
}

#if defined(__MESSENGER_GM__)
void MessengerManager::SendGMList(const std::string& account)
{
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());

	if (!ch)
		return;

	LPDESC d = ch->GetDesc();

	if (!d)
		return;

	TPacketGCMessenger pack;

	pack.header = HEADER_GC_MESSENGER;
	pack.subheader = MESSENGER_SUBHEADER_GC_GM_LIST;
	pack.size = sizeof(TPacketGCMessenger);

	TPacketGCMessengerGMListOnline pack_online;
	TPacketGCMessengerGMListOffline pack_offline;

	TEMP_BUFFER buf(128 * 1024); // 128k

	//KeyGSet::const_iterator it = m_GMRelation[account].begin(), eit = m_GMRelation[account].end();
	itertype(m_GMRelation[account]) it = m_GMRelation[account].begin(), eit = m_GMRelation[account].end();

	while (it != eit)
	{
		if (m_set_loginAccount.find(*it) != m_set_loginAccount.end())
		{
			pack_online.connected = 1;
			pack_online.length = it->size();

			buf.write(&pack_online, sizeof(TPacketGCMessengerGMListOnline));
			buf.write(it->c_str(), it->size());
		}
		else
		{
			pack_offline.connected = 0;
			pack_offline.length = it->size();

			buf.write(&pack_offline, sizeof(TPacketGCMessengerGMListOffline));
			buf.write(it->c_str(), it->size());
		}

		++it;
	}

	pack.size += buf.size();

	d->BufferedPacket(&pack, sizeof(TPacketGCMessenger));
	d->Packet(buf.read_peek(), buf.size());
}

void MessengerManager::SendGMLogin(const std::string& account, const std::string& companion)
{
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (!d)
		return;

	if (!d->GetCharacter())
		return;

	BYTE bLen = companion.size();

	TPacketGCMessenger pack;

	pack.header = HEADER_GC_MESSENGER;
	pack.subheader = MESSENGER_SUBHEADER_GC_GM_LOGIN;
	pack.size = sizeof(TPacketGCMessenger) + sizeof(BYTE) + bLen;

	d->BufferedPacket(&pack, sizeof(TPacketGCMessenger));
	d->BufferedPacket(&bLen, sizeof(BYTE));
	d->Packet(companion.c_str(), companion.size());
}

void MessengerManager::SendGMLogout(const std::string& account, const std::string& companion)
{
	if (!companion.size())
		return;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (!d)
		return;

	BYTE bLen = companion.size();

	TPacketGCMessenger pack;

	pack.header = HEADER_GC_MESSENGER;
	pack.subheader = MESSENGER_SUBHEADER_GC_GM_LOGOUT;
	pack.size = sizeof(TPacketGCMessenger) + sizeof(BYTE) + bLen;

	d->BufferedPacket(&pack, sizeof(TPacketGCMessenger));
	d->BufferedPacket(&bLen, sizeof(BYTE));
	d->Packet(companion.c_str(), companion.size());
}

void MessengerManager::LoadGMList(SQLMsg* msg)
{
	if (NULL == msg)
		return;

	if (NULL == msg->Get())
		return;

	if (msg->Get()->uiNumRows == 0)
		return;

	std::string account;

	sys_log(1, "Messenger::LoadGMList");

	for (uint i = 0; i < msg->Get()->uiNumRows; ++i)
	{
		MYSQL_ROW row = mysql_fetch_row(msg->Get()->pSQLResult);

		if (row[0] && row[1])
		{
			if (account.length() == 0)
				account = row[0];

			m_GMRelation[row[0]].insert(row[1]);
			m_InverseGMRelation[row[1]].insert(row[0]);
		}
	}

	SendGMList(account);

	for (KeyGSet::iterator it = m_InverseGMRelation[account].begin(); it != m_InverseGMRelation[account].end(); ++it)
		SendGMLogin(*it, account);
}
#endif

#if defined(__MESSENGER_BLOCK_SYSTEM__)
void MessengerManager::SendBlockList(const std::string& account)
{
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());

	if (!ch)
		return;

	LPDESC d = ch->GetDesc();

	if (!d)
		return;

	TPacketGCMessenger pack;

	pack.header = HEADER_GC_MESSENGER;
	pack.subheader = MESSENGER_SUBHEADER_GC_BLOCK_LIST;
	pack.size = sizeof(TPacketGCMessenger);

	TPacketGCMessengerBlockListOnline pack_online;
	TPacketGCMessengerBlockListOffline pack_offline;

	TEMP_BUFFER buf(128 * 1024);

	//KeyBSet::const_iterator it = m_BlockRelation[account].begin(), eit = m_BlockRelation[account].end();
	itertype(m_BlockRelation[account]) it = m_BlockRelation[account].begin(), eit = m_BlockRelation[account].end();

	while (it != eit)
	{
		if (m_set_loginAccount.find(*it) != m_set_loginAccount.end())
		{
			pack_online.connected = 1;
			pack_online.length = it->size();

			buf.write(&pack_online, sizeof(TPacketGCMessengerBlockListOnline));
			buf.write(it->c_str(), it->size());
		}
		else
		{
			pack_offline.connected = 0;
			pack_offline.length = it->size();

			buf.write(&pack_offline, sizeof(TPacketGCMessengerBlockListOffline));
			buf.write(it->c_str(), it->size());
		}

		++it;
	}

	pack.size += buf.size();

	d->BufferedPacket(&pack, sizeof(TPacketGCMessenger));
	d->Packet(buf.read_peek(), buf.size());
}

void MessengerManager::SendBlockLogin(const std::string& account, const std::string& companion)
{
	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (!d)
		return;

	if (!d->GetCharacter())
		return;

	BYTE bLen = companion.size();

	TPacketGCMessenger pack;

	pack.header = HEADER_GC_MESSENGER;
	pack.subheader = MESSENGER_SUBHEADER_GC_BLOCK_LOGIN;
	pack.size = sizeof(TPacketGCMessenger) + sizeof(BYTE) + bLen;

	d->BufferedPacket(&pack, sizeof(TPacketGCMessenger));
	d->BufferedPacket(&bLen, sizeof(BYTE));
	d->Packet(companion.c_str(), companion.size());
}

void MessengerManager::SendBlockLogout(const std::string& account, const std::string& companion)
{
	if (!companion.size())
		return;

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (!d)
		return;

	BYTE bLen = companion.size();

	TPacketGCMessenger pack;

	pack.header = HEADER_GC_MESSENGER;
	pack.subheader = MESSENGER_SUBHEADER_GC_BLOCK_LOGOUT;
	pack.size = sizeof(TPacketGCMessenger) + sizeof(BYTE) + bLen;

	d->BufferedPacket(&pack, sizeof(TPacketGCMessenger));
	d->BufferedPacket(&bLen, sizeof(BYTE));
	d->Packet(companion.c_str(), companion.size());
}

void MessengerManager::LoadBlockList(SQLMsg* msg)
{
	if (NULL == msg || NULL == msg->Get() || msg->Get()->uiNumRows == 0)
		return;

	std::string account;

	for (uint i = 0; i < msg->Get()->uiNumRows; ++i)
	{
		MYSQL_ROW row = mysql_fetch_row(msg->Get()->pSQLResult);

		if (row[0] && row[1])
		{
			if (account.length() == 0)
				account = row[0];

			m_BlockRelation[row[0]].insert(row[1]);
			m_InverseBlockRelation[row[1]].insert(row[0]);
		}
	}

	SendBlockList(account);

	for (KeyBSet::iterator it = m_InverseBlockRelation[account].begin(); it != m_InverseBlockRelation[account].end(); ++it)
		SendBlockLogin(*it, account);
}

void MessengerManager::__AddToBlockList(const std::string& account, const std::string& companion)
{
	m_BlockRelation[account].insert(companion);
	m_InverseBlockRelation[companion].insert(account);

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (d)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s is now blocked."), companion.c_str());
	}

	LPCHARACTER tch = CHARACTER_MANAGER::instance().FindPC(companion.c_str());

	if (tch)
		SendBlockLogin(account, companion);
	else
		SendBlockLogout(account, companion);
}

void MessengerManager::AddToBlockList(const std::string& account, const std::string& companion)
{
	if (companion.size() == 0)
		return;

	if (m_BlockRelation[account].find(companion) != m_BlockRelation[account].end())
		return;

	DBManager::instance().Query("INSERT INTO messenger_block_list%s VALUES ('%s', '%s', NOW())",
		get_table_postfix(), account.c_str(), companion.c_str());

	__AddToBlockList(account, companion);

	TPacketGGMessenger p2ppck;

	p2ppck.bHeader = HEADER_GG_MESSENGER_BLOCK_ADD;
	strlcpy(p2ppck.szAccount, account.c_str(), sizeof(p2ppck.szAccount));
	strlcpy(p2ppck.szCompanion, companion.c_str(), sizeof(p2ppck.szCompanion));
	P2P_MANAGER::instance().Send(&p2ppck, sizeof(TPacketGGMessenger));
}

void MessengerManager::__RemoveFromBlockList(const std::string& account, const std::string& companion)
{
	m_BlockRelation[account].erase(companion);
	m_InverseBlockRelation[companion].erase(account);

	LPCHARACTER ch = CHARACTER_MANAGER::instance().FindPC(account.c_str());
	LPDESC d = ch ? ch->GetDesc() : NULL;

	if (d)
		ch->ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%s is no longer blocked."), companion.c_str());
}

bool MessengerManager::IsBlocked(const char* c_szAccount, const char* c_szName)
{
	if (m_BlockRelation.empty())
		return false;

	std::string strAccount(c_szAccount), strName(c_szName);
	for (KeyBSet::iterator it = m_BlockRelation[strAccount].begin(); it != m_BlockRelation[strAccount].end(); ++it)
	{
		if (it->compare(strName) == 0)
			return true;
	}

	return false;
}

void MessengerManager::RemoveFromBlockList(const std::string& account, const std::string& companion)
{
	if (companion.size() == 0)
		return;

	char szCompanion[CHARACTER_NAME_MAX_LEN + 1];
	DBManager::instance().EscapeString(szCompanion, sizeof(szCompanion), companion.c_str(), companion.length());
	DBManager::instance().Query("DELETE FROM messenger_block_list%s WHERE `account` = '%s' AND `companion` = '%s'",
		get_table_postfix(), account.c_str(), szCompanion);

	__RemoveFromBlockList(account, companion);

	TPacketGGMessenger p2ppck;

	p2ppck.bHeader = HEADER_GG_MESSENGER_BLOCK_REMOVE;
	strlcpy(p2ppck.szAccount, account.c_str(), sizeof(p2ppck.szAccount));
	strlcpy(p2ppck.szCompanion, companion.c_str(), sizeof(p2ppck.szCompanion));
	P2P_MANAGER::instance().Send(&p2ppck, sizeof(TPacketGGMessenger));
}

void MessengerManager::RemoveAllBlockList(keyA account)
{
	std::set<keyB> company(m_BlockRelation[account]);

	DBManager::instance().Query("DELETE FROM messenger_block_list%s WHERE `account` = '%s' OR companion = '%s'",
		get_table_postfix(), account.c_str(), account.c_str());

	for (std::set<keyT>::iterator iter = company.begin(); iter != company.end(); iter++)
	{
		this->RemoveFromList(account, *iter);
	}

	for (std::set<keyB>::iterator iter = company.begin(); iter != company.end();)
	{
		company.erase(iter++);
	}

	company.clear();
}
#endif
