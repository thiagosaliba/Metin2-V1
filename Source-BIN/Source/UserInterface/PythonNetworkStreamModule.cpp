#include "StdAfx.h"
#include "PythonNetworkStream.h"
//#include "PythonNetworkDatagram.h"
#include "AccountConnector.h"
#include "PythonGuild.h"
#include "Test.h"

#include "AbstractPlayer.h"

static std::string gs_stServerInfo;
extern BOOL gs_bEmpireLanuageEnable;
std::list<std::string> g_kList_strCommand;

PyObject* netGetBettingGuildWarValue(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.EXPORT_GetBettingGuildWarValue(szName));
}

PyObject* netSetServerInfo(PyObject* poSelf, PyObject* poArgs)
{
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	gs_stServerInfo = szFileName;
	return Py_BuildNone();
}

PyObject* netGetServerInfo(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("s", gs_stServerInfo.c_str());
}

PyObject* netClearServerInfo(PyObject* poSelf, PyObject* poArgs)
{
	gs_stServerInfo = "";
	return Py_BuildNone();
}

PyObject* netPreserveServerCommand(PyObject* poSelf, PyObject* poArgs)
{
	char* szLine;
	if (!PyTuple_GetString(poArgs, 0, &szLine))
		return Py_BuildException();

	g_kList_strCommand.push_back(szLine);

	return Py_BuildNone();
}

PyObject* netGetPreservedServerCommand(PyObject* poSelf, PyObject* poArgs)
{
	if (g_kList_strCommand.empty())
		return Py_BuildValue("s", "");

	std::string strCommand = g_kList_strCommand.front();
	g_kList_strCommand.pop_front();

	return Py_BuildValue("s", strCommand.c_str());
}

PyObject* netStartGame(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.StartGame();

	return Py_BuildNone();
}

PyObject* netIsTest(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", __IS_TEST_SERVER_MODE__);
}

PyObject* netWarp(PyObject* poSelf, PyObject* poArgs)
{
	int nX;
	if (!PyTuple_GetInteger(poArgs, 0, &nX))
		return Py_BuildException();

	int nY;
	if (!PyTuple_GetInteger(poArgs, 1, &nY))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.Warp(nX, nY);

	return Py_BuildNone();
}

PyObject* netLoadInsultList(PyObject* poSelf, PyObject* poArgs)
{
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.LoadInsultList(szFileName));
}

PyObject* netUploadMark(PyObject* poSelf, PyObject* poArgs)
{
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.UploadMark(szFileName));
}

PyObject* netUploadSymbol(PyObject* poSelf, PyObject* poArgs)
{
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.UploadSymbol(szFileName));
}

PyObject* netGetLoginID(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("s", rkNetStream.GetLoginId().c_str());
}

PyObject* netGetGuildID(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.GetGuildID());
}

PyObject* netGetEmpireID(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.GetEmpireID());
}

PyObject* netGetMainActorVID(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.GetMainActorVID());
}

PyObject* netGetMainActorRace(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.GetMainActorRace());
}

PyObject* netGetMainActorEmpire(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.GetMainActorEmpire());
}

PyObject* netGetMainActorSkillGroup(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.GetMainActorSkillGroup());
}

PyObject* netIsSelectedEmpire(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.IsSelectedEmpire());
}

PyObject* netGetAccountCharacterSlotDataInteger(PyObject* poSelf, PyObject* poArgs)
{
	int nIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &nIndex))
		return Py_BuildException();

	int nType;
	if (!PyTuple_GetInteger(poArgs, 1, &nType))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	UINT uResult = rkNetStream.GetAccountCharacterSlotDatau(nIndex, nType);
	return Py_BuildValue("i", uResult);
}

PyObject* netGetAccountCharacterSlotDataString(PyObject* poSelf, PyObject* poArgs)
{
	int nIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &nIndex))
		return Py_BuildException();

	int nType;
	if (!PyTuple_GetInteger(poArgs, 1, &nType))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("s", rkNetStream.GetAccountCharacterSlotDataz(nIndex, nType));
}

// SUPPORT_BGM
PyObject* netGetFieldMusicFileName(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("s", rkNetStream.GetFieldMusicFileName());
}

PyObject* netGetFieldMusicVolume(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("f", rkNetStream.GetFieldMusicVolume());
}
// END_OF_SUPPORT_BGM

PyObject* netSetPhaseWindow(PyObject* poSelf, PyObject* poArgs)
{
	int ePhaseWnd;
	if (!PyTuple_GetInteger(poArgs, 0, &ePhaseWnd))
		return Py_BuildException();

	PyObject* poPhaseWnd;
	if (!PyTuple_GetObject(poArgs, 1, &poPhaseWnd))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetPhaseWindow(ePhaseWnd, poPhaseWnd);
	return Py_BuildNone();
}

PyObject* netClearPhaseWindow(PyObject* poSelf, PyObject* poArgs)
{
	int ePhaseWnd;
	if (!PyTuple_GetInteger(poArgs, 0, &ePhaseWnd))
		return Py_BuildException();

	PyObject* poPhaseWnd;
	if (!PyTuple_GetObject(poArgs, 1, &poPhaseWnd))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ClearPhaseWindow(ePhaseWnd, poPhaseWnd);
	return Py_BuildNone();
}

PyObject* netSetServerCommandParserWindow(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poPhaseWnd;
	if (!PyTuple_GetObject(poArgs, 0, &poPhaseWnd))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetServerCommandParserWindow(poPhaseWnd);
	return Py_BuildNone();
}

PyObject* netSetAccountConnectorHandler(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poPhaseWnd;
	if (!PyTuple_GetObject(poArgs, 0, &poPhaseWnd))
		return Py_BuildException();

	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rkAccountConnector.SetHandler(poPhaseWnd);
	return Py_BuildNone();
}

PyObject* netSetHandler(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poHandler;

	if (!PyTuple_GetObject(poArgs, 0, &poHandler))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetHandler(poHandler);
	return Py_BuildNone();
}

PyObject* netSetTCPRecvBufferSize(PyObject* poSelf, PyObject* poArgs)
{
	int bufSize;
	if (!PyTuple_GetInteger(poArgs, 0, &bufSize))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetRecvBufferSize(bufSize);
	return Py_BuildNone();
}

PyObject* netSetTCPSendBufferSize(PyObject* poSelf, PyObject* poArgs)
{
	int bufSize;
	if (!PyTuple_GetInteger(poArgs, 0, &bufSize))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetSendBufferSize(bufSize);
	return Py_BuildNone();
}

PyObject* netSetUDPRecvBufferSize(PyObject* poSelf, PyObject* poArgs)
{
	int bufSize;
	if (!PyTuple_GetInteger(poArgs, 0, &bufSize))
		return Py_BuildException();

	//CPythonNetworkDatagram::Instance().SetRecvBufferSize(bufSize);
	return Py_BuildNone();
}

PyObject* netSetMarkServer(PyObject* poSelf, PyObject* poArgs)
{
	char* szAddr;
	if (!PyTuple_GetString(poArgs, 0, &szAddr))
		return Py_BuildException();

	int port;
	if (!PyTuple_GetInteger(poArgs, 1, &port))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetMarkServer(szAddr, port);
	return Py_BuildNone();
}

PyObject* netConnectTCP(PyObject* poSelf, PyObject* poArgs)
{
	char* szAddr;
	if (!PyTuple_GetString(poArgs, 0, &szAddr))
		return Py_BuildException();

	int port;
	if (!PyTuple_GetInteger(poArgs, 1, &port))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ConnectLoginServer(szAddr, port);
	return Py_BuildNone();
}

PyObject* netConnectUDP(PyObject* poSelf, PyObject* poArgs)
{
	char* c_szIP;
	if (!PyTuple_GetString(poArgs, 0, &c_szIP))
		return Py_BuildException();
	int iPort;
	if (!PyTuple_GetInteger(poArgs, 1, &iPort))
		return Py_BuildException();

	//CPythonNetworkDatagram::Instance().SetConnection(c_szIP, iPort);
	return Py_BuildNone();
}

PyObject* netConnectToAccountServer(PyObject* poSelf, PyObject* poArgs)
{
	char* addr;
	if (!PyTuple_GetString(poArgs, 0, &addr))
		return Py_BuildException();

	int port;
	if (!PyTuple_GetInteger(poArgs, 1, &port))
		return Py_BuildException();

	char* account_addr;
	if (!PyTuple_GetString(poArgs, 2, &account_addr))
		return Py_BuildException();

	int account_port;
	if (!PyTuple_GetInteger(poArgs, 3, &account_port))
		return Py_BuildException();

	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rkAccountConnector.Connect(addr, port, account_addr, account_port);
	return Py_BuildNone();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PyObject* netSetLoginInfo(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	char* szPwd;
	if (!PyTuple_GetString(poArgs, 1, &szPwd))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rkNetStream.SetLoginInfo(szName, szPwd);
	rkAccountConnector.SetLoginInfo(szName, szPwd);
	return Py_BuildNone();
}

PyObject* netSetOfflinePhase(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetOffLinePhase();
	return Py_BuildNone();
}

PyObject* netSendSelectEmpirePacket(PyObject* poSelf, PyObject* poArgs)
{
	int iEmpireIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iEmpireIndex))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendSelectEmpirePacket(iEmpireIndex);
	return Py_BuildNone();
}

