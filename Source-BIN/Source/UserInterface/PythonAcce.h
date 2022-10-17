#pragma once

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
#include "Packet.h"

class CPythonAcce : public CSingleton<CPythonAcce>
{
public:

	enum EAcce
	{
		ABSORPTION_SOCKET = 0,
		ABSORBED_SOCKET = 1,
		WINDOW_MAX_MATERIALS = 2,
		LIMIT_RANGE = 1000,
	};

	DWORD dwPrice;
	typedef std::vector<TAcceMaterial> TAcceMaterials;

public:
	CPythonAcce();
	virtual ~CPythonAcce();

	void Clear();
	void AddMaterial(DWORD dwRefPrice, BYTE bPos, TItemPos tPos);
	void AddResult(DWORD dwItemVnum, DWORD dwMinAbs, DWORD dwMaxAbs);
	void RemoveMaterial(DWORD dwRefPrice, BYTE bPos);
	DWORD GetPrice() { return dwPrice; }
	bool GetAttachedItem(BYTE bPos, BYTE& bHere, WORD& wCell);
	void GetResultItem(DWORD& dwItemVnum, DWORD& dwMinAbs, DWORD& dwMaxAbs);
	BOOL IsCleanScroll(DWORD dwItemVnum);

protected:
	TAcceResult	m_vAcceResult;
	TAcceMaterials m_vAcceMaterials;
};
#endif
