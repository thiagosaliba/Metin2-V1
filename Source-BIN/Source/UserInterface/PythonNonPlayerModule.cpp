#include "StdAfx.h"
#include "PythonNonPlayer.h"
#include "InstanceBase.h"
#include "PythonCharacterManager.h"

PyObject* nonplayerGetEventType(PyObject* poSelf, PyObject* poArgs)
{
	int iVirtualNumber;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualNumber))
		return Py_BuildException();

	BYTE iType = CPythonNonPlayer::Instance().GetEventType(iVirtualNumber);

	return Py_BuildValue("i", iType);
}

PyObject* nonplayerGetEventTypeByVID(PyObject* poSelf, PyObject* poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BuildException();

	BYTE iType = CPythonNonPlayer::Instance().GetEventTypeByVID(iVirtualID);

	return Py_BuildValue("i", iType);
}

PyObject* nonplayerGetLevelByVID(PyObject* poSelf, PyObject* poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pInstance)
		return Py_BuildValue("i", -1);

	const CPythonNonPlayer::TMobTable* pMobTable = CPythonNonPlayer::Instance().GetTable(pInstance->GetVirtualNumber());

	if (!pMobTable)
		return Py_BuildValue("i", -1);

	float fAverageLevel = pMobTable->bLevel; //(float(pMobTable->abLevelRange[0]) + float(pMobTable->abLevelRange[1])) / 2.0f;
	fAverageLevel = floor(fAverageLevel + 0.5f);
	return Py_BuildValue("i", int(fAverageLevel));
}

PyObject* nonplayerGetGradeByVID(PyObject* poSelf, PyObject* poArgs)
{
	int iVirtualID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVirtualID))
		return Py_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVirtualID);

	if (!pInstance)
		return Py_BuildValue("i", -1);

	const CPythonNonPlayer::TMobTable* pMobTable = CPythonNonPlayer::Instance().GetTable(pInstance->GetVirtualNumber());

	if (!pMobTable)
		return Py_BuildValue("i", -1);

	return Py_BuildValue("i", pMobTable->bRank);
}

PyObject* nonplayerGetMonsterName(PyObject* poSelf, PyObject* poArgs)
{
	int iVNum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVNum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
	return Py_BuildValue("s", rkNonPlayer.GetMonsterName(iVNum));
}

PyObject* nonplayerLoadNonPlayerData(PyObject* poSelf, PyObject* poArgs)
{
	char* szFileName;
	if (!PyTuple_GetString(poArgs, 0, &szFileName))
		return Py_BuildException();

	CPythonNonPlayer::Instance().LoadNonPlayerData(szFileName);
	return Py_BuildNone();
}

#ifdef ENABLE_SEND_TARGET_INFO
PyObject* nonplayerGetMonsterMaxHP(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterMaxHP(iVnum));
}

PyObject* nonplayerGetRaceNumByVID(PyObject* poSelf, PyObject* poArgs)
{
	int iVID;
	if (!PyTuple_GetInteger(poArgs, 0, &iVID))
		return Py_BuildException();

	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetInstancePtr(iVID);

	if (!pInstance)
		return Py_BuildValue("i", -1);

	const CPythonNonPlayer::TMobTable* pMobTable = CPythonNonPlayer::Instance().GetTable(pInstance->GetVirtualNumber());

	if (!pMobTable)
		return Py_BuildValue("i", -1);

	return Py_BuildValue("i", pMobTable->dwVnum);
}

PyObject* nonplayerGetMonsterRaceFlag(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterRaceFlag(iVnum));
}

PyObject* nonplayerIsMonsterRaceFlag(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	int iFlag;
	if (!PyTuple_GetInteger(poArgs, 1, &iFlag))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.IsMonsterRaceFlag(iVnum, iFlag) ? 1 : 0);
}

PyObject* nonplayerGetMonsterLevel(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterLevel(iVnum));
}

