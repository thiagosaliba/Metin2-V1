#include "StdAfx.h"
#include "GrpTextInstance.h"
#include "StateManager.h"
#include "IME.h"
#include "TextTag.h"
#include "../EterLocale/StringCodec.h"
#include "../EterBase/Utils.h"
#include "../EterLocale/Arabic.h"
#ifdef ENABLE_TEXT_IMAGE_LINE
#	include "ResourceManager.h"
#endif

extern DWORD GetDefaultCodePage();

const float c_fFontFeather = 0.5f;

CDynamicPool<CGraphicTextInstance> CGraphicTextInstance::ms_kPool;

static int gs_mx = 0;
static int gs_my = 0;

static std::wstring gs_hyperlinkText;

void CGraphicTextInstance::Hyperlink_UpdateMousePos(int x, int y)
{
	gs_mx = x;
	gs_my = y;
	gs_hyperlinkText = L"";
}

int CGraphicTextInstance::Hyperlink_GetText(char* buf, int len)
{
	if (gs_hyperlinkText.empty())
		return 0;

	int codePage = GetDefaultCodePage();

	return Ymir_WideCharToMultiByte(codePage, 0, gs_hyperlinkText.c_str(), gs_hyperlinkText.length(), buf, len, NULL, NULL);
}

int CGraphicTextInstance::__DrawCharacter(CGraphicFontTexture* pFontTexture, WORD codePage, wchar_t text, DWORD dwColor)
{
	CGraphicFontTexture::TCharacterInfomation* pInsCharInfo = pFontTexture->GetCharacterInfomation(codePage, text);

#ifdef WJ_MULTI_TEXTLINE
	bool bHasToken = false;
	if (pInsCharInfo && !m_bDisableEnterToken)
	{
		if (m_wcEndLine != NULL)
		{
			if (m_wcEndLine == '\\' && text == 'n')
			{
				m_pCharInfoVector.erase(m_pCharInfoVector.begin() + m_pCharInfoVector.size() - 1);
				m_bEnterTokenVector.erase(m_bEnterTokenVector.begin() + m_bEnterTokenVector.size() - 1);
				bHasToken = true;
			}
		}
	}
	m_wcEndLine = text;
#endif

	if (pInsCharInfo)
	{
#ifdef WJ_MULTI_TEXTLINE
		m_bEnterTokenVector.push_back(bHasToken);
#endif
		m_dwColorInfoVector.push_back(dwColor);
		m_pCharInfoVector.push_back(pInsCharInfo);

		m_textWidth += pInsCharInfo->advance;
		m_textHeight = max(pInsCharInfo->height, m_textHeight);
		return pInsCharInfo->advance;
	}

	return 0;
}

void CGraphicTextInstance::__GetTextPos(DWORD index, float* x, float* y)
{
	index = min(index, m_pCharInfoVector.size());

	float sx = 0;
	float sy = 0;
	float fFontMaxHeight = 0;

	for (DWORD i = 0; i < index; ++i)
	{
		if (sx + float(m_pCharInfoVector[i]->width) > m_fLimitWidth)
		{
			sx = 0;
			sy += fFontMaxHeight;
		}

		sx += float(m_pCharInfoVector[i]->advance);
		fFontMaxHeight = max(float(m_pCharInfoVector[i]->height), fFontMaxHeight);
	}

	*x = sx;
	*y = sy;
}

bool isNumberic(const char chr)
{
	if (chr >= '0' && chr <= '9')
		return true;
	return false;
}

bool IsValidToken(const char* iter)
{
	return	iter[0] == '@' &&
		isNumberic(iter[1]) &&
		isNumberic(iter[2]) &&
		isNumberic(iter[3]) &&
		isNumberic(iter[4]);
}

const char* FindToken(const char* begin, const char* end)
{
	while (begin < end)
	{
		begin = std::find(begin, end, '@');

		if (end - begin > 5 && IsValidToken(begin))
		{
			return begin;
		}
		else
		{
			++begin;
		}
	}

	return end;
}

int ReadToken(const char* token)
{
	int nRet = (token[1] - '0') * 1000 + (token[2] - '0') * 100 + (token[3] - '0') * 10 + (token[4] - '0');
	if (nRet == 9999)
		return CP_UTF8;
	return nRet;
}

