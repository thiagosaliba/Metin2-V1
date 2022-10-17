#pragma once

#include "../EterLib/NetStream.h"

class CServerStateChecker : public CSingleton<CServerStateChecker>
{
public:
	CServerStateChecker();
	virtual ~CServerStateChecker();

	void Create(PyObject* poWnd);
	void AddChannel(UINT uServerIndex, const char* c_szAddr, UINT uPort);
	void Request();
	void Update();

	void Initialize();

#if defined(ENABLE_RTT_STATISTICS)
public:
	enum ERTTMisc
	{
		MAX_RTT_REQUESTS = 4,
		MAX_RTT_REQUEST_MS = 5000,
	};

	typedef struct SEchoReply
	{
		DWORD dwRTT;
		float fPktLossPct;
	} TEchoReply;

	void GetEchoReply(TEchoReply* EchoReply);
	void ResetEchoReply();

protected:
	DWORD m_dwLastRTT;
	BYTE m_byPktRequest;
	BYTE m_byPktResponse;
	BYTE m_byPktLoss;
#endif

private:
	typedef struct SChannel
	{
		UINT uServerIndex;
		const char* c_szAddr;
		UINT uPort;
	} TChannel;

	PyObject* m_poWnd;

	std::list<TChannel> m_lstChannel;

	CNetworkStream m_kStream;
};