PyObject* nonplayerGetMonsterDamage(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	DWORD dwDmg1 = rkNonPlayer.GetMonsterDamage1(iVnum);
	DWORD dwDmg2 = rkNonPlayer.GetMonsterDamage2(iVnum);

	return Py_BuildValue("ii", dwDmg1, dwDmg2);
}

PyObject* nonplayerGetMonsterExp(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterExp(iVnum));
}

PyObject* nonplayerGetMonsterDamageMultiply(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("f", rkNonPlayer.GetMonsterDamageMultiply(iVnum));
}

PyObject* nonplayerGetMonsterST(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterST(iVnum));
}

PyObject* nonplayerGetMonsterDX(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterDX(iVnum));
}

PyObject* nonplayerIsMonsterStone(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.IsMonsterStone(iVnum) ? 1 : 0);
}

PyObject* nonplayerGetMonsterRegenCycle(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterRegenCycle(iVnum));
}

PyObject* nonplayerGetMonsterRegenPercent(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterRegenPercent(iVnum));
}

PyObject* nonplayerGetMonsterGoldMin(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterGoldMin(iVnum));
}

PyObject* nonplayerGetMonsterGoldMax(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterGoldMax(iVnum));
}

PyObject* nonplayerGetMonsterResist(PyObject* poSelf, PyObject* poArgs)
{
	int iVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iVnum))
		return Py_BuildException();

	BYTE bResistNum;
	if (!PyTuple_GetInteger(poArgs, 1, &bResistNum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();

	return Py_BuildValue("i", rkNonPlayer.GetMonsterResist(iVnum, bResistNum));
}

PyObject* nonplayerGetMonsterDropCount(PyObject* poSelf, PyObject* poArgs)
{
	int iRaceVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iRaceVnum))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
	return Py_BuildValue("i", rkNonPlayer.GetMonsterDropCount(iRaceVnum));
}

PyObject* nonplayerGetMonsterDropItem(PyObject* poSelf, PyObject* poArgs)
{
	int iRaceVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iRaceVnum))
		return Py_BuildException();

	BYTE bIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &bIndex))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
	return Py_BuildValue("ii",
		rkNonPlayer.GetMonsterDropItemVnum(iRaceVnum, bIndex),
		rkNonPlayer.GetMonsterDropItemCount(iRaceVnum, bIndex)
	);
}

PyObject* nonplayerGetMonsterDropItemVnum(PyObject* poSelf, PyObject* poArgs)
{
	int iRaceVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iRaceVnum))
		return Py_BuildException();

	BYTE bIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &bIndex))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
	return Py_BuildValue("i", rkNonPlayer.GetMonsterDropItemVnum(iRaceVnum, bIndex));
}

PyObject* nonplayerGetMonsterDropItemCount(PyObject* poSelf, PyObject* poArgs)
{
	int iRaceVnum;
	if (!PyTuple_GetInteger(poArgs, 0, &iRaceVnum))
		return Py_BuildException();

	BYTE bIndex;
	if (!PyTuple_GetInteger(poArgs, 1, &bIndex))
		return Py_BuildException();

	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
	return Py_BuildValue("i", rkNonPlayer.GetMonsterDropItemCount(iRaceVnum, bIndex));
}

PyObject* nonplayerClearMonsterDrop(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
	rkNonPlayer.ClearItemDropInfo();
	return Py_BuildNone();
}
#endif