void CGraphicTextInstance::Update()
{
	if (m_isUpdate) // ���ڿ��� �ٲ���� ���� ������Ʈ �Ѵ�.
		return;

	if (m_roText.IsNull())
	{
		Tracef("CGraphicTextInstance::Update - ��Ʈ�� �������� �ʾҽ��ϴ�\n");
		return;
	}

	if (m_roText->IsEmpty())
		return;

	CGraphicFontTexture* pFontTexture = m_roText->GetFontTexturePointer();
	if (!pFontTexture)
		return;

	UINT defCodePage = GetDefaultCodePage();

	UINT dataCodePage = defCodePage; // �ƶ� �� ��Ʈ�� ���� �����͸� UTF8 �� ����Ϸ� ������ ����

	CGraphicFontTexture::TCharacterInfomation* pSpaceInfo = pFontTexture->GetCharacterInfomation(dataCodePage, ' ');

	int spaceHeight = pSpaceInfo ? pSpaceInfo->height : 12;

	m_pCharInfoVector.clear();
	m_dwColorInfoVector.clear();
	m_hyperlinkVector.clear();

#ifdef WJ_MULTI_TEXTLINE
	m_bEnterTokenVector.clear();
	m_wcEndLine = NULL;
#endif

#ifdef ENABLE_TEXT_IMAGE_LINE
	if (m_imageVector.size() != 0)
	{
		for (std::vector<SImage>::iterator itor = m_imageVector.begin(); itor != m_imageVector.end(); ++itor)
		{
			SImage& rImg = *itor;
			if (rImg.pInstance)
			{
				CGraphicImageInstance::Delete(rImg.pInstance);
				rImg.pInstance = NULL;
			}
		}
	}
	m_imageVector.clear();
#endif

	m_textWidth = 0;
#ifdef WJ_MULTI_TEXTLINE
	m_textHeight = (m_iLineHeight != 0) ? m_iLineHeight : spaceHeight;
#else
	m_textHeight = spaceHeight;
#endif

	/* wstring begin */

	const char* begin = m_stText.c_str();
	const char* end = begin + m_stText.length();

	int wTextMax = (end - begin) * 2;
	wchar_t* wText = (wchar_t*)_alloca(sizeof(wchar_t) * wTextMax);

	DWORD dwColor = m_dwTextColor;

	/* wstring end */
	while (begin < end)
	{
		const char* token = FindToken(begin, end);

		int wTextLen = Ymir_MultiByteToWideChar(dataCodePage, 0, begin, token - begin, wText, wTextMax);

		if (m_isSecret)
		{
			for (int i = 0; i < wTextLen; ++i)
				__DrawCharacter(pFontTexture, dataCodePage, '*', dwColor);
		}
		else
		{
			if (defCodePage == CP_ARABIC) // ARABIC
			{

				wchar_t* wArabicText = (wchar_t*)_alloca(sizeof(wchar_t) * wTextLen);
				int wArabicTextLen = Arabic_MakeShape(wText, wTextLen, wArabicText, wTextLen);

				bool isEnglish = true;
				int nEnglishBase = wArabicTextLen - 1;

				// <<������ ��ũ>>
				int x = 0;

				int len;
				int hyperlinkStep = 0;
				SHyperlink kHyperlink;
				std::wstring hyperlinkBuffer;
				int no_hyperlink = 0;

				// �ɺ��� ������ �ƶ��� ���� �����ؾ��Ѵ�
				if (Arabic_IsInSymbol(wArabicText[wArabicTextLen - 1]))
				{
					isEnglish = false;
				}

				int i = 0;
				for (i = wArabicTextLen - 1; i >= 0; --i)
				{
					wchar_t wArabicChar = wArabicText[i];

					if (isEnglish)
					{
						// <<�ɺ��� ��� (ex. ��ȣ, ����)>> -> ���� ��� ����.
						// <<(�ɺ��� �ƴ� �͵� : ����, ����, �ƶ���)>>
						//	(1) �� ���� �ɺ� or
						//	(2)
						//		1) �� ���ڰ� �ƶ��� �ƴ� &&
						//		2) �� ���ڰ� �ƶ��� �ƴ� &&
						//		3) �� ���ڰ� �ɺ�'|'�� �ƴ� &&
						//		or
						//	(3) ���� �ɺ��� '|'
						// <<�ƶ��� ���� �Ѿ�� ��� : �ɺ�����.>>
						//	1) �ձ��� �ƶ���
						//	2) �ޱ��� �ƶ���
						//
						if (Arabic_IsInSymbol(wArabicChar) && (
							(i == 0) ||
							(i > 0 &&
								!(Arabic_HasPresentation(wArabicText, i - 1) || Arabic_IsInPresentation(wArabicText[i + 1])) && // �ձ���, �ޱ��ڰ� �ƶ��� �ƴ�.
								wArabicText[i + 1] != '|'
								) ||
							wArabicText[i] == '|'
							)) // if end.
						{
							// pass
							int temptest = 1;
						}
						// (1) �ƶ����̰ų� (2)�ƶ��� ������ �ɺ��̶�� �ƶ��� ��� ��ȯ
						else if (Arabic_IsInPresentation(wArabicChar) || Arabic_IsInSymbol(wArabicChar))
						{
							// �� �������� ��� �׸���.
							for (int e = i + 1; e <= nEnglishBase;)
							{
								int ret = GetTextTag(&wArabicText[e], wArabicTextLen - e, len, hyperlinkBuffer);

								if (ret == TEXT_TAG_PLAIN || ret == TEXT_TAG_TAG)
								{
									if (hyperlinkStep == 1)
										hyperlinkBuffer.append(1, wArabicText[e]);
									else
									{
										int charWidth = __DrawCharacter(pFontTexture, dataCodePage, wArabicText[e], dwColor);
										kHyperlink.ex += charWidth;
										//x += charWidth;

										// ���� �߰��� �����۸�ũ�� ��ǥ ����.
										for (int j = 1; j <= no_hyperlink; j++)
										{
											if (m_hyperlinkVector.size() < j)
												break;

											SHyperlink& tempLink = m_hyperlinkVector[m_hyperlinkVector.size() - j];
											tempLink.ex += charWidth;
											tempLink.sx += charWidth;
										}
									}
								}
								else
								{
									if (ret == TEXT_TAG_COLOR)
										dwColor = htoi(hyperlinkBuffer.c_str(), 8);
									else if (ret == TEXT_TAG_RESTORE_COLOR)
										dwColor = m_dwTextColor;
									else if (ret == TEXT_TAG_HYPERLINK_START)
									{
										hyperlinkStep = 1;
										hyperlinkBuffer = L"";
									}
									else if (ret == TEXT_TAG_HYPERLINK_END)
									{
										if (hyperlinkStep == 1)
										{
											++hyperlinkStep;
											kHyperlink.ex = kHyperlink.sx = 0; // ���� �ؽ�Ʈ�� ���۵Ǵ� ��ġ
										}
										else
										{
											kHyperlink.text = hyperlinkBuffer;
											m_hyperlinkVector.push_back(kHyperlink);
											no_hyperlink++;

											hyperlinkStep = 0;
											hyperlinkBuffer = L"";
										}
									}
								}
								e += len;
							}

							int charWidth = __DrawCharacter(pFontTexture, dataCodePage, Arabic_ConvSymbol(wArabicText[i]), dwColor);
							kHyperlink.ex += charWidth;

							// ���� �߰��� �����۸�ũ�� ��ǥ ����.
							for (int j = 1; j <= no_hyperlink; j++)
							{
								if (m_hyperlinkVector.size() < j)
									break;

								SHyperlink& tempLink = m_hyperlinkVector[m_hyperlinkVector.size() - j];
								tempLink.ex += charWidth;
								tempLink.sx += charWidth;
							}

							isEnglish = false;
						}
					}
					else // [[[�ƶ��� ���]]]
					{
						// �ƶ����̰ų� �ƶ��� ����� ������ �ɺ��̶��
						if (Arabic_IsInPresentation(wArabicChar) || Arabic_IsInSymbol(wArabicChar))
						{
							int charWidth = __DrawCharacter(pFontTexture, dataCodePage, Arabic_ConvSymbol(wArabicText[i]), dwColor);
							kHyperlink.ex += charWidth;
							x += charWidth;

							// ���� �߰��� �����۸�ũ�� ��ǥ ����.
							for (int j = 1; j <= no_hyperlink; j++)
							{
								if (m_hyperlinkVector.size() < j)
									break;

								SHyperlink& tempLink = m_hyperlinkVector[m_hyperlinkVector.size() - j];
								tempLink.ex += charWidth;
								tempLink.sx += charWidth;
							}
						}
						else // �����̰ų�, ���� ������ ������ �ɺ��̶��,
						{
							nEnglishBase = i;
							isEnglish = true;
						}
					}
				}

				if (isEnglish)
				{
					for (int e = i + 1; e <= nEnglishBase;)
					{
						int ret = GetTextTag(&wArabicText[e], wArabicTextLen - e, len, hyperlinkBuffer);

						if (ret == TEXT_TAG_PLAIN || ret == TEXT_TAG_TAG)
						{
							if (hyperlinkStep == 1)
								hyperlinkBuffer.append(1, wArabicText[e]);
							else
							{
								int charWidth = __DrawCharacter(pFontTexture, dataCodePage, wArabicText[e], dwColor);
								kHyperlink.ex += charWidth;

								// ���� �߰��� �����۸�ũ�� ��ǥ ����.
								for (int j = 1; j <= no_hyperlink; j++)
								{
									if (m_hyperlinkVector.size() < j)
										break;

									SHyperlink& tempLink = m_hyperlinkVector[m_hyperlinkVector.size() - j];
									tempLink.ex += charWidth;
									tempLink.sx += charWidth;
								}
							}
						}
						else
						{
							if (ret == TEXT_TAG_COLOR)
								dwColor = htoi(hyperlinkBuffer.c_str(), 8);
							else if (ret == TEXT_TAG_RESTORE_COLOR)
								dwColor = m_dwTextColor;
							else if (ret == TEXT_TAG_HYPERLINK_START)
							{
								hyperlinkStep = 1;
								hyperlinkBuffer = L"";
							}
							else if (ret == TEXT_TAG_HYPERLINK_END)
							{
								if (hyperlinkStep == 1)
								{
									++hyperlinkStep;
									kHyperlink.ex = kHyperlink.sx = 0; // ���� �ؽ�Ʈ�� ���۵Ǵ� ��ġ
								}
								else
								{
									kHyperlink.text = hyperlinkBuffer;
									m_hyperlinkVector.push_back(kHyperlink);
									no_hyperlink++;

									hyperlinkStep = 0;
									hyperlinkBuffer = L"";
								}
							}
						}
						e += len;
					}

				}
			}
			else // �ƶ��� �ٸ� ����.
			{
				int x = 0;
				int len;
				int hyperlinkStep = 0;
				SHyperlink kHyperlink;
				std::wstring hyperlinkBuffer;

#ifdef ENABLE_TEXT_IMAGE_LINE
				SImage kImage;
				int imageStep = 0;
				std::wstring imageBuffer;
#endif

				for (int i = 0; i < wTextLen; )
				{
					int ret = GetTextTag(&wText[i], wTextLen - i, len, hyperlinkBuffer);

					if (ret == TEXT_TAG_PLAIN || ret == TEXT_TAG_TAG)
					{
						if (hyperlinkStep == 1)
							hyperlinkBuffer.append(1, wText[i]);
#ifdef ENABLE_TEXT_IMAGE_LINE
						else if (imageStep == 1)
							imageBuffer.append(1, wText[i]);
#endif
						else
						{
							int charWidth = __DrawCharacter(pFontTexture, dataCodePage, wText[i], dwColor);
							kHyperlink.ex += charWidth;
							x += charWidth;
						}
					}
					else
					{
						if (ret == TEXT_TAG_COLOR)
							dwColor = htoi(hyperlinkBuffer.c_str(), 8);
						else if (ret == TEXT_TAG_RESTORE_COLOR)
							dwColor = m_dwTextColor;
						else if (ret == TEXT_TAG_HYPERLINK_START)
						{
							hyperlinkStep = 1;
							hyperlinkBuffer = L"";
						}
						else if (ret == TEXT_TAG_HYPERLINK_END)
						{
							if (hyperlinkStep == 1)
							{
								++hyperlinkStep;
								kHyperlink.ex = kHyperlink.sx = x; // ���� �ؽ�Ʈ�� ���۵Ǵ� ��ġ
							}
							else
							{
								kHyperlink.text = hyperlinkBuffer;
								m_hyperlinkVector.push_back(kHyperlink);

								hyperlinkStep = 0;
								hyperlinkBuffer = L"";
							}
						}
#ifdef ENABLE_TEXT_IMAGE_LINE
						else if (ret == TEXT_TAG_IMAGE_START)
						{
							imageStep = 1;
							imageBuffer = L"";
						}
						else if (ret == TEXT_TAG_IMAGE_END)
						{
							kImage.x = x;

							char retBuf[1024];
							int retLen = Ymir_WideCharToMultiByte(GetDefaultCodePage(), 0, imageBuffer.c_str(), imageBuffer.length(), retBuf, sizeof(retBuf) - 1, NULL, NULL);
							retBuf[retLen] = '\0';

							char szPath[255];
							_snprintf(szPath, sizeof(szPath), "icon/%s.tga", retBuf);
							if (CResourceManager::Instance().IsFileExist(szPath))
							{
								CGraphicImage* pImage = (CGraphicImage*)CResourceManager::Instance().GetResourcePointer(szPath);
								kImage.pInstance = CGraphicImageInstance::New();
								kImage.pInstance->SetImagePointer(pImage);
								m_imageVector.push_back(kImage);
								memset(&kImage, 0, sizeof(SImage));
								for (int i = 0; i < pImage->GetWidth() / (pSpaceInfo->width - 1); ++i)
									x += __DrawCharacter(pFontTexture, dataCodePage, ' ', dwColor);
								if (pImage->GetWidth() % (pSpaceInfo->width - 1) > 1)
									x += __DrawCharacter(pFontTexture, dataCodePage, ' ', dwColor);
							}
							imageStep = 0;
							imageBuffer = L"";
						}
#endif
					}
					i += len;
				}
			}
		}

		if (token < end)
		{
			int newCodePage = ReadToken(token);
			dataCodePage = newCodePage; // �ƶ� �� ��Ʈ�� ���� �����͸� UTF8 �� ����Ϸ� ������ ����
			begin = token + 5;
		}
		else
		{
			begin = token;
		}
	}

	pFontTexture->UpdateTexture();

	m_isUpdate = true;
}

