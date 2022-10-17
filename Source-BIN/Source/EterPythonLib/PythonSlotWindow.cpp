#include "StdAfx.h"
#include "../EterBase/CRC32.h"
#include "../EterBase/Filename.h"
#include "PythonWindow.h"
#include "PythonSlotWindow.h"

//#define __RENDER_SLOT_AREA__

using namespace UI;

class UI::CSlotWindow::CSlotButton : public CButton
{
public:
	enum ESlotButtonType
	{
		SLOT_BUTTON_TYPE_PLUS,
		SLOT_BUTTON_TYPE_COVER,
	};

public:
	CSlotButton(ESlotButtonType dwType, DWORD dwSlotNumber, CSlotWindow* pParent) : CButton(NULL)
	{
		m_dwSlotButtonType = dwType;
		m_dwSlotNumber = dwSlotNumber;
		m_pParent = pParent;
	}
	virtual ~CSlotButton()
	{
	}

	BOOL OnMouseLeftButtonDown()
	{
		if (!IsEnable())
			return true;
		if (UI::CWindowManager::Instance().IsAttaching())
			return true;

		m_isPressed = TRUE;
		Down();

		return true;
	}
	BOOL OnMouseLeftButtonUp()
	{
		if (!IsEnable())
			return true;
		if (!IsPressed())
			return true;

		if (IsIn())
			SetCurrentVisual(&m_overVisual);
		else
			SetCurrentVisual(&m_upVisual);

		m_pParent->OnPressedSlotButton(m_dwSlotButtonType, m_dwSlotNumber);

		return true;
	}

	void OnMouseOverIn()
	{
		if (IsEnable())
			SetCurrentVisual(&m_overVisual);

		//m_pParent->OnOverInItem(m_dwSlotNumber);
		//m_pParent->OnOverIn(m_dwSlotNumber);
		TSlot* pSlot;
		if (m_pParent->GetSlotPointer(m_dwSlotNumber, &pSlot))
		{
			if (pSlot->isItem)
				m_pParent->OnOverInItem(m_dwSlotNumber);
			else
				m_pParent->OnOverIn(m_dwSlotNumber);
		}
	}
	void OnMouseOverOut()
	{
		if (IsEnable())
		{
			SetUp();
			SetCurrentVisual(&m_upVisual);
		}

		//m_pParent->OnOverOutItem();
		//m_pParent->OnOverOut();
		TSlot* pSlot;
		if (m_pParent->GetSlotPointer(m_dwSlotNumber, &pSlot))
		{
			if (pSlot->isItem)
				m_pParent->OnOverOutItem();
			else
				m_pParent->OnOverOut();
		}
	}

protected:
	ESlotButtonType m_dwSlotButtonType;
	DWORD m_dwSlotNumber;
	CSlotWindow* m_pParent;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class UI::CSlotWindow::CCoverButton : public CSlotButton
{
public:
	CCoverButton(ESlotButtonType dwType, DWORD dwSlotNumber, CSlotWindow* pParent) : CSlotButton(dwType, dwSlotNumber, pParent)
	{
		m_bLeftButtonEnable = TRUE;
		m_bRightButtonEnable = TRUE;
	}
	virtual ~CCoverButton()
	{
	}

	void SetLeftButtonEnable(BOOL bEnable)
	{
		m_bLeftButtonEnable = bEnable;
	}
	void SetRightButtonEnable(BOOL bEnable)
	{
		m_bRightButtonEnable = bEnable;
	}

	void OnRender()
	{
	}

	void RenderButton()
	{
		CButton::OnRender();
	}

	BOOL OnMouseLeftButtonDown()
	{
		if (!IsEnable())
			return true;
		if (m_bLeftButtonEnable)
		{
			CButton::OnMouseLeftButtonDown();
		}
		else
		{
			m_pParent->OnMouseLeftButtonDown();
		}
		return true;
	}
	BOOL OnMouseLeftButtonUp()
	{
		if (!IsEnable())
			return true;
		if (m_bLeftButtonEnable)
		{
			CButton::OnMouseLeftButtonUp();
			m_pParent->OnMouseLeftButtonDown();
		}
		else
		{
			m_pParent->OnMouseLeftButtonUp();
		}
		return true;
	}

	BOOL OnMouseRightButtonDown()
	{
		if (!IsEnable())
			return true;
		if (m_bRightButtonEnable)
			CButton::OnMouseLeftButtonDown();
		return true;
	}
	BOOL OnMouseRightButtonUp()
	{
		if (!IsEnable())
			return true;
		m_pParent->OnMouseRightButtonDown();
		if (m_bRightButtonEnable)
			CButton::OnMouseLeftButtonUp();
		return true;
	}

protected:
	BOOL m_bLeftButtonEnable;
	BOOL m_bRightButtonEnable;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(ENABLE_HIGH_LIGHT_IMAGE)
class UI::CSlotWindow::CHighLightImage : public CExpandedImageBox
{
public:
	CHighLightImage(CSlotWindow* pParent, DWORD dwSlotNumber) : CExpandedImageBox(NULL)
	{
		m_pParent = pParent;
		m_dwSlotNumber = dwSlotNumber;
		m_bAlphaGlow = false;
		m_fCurAlpha = 1.0f;
		m_fCurRot = 0.0f;
		m_fRotSpeed = 0.0f;
		m_fAlphaSpeed = 0.0f;
	}
	virtual ~CHighLightImage()
	{
	}
	void SetColor(DWORD dwColor, float fCurAlpha = 1.0f)
	{
		const float f = 1.0f / 255.0f;
		const float r = f * (FLOAT)(unsigned char)(dwColor >> 16);
		const float g = f * (FLOAT)(unsigned char)(dwColor >> 8);
		const float b = f * (FLOAT)(unsigned char)(dwColor >> 0);
		const float a = fCurAlpha;

		m_Color = D3DXCOLOR(r, g, b, a);
		SetDiffuseColor(m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	}
	void SetRotationSpeed(float fSpeed)
	{
		m_fRotSpeed = fSpeed;
	}
	void SetAlphaSpeed(float fSpeed, float fCurAlpha = 1.0f)
	{
		m_fCurAlpha = fCurAlpha;
		if (fSpeed > 0.0f && fSpeed < 1.0f)
		{
			m_bAlphaGlow = true;
			m_fAlphaSpeed = fSpeed;
		}
	}
	BOOL OnMouseLeftButtonDown()
	{
		m_pParent->OnMouseLeftButtonDown();
		return true;
	}
	BOOL OnMouseLeftButtonUp()
	{
		m_pParent->OnMouseLeftButtonUp();
		return true;
	}
	BOOL OnMouseRightButtonDown()
	{
		m_pParent->OnMouseRightButtonDown();
		return true;
	}
	BOOL OnMouseLeftButtonDoubleClick()
	{
		m_pParent->OnMouseLeftButtonDoubleClick();
		return true;
	}
	void OnMouseOverIn()
	{
		TSlot* pSlot;
		if (m_pParent->GetSlotPointer(m_dwSlotNumber, &pSlot))
		{
			if (pSlot->isItem)
				m_pParent->OnOverInItem(m_dwSlotNumber);
			else
				m_pParent->OnOverIn(m_dwSlotNumber);
		}
	}
	void OnMouseOverOut()
	{
		TSlot* pSlot;
		if (m_pParent->GetSlotPointer(m_dwSlotNumber, &pSlot))
		{
			if (pSlot->isItem)
				m_pParent->OnOverOutItem();
			else
				m_pParent->OnOverOut();
		}
	}
	virtual void OnUpdate()
	{
		if (IsShow())
		{
			if (m_bAlphaGlow && m_fAlphaSpeed != 0.0f)
			{
				if (m_fCurAlpha < 0.f || m_fCurAlpha >= 1.f)
					m_fAlphaSpeed = -m_fAlphaSpeed;

				m_fCurAlpha += m_fAlphaSpeed;
				SetDiffuseColor(m_Color.r, m_Color.g, m_Color.b, m_fCurAlpha);
			}

			if (m_fRotSpeed != 0.0f)
			{
				if (m_fCurRot == 360.0f)
					m_fCurRot = 0.0f;
				else
					m_fCurRot += m_fRotSpeed;

				SetRotation(m_fCurRot);
			}
		}
	}

protected:
	CSlotWindow* m_pParent;
	DWORD m_dwSlotNumber;
	D3DXCOLOR m_Color;
	bool m_bAlphaGlow;
	float m_fCurAlpha;
	float m_fCurRot;
	float m_fRotSpeed;
	float m_fAlphaSpeed;
};
#endif

class UI::CSlotWindow::CCoolTimeFinishEffect : public CAniImageBox
{
public:
	CCoolTimeFinishEffect(CSlotWindow* pParent, DWORD dwSlotIndex) : CAniImageBox(NULL)
	{
		m_pParent = pParent;
		m_dwSlotIndex = dwSlotIndex;
	}
	virtual ~CCoolTimeFinishEffect()
	{
	}

	void OnEndFrame()
	{
		((CSlotWindow*)m_pParent)->ReserveDestroyCoolTimeFinishEffect(m_dwSlotIndex);
	}

protected:
	DWORD m_dwSlotIndex;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

// Set & Append

void CSlotWindow::SetSlotType(DWORD dwType)
{
	m_dwSlotType = dwType;
}

void CSlotWindow::SetSlotStyle(DWORD dwStyle)
{
	m_dwSlotStyle = dwStyle;
}

void CSlotWindow::AppendSlot(DWORD dwIndex, int ixPosition, int iyPosition, int ixCellSize, int iyCellSize)
{
	TSlot Slot;
	Slot.pInstance = NULL;
	Slot.pNumberLine = NULL;
	Slot.pCoverButton = NULL;
	Slot.pSlotButton = NULL;
	Slot.pSignImage = NULL;
	Slot.pFinishCoolTimeEffect = NULL;
#if defined(WJ_ENABLE_PICKUP_ITEM_EFFECT)
	Slot.pSlotEffect.fill(nullptr);
#endif
#if defined(ENABLE_SLOT_COVER_IMAGE_SYSTEM)
	Slot.pCoverImage = nullptr;
#endif
#if defined(ENABLE_HIGH_LIGHT_IMAGE)
	Slot.pHighLightImage = NULL;
#endif

	ClearSlot(&Slot);
	Slot.dwSlotNumber = dwIndex;
	Slot.dwCenterSlotNumber = dwIndex;
	Slot.dwRealSlotNumber = dwIndex;
	Slot.dwRealCenterSlotNumber = dwIndex;
	Slot.ixPosition = ixPosition;
	Slot.iyPosition = iyPosition;
	Slot.ixCellSize = ixCellSize;
	Slot.iyCellSize = iyCellSize;
	m_SlotList.push_back(Slot);
}

void CSlotWindow::SetCoverButton(DWORD dwIndex, const char* c_szUpImageName, const char* c_szOverImageName, const char* c_szDownImageName, const char* c_szDisableImageName, BOOL bLeftButtonEnable, BOOL bRightButtonEnable)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	CCoverButton*& rpCoverButton = pSlot->pCoverButton;

	if (!rpCoverButton)
	{
		rpCoverButton = new CCoverButton(CSlotButton::SLOT_BUTTON_TYPE_COVER, pSlot->dwSlotNumber, this);
		CWindowManager::Instance().SetParent(rpCoverButton, this);
	}

	rpCoverButton->SetLeftButtonEnable(bLeftButtonEnable);
	rpCoverButton->SetRightButtonEnable(bRightButtonEnable);
	rpCoverButton->SetUpVisual(c_szUpImageName);
	rpCoverButton->SetOverVisual(c_szOverImageName);
	rpCoverButton->SetDownVisual(c_szDownImageName);
	rpCoverButton->SetDisableVisual(c_szDisableImageName);
	rpCoverButton->Enable();
	rpCoverButton->Show();

	// NOTE : Cover 버튼이 Plus 버튼을 가려버려서 임시 코드를..
	if (pSlot->pSlotButton)
	{
		SetTop(pSlot->pSlotButton);
	}
}

void CSlotWindow::EnableCoverButton(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	if (!pSlot->pCoverButton)
		return;

	pSlot->pCoverButton->Enable();
}

void CSlotWindow::DisableCoverButton(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	if (!pSlot->pCoverButton)
		return;

	pSlot->pCoverButton->Disable();
}

void CSlotWindow::SetAlwaysRenderCoverButton(DWORD dwIndex, bool bAlwaysRender)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	if (bAlwaysRender)
		SET_BIT(pSlot->dwState, SLOT_STATE_ALWAYS_RENDER_COVER);
	else
		REMOVE_BIT(pSlot->dwState, SLOT_STATE_ALWAYS_RENDER_COVER);
}

void CSlotWindow::ShowSlotBaseImage(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->bRenderBaseSlotImage = true;
}

void CSlotWindow::HideSlotBaseImage(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->bRenderBaseSlotImage = false;
}

BOOL CSlotWindow::IsDisableCoverButton(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return false;

	if (!pSlot->pCoverButton)
		return false;

	return pSlot->pCoverButton->IsDisable();
}

void CSlotWindow::SetSlotBaseImage(const char* c_szFileName, float fr, float fg, float fb, float fa)
{
	__CreateBaseImage(c_szFileName, fr, fg, fb, fa);
}

void CSlotWindow::AppendSlotButton(const char* c_szUpImageName, const char* c_szOverImageName, const char* c_szDownImageName)
{
	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;
		CSlotButton*& rpSlotButton = rSlot.pSlotButton;

		if (!rpSlotButton)
		{
			rpSlotButton = new CSlotButton(CSlotButton::SLOT_BUTTON_TYPE_PLUS, rSlot.dwSlotNumber, this);
			rpSlotButton->AddFlag(FLAG_FLOAT);
			CWindowManager::Instance().SetParent(rpSlotButton, this);
		}

		rpSlotButton->SetUpVisual(c_szUpImageName);
		rpSlotButton->SetOverVisual(c_szOverImageName);
		rpSlotButton->SetDownVisual(c_szDownImageName);
		rpSlotButton->SetPosition(rSlot.ixPosition + 1, rSlot.iyPosition + 19);
		rpSlotButton->Hide();
	}
}

void CSlotWindow::AppendRequirementSignImage(const char* c_szImageName)
{
	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;
		CImageBox*& rpSignImage = rSlot.pSignImage;

		if (!rpSignImage)
		{
			rpSignImage = new CImageBox(NULL);
			CWindowManager::Instance().SetParent(rpSignImage, this);
		}

		rpSignImage->LoadImage(c_szImageName);
		rpSignImage->Hide();
	}
}