void initNonPlayer()
{
	static PyMethodDef s_methods[] =
	{
		{ "GetEventType", nonplayerGetEventType, METH_VARARGS },
		{ "GetEventTypeByVID", nonplayerGetEventTypeByVID, METH_VARARGS },
		{ "GetLevelByVID", nonplayerGetLevelByVID, METH_VARARGS },
		{ "GetGradeByVID", nonplayerGetGradeByVID, METH_VARARGS },
		{ "GetMonsterName", nonplayerGetMonsterName, METH_VARARGS },

#ifdef ENABLE_SEND_TARGET_INFO
		{ "GetRaceNumByVID", nonplayerGetRaceNumByVID, METH_VARARGS },
		{ "GetMonsterMaxHP", nonplayerGetMonsterMaxHP, METH_VARARGS },
		{ "GetMonsterRaceFlag", nonplayerGetMonsterRaceFlag, METH_VARARGS },
		{ "IsMonsterRaceFlag", nonplayerIsMonsterRaceFlag, METH_VARARGS },
		{ "GetMonsterLevel", nonplayerGetMonsterLevel, METH_VARARGS },
		{ "GetMonsterDamage", nonplayerGetMonsterDamage, METH_VARARGS },
		{ "GetMonsterExp", nonplayerGetMonsterExp, METH_VARARGS },
		{ "GetMonsterDamageMultiply", nonplayerGetMonsterDamageMultiply, METH_VARARGS },
		{ "GetMonsterST", nonplayerGetMonsterST, METH_VARARGS },
		{ "GetMonsterDX", nonplayerGetMonsterDX, METH_VARARGS },
		{ "IsMonsterStone", nonplayerIsMonsterStone, METH_VARARGS },
		{ "GetMonsterRegenCycle", nonplayerGetMonsterRegenCycle, METH_VARARGS },
		{ "GetMonsterRegenPercent", nonplayerGetMonsterRegenPercent, METH_VARARGS },
		{ "GetMonsterGoldMin", nonplayerGetMonsterGoldMin, METH_VARARGS },
		{ "GetMonsterGoldMax", nonplayerGetMonsterGoldMax, METH_VARARGS },
		{ "GetMonsterResist", nonplayerGetMonsterResist, METH_VARARGS },

		{ "GetMonsterDropCount", nonplayerGetMonsterDropCount, METH_VARARGS },
		{ "GetMonsterDropItem", nonplayerGetMonsterDropItem, METH_VARARGS },
		{ "GetMonsterDropItemVnum", nonplayerGetMonsterDropItemVnum, METH_VARARGS },
		{ "GetMonsterDropItemCount", nonplayerGetMonsterDropItemCount, METH_VARARGS },
		{ "ClearMonsterDrop", nonplayerClearMonsterDrop, METH_VARARGS },
#endif

		{ "LoadNonPlayerData", nonplayerLoadNonPlayerData, METH_VARARGS },

		{ NULL, NULL, NULL },
	};

	PyObject* poModule = Py_InitModule("nonplayer", s_methods);

	PyModule_AddIntConstant(poModule, "ON_CLICK_EVENT_NONE", CPythonNonPlayer::ON_CLICK_EVENT_NONE);
	PyModule_AddIntConstant(poModule, "ON_CLICK_EVENT_BATTLE", CPythonNonPlayer::ON_CLICK_EVENT_BATTLE);
	PyModule_AddIntConstant(poModule, "ON_CLICK_EVENT_SHOP", CPythonNonPlayer::ON_CLICK_EVENT_SHOP);
	PyModule_AddIntConstant(poModule, "ON_CLICK_EVENT_TALK", CPythonNonPlayer::ON_CLICK_EVENT_TALK);
	PyModule_AddIntConstant(poModule, "ON_CLICK_EVENT_VEHICLE", CPythonNonPlayer::ON_CLICK_EVENT_VEHICLE);

	PyModule_AddIntConstant(poModule, "PAWN", CPythonNonPlayer::MOB_RANK_PAWN);
	PyModule_AddIntConstant(poModule, "S_PAWN", CPythonNonPlayer::MOB_RANK_S_PAWN);
	PyModule_AddIntConstant(poModule, "KNIGHT", CPythonNonPlayer::MOB_RANK_KNIGHT);
	PyModule_AddIntConstant(poModule, "S_KNIGHT", CPythonNonPlayer::MOB_RANK_S_KNIGHT);
	PyModule_AddIntConstant(poModule, "BOSS", CPythonNonPlayer::MOB_RANK_BOSS);
	PyModule_AddIntConstant(poModule, "KING", CPythonNonPlayer::MOB_RANK_KING);

	PyModule_AddIntConstant(poModule, "MOB_RESIST_SWORD", CPythonNonPlayer::MOB_RESIST_SWORD);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_TWOHAND", CPythonNonPlayer::MOB_RESIST_TWOHAND);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_DAGGER", CPythonNonPlayer::MOB_RESIST_DAGGER);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_BELL", CPythonNonPlayer::MOB_RESIST_BELL);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_FAN", CPythonNonPlayer::MOB_RESIST_FAN);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_BOW", CPythonNonPlayer::MOB_RESIST_BOW);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_CLAW", CPythonNonPlayer::MOB_RESIST_CLAW);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_FIRE", CPythonNonPlayer::MOB_RESIST_FIRE);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_ELECT", CPythonNonPlayer::MOB_RESIST_ELECT);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_MAGIC", CPythonNonPlayer::MOB_RESIST_MAGIC);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_WIND", CPythonNonPlayer::MOB_RESIST_WIND);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_POISON", CPythonNonPlayer::MOB_RESIST_POISON);
	PyModule_AddIntConstant(poModule, "MOB_RESIST_BLEEDING", CPythonNonPlayer::MOB_RESIST_BLEEDING);
	PyModule_AddIntConstant(poModule, "MOB_RESISTS_MAX_NUM", CPythonNonPlayer::MOB_RESISTS_MAX_NUM);
