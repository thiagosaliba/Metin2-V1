// AreaLoaderThread.h: interface for the CAreaLoaderThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__INC_AREA_LOADER_THREAD_H__)
#define __INC_AREA_LOADER_THREAD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../EterLib/Mutex.h"

class CTerrain;
class CArea;

class TEMP_CAreaLoaderThread
{
public:
	TEMP_CAreaLoaderThread();
	virtual ~TEMP_CAreaLoaderThread();

	bool Create(void* arg);
	void Shutdown();

	void Request(CTerrain* pTerrain);

	bool Fetch(CTerrain** ppTerrian);

	void Request(CArea* pArea);

	bool Fetch(CArea** ppArea);

protected:
	static UINT CALLBACK EntryPoint(void* pThis);
	UINT Run(void* arg);

	void* Arg() const { return m_pArg; }
	void Arg(void* arg) { m_pArg = arg; }

	HANDLE m_hThread;

private:
	void* m_pArg;
	unsigned m_uThreadID;

protected:
	UINT Setup();
	UINT Execute(void* pvArg);
	void Destroy();
	void ProcessTerrain();
	void ProcessArea();

private:
	std::deque<CTerrain*> m_pTerrainRequestDeque;
	Mutex m_TerrainRequestMutex;

	std::deque<CTerrain*> m_pTerrainCompleteDeque;
	Mutex m_TerrainCompleteMutex;

	std::deque<CArea*> m_pAreaRequestDeque;
	Mutex m_AreaRequestMutex;

	std::deque<CArea*> m_pAreaCompleteDeque;
	Mutex m_AreaCompleteMutex;

	HANDLE m_hSemaphore;
	int m_iRestSemCount;
	bool m_bShutdowned;

};

#endif // __INC_AREA_LOADER_THREAD_H__