BOOL CSlotWindow::HasSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return false;

	return true;
}

void CSlotWindow::SetSlot(DWORD dwIndex, DWORD dwVirtualNumber, BYTE byWidth, BYTE byHeight, CGraphicImage* pImage, D3DXCOLOR& diffuseColor)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	if (pSlot->isItem)
	{
		if (pSlot->dwItemIndex == dwVirtualNumber)
		{
			pSlot->dwState = 0;
			pSlot->isItem = true;

#if defined(ENABLE_HIGH_LIGHT_IMAGE)
			if (pSlot->pHighLightImage && pSlot->bEnableHighLight)
			{
				pSlot->pHighLightImage->Show();
			}
#endif

			if (pImage && pSlot->pInstance)
			{
				pSlot->pInstance->SetImagePointer(pImage);
			}

			return;
		}
	}

	ClearSlot(pSlot);
	pSlot->dwState = 0;
	pSlot->isItem = true;
	pSlot->dwItemIndex = dwVirtualNumber;

	if (pImage)
	{
		assert(NULL == pSlot->pInstance);
		pSlot->pInstance = CGraphicImageInstance::New();
		pSlot->pInstance->SetDiffuseColor(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
		pSlot->pInstance->SetImagePointer(pImage);
	}

	pSlot->byxPlacedItemSize = byWidth;
	pSlot->byyPlacedItemSize = byHeight;

	if (pSlot->pCoverButton)
	{
		pSlot->pCoverButton->Show();
	}

#if defined(ENABLE_HIGH_LIGHT_IMAGE)
	if (pSlot->pHighLightImage && pSlot->bEnableHighLight)
	{
		pSlot->pHighLightImage->Show();
	}
#endif
}