void CGraphicTextInstance::Render(RECT* pClipRect)
{
	if (!m_isUpdate)
		return;

	CGraphicText* pkText = m_roText.GetPointer();
	if (!pkText)
		return;

	CGraphicFontTexture* pFontTexture = pkText->GetFontTexturePointer();
	if (!pFontTexture)
		return;

	float fStanX = m_v3Position.x;
	float fStanY = m_v3Position.y + 1.0f;

	UINT defCodePage = GetDefaultCodePage();

	if (defCodePage == CP_ARABIC)
	{
		switch (m_hAlign)
		{
		case HORIZONTAL_ALIGN_LEFT:
			fStanX -= m_textWidth;
			break;

		case HORIZONTAL_ALIGN_CENTER:
			fStanX -= float(m_textWidth / 2);
			break;
		}
	}
	else
	{
		switch (m_hAlign)
		{
		case HORIZONTAL_ALIGN_RIGHT:
			fStanX -= m_textWidth;
			break;

		case HORIZONTAL_ALIGN_CENTER:
			fStanX -= float(m_textWidth / 2);
			break;
		}
	}

	switch (m_vAlign)
	{
	case VERTICAL_ALIGN_BOTTOM:
		fStanY -= m_textHeight;
		break;

	case VERTICAL_ALIGN_CENTER:
		fStanY -= float(m_textHeight) / 2.0f;
		break;
	}

	//WORD FillRectIndices[6] = { 0, 2, 1, 2, 3, 1 };

	STATEMANAGER.SaveRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	STATEMANAGER.SaveRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DWORD dwFogEnable = STATEMANAGER.GetRenderState(D3DRS_FOGENABLE);
	DWORD dwLighting = STATEMANAGER.GetRenderState(D3DRS_LIGHTING);
	STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, false);
	STATEMANAGER.SetRenderState(D3DRS_LIGHTING, false);

	STATEMANAGER.SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

