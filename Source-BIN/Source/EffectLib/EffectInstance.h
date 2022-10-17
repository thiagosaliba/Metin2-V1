#pragma once

#include "../EterLib/GrpObjectInstance.h"
#include "../EterLib/Pool.h"
#include "../MilesLib/Type.h"

#include "EffectElementBaseInstance.h"
#include "EffectData.h"
#include "EffectMeshInstance.h"
#include "ParticleSystemInstance.h"
#include "SimpleLightInstance.h"

class CEffectInstance : public CGraphicObjectInstance
{
public:
	typedef std::vector<CEffectElementBaseInstance*> TEffectElementInstanceVector;

	enum
	{
		ID = EFFECT_OBJECT
	};

	int GetType() const
	{
		return CEffectInstance::ID;
	}

	bool GetBoundingSphere(D3DXVECTOR3& v3Center, float& fRadius);

	static void DestroySystem();

	static CEffectInstance* New();
	static void Delete(CEffectInstance* pkEftInst);

	static void ResetRenderingEffectCount();
	static int GetRenderingEffectCount();

public:
	CEffectInstance();
	virtual ~CEffectInstance();

	bool LessRenderOrder(CEffectInstance* pkEftInst);

#ifdef ENABLE_SKILL_COLOR_SYSTEM
	void SetEffectDataPointer(CEffectData* pEffectData, DWORD* dwSkillColor = NULL, DWORD EffectID = 0);
#else
	void SetEffectDataPointer(CEffectData* pEffectData);
#endif

	void Clear();
	BOOL isAlive();
	void SetActive();
	void SetDeactive();
	void SetGlobalMatrix(const D3DXMATRIX& c_rmatGlobal);
	void UpdateSound();
	void OnUpdate();
	void OnRender();
	void OnBlendRender() {} // Not used
	void OnRenderToShadowMap() {} // Not used
	void OnRenderShadow() {} // Not used
	void OnRenderPCBlocker() {} // Not used

protected:
	void __Initialize();

	void __SetParticleData(CParticleSystemData* pData);
	void __SetMeshData(CEffectMeshScript* pMesh);
	void __SetLightData(CLightData* pData);

	virtual void OnUpdateCollisionData(const CStaticCollisionDataVector* pscdVector) {} // Not used
	virtual void OnUpdateHeighInstance(CAttributeInstance* pAttributeInstance) {}
	virtual bool OnGetObjectHeight(float fX, float fY, float* pfHeight) { return false; }

protected:
	BOOL m_isAlive;
	DWORD m_dwFrame;
	D3DXMATRIX m_matGlobal;

	CEffectData* m_pkEftData;

	std::vector<CParticleSystemInstance*> m_ParticleInstanceVector;
	std::vector<CEffectMeshInstance*> m_MeshInstanceVector;
	std::vector<CLightInstance*> m_LightInstanceVector;

	NSound::TSoundInstanceVector* m_pSoundInstanceVector;

	float m_fBoundingSphereRadius;
	D3DXVECTOR3 m_v3BoundingSpherePosition;

	float m_fLastTime;

public:
	static CDynamicPool<CEffectInstance> ms_kPool;
	static int ms_iRenderingEffectCount;

protected:
	float m_fScaleParticle;
	D3DXVECTOR3 m_v3ScaleMesh;

public:
	void SetParticleScale(float fScale) { m_fScaleParticle = fScale; }
	void SetMeshScale(D3DXVECTOR3 rv3Scale) { m_v3ScaleMesh = rv3Scale; }
	D3DXVECTOR3 GetMeshScale() { return m_v3ScaleMesh; }
	const D3DXMATRIX& GetGlobalMatrix() const { return m_matGlobal; };

};