PyObject* netSendLoginPacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	char* szPwd;
	if (!PyTuple_GetString(poArgs, 1, &szPwd))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendLoginPacket(szName, szPwd);
	return Py_BuildNone();
}

PyObject* netDirectEnter(PyObject* poSelf, PyObject* poArgs)
{
	int nChrSlot;
	if (!PyTuple_GetInteger(poArgs, 0, &nChrSlot))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ConnectGameServer(nChrSlot);
	return Py_BuildNone();
}

PyObject* netSendSelectCharacterPacket(PyObject* poSelf, PyObject* poArgs)
{
	int Index;
	if (!PyTuple_GetInteger(poArgs, 0, &Index))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendSelectCharacterPacket((BYTE)Index);
	return Py_BuildNone();
}

PyObject* netSendChangeNamePacket(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();
	char* szName;
	if (!PyTuple_GetString(poArgs, 1, &szName))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendChangeNamePacket((BYTE)iIndex, szName);
	return Py_BuildNone();
}

PyObject* netEnableChatInsultFilter(PyObject* poSelf, PyObject* poArgs)
{
	int nEnable;
	if (!PyTuple_GetInteger(poArgs, 0, &nEnable))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.EnableChatInsultFilter(nEnable ? true : false);
	return Py_BuildNone();
}

PyObject* netIsChatInsultIn(PyObject* poSelf, PyObject* poArgs)
{
	char* szMsg;
	if (!PyTuple_GetString(poArgs, 0, &szMsg))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.IsChatInsultIn(szMsg));
}

PyObject* netIsInsultIn(PyObject* poSelf, PyObject* poArgs)
{
	char* szMsg;
	if (!PyTuple_GetString(poArgs, 0, &szMsg))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.IsInsultIn(szMsg));
}

#ifdef ENABLE_MESSENGER_STATUS
PyObject* netCheckMessengerStatus(PyObject* poSelf, PyObject* poArgs)
{
	char* szTarget;
	if (!PyTuple_GetString(poArgs, 0, &szTarget))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.CheckMessengerStatus(szTarget));
}
#endif

PyObject* netSendWhisperPacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	char* szLine;

	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	if (!PyTuple_GetString(poArgs, 1, &szLine))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendWhisperPacket(szName, szLine);
	return Py_BuildNone();
}

PyObject* netSendCharacterPositionPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iPosition;
	if (!PyTuple_GetInteger(poArgs, 0, &iPosition))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendCharacterPositionPacket((BYTE)iPosition);
	return Py_BuildNone();
}

PyObject* netSendChatPacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szLine;
	if (!PyTuple_GetString(poArgs, 0, &szLine))
		return Py_BuildException();
	int iType;
	if (!PyTuple_GetInteger(poArgs, 1, &iType))
	{
		iType = CHAT_TYPE_TALKING;
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendChatPacket(szLine, iType);
	return Py_BuildNone();
}

PyObject* netSendEmoticon(PyObject* poSelf, PyObject* poArgs)
{
	int eEmoticon;
	if (!PyTuple_GetInteger(poArgs, 0, &eEmoticon))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendEmoticon(eEmoticon);
	return Py_BuildNone();
}

PyObject* netSendCreateCharacterPacket(PyObject* poSelf, PyObject* poArgs)
{
	int index;
	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	char* name;
	if (!PyTuple_GetString(poArgs, 1, &name))
		return Py_BuildException();

	int job;
	if (!PyTuple_GetInteger(poArgs, 2, &job))
		return Py_BuildException();

	int shape;
	if (!PyTuple_GetInteger(poArgs, 3, &shape))
		return Py_BuildException();

	int stat1;
	if (!PyTuple_GetInteger(poArgs, 4, &stat1))
		return Py_BuildException();

	int stat2;
	if (!PyTuple_GetInteger(poArgs, 5, &stat2))
		return Py_BuildException();

	int stat3;
	if (!PyTuple_GetInteger(poArgs, 6, &stat3))
		return Py_BuildException();

	int stat4;
	if (!PyTuple_GetInteger(poArgs, 7, &stat4))
		return Py_BuildException();

	char* pin;
	if (!PyTuple_GetString(poArgs, 8, &pin))
		return Py_BuildException();

	if (index < 0 && index > 3)
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendCreateCharacterPacket((BYTE)index, name, (BYTE)job, (BYTE)shape, stat1, stat2, stat3, stat4, pin);
	return Py_BuildNone();
}

PyObject* netSendDestroyCharacterPacket(PyObject* poSelf, PyObject* poArgs)
{
	int index;
	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	char* szPrivateCode;
	if (!PyTuple_GetString(poArgs, 1, &szPrivateCode))
		return Py_BuildException();

	if (index < 0 && index > 3)
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendDestroyCharacterPacket((BYTE)index, szPrivateCode);
	return Py_BuildNone();
}

PyObject* netSendCharacterPinCodePacket(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	char* szPinCode;
	if (!PyTuple_GetString(poArgs, 1, &szPinCode))
		return Py_BuildException();

	if (iIndex < 0 && iIndex > 3)
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendCharacterPinCodePacket((BYTE)iIndex, szPinCode);
	return Py_BuildNone();
}

PyObject* netSendEnterGamePacket(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendEnterGame();
	return Py_BuildNone();
}

PyObject* netOnClickPacket(PyObject* poSelf, PyObject* poArgs)
{
	int index;
	if (!PyTuple_GetInteger(poArgs, 0, &index))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendOnClickPacket(index);

	return Py_BuildNone();
}

PyObject* netSendItemUsePacket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BuildException();
		break;
	case 2:
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemUsePacket(Cell);
	return Py_BuildNone();
}

PyObject* netSendItemUseToItemPacket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos SourceCell;
	TItemPos TargetCell;
	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &SourceCell.cell))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 1, &TargetCell.cell))
			return Py_BuildException();
		break;
	case 4:
		if (!PyTuple_GetByte(poArgs, 0, &SourceCell.window_type))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 1, &SourceCell.cell))
			return Py_BuildException();

		if (!PyTuple_GetByte(poArgs, 2, &TargetCell.window_type))
			return Py_BuildException();

		if (!PyTuple_GetInteger(poArgs, 3, &TargetCell.cell))
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemUseToItemPacket(SourceCell, TargetCell);
	return Py_BuildNone();
}

PyObject* netSendItemDropPacket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;
	switch (PyTuple_Size(poArgs))
	{
	case 1:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BuildException();
		break;
	case 2:
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
#if defined(ENABLE_CHEQUE_SYSTEM)
	rkNetStream.SendItemDropPacket(Cell, 0, 0);
#else
	rkNetStream.SendItemDropPacket(Cell, 0);
#endif
	return Py_BuildNone();
}

#ifdef ENABLE_NEW_DROP_DIALOG
PyObject* netSendItemDestroyPacket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;

	if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemDestroyPacket(Cell);
	return Py_BuildNone();
}
#endif

PyObject* netSendItemDropPacketNew(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;
	int count;
	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &count))
			return Py_BuildException();

		break;
	case 3:
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &count))
			return Py_BuildException();

		break;
	default:
		return Py_BuildException();
	}
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
#if defined(ENABLE_CHEQUE_SYSTEM)
	rkNetStream.SendItemDropPacketNew(Cell, 0, 0, count);
#else
	rkNetStream.SendItemDropPacketNew(Cell, 0, count);
#endif
	return Py_BuildNone();
}

PyObject* netSendElkDropPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iElk;
	if (!PyTuple_GetInteger(poArgs, 0, &iElk))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
#if defined(ENABLE_CHEQUE_SYSTEM)
	rkNetStream.SendItemDropPacket(TItemPos(RESERVED_WINDOW, 0), (DWORD)iElk, 0);
#else
	rkNetStream.SendItemDropPacket(TItemPos(RESERVED_WINDOW, 0), (DWORD)iElk);
#endif
	return Py_BuildNone();
}

PyObject* netSendGoldDropPacketNew(PyObject* poSelf, PyObject* poArgs)
{
	int iElk;
	if (!PyTuple_GetInteger(poArgs, 0, &iElk))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
#if defined(ENABLE_CHEQUE_SYSTEM)
	rkNetStream.SendItemDropPacketNew(TItemPos(RESERVED_WINDOW, 0), (DWORD)iElk, 0, 0);
#else
	rkNetStream.SendItemDropPacketNew(TItemPos(RESERVED_WINDOW, 0), (DWORD)iElk, 0);
#endif
	return Py_BuildNone();
}

#if defined(ENABLE_CHEQUE_SYSTEM)
PyObject* netSendChequeDropPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iCheque;
	if (!PyTuple_GetInteger(poArgs, 0, &iCheque))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemDropPacket(TItemPos(RESERVED_WINDOW, 0), 0, (DWORD)iCheque);
	return Py_BuildNone();
}