#if defined(__VERTEX_BUFFER_PERFORMANCE__)
	{
		const float fFontHalfWeight = 1.0f;

		float fCurX, fCurXoutLine;
		float fCurY, fCurYoutline;

		float fFontSx, fFontSy, fFontEx, fFontEy;
		float fFontWidth, fFontHeight, fFontMaxHeight;
		float fFontAdvance;

		fCurX = fStanX;
		fCurY = fStanY;
		fFontMaxHeight = 0.0f;
		fFontHeight = 0.0f;

		fCurXoutLine = fStanX;
		fCurYoutline = fStanY;

		static SVertex akVertex[CGraphicBase::PDT_TEXTLINE_VERTEX_NUM];

		CGraphicFontTexture::TCharacterInfomation* pCurCharInfo;
		int iActualVertexIdx = 0;

#if defined(WJ_MULTI_TEXTLINE)
		if (HasEnterToken())
		{
			ReAdjustedStanXY(0, fStanX, fStanY);
			fCurX = fStanX;
			fCurY += fFontHeight;

			fCurXoutLine = fStanX;
			fCurYoutline += fFontMaxHeight;
		}
#endif

		for (int i = 0; i < m_pCharInfoVector.size(); ++i)
		{
			if ((iActualVertexIdx + 20) >= CGraphicBase::PDT_TEXTLINE_VERTEX_NUM)
			{
				//TraceError("Buffer is too small.");
				break;
			}

			if (m_isOutline)
			{
				pCurCharInfo = m_pCharInfoVector[i];

				fFontWidth = float(pCurCharInfo->width);
				fFontHeight = float(pCurCharInfo->height);
				fFontAdvance = float(pCurCharInfo->advance);

				if ((fCurXoutLine + fFontWidth) - m_v3Position.x > m_fLimitWidth)
				{
					if (m_isMultiLine)
					{
						fCurXoutLine = fStanX;
						fCurYoutline += fFontMaxHeight;
					}
					else
						break;
				}

#if defined(WJ_MULTI_TEXTLINE)
				if (m_bEnterTokenVector[i] && !m_bDisableEnterToken)
				{
					fCurX = fStanX;
					fCurY += fFontHeight;

					fCurXoutLine = fStanX;
					fCurYoutline += fFontMaxHeight;

					continue;
				}
#endif

				if (pClipRect)
				{
					if (fCurYoutline <= pClipRect->top)
					{
						fCurXoutLine += fFontAdvance;
						continue;
					}
				}

				fFontSx = fCurXoutLine - 0.5f;
				fFontSy = fCurYoutline - 0.5f;
				fFontEx = fFontSx + fFontWidth;
				fFontEy = fFontSy + fFontHeight;

				pFontTexture->SelectTexture(pCurCharInfo->index);
				STATEMANAGER.SetTexture(0, pFontTexture->GetD3DTexture());

				for (int i = iActualVertexIdx; i < (iActualVertexIdx + 16); i += 4)
				{
					akVertex[i].u = pCurCharInfo->left;
					akVertex[i].v = pCurCharInfo->top;
					akVertex[i + 1].u = pCurCharInfo->left;
					akVertex[i + 1].v = pCurCharInfo->bottom;
					akVertex[i + 2].u = pCurCharInfo->right;
					akVertex[i + 2].v = pCurCharInfo->top;
					akVertex[i + 3].u = pCurCharInfo->right;
					akVertex[i + 3].v = pCurCharInfo->bottom;

					akVertex[i].color = akVertex[i + 1].color = akVertex[i + 2].color = akVertex[i + 3].color = m_dwOutlineColor;
				}

				float feather = 0.0f; // m_fFontFeather

				akVertex[iActualVertexIdx + 0].y = fFontSy - feather;
				akVertex[iActualVertexIdx + 1].y = fFontEy + feather;
				akVertex[iActualVertexIdx + 2].y = fFontSy - feather;
				akVertex[iActualVertexIdx + 3].y = fFontEy + feather;

				akVertex[iActualVertexIdx + 0].x = fFontSx - fFontHalfWeight - feather;
				akVertex[iActualVertexIdx + 1].x = fFontSx - fFontHalfWeight - feather;
				akVertex[iActualVertexIdx + 2].x = fFontEx - fFontHalfWeight + feather;
				akVertex[iActualVertexIdx + 3].x = fFontEx - fFontHalfWeight + feather;

				akVertex[iActualVertexIdx + 0].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 1].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 2].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 3].z = m_v3Position.z;

				// 0 - 3 end

				akVertex[iActualVertexIdx + 4].y = fFontSy - feather;
				akVertex[iActualVertexIdx + 5].y = fFontEy + feather;
				akVertex[iActualVertexIdx + 6].y = fFontSy - feather;
				akVertex[iActualVertexIdx + 7].y = fFontEy + feather;

				akVertex[iActualVertexIdx + 4].x = fFontSx + fFontHalfWeight - feather;
				akVertex[iActualVertexIdx + 5].x = fFontSx + fFontHalfWeight - feather;
				akVertex[iActualVertexIdx + 6].x = fFontEx + fFontHalfWeight + feather;
				akVertex[iActualVertexIdx + 7].x = fFontEx + fFontHalfWeight + feather;

				akVertex[iActualVertexIdx + 4].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 5].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 6].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 7].z = m_v3Position.z;

				// 4 - 7 end

				akVertex[iActualVertexIdx + 8].x = fFontSx - feather;
				akVertex[iActualVertexIdx + 9].x = fFontSx - feather;
				akVertex[iActualVertexIdx + 10].x = fFontEx + feather;
				akVertex[iActualVertexIdx + 11].x = fFontEx + feather;

				akVertex[iActualVertexIdx + 8].y = fFontSy - fFontHalfWeight - feather;
				akVertex[iActualVertexIdx + 9].y = fFontEy - fFontHalfWeight + feather;
				akVertex[iActualVertexIdx + 10].y = fFontSy - fFontHalfWeight - feather;
				akVertex[iActualVertexIdx + 11].y = fFontEy - fFontHalfWeight + feather;

				akVertex[iActualVertexIdx + 8].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 9].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 10].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 11].z = m_v3Position.z;

				// 8 - 11 end

				akVertex[iActualVertexIdx + 12].x = fFontSx - feather;
				akVertex[iActualVertexIdx + 13].x = fFontSx - feather;
				akVertex[iActualVertexIdx + 14].x = fFontEx + feather;
				akVertex[iActualVertexIdx + 15].x = fFontEx + feather;

				akVertex[iActualVertexIdx + 12].y = fFontSy + fFontHalfWeight - feather;
				akVertex[iActualVertexIdx + 13].y = fFontEy + fFontHalfWeight + feather;
				akVertex[iActualVertexIdx + 14].y = fFontSy + fFontHalfWeight - feather;
				akVertex[iActualVertexIdx + 15].y = fFontEy + fFontHalfWeight + feather;

				akVertex[iActualVertexIdx + 12].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 13].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 14].z = m_v3Position.z;
				akVertex[iActualVertexIdx + 15].z = m_v3Position.z;

				// 12 - 15 end

				fCurXoutLine += fFontAdvance;

				iActualVertexIdx += 16;
			}

