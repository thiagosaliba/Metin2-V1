#include "StdAfx.h"

#include "PythonApplication.h"
#include "PythonIllustratedManager.h"
#include "../EterLib/Camera.h"
#include "../EterLib/GrpRenderTargetTexture.h"
#include "../EterLib/CRenderTargetManager.h"
#include "../EterPythonLib/PythonGraphic.h"
#include "../GameLib/ActorInstance.h"

CPythonIllustratedManager::CPythonIllustratedManager():
	m_pModelInstance(NULL),
	m_pBackgroundImage(NULL),
	m_fUnk08(0.0f),
	m_fUnk12(0.0f),
	m_fUnk16(0.0f),
	m_fUnk20(0.0f),
	m_fUnk24(0.0f),
	m_bShow(false)
{
	m_mapModelInstance.clear();
}

CPythonIllustratedManager::~CPythonIllustratedManager()
{
	Destroy();
}

void CPythonIllustratedManager::__Initialize()
{
	m_pBackgroundImage = NULL;
	m_pModelInstance = NULL;
	m_fUnk08 = 0.0f;
	m_fUnk12 = 0.0f;
	m_fUnk16 = 0.0f;
	m_fUnk20 = 0.0f;
	m_fUnk24 = 0.0f;
	m_bShow = false;
}

void CPythonIllustratedManager::Destroy()
{
	m_fUnk08 = 0.0f;
	m_fUnk12 = 0.0f;
	m_fUnk16 = 0.0f;
	m_fUnk20 = 0.0f;
	m_fUnk24 = 0.0f;
	m_bShow = false;
	m_pModelInstance = NULL;

	if (m_pBackgroundImage)
		delete m_pBackgroundImage;
	m_pBackgroundImage = NULL;

	m_mapModelInstance.clear();
}

void CPythonIllustratedManager::ClearInstances()
{
	m_pModelInstance = NULL;
	m_mapModelInstance.clear();
}

void CPythonIllustratedManager::ModelViewReset()
{
	if (!m_pModelInstance)
		return;

	m_fUnk12 = 0.0f;
	m_fUnk20 = 0.0f;
	m_pModelInstance->SetRotation(0.0f);
	m_pModelInstance->NEW_SetPixelPosition(TPixelPosition(0.0f, 0.0f, 0.0f));
}

void CPythonIllustratedManager::ModelRotation(float fRot)
{
	if (!m_pModelInstance)
		return;

	m_pModelInstance->SetRotation(fmod(fRot, 360.0f));
}

void CPythonIllustratedManager::ModelZoom(bool bZoom)
{
	if (!m_pModelInstance)
		return;

	if (bZoom)
	{
		m_fUnk12 = m_fUnk12 - m_fUnk16;
		float v3 = -(m_fUnk08 * 8.9f - m_fUnk08 * 3.0f);
		m_fUnk12 = fmax(v3, m_fUnk12);
	}
	else
	{
		m_fUnk12 = m_fUnk12 + m_fUnk16;
		float v6 = 14000.0f - m_fUnk08 * 8.9f;
		float v7 = m_fUnk08 * 8.9f + m_fUnk08 * 5.0f;
		m_fUnk12 = fmin(m_fUnk12, v6);
		m_fUnk12 = fmin(m_fUnk12, v7);
	}
}

void CPythonIllustratedManager::ModelUpDown(bool bUp)
{
	if (!m_pModelInstance)
		return;

	if (bUp)
	{
		m_fUnk20 = m_fUnk20 + m_fUnk24;
		m_fUnk20 = fmin(m_fUnk08, m_fUnk20);
	}
	else
	{
		m_fUnk20 = m_fUnk20 - m_fUnk24;
		m_fUnk20 = fmax(-m_fUnk08 / 2.0f, m_fUnk20);
	}
}

bool CPythonIllustratedManager::CreateBackground(DWORD dwWidth, DWORD dwHeight)
{
	if (!m_pBackgroundImage)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer("d:/ymir work/ui/game/monster_card/model_view_bg.sub");
		m_pBackgroundImage = new CGraphicImageInstance;
		m_pBackgroundImage->SetImagePointer(static_cast<CGraphicImage*>(pResource));
		m_pBackgroundImage->SetScale(static_cast<float>(dwWidth) / 240.0f, static_cast<float>(dwHeight) / 306.0f);
	}

	return true;
}

CInstanceBase* CPythonIllustratedManager::GetInstancePtr(DWORD dwVnum)
{
	TModelInstanceMap::iterator itor = m_mapModelInstance.find(dwVnum);
	if (m_mapModelInstance.end() == itor)
		return NULL;

	return itor->second;
}