PyObject* netSendChequeDropPacketNew(PyObject* poSelf, PyObject* poArgs)
{
	int iCheque;
	if (!PyTuple_GetInteger(poArgs, 0, &iCheque))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemDropPacketNew(TItemPos(RESERVED_WINDOW, 0), 0, (DWORD)iCheque, 0);
	return Py_BuildNone();
}
#endif

PyObject* netSendItemMovePacket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;
	TItemPos ChangeCell;
	int num;

	switch (PyTuple_Size(poArgs))
	{
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &ChangeCell.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &num))
			return Py_BuildException();
		break;
	case 5:
	{
		if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BuildException();
		if (!PyTuple_GetByte(poArgs, 2, &ChangeCell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 3, &ChangeCell.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 4, &num))
			return Py_BuildException();
	}
	break;
	default:
		return Py_BuildException();
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemMovePacket(Cell, ChangeCell, (WORD)num);
	return Py_BuildNone();
}

PyObject* netSendItemPickUpPacket(PyObject* poSelf, PyObject* poArgs)
{
	int vid;
	if (!PyTuple_GetInteger(poArgs, 0, &vid))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendItemPickUpPacket(vid);
	return Py_BuildNone();
}

PyObject* netSendGiveItemPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iTargetVID;
	TItemPos Cell;
	int iItemCount;
	switch (PyTuple_Size(poArgs))
	{
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &iTargetVID))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &iItemCount))
			return Py_BuildException();
		break;
	case 4:
		if (!PyTuple_GetInteger(poArgs, 0, &iTargetVID))
			return Py_BuildException();
		if (!PyTuple_GetByte(poArgs, 1, &Cell.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &Cell.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 3, &iItemCount))
			return Py_BuildException();
		break;
	default:
		break;
	}

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendGiveItemPacket(iTargetVID, Cell, iItemCount);
	return Py_BuildNone();
}

PyObject* netSendShopEndPacket(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendShopEndPacket();
	return Py_BuildNone();
}

PyObject* netSendShopBuyPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iCount;
	if (!PyTuple_GetInteger(poArgs, 0, &iCount))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendShopBuyPacket(iCount);
	return Py_BuildNone();
}

PyObject* netSendShopSellPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotNumber;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotNumber))
		return Py_BuildException();
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendShopSellPacket(iSlotNumber);
	return Py_BuildNone();
}

PyObject* netSendShopSellPacketNew(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotNumber;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotNumber))
		return Py_BuildException();
	int iCount;
	if (!PyTuple_GetInteger(poArgs, 1, &iCount))
		return Py_BuildException();
	int iType;
	if (!PyTuple_GetInteger(poArgs, 2, &iType))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendShopSellPacketNew(iSlotNumber, iCount, iType);
	return Py_BuildNone();
}

PyObject* netSendExchangeStartPacket(PyObject* poSelf, PyObject* poArgs)
{
	int vid;
	if (!PyTuple_GetInteger(poArgs, 0, &vid))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeStartPacket(vid);
	return Py_BuildNone();
}

PyObject* netSendExchangeElkAddPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iElk;
	if (!PyTuple_GetInteger(poArgs, 0, &iElk))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeElkAddPacket(iElk);
	return Py_BuildNone();
}

#if defined(ENABLE_CHEQUE_SYSTEM)
PyObject* netSendExchangeChequeAddPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iCheque;
	if (!PyTuple_GetInteger(poArgs, 0, &iCheque))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeChequeAddPacket(iCheque);
	return Py_BuildNone();
}
#endif

PyObject* netSendExchangeItemAddPacket(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bWindowType;
	if (!PyTuple_GetInteger(poArgs, 0, &bWindowType))
		return Py_BuildException();
	WORD wSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &wSlotIndex))
		return Py_BuildException();
	int iDisplaySlotIndex;
	if (!PyTuple_GetInteger(poArgs, 2, &iDisplaySlotIndex))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeItemAddPacket(TItemPos(bWindowType, wSlotIndex), iDisplaySlotIndex);
	return Py_BuildNone();
}

PyObject* netSendExchangeItemDelPacket(PyObject* poSelf, PyObject* poArgs)
{
	int pos;
	if (!PyTuple_GetInteger(poArgs, 0, &pos))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeItemDelPacket((BYTE)pos);
	return Py_BuildNone();
}

PyObject* netSendExchangeAcceptPacket(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeAcceptPacket();
	return Py_BuildNone();
}

PyObject* netSendExchangeExitPacket(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendExchangeExitPacket();
	return Py_BuildNone();
}

PyObject* netExitApplication(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ExitApplication();
	return Py_BuildNone();
}

PyObject* netExitGame(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ExitGame();
	return Py_BuildNone();
}

PyObject* netLogOutGame(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.LogOutGame();
	return Py_BuildNone();
}

PyObject* netDisconnect(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetOffLinePhase();
	rkNetStream.Disconnect();

	return Py_BuildNone();
}

PyObject* netIsConnect(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.IsOnline());
}

PyObject* netToggleGameDebugInfo(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.ToggleGameDebugInfo();
	return Py_BuildNone();
}

PyObject* netRegisterEmoticonString(PyObject* poSelf, PyObject* poArgs)
{
	char* pcEmoticonString;
	if (!PyTuple_GetString(poArgs, 0, &pcEmoticonString))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.RegisterEmoticonString(pcEmoticonString);
	return Py_BuildNone();
}

#ifdef ENABLE_MESSENGER_BLOCK
PyObject* netSendMessengerBlockAddByVIDPacket(PyObject* poSelf, PyObject* poArgs)
{
	int vid;
	if (!PyTuple_GetInteger(poArgs, 0, &vid))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMessengerBlockAddByVIDPacket(vid);

	return Py_BuildNone();
}

PyObject* netSendMessengerBlockAddByNamePacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMessengerBlockAddByNamePacket(szName);

	return Py_BuildNone();
}

PyObject* netSendMessengerBlockRemoveByVIDPacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szKey;
	if (!PyTuple_GetString(poArgs, 0, &szKey))
		return Py_BuildException();

	char* szName;
	if (!PyTuple_GetString(poArgs, 1, &szName))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMessengerBlockRemoveByVIDPacket(szKey, szName);

	return Py_BuildNone();
}
#endif

PyObject* netSendMessengerAddByVIDPacket(PyObject* poSelf, PyObject* poArgs)
{
	int vid;
	if (!PyTuple_GetInteger(poArgs, 0, &vid))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMessengerAddByVIDPacket(vid);

	return Py_BuildNone();
}

PyObject* netSendMessengerAddByNamePacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMessengerAddByNamePacket(szName);

	return Py_BuildNone();
}

PyObject* netSendMessengerRemovePacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szKey;
	if (!PyTuple_GetString(poArgs, 0, &szKey))
		return Py_BuildException();
	char* szName;
	if (!PyTuple_GetString(poArgs, 1, &szName))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMessengerRemovePacket(szKey, szName);

	return Py_BuildNone();
}

PyObject* netSendPartyInvitePacket(PyObject* poSelf, PyObject* poArgs)
{
	int vid;
	if (!PyTuple_GetInteger(poArgs, 0, &vid))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyInvitePacket(vid);

	return Py_BuildNone();
}

PyObject* netSendPartyInviteAnswerPacket(PyObject* poSelf, PyObject* poArgs)
{
	int vid;
	if (!PyTuple_GetInteger(poArgs, 0, &vid))
		return Py_BuildException();
	int answer;
	if (!PyTuple_GetInteger(poArgs, 1, &answer))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyInviteAnswerPacket(vid, answer);

	return Py_BuildNone();
}

PyObject* netSendPartyExitPacket(PyObject* poSelf, PyObject* poArgs)
{
	IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();

	DWORD dwVID = rPlayer.GetMainCharacterIndex();
	DWORD dwPID;
	if (rPlayer.PartyMemberVIDToPID(dwVID, &dwPID))
		rns.SendPartyRemovePacket(dwPID);

	return Py_BuildNone();
}

PyObject* netSendPartyRemovePacket(PyObject* poSelf, PyObject* poArgs)
{
	int vid;
	if (!PyTuple_GetInteger(poArgs, 0, &vid))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyRemovePacket(vid);

	return Py_BuildNone();
}

PyObject* netSendPartySetStatePacket(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();
	int iState;
	if (!PyTuple_GetInteger(poArgs, 1, &iState))
		return Py_BuildException();
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 2, &iFlag))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartySetStatePacket(iVID, iState, iFlag);

	return Py_BuildNone();
}

PyObject* netSendPartyUseSkillPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iSkillIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSkillIndex))
		return Py_BuildException();
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 1, &iVID))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyUseSkillPacket(iSkillIndex, iVID);

	return Py_BuildNone();
}

PyObject* netSendPartyParameterPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iMode;
	if (!PyTuple_GetInteger(poArgs, 0, &iMode))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendPartyParameterPacket(iMode);

	return Py_BuildNone();
}

PyObject* netSendSafeboxSaveMoneyPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iMoney;
	if (!PyTuple_GetInteger(poArgs, 0, &iMoney))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxMoneyPacket(SAFEBOX_MONEY_STATE_SAVE, iMoney);

	return Py_BuildNone();
}