#if defined(WJ_MULTI_TEXTLINE)
			if (HasEnterToken() && m_bEnterTokenVector[i] && !m_bDisableEnterToken)
			{
				ReAdjustedStanXY(i, fStanX, fStanY);
				fCurX = fStanX;
				fCurY += fFontMaxHeight;
				continue;
			}
#endif

			pCurCharInfo = m_pCharInfoVector[i];

			fFontWidth = float(pCurCharInfo->width);
			fFontHeight = float(pCurCharInfo->height);
			fFontMaxHeight = std::max<float>(fFontHeight, pCurCharInfo->height);
			fFontAdvance = float(pCurCharInfo->advance);

			if ((fCurX + fFontWidth) - m_v3Position.x > m_fLimitWidth)
			{
				if (m_isMultiLine)
				{
					fCurX = fStanX;
					fCurY += fFontMaxHeight;
				}
				else
					break;
			}

			if (pClipRect)
			{
				if (fCurY <= pClipRect->top)
				{
					fCurX += fFontAdvance;
					continue;
				}
			}

			fFontSx = fCurX - 0.5f;
			fFontSy = fCurY - 0.5f;
			fFontEx = fFontSx + fFontWidth;
			fFontEy = fFontSy + fFontHeight;

			pFontTexture->SelectTexture(pCurCharInfo->index);
			STATEMANAGER.SetTexture(0, pFontTexture->GetD3DTexture());

			akVertex[iActualVertexIdx].x = fFontSx;
			akVertex[iActualVertexIdx].y = fFontSy;
			akVertex[iActualVertexIdx].z = m_v3Position.z;

			akVertex[iActualVertexIdx].u = pCurCharInfo->left;
			akVertex[iActualVertexIdx].v = pCurCharInfo->top;

			akVertex[iActualVertexIdx + 1].x = fFontSx;
			akVertex[iActualVertexIdx + 1].y = fFontEy;
			akVertex[iActualVertexIdx + 1].z = m_v3Position.z;

			akVertex[iActualVertexIdx + 1].u = pCurCharInfo->left;
			akVertex[iActualVertexIdx + 1].v = pCurCharInfo->bottom;

			akVertex[iActualVertexIdx + 2].x = fFontEx;
			akVertex[iActualVertexIdx + 2].y = fFontSy;
			akVertex[iActualVertexIdx + 2].z = m_v3Position.z;

			akVertex[iActualVertexIdx + 2].u = pCurCharInfo->right;
			akVertex[iActualVertexIdx + 2].v = pCurCharInfo->top;

			akVertex[iActualVertexIdx + 3].x = fFontEx;
			akVertex[iActualVertexIdx + 3].y = fFontEy;
			akVertex[iActualVertexIdx + 3].z = m_v3Position.z;

			akVertex[iActualVertexIdx + 3].u = pCurCharInfo->right;
			akVertex[iActualVertexIdx + 3].v = pCurCharInfo->bottom;

			akVertex[iActualVertexIdx + 0].color = akVertex[iActualVertexIdx + 1].color = akVertex[iActualVertexIdx + 2].color =
				akVertex[iActualVertexIdx + 3].color = m_dwColorInfoVector[i];

			iActualVertexIdx += 4;

			fCurX += fFontAdvance;
		}

		int iSize = iActualVertexIdx;
		int iDrawSize = iSize >= 2 ? (iSize - 2) : 0;

		if (iDrawSize > 0)
		{
			if (CGraphicBase::SetPDTTextLineStream((SPDTVertexRaw*)akVertex, iSize))
				STATEMANAGER.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, iDrawSize);
		}
	}
