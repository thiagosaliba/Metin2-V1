#ifndef __INC_MESSENGER_MANAGER_H__
#define __INC_MESSENGER_MANAGER_H__

#include "db.h"

typedef std::string keyT;
typedef const std::string& keyA;

typedef std::set<std::string> KeyTSet;
typedef std::map<std::string, KeyTSet> KeyTRelation;

#if defined(__MESSENGER_GM__)
typedef std::string keyG;
typedef std::set<std::string> KeyGSet;
typedef std::map<std::string, KeyGSet> KeyGRelation;
#endif

#if defined(__MESSENGER_BLOCK_SYSTEM__)
typedef std::string keyB;
typedef std::set<std::string> KeyBSet;
typedef std::map<std::string, KeyBSet> KeyBRelation;
#endif

class MessengerManager : public singleton<MessengerManager>
{
public:
	MessengerManager();
	virtual ~MessengerManager();

public:
	void P2PLogin(const std::string& account);
	void P2PLogout(const std::string& account);

	void Login(const std::string& account);
	void Logout(const std::string& account);

	void RequestToAdd(LPCHARACTER ch, LPCHARACTER target);
	void AuthToAdd(const std::string& account, const std::string& companion, bool bDeny);

	void __AddToList(const std::string& account, const std::string& companion); // 실제 m_Relation, m_InverseRelation 수정하는 메소드
	void AddToList(const std::string& account, const std::string& companion);

	void __RemoveFromList(const std::string& account, const std::string& companion); // 실제 m_Relation, m_InverseRelation 수정하는 메소드
	bool IsInList(const std::string& account, const std::string& companion);
	bool IsFriend(const char* c_szAccount /* Me */, const char* c_szName);
	void RemoveFromList(const std::string& account, const std::string& companion);
	void RemoveAllList(const std::string& account);

#if defined(__MESSENGER_BLOCK_SYSTEM__)
	void __AddToBlockList(const std::string& account, const std::string& companion);
	void AddToBlockList(const std::string& account, const std::string& companion);

	void __RemoveFromBlockList(const std::string& account, const std::string& companion);
	bool IsBlocked(const char* c_szAccount /* Me */, const char* c_szName);
	void RemoveFromBlockList(const std::string& account, const std::string& companion);
	void RemoveAllBlockList(const std::string& account);
#endif

	void Initialize();

private:
	void SendList(const std::string& account);
	void SendLogin(const std::string& account, const std::string& companion);
	void SendLogout(const std::string& account, const std::string& companion);

	void LoadList(SQLMsg* pmsg);

#if defined(__MESSENGER_GM__)
	void SendGMList(const std::string& account);
	void SendGMLogin(const std::string& account, const std::string& companion);
	void SendGMLogout(const std::string& account, const std::string& companion);

	void LoadGMList(SQLMsg* pmsg);
#endif

#if defined(__MESSENGER_BLOCK_SYSTEM__)
	void SendBlockList(const std::string& account);
	void SendBlockLogin(const std::string& account, const std::string& companion);
	void SendBlockLogout(const std::string& account, const std::string& companion);

	void LoadBlockList(SQLMsg* pmsg);
#endif

	void Destroy();

	KeyTSet m_set_loginAccount;
	KeyTRelation m_Relation;
	KeyTRelation m_InverseRelation;
	std::set<DWORD> m_set_requestToAdd;

#if defined(__MESSENGER_GM__)
	KeyGRelation m_GMRelation;
	KeyGRelation m_InverseGMRelation;
#endif

#if defined(__MESSENGER_BLOCK_SYSTEM__)
	KeyBRelation m_BlockRelation;
	KeyBRelation m_InverseBlockRelation;
	std::set<DWORD> m_set_requestToBlockAdd;
#endif
};

#endif // __INC_MESSENGER_MANAGER_H__