#ifdef ENABLE_MINI_GAME_OKEY
void CSlotWindow::SetCardSlot(DWORD dwIndex, DWORD dwVirtualNumber, BYTE byWidth, BYTE byHeight, const char* c_szFileName, D3DXCOLOR& diffuseColor)
{
	CGraphicImage* pImage = (CGraphicImage*)CResourceManager::Instance().GetResourcePointer(c_szFileName);
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	if (pSlot->isItem)
		if (pSlot->dwItemIndex == dwVirtualNumber)
		{
			pSlot->dwState = 0;
			pSlot->isItem = TRUE;
			if (pImage && pSlot->pInstance)
			{
				pSlot->pInstance->SetImagePointer(pImage);
			}
			return;
		}

	ClearSlot(pSlot);
	pSlot->dwState = 0;
	pSlot->isItem = TRUE;
	pSlot->dwItemIndex = dwVirtualNumber;

	if (pImage)
	{
		assert(NULL == pSlot->pInstance);
		pSlot->pInstance = CGraphicImageInstance::New();
		pSlot->pInstance->SetImagePointer(pImage);
		pSlot->pInstance->SetDiffuseColor(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);
	}

	pSlot->byxPlacedItemSize = byWidth;
	pSlot->byyPlacedItemSize = byHeight;

	if (pSlot->pCoverButton)
	{
		pSlot->pCoverButton->Show();
	}
}
#endif

void CSlotWindow::SetSlotCount(DWORD dwIndex, DWORD dwCount)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	if (dwCount <= 0)
	{
		if (pSlot->pNumberLine)
		{
			delete pSlot->pNumberLine;
			pSlot->pNumberLine = NULL;
		}
	}
	else
	{
		char szCount[16 + 1];
		_snprintf(szCount, sizeof(szCount), "%d", dwCount);

		if (!pSlot->pNumberLine)
		{
			CNumberLine* pNumberLine = new UI::CNumberLine(this);
			pNumberLine->SetHorizontalAlign(CNumberLine::HORIZONTAL_ALIGN_RIGHT);
			pNumberLine->Show();
			pSlot->pNumberLine = pNumberLine;
		}

		pSlot->pNumberLine->SetNumber(szCount);
	}
}

void CSlotWindow::SetSlotCountNew(DWORD dwIndex, DWORD dwGrade, DWORD dwCount)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	if (dwCount <= 0)
	{
		if (pSlot->pNumberLine)
		{
			delete pSlot->pNumberLine;
			pSlot->pNumberLine = NULL;
		}
	}
	else
	{
		char szCount[16 + 1];

		switch (dwGrade)
		{
		case 0:
			_snprintf(szCount, sizeof(szCount), "%d", dwCount);
			break;
		case 1:
			_snprintf(szCount, sizeof(szCount), "m%d", dwCount);
			break;
		case 2:
			_snprintf(szCount, sizeof(szCount), "g%d", dwCount);
			break;
		case 3:
			_snprintf(szCount, sizeof(szCount), "p");
			break;
		}

		if (!pSlot->pNumberLine)
		{
			CNumberLine* pNumberLine = new UI::CNumberLine(this);
			pNumberLine->SetHorizontalAlign(CNumberLine::HORIZONTAL_ALIGN_RIGHT);
			pNumberLine->Show();
			pSlot->pNumberLine = pNumberLine;
		}

		pSlot->pNumberLine->SetNumber(szCount);
	}
}

void CSlotWindow::SetRealSlotNumber(DWORD dwIndex, DWORD dwSlotRealNumber)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
	{
		return;
	}

	pSlot->dwRealSlotNumber = dwSlotRealNumber;
}

void CSlotWindow::SetSlotCoolTime(DWORD dwIndex, float fCoolTime, float fElapsedTime)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
	{
		return;
	}

	pSlot->fCoolTime = fCoolTime;
	pSlot->fStartCoolTime = CTimer::Instance().GetCurrentSecond() - fElapsedTime;
}

void CSlotWindow::StoreSlotCoolTime(DWORD dwKey, DWORD dwSlotIndex, float fCoolTime, float fElapsedTime)
{
	std::map<DWORD, SStoreCoolDown>::iterator it = m_CoolDownStore[dwKey].find(dwSlotIndex);
	if (it != m_CoolDownStore[dwKey].end())
	{
		it->second.fCoolTime = fCoolTime;
		it->second.fElapsedTime = CTimer::Instance().GetCurrentSecond() - fElapsedTime;
		it->second.bActive = false;
	}
	else
	{
		SStoreCoolDown m_storeCoolDown;
		m_storeCoolDown.fCoolTime = fCoolTime;
		m_storeCoolDown.fElapsedTime = CTimer::Instance().GetCurrentSecond() - fElapsedTime;
		m_storeCoolDown.bActive = false;
		m_CoolDownStore[dwKey].insert(std::map<DWORD, SStoreCoolDown>::value_type(dwSlotIndex, m_storeCoolDown));
	}
}

void CSlotWindow::RestoreSlotCoolTime(DWORD dwKey)
{
	for (std::map<DWORD, SStoreCoolDown>::iterator it = m_CoolDownStore[dwKey].begin(); it != m_CoolDownStore[dwKey].end(); it++)
	{
		TSlot* pSlot;
		if (!GetSlotPointer(it->first, &pSlot))
			return;

		pSlot->fCoolTime = it->second.fCoolTime;
		pSlot->fStartCoolTime = it->second.fElapsedTime;
		if (it->second.bActive)
			ActivateSlot(it->first);
		else
			DeactivateSlot(it->first);
	}
}

