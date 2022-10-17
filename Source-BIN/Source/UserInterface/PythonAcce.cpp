#include "StdAfx.h"

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
#include "PythonAcce.h"
#include "PythonNetworkStream.h"

CPythonAcce::CPythonAcce()
{
	Clear();
}

CPythonAcce::~CPythonAcce()
{
	Clear();
}

void CPythonAcce::Clear()
{
	dwPrice = 0;

	ZeroMemory(&m_vAcceResult, sizeof(m_vAcceResult));

	m_vAcceMaterials.clear();
	m_vAcceMaterials.resize(CPythonAcce::WINDOW_MAX_MATERIALS);

	ZeroMemory(m_vAcceMaterials.data(), m_vAcceMaterials.size() * sizeof(m_vAcceMaterials[0]));
}

void CPythonAcce::AddMaterial(DWORD dwRefPrice, BYTE bPos, TItemPos tPos)
{
	if (bPos >= CPythonAcce::WINDOW_MAX_MATERIALS)
		return;

	if (bPos == 0)
		dwPrice = dwRefPrice;

	TAcceMaterial& tMaterial = m_vAcceMaterials[bPos];
	tMaterial.bHere = 1;
	tMaterial.wCell = tPos.cell;
}

void CPythonAcce::AddResult(DWORD dwItemVnum, DWORD dwMinAbs, DWORD dwMaxAbs)
{
	TAcceResult& tResult = m_vAcceResult;
	tResult.dwItemVnum = dwItemVnum;
	tResult.dwMinAbs = dwMinAbs;
	tResult.dwMaxAbs = dwMaxAbs;
}

void CPythonAcce::RemoveMaterial(DWORD dwRefPrice, BYTE bPos)
{
	if (bPos >= CPythonAcce::WINDOW_MAX_MATERIALS)
		return;

	if (bPos == 1)
		dwPrice = dwRefPrice;

	memset(&m_vAcceMaterials[bPos], 0, sizeof(TAcceMaterial));
}

bool CPythonAcce::GetAttachedItem(BYTE bPos, BYTE& bHere, WORD& wCell)
{
	if (bPos >= CPythonAcce::WINDOW_MAX_MATERIALS)
		return false;

	bHere = m_vAcceMaterials[bPos].bHere;
	wCell = m_vAcceMaterials[bPos].wCell;
	return true;
}

void CPythonAcce::GetResultItem(DWORD& dwItemVnum, DWORD& dwMinAbs, DWORD& dwMaxAbs)
{
	dwItemVnum = m_vAcceResult.dwItemVnum;
	dwMinAbs = m_vAcceResult.dwMinAbs;
	dwMaxAbs = m_vAcceResult.dwMaxAbs;
}

BOOL CPythonAcce::IsCleanScroll(DWORD dwItemVnum)
{
	switch (dwItemVnum)
	{
	case 39046:
	case 90000:
		return TRUE;
	}

	return FALSE;
}

PyObject* acceSendCloseRequest(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendAcceClosePacket();
	return Py_BuildNone();
}

PyObject* acceAdd(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bPos = 0;
	TItemPos tPos;

	if (!PyTuple_GetInteger(poArgs, 0, &tPos.window_type))
		return Py_BuildException();
	else if (!PyTuple_GetInteger(poArgs, 1, &tPos.cell))
		return Py_BuildException();
	else if (!PyTuple_GetInteger(poArgs, 2, &bPos))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendAcceAddPacket(tPos, bPos);
	return Py_BuildNone();
}

PyObject* acceRemove(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bPos))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendAcceRemovePacket(bPos);
	return Py_BuildNone();
}

PyObject* acceGetPrice(PyObject* poSelf, PyObject* poArgs)
{
	return Py_BuildValue("i", CPythonAcce::Instance().GetPrice());
}

PyObject* acceGetAttachedItem(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bPos;
	if (!PyTuple_GetInteger(poArgs, 0, &bPos))
		return Py_BuildException();

	BYTE bHere;
	WORD wCell;
	bool bGet = CPythonAcce::Instance().GetAttachedItem(bPos, bHere, wCell);
	if (!bGet)
	{
		bHere = 0;
		wCell = 0;
	}

	return Py_BuildValue("ii", bHere, wCell);
}

PyObject* acceGetResultItem(PyObject* poSelf, PyObject* poArgs)
{
	DWORD dwItemVnum, dwMinAbs, dwMaxAbs;
	CPythonAcce::Instance().GetResultItem(dwItemVnum, dwMinAbs, dwMaxAbs);
	return Py_BuildValue("iii", dwItemVnum, dwMinAbs, dwMaxAbs);
}

PyObject* acceSendRefineRequest(PyObject* poSelf, PyObject* poArgs)
{
	BYTE bHere;
	WORD wCell;
	bool bGet = CPythonAcce::Instance().GetAttachedItem(1, bHere, wCell);
	if (bGet)
	{
		if (bHere)
		{
			CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
			rkNetStream.SendAcceRefinePacket();
		}
	}

	return Py_BuildNone();
}

PyObject* acceIsCleanScroll(PyObject* poSelf, PyObject* poArgs)
{
	int iIndex;
	if (!PyTuple_GetInteger(poArgs, 0, &iIndex))
		return Py_BuildException();

	return Py_BuildValue("i", CPythonAcce::Instance().IsCleanScroll(iIndex));
}

void initAcce()
{
	static PyMethodDef functions[] = {
		{ "SendCloseRequest", acceSendCloseRequest, METH_VARARGS },
		{ "Add", acceAdd, METH_VARARGS },
		{ "Remove", acceRemove, METH_VARARGS },
		{ "GetPrice", acceGetPrice, METH_VARARGS },
		{ "GetAttachedItem", acceGetAttachedItem, METH_VARARGS },
		{ "GetResultItem", acceGetResultItem, METH_VARARGS },
		{ "SendRefineRequest", acceSendRefineRequest, METH_VARARGS },
		{ "IsCleanScroll", acceIsCleanScroll, METH_VARARGS },
		{ NULL, NULL, NULL },
	};

	PyObject* pModule = Py_InitModule("acce", functions);

	PyModule_AddIntConstant(pModule, "ABSORPTION_SOCKET", CPythonAcce::ABSORPTION_SOCKET);
	PyModule_AddIntConstant(pModule, "ABSORBED_SOCKET", CPythonAcce::ABSORBED_SOCKET);
	PyModule_AddIntConstant(pModule, "WINDOW_MAX_MATERIALS", CPythonAcce::WINDOW_MAX_MATERIALS);
	PyModule_AddIntConstant(pModule, "LIMIT_RANGE", CPythonAcce::LIMIT_RANGE);
}
#endif
