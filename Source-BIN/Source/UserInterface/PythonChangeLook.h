#pragma once

#ifdef ENABLE_CHANGE_LOOK_SYSTEM
#include "Packet.h"

class CPythonChangeLook : public CSingleton<CPythonChangeLook>
{
public:

	enum EChangeLook
	{
		CHANGE_LOOK_TICKET_VNUM = 72326,
		WINDOW_MAX_MATERIALS = 3,
		LIMIT_RANGE = 1000,
	};

	DWORD dwCost;
	typedef std::vector<TChangeLookMaterial> TChangeLookMaterials;

public:
	CPythonChangeLook();
	virtual ~CPythonChangeLook();

	void Clear();
	void AddMaterial(BYTE bPos, TItemPos tPos);
	void RemoveMaterial(BYTE bPos);
	void RemoveAllMaterials();
	DWORD GetCost() { return dwCost; }
	void SetCost(DWORD dwCostR) { dwCost = dwCostR; }
	bool GetAttachedItem(BYTE bPos, BYTE& bHere, WORD& wCell);
	BOOL IsCleanScroll(DWORD dwItemVnum);
	BOOL IsTransmutationTicket(DWORD dwItemVnum);

protected:
	TChangeLookMaterials m_vMaterials;
};
#endif