#if defined(WJ_ENABLE_PICKUP_ITEM_EFFECT)
D3DXCOLOR CSlotWindow::GetSlotColor(const BYTE c_bType) const
{
	switch (c_bType)
	{
	case ESlotColor::COLOR_TYPE_ORANGE:
		return D3DXCOLOR(0xFFA500);
	case ESlotColor::COLOR_TYPE_WHITE:
		return D3DXCOLOR(0xFFFFFF);
	case ESlotColor::COLOR_TYPE_RED:
		return D3DXCOLOR(0xFF0000);
	case ESlotColor::COLOR_TYPE_GREEN:
		return D3DXCOLOR(0x008000);
	case ESlotColor::COLOR_TYPE_YELLOW:
		return D3DXCOLOR(0xFFFF00);
	case ESlotColor::COLOR_TYPE_SKY:
		return D3DXCOLOR(0x87CEFA);
	case ESlotColor::COLOR_TYPE_PINK:
		return D3DXCOLOR(0xFFC0CB);
	case ESlotColor::COLOR_TYPE_BLUE:
		return D3DXCOLOR(0x0000FF);
	case ESlotColor::COLOR_TYPE_DARK_BLUE:
		return D3DXCOLOR(0x00008B);
	case ESlotColor::COLOR_TYPE_PURPLE:
		return D3DXCOLOR(0x800080);
	case ESlotColor::COLOR_TYPE_GRAY:
		return D3DXCOLOR(0x800080);
	case ESlotColor::COLOR_TYPE_SILVER:
		return D3DXCOLOR(0xC0C0C0);
	}

	return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void CSlotWindow::SetSlotDiffuseColor(const DWORD c_dwIndex, const BYTE c_bType)
{
	TSlot* pSlot;
	if (!GetSlotPointer(c_dwIndex, &pSlot))
		return;

	const D3DXCOLOR c_kColor = { GetSlotColor(c_bType) };
	for (auto& pSlotEffect : pSlot->pSlotEffect)
	{
		if (pSlotEffect == nullptr)
			continue;

#if defined(__ANI_IMAGE_PERFORMANCE__)
		pSlotEffect->SetDiffuseColor(c_kColor.r, c_kColor.g, c_kColor.b, c_kColor.a);
#else
		const auto& vecImage = pSlotEffect->GetImageVector();
		if (vecImage.empty())
			continue;

		for (CGraphicExpandedImageInstance* pSlotImage : vecImage)
		{
			if (pSlotImage == nullptr || pSlotImage->IsEmpty())
				continue;

			pSlotImage->SetDiffuseColor(c_kColor.r, c_kColor.g, c_kColor.b, c_kColor.a);
		}
#endif
	}
}
#endif

#if defined(ENABLE_SLOT_COVER_IMAGE_SYSTEM)
void CSlotWindow::SetSlotCoverImage(const DWORD c_dwSlotIndex, const char* c_szFileName)
{
	TSlot* pSlot;
	if (!GetSlotPointer(c_dwSlotIndex, &pSlot))
		return;

	auto& pCoverImage = pSlot->pCoverImage;
	if (pCoverImage == nullptr)
		pCoverImage = std::make_shared<CImageBox>(nullptr);

	pCoverImage->LoadImage(c_szFileName);
	pCoverImage->Show();
}

void CSlotWindow::EnableSlotCoverImage(const DWORD c_dwSlotIndex, const bool c_bEnable)
{
	TSlot* pSlot;
	if (!GetSlotPointer(c_dwSlotIndex, &pSlot))
		return;

	const auto& pCoverImage = pSlot->pCoverImage;
	if (pCoverImage == nullptr)
		return;

	if (c_bEnable)
		pCoverImage->Show();
	else
		pCoverImage->Hide();
}
#endif

#if defined(ENABLE_HIGH_LIGHT_IMAGE)
void CSlotWindow::AppendHighLightImage(DWORD dwIndex, const char* c_sImageName, float fAlphaSpeed, float fRotationSpeed, float fCurAlpha, DWORD dwDiffuse)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	CHighLightImage*& rpHighLightImage = pSlot->pHighLightImage;

	if (!rpHighLightImage)
	{
		rpHighLightImage = new CHighLightImage(this, pSlot->dwSlotNumber);
		CWindowManager::Instance().SetParent(rpHighLightImage, this);
	}

	rpHighLightImage->LoadImage(c_sImageName);
	// rpHighLightImage->SetScale(m_v2Scale.x, m_v2Scale.y);
	rpHighLightImage->SetColor(dwDiffuse, fCurAlpha);
	rpHighLightImage->SetAlphaSpeed(fAlphaSpeed, fCurAlpha);
	rpHighLightImage->SetRotationSpeed(fRotationSpeed);
	rpHighLightImage->SetRenderingMode(CGraphicExpandedImageInstance::RENDERING_MODE_NORMAL);
	rpHighLightImage->Hide();

	/*
	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;
		CHighLightImage*& rpHighLightImage = rSlot.pHighLightImage;

		if (!rpHighLightImage)
		{
			rpHighLightImage = new CHighLightImage(this, rSlot.dwSlotNumber);
			CWindowManager::Instance().SetParent(rpHighLightImage, this);
		}

		rpHighLightImage->LoadImage(c_sImageName);
		// rpHighLightImage->SetScale(m_v2Scale.x, m_v2Scale.y);
		rpHighLightImage->SetColor(dwDiffuse, fCurAlpha);
		rpHighLightImage->SetAlphaSpeed(fAlphaSpeed, fCurAlpha);
		rpHighLightImage->SetRotationSpeed(fRotationSpeed);
		rpHighLightImage->SetRenderingMode(CGraphicExpandedImageInstance::RENDERING_MODE_NORMAL);
		rpHighLightImage->Hide();
	}
	*/
}

void CSlotWindow::EnableHighLightImage(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	if (!pSlot->pHighLightImage)
		return;

	pSlot->bEnableHighLight = TRUE;
	pSlot->pHighLightImage->Show();
}

void CSlotWindow::DisableHighLightImage(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	if (!pSlot->pHighLightImage)
		return;

	pSlot->bEnableHighLight = FALSE;
	pSlot->pHighLightImage->Hide();
}
#endif

void CSlotWindow::ActivateSlot(DWORD dwSlotIndex)
{
#if defined(WJ_ENABLE_PICKUP_ITEM_EFFECT)
	TSlot* pSlot;
	if (!GetSlotPointer(dwSlotIndex, &pSlot))
		return;

	for (size_t i = 0; i < pSlot->pSlotEffect.size(); i++)
	{
		auto& pSlotEffect = pSlot->pSlotEffect.at(i);
		if (pSlotEffect == nullptr)
		{
			char buf[128];
			pSlotEffect = std::make_shared<CAniImageBox>(nullptr);

			for (BYTE j = 0; j < 13; j++)
			{
				if (i == 0)
					snprintf(buf, sizeof(buf), "d:/ymir work/ui/public/slotactiveeffect/%02d.sub", j);
				else
					snprintf(buf, sizeof(buf), "d:/ymir work/ui/public/slotactiveeffect/slot%d/%02d.sub", (i + 1), j);

				pSlotEffect->AppendImage(buf);
				pSlotEffect->SetRenderingMode(CGraphicExpandedImageInstance::RENDERING_MODE_SCREEN);
			}
		}

		if (i == (pSlot->byyPlacedItemSize - 1))
			pSlotEffect->Show();
		else
			pSlotEffect->Hide();
	}
#else
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->bActive = TRUE;

	if (!m_pSlotActiveEffect)
	{
		__CreateSlotEnableEffect();
	}

	std::map<DWORD, SStoreCoolDown>::iterator it = m_CoolDownStore[1].find(dwIndex);
	if (it != m_CoolDownStore[1].end())
		it->second.bActive = true;
#endif
}

void CSlotWindow::DeactivateSlot(DWORD dwSlotIndex)
{
#if defined(WJ_ENABLE_PICKUP_ITEM_EFFECT)
	TSlot* pSlot;
	if (!GetSlotPointer(dwSlotIndex, &pSlot))
		return;

	for (const auto& pSlotEffect : pSlot->pSlotEffect)
		if (pSlotEffect)
			pSlotEffect->Hide();

	SetSlotDiffuseColor(dwSlotIndex, ESlotColor::COLOR_TYPE_MAX);
#else
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->bActive = FALSE;

	std::map<DWORD, SStoreCoolDown>::iterator it = m_CoolDownStore[1].find(dwIndex);
	if (it != m_CoolDownStore[1].end())
		it->second.bActive = false;
#endif
}

void CSlotWindow::ClearSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	ClearSlot(pSlot);
}

void CSlotWindow::ClearSlot(TSlot* pSlot)
{
	pSlot->bActive = FALSE;
	pSlot->byxPlacedItemSize = 1;
	pSlot->byyPlacedItemSize = 1;

	pSlot->isItem = FALSE;
	pSlot->dwState = 0;
	pSlot->fCoolTime = 0.0f;
	pSlot->fStartCoolTime = 0.0f;
	pSlot->dwCenterSlotNumber = 0xffffffff;

	pSlot->dwItemIndex = 0;
	pSlot->bRenderBaseSlotImage = true;

	if (pSlot->pInstance)
	{
		CGraphicImageInstance::Delete(pSlot->pInstance);
		pSlot->pInstance = NULL;
	}

	if (pSlot->pCoverButton)
		pSlot->pCoverButton->Hide();

	if (pSlot->pSlotButton)
		pSlot->pSlotButton->Hide();

	if (pSlot->pSignImage)
		pSlot->pSignImage->Hide();

	if (pSlot->pFinishCoolTimeEffect)
		pSlot->pFinishCoolTimeEffect->Hide();

#if defined(WJ_ENABLE_PICKUP_ITEM_EFFECT)
	for (const auto& pSlotEffect : pSlot->pSlotEffect)
	{
		if (pSlotEffect)
			pSlotEffect->Hide();
	}
#endif

#if defined(ENABLE_SLOT_COVER_IMAGE_SYSTEM)
	if (pSlot->pCoverImage)
		pSlot->pCoverImage->Hide();
#endif

#if defined(ENABLE_HIGH_LIGHT_IMAGE)
	pSlot->bEnableHighLight = false;
	if (pSlot->pHighLightImage)
		pSlot->pHighLightImage->Hide();
#endif
}

void CSlotWindow::ClearAllSlot()
{
	Destroy();
}

void CSlotWindow::RefreshSlot()
{
	OnRefreshSlot();

	// NOTE : Refresh 될때 ToolTip 도 갱신 합니다 - [levites]
	if (IsRendering())
	{
		TSlot* pSlot;
		if (GetPickedSlotPointer(&pSlot))
		{
			if (pSlot->isItem)
			{
				OnOverOutItem();
				OnOverInItem(pSlot->dwSlotNumber);
			}
			else
			{
				OnOverOut();
				OnOverIn(pSlot->dwSlotNumber);
			}
		}
	}
}