PyObject* netSendSafeboxWithdrawMoneyPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iMoney;
	if (!PyTuple_GetInteger(poArgs, 0, &iMoney))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxMoneyPacket(SAFEBOX_MONEY_STATE_WITHDRAW, iMoney);

	return Py_BuildNone();
}

PyObject* netSendSafeboxCheckinPacket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos InventoryPos;
	int iSafeBoxPos;

	switch (PyTuple_Size(poArgs))
	{
	case 2:
		InventoryPos.window_type = INVENTORY;
		if (!PyTuple_GetInteger(poArgs, 0, &InventoryPos.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &iSafeBoxPos))
			return Py_BuildException();
		break;
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &InventoryPos.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &InventoryPos.cell))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &iSafeBoxPos))
			return Py_BuildException();
		break;

	}

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxCheckinPacket(InventoryPos, iSafeBoxPos);

	return Py_BuildNone();
}

PyObject* netSendSafeboxCheckoutPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iSafeBoxPos;
	TItemPos InventoryPos;

	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &iSafeBoxPos))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &InventoryPos.cell))
			return Py_BuildException();
		break;
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &iSafeBoxPos))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &InventoryPos.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &InventoryPos.cell))
			return Py_BuildException();
		break;
	default:
		break;
	}

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxCheckoutPacket(iSafeBoxPos, InventoryPos);

	return Py_BuildNone();
}

PyObject* netSendSafeboxItemMovePacket(PyObject* poSelf, PyObject* poArgs)
{
	int iSourcePos;
	if (!PyTuple_GetInteger(poArgs, 0, &iSourcePos))
		return Py_BuildException();
	int iTargetPos;
	if (!PyTuple_GetInteger(poArgs, 1, &iTargetPos))
		return Py_BuildException();
	int iCount;
	if (!PyTuple_GetInteger(poArgs, 2, &iCount))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSafeBoxItemMovePacket(iSourcePos, iTargetPos, iCount);

	return Py_BuildNone();
}

PyObject* netSendMallCheckoutPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iMallPos;
	TItemPos InventoryPos;

	switch (PyTuple_Size(poArgs))
	{
	case 2:
		if (!PyTuple_GetInteger(poArgs, 0, &iMallPos))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &InventoryPos.cell))
			return Py_BuildException();
		break;
	case 3:
		if (!PyTuple_GetInteger(poArgs, 0, &iMallPos))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 1, &InventoryPos.window_type))
			return Py_BuildException();
		if (!PyTuple_GetInteger(poArgs, 2, &InventoryPos.cell))
			return Py_BuildException();
		break;
	default:
		return Py_BuildException();
	}
	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendMallCheckoutPacket(iMallPos, InventoryPos);

	return Py_BuildNone();
}

PyObject* netSendAnswerMakeGuildPacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendAnswerMakeGuildPacket(szName);

	return Py_BuildNone();
}

PyObject* netSendQuestInputStringPacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szString;
	if (!PyTuple_GetString(poArgs, 0, &szString))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendQuestInputStringPacket(szString);

	return Py_BuildNone();
}

PyObject* netSendQuestConfirmPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iAnswer;
	if (!PyTuple_GetInteger(poArgs, 0, &iAnswer))
		return Py_BuildException();
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 1, &iPID))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendQuestConfirmPacket(iAnswer, iPID);

	return Py_BuildNone();
}

PyObject* netSendGuildAddMemberPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildAddMemberPacket(iVID);

	return Py_BuildNone();
}

PyObject* netSendGuildRemoveMemberPacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szKey;
	if (!PyTuple_GetString(poArgs, 0, &szKey))
		return Py_BuildException();

	CPythonGuild::TGuildMemberData* pGuildMemberData;
	if (!CPythonGuild::Instance().GetMemberDataPtrByName(szKey, &pGuildMemberData))
	{
		TraceError("netSendGuildRemoveMemberPacket(szKey=%s) - Can't Find Guild Member\n", szKey);
		return Py_BuildNone();
	}

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildRemoveMemberPacket(pGuildMemberData->dwPID);

	return Py_BuildNone();
}

PyObject* netSendGuildChangeGradeNamePacket(PyObject* poSelf, PyObject* poArgs)
{
	int iGradeNumber;
	if (!PyTuple_GetInteger(poArgs, 0, &iGradeNumber))
		return Py_BuildException();
	char* szGradeName;
	if (!PyTuple_GetString(poArgs, 1, &szGradeName))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChangeGradeNamePacket(iGradeNumber, szGradeName);

	return Py_BuildNone();
}

PyObject* netSendGuildChangeGradeAuthorityPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iGradeNumber;
	if (!PyTuple_GetInteger(poArgs, 0, &iGradeNumber))
		return Py_BuildException();
	int iAuthority;
	if (!PyTuple_GetInteger(poArgs, 1, &iAuthority))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChangeGradeAuthorityPacket(iGradeNumber, iAuthority);

	return Py_BuildNone();
}

PyObject* netSendGuildOfferPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iExperience;
	if (!PyTuple_GetInteger(poArgs, 0, &iExperience))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildOfferPacket(iExperience);

	return Py_BuildNone();
}

PyObject* netSnedGuildPostCommentPacket(PyObject* poSelf, PyObject* poArgs)
{
	char* szComment;
	if (!PyTuple_GetString(poArgs, 0, &szComment))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildPostCommentPacket(szComment);

	return Py_BuildNone();
}

PyObject* netSnedGuildDeleteCommentPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildDeleteCommentPacket(iIndex);

	return Py_BuildNone();
}

PyObject* netSendGuildRefreshCommentsPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iHightestIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iHightestIndex))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildRefreshCommentsPacket(iHightestIndex);

	return Py_BuildNone();
}

PyObject* netSendGuildChangeMemberGradePacket(PyObject* poSelf, PyObject* poArgs)
{
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPID))
		return Py_BuildException();
	int iGradeNumber;
	if (!PyTuple_GetInteger(poArgs, 1, &iGradeNumber))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChangeMemberGradePacket(iPID, iGradeNumber);

	return Py_BuildNone();
}

PyObject* netSendGuildUseSkillPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iSkillID;
	if (!PyTuple_GetInteger(poArgs, 0, &iSkillID))
		return Py_BuildException();
	int iTargetVID;
	if (!PyTuple_GetInteger(poArgs, 1, &iTargetVID))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildUseSkillPacket(iSkillID, iTargetVID);

	return Py_BuildNone();
}

PyObject* netSendGuildChangeMemberGeneralPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 0, &iPID))
		return Py_BuildException();
	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 1, &iFlag))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChangeMemberGeneralPacket(iPID, iFlag);

	return Py_BuildNone();
}

PyObject* netSendGuildInviteAnswerPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iGuildID;
	if (!PyTuple_GetInteger(poArgs, 0, &iGuildID))
		return Py_BuildException();
	int iAnswer;
	if (!PyTuple_GetInteger(poArgs, 1, &iAnswer))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildInviteAnswerPacket(iGuildID, iAnswer);

	return Py_BuildNone();
}

PyObject* netSendGuildChargeGSPPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iGSP;
	if (!PyTuple_GetInteger(poArgs, 0, &iGSP))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildChargeGSPPacket(iGSP);

	return Py_BuildNone();
}

PyObject* netSendGuildDepositMoneyPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iGSP;
	if (!PyTuple_GetInteger(poArgs, 0, &iGSP))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildDepositMoneyPacket(iGSP);

	return Py_BuildNone();
}

PyObject* netSendGuildWithdrawMoneyPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iGSP;
	if (!PyTuple_GetInteger(poArgs, 0, &iGSP))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendGuildWithdrawMoneyPacket(iGSP);

	return Py_BuildNone();
}

PyObject* netSendRequestRefineInfoPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();

	//	CPythonNetworkStream& rns=CPythonNetworkStream::Instance();
	//	rns.SendRequestRefineInfoPacket(iSlotIndex);
	assert(!"netSendRequestRefineInfoPacket - ���̻� ������� �ʴ� �Լ� �Դϴ�");

	return Py_BuildNone();
}

PyObject* netSendRefinePacket(PyObject* poSelf, PyObject* poArgs)
{
	int iSlotIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iSlotIndex))
		return Py_BuildException();
	int iType;
	if (!PyTuple_GetInteger(poArgs, 1, &iType))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendRefinePacket(iSlotIndex, iType);

	return Py_BuildNone();
}

PyObject* netSendSelectItemPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iItemPos;
	if (!PyTuple_GetInteger(poArgs, 0, &iItemPos))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSelectItemPacket(iItemPos);

	return Py_BuildNone();
}

PyObject* netSetPacketSequenceMode(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	CAccountConnector& rkAccountConnector = CAccountConnector::Instance();
	rns.SetPacketSequenceMode(true);
	rkAccountConnector.SetPacketSequenceMode(true);

	return Py_BuildNone();
}