#else
	{
		const float fFontHalfWeight = 1.0f;

		float fCurX, fCurY;

		float fFontSx, fFontSy, fFontEx, fFontEy;
		float fFontWidth, fFontHeight, fFontMaxHeight;
		float fFontAdvance;

		SVertex akVertex[4];
		akVertex[0].z = akVertex[1].z = akVertex[2].z = akVertex[3].z = m_v3Position.z;

		CGraphicFontTexture::TCharacterInfomation* pCurCharInfo;

		// �׵θ�
		if (m_isOutline)
		{
			fCurX = fStanX;
			fCurY = fStanY;
			fFontMaxHeight = 0.0f;

			CGraphicFontTexture::TPCharacterInfomationVector::iterator i;
			for (i = m_pCharInfoVector.begin(); i != m_pCharInfoVector.end(); ++i)
			{
				pCurCharInfo = *i;

				fFontWidth = float(pCurCharInfo->width);
				fFontHeight = float(pCurCharInfo->height);
				fFontAdvance = float(pCurCharInfo->advance);

				// NOTE : ��Ʈ ��¿� Width ������ �Ӵϴ�. - [levites]
				if ((fCurX + fFontWidth) - m_v3Position.x > m_fLimitWidth)
				{
					if (m_isMultiLine)
						fCurX = fStanX;
					else
						break;
				}

#if defined(WJ_MULTI_TEXTLINE)
				if (m_bEnterTokenVector[i] && !m_bDisableEnterToken)
				{
					fCurX = fStanX;
					fCurY += fFontHeight;

					fCurXoutLine = fStanX;
					fCurYoutline += fFontMaxHeight;

					continue;
				}
#endif

				if (pClipRect)
				{
					if (fCurY <= pClipRect->top)
					{
						fCurX += fFontAdvance;
						continue;
					}
				}

				fFontSx = fCurX - 0.5f;
				fFontSy = fCurY - 0.5f;
				fFontEx = fFontSx + fFontWidth;
				fFontEy = fFontSy + fFontHeight;

				pFontTexture->SelectTexture(pCurCharInfo->index);
				STATEMANAGER.SetTexture(0, pFontTexture->GetD3DTexture());

				akVertex[0].u = akVertex[1].u = pCurCharInfo->left;
				akVertex[2].u = akVertex[3].u = pCurCharInfo->right;
				akVertex[0].v = akVertex[2].v = pCurCharInfo->top;
				akVertex[1].v = akVertex[3].v = pCurCharInfo->bottom;

				akVertex[3].color = akVertex[2].color = akVertex[1].color = akVertex[0].color = m_dwOutlineColor;

				float feather = 0.0f; //m_fFontFeather

				akVertex[0].y = akVertex[2].y = fFontSy;
				akVertex[1].y = akVertex[3].y = fFontEy;

				// ��
				akVertex[0].x = akVertex[1].x = fFontSx - fFontHalfWeight - feather;
				akVertex[2].x = akVertex[3].x = fFontEx - fFontHalfWeight + feather;
				if (CGraphicBase::SetPDTStream((SPDTVertex*)akVertex, 4))
					STATEMANAGER.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

				// ����
				akVertex[0].x = akVertex[1].x = fFontSx + fFontHalfWeight - feather;
				akVertex[2].x = akVertex[3].x = fFontEx + fFontHalfWeight + feather;
				if (CGraphicBase::SetPDTStream((SPDTVertex*)akVertex, 4))
					STATEMANAGER.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

				akVertex[0].x = akVertex[1].x = fFontSx;
				akVertex[2].x = akVertex[3].x = fFontEx;

				// ��
				akVertex[0].y = akVertex[2].y = fFontSy - fFontHalfWeight - feather;
				akVertex[1].y = akVertex[3].y = fFontEy - fFontHalfWeight + feather;
				// 2004.12.16.myevan DrawPrimitiveUP
				if (CGraphicBase::SetPDTStream((SPDTVertex*)akVertex, 4))
					STATEMANAGER.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

				// �Ʒ�
				akVertex[0].y = akVertex[2].y = fFontSy + fFontHalfWeight - feather;
				akVertex[1].y = akVertex[3].y = fFontEy + fFontHalfWeight + feather;
				// 2004.12.16.myevan DrawPrimitiveUP
				if (CGraphicBase::SetPDTStream((SPDTVertex*)akVertex, 4))
					STATEMANAGER.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

				fCurX += fFontAdvance;
			}
		}

		// ���� ��Ʈ
		fCurX = fStanX;
		fCurY = fStanY;
		fFontMaxHeight = 0.0f;

		for (int i = 0; i < m_pCharInfoVector.size(); ++i)
		{
#if defined(WJ_MULTI_TEXTLINE)
			if (m_bEnterTokenVector[i] && !m_bDisableEnterToken)
			{
				fCurX = fStanX;
				fCurY += fFontMaxHeight;
				continue;
			}
#endif

			pCurCharInfo = m_pCharInfoVector[i];

			fFontWidth = float(pCurCharInfo->width);
			fFontHeight = float(pCurCharInfo->height);
			fFontMaxHeight = max(fFontHeight, pCurCharInfo->height);
			fFontAdvance = float(pCurCharInfo->advance);

			// NOTE : ��Ʈ ��¿� Width ������ �Ӵϴ�. - [levites]
			if ((fCurX + fFontWidth) - m_v3Position.x > m_fLimitWidth)
			{
				if (m_isMultiLine)
				{
					fCurX = fStanX;
					fCurY += fFontMaxHeight;
				}
				else
				{
					break;
				}
			}

			if (pClipRect)
			{
				if (fCurY <= pClipRect->top)
				{
					fCurX += fFontAdvance;
					continue;
				}
			}

			fFontSx = fCurX - 0.5f;
			fFontSy = fCurY - 0.5f;
			fFontEx = fFontSx + fFontWidth;
			fFontEy = fFontSy + fFontHeight;

			pFontTexture->SelectTexture(pCurCharInfo->index);
			STATEMANAGER.SetTexture(0, pFontTexture->GetD3DTexture());

			akVertex[0].x = fFontSx;
			akVertex[0].y = fFontSy;
			akVertex[0].u = pCurCharInfo->left;
			akVertex[0].v = pCurCharInfo->top;

			akVertex[1].x = fFontSx;
			akVertex[1].y = fFontEy;
			akVertex[1].u = pCurCharInfo->left;
			akVertex[1].v = pCurCharInfo->bottom;

			akVertex[2].x = fFontEx;
			akVertex[2].y = fFontSy;
			akVertex[2].u = pCurCharInfo->right;
			akVertex[2].v = pCurCharInfo->top;

			akVertex[3].x = fFontEx;
			akVertex[3].y = fFontEy;
			akVertex[3].u = pCurCharInfo->right;
			akVertex[3].v = pCurCharInfo->bottom;

			//m_dwColorInfoVector[i];
			//m_dwTextColor;
			akVertex[0].color = akVertex[1].color = akVertex[2].color = akVertex[3].color = m_dwColorInfoVector[i];

			// 20041216.myevan.DrawPrimitiveUP
			if (CGraphicBase::SetPDTStream((SPDTVertex*)akVertex, 4))
				STATEMANAGER.DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			//STATEMANAGER.DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, akVertex, sizeof(SVertex));

			fCurX += fFontAdvance;
		}
	}
