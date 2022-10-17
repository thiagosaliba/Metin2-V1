#pragma once

#include "ActorInstance.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CGameEventManager
//
// NOTE : Game�󿡼� �Ͼ�� ����, ȭ�� ��鸲, Range ������� �����Ӱ� Ÿ�� ���� Event
// �� ���������� �����ϴ� Ŭ����. - [levites]
//
// NOTE : ���ӻ��� Cinematic Event �� ��Ʈ���ϴ� �Ŵ����� ����Ѵ�. 2004.07.19 - [levites]
//

class CGameEventManager : public CSingleton<CGameEventManager>, public CScreen
{
public:
	CGameEventManager();
	virtual ~CGameEventManager();

	void SetCenterPosition(float fx, float fy, float fz);
	void Update();

	void ProcessEventScreenWaving(CActorInstance* pActorInstance, const CRaceMotionData::TScreenWavingEventData* c_pData);

protected:
	TPixelPosition m_CenterPosition;

};