bool CPythonIllustratedManager::CreateModelInstance(DWORD dwVnum)
{
	if (GetInstancePtr(dwVnum))
		return true;

	CInstanceBase::SCreateData kCreateData;
	memset(&kCreateData, 0, sizeof(kCreateData));
	if (dwVnum < CRaceData::RACE_MAX_NUM)
		kCreateData.m_bType = CActorInstance::TYPE_PC;
	else
		kCreateData.m_bType = CActorInstance::TYPE_OBJECT;
	kCreateData.m_dwRace = dwVnum;
	kCreateData.m_fScale = 1.0f;

	CInstanceBase * pModel = CInstanceBase::New();
	if (!pModel->Create(kCreateData))
	{
		if (pModel)
			CInstanceBase::Delete(pModel);
		return false;
	}

	CActorInstance& rkActor = pModel->GetGraphicThingInstanceRef();
	float fHeight = rkActor.GetHeight();

	m_mapModelInstance.insert(TModelInstanceMap::value_type(dwVnum, pModel));
	return true;
}

bool CPythonIllustratedManager::SelectModel(DWORD dwVnum)
{
	if (!dwVnum)
	{
		Tracef("MC SelectModel failed 0 : %u\n", dwVnum);
		m_pModelInstance = NULL;
		return false;
	}

	if (!CreateModelInstance(dwVnum))
	{
		Tracef("MC SelectModel failed 1 : %u\n", dwVnum);
		m_pModelInstance = NULL;
		return false;
	}

	m_pModelInstance = GetInstancePtr(dwVnum);
	if (!m_pModelInstance)
	{
		Tracef("MC SelectModel failed 2 : %u\n", dwVnum);
		return false;
	}

	m_pModelInstance->Refresh(CRaceMotionData::NAME_WAIT, true);
	m_pModelInstance->SetLODLimits(0, 100.0f);
	m_pModelInstance->SetAlwaysRender(true);
	m_pModelInstance->SetRotation(0.0f);
	m_pModelInstance->NEW_SetPixelPosition(TPixelPosition(0.0f, 0.0f, 0.0f));

	CActorInstance& rkActor = m_pModelInstance->GetGraphicThingInstanceRef();
	float fHeight = rkActor.GetHeight();
	m_fUnk08 = fHeight;
	m_fUnk12 = 0.0f;
	m_fUnk20 = 0.0f;
	m_fUnk16 = m_fUnk08 * 8.9f / 140.0f;
	m_fUnk24 = m_fUnk08 / 35.0f;

	CCameraManager::instance().SetCurrentCamera(CCameraManager::ILLUSTRATED_CAMERA);
	CCamera* pCam = CCameraManager::instance().GetCurrentCamera();
	pCam->SetTargetHeight(m_fUnk08 / 2.0f);
	CCameraManager::instance().ResetToPreviousCamera();
	return true;
}

void CPythonIllustratedManager::RenderBackground()
{
	if (!m_bShow)
		return;

	if (!m_pBackgroundImage)
		return;

	RECT rectRender;
	if (!CRenderTargetManager::instance().GetRenderTargetRect(CPythonApplication::RENDER_TARGET_ILLUSTRATION, rectRender))
		return;

	if (!CRenderTargetManager::instance().ChangeRenderTarget(CPythonApplication::RENDER_TARGET_ILLUSTRATION))
		return;

	CRenderTargetManager::instance().ClearRenderTarget();
	CPythonGraphic::Instance().SetInterfaceRenderState();

	m_pBackgroundImage->Render();

	CRenderTargetManager::instance().ResetRenderTarget();
}

void CPythonIllustratedManager::DeformModel()
{
	if (!m_bShow)
		return;

	if (m_pModelInstance)
		m_pModelInstance->Deform();
}

