#ifndef __INC_P2P_MANAGER_H__
#define __INC_P2P_MANAGER_H__

#include <boost/unordered_map.hpp>

#include "input.h"
#include "../../common/stl.h"
#include "../../common/service.h"

typedef struct _CCI
{
	char szName[CHARACTER_NAME_MAX_LEN + 1];
	DWORD dwPID;
	BYTE bEmpire;
	long lMapIndex;
	BYTE bChannel;
	BYTE bLanguage;

	LPDESC pkDesc;
} CCI;

class P2P_MANAGER : public singleton<P2P_MANAGER>
{
public:
	P2P_MANAGER();
	~P2P_MANAGER();

	// �Ʒ� Register* Unregister* pair���� ���������� ��� ���� ��ƾ�� ����Ѵ�.
	// ���� ��������� ǥ���ϱ� ���� ��
	void RegisterAcceptor(LPDESC d);
	void UnregisterAcceptor(LPDESC d);

	void RegisterConnector(LPDESC d);
	void UnregisterConnector(LPDESC d);

	void EraseUserByDesc(LPDESC d); // �ش� desc�� �ִ� �������� �����.

	void FlushOutput();

	void Boot(LPDESC d); // p2p ó���� �ʿ��� ������ �����ش�. (�� ĳ������ �α��� ���� ��)

	void Send(const void* c_pvData, int iSize, LPDESC except = NULL);

	void Login(LPDESC d, const TPacketGGLogin* p);
	void Logout(const char* c_pszName);

	CCI* Find(const char* c_pszName);
	CCI* FindByPID(DWORD pid);

	int GetCount();
	int GetPIDCount() { return m_map_pkCCI.size(); };
	int GetEmpireUserCount(int idx);
	int GetDescCount();
	void GetP2PHostNames(std::string& hostNames);

private:
	void Logout(CCI* pkCCI);

	CInputProcessor* m_pkInputProcessor;
	int m_iHandleCount;

	typedef boost::unordered_map<std::string, CCI*, stringhash> TCCIMap;
	typedef boost::unordered_map<DWORD, CCI*> TPIDCCIMap;
	typedef boost::unordered_map<BYTE, CCI*> TLocaleCCIMap;

	TR1_NS::unordered_set<LPDESC> m_set_pkPeers;
	TCCIMap m_map_pkCCI;
	TPIDCCIMap m_map_dwPID_pkCCI;
	TLocaleCCIMap m_map_bLocale_pkCCI;

	int m_aiEmpireUserCount[EMPIRE_MAX_NUM];
};

#endif // __INC_P2P_MANAGER_H__