void CSlotWindow::OnRefreshSlot()
{
}

DWORD CSlotWindow::GetSlotCount()
{
	return m_SlotList.size();
}

void CSlotWindow::LockSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->dwState |= SLOT_STATE_LOCK;
}

void CSlotWindow::UnlockSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->dwState ^= SLOT_STATE_LOCK;
}

void CSlotWindow::SetCantUseSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->dwState |= SLOT_STATE_CANT_USE;
}

void CSlotWindow::SetUseSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	pSlot->dwState ^= SLOT_STATE_CANT_USE;
}

#ifdef WJ_ENABLE_TRADABLE_ICON
void CSlotWindow::SetCantMouseEventSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	SET_BIT(pSlot->dwState, SLOT_STATE_CANT_MOUSE_EVENT);
	//pSlot->dwState ^= SLOT_STATE_CANT_MOUSE_EVENT;
}

void CSlotWindow::SetCanMouseEventSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	REMOVE_BIT(pSlot->dwState, SLOT_STATE_CANT_MOUSE_EVENT);
	//pSlot->dwState |= SLOT_STATE_CANT_MOUSE_EVENT;
}

void CSlotWindow::SetUnusableSlotOnTopWnd(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	SET_BIT(pSlot->dwState, SLOT_STATE_UNUSABLE_ON_TOP_WND);
	//pSlot->dwState ^= SLOT_STATE_UNUSABLE_ON_TOP_WND;
}

void CSlotWindow::SetUsableSlotOnTopWnd(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	REMOVE_BIT(pSlot->dwState, SLOT_STATE_UNUSABLE_ON_TOP_WND);
	//pSlot->dwState |= SLOT_STATE_UNUSABLE_ON_TOP_WND;
}
#endif

void CSlotWindow::DisableSlotHighlightedGreen(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
	{
		return;
	}

	REMOVE_BIT(pSlot->dwState, SLOT_STATE_HIGHLIGHT_GREEN);
}

void CSlotWindow::SetSlotHighlightedGreeen(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
	{
		return;
	}

	SET_BIT(pSlot->dwState, SLOT_STATE_HIGHLIGHT_GREEN);
}

void CSlotWindow::EnableSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;

	REMOVE_BIT(pSlot->dwState, SLOT_STATE_DISABLE);
	//pSlot->dwState |= SLOT_STATE_DISABLE;
}

void CSlotWindow::DisableSlot(DWORD dwIndex)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwIndex, &pSlot))
		return;
	SET_BIT(pSlot->dwState, SLOT_STATE_DISABLE);
	//pSlot->dwState ^= SLOT_STATE_DISABLE;
}

// Select

void CSlotWindow::SelectSlot(DWORD dwSelectingIndex)
{
	std::list<DWORD>::iterator itor = m_dwSelectedSlotIndexList.begin();
	for (; itor != m_dwSelectedSlotIndexList.end();)
	{
		if (dwSelectingIndex == *itor)
		{
			itor = m_dwSelectedSlotIndexList.erase(itor);
			return;
		}
		else
		{
			++itor;
		}
	}

	TSlot* pSlot;
	if (GetSlotPointer(dwSelectingIndex, &pSlot))
	{
		if (!pSlot->isItem)
			return;

		m_dwSelectedSlotIndexList.push_back(dwSelectingIndex);
	}
}

BOOL CSlotWindow::isSelectedSlot(DWORD dwIndex)
{
	std::list<DWORD>::iterator itor = m_dwSelectedSlotIndexList.begin();
	for (; itor != m_dwSelectedSlotIndexList.end(); ++itor)
	{
		if (dwIndex == *itor)
			return true;
	}

	return false;
}

void CSlotWindow::ClearSelected()
{
	m_dwSelectedSlotIndexList.clear();
}

DWORD CSlotWindow::GetSelectedSlotCount()
{
	return m_dwSelectedSlotIndexList.size();
}

DWORD CSlotWindow::GetSelectedSlotNumber(DWORD dwIndex)
{
	if (dwIndex >= m_dwSelectedSlotIndexList.size())
		return DWORD(-1);

	DWORD dwCount = 0;
	std::list<DWORD>::iterator itor = m_dwSelectedSlotIndexList.begin();
	for (; itor != m_dwSelectedSlotIndexList.end(); ++itor)
	{
		if (dwIndex == dwCount)
			break;

		++dwCount;
	}

	return *itor;
}

void CSlotWindow::ShowSlotButton(DWORD dwSlotNumber)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwSlotNumber, &pSlot))
		return;

	if (pSlot->pSlotButton)
	{
		pSlot->pSlotButton->Show();
	}
}

void CSlotWindow::HideAllSlotButton()
{
	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;

		if (rSlot.pSlotButton)
		{
			rSlot.pSlotButton->Hide();
		}
	}
}

void CSlotWindow::ShowRequirementSign(DWORD dwSlotNumber)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwSlotNumber, &pSlot))
		return;

	if (!pSlot->pSignImage)
		return;

	pSlot->pSignImage->Show();
}

void CSlotWindow::HideRequirementSign(DWORD dwSlotNumber)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwSlotNumber, &pSlot))
		return;

	if (!pSlot->pSignImage)
		return;

	pSlot->pSignImage->Hide();
}

// Event

BOOL CSlotWindow::OnMouseLeftButtonDown()
{
	TSlot* pSlot;
	if (!GetPickedSlotPointer(&pSlot))
	{
		UI::CWindowManager::Instance().DeattachIcon();
		return true;
	}

#ifdef WJ_ENABLE_TRADABLE_ICON
	if (!(pSlot->dwState & SLOT_STATE_CANT_MOUSE_EVENT))
		if (pSlot->isItem && !(pSlot->dwState & SLOT_STATE_LOCK))
			OnSelectItemSlot(pSlot->dwSlotNumber);
		else
			OnSelectEmptySlot(pSlot->dwSlotNumber);
#else
	if (pSlot->isItem && !(pSlot->dwState & SLOT_STATE_LOCK))
		OnSelectItemSlot(pSlot->dwSlotNumber);
	else
		OnSelectEmptySlot(pSlot->dwSlotNumber);
#endif

	return true;
}

BOOL CSlotWindow::OnMouseLeftButtonUp()
{
	if (UI::CWindowManager::Instance().IsAttaching())
		if (UI::CWindowManager::Instance().IsDragging())
			if (IsIn())
			{
				UI::CWindow* pWin = UI::CWindowManager::Instance().GetPointWindow();

				if (pWin)
					if (pWin->IsType(UI::CSlotWindow::Type()))
					{
						UI::CSlotWindow* pSlotWin = (UI::CSlotWindow*)pWin;

						TSlot* pSlot;
						if (!pSlotWin->GetPickedSlotPointer(&pSlot))
						{
							UI::CWindowManager::Instance().DeattachIcon();
							return true;
						}

#ifdef WJ_ENABLE_TRADABLE_ICON
						if (!(pSlot->dwState & SLOT_STATE_CANT_MOUSE_EVENT))
							if (pSlot->isItem)
								pSlotWin->OnSelectItemSlot(pSlot->dwSlotNumber);
							else
								pSlotWin->OnSelectEmptySlot(pSlot->dwSlotNumber);
#else
						if (pSlot->isItem)
							pSlotWin->OnSelectItemSlot(pSlot->dwSlotNumber);
						else
							pSlotWin->OnSelectEmptySlot(pSlot->dwSlotNumber);
#endif

						return true;
					}

				return false;
			}

	return false;
}

BOOL CSlotWindow::OnMouseRightButtonDown()
{
	TSlot* pSlot;
	if (!GetPickedSlotPointer(&pSlot))
		return true;

#ifdef WJ_ENABLE_TRADABLE_ICON
	if (!(pSlot->dwState & SLOT_STATE_CANT_MOUSE_EVENT))
		if (pSlot->isItem)
			OnUnselectItemSlot(pSlot->dwSlotNumber);
		else
			OnUnselectEmptySlot(pSlot->dwSlotNumber);
#else
	if (pSlot->isItem)
		OnUnselectItemSlot(pSlot->dwSlotNumber);
	else
		OnUnselectEmptySlot(pSlot->dwSlotNumber);
#endif

	return true;
}