void CPythonIllustratedManager::RenderModel()
{
	if (!m_bShow)
		return;

	RECT rectRender;
	if (!CRenderTargetManager::instance().GetRenderTargetRect(CPythonApplication::RENDER_TARGET_ILLUSTRATION, rectRender))
		return;

	if (!CRenderTargetManager::instance().ChangeRenderTarget(CPythonApplication::RENDER_TARGET_ILLUSTRATION))
		return;

	if (!m_pModelInstance)
	{
		CRenderTargetManager::instance().ResetRenderTarget();
		return;
	}

	CPythonGraphic::Instance().ClearDepthBuffer();

	float fFov = CPythonGraphic::Instance().GetFOV();
	float fAspect = CPythonGraphic::Instance().GetAspect();
	float fNearY = CPythonGraphic::Instance().GetNearY();
	float fFarY = CPythonGraphic::Instance().GetFarY();

	float fWidth = static_cast<float>(rectRender.right) - static_cast<float>(rectRender.left);
	float fHeight = static_cast<float>(rectRender.bottom) - static_cast<float>(rectRender.top);

#ifdef ENABLE_FOG_FIX
	CPythonBackground& rkBG = CPythonBackground::Instance();
	BOOL bIsFog = rkBG.GetFogMode();
#else
	BOOL bIsFog = STATEMANAGER.GetRenderState(D3DRS_FOGENABLE);
#endif
	STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, 0);
	CCameraManager::instance().SetCurrentCamera(CCameraManager::ILLUSTRATED_CAMERA);
	CCamera* pCam = CCameraManager::instance().GetCurrentCamera();

	CPythonGraphic::Instance().PushState();

	D3DXVECTOR3 v3Eye(0.0f, -(m_fUnk08 * 8.9f + m_fUnk12), 0.0f);
	D3DXVECTOR3 v3Target (0.0f, m_fUnk20, 0.0f);
	D3DXVECTOR3 v3Up (0.0f, 0.0f, 1.0f);
	pCam->SetViewParams(v3Eye, v3Target, v3Up);

	CPythonApplication::Instance().TargetIllustratedCamera();

	CPythonGraphic::Instance().SetPerspective(10.0f, fWidth / fHeight, 100.0f, 15000.0f);

	m_pModelInstance->Render();

	CCameraManager::instance().ResetToPreviousCamera();
	CPythonGraphic::Instance().PopState();
	CPythonGraphic::Instance().SetPerspective(fFov, fAspect, fNearY, fFarY);
	CRenderTargetManager::instance().ResetRenderTarget();
	STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, bIsFog);
}

void CPythonIllustratedManager::UpdateModel()
{
	if (!m_bShow)
		return;

	if (!m_pModelInstance)
		return;

	m_pModelInstance->Transform();
	CActorInstance& rkModelActor = m_pModelInstance->GetGraphicThingInstanceRef();
	rkModelActor.RotationProcess();
}

void CPythonIllustratedManager::ChangeMotion()
{
	if (!m_pModelInstance)
		return;

	CActorInstance& rkActor = m_pModelInstance->GetGraphicThingInstanceRef();
	WORD wMotionIndex = rkActor.NextValidMotion();
	if (m_pModelInstance->IsWalking())
		m_pModelInstance->EndWalking();
	m_pModelInstance->Refresh(wMotionIndex, true);
}

void CPythonIllustratedManager::SetHair(DWORD dwHair)
{
	if (!m_pModelInstance)
		return;

	m_pModelInstance->SetHair(dwHair);
	CActorInstance& rkActor = m_pModelInstance->GetGraphicThingInstanceRef();
	WORD wMotionIndex = rkActor.GetCurrentMotionIndex();
	m_pModelInstance->Refresh(wMotionIndex, true);
	//m_pModelInstance->SetLODLimits(CRaceData::PART_HAIR, 100.0f);
}

void CPythonIllustratedManager::SetArmor(DWORD dwArmor)
{
	if (!m_pModelInstance)
		return;

	CItemData * pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwArmor, &pItemData))
		return;

	m_pModelInstance->SetShape(pItemData->GetValue(3), pItemData->GetType() == CItemData::ITEM_TYPE_ARMOR ? 100.0f: 0.0f);// Specular on costumes makes full of oil
	CActorInstance& rkActor = m_pModelInstance->GetGraphicThingInstanceRef();
	WORD wMotionIndex = rkActor.GetCurrentMotionIndex();
	m_pModelInstance->Refresh(wMotionIndex, true);
	if (m_pModelInstance->GetPart(CRaceData::PART_HAIR))
		m_pModelInstance->SetHair(m_pModelInstance->GetPart(CRaceData::PART_HAIR));
	if (m_pModelInstance->GetPart(CRaceData::PART_WEAPON))
		m_pModelInstance->SetWeapon(m_pModelInstance->GetPart(CRaceData::PART_WEAPON));
	//m_pModelInstance->SetLODLimits(CRaceData::PART_HAIR, 100.0f);
	//m_pModelInstance->SetLODLimits(CRaceData::PART_WEAPON, 100.0f);
	//m_pModelInstance->SetLODLimits(CRaceData::PART_MAIN, 100.0f);
}

void CPythonIllustratedManager::SetWeapon(DWORD dwWeapon)
{
	if (!m_pModelInstance)
		return;

	if (m_pModelInstance->SetWeapon(dwWeapon))
	{
		CActorInstance& rkActor = m_pModelInstance->GetGraphicThingInstanceRef();
		WORD wMotionIndex = rkActor.GetCurrentMotionIndex();
		m_pModelInstance->Refresh(wMotionIndex, true);
		//m_pModelInstance->SetLODLimits(CRaceData::PART_WEAPON, 100.0f);
	}
}

void CPythonIllustratedManager::SetShow(bool bShow)
{
	m_bShow = bShow;
}
