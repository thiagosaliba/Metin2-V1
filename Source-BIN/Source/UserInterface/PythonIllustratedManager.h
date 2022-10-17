#pragma once
#include "InstanceBase.h"

class CPythonIllustratedManager : public CSingleton<CPythonIllustratedManager>
{
public:
		CPythonIllustratedManager();
		virtual ~CPythonIllustratedManager();

		void __Initialize();
		void Destroy();
		bool CreateBackground(DWORD dwWidth, DWORD dwHeight);
		CInstanceBase* GetInstancePtr(DWORD dwVnum);

		bool CreateModelInstance(DWORD dwVnum);
		bool SelectModel(DWORD dwVnum);
		void RenderBackground();
		void DeformModel();
		void RenderModel();
		void UpdateModel();
		void ChangeMotion();
		void SetShow(bool bShow);
		void ClearInstances();
		void ModelViewReset();
		void ModelRotation(float fRot);
		void ModelZoom(bool bZoom);
		void ModelUpDown(bool bUp);
		void SetHair(DWORD dwHair);
		void SetArmor(DWORD dwArmor);
		void SetWeapon(DWORD dwWeapon);

	private:
		CInstanceBase* m_pModelInstance;
		CGraphicImageInstance* m_pBackgroundImage;
		typedef std::map<DWORD, CInstanceBase *> TModelInstanceMap;
		TModelInstanceMap m_mapModelInstance;
		float m_fUnk08;
		float m_fUnk12;
		float m_fUnk16;
		float m_fUnk20;
		float m_fUnk24;
		bool m_bShow;
};