#if defined(ENABLE_ELEMENT_ADD)
	PyModule_AddIntConstant(poModule, "MOB_ELEMENT_MAX_NUM", CPythonNonPlayer::MOB_ELEMENT_MAX_NUM);
#endif

	PyModule_AddIntConstant(poModule, "RACE_FLAG_ANIMAL", CPythonNonPlayer::MOB_FLAG_ANIMAL);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_UNDEAD", CPythonNonPlayer::MOB_FLAG_UNDEAD);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_DEVIL", CPythonNonPlayer::MOB_FLAG_DEVIL);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_HUMAN", CPythonNonPlayer::MOB_FLAG_HUMAN);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_ORC", CPythonNonPlayer::MOB_FLAG_ORC);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_MILGYO", CPythonNonPlayer::MOB_FLAG_MILGYO);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_INSECT", CPythonNonPlayer::MOB_FLAG_INSECT);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_FIRE", CPythonNonPlayer::MOB_FLAG_FIRE);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_ICE", CPythonNonPlayer::MOB_FLAG_ICE);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_DESERT", CPythonNonPlayer::MOB_FLAG_DESERT);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_TREE", CPythonNonPlayer::MOB_FLAG_TREE);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_ATT_ELEC", CPythonNonPlayer::MOB_FLAG_ATT_ELEC);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_ATT_FIRE", CPythonNonPlayer::MOB_FLAG_ATT_FIRE);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_ATT_ICE", CPythonNonPlayer::MOB_FLAG_ATT_ICE);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_ATT_WIND", CPythonNonPlayer::MOB_FLAG_ATT_WIND);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_ATT_EARTH", CPythonNonPlayer::MOB_FLAG_ATT_EARTH);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_ATT_DARK", CPythonNonPlayer::MOB_FLAG_ATT_DARK);
	PyModule_AddIntConstant(poModule, "RACE_FLAG_ZODIAC", CPythonNonPlayer::MOB_FLAG_ZODIAC);

#if defined(ENABLE_SEND_TARGET_INFO)
	PyModule_AddIntConstant(poModule, "MAX_DROP_INFO_GRID_SLOT_X", CPythonNonPlayer::MAX_DROP_INFO_GRID_SLOT_X);
	PyModule_AddIntConstant(poModule, "MAX_DROP_INFO_GRID_SLOT_Y", CPythonNonPlayer::MAX_DROP_INFO_GRID_SLOT_Y);
	PyModule_AddIntConstant(poModule, "MAX_DROP_INFO_GRID_SIZE", CPythonNonPlayer::MAX_DROP_INFO_GRID_SIZE);
#endif
}