PyObject* netSetEmpireLanguageMode(PyObject* poSelf, PyObject* poArgs)
{
	int iMode;
	if (!PyTuple_GetInteger(poArgs, 0, &iMode))
		return Py_BuildException();

	//CPythonNetworkStream& rns=CPythonNetworkStream::Instance();
	gs_bEmpireLanuageEnable = iMode;

	return Py_BuildNone();
}

PyObject* netSetSkillGroupFake(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.__TEST_SetSkillGroupFake(iIndex);

	return Py_BuildNone();
}

#include "GuildMarkUploader.h"
#include "GuildMarkDownloader.h"

PyObject* netSendGuildSymbol(PyObject* poSelf, PyObject* poArgs)
{
	char* szIP;
	if (!PyTuple_GetString(poArgs, 0, &szIP))
		return Py_BuildException();
	int iPort;
	if (!PyTuple_GetInteger(poArgs, 1, &iPort))
		return Py_BuildException();
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 2, &szFileName))
		return Py_BuildException();
	int iGuildID;
	if (!PyTuple_GetInteger(poArgs, 3, &iGuildID))
		return Py_BuildException();

	CNetworkAddress kAddress;
	kAddress.Set(szIP, iPort);

	UINT uiError;

	CGuildMarkUploader& rkGuildMarkUploader = CGuildMarkUploader::Instance();
	if (!rkGuildMarkUploader.ConnectToSendSymbol(kAddress, 0, 0, iGuildID, szFileName, &uiError))
	{
		assert(!"Failed connecting to send symbol");
	}

	return Py_BuildNone();
}

PyObject* netDisconnectUploader(PyObject* poSelf, PyObject* poArgs)
{
	CGuildMarkUploader& rkGuildMarkUploader = CGuildMarkUploader::Instance();
	rkGuildMarkUploader.Disconnect();
	return Py_BuildNone();
}

PyObject* netRecvGuildSymbol(PyObject* poSelf, PyObject* poArgs)
{
	char* szIP;
	if (!PyTuple_GetString(poArgs, 0, &szIP))
		return Py_BuildException();
	int iPort;
	if (!PyTuple_GetInteger(poArgs, 1, &iPort))
		return Py_BuildException();
	int iGuildID;
	if (!PyTuple_GetInteger(poArgs, 2, &iGuildID))
		return Py_BuildException();

	CNetworkAddress kAddress;
	kAddress.Set(szIP, iPort);

	std::vector<DWORD> kVec_dwGuildID;
	kVec_dwGuildID.clear();
	kVec_dwGuildID.push_back(iGuildID);

	if (kVec_dwGuildID.size() > 0)
	{
		CGuildMarkDownloader& rkGuildMarkDownloader = CGuildMarkDownloader::Instance();
		if (!rkGuildMarkDownloader.ConnectToRecvSymbol(kAddress, 0, 0, kVec_dwGuildID))
		{
			assert(!"Failed connecting to recv symbol");
		}
	}

	return Py_BuildNone();
}

PyObject* netRegisterErrorLog(PyObject* poSelf, PyObject* poArgs)
{
	char* szLog;
	if (!PyTuple_GetString(poArgs, 0, &szLog))
		return Py_BuildException();

	return Py_BuildNone();
}

#ifdef ENABLE_SEND_TARGET_INFO
PyObject* netTargetInfoLoad(PyObject* poSelf, PyObject* poArgs)
{
	DWORD dwVID;

	if (!PyArg_ParseTuple(poArgs, "i", &dwVID))
	{
		return Py_BuildException();
	}
	if (dwVID < 0)
	{
		return Py_BuildNone();
	}

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendTargetInfoLoadPacket(dwVID);

	return Py_BuildNone();
}
#endif

#ifdef ENABLE_MOVE_CHANNEL
PyObject* netSetMapIndex(PyObject* poSelf, PyObject* poArgs)
{
	int iMapIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iMapIndex))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetMapIndex(iMapIndex);

	return Py_BuildNone();
}

PyObject* netGetMapIndex(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.GetMapIndex());
}

PyObject* netSetChannelNumber(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bChannelNumber;
	if (!PyTuple_GetInteger(poArgs, 0, &bChannelNumber))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SetChannelNumber(bChannelNumber);

	return Py_BuildNone();
}

PyObject* netGetChannelNumber(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.GetChannelNumber());
}
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
PyObject* netSendMiniGameCatchKing(PyObject* poSelf, PyObject* poArgs)
{
	int iSubHeader;
	if (!PyTuple_GetInteger(poArgs, 0, &iSubHeader))
		return Py_BuildException();

	int iSubArgument;
	if (!PyTuple_GetInteger(poArgs, 1, &iSubArgument))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendMiniGameCatchKing(iSubHeader, iSubArgument);
	return Py_BuildNone();
}
#endif

#if defined(ENABLE_PRIVATESHOP_SEARCH_SYSTEM)
PyObject* netSendPrivateShopSearchInfo(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bIdx = 0;

	BYTE bJob;
	if (!PyTuple_GetByte(poArgs, bIdx++, &bJob))
		return Py_BuildException();
	BYTE bMaskType;
	if (!PyTuple_GetByte(poArgs, bIdx++, &bMaskType))
		return Py_BuildException();
	int iMaskSub;
	if (!PyTuple_GetInteger(poArgs, bIdx++, &iMaskSub))
		return Py_BuildException();
	int iMinRefine;
	if (!PyTuple_GetInteger(poArgs, bIdx++, &iMinRefine))
		return Py_BuildException();
	int iMaxRefine;
	if (!PyTuple_GetInteger(poArgs, bIdx++, &iMaxRefine))
		return Py_BuildException();
	int iMinLevel;
	if (!PyTuple_GetInteger(poArgs, bIdx++, &iMinLevel))
		return Py_BuildException();
	int iMaxLevel;
	if (!PyTuple_GetInteger(poArgs, bIdx++, &iMaxLevel))
		return Py_BuildException();
	int iMinGold;
	if (!PyTuple_GetInteger(poArgs, bIdx++, &iMinGold))
		return Py_BuildException();
	int iMaxGold;
	if (!PyTuple_GetInteger(poArgs, bIdx++, &iMaxGold))
		return Py_BuildException();
	char* szItemName;
	if (!PyTuple_GetString(poArgs, bIdx++, &szItemName))
		return Py_BuildException();
#if defined(ENABLE_CHEQUE_SYSTEM)
	int iMinCheque;
	if (!PyTuple_GetInteger(poArgs, bIdx++, &iMinCheque))
		return Py_BuildException();
	int iMaxCheque;
	if (!PyTuple_GetInteger(poArgs, bIdx++, &iMaxCheque))
		return Py_BuildException();
#endif

	CPythonNetworkStream::Instance().SendPrivateShopSearchInfoPacket(bJob, bMaskType, iMaskSub, iMinRefine, iMaxRefine, iMinLevel, iMaxLevel, iMinGold, iMaxGold, szItemName
#if defined(ENABLE_CHEQUE_SYSTEM)
		, iMinCheque, iMaxCheque
#endif
	);
	return Py_BuildNone();
}

PyObject* netSendPrivateShopSearchInfoSub(PyObject* poSelf, PyObject* poArgs)
{
	int iPage;
	if (!PyTuple_GetInteger(poArgs, 0, &iPage))
		return Py_BuildException();

	CPythonNetworkStream::Instance().PrivateShopSearchChangePage(iPage);
	return Py_BuildNone();
}

PyObject* netSendPrivateShopSerchBuyItem(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPrivateShopSearchBuyItem(iIndex);
	return Py_BuildNone();
}

PyObject* netClosePrivateShopSearchWindow(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream::Instance().SendPrivateShopSearchClose();
	return Py_BuildNone();
}
#endif

PyObject* netSendChangeLanguagePacket(PyObject* poSelf, PyObject* poArgs)
{
	int iLanguage;
	if (!PyTuple_GetInteger(poArgs, 0, &iLanguage))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendChangeLanguagePacket(iLanguage);
	return Py_BuildNone();
}

#ifdef ENABLE_SKILLBOOK_COMB_SYSTEM
PyObject* netSendSkillBookCombinationPacket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos CombItemPos[SKILLBOOK_COMB_SLOT_MAX];
	PyObject* pDic;
	if (!PyTuple_GetObject(poArgs, 0, &pDic))
		return Py_BuildException();

	BYTE bAction;
	if (!PyTuple_GetByte(poArgs, 1, &bAction))
		return Py_BuildException();

	int pos = 0;
	PyObject* key, * value;
	int size = PyDict_Size(pDic);

	while (PyDict_Next(pDic, &pos, &key, &value))
	{
		int i = PyInt_AsLong(key);
		if (i > SKILLBOOK_COMB_SLOT_MAX)
			return Py_BuildException();

		if (!PyTuple_GetByte(value, 0, &CombItemPos[i].window_type)
			|| !PyTuple_GetInteger(value, 1, &CombItemPos[i].cell))
		{
			return Py_BuildException();
		}
	}

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendSkillBookCombinationPacket(CombItemPos, bAction);

	return Py_BuildNone();
}
#endif