BOOL CSlotWindow::OnMouseLeftButtonDoubleClick()
{
	TSlot* pSlot;
#ifdef WJ_ENABLE_TRADABLE_ICON
	if (GetPickedSlotPointer(&pSlot))
		if (!(pSlot->dwState & SLOT_STATE_CANT_MOUSE_EVENT))
			OnUseSlot();
#else
	if (GetPickedSlotPointer(&pSlot))
		OnUseSlot();
#endif

	return true;
}

void CSlotWindow::OnMouseOverOut()
{
	OnOverOutItem();
	OnOverOut();
}

void CSlotWindow::OnMouseOver()
{
	// FIXME : 윈도우를 드래깅 하는 도중에 SetTop이 되어버리면 Capture가 풀어져 버린다. 그것의 방지 코드.
	//         좀 더 근본적인 해결책을 찾아야 할 듯 - [levites]
//	if (UI::CWindowManager::Instance().IsCapture())
//	if (!UI::CWindowManager::Instance().IsAttaching())
//		return;

	TSlot* pSlot;
	CWindow* pPointWindow = UI::CWindowManager::Instance().GetPointWindow();
	if (this == pPointWindow)
	{
		if (GetPickedSlotPointer(&pSlot))
		{
			if (OnOverInItem(pSlot->dwSlotNumber))
				return;
			else
				OnOverOutItem();

			if (OnOverIn(pSlot->dwSlotNumber))
				return;
			else
				OnOverOut();

			return;
		}
	}

	OnOverOutItem();
	OnOverOut();
}

void CSlotWindow::OnSelectEmptySlot(int iSlotNumber)
{
	PyCallClassMemberFunc(m_poHandler, "OnSelectEmptySlot", Py_BuildValue("(i)", iSlotNumber));
}
void CSlotWindow::OnSelectItemSlot(int iSlotNumber)
{
	//	OnOverOutItem();
	PyCallClassMemberFunc(m_poHandler, "OnSelectItemSlot", Py_BuildValue("(i)", iSlotNumber));

	if (UI::CWindowManager::Instance().IsAttaching())
		OnOverOutItem();
}
void CSlotWindow::OnUnselectEmptySlot(int iSlotNumber)
{
	PyCallClassMemberFunc(m_poHandler, "OnUnselectEmptySlot", Py_BuildValue("(i)", iSlotNumber));
}
void CSlotWindow::OnUnselectItemSlot(int iSlotNumber)
{
	PyCallClassMemberFunc(m_poHandler, "OnUnselectItemSlot", Py_BuildValue("(i)", iSlotNumber));
}
void CSlotWindow::OnUseSlot()
{
	TSlot* pSlot;
	if (GetPickedSlotPointer(&pSlot))
		if (pSlot->isItem)
		{
			PyCallClassMemberFunc(m_poHandler, "OnUseSlot", Py_BuildValue("(i)", pSlot->dwSlotNumber));
		}
}

BOOL CSlotWindow::OnOverIn(DWORD dwSlotNumber)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwSlotNumber, &pSlot))
		return false;

	if (pSlot->isItem)
		return false;

	if (pSlot->dwSlotNumber == m_dwOverInSlotNumber)
		return true;

	m_dwOverInSlotNumber = dwSlotNumber;
	PyCallClassMemberFunc(m_poHandler, "OnOverIn", Py_BuildValue("(i)", dwSlotNumber));
	return true;
}

void CSlotWindow::OnOverOut()
{
	if (SLOT_NUMBER_NONE == m_dwOverInSlotNumber)
		return;

	m_dwOverInSlotNumber = SLOT_NUMBER_NONE;
	PyCallClassMemberFunc(m_poHandler, "OnOverOut", Py_BuildValue("()"));
}

BOOL CSlotWindow::OnOverInItem(DWORD dwSlotNumber)
{
	TSlot* pSlot;
	if (!GetSlotPointer(dwSlotNumber, &pSlot))
		return false;

	if (!pSlot->isItem)
		return false;

	if (pSlot->dwSlotNumber == m_dwToolTipSlotNumber)
		return true;

	m_dwToolTipSlotNumber = dwSlotNumber;
	PyCallClassMemberFunc(m_poHandler, "OnOverInItem", Py_BuildValue("(i)", dwSlotNumber));

	return true;
}

void CSlotWindow::OnOverOutItem()
{
	if (SLOT_NUMBER_NONE == m_dwToolTipSlotNumber)
		return;

	m_dwToolTipSlotNumber = SLOT_NUMBER_NONE;
	PyCallClassMemberFunc(m_poHandler, "OnOverOutItem", Py_BuildValue("()"));
}

void CSlotWindow::OnPressedSlotButton(DWORD dwType, DWORD dwSlotNumber, BOOL isLeft)
{
	if (CSlotButton::SLOT_BUTTON_TYPE_PLUS == dwType)
	{
		PyCallClassMemberFunc(m_poHandler, "OnPressedSlotButton", Py_BuildValue("(i)", dwSlotNumber));
	}
	else if (CSlotButton::SLOT_BUTTON_TYPE_COVER == dwType)
	{
		if (isLeft)
		{
			OnMouseLeftButtonDown();
		}
	}
}

void CSlotWindow::OnUpdate()
{
	for (std::deque<DWORD>::iterator itor = m_ReserveDestroyEffectDeque.begin(); itor != m_ReserveDestroyEffectDeque.end(); ++itor)
	{
		DWORD dwSlotIndex = *itor;

		TSlot* pSlot;
		if (!GetSlotPointer(dwSlotIndex, &pSlot))
			continue;

		__DestroyFinishCoolTimeEffect(pSlot);
	}
	m_ReserveDestroyEffectDeque.clear();

	if (m_pSlotActiveEffect)
		m_pSlotActiveEffect->Update();
}