#endif

	if (m_isCursor)
	{
		// Draw Cursor
		float sx, sy, ex, ey;
		TDiffuse diffuse;

		int curpos = CIME::GetCurPos();
		int compend = curpos + CIME::GetCompLen();

		__GetTextPos(curpos, &sx, &sy);

		// If Composition
		if (curpos < compend)
		{
			diffuse = 0x7fffffff;
			__GetTextPos(compend, &ex, &sy);
		}
		else
		{
			diffuse = 0xffffffff;
			ex = sx + 2;
		}

		// FOR_ARABIC_ALIGN
		if (defCodePage == CP_ARABIC)
		{
			sx += m_v3Position.x - m_textWidth;
			ex += m_v3Position.x - m_textWidth;
			sy += m_v3Position.y;
			ey = sy + m_textHeight;
		}
		else
		{
			sx += m_v3Position.x;
			sy += m_v3Position.y;
			ex += m_v3Position.x;
			ey = sy + m_textHeight;
		}

		switch (m_vAlign)
		{
		case VERTICAL_ALIGN_BOTTOM:
			sy -= m_textHeight;
			break;

		case VERTICAL_ALIGN_CENTER:
			sy -= float(m_textHeight) / 2.0f;
			break;
		}
		// ����ȭ ����
		// �����ؽ��ĸ� ����Ѵٸ�... STRIP�� �����ϰ�, �ؽ��İ� ����ǰų� ������ DrawPrimitive�� ȣ����
		// �ִ��� ���ڸ� ���̵�������!

		TPDTVertex vertices[4];
		vertices[0].diffuse = diffuse;
		vertices[1].diffuse = diffuse;
		vertices[2].diffuse = diffuse;
		vertices[3].diffuse = diffuse;
		vertices[0].position = TPosition(sx, sy, 0.0f);
		vertices[1].position = TPosition(ex, sy, 0.0f);
		vertices[2].position = TPosition(sx, ey, 0.0f);
		vertices[3].position = TPosition(ex, ey, 0.0f);

		STATEMANAGER.SetTexture(0, NULL);

		// 2004.11.18.myevan.DrawIndexPrimitiveUP -> DynamicVertexBuffer
		CGraphicBase::SetDefaultIndexBuffer(CGraphicBase::DEFAULT_IB_FILL_RECT);
		if (CGraphicBase::SetPDTStream(vertices, 4))
			STATEMANAGER.DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 4, 0, 2);

		int ulbegin = CIME::GetULBegin();
		int ulend = CIME::GetULEnd();

		if (ulbegin < ulend)
		{
			__GetTextPos(curpos + ulbegin, &sx, &sy);
			__GetTextPos(curpos + ulend, &ex, &sy);

			sx += m_v3Position.x;
			sy += m_v3Position.y + m_textHeight;
			ex += m_v3Position.x;
			ey = sy + 2;

			vertices[0].diffuse = 0xFFFF0000;
			vertices[1].diffuse = 0xFFFF0000;
			vertices[2].diffuse = 0xFFFF0000;
			vertices[3].diffuse = 0xFFFF0000;
			vertices[0].position = TPosition(sx, sy, 0.0f);
			vertices[1].position = TPosition(ex, sy, 0.0f);
			vertices[2].position = TPosition(sx, ey, 0.0f);
			vertices[3].position = TPosition(ex, ey, 0.0f);

			STATEMANAGER.DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, c_FillRectIndices, D3DFMT_INDEX16, vertices, sizeof(TPDTVertex));
		}
	}

	STATEMANAGER.RestoreRenderState(D3DRS_SRCBLEND);
	STATEMANAGER.RestoreRenderState(D3DRS_DESTBLEND);

	STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, dwFogEnable);
	STATEMANAGER.SetRenderState(D3DRS_LIGHTING, dwLighting);

	// �ݰ��� ��ũ ����ִ� �κ�.
	if (m_hyperlinkVector.size() != 0)
	{
		int lx = gs_mx - m_v3Position.x;
		int ly = gs_my - m_v3Position.y;

		// �ƶ��� ��ǥ ��ȣ�� �ٲ��ش�.
		if (GetDefaultCodePage() == CP_ARABIC)
		{
			lx = -lx;
			ly = -ly + m_textHeight;
		}

		if (lx >= 0 && ly >= 0 && lx < m_textWidth && ly < m_textHeight)
		{
			std::vector<SHyperlink>::iterator it = m_hyperlinkVector.begin();

			while (it != m_hyperlinkVector.end())
			{
				SHyperlink& link = *it++;
				if (lx >= link.sx && lx < link.ex)
				{
					gs_hyperlinkText = link.text;
					/*
					OutputDebugStringW(link.text.c_str());
					OutputDebugStringW(L"\n");
					*/
					break;
				}
			}
		}
	}

#ifdef ENABLE_TEXT_IMAGE_LINE
	if (m_imageVector.size() != 0)
	{
		for (std::vector<SImage>::iterator itor = m_imageVector.begin(); itor != m_imageVector.end(); ++itor)
		{
			SImage& rImg = *itor;
			if (rImg.pInstance)
			{
				rImg.pInstance->SetPosition(fStanX + rImg.x, (fStanY + 7.0) - (rImg.pInstance->GetHeight() / 2));
				rImg.pInstance->Render();
			}
		}
	}
#endif
}

#ifdef WJ_MULTI_TEXTLINE
void CGraphicTextInstance::SetLineHeight(int iLineHeight)
{
	m_iLineHeight += m_iLineHeight;
}

int CGraphicTextInstance::GetLineHeight()
{
	return m_iLineHeight;
}

bool CGraphicTextInstance::HasEnterToken()
{
	bool bHasEnterToken = false;

	for (DWORD i = 0; i < m_bEnterTokenVector.size() && !bHasEnterToken; ++i)
		bHasEnterToken = m_bEnterTokenVector[i];

	return bHasEnterToken;
}

void CGraphicTextInstance::ReAdjustedStanXY(int startPosition, float& fStanX, float& fStanY)
{
	startPosition = (startPosition > 0) ? startPosition + 1 : startPosition;

	UINT defCodePage = GetDefaultCodePage();
	WORD textWith = 0;

	for (DWORD i = startPosition; i < m_pCharInfoVector.size(); ++i)
	{
		if (m_bEnterTokenVector[i])
			break;

		textWith += m_pCharInfoVector[i]->advance;
	}

	fStanX = m_v3Position.x;
	fStanY = m_v3Position.y + 1.0f;

	if (defCodePage == CP_ARABIC)
	{
		switch (m_hAlign)
		{
		case HORIZONTAL_ALIGN_LEFT:
			fStanX -= textWith;
			break;

		case HORIZONTAL_ALIGN_CENTER:
			fStanX -= float(textWith / 2);
			break;
		}
	}
	else
	{
		switch (m_hAlign)
		{
		case HORIZONTAL_ALIGN_RIGHT:
			fStanX -= textWith;
			break;

		case HORIZONTAL_ALIGN_CENTER:
			fStanX -= float(textWith / 2);
			break;
		}
	}

	switch (m_vAlign)
	{
	case VERTICAL_ALIGN_BOTTOM:
		fStanY -= m_textHeight;
		break;

	case VERTICAL_ALIGN_CENTER:
		fStanY -= float(m_textHeight) / 2.0f;
		break;
	}
}

void CGraphicTextInstance::DisableEnterToken()
{
	m_bDisableEnterToken = true;
}
#endif

void CGraphicTextInstance::CreateSystem(UINT uCapacity)
{
	ms_kPool.Create(uCapacity);
}

void CGraphicTextInstance::DestroySystem()
{
	ms_kPool.Destroy();
}

CGraphicTextInstance* CGraphicTextInstance::New()
{
	return ms_kPool.Alloc();
}

void CGraphicTextInstance::Delete(CGraphicTextInstance* pkInst)
{
	pkInst->Destroy();
	ms_kPool.Free(pkInst);
}

void CGraphicTextInstance::ShowCursor()
{
	m_isCursor = true;
}

void CGraphicTextInstance::HideCursor()
{
	m_isCursor = false;
}

void CGraphicTextInstance::ShowOutLine()
{
	m_isOutline = true;
}

void CGraphicTextInstance::HideOutLine()
{
	m_isOutline = false;
}

void CGraphicTextInstance::SetColor(DWORD color)
{
	if (m_dwTextColor != color)
	{
		for (int i = 0; i < m_pCharInfoVector.size(); ++i)
			if (m_dwColorInfoVector[i] == m_dwTextColor)
				m_dwColorInfoVector[i] = color;

		m_dwTextColor = color;
	}
}