PyObject* netSendWhisperDetails(PyObject* poSelf, PyObject* poArgs)
{
	char* name;
	if (!PyTuple_GetString(poArgs, 0, &name))
		return Py_BuildException();

	CPythonNetworkStream& net = CPythonNetworkStream::Instance();
	net.SendWhisperDetails(name);

	return Py_BuildNone();
}

PyObject* netIsVerifiedPIN(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bIndex;
	if (!PyTuple_GetByte(poArgs, 0, &bIndex))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.IsVerifiedPIN(bIndex));
}

PyObject* netIsChangedHWID(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("i", rkNetStream.IsChangedHWID());
}

#if defined(ENABLE_MYSHOP_DECO)
PyObject* netSendMyShopDecoSet(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bType;
	if (!PyTuple_GetByte(poArgs, 0, &bType))
		return Py_BuildException();

	int iPolyVnum;
	if (!PyTuple_GetInteger(poArgs, 1, &iPolyVnum))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendMyShopDecoSet(bType, (DWORD)iPolyVnum);

	return Py_BuildNone();
}
#endif

#if defined(ENABLE_MAILBOX)
PyObject* netSendPostWriteConfirm(PyObject* poSelf, PyObject* poArgs)
{
	char* szName;
	if (!PyTuple_GetString(poArgs, 0, &szName))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPostWriteConfirm(szName);
	return Py_BuildNone();
}

PyObject* netSendMailBoxClose(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream::Instance().SendMailBoxClose();
	return Py_BuildNone();
}

PyObject* netSendPostAllDelete(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream::Instance().SendPostAllDelete();
	return Py_BuildNone();
}

PyObject* netSendPostAllGetItems(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream::Instance().SendPostAllGetItems();
	return Py_BuildNone();
}

PyObject* netSendPostDelete(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPostDelete(index);
	return Py_BuildNone();
}

PyObject* netSendPostGetItems(PyObject* poSelf, PyObject* poArgs)
{
	BYTE index;
	if (!PyTuple_GetByte(poArgs, 0, &index))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPostGetItems(index);
	return Py_BuildNone();
}

PyObject* netSendPostWrite(PyObject* poSelf, PyObject* poArgs)
{
	BYTE Idx = 0;

	char* szName;
	if (!PyTuple_GetString(poArgs, Idx++, &szName))
		return Py_BuildException();

	char* szTitle;
	if (!PyTuple_GetString(poArgs, Idx++, &szTitle))
		return Py_BuildException();

	char* szMessage;
	if (!PyTuple_GetString(poArgs, Idx++, &szMessage))
		return Py_BuildException();

	TItemPos Cell;
	if (!PyTuple_GetByte(poArgs, Idx++, &Cell.window_type))
		return Py_BuildException();
	if (!PyTuple_GetInteger(poArgs, Idx++, &Cell.cell))
		return Py_BuildException();

	int iYang;
	if (!PyTuple_GetInteger(poArgs, Idx++, &iYang))
		return Py_BuildException();

	int iWon;
	if (!PyTuple_GetInteger(poArgs, Idx++, &iWon))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPostWrite(szName, szTitle, szMessage, Cell, iYang, iWon);
	return Py_BuildNone();
}

PyObject* netRequestPostAddData(PyObject* poSelf, PyObject* poArgs)
{
	BYTE ButtonIndex;
	if (!PyTuple_GetByte(poArgs, 0, &ButtonIndex))
		return Py_BuildException();

	BYTE DataIndex;
	if (!PyTuple_GetByte(poArgs, 1, &DataIndex))
		return Py_BuildException();

	CPythonNetworkStream::Instance().RequestPostAddData(ButtonIndex, DataIndex);
	return Py_BuildNone();
}
#endif

#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
PyObject* netSendChangeLookCheckIn(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;
	if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
		return Py_BuildException();
	if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
		return Py_BuildException();

	BYTE bChangeLookSlot;
	if (!PyTuple_GetByte(poArgs, 2, &bChangeLookSlot))
		return Py_BuildException();

	if (bChangeLookSlot >= static_cast<BYTE>(EChangeLookSlots::CHANGE_LOOK_SLOT_MAX))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendChangeLookCheckInPacket(Cell, bChangeLookSlot);
	return Py_BuildNone();
}

PyObject* netSendChangeLookCheckOut(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bChangeLookSlot;
	if (!PyTuple_GetByte(poArgs, 0, &bChangeLookSlot))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendChangeLookCheckOutPacket(bChangeLookSlot);
	return Py_BuildNone();
}

PyObject* netSendChangeLookCheckInFreeYangItem(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;
	if (!PyTuple_GetByte(poArgs, 0, &Cell.window_type))
		return Py_BuildException();
	if (!PyTuple_GetInteger(poArgs, 1, &Cell.cell))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendChangeLookCheckInFreeYangItemPacket(Cell);
	return Py_BuildNone();
}

PyObject* netSendChangeLookCheckOutFreeYangItem(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream::Instance().SendChangeLookCheckOutFreeYangItemPacket();
	return Py_BuildNone();
}

PyObject* netSendChangeLookAccept(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream::Instance().SendChangeLookAcceptPacket();
	return Py_BuildNone();
}

PyObject* netSendChangeLookCancel(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream::Instance().SendChangeLookCancelPacket();
	return Py_BuildNone();
}
#endif