void CSlotWindow::OnRender()
{
	RenderSlotBaseImage();

	switch (m_dwSlotStyle)
	{
	case SLOT_STYLE_PICK_UP:
		OnRenderPickingSlot();
		break;
	case SLOT_STYLE_SELECT:
		OnRenderSelectedSlot();
		break;
	}

	std::for_each(m_pChildList.begin(), m_pChildList.end(), std::mem_fn(&CWindow::OnRender));

	TSlotListIterator itor;

	//
	// 모든 슬롯 상자 그리기
	//////////////////////////////////////////////////////////////////////////
#ifdef __RENDER_SLOT_AREA__
	CPythonGraphic::Instance().SetDiffuseColor(0.5f, 0.5f, 0.5f);
	for (itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;
		CPythonGraphic::Instance().RenderBox2d(m_rect.left + rSlot.ixPosition,
			m_rect.top + rSlot.iyPosition,
			m_rect.left + rSlot.ixPosition + rSlot.ixCellSize,
			m_rect.top + rSlot.iyPosition + rSlot.iyCellSize);
	}
	CPythonGraphic::Instance().SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	CPythonGraphic::Instance().RenderBox2d(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
#endif
	//////////////////////////////////////////////////////////////////////////

	for (itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;

		if (rSlot.pSlotButton)
		{
			rSlot.pSlotButton->SetPosition(rSlot.ixPosition + 1, rSlot.iyPosition + 19);
		}
		if (rSlot.pSignImage)
		{
			rSlot.pSignImage->SetPosition(rSlot.ixPosition - 7, rSlot.iyPosition + 10);
		}

		if (rSlot.pInstance)
		{
			rSlot.pInstance->SetPosition(m_rect.left + rSlot.ixPosition, m_rect.top + rSlot.iyPosition);
			rSlot.pInstance->Render();
		}

#if defined(ENABLE_SLOT_COVER_IMAGE_SYSTEM)
		if (rSlot.pCoverImage)
		{
			rSlot.pCoverImage->SetPosition(m_rect.left + rSlot.ixPosition, m_rect.top + rSlot.iyPosition);
			rSlot.pCoverImage->Render();
		}
#endif

		if (!rSlot.isItem)
		{
			if (IS_SET(rSlot.dwState, SLOT_STATE_ALWAYS_RENDER_COVER))
			{
				rSlot.pCoverButton->Show();
				rSlot.pCoverButton->SetPosition(rSlot.ixPosition, rSlot.iyPosition);
				rSlot.pCoverButton->RenderButton();
			}

			continue;
		}

		if (IS_SET(rSlot.dwState, SLOT_STATE_DISABLE))
		{
			CPythonGraphic::Instance().SetDiffuseColor(1.0f, 0.0f, 0.0f, 0.3f);
			CPythonGraphic::Instance().RenderBar2d(m_rect.left + rSlot.ixPosition,
				m_rect.top + rSlot.iyPosition,
				m_rect.left + rSlot.ixPosition + rSlot.ixCellSize,
				m_rect.top + rSlot.iyPosition + rSlot.iyCellSize);
		}

#ifdef WJ_ENABLE_TRADABLE_ICON
		if (IS_SET(rSlot.dwState, SLOT_STATE_CANT_MOUSE_EVENT))
		{
			CPythonGraphic::Instance().SetDiffuseColor(1.0f, 0.0f, 0.0f, 0.3f);
			CPythonGraphic::Instance().RenderBar2d(m_rect.left + rSlot.ixPosition,
				m_rect.top + rSlot.iyPosition,
				m_rect.left + rSlot.ixPosition + rSlot.byxPlacedItemSize * rSlot.ixCellSize,
				m_rect.top + rSlot.iyPosition + rSlot.byyPlacedItemSize * rSlot.iyCellSize);
		}

		if (IS_SET(rSlot.dwState, SLOT_STATE_UNUSABLE_ON_TOP_WND))
		{
			CPythonGraphic::Instance().SetDiffuseColor(1.0f, 1.0f, 1.0f, 0.3f);
			CPythonGraphic::Instance().RenderBar2d(m_rect.left + rSlot.ixPosition,
				m_rect.top + rSlot.iyPosition,
				m_rect.left + rSlot.ixPosition + rSlot.byxPlacedItemSize * rSlot.ixCellSize,
				m_rect.top + rSlot.iyPosition + rSlot.byyPlacedItemSize * rSlot.iyCellSize);
		}
#endif

		if (IS_SET(rSlot.dwState, SLOT_STATE_HIGHLIGHT_GREEN))
		{
			CPythonGraphic::Instance().SetDiffuseColor(74.0f / 255.f, 1.0f, 74.0f / 255.0f, 0.3f);
			CPythonGraphic::Instance().RenderBar2d(m_rect.left + rSlot.ixPosition,
				m_rect.top + rSlot.iyPosition,
				m_rect.left + rSlot.ixPosition + rSlot.byxPlacedItemSize * rSlot.ixCellSize,
				m_rect.top + rSlot.iyPosition + rSlot.byyPlacedItemSize * rSlot.iyCellSize);
		}

		if (rSlot.fCoolTime != 0.0f)
		{
			float fcurTime = CTimer::Instance().GetCurrentSecond();
			float fPercentage = (fcurTime - rSlot.fStartCoolTime) / rSlot.fCoolTime;

			CPythonGraphic::Instance().RenderCoolTimeBox(m_rect.left + rSlot.ixPosition + 16.0f, m_rect.top + rSlot.iyPosition + 16.0f, 16.0f, fPercentage);

			if (fcurTime - rSlot.fStartCoolTime >= rSlot.fCoolTime)
			{
				// 쿨타임이 끝난지 1초 이내라면..
				if ((fcurTime - rSlot.fStartCoolTime) - rSlot.fCoolTime < 1.0f)
					__CreateFinishCoolTimeEffect(&rSlot);

				rSlot.fCoolTime = 0.0f;
				rSlot.fStartCoolTime = 0.0f;
			}
		}

		if (rSlot.pCoverButton)
		{
			rSlot.pCoverButton->SetPosition(rSlot.ixPosition, rSlot.iyPosition);
			rSlot.pCoverButton->RenderButton();
		}

		if (rSlot.pNumberLine)
		{
			int ix = rSlot.byxPlacedItemSize * ITEM_WIDTH + rSlot.ixPosition - 4;
			int iy = rSlot.iyPosition + rSlot.byyPlacedItemSize * ITEM_HEIGHT - 12 + 2;

			rSlot.pNumberLine->SetPosition(ix, iy);
			rSlot.pNumberLine->Update();
			rSlot.pNumberLine->Render();
		}

		if (rSlot.pFinishCoolTimeEffect)
		{
			rSlot.pFinishCoolTimeEffect->SetPosition(rSlot.ixPosition, rSlot.iyPosition);
			rSlot.pFinishCoolTimeEffect->Update();
			rSlot.pFinishCoolTimeEffect->Render();
		}

		if (rSlot.bActive)
		{
			if (m_pSlotActiveEffect)
			{
				m_pSlotActiveEffect->SetPosition(m_rect.left + rSlot.ixPosition, m_rect.top + rSlot.iyPosition);
				m_pSlotActiveEffect->Render();
			}
		}

#if defined(WJ_ENABLE_PICKUP_ITEM_EFFECT)
		for (const auto& pSlotEffect : rSlot.pSlotEffect)
		{
			if (pSlotEffect == nullptr || pSlotEffect->IsShow() == false)
				continue;

			pSlotEffect->SetPosition(m_rect.left + rSlot.ixPosition, m_rect.top + rSlot.iyPosition);
			pSlotEffect->Update();
			pSlotEffect->Render();
		}
#endif

#if defined(ENABLE_HIGH_LIGHT_IMAGE)
		if (rSlot.pHighLightImage && rSlot.bEnableHighLight)
		{
			rSlot.pHighLightImage->SetPosition(
				rSlot.ixPosition + rSlot.byxPlacedItemSize * rSlot.ixCellSize / 2 - rSlot.pHighLightImage->GetWidth() / 2,
				rSlot.iyPosition + rSlot.iyCellSize / 2 - rSlot.pHighLightImage->GetHeight() / 2
			);
		}
#endif
	}

	RenderLockedSlot();
}

void CSlotWindow::RenderSlotBaseImage()
{
	if (!m_pBaseImageInstance)
		return;

	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;

		if (!rSlot.bRenderBaseSlotImage)
			continue;

		m_pBaseImageInstance->SetPosition(m_rect.left + rSlot.ixPosition, m_rect.top + rSlot.iyPosition);
		m_pBaseImageInstance->Render();
	}
}

void CSlotWindow::OnRenderPickingSlot()
{
	if (!UI::CWindowManager::Instance().IsAttaching())
		return;

	TSlot* pSlot;
	if (!GetSelectedSlotPointer(&pSlot))
		return;

	CPythonGraphic::Instance().SetDiffuseColor(1.0f, 1.0f, 1.0f, 0.5f);
	CPythonGraphic::Instance().RenderBar2d(m_rect.left + pSlot->ixPosition,
		m_rect.top + pSlot->iyPosition,
		m_rect.left + pSlot->ixPosition + pSlot->ixCellSize,
		m_rect.top + pSlot->iyPosition + pSlot->iyCellSize);
}

void CSlotWindow::OnRenderSelectedSlot()
{
	std::list<DWORD>::iterator itor = m_dwSelectedSlotIndexList.begin();
	for (; itor != m_dwSelectedSlotIndexList.end(); ++itor)
	{
		TSlot* pSlot;
		if (!GetSlotPointer(*itor, &pSlot))
			continue;

		CPythonGraphic::Instance().SetDiffuseColor(1.0f, 1.0f, 1.0f, 0.5f);
		CPythonGraphic::Instance().RenderBar2d(m_rect.left + pSlot->ixPosition,
			m_rect.top + pSlot->iyPosition,
			m_rect.left + pSlot->ixPosition + pSlot->ixCellSize,
			m_rect.top + pSlot->iyPosition + pSlot->iyCellSize);
	}
}

void CSlotWindow::RenderLockedSlot()
{
	CPythonGraphic::Instance().SetDiffuseColor(0.0f, 0.0f, 0.0f, 0.5f);
	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;

		if (!rSlot.isItem)
			continue;

		if (rSlot.dwState & SLOT_STATE_LOCK)
		{
			CPythonGraphic::Instance().RenderBar2d(m_rect.left + rSlot.ixPosition,
				m_rect.top + rSlot.iyPosition,
				m_rect.left + rSlot.ixPosition + rSlot.ixCellSize,
				m_rect.top + rSlot.iyPosition + rSlot.iyCellSize);
		}
	}
}