void CGraphicTextInstance::SetColor(float r, float g, float b, float a)
{
	SetColor(D3DXCOLOR(r, g, b, a));
}

void CGraphicTextInstance::SetOutlineColor(DWORD color)
{
	m_dwOutlineColor = color;
}

void CGraphicTextInstance::SetOutlineColor(float r, float g, float b, float a)
{
	m_dwOutlineColor = D3DXCOLOR(r, g, b, a);
}

void CGraphicTextInstance::SetSecret(bool Value)
{
	m_isSecret = Value;
}

void CGraphicTextInstance::SetOutline(bool Value)
{
	m_isOutline = Value;
}

void CGraphicTextInstance::SetFeather(bool Value)
{
	if (Value)
	{
		m_fFontFeather = c_fFontFeather;
	}
	else
	{
		m_fFontFeather = 0.0f;
	}
}

void CGraphicTextInstance::SetMultiLine(bool Value)
{
	m_isMultiLine = Value;
}

void CGraphicTextInstance::SetHorizonalAlign(int hAlign)
{
	m_hAlign = hAlign;
}

void CGraphicTextInstance::SetVerticalAlign(int vAlign)
{
	m_vAlign = vAlign;
}

void CGraphicTextInstance::SetMax(int iMax)
{
	m_iMax = iMax;
}

void CGraphicTextInstance::SetLimitWidth(float fWidth)
{
	m_fLimitWidth = fWidth;
}

void CGraphicTextInstance::SetValueString(const std::string& c_stValue)
{
	if (0 == m_stText.compare(c_stValue))
		return;

	m_stText = c_stValue;
	m_isUpdate = false;
}

void CGraphicTextInstance::SetValue(const char* c_szText, size_t len)
{
	if (0 == m_stText.compare(c_szText))
		return;

	m_stText = c_szText;
	m_isUpdate = false;
}

void CGraphicTextInstance::SetPosition(float fx, float fy, float fz)
{
	m_v3Position.x = fx;
	m_v3Position.y = fy;
	m_v3Position.z = fz;
}

void CGraphicTextInstance::SetTextPointer(CGraphicText* pText)
{
	m_roText = pText;
}

const std::string& CGraphicTextInstance::GetValueStringReference()
{
	return m_stText;
}

WORD CGraphicTextInstance::GetTextLineCount()
{
	CGraphicFontTexture::TCharacterInfomation* pCurCharInfo;
	CGraphicFontTexture::TPCharacterInfomationVector::iterator itor;

	float fx = 0.0f;
	WORD wLineCount = 1;
	for (itor = m_pCharInfoVector.begin(); itor != m_pCharInfoVector.end(); ++itor)
	{
		pCurCharInfo = *itor;

		float fFontWidth = float(pCurCharInfo->width);
		float fFontAdvance = float(pCurCharInfo->advance);
		//float fFontHeight = float(pCurCharInfo->height);

		// NOTE : ��Ʈ ��¿� Width ������ �Ӵϴ�. - [levites]
		if (fx + fFontWidth > m_fLimitWidth)
		{
			fx = 0.0f;
			++wLineCount;
		}

		fx += fFontAdvance;
	}

#ifdef WJ_MULTI_TEXTLINE
	if (HasEnterToken())
		++wLineCount;
#endif

	return wLineCount;
}

void CGraphicTextInstance::GetTextSize(int* pRetWidth, int* pRetHeight)
{
#ifdef WJ_MULTI_TEXTLINE
	if (HasEnterToken())
	{
		WORD wTextWidth = 0, wMaxTextWidth = 0;
		for (DWORD i = 0; i < m_pCharInfoVector.size(); i++)
		{
			if (m_bEnterTokenVector[i])
			{
				if (wTextWidth >= wMaxTextWidth)
					wMaxTextWidth = wTextWidth;

				wTextWidth = 0;
				continue;
			}
			wTextWidth += m_pCharInfoVector[i]->width;
		}
		*pRetWidth = wMaxTextWidth;
	}
	else
		*pRetWidth = m_textWidth;
	*pRetHeight = m_textHeight;
#else
	* pRetWidth = m_textWidth;
	*pRetHeight = m_textHeight;
#endif
}

int CGraphicTextInstance::PixelPositionToCharacterPosition(int iPixelPosition)
{
	int icurPosition = 0;
	for (int i = 0; i < (int)m_pCharInfoVector.size(); ++i)
	{
		CGraphicFontTexture::TCharacterInfomation* pCurCharInfo = m_pCharInfoVector[i];
		icurPosition += pCurCharInfo->width;

		if (iPixelPosition < icurPosition)
			return i;
	}

	return -1;
}

int CGraphicTextInstance::GetHorizontalAlign()
{
	return m_hAlign;
}

void CGraphicTextInstance::__Initialize()
{
	m_roText = NULL;

	m_hAlign = HORIZONTAL_ALIGN_LEFT;
	m_vAlign = VERTICAL_ALIGN_TOP;

	m_iMax = 0;
	m_fLimitWidth = 1600.0f; // NOTE : �ػ��� �ִ�ġ. �̺��� ��� �� ���� ������? - [levites]

	m_isCursor = false;
	m_isSecret = false;
	m_isMultiLine = false;

	m_isOutline = false;
	m_fFontFeather = c_fFontFeather;

#ifdef WJ_MULTI_TEXTLINE
	m_iLineHeight = 0;
	m_bDisableEnterToken = false;
#endif

	m_isUpdate = false;

	m_textWidth = 0;
	m_textHeight = 0;

	m_v3Position.x = m_v3Position.y = m_v3Position.z = 0.0f;

	m_dwOutlineColor = 0xff000000;
}

void CGraphicTextInstance::Destroy()
{
	m_stText = "";
	m_pCharInfoVector.clear();
	m_dwColorInfoVector.clear();
	m_hyperlinkVector.clear();

#ifdef WJ_MULTI_TEXTLINE
	m_bEnterTokenVector.clear();
	m_bDisableEnterToken = false;
	m_wcEndLine = NULL;
#endif

#ifdef ENABLE_TEXT_IMAGE_LINE
	if (m_imageVector.size() != 0)
	{
		for (std::vector<SImage>::iterator itor = m_imageVector.begin(); itor != m_imageVector.end(); ++itor)
		{
			SImage& rImg = *itor;
			if (rImg.pInstance)
			{
				CGraphicImageInstance::Delete(rImg.pInstance);
				rImg.pInstance = NULL;
			}
		}
	}
	m_imageVector.clear();
#endif

	__Initialize();
}

CGraphicTextInstance::CGraphicTextInstance()
{
	__Initialize();
}

CGraphicTextInstance::~CGraphicTextInstance()
{
	Destroy();
}