void initnet()
{
	static PyMethodDef s_methods[] =
	{
		{ "GetBettingGuildWarValue", netGetBettingGuildWarValue, METH_VARARGS },
		{ "EnableChatInsultFilter", netEnableChatInsultFilter, METH_VARARGS },
		{ "SetServerInfo", netSetServerInfo, METH_VARARGS },
		{ "GetServerInfo", netGetServerInfo, METH_VARARGS },
		{ "ClearServerInfo", netClearServerInfo, METH_VARARGS },
		{ "PreserveServerCommand", netPreserveServerCommand, METH_VARARGS },
		{ "GetPreservedServerCommand", netGetPreservedServerCommand, METH_VARARGS },

		{ "StartGame", netStartGame, METH_VARARGS },
		{ "Warp", netWarp, METH_VARARGS },
		{ "IsTest", netIsTest, METH_VARARGS },
		{ "SetMarkServer", netSetMarkServer, METH_VARARGS },
		{ "IsChatInsultIn", netIsChatInsultIn, METH_VARARGS },
		{ "IsInsultIn", netIsInsultIn, METH_VARARGS },
		{ "LoadInsultList", netLoadInsultList, METH_VARARGS },
		{ "UploadMark", netUploadMark, METH_VARARGS },
		{ "UploadSymbol", netUploadSymbol, METH_VARARGS },
		{ "GetLoginID", netGetLoginID, METH_VARARGS },
		{ "GetGuildID", netGetGuildID, METH_VARARGS },
		{ "GetEmpireID", netGetEmpireID, METH_VARARGS },
		{ "GetMainActorVID", netGetMainActorVID, METH_VARARGS },
		{ "GetMainActorRace", netGetMainActorRace, METH_VARARGS },
		{ "GetMainActorEmpire", netGetMainActorEmpire, METH_VARARGS },
		{ "GetMainActorSkillGroup", netGetMainActorSkillGroup, METH_VARARGS },
		{ "GetAccountCharacterSlotDataInteger", netGetAccountCharacterSlotDataInteger, METH_VARARGS },
		{ "GetAccountCharacterSlotDataString", netGetAccountCharacterSlotDataString, METH_VARARGS },

		// FIELD_MUSIC
		{ "GetFieldMusicFileName", netGetFieldMusicFileName, METH_VARARGS },
		{ "GetFieldMusicVolume", netGetFieldMusicVolume, METH_VARARGS },
		// END_OF_FIELD_MUSIC

		{ "ToggleGameDebugInfo", netToggleGameDebugInfo, METH_VARARGS },
		{ "SetLoginInfo", netSetLoginInfo, METH_VARARGS },
		{ "SetPhaseWindow", netSetPhaseWindow, METH_VARARGS },
		{ "ClearPhaseWindow", netClearPhaseWindow, METH_VARARGS },
		{ "SetServerCommandParserWindow", netSetServerCommandParserWindow, METH_VARARGS },
		{ "SetAccountConnectorHandler", netSetAccountConnectorHandler, METH_VARARGS },
		{ "SetHandler", netSetHandler, METH_VARARGS },
		{ "SetTCPRecvBufferSize", netSetTCPRecvBufferSize, METH_VARARGS },
		{ "SetTCPSendBufferSize", netSetTCPSendBufferSize, METH_VARARGS },
		{ "SetUDPRecvBufferSize", netSetUDPRecvBufferSize, METH_VARARGS },
		{ "DirectEnter", netDirectEnter, METH_VARARGS },

		{ "LogOutGame", netLogOutGame, METH_VARARGS },
		{ "ExitGame", netExitGame, METH_VARARGS },
		{ "ExitApplication", netExitApplication, METH_VARARGS },
		{ "ConnectTCP", netConnectTCP, METH_VARARGS },
		{ "ConnectUDP", netConnectUDP, METH_VARARGS },
		{ "ConnectToAccountServer", netConnectToAccountServer, METH_VARARGS },

		{ "SendLoginPacket", netSendLoginPacket, METH_VARARGS },
		{ "SendSelectEmpirePacket", netSendSelectEmpirePacket, METH_VARARGS },
		{ "SendSelectCharacterPacket", netSendSelectCharacterPacket, METH_VARARGS },
		{ "SendChangeNamePacket", netSendChangeNamePacket, METH_VARARGS },
		{ "SendCreateCharacterPacket", netSendCreateCharacterPacket, METH_VARARGS },
		{ "SendDestroyCharacterPacket", netSendDestroyCharacterPacket, METH_VARARGS },
		{ "SendCharacterPinCodePacket", netSendCharacterPinCodePacket, METH_VARARGS },
		{ "SendEnterGamePacket", netSendEnterGamePacket, METH_VARARGS },

		{ "SendItemUsePacket", netSendItemUsePacket, METH_VARARGS },
		{ "SendItemUseToItemPacket", netSendItemUseToItemPacket, METH_VARARGS },
		{ "SendItemDropPacket", netSendItemDropPacket, METH_VARARGS },
		{ "SendItemDropPacketNew", netSendItemDropPacketNew, METH_VARARGS },
#ifdef ENABLE_NEW_DROP_DIALOG
		{ "SendItemDestroyPacket", netSendItemDestroyPacket, METH_VARARGS },
#endif
		{ "SendElkDropPacket", netSendElkDropPacket, METH_VARARGS },
		{ "SendGoldDropPacketNew", netSendGoldDropPacketNew, METH_VARARGS },
#if defined(ENABLE_CHEQUE_SYSTEM)
		{ "SendChequeDropPacket", netSendChequeDropPacket, METH_VARARGS },
		{ "SendChequeDropPacketNew", netSendChequeDropPacketNew, METH_VARARGS },
#endif
		{ "SendItemMovePacket", netSendItemMovePacket, METH_VARARGS },
		{ "SendItemPickUpPacket", netSendItemPickUpPacket, METH_VARARGS },
		{ "SendGiveItemPacket", netSendGiveItemPacket, METH_VARARGS },

		{ "SetOfflinePhase", netSetOfflinePhase, METH_VARARGS },
		{ "Disconnect", netDisconnect, METH_VARARGS },
		{ "IsConnect", netIsConnect, METH_VARARGS },

		{ "SendChatPacket", netSendChatPacket, METH_VARARGS },
		{ "SendEmoticon", netSendEmoticon, METH_VARARGS },
		{ "SendWhisperPacket", netSendWhisperPacket, METH_VARARGS },

		{ "SendCharacterPositionPacket", netSendCharacterPositionPacket, METH_VARARGS },

		{ "SendShopEndPacket", netSendShopEndPacket, METH_VARARGS },
		{ "SendShopBuyPacket", netSendShopBuyPacket, METH_VARARGS },
		{ "SendShopSellPacket", netSendShopSellPacket, METH_VARARGS },
		{ "SendShopSellPacketNew", netSendShopSellPacketNew, METH_VARARGS },

		{ "SendExchangeStartPacket", netSendExchangeStartPacket, METH_VARARGS },
		{ "SendExchangeItemAddPacket", netSendExchangeItemAddPacket, METH_VARARGS },
		{ "SendExchangeItemDelPacket", netSendExchangeItemDelPacket, METH_VARARGS },
		{ "SendExchangeElkAddPacket", netSendExchangeElkAddPacket, METH_VARARGS },
#if defined(ENABLE_CHEQUE_SYSTEM)
		{ "SendExchangeChequeAddPacket", netSendExchangeChequeAddPacket, METH_VARARGS },
#endif
		{ "SendExchangeAcceptPacket", netSendExchangeAcceptPacket, METH_VARARGS },
		{ "SendExchangeExitPacket", netSendExchangeExitPacket, METH_VARARGS },

		{ "SendOnClickPacket", netOnClickPacket, METH_VARARGS },

		// Emoticon String
		{ "RegisterEmoticonString", netRegisterEmoticonString, METH_VARARGS },

		// Messenger
		{ "SendMessengerAddByVIDPacket", netSendMessengerAddByVIDPacket, METH_VARARGS },
		{ "SendMessengerAddByNamePacket", netSendMessengerAddByNamePacket, METH_VARARGS },
		{ "SendMessengerRemovePacket", netSendMessengerRemovePacket, METH_VARARGS },
#ifdef ENABLE_MESSENGER_BLOCK
		{ "SendMessengerBlockAddByVIDPacket", netSendMessengerBlockAddByVIDPacket, METH_VARARGS },
		{ "SendMessengerBlockAddByNamePacket", netSendMessengerBlockAddByNamePacket, METH_VARARGS },
		{ "SendMessengerBlockRemoveByVIDPacket", netSendMessengerBlockRemoveByVIDPacket, METH_VARARGS },
#endif

		// Party
		{ "SendPartyInvitePacket", netSendPartyInvitePacket, METH_VARARGS },
		{ "SendPartyInviteAnswerPacket", netSendPartyInviteAnswerPacket, METH_VARARGS },
		{ "SendPartyExitPacket", netSendPartyExitPacket, METH_VARARGS },
		{ "SendPartyRemovePacket", netSendPartyRemovePacket, METH_VARARGS },
		{ "SendPartySetStatePacket", netSendPartySetStatePacket, METH_VARARGS },
		{ "SendPartyUseSkillPacket", netSendPartyUseSkillPacket, METH_VARARGS },
		{ "SendPartyParameterPacket", netSendPartyParameterPacket, METH_VARARGS },

		// Safebox
		{ "SendSafeboxSaveMoneyPacket", netSendSafeboxSaveMoneyPacket, METH_VARARGS },
		{ "SendSafeboxWithdrawMoneyPacket", netSendSafeboxWithdrawMoneyPacket, METH_VARARGS },
		{ "SendSafeboxCheckinPacket", netSendSafeboxCheckinPacket, METH_VARARGS },
		{ "SendSafeboxCheckoutPacket", netSendSafeboxCheckoutPacket, METH_VARARGS },
		{ "SendSafeboxItemMovePacket", netSendSafeboxItemMovePacket, METH_VARARGS },

		// Mall
		{ "SendMallCheckoutPacket", netSendMallCheckoutPacket, METH_VARARGS },

		// Guild
		{ "SendAnswerMakeGuildPacket", netSendAnswerMakeGuildPacket, METH_VARARGS },
		{ "SendQuestInputStringPacket", netSendQuestInputStringPacket, METH_VARARGS },
		{ "SendQuestConfirmPacket", netSendQuestConfirmPacket, METH_VARARGS },
		{ "SendGuildAddMemberPacket", netSendGuildAddMemberPacket, METH_VARARGS },
		{ "SendGuildRemoveMemberPacket", netSendGuildRemoveMemberPacket, METH_VARARGS },
		{ "SendGuildChangeGradeNamePacket", netSendGuildChangeGradeNamePacket, METH_VARARGS },
		{ "SendGuildChangeGradeAuthorityPacket", netSendGuildChangeGradeAuthorityPacket, METH_VARARGS },
		{ "SendGuildOfferPacket", netSendGuildOfferPacket, METH_VARARGS },
		{ "SendGuildPostCommentPacket", netSnedGuildPostCommentPacket, METH_VARARGS },
		{ "SendGuildDeleteCommentPacket", netSnedGuildDeleteCommentPacket, METH_VARARGS },
		{ "SendGuildRefreshCommentsPacket", netSendGuildRefreshCommentsPacket, METH_VARARGS },
		{ "SendGuildChangeMemberGradePacket", netSendGuildChangeMemberGradePacket, METH_VARARGS },
		{ "SendGuildUseSkillPacket", netSendGuildUseSkillPacket, METH_VARARGS },
		{ "SendGuildChangeMemberGeneralPacket", netSendGuildChangeMemberGeneralPacket, METH_VARARGS },
		{ "SendGuildInviteAnswerPacket", netSendGuildInviteAnswerPacket, METH_VARARGS },
		{ "SendGuildChargeGSPPacket", netSendGuildChargeGSPPacket, METH_VARARGS },
		{ "SendGuildDepositMoneyPacket", netSendGuildDepositMoneyPacket, METH_VARARGS },
		{ "SendGuildWithdrawMoneyPacket", netSendGuildWithdrawMoneyPacket, METH_VARARGS },

		// Refine
		{ "SendRequestRefineInfoPacket", netSendRequestRefineInfoPacket, METH_VARARGS },
		{ "SendRefinePacket", netSendRefinePacket, METH_VARARGS },
		{ "SendSelectItemPacket", netSendSelectItemPacket, METH_VARARGS },

		// SYSTEM
		{ "SetPacketSequenceMode", netSetPacketSequenceMode, METH_VARARGS },
		{ "SetEmpireLanguageMode", netSetEmpireLanguageMode, METH_VARARGS },

		// For Test
		{ "SetSkillGroupFake", netSetSkillGroupFake, METH_VARARGS },

		// Guild Symbol
		{ "SendGuildSymbol", netSendGuildSymbol, METH_VARARGS },
		{ "DisconnectUploader", netDisconnectUploader, METH_VARARGS },
		{ "RecvGuildSymbol", netRecvGuildSymbol, METH_VARARGS },

		// Log
		{ "RegisterErrorLog", netRegisterErrorLog, METH_VARARGS },

#ifdef ENABLE_SEND_TARGET_INFO
		{ "SendTargetInfoLoad", netTargetInfoLoad, METH_VARARGS },
#endif

#ifdef ENABLE_MOVE_CHANNEL
		{ "SetMapIndex", netSetMapIndex, METH_VARARGS },
		{ "GetMapIndex", netGetMapIndex, METH_VARARGS },
		{ "SetChannelNumber", netSetChannelNumber, METH_VARARGS },
		{ "GetChannelNumber", netGetChannelNumber, METH_VARARGS },
#endif

#ifdef ENABLE_MINI_GAME_CATCH_KING
		{ "SendMiniGameCatchKing", netSendMiniGameCatchKing, METH_VARARGS },
#endif

#if defined(ENABLE_PRIVATESHOP_SEARCH_SYSTEM)
		{ "SendPrivateShopSearchInfo", netSendPrivateShopSearchInfo, METH_VARARGS },
		{ "SendPrivateShopSearchInfoSub", netSendPrivateShopSearchInfoSub, METH_VARARGS },
		{ "SendPrivateShopSerchBuyItem", netSendPrivateShopSerchBuyItem, METH_VARARGS },
		{ "ClosePrivateShopSearchWindow", netClosePrivateShopSearchWindow, METH_VARARGS },
#endif

		{ "SendChangeLanguagePacket", netSendChangeLanguagePacket, METH_VARARGS },

#if defined(ENABLE_SKILLBOOK_COMB_SYSTEM)
		{ "SendSkillBookCombinationPacket", netSendSkillBookCombinationPacket, METH_VARARGS },
#endif

		{ "SendWhisperDetails", netSendWhisperDetails, METH_VARARGS },

		{ "IsVerifiedPIN", netIsVerifiedPIN, METH_VARARGS },
		{ "IsChangedHWID", netIsChangedHWID, METH_VARARGS },

#if defined(ENABLE_MYSHOP_DECO)
		{ "SendMyShopDecoSet", netSendMyShopDecoSet, METH_VARARGS },
#endif

#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
		// Change Look
		{ "SendChangeLookCheckIn", netSendChangeLookCheckIn, METH_VARARGS },
		{ "SendChangeLookCheckOut", netSendChangeLookCheckOut, METH_VARARGS },
		{ "SendChangeLookCheckInFreeYangItem", netSendChangeLookCheckInFreeYangItem, METH_VARARGS },
		{ "SendChangeLookCheckOutFreeYangItem", netSendChangeLookCheckOutFreeYangItem, METH_VARARGS },
		{ "SendChangeLookCheckOut", netSendChangeLookCheckOut, METH_VARARGS },
		{ "SendChangeLookAccept", netSendChangeLookAccept, METH_VARARGS },
		{ "SendChangeLookCanCle", netSendChangeLookCancel, METH_VARARGS },
#endif

#if defined(ENABLE_MAILBOX)
		// MailBox
		{ "SendPostDelete", netSendPostDelete, METH_VARARGS },
		{ "SendPostGetItems", netSendPostGetItems, METH_VARARGS },
		{ "SendPostWriteConfirm", netSendPostWriteConfirm, METH_VARARGS },
		{ "SendPostWrite", netSendPostWrite, METH_VARARGS },
		{ "SendMailBoxClose", netSendMailBoxClose, METH_VARARGS },
		{ "SendPostAllDelete", netSendPostAllDelete, METH_VARARGS },
		{ "SendPostAllGetItems", netSendPostAllGetItems, METH_VARARGS },
		{ "RequestPostAddData", netRequestPostAddData, METH_VARARGS },
#endif

		{ NULL, NULL, NULL },
	};

	PyObject* poModule = Py_InitModule("net", s_methods);

	PyModule_AddIntConstant(poModule, "ERROR_NONE", CPythonNetworkStream::ERROR_NONE);
	PyModule_AddIntConstant(poModule, "ERROR_CONNECT_MARK_SERVER", CPythonNetworkStream::ERROR_CONNECT_MARK_SERVER);
	PyModule_AddIntConstant(poModule, "ERROR_LOAD_MARK", CPythonNetworkStream::ERROR_LOAD_MARK);
	PyModule_AddIntConstant(poModule, "ERROR_MARK_WIDTH", CPythonNetworkStream::ERROR_MARK_WIDTH);
	PyModule_AddIntConstant(poModule, "ERROR_MARK_HEIGHT", CPythonNetworkStream::ERROR_MARK_HEIGHT);

	// MARK_BUG_FIX
	PyModule_AddIntConstant(poModule, "ERROR_MARK_UPLOAD_NEED_RECONNECT", CPythonNetworkStream::ERROR_MARK_UPLOAD_NEED_RECONNECT);
	PyModule_AddIntConstant(poModule, "ERROR_MARK_CHECK_NEED_RECONNECT", CPythonNetworkStream::ERROR_MARK_CHECK_NEED_RECONNECT);
	// END_OF_MARK_BUG_FIX

	PyModule_AddIntConstant(poModule, "PHASE_WINDOW_LOGIN", CPythonNetworkStream::PHASE_WINDOW_LOGIN);
	PyModule_AddIntConstant(poModule, "PHASE_WINDOW_SELECT", CPythonNetworkStream::PHASE_WINDOW_SELECT);
	PyModule_AddIntConstant(poModule, "PHASE_WINDOW_CREATE", CPythonNetworkStream::PHASE_WINDOW_CREATE);
	PyModule_AddIntConstant(poModule, "PHASE_WINDOW_LOAD", CPythonNetworkStream::PHASE_WINDOW_LOAD);
	PyModule_AddIntConstant(poModule, "PHASE_WINDOW_GAME", CPythonNetworkStream::PHASE_WINDOW_GAME);
	PyModule_AddIntConstant(poModule, "PHASE_WINDOW_EMPIRE", CPythonNetworkStream::PHASE_WINDOW_EMPIRE);
	PyModule_AddIntConstant(poModule, "PHASE_WINDOW_LOGO", CPythonNetworkStream::PHASE_WINDOW_LOGO);

	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_ID", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_ID);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_NAME", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_NAME);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_RACE", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_RACE);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_LEVEL", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_LEVEL);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_STR", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_STR);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_DEX", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_DEX);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_INT", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_INT);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_HTH", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_HTH);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_PLAYTIME", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_PLAYTIME);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_FORM", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_FORM);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_ADDR", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_ADDR);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_PORT", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_PORT);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_GUILD_ID", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_GUILD_ID);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_GUILD_NAME", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_GUILD_NAME);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_CHANGE_NAME_FLAG);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_HAIR", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_HAIR);
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_ACCE", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_ACCE);
#endif
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_LAST_PLAYTIME", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_LAST_PLAYTIME);
#if defined(ENABLE_CONQUEROR_LEVEL)
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_CONQUEROR_LEVEL", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_CONQUEROR_LEVEL);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_SUNGMA_STR", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_SUNGMA_STR);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_SUNGMA_HP", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_SUNGMA_HP);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_SUNGMA_MOVE", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_SUNGMA_MOVE);
	PyModule_AddIntConstant(poModule, "ACCOUNT_CHARACTER_SLOT_SUNGMA_IMMUNE", CPythonNetworkStream::ACCOUNT_CHARACTER_SLOT_SUNGMA_IMMUNE);