// Check Slot

BOOL CSlotWindow::GetSlotPointer(DWORD dwIndex, TSlot** ppSlot)
{
	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;

		if (dwIndex == rSlot.dwSlotNumber)
		{
			*ppSlot = &rSlot;
			return true;
		}
	}

	return false;
}

BOOL CSlotWindow::GetSelectedSlotPointer(TSlot** ppSlot)
{
	long lx, ly;
	GetMouseLocalPosition(lx, ly);

	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;

		if (lx >= rSlot.ixPosition)
		{
			if (ly >= rSlot.iyPosition)
			{
				if (lx <= rSlot.ixPosition + rSlot.ixCellSize)
				{
					if (ly <= rSlot.iyPosition + rSlot.iyCellSize)
					{
						*ppSlot = &rSlot;
						return true;
					}
				}
			}
		}
	}

	return false;
}

BOOL CSlotWindow::GetPickedSlotPointer(TSlot** ppSlot)
{
	long lx, ly;
	CWindowManager::Instance().GetMousePosition(lx, ly);

	int ixLocal = lx - m_rect.left;
	int iyLocal = ly - m_rect.top;

	// NOTE : 왼쪽 맨위 상단 한곳이 기준 이라는 점을 이용해 왼쪽 위에서부터 오른쪽 아래로
	// 차례로 검색해 감으로써 덮혀 있는 Slot은 자동 무시 된다는 특성을 이용한다. - [levites]
	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;

		int ixCellSize = rSlot.ixCellSize;
		int iyCellSize = rSlot.iyCellSize;

		// NOTE : Item이 Hide 되어있을 경우를 위한..
		if (rSlot.isItem)
		{
			ixCellSize = max(rSlot.ixCellSize, int(rSlot.byxPlacedItemSize * ITEM_WIDTH));
			iyCellSize = max(rSlot.iyCellSize, int(rSlot.byyPlacedItemSize * ITEM_HEIGHT));
		}

		if (ixLocal >= rSlot.ixPosition)
			if (iyLocal >= rSlot.iyPosition)
				if (ixLocal <= rSlot.ixPosition + ixCellSize)
					if (iyLocal <= rSlot.iyPosition + iyCellSize)
					{
						*ppSlot = &rSlot;
						return true;
					}
	}

	return false;
}

void CSlotWindow::SetUseMode(BOOL bFlag)
{
	m_isUseMode = bFlag;
}

void CSlotWindow::SetUsableItem(BOOL bFlag)
{
	m_isUsableItem = bFlag;
}

void CSlotWindow::ReserveDestroyCoolTimeFinishEffect(DWORD dwSlotIndex)
{
	m_ReserveDestroyEffectDeque.push_back(dwSlotIndex);
}

DWORD CSlotWindow::Type()
{
	static int s_Type = GetCRC32("CSlotWindow", strlen("CSlotWindow"));
	return s_Type;
}

BOOL CSlotWindow::OnIsType(DWORD dwType)
{
	if (CSlotWindow::Type() == dwType)
		return true;

	return CWindow::OnIsType(dwType);
}

void CSlotWindow::__CreateToggleSlotImage()
{
	__DestroyToggleSlotImage();

	m_pToggleSlotImage = new CImageBox(NULL);
	m_pToggleSlotImage->LoadImage("d:/ymir work/ui/public/slot_toggle.sub");
	m_pToggleSlotImage->Show();
}

void CSlotWindow::__CreateSlotEnableEffect()
{
	__DestroySlotEnableEffect();

	m_pSlotActiveEffect = new CAniImageBox(NULL);
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/00.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/01.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/02.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/03.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/04.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/05.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/06.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/07.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/08.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/09.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/10.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/11.sub");
	m_pSlotActiveEffect->AppendImage("d:/ymir work/ui/public/slotactiveeffect/12.sub");
	m_pSlotActiveEffect->SetRenderingMode(CGraphicExpandedImageInstance::RENDERING_MODE_SCREEN);
	m_pSlotActiveEffect->Show();
}

void CSlotWindow::__CreateFinishCoolTimeEffect(TSlot* pSlot)
{
	__DestroyFinishCoolTimeEffect(pSlot);

	CAniImageBox* pFinishCoolTimeEffect = new CCoolTimeFinishEffect(this, pSlot->dwSlotNumber);
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/00.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/01.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/02.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/03.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/04.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/05.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/06.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/07.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/08.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/09.sub");
	pFinishCoolTimeEffect->AppendImage("d:/ymir work/ui/public/slotfinishcooltimeeffect/10.sub");
	pFinishCoolTimeEffect->SetRenderingMode(CGraphicExpandedImageInstance::RENDERING_MODE_SCREEN);
	pFinishCoolTimeEffect->ResetFrame();
	pFinishCoolTimeEffect->SetDelay(2);
	pFinishCoolTimeEffect->Show();

	pSlot->pFinishCoolTimeEffect = pFinishCoolTimeEffect;
}

void CSlotWindow::__CreateBaseImage(const char* c_szFileName, float fr, float fg, float fb, float fa)
{
	__DestroyBaseImage();

	CGraphicImage* pImage = (CGraphicImage*)CResourceManager::Instance().GetResourcePointer(c_szFileName);
	m_pBaseImageInstance = CGraphicImageInstance::New();
	m_pBaseImageInstance->SetImagePointer(pImage);
	m_pBaseImageInstance->SetDiffuseColor(fr, fg, fb, fa);
}

void CSlotWindow::__DestroyToggleSlotImage()
{
	SAFE_DELETE(m_pToggleSlotImage)
}

void CSlotWindow::__DestroySlotEnableEffect()
{
	SAFE_DELETE(m_pSlotActiveEffect)
}

void CSlotWindow::__DestroyFinishCoolTimeEffect(TSlot* pSlot)
{
	SAFE_DELETE(pSlot->pFinishCoolTimeEffect)
}

void CSlotWindow::__DestroyBaseImage()
{
	if (m_pBaseImageInstance)
	{
		CGraphicImageInstance::Delete(m_pBaseImageInstance);
		m_pBaseImageInstance = NULL;
	}
}

void CSlotWindow::__Initialize()
{
	m_dwSlotType = 0;
	m_dwSlotStyle = SLOT_STYLE_PICK_UP;
	m_dwToolTipSlotNumber = SLOT_NUMBER_NONE;
	m_dwOverInSlotNumber = SLOT_NUMBER_NONE;
	m_CoolDownStore.clear();

	m_isUseMode = FALSE;
	m_isUsableItem = FALSE;

	m_pToggleSlotImage = NULL;
	m_pSlotActiveEffect = NULL;
	m_pBaseImageInstance = NULL;
}

void CSlotWindow::Destroy()
{
	for (TSlotListIterator itor = m_SlotList.begin(); itor != m_SlotList.end(); ++itor)
	{
		TSlot& rSlot = *itor;

		ClearSlot(&rSlot);

		SAFE_DELETE(rSlot.pNumberLine)

		if (rSlot.pCoverButton)
			CWindowManager::Instance().DestroyWindow(rSlot.pCoverButton);

		if (rSlot.pSlotButton)
			CWindowManager::Instance().DestroyWindow(rSlot.pSlotButton);

		if (rSlot.pSignImage)
			CWindowManager::Instance().DestroyWindow(rSlot.pSignImage);

		if (rSlot.pFinishCoolTimeEffect)
			CWindowManager::Instance().DestroyWindow(rSlot.pFinishCoolTimeEffect);

#if defined(ENABLE_HIGH_LIGHT_IMAGE)
		if (rSlot.pHighLightImage)
			CWindowManager::Instance().DestroyWindow(rSlot.pHighLightImage);
#endif
	}

	m_SlotList.clear();

	__DestroyToggleSlotImage();
	__DestroySlotEnableEffect();
	__DestroyBaseImage();

	__Initialize();
}

CSlotWindow::CSlotWindow(PyObject* ppyObject) : CWindow(ppyObject)
{
	__Initialize();
}

CSlotWindow::~CSlotWindow()
{
	Destroy();
}