#endif

	PyModule_AddIntConstant(poModule, "SERVER_COMMAND_LOG_OUT", CPythonNetworkStream::SERVER_COMMAND_LOG_OUT);
	PyModule_AddIntConstant(poModule, "SERVER_COMMAND_RETURN_TO_SELECT_CHARACTER", CPythonNetworkStream::SERVER_COMMAND_RETURN_TO_SELECT_CHARACTER);
	PyModule_AddIntConstant(poModule, "SERVER_COMMAND_QUIT", CPythonNetworkStream::SERVER_COMMAND_QUIT);

	PyModule_AddIntConstant(poModule, "EMPIRE_A", 1);
	PyModule_AddIntConstant(poModule, "EMPIRE_B", 2);
	PyModule_AddIntConstant(poModule, "EMPIRE_C", 3);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL", DS_SUB_HEADER_REFINE_FAIL);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE", DS_SUB_HEADER_REFINE_FAIL_MAX_REFINE);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL", DS_SUB_HEADER_REFINE_FAIL_INVALID_MATERIAL);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY", DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MONEY);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL", DS_SUB_HEADER_REFINE_FAIL_NOT_ENOUGH_MATERIAL);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL", DS_SUB_HEADER_REFINE_FAIL_TOO_MUCH_MATERIAL);
	PyModule_AddIntConstant(poModule, "DS_SUB_HEADER_REFINE_SUCCEED", DS_SUB_HEADER_REFINE_SUCCEED);
}