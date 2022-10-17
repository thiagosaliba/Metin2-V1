#include "StdAfx.h"
#include "InstanceBase.h"
#include "PythonBackground.h"
#include "PythonNonPlayer.h"
#include "PythonPlayer.h"
#include "PythonCharacterManager.h"
#include "AbstractPlayer.h"
#include "AbstractApplication.h"
#include "packet.h"

#include "../EterLib/StateManager.h"
#include "../GameLib/ItemManager.h"
#include "../GameLib/RaceManager.h"

#ifdef ENABLE_GRAPHIC_ON_OFF
#include "PythonSystem.h"
#endif

BOOL HAIR_COLOR_ENABLE = FALSE;
BOOL USE_ARMOR_SPECULAR = FALSE;
BOOL RIDE_HORSE_ENABLE = TRUE;
const float c_fDefaultRotationSpeed = 1200.0f;
const float c_fDefaultHorseRotationSpeed = 600.0f; // 300.0f

bool IsWall(unsigned race)
{
	switch (race)
	{
	case 14201:
	case 14202:
	case 14203:
	case 14204:
		return true;
		break;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////

CInstanceBase::SHORSE::SHORSE()
{
	__Initialize();
}

CInstanceBase::SHORSE::~SHORSE()
{
	assert(m_pkActor == NULL);
}

void CInstanceBase::SHORSE::__Initialize()
{
	m_isMounting = false;
	m_pkActor = NULL;
}

void CInstanceBase::SHORSE::SetAttackSpeed(UINT uAtkSpd)
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor = GetActorRef();
	rkActor.SetAttackSpeed(uAtkSpd / 100.0f);
}

void CInstanceBase::SHORSE::SetMoveSpeed(UINT uMovSpd)
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor = GetActorRef();
	rkActor.SetMoveSpeed(uMovSpd / 100.0f);
}

void CInstanceBase::SHORSE::Create(const TPixelPosition& c_rkPPos, UINT eRace, UINT eHitEffect)
{
	assert(NULL == m_pkActor && "CInstanceBase::SHORSE::Create - ALREADY MOUNT");

	m_pkActor = new CActorInstance;

	CActorInstance& rkActor = GetActorRef();
	rkActor.SetEventHandler(CActorInstance::IEventHandler::GetEmptyPtr());
	if (!rkActor.SetRace(eRace))
	{
		delete m_pkActor;
		m_pkActor = NULL;
		return;
	}

	rkActor.SetShape(0);
	rkActor.SetBattleHitEffect(eHitEffect);
	rkActor.SetAlphaValue(0.0f);
	rkActor.BlendAlphaValue(1.0f, 0.5f);
	rkActor.SetMoveSpeed(1.0f);
	rkActor.SetAttackSpeed(1.0f);
	rkActor.SetMotionMode(CRaceMotionData::MODE_GENERAL);
	rkActor.Stop();
	rkActor.RefreshActorInstance();

	rkActor.SetCurPixelPosition(c_rkPPos);

	m_isMounting = true;
}

void CInstanceBase::SHORSE::Destroy()
{
	if (m_pkActor)
	{
		m_pkActor->Destroy();
		delete m_pkActor;
	}

	__Initialize();
}

CActorInstance& CInstanceBase::SHORSE::GetActorRef()
{
	assert(NULL != m_pkActor && "CInstanceBase::SHORSE::GetActorRef");
	return *m_pkActor;
}

CActorInstance* CInstanceBase::SHORSE::GetActorPtr()
{
	return m_pkActor;
}

UINT CInstanceBase::SHORSE::GetLevel()
{
	if (m_pkActor)
	{
		DWORD mount = m_pkActor->GetRace();
		switch (mount)
		{
		case 20101:
		case 20102:
		case 20103:
			return 1;
		case 20104:
		case 20105:
		case 20106:
			return 2;
		case 20107:
		case 20108:
		case 20109:

		case 20110: // #0000673: [M2EU] ���ο� Ż�� Ÿ�� ���� �ȵ�
		case 20111: // #0000673: [M2EU] ���ο� Ż�� Ÿ�� ���� �ȵ�
		case 20112: // #0000673: [M2EU] ���ο� Ż�� Ÿ�� ���� �ȵ�
		case 20113: // #0000673: [M2EU] ���ο� Ż�� Ÿ�� ���� �ȵ�

		case 20114:
		case 20115:
		case 20116:
		case 20117:
		case 20118:
		case 20120:
		case 20121:
		case 20122:
		case 20123:
		case 20124:
		case 20125:
			return 3;
		case 20119: // �󸶴� �̺�Ʈ�� �渶�� ��ų�Ұ�, ���ݰ����� ����2�� ����
		case 20219: // �ҷ��� �̺�Ʈ�� �渶�� ��ų�Ұ�, ���ݰ����� ����2�� ���� (=�󸶴� �渶 Ŭ��)
		case 20220:
		case 20221:
		case 20222:
			return 2;
		}

		// ����Ʈ Ȯ�� �ý��ۿ� Ư�� ó�� (20201 ~ 20212 �뿪�� ����ϰ� ������� 4���� ������ �ʱ�, �߱�, �����)
		// -- Ż�� ������ Ŭ�󿡼� �����ϰ� ����/��ų ��밡�� ���ε� Ŭ�󿡼� ó���ϴ� �� ��ü�� ������ �ִ� ��.. [hyo]
		{
			// �߱� Ż���� ����2 (���� ����, ��ų �Ұ�)
			if ((20205 <= mount && 20208 >= mount) || // ������ ����������, ������ �����ϼ���
				(20214 == mount) || (20217 == mount) ||
				(20224 == mount) || (20229 == mount))
				return 2;

			// ��� Ż���� ����3 (���� ����, ��ų ����)
			if ((20209 <= mount && 20212 >= mount) || // ����� ����������, ����� �����ϼ���
				(20215 == mount) || (20218 == mount) ||
				(20225 == mount) || (20230 == mount))
				return 3;
		}

		{
			if ((20226 <= mount && 20227 >= mount) ||
				(20231 <= mount && 20252 >= mount) ||
				(20253 <= mount && 20255 >= mount) ||
				(20257 <= mount && 20258 >= mount) ||
				(20259 <= mount && 20260 >= mount) ||
				(20261 <= mount && 20262 >= mount) ||
				(20263 <= mount && 20283 >= mount)
				)
				return 3;
		}
	}

	return 0;
}

bool CInstanceBase::SHORSE::IsNewMount()
{
	if (!m_pkActor)
		return false;

	DWORD mount = m_pkActor->GetRace();

	if ((20205 <= mount && 20208 >= mount) || // ������ ����������, ������ �����ϼ���
		(20214 == mount) || (20217 == mount) ||
		(20224 == mount) || (20229 == mount))
		return true;

	// ��� Ż��
	if ((20209 <= mount && 20212 >= mount) || // ����� ����������, ����� �����ϼ���
		(20215 == mount) || (20218 == mount) ||
		(20225 == mount) || (20230 == mount))
		return true;

	return false;
}

bool CInstanceBase::SHORSE::CanUseSkill()
{
	// ����ų�� ���� ������ 3 �̻��̾�߸� ��.
	if (IsMounting())
		return 2 < GetLevel();

	return true;
}

bool CInstanceBase::SHORSE::CanAttack()
{
	if (IsMounting())
		if (GetLevel() <= 1)
			return false;

	return true;
}

bool CInstanceBase::SHORSE::IsMounting()
{
	return m_isMounting;
}

void CInstanceBase::SHORSE::Deform()
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor = GetActorRef();
	rkActor.INSTANCEBASE_Deform();
}

void CInstanceBase::SHORSE::Render()
{
	if (!IsMounting())
		return;

	CActorInstance& rkActor = GetActorRef();
	rkActor.Render();
}

void CInstanceBase::__AttachHorseSaddle()
{
	if (!IsMountingHorse())
		return;
	m_kHorse.m_pkActor->AttachModelInstance(CRaceData::PART_MAIN, "saddle", m_GraphicThingInstance, CRaceData::PART_MAIN);
}

void CInstanceBase::__DetachHorseSaddle()
{
	if (!IsMountingHorse())
		return;
	m_kHorse.m_pkActor->DetachModelInstance(CRaceData::PART_MAIN, m_GraphicThingInstance, CRaceData::PART_MAIN);
}

//////////////////////////////////////////////////////////////////////////////////////

void CInstanceBase::BlockMovement()
{
	m_GraphicThingInstance.BlockMovement();
}

bool CInstanceBase::IsBlockObject(const CGraphicObjectInstance& c_rkBGObj)
{
	return m_GraphicThingInstance.IsBlockObject(c_rkBGObj);
}

bool CInstanceBase::AvoidObject(const CGraphicObjectInstance& c_rkBGObj)
{
	return m_GraphicThingInstance.AvoidObject(c_rkBGObj);
}

///////////////////////////////////////////////////////////////////////////////////

bool __ArmorVnumToShape(int iVnum, DWORD* pdwShape)
{
	*pdwShape = iVnum;

	/////////////////////////////////////////

	if (0 == iVnum || 1 == iVnum)
		return false;

	if (!USE_ARMOR_SPECULAR)
		return false;

	CItemData* pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(iVnum, &pItemData))
		return false;

	enum
	{
		SHAPE_VALUE_SLOT_INDEX = 3,
	};

	*pdwShape = pItemData->GetValue(SHAPE_VALUE_SLOT_INDEX);

	return true;
}

// 2004.07.05.myevan.�ý�ź�� ���̴� ����
class CActorInstanceBackground : public IBackground
{
public:
	CActorInstanceBackground() {}
	virtual ~CActorInstanceBackground() {}
	bool IsBlock(int x, int y)
	{
		CPythonBackground& rkBG = CPythonBackground::Instance();
		return rkBG.isAttrOn(x, y, CTerrainImpl::ATTRIBUTE_BLOCK);
	}
};

static CActorInstanceBackground gs_kActorInstBG;

bool CInstanceBase::LessRenderOrder(CInstanceBase* pkInst)
{
	int nMainAlpha = (__GetAlphaValue() < 1.0f) ? 1 : 0;
	int nTestAlpha = (pkInst->__GetAlphaValue() < 1.0f) ? 1 : 0;
	if (nMainAlpha < nTestAlpha)
		return true;
	if (nMainAlpha > nTestAlpha)
		return false;

	if (GetRace() < pkInst->GetRace())
		return true;
	if (GetRace() > pkInst->GetRace())
		return false;

	if (GetShape() < pkInst->GetShape())
		return true;

	if (GetShape() > pkInst->GetShape())
		return false;

	UINT uLeftLODLevel = __LessRenderOrder_GetLODLevel();
	UINT uRightLODLevel = pkInst->__LessRenderOrder_GetLODLevel();
	if (uLeftLODLevel < uRightLODLevel)
		return true;
	if (uLeftLODLevel > uRightLODLevel)
		return false;

	if (m_awPart[CRaceData::PART_WEAPON] < pkInst->m_awPart[CRaceData::PART_WEAPON])
		return true;

	return false;
}

UINT CInstanceBase::__LessRenderOrder_GetLODLevel()
{
	CGrannyLODController* pLODCtrl = m_GraphicThingInstance.GetLODControllerPointer(0);
	if (!pLODCtrl)
		return 0;

	return pLODCtrl->GetLODLevel();
}

bool CInstanceBase::__Background_GetWaterHeight(const TPixelPosition& c_rkPPos, float* pfHeight)
{
	long lHeight;
	if (!CPythonBackground::Instance().GetWaterHeight(int(c_rkPPos.x), int(c_rkPPos.y), &lHeight))
		return false;

	*pfHeight = float(lHeight);

	return true;
}

bool CInstanceBase::__Background_IsWaterPixelPosition(const TPixelPosition& c_rkPPos)
{
	return CPythonBackground::Instance().isAttrOn(c_rkPPos.x, c_rkPPos.y, CTerrainImpl::ATTRIBUTE_WATER);
}

const float PC_DUST_RANGE = 2000.0f;
const float NPC_DUST_RANGE = 1000.0f;

DWORD CInstanceBase::ms_dwUpdateCounter = 0;
DWORD CInstanceBase::ms_dwRenderCounter = 0;
DWORD CInstanceBase::ms_dwDeformCounter = 0;

CDynamicPool<CInstanceBase> CInstanceBase::ms_kPool;

bool CInstanceBase::__IsInDustRange()
{
	if (!__IsExistMainInstance())
		return false;

	CInstanceBase* pkInstMain = __GetMainInstancePtr();

	float fDistance = NEW_GetDistanceFromDestInstance(*pkInstMain);

	if (IsPC())
	{
		if (fDistance <= PC_DUST_RANGE)
			return true;
	}

	if (fDistance <= NPC_DUST_RANGE)
		return true;

	return false;
}

void CInstanceBase::__EnableSkipCollision()
{
	if (__IsMainInstance())
	{
		TraceError("CInstanceBase::__EnableSkipCollision - �ڽ��� �浹�˻罺ŵ�� �Ǹ� �ȵȴ�!!");
		return;
	}
	m_GraphicThingInstance.EnableSkipCollision();
}

void CInstanceBase::__DisableSkipCollision()
{
	m_GraphicThingInstance.DisableSkipCollision();
}

DWORD CInstanceBase::__GetShadowMapColor(float x, float y)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	return rkBG.GetShadowMapColor(x, y);
}

float CInstanceBase::__GetBackgroundHeight(float x, float y)
{
	CPythonBackground& rkBG = CPythonBackground::Instance();
	return rkBG.GetHeight(x, y);
}

#ifdef __MOVIE_MODE__
BOOL CInstanceBase::IsMovieMode()
{
	if (IsAffect(AFFECT_INVISIBILITY))
		return true;

	return false;
}
#endif

BOOL CInstanceBase::IsInvisibility()
{
	if (IsAffect(AFFECT_INVISIBILITY) /* || IsAffect(AFFECT_REVIVE_INVISIBILITY) /*|| IsAffect(AFFECT_EUNHYEONG) */)
		return true;

	return false;
}

BOOL CInstanceBase::IsStealth()
{
	if (IsAffect(AFFECT_EUNHYEONG))
		return true;

	return false;
}

BOOL CInstanceBase::IsParalysis()
{
	return m_GraphicThingInstance.IsParalysis();
}

BOOL CInstanceBase::IsGameMaster()
{
	if (m_kAffectFlagContainer.IsSet(AFFECT_YMIR))
		return true;
	return false;
}

BOOL CInstanceBase::IsSameEmpire(CInstanceBase& rkInstDst)
{
	if (0 == rkInstDst.m_dwEmpireID)
		return TRUE;

	if (IsGameMaster())
		return TRUE;

	if (rkInstDst.IsGameMaster())
		return TRUE;

	if (rkInstDst.m_dwEmpireID == m_dwEmpireID)
		return TRUE;

	return FALSE;
}

DWORD CInstanceBase::GetEmpireID()
{
	return m_dwEmpireID;
}

DWORD CInstanceBase::GetGuildID()
{
	return m_dwGuildID;
}

#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
BYTE CInstanceBase::GetGuildLeaderGrade()
{
	return m_bGuildLeaderGrade;
}
#endif

#ifdef WJ_SHOW_MOB_INFO
DWORD CInstanceBase::GetAIFlag()
{
	return m_dwAIFlag;
}
#endif

#ifdef ENABLE_SKILL_COLOR_SYSTEM
DWORD* CInstanceBase::GetSkillColor(DWORD dwSkillIndex)
{
	DWORD dwSkillSlot = dwSkillIndex + 1;
	CPythonSkill::SSkillData* c_pSkillData;
	if (!CPythonSkill::Instance().GetSkillData(dwSkillSlot, &c_pSkillData))
		return 0;

	WORD dwEffectID = c_pSkillData->GradeData[CPythonSkill::SKILL_GRADE_COUNT].wMotionIndex - CRaceMotionData::NAME_SKILL - (1 * 25);
	return m_GraphicThingInstance.GetSkillColorByMotionID(dwEffectID);
}
#endif

int CInstanceBase::GetAlignment()
{
	return m_sAlignment;
}

UINT CInstanceBase::GetAlignmentGrade()
{
	if (m_sAlignment >= 12000)
		return 0;
	else if (m_sAlignment >= 8000)
		return 1;
	else if (m_sAlignment >= 4000)
		return 2;
	else if (m_sAlignment >= 1000)
		return 3;
	else if (m_sAlignment >= 0)
		return 4;
	else if (m_sAlignment > -4000)
		return 5;
	else if (m_sAlignment > -8000)
		return 6;
	else if (m_sAlignment > -12000)
		return 7;

	return 8;
}

int CInstanceBase::GetAlignmentType()
{
	switch (GetAlignmentGrade())
	{
	case 0:
	case 1:
	case 2:
	case 3:
	{
		return ALIGNMENT_TYPE_WHITE;
		break;
	}

	case 5:
	case 6:
	case 7:
	case 8:
	{
		return ALIGNMENT_TYPE_DARK;
		break;
	}
	}

	return ALIGNMENT_TYPE_NORMAL;
}

#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
BYTE CInstanceBase::GetGuildLeaderGradeType()
{
	if (m_bGuildLeaderGrade == 3)
		return 0;
	else if (m_bGuildLeaderGrade == 2)
		return 1;

	return 2;
}
#endif

BYTE CInstanceBase::GetPKMode()
{
	return m_byPKMode;
}

bool CInstanceBase::IsKiller()
{
	return m_isKiller;
}

bool CInstanceBase::IsPartyMember()
{
	return m_isPartyMember;
}

BOOL CInstanceBase::IsInSafe()
{
	const TPixelPosition& c_rkPPosCur = m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	if (CPythonBackground::Instance().isAttrOn(c_rkPPosCur.x, c_rkPPosCur.y, CTerrainImpl::ATTRIBUTE_BANPK))
		return TRUE;

	return FALSE;
}

float CInstanceBase::CalculateDistanceSq3d(const TPixelPosition& c_rkPPosDst)
{
	const TPixelPosition& c_rkPPosSrc = m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
	return SPixelPosition_CalculateDistanceSq3d(c_rkPPosSrc, c_rkPPosDst);
}

void CInstanceBase::OnSelected()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

	__AttachSelectEffect();
}

void CInstanceBase::OnUnselected()
{
	__DetachSelectEffect();
}

void CInstanceBase::OnTargeted()
{
#ifdef __MOVIE_MODE__
	if (!__IsExistMainInstance())
		return;
#endif

	if (IsStoneDoor())
		return;

	if (IsDead())
		return;

	__AttachTargetEffect();
}

void CInstanceBase::OnUntargeted()
{
	__DetachTargetEffect();
}

void CInstanceBase::DestroySystem()
{
	ms_kPool.Clear();
}

void CInstanceBase::CreateSystem(UINT uCapacity)
{
	ms_kPool.Create(uCapacity);

	memset(ms_adwCRCAffectEffect, 0, sizeof(ms_adwCRCAffectEffect));

	ms_fDustGap = 250.0f;
	ms_fHorseDustGap = 500.0f;
}

CInstanceBase* CInstanceBase::New()
{
	return ms_kPool.Alloc();
}

void CInstanceBase::Delete(CInstanceBase* pkInst)
{
	pkInst->Destroy();
	ms_kPool.Free(pkInst);
}

void CInstanceBase::SetMainInstance()
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();

	DWORD dwVID = GetVirtualID();
	rkChrMgr.SetMainInstance(dwVID);

	m_GraphicThingInstance.SetMainInstance();
}

CInstanceBase* CInstanceBase::__GetMainInstancePtr()
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	return rkChrMgr.GetMainInstancePtr();
}

void CInstanceBase::__ClearMainInstance()
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	rkChrMgr.ClearMainInstance();
}

/* ���� �÷��̾� ĳ�������� ����.*/
bool CInstanceBase::__IsMainInstance()
{
	if (this == __GetMainInstancePtr())
		return true;

	return false;
}

bool CInstanceBase::__IsExistMainInstance()
{
	if (__GetMainInstancePtr())
		return true;
	else
		return false;
}

bool CInstanceBase::__MainCanSeeHiddenThing()
{
	return false;
	//CInstanceBase* pInstance = __GetMainInstancePtr();
	//return pInstance->IsAffect(AFFECT_GAMJI);
}

float CInstanceBase::__GetBowRange()
{
	float fRange = 2500.0f - 100.0f;

	if (__IsMainInstance())
	{
		IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();
		fRange += float(rPlayer.GetStatus(POINT_BOW_DISTANCE));
	}

	return fRange;
}

CInstanceBase* CInstanceBase::__FindInstancePtr(DWORD dwVID)
{
	CPythonCharacterManager& rkChrMgr = CPythonCharacterManager::Instance();
	return rkChrMgr.GetInstancePtr(dwVID);
}

bool CInstanceBase::__FindRaceType(DWORD dwRace, BYTE* pbType)
{
	CPythonNonPlayer& rkNonPlayer = CPythonNonPlayer::Instance();
	return rkNonPlayer.GetInstanceType(dwRace, pbType);
}

bool CInstanceBase::Create(const SCreateData& c_rkCreateData)
{
	IAbstractApplication::GetSingleton().SkipRenderBuffering(300);

	SetInstanceType(c_rkCreateData.m_bType);

	if (!SetRace(c_rkCreateData.m_dwRace))
		return false;

#ifdef ENABLE_GENDER_ALIGNMENT
	if (IsPC())
		SetJob(c_rkCreateData.m_bJob);
#endif

	SetVirtualID(c_rkCreateData.m_dwVID);

	if (c_rkCreateData.m_isMain)
		SetMainInstance();

	if (IsGuildWall())
	{
		unsigned center_x;
		unsigned center_y;

		c_rkCreateData.m_kAffectFlags.ConvertToPosition(&center_x, &center_y);

		float center_z = __GetBackgroundHeight(center_x, center_y);
		NEW_SetPixelPosition(TPixelPosition(float(c_rkCreateData.m_lPosX), float(c_rkCreateData.m_lPosY), center_z));
	}
	else
	{
		SCRIPT_SetPixelPosition(float(c_rkCreateData.m_lPosX), float(c_rkCreateData.m_lPosY));
	}

	if (0 != c_rkCreateData.m_dwMountVnum)
		MountHorse(c_rkCreateData.m_dwMountVnum);

	SetArmor(c_rkCreateData.m_dwArmor);

	if (IsPC())
	{
		SetHair(c_rkCreateData.m_dwHair);
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
		SetAcce(c_rkCreateData.m_dwAcce);
#endif
		SetWeapon(c_rkCreateData.m_dwWeapon);
#ifdef ENABLE_QUIVER_SYSTEM
		SetArrow(c_rkCreateData.m_dwArrow);
#endif
		SetLanguage(c_rkCreateData.m_bLanguage);

#ifdef ENABLE_SKILL_COLOR_SYSTEM
		ChangeSkillColor(*c_rkCreateData.m_dwSkillColor);
		memcpy(m_dwSkillColor, *c_rkCreateData.m_dwSkillColor, sizeof(m_dwSkillColor));
#endif
	}

	__Create_SetName(c_rkCreateData);

	m_dwLevel = c_rkCreateData.m_dwLevel;
#if defined(ENABLE_CONQUEROR_LEVEL)
	m_dwConquerorLevel = c_rkCreateData.m_dwConquerorLevel;
#endif
#ifdef WJ_SHOW_MOB_INFO
	m_dwAIFlag = c_rkCreateData.m_dwAIFlag;
#endif
	m_dwGuildID = c_rkCreateData.m_dwGuildID;
	m_dwEmpireID = c_rkCreateData.m_dwEmpireID;
#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
	m_bGuildLeaderGrade = c_rkCreateData.m_bGuildLeaderGrade;
#endif

	SetVirtualNumber(c_rkCreateData.m_dwRace);
	SetRotation(c_rkCreateData.m_fRot);

	SetAlignment(c_rkCreateData.m_sAlignment);
	SetPKMode(c_rkCreateData.m_byPKMode);

#if defined(ENABLE_CONQUEROR_LEVEL)
	if (IsPC() && c_rkCreateData.m_dwConquerorLevel)
		SetConquerorLevelText(c_rkCreateData.m_dwConquerorLevel);
	else
		SetLevelText(c_rkCreateData.m_dwLevel);
#else
	SetLevelText(c_rkCreateData.m_dwLevel);
#endif

	SetMoveSpeed(c_rkCreateData.m_dwMovSpd);
	SetAttackSpeed(c_rkCreateData.m_dwAtkSpd);

	float fScale = 1.0f;
	BYTE bScale = CPythonNonPlayer::Instance().GetMonsterScalePercent(c_rkCreateData.m_dwRace);
	if (bScale != 100)
		fScale = static_cast<float>(bScale / 100.0f);

#ifdef ENABLE_GROWTH_PET_SYSTEM
	if (c_rkCreateData.m_bType == CActorInstance::TYPE_PET)
		fScale = static_cast<float>(BYTE(100 + c_rkCreateData.m_dwLevel) / 100.0f);
#endif

	m_GraphicThingInstance.SetScaleWorld(fScale, fScale, fScale);

	// NOTE : Dress �� �԰� ������ Alpha �� ���� �ʴ´�.
	if (!IsWearingDress())
	{
		// NOTE : �ݵ�� Affect ���� ���ʿ� �־�� ��
		m_GraphicThingInstance.SetAlphaValue(0.0f);
		m_GraphicThingInstance.BlendAlphaValue(1.0f, 0.5f);
	}

	if (!IsGuildWall())
	{
		SetAffectFlagContainer(c_rkCreateData.m_kAffectFlags);
	}

	// NOTE : �ݵ�� Affect ���� �Ŀ� �ؾ� ��
	AttachTextTail();
	RefreshTextTail();

	if (c_rkCreateData.m_dwStateFlags & ADD_CHARACTER_STATE_SPAWN)
	{
		if (IsAffect(AFFECT_SPAWN))
			__AttachEffect(EFFECT_SPAWN_APPEAR);

		if (IsPC())
		{
			Refresh(CRaceMotionData::NAME_WAIT, true);
		}
		else
		{
			Refresh(CRaceMotionData::NAME_SPAWN, false);
		}
	}
	else
	{
		Refresh(CRaceMotionData::NAME_WAIT, true);
	}

	__AttachEmpireEffect(c_rkCreateData.m_dwEmpireID);

	RegisterBoundingSphere();

	if (c_rkCreateData.m_dwStateFlags & ADD_CHARACTER_STATE_DEAD)
		m_GraphicThingInstance.DieEnd();

	SetStateFlags(c_rkCreateData.m_dwStateFlags);

	m_GraphicThingInstance.SetBattleHitEffect(ms_adwCRCAffectEffect[EFFECT_HIT]);

	if (!IsPC())
	{
		DWORD dwBodyColor = CPythonNonPlayer::Instance().GetMonsterColor(c_rkCreateData.m_dwRace);
		if (0 != dwBodyColor)
		{
			SetModulateRenderMode();
			SetAddColor(dwBodyColor);
		}
	}

	__AttachHorseSaddle();

	// ��� �ɺ��� ���� �ӽ� �ڵ�, ���� ��ġ�� ã�� ��
	const int c_iGuildSymbolRace = 14200;
	if (c_iGuildSymbolRace == GetRace())
	{
		std::string strFileName = GetGuildSymbolFileName(m_dwGuildID);
		if (IsFile(strFileName.c_str()))
			m_GraphicThingInstance.ChangeMaterial(strFileName.c_str());
	}

	return true;
}

#ifdef ENABLE_SKILL_COLOR_SYSTEM
void CInstanceBase::ChangeSkillColor(const DWORD* c_dwSkillColor)
{
	unsigned long ulSkillVnumColor[CRaceMotionData::SKILL_NUM][ESkillColorLength::MAX_EFFECT_COUNT];
	memset(ulSkillVnumColor, 0, sizeof(ulSkillVnumColor));

	unsigned long ulSkillColorSlot[ESkillColorLength::MAX_SKILL_COUNT + MAX_BUFF_COUNT][ESkillColorLength::MAX_EFFECT_COUNT];
	memcpy(ulSkillColorSlot, c_dwSkillColor, sizeof(ulSkillColorSlot));

	for (uint16_t byRace = 0; byRace < MAIN_RACE_MAX_NUM; byRace++)
	{
		uint16_t bySkillVnum = 0;
		switch (byRace)
		{
		case MAIN_RACE_WARRIOR_M:
			bySkillVnum = 1;
			break;
		case MAIN_RACE_ASSASSIN_W:
			bySkillVnum = 31;
			break;
		case MAIN_RACE_SURA_M:
			bySkillVnum = 76;
			break;
		case MAIN_RACE_SHAMAN_W:
			bySkillVnum = 111;
			break;
		case MAIN_RACE_WARRIOR_W:
			bySkillVnum = 16;
			break;
		case MAIN_RACE_ASSASSIN_M:
			bySkillVnum = 46;
			break;
		case MAIN_RACE_SURA_W:
			bySkillVnum = 61;
			break;
		case MAIN_RACE_SHAMAN_M:
			bySkillVnum = 91;
			break;
		case MAIN_RACE_WOLFMAN_M:
			bySkillVnum = 170;
			break;
		}
		uint16_t byNextSkillVnum = bySkillVnum;

		for (uint16_t bySkillSlot = 0; bySkillSlot < ESkillColorLength::MAX_SKILL_COUNT; bySkillSlot++)
		{
			if (bySkillSlot > 6)
				continue;

			for (uint16_t byLayer = 0; byLayer < ESkillColorLength::MAX_EFFECT_COUNT; ++byLayer)
			{
				ulSkillVnumColor[byNextSkillVnum][byLayer] = ulSkillColorSlot[byNextSkillVnum - bySkillVnum][byLayer];
			}
			++byNextSkillVnum;
		}
	}

#if defined(ENABLE_CONQUEROR_LEVEL)
	for (uint16_t bySkillVnum = 176 /* SKILL_FINISH */; bySkillVnum <= 183 /* SKILL_ILIPUNGU*/; bySkillVnum++)
	{
		uint16_t byConquerorSkillSlot = ESkillColorLength::MAX_SKILL_COUNT - 2;
		for (uint16_t byLayer = 0; byLayer < ESkillColorLength::MAX_EFFECT_COUNT; ++byLayer)
			ulSkillVnumColor[bySkillVnum][byLayer] = ulSkillColorSlot[byConquerorSkillSlot][byLayer];
	}
#endif

	for (uint16_t byBuffSlot = ESkillColorLength::BUFF_BEGIN; byBuffSlot < ESkillColorLength::MAX_COLOR_SLOTS; byBuffSlot++)
	{
		BYTE byBuffSkillVnum = 0;
		switch (byBuffSlot)
		{
		case BUFF_BEGIN + 0:
			byBuffSkillVnum = 94;
			break;
		case BUFF_BEGIN + 1:
			byBuffSkillVnum = 95;
			break;
		case BUFF_BEGIN + 2:
			byBuffSkillVnum = 96;
			break;
		case BUFF_BEGIN + 3:
			byBuffSkillVnum = 110;
			break;
		case BUFF_BEGIN + 4:
			byBuffSkillVnum = 111;
			break;
		case BUFF_BEGIN + 5:
			byBuffSkillVnum = 175;
			break;
		}

		if (byBuffSkillVnum == 0)
			continue;

		for (uint16_t byLayer = 0; byLayer < ESkillColorLength::MAX_EFFECT_COUNT; ++byLayer)
			ulSkillVnumColor[byBuffSkillVnum][byLayer] = ulSkillColorSlot[byBuffSlot][byLayer];
	}

	m_GraphicThingInstance.ChangeSkillColor(*ulSkillVnumColor);
}
#endif

void CInstanceBase::__Create_SetName(const SCreateData& c_rkCreateData)
{
	if (IsGoto())
	{
		SetNameString("", 0);
		return;
	}

	if (IsWarp())
	{
		__Create_SetWarpName(c_rkCreateData);
		return;
	}

	SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
}

void CInstanceBase::__Create_SetWarpName(const SCreateData& c_rkCreateData)
{
	const char* c_szName;
	if (CPythonNonPlayer::Instance().GetName(c_rkCreateData.m_dwRace, &c_szName))
	{
		std::string strName = c_szName;
		int iFindingPos = strName.find_first_of(" ", 0);
		if (iFindingPos > 0)
		{
			strName.resize(iFindingPos);
		}
		SetNameString(strName.c_str(), strName.length());
	}
	else
	{
		SetNameString(c_rkCreateData.m_stName.c_str(), c_rkCreateData.m_stName.length());
	}
}

void CInstanceBase::SetNameString(const char* c_szName, int len)
{
	m_stName.assign(c_szName, len);
}

bool CInstanceBase::SetRace(DWORD eRace)
{
	m_dwRace = eRace;

	if (!m_GraphicThingInstance.SetRace(eRace))
		return false;

	if (!__FindRaceType(m_dwRace, &m_eRaceType))
		m_eRaceType = CActorInstance::TYPE_PC;

	return true;
}

BOOL CInstanceBase::__IsChangableWeapon(int iWeaponID)
{
	// �巹�� �԰� �������� ���ɿ��� ���� ������ �ʰ�..
	if (IsWearingDress())
	{
		const int c_iBouquets[] =
		{
			50201, // Bouquet for Assassin
			50202, // Bouquet for Shaman
			50203,
			50204,
			0, // #0000545: [M2CN] ���� �巹���� ��� ���� ����
		};

		for (int i = 0; c_iBouquets[i] != 0; ++i)
			if (iWeaponID == c_iBouquets[i])
				return true;

		return false;
	}
	else
		return true;
}

BOOL CInstanceBase::IsWearingDress()
{
	const int c_iWeddingDressShape = 201;
	return c_iWeddingDressShape == m_eShape;
}

BOOL CInstanceBase::IsHoldingPickAxe()
{
	const int c_iPickAxeStart = 29101;
	const int c_iPickAxeEnd = 29110;
	return m_awPart[CRaceData::PART_WEAPON] >= c_iPickAxeStart && m_awPart[CRaceData::PART_WEAPON] <= c_iPickAxeEnd;
}

BOOL CInstanceBase::IsNewMount()
{
	return m_kHorse.IsNewMount();
}

BOOL CInstanceBase::IsMountingHorse()
{
	return m_kHorse.IsMounting();
}

void CInstanceBase::MountHorse(UINT eRace)
{
	m_kHorse.Destroy();
	m_kHorse.Create(m_GraphicThingInstance.NEW_GetCurPixelPositionRef(), eRace, ms_adwCRCAffectEffect[EFFECT_HIT]);

	SetMotionMode(CRaceMotionData::MODE_HORSE);
	SetRotationSpeed(c_fDefaultHorseRotationSpeed);

	m_GraphicThingInstance.MountHorse(m_kHorse.GetActorPtr());
	m_GraphicThingInstance.Stop();
	m_GraphicThingInstance.RefreshActorInstance();
}

void CInstanceBase::DismountHorse()
{
	m_kHorse.Destroy();
}

void CInstanceBase::GetInfo(std::string* pstInfo)
{
	char szInfo[256];
	sprintf(szInfo, "Inst - UC %d, RC %d Pool - %d ",
		ms_dwUpdateCounter,
		ms_dwRenderCounter,
		ms_kPool.GetCapacity()
	);

	pstInfo->append(szInfo);
}

void CInstanceBase::ResetPerformanceCounter()
{
	ms_dwUpdateCounter = 0;
	ms_dwRenderCounter = 0;
	ms_dwDeformCounter = 0;
}

bool CInstanceBase::NEW_IsLastPixelPosition()
{
	return m_GraphicThingInstance.IsPushing();
}

const TPixelPosition& CInstanceBase::NEW_GetLastPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetLastPixelPositionRef();
}

void CInstanceBase::NEW_SetDstPixelPositionZ(FLOAT z)
{
	m_GraphicThingInstance.NEW_SetDstPixelPositionZ(z);
}

void CInstanceBase::NEW_SetDstPixelPosition(const TPixelPosition& c_rkPPosDst)
{
	m_GraphicThingInstance.NEW_SetDstPixelPosition(c_rkPPosDst);
}

void CInstanceBase::NEW_SetSrcPixelPosition(const TPixelPosition& c_rkPPosSrc)
{
	m_GraphicThingInstance.NEW_SetSrcPixelPosition(c_rkPPosSrc);
}

const TPixelPosition& CInstanceBase::NEW_GetCurPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetCurPixelPositionRef();
}

const TPixelPosition& CInstanceBase::NEW_GetDstPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetDstPixelPositionRef();
}

const TPixelPosition& CInstanceBase::NEW_GetSrcPixelPositionRef()
{
	return m_GraphicThingInstance.NEW_GetSrcPixelPositionRef();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CInstanceBase::OnSyncing()
{
	m_GraphicThingInstance.__OnSyncing();
}

void CInstanceBase::OnWaiting()
{
	m_GraphicThingInstance.__OnWaiting();
}

void CInstanceBase::OnMoving()
{
	m_GraphicThingInstance.__OnMoving();
}

#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
void CInstanceBase::ChangeGuild(DWORD dwGuildID, BYTE bGuildLeaderGrade)
#else
void CInstanceBase::ChangeGuild(DWORD dwGuildID)
#endif
{
	m_dwGuildID = dwGuildID;
#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
	m_bGuildLeaderGrade = bGuildLeaderGrade;
#endif

	DetachTextTail();
	AttachTextTail();
	RefreshTextTail();
}

DWORD CInstanceBase::GetPart(CRaceData::EParts part)
{
	assert(part >= 0 && part < CRaceData::PART_MAX_NUM);
	return m_awPart[part];
}

DWORD CInstanceBase::GetShape()
{
	return m_eShape;
}

bool CInstanceBase::CanAct()
{
	return m_GraphicThingInstance.CanAct();
}

bool CInstanceBase::CanMove()
{
	return m_GraphicThingInstance.CanMove();
}

bool CInstanceBase::CanUseSkill()
{
	if (IsPoly())
		return false;

	if (IsWearingDress())
		return false;

	if (IsHoldingPickAxe())
		return false;

	if (!m_kHorse.CanUseSkill())
		return false;

	if (!m_GraphicThingInstance.CanUseSkill())
		return false;

	return true;
}

bool CInstanceBase::CanAttack()
{
	if (!m_kHorse.CanAttack())
		return false;

	if (IsWearingDress())
		return false;

	if (IsHoldingPickAxe())
		return false;

	return m_GraphicThingInstance.CanAttack();
}

bool CInstanceBase::CanFishing()
{
	return m_GraphicThingInstance.CanFishing();
}

BOOL CInstanceBase::IsBowMode()
{
	return m_GraphicThingInstance.IsBowMode();
}

BOOL CInstanceBase::IsHandMode()
{
	return m_GraphicThingInstance.IsHandMode();
}

BOOL CInstanceBase::IsFishingMode()
{
	if (CRaceMotionData::MODE_FISHING == m_GraphicThingInstance.GetMotionMode())
		return true;

	return false;
}

BOOL CInstanceBase::IsFishing()
{
	return m_GraphicThingInstance.IsFishing();
}

BOOL CInstanceBase::IsDead()
{
	return m_GraphicThingInstance.IsDead();
}

BOOL CInstanceBase::IsStun()
{
	return m_GraphicThingInstance.IsStun();
}

BOOL CInstanceBase::IsSleep()
{
	return m_GraphicThingInstance.IsSleep();
}

BOOL CInstanceBase::__IsSyncing()
{
	return m_GraphicThingInstance.__IsSyncing();
}

void CInstanceBase::NEW_SetOwner(DWORD dwVIDOwner)
{
	m_GraphicThingInstance.SetOwner(dwVIDOwner);
}

float CInstanceBase::GetLocalTime()
{
	return m_GraphicThingInstance.GetLocalTime();
}

void CInstanceBase::PushUDPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
}

DWORD ELTimer_GetServerFrameMSec();

void CInstanceBase::PushTCPStateExpanded(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg, UINT uTargetVID)
{
	SCommand kCmdNew;
	kCmdNew.m_kPPosDst = c_rkPPosDst;
	kCmdNew.m_dwChkTime = dwCmdTime + 100;
	kCmdNew.m_dwCmdTime = dwCmdTime;
	kCmdNew.m_fDstRot = fDstRot;
	kCmdNew.m_eFunc = eFunc;
	kCmdNew.m_uArg = uArg;
	kCmdNew.m_uTargetVID = uTargetVID;
	m_kQue_kCmdNew.push_back(kCmdNew);
}

void CInstanceBase::PushTCPState(DWORD dwCmdTime, const TPixelPosition& c_rkPPosDst, float fDstRot, UINT eFunc, UINT uArg)
{
	if (__IsMainInstance())
	{
		//assert(!"CInstanceBase::PushTCPState �÷��̾� �ڽſ��� �̵���Ŷ�� ���� �ȵȴ�!");
		TraceError("CInstanceBase::PushTCPState You can't send move packets to yourself!");
		return;
	}

	int nNetworkGap = ELTimer_GetServerFrameMSec() - dwCmdTime;

	m_nAverageNetworkGap = (m_nAverageNetworkGap * 70 + nNetworkGap * 30) / 100;

	/*
	if (m_dwBaseCmdTime == 0)
	{
		m_dwBaseChkTime = ELTimer_GetFrameMSec()-nNetworkGap;
		m_dwBaseCmdTime = dwCmdTime;

		Tracenf("VID[%d] ��Ʈ���� [%d]", GetVirtualID(), nNetworkGap);
	}
	*/

	//m_dwBaseChkTime - m_dwBaseCmdTime + ELTimer_GetServerMSec();

	SCommand kCmdNew;
	kCmdNew.m_kPPosDst = c_rkPPosDst;
	kCmdNew.m_dwChkTime = dwCmdTime + m_nAverageNetworkGap; //m_dwBaseChkTime + (dwCmdTime - m_dwBaseCmdTime);// + nNetworkGap;
	kCmdNew.m_dwCmdTime = dwCmdTime;
	kCmdNew.m_fDstRot = fDstRot;
	kCmdNew.m_eFunc = eFunc;
	kCmdNew.m_uArg = uArg;
	m_kQue_kCmdNew.push_back(kCmdNew);

	//int nApplyGap = kCmdNew.m_dwChkTime - ELTimer_GetServerFrameMSec();

	//if (nApplyGap < -500 || nApplyGap>500)
	//	Tracenf("VID[%d] NAME[%s] ��Ʈ���� [cur:%d ave:%d] �۵��ð� (%d)", GetVirtualID(), GetNameString(), nNetworkGap, m_nAverageNetworkGap, nApplyGap);
}

/*
CInstanceBase::TStateQueue::iterator CInstanceBase::FindSameState(TStateQueue& rkQuekStt, DWORD dwCmdTime, UINT eFunc, UINT uArg)
{
	TStateQueue::iterator i = rkQuekStt.begin();
	while (rkQuekStt.end() != i)
	{
		SState& rkSttEach = *i;
		if (rkSttEach.m_dwCmdTime == dwCmdTime)
			if (rkSttEach.m_eFunc == eFunc)
				if (rkSttEach.m_uArg == uArg)
					break;
		++i;
	}

	return i;
}
*/

BOOL CInstanceBase::__CanProcessNetworkStatePacket()
{
	if (m_GraphicThingInstance.IsDead())
		return FALSE;
	if (m_GraphicThingInstance.IsKnockDown())
		return FALSE;
	if (m_GraphicThingInstance.IsUsingSkill())
		if (!m_GraphicThingInstance.CanCancelSkill())
			return FALSE;

	return TRUE;
}

BOOL CInstanceBase::__IsEnableTCPProcess(UINT eCurFunc)
{
	if (m_GraphicThingInstance.IsActTargetEmotion())
	{
		return FALSE;
	}

	if (!m_bEnableTCPState)
	{
		if (FUNC_EMOTION != eCurFunc)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CInstanceBase::StateProcess()
{
	while (1)
	{
		if (m_kQue_kCmdNew.empty())
			return;

		DWORD dwDstChkTime = m_kQue_kCmdNew.front().m_dwChkTime;
		DWORD dwCurChkTime = ELTimer_GetServerFrameMSec();

		if (dwCurChkTime < dwDstChkTime)
			return;

		SCommand kCmdTop = m_kQue_kCmdNew.front();
		m_kQue_kCmdNew.pop_front();

		TPixelPosition kPPosDst = kCmdTop.m_kPPosDst;
		//DWORD dwCmdTime = kCmdTop.m_dwCmdTime;
		FLOAT fRotDst = kCmdTop.m_fDstRot;
		UINT eFunc = kCmdTop.m_eFunc;
		UINT uArg = kCmdTop.m_uArg;
		UINT uVID = GetVirtualID();
		UINT uTargetVID = kCmdTop.m_uTargetVID;

		TPixelPosition kPPosCur;
		NEW_GetPixelPosition(&kPPosCur);

		/*
		if (IsPC())
			Tracenf("%d cmd: vid=%d[%s] func=%d arg=%d curPos=(%f, %f) dstPos=(%f, %f) rot=%f (time %d)",
				ELTimer_GetMSec(),
				uVID, m_stName.c_str(), eFunc, uArg,
				kPPosCur.x, kPPosCur.y,
				kPPosDst.x, kPPosDst.y, fRotDst, dwCmdTime - m_dwBaseCmdTime);
		*/

		TPixelPosition kPPosDir = kPPosDst - kPPosCur;
		float fDirLen = (float)sqrt(kPPosDir.x * kPPosDir.x + kPPosDir.y * kPPosDir.y);

		//Tracenf("�Ÿ� %f", fDirLen);

		if (!__CanProcessNetworkStatePacket())
		{
			Lognf(0, "vid=%d ������ �� ���� ���¶� ��ŵ IsDead=%d, IsKnockDown=%d", uVID, m_GraphicThingInstance.IsDead(), m_GraphicThingInstance.IsKnockDown());
			return;
		}

		if (!__IsEnableTCPProcess(eFunc))
		{
			return;
		}

		switch (eFunc)
		{
		case FUNC_WAIT:
		{
			//Tracenf("%s (%f, %f) -> (%f, %f) �����Ÿ� %f", GetNameString(), kPPosCur.x, kPPosCur.y, kPPosDst.x, kPPosDst.y, fDirLen);
			if (fDirLen > 1.0f)
			{
				//NEW_GetSrcPixelPositionRef() = kPPosCur;
				//NEW_GetDstPixelPositionRef() = kPPosDst;
				NEW_SetSrcPixelPosition(kPPosCur);
				NEW_SetDstPixelPosition(kPPosDst);

				__EnableSkipCollision();

				m_fDstRot = fRotDst;
				m_isGoing = TRUE;

				m_kMovAfterFunc.eFunc = FUNC_WAIT;

				if (!IsWalking())
					StartWalking();

				//Tracen("��ǥ����");
			}
			else
			{
				//Tracen("���� ����");

				m_isGoing = FALSE;

				if (!IsWaiting())
					EndWalking();

				SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
				SetAdvancingRotation(fRotDst);
				SetRotation(fRotDst);
			}
			break;
		}

		case FUNC_MOVE:
		{
			//NEW_GetSrcPixelPositionRef() = kPPosCur;
			//NEW_GetDstPixelPositionRef() = kPPosDst;
			NEW_SetSrcPixelPosition(kPPosCur);
			NEW_SetDstPixelPosition(kPPosDst);
			m_fDstRot = fRotDst;
			m_isGoing = TRUE;
			__EnableSkipCollision();
			//m_isSyncMov = TRUE;

			m_kMovAfterFunc.eFunc = FUNC_MOVE;

			if (!IsWalking())
			{
				//Tracen("�Ȱ� ���� �ʾ� �ȱ� ����");
				StartWalking();
			}
			else
			{
				//Tracen("�̹� �ȴ��� ");
			}
			break;
		}

		case FUNC_COMBO:
		{
			if (fDirLen >= 50.0f)
			{
				NEW_SetSrcPixelPosition(kPPosCur);
				NEW_SetDstPixelPosition(kPPosDst);
				m_fDstRot = fRotDst;
				m_isGoing = TRUE;
				__EnableSkipCollision();

				m_kMovAfterFunc.eFunc = FUNC_COMBO;
				m_kMovAfterFunc.uArg = uArg;

				if (!IsWalking())
					StartWalking();
			}
			else
			{
				//Tracen("��� ���� ����");

				m_isGoing = FALSE;

				if (IsWalking())
					EndWalking();

				SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
				RunComboAttack(fRotDst, uArg);
			}
			break;
		}

		case FUNC_ATTACK:
		{
			if (fDirLen >= 50.0f)
			{
				//NEW_GetSrcPixelPositionRef() = kPPosCur;
				//NEW_GetDstPixelPositionRef() = kPPosDst;
				NEW_SetSrcPixelPosition(kPPosCur);
				NEW_SetDstPixelPosition(kPPosDst);
				m_fDstRot = fRotDst;
				m_isGoing = TRUE;
				__EnableSkipCollision();
				//m_isSyncMov = TRUE;

				m_kMovAfterFunc.eFunc = FUNC_ATTACK;

				if (!IsWalking())
					StartWalking();

				//Tracen("�ʹ� �־ �̵� �� ����");
			}
			else
			{
				//Tracen("�븻 ���� ����");

				m_isGoing = FALSE;

				if (IsWalking())
					EndWalking();

				SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
				BlendRotation(fRotDst);

				RunNormalAttack(fRotDst);

				//Tracen("������ ������ ���� ����");
			}
			break;
		}

		case FUNC_MOB_SKILL:
		{
			if (fDirLen >= 50.0f)
			{
				NEW_SetSrcPixelPosition(kPPosCur);
				NEW_SetDstPixelPosition(kPPosDst);
				m_fDstRot = fRotDst;
				m_isGoing = TRUE;
				__EnableSkipCollision();

				m_kMovAfterFunc.eFunc = FUNC_MOB_SKILL;
				m_kMovAfterFunc.uArg = uArg;

				if (!IsWalking())
					StartWalking();
			}
			else
			{
				m_isGoing = FALSE;

				if (IsWalking())
					EndWalking();

				SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
				BlendRotation(fRotDst);

				m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_SPECIAL_1 + uArg);
			}
			break;
		}

		case FUNC_EMOTION:
		{
			if (fDirLen > 100.0f)
			{
				NEW_SetSrcPixelPosition(kPPosCur);
				NEW_SetDstPixelPosition(kPPosDst);
				m_fDstRot = fRotDst;
				m_isGoing = TRUE;

				if (__IsMainInstance())
					__EnableSkipCollision();

				m_kMovAfterFunc.eFunc = FUNC_EMOTION;
				m_kMovAfterFunc.uArg = uArg;
				m_kMovAfterFunc.uArgExpanded = uTargetVID;
				m_kMovAfterFunc.kPosDst = kPPosDst;

				if (!IsWalking())
					StartWalking();
			}
			else
			{
				__ProcessFunctionEmotion(uArg, uTargetVID, kPPosDst);
			}
			break;
		}

		default:
		{
			if (eFunc & FUNC_SKILL)
			{
				if (fDirLen >= 50.0f)
				{
					//NEW_GetSrcPixelPositionRef() = kPPosCur;
					//NEW_GetDstPixelPositionRef() = kPPosDst;
					NEW_SetSrcPixelPosition(kPPosCur);
					NEW_SetDstPixelPosition(kPPosDst);
					m_fDstRot = fRotDst;
					m_isGoing = TRUE;
					//m_isSyncMov = TRUE;
					__EnableSkipCollision();

					m_kMovAfterFunc.eFunc = eFunc;
					m_kMovAfterFunc.uArg = uArg;

					if (!IsWalking())
						StartWalking();

					//Tracen("�ʹ� �־ �̵� �� ����");
				}
				else
				{
					//Tracen("��ų ����");

					m_isGoing = FALSE;

					if (IsWalking())
						EndWalking();

					SCRIPT_SetPixelPosition(kPPosDst.x, kPPosDst.y);
					SetAdvancingRotation(fRotDst);
					SetRotation(fRotDst);

					NEW_UseSkill(0, eFunc & 0x7f, uArg & 0x0f, (uArg >> 4) ? true : false);
					//Tracen("������ ������ ���� ����");
				}
			}
			break;
		}
		}
	}
}

void CInstanceBase::MovementProcess()
{
	TPixelPosition kPPosCur;
	NEW_GetPixelPosition(&kPPosCur);

	// ������ ��ǥ���̹Ƿ� y�� -ȭ�ؼ� ���Ѵ�.

	TPixelPosition kPPosNext;
	{
		const D3DXVECTOR3& c_rkV3Mov = m_GraphicThingInstance.GetMovementVectorRef();

		kPPosNext.x = kPPosCur.x + (+c_rkV3Mov.x);
		kPPosNext.y = kPPosCur.y + (-c_rkV3Mov.y);
		kPPosNext.z = kPPosCur.z + (+c_rkV3Mov.z);
	}

	TPixelPosition kPPosDeltaSC = kPPosCur - NEW_GetSrcPixelPositionRef();
	TPixelPosition kPPosDeltaSN = kPPosNext - NEW_GetSrcPixelPositionRef();
	TPixelPosition kPPosDeltaSD = NEW_GetDstPixelPositionRef() - NEW_GetSrcPixelPositionRef();

	float fCurLen = sqrtf(kPPosDeltaSC.x * kPPosDeltaSC.x + kPPosDeltaSC.y * kPPosDeltaSC.y);
	float fNextLen = sqrtf(kPPosDeltaSN.x * kPPosDeltaSN.x + kPPosDeltaSN.y * kPPosDeltaSN.y);
	float fTotalLen = sqrtf(kPPosDeltaSD.x * kPPosDeltaSD.x + kPPosDeltaSD.y * kPPosDeltaSD.y);
	float fRestLen = fTotalLen - fCurLen;

	if (__IsMainInstance())
	{
		if (m_isGoing && IsWalking())
		{
			float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(NEW_GetSrcPixelPositionRef(), NEW_GetDstPixelPositionRef());

			SetAdvancingRotation(fDstRot);

			if (fRestLen <= 0.0)
			{
				if (IsWalking())
					EndWalking();

				//Tracen("��ǥ ���� ����");

				m_isGoing = FALSE;

				BlockMovement();

				if (FUNC_EMOTION == m_kMovAfterFunc.eFunc)
				{
					DWORD dwMotionNumber = m_kMovAfterFunc.uArg;
					DWORD dwTargetVID = m_kMovAfterFunc.uArgExpanded;
					__ProcessFunctionEmotion(dwMotionNumber, dwTargetVID, m_kMovAfterFunc.kPosDst);
					m_kMovAfterFunc.eFunc = FUNC_WAIT;
					return;
				}
			}
		}
	}
	else
	{
		if (m_isGoing && IsWalking())
		{
			float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(NEW_GetSrcPixelPositionRef(), NEW_GetDstPixelPositionRef());

			SetAdvancingRotation(fDstRot);

			// ���� ���Ͻð� �ʾ� �ʹ� ���� �̵��ߴٸ�..
			if (fRestLen < -100.0f)
			{
				NEW_SetSrcPixelPosition(kPPosCur);

				float fDstRot = NEW_GetAdvancingRotationFromPixelPosition(kPPosCur, NEW_GetDstPixelPositionRef());
				SetAdvancingRotation(fDstRot);
				//Tracenf("VID %d ���� ���⼳�� (%f, %f) %f rest %f", GetVirtualID(), kPPosCur.x, kPPosCur.y, fDstRot, fRestLen);

				// �̵����̶�� �������� ���߰� �Ѵ�
				if (FUNC_MOVE == m_kMovAfterFunc.eFunc)
				{
					m_kMovAfterFunc.eFunc = FUNC_WAIT;
				}
			}
			// �����ߴٸ�...
			else if (fCurLen <= fTotalLen && fTotalLen <= fNextLen)
			{
				if (m_GraphicThingInstance.IsDead() || m_GraphicThingInstance.IsKnockDown())
				{
					__DisableSkipCollision();

					//Tracen("��� ���¶� ���� ��ŵ");

					m_isGoing = FALSE;

					//Tracen("�ൿ �Ҵ� ���¶� ���� ���� ��ŵ");
				}
				else
				{
					switch (m_kMovAfterFunc.eFunc)
					{
					case FUNC_ATTACK:
					{
						if (IsWalking())
							EndWalking();

						__DisableSkipCollision();
						m_isGoing = FALSE;

						BlockMovement();
						SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
						SetAdvancingRotation(m_fDstRot);
						SetRotation(m_fDstRot);

						RunNormalAttack(m_fDstRot);
						break;
					}

					case FUNC_COMBO:
					{
						if (IsWalking())
							EndWalking();

						__DisableSkipCollision();
						m_isGoing = FALSE;

						BlockMovement();
						SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
						RunComboAttack(m_fDstRot, m_kMovAfterFunc.uArg);
						break;
					}

					case FUNC_EMOTION:
					{
						m_isGoing = FALSE;
						m_kMovAfterFunc.eFunc = FUNC_WAIT;
						__DisableSkipCollision();
						BlockMovement();

						DWORD dwMotionNumber = m_kMovAfterFunc.uArg;
						DWORD dwTargetVID = m_kMovAfterFunc.uArgExpanded;
						__ProcessFunctionEmotion(dwMotionNumber, dwTargetVID, m_kMovAfterFunc.kPosDst);
						break;
					}

					case FUNC_MOVE:
					{
						break;
					}

					case FUNC_MOB_SKILL:
					{
						if (IsWalking())
							EndWalking();

						__DisableSkipCollision();
						m_isGoing = FALSE;

						BlockMovement();
						SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
						SetAdvancingRotation(m_fDstRot);
						SetRotation(m_fDstRot);

						m_GraphicThingInstance.InterceptOnceMotion(CRaceMotionData::NAME_SPECIAL_1 + m_kMovAfterFunc.uArg);
						break;
					}

					default:
					{
						if (m_kMovAfterFunc.eFunc & FUNC_SKILL)
						{
							SetAdvancingRotation(m_fDstRot);
							BlendRotation(m_fDstRot);
							NEW_UseSkill(0, m_kMovAfterFunc.eFunc & 0x7f, m_kMovAfterFunc.uArg & 0x0f, (m_kMovAfterFunc.uArg >> 4) ? true : false);
						}
						else
						{
							//Tracenf("VID %d ��ų ���� (%f, %f) rot %f", GetVirtualID(), NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y, m_fDstRot);

							__DisableSkipCollision();
							m_isGoing = FALSE;

							BlockMovement();
							SCRIPT_SetPixelPosition(NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y);
							SetAdvancingRotation(m_fDstRot);
							BlendRotation(m_fDstRot);
							if (!IsWaiting())
							{
								EndWalking();
							}

							//Tracenf("VID %d ���� (%f, %f) rot %f IsWalking %d", GetVirtualID(), NEW_GetDstPixelPositionRef().x, NEW_GetDstPixelPositionRef().y, m_fDstRot, IsWalking());
						}
						break;
					}
					}
				}
			}
		}
	}

	if (IsWalking() || m_GraphicThingInstance.IsUsingMovingSkill())
	{
		float fRotation = m_GraphicThingInstance.GetRotation();
		float fAdvancingRotation = m_GraphicThingInstance.GetAdvancingRotation();
		int iDirection = GetRotatingDirection(fRotation, fAdvancingRotation);

		if (DEGREE_DIRECTION_SAME != m_iRotatingDirection)
		{
			if (DEGREE_DIRECTION_LEFT == iDirection)
			{
				fRotation = fmodf(fRotation + m_fRotSpd * m_GraphicThingInstance.GetSecondElapsed(), 360.0f);
			}
			else if (DEGREE_DIRECTION_RIGHT == iDirection)
			{
				fRotation = fmodf(fRotation - m_fRotSpd * m_GraphicThingInstance.GetSecondElapsed() + 360.0f, 360.0f);
			}

			if (m_iRotatingDirection != GetRotatingDirection(fRotation, fAdvancingRotation))
			{
				m_iRotatingDirection = DEGREE_DIRECTION_SAME;
				fRotation = fAdvancingRotation;
			}

			m_GraphicThingInstance.SetRotation(fRotation);
		}

		if (__IsInDustRange())
		{
			float fDustDistance = NEW_GetDistanceFromDestPixelPosition(m_kPPosDust);
			if (IsMountingHorse())
			{
				if (fDustDistance > ms_fHorseDustGap)
				{
					NEW_GetPixelPosition(&m_kPPosDust);
					__AttachEffect(EFFECT_HORSE_DUST);
				}
			}
			else
			{
				if (fDustDistance > ms_fDustGap)
				{
					NEW_GetPixelPosition(&m_kPPosDust);
					__AttachEffect(EFFECT_DUST);
				}
			}
		}
	}
}

void CInstanceBase::__ProcessFunctionEmotion(DWORD dwMotionNumber, DWORD dwTargetVID, const TPixelPosition& c_rkPosDst)
{
	if (IsWalking())
		EndWalkingWithoutBlending();

	__EnableChangingTCPState();
	SCRIPT_SetPixelPosition(c_rkPosDst.x, c_rkPosDst.y);

	CInstanceBase* pTargetInstance = CPythonCharacterManager::Instance().GetInstancePtr(dwTargetVID);
	if (pTargetInstance)
	{
		pTargetInstance->__EnableChangingTCPState();

		if (pTargetInstance->IsWalking())
			pTargetInstance->EndWalkingWithoutBlending();

		WORD wMotionNumber1 = HIWORD(dwMotionNumber);
		WORD wMotionNumber2 = LOWORD(dwMotionNumber);

		int src_job = RaceToJob(GetRace());
		int dst_job = RaceToJob(pTargetInstance->GetRace());

		NEW_LookAtDestInstance(*pTargetInstance);
		m_GraphicThingInstance.InterceptOnceMotion(wMotionNumber1 + dst_job);
		m_GraphicThingInstance.SetRotation(m_GraphicThingInstance.GetTargetRotation());
		m_GraphicThingInstance.SetAdvancingRotation(m_GraphicThingInstance.GetTargetRotation());

		pTargetInstance->NEW_LookAtDestInstance(*this);
		pTargetInstance->m_GraphicThingInstance.InterceptOnceMotion(wMotionNumber2 + src_job);
		pTargetInstance->m_GraphicThingInstance.SetRotation(pTargetInstance->m_GraphicThingInstance.GetTargetRotation());
		pTargetInstance->m_GraphicThingInstance.SetAdvancingRotation(pTargetInstance->m_GraphicThingInstance.GetTargetRotation());

		if (pTargetInstance->__IsMainInstance())
		{
			IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();
			rPlayer.EndEmotionProcess();
		}
	}

	if (__IsMainInstance())
	{
		IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();
		rPlayer.EndEmotionProcess();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Update & Deform & Render

int g_iAccumulationTime = 0;

void CInstanceBase::Update()
{
	++ms_dwUpdateCounter;

	StateProcess();
	m_GraphicThingInstance.PhysicsProcess();
	m_GraphicThingInstance.RotationProcess();
	m_GraphicThingInstance.ComboProcess();
	m_GraphicThingInstance.AccumulationMovement();

	if (m_GraphicThingInstance.IsMovement())
	{
		TPixelPosition kPPosCur;
		NEW_GetPixelPosition(&kPPosCur);

		DWORD dwCurTime = ELTimer_GetFrameMSec();
		//if (m_dwNextUpdateHeightTime<dwCurTime)
		{
			m_dwNextUpdateHeightTime = dwCurTime;
			kPPosCur.z = __GetBackgroundHeight(kPPosCur.x, kPPosCur.y);
			NEW_SetPixelPosition(kPPosCur);
		}

		// SetMaterialColor
		{
			DWORD dwMtrlColor = __GetShadowMapColor(kPPosCur.x, kPPosCur.y);
			m_GraphicThingInstance.SetMaterialColor(dwMtrlColor);
		}
	}

	m_GraphicThingInstance.UpdateAdvancingPointInstance();

	AttackProcess();
	MovementProcess();

	m_GraphicThingInstance.MotionProcess(IsPC());

#ifdef ENABLE_GRAPHIC_ON_OFF
	if (CPythonSystem::instance().GetEffectLevel() == 4)
	{
		m_GraphicThingInstance.SetDeactiveAllAttachingEffect();
	}
	else if (CPythonSystem::instance().GetEffectLevel() == 3)
	{
		if (!__IsMainInstance())
			m_GraphicThingInstance.SetDeactiveAllAttachingEffect();
		else
			if (!IsInvisibility() && !IsStealth())
				m_GraphicThingInstance.SetActiveAllAttachingEffect();
	}
	else if (CPythonSystem::instance().GetEffectLevel() == 2)
	{
		if (!IsPC())
			m_GraphicThingInstance.SetDeactiveAllAttachingEffect();
		else
			if (!IsInvisibility() && !IsStealth())
				m_GraphicThingInstance.SetActiveAllAttachingEffect();
	}
	else if (CPythonSystem::instance().GetEffectLevel() == 1)
	{
		if (!__IsMainInstance() || !IsEnemy() && !IsNPC())
			m_GraphicThingInstance.SetDeactiveAllAttachingEffect();
		else
			if (!IsInvisibility() && !IsStealth())
				m_GraphicThingInstance.SetActiveAllAttachingEffect();
	}
	else
	{
		if (!IsInvisibility() && !IsStealth())
			m_GraphicThingInstance.SetActiveAllAttachingEffect();
	}

	if (IsPet() || IsPetPay())
	{
		if (CPythonSystem::instance().IsPetStatus() == 1)
		{
			if (!IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, true);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, true);
			}
		}
		else
		{
			if (IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, false);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, false);
			}
		}
	}

	if (IsShop())
	{
		if (CPythonSystem::instance().GetPrivateShopLevel() >= 3)
		{
			if (!IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, true);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, true);
			}
		}
		else
		{
			if (IsAffect(AFFECT_INVISIBILITY))
			{
				__SetAffect(AFFECT_INVISIBILITY, false);
				m_kAffectFlagContainer.Set(AFFECT_INVISIBILITY, false);
			}
		}
	}
#endif

#ifdef ENABLE_EMOTION_HIDE_WEAPON
#ifdef ENABLE_EXPRESSING_EMOTION
	if (m_GraphicThingInstance.GetCurrentMotionIndex() < CRaceMotionData::NAME_CLAP || m_GraphicThingInstance.GetCurrentMotionIndex() > CRaceMotionData::NAME_SELFIE || m_GraphicThingInstance.GetCurrentMotionIndex() == CRaceMotionData::NAME_DIG)
#else
	if (m_GraphicThingInstance.GetCurrentMotionIndex() < CRaceMotionData::NAME_CLAP || m_GraphicThingInstance.GetCurrentMotionIndex() == CRaceMotionData::NAME_DIG)
#endif
	{
		if (m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON) != m_awPart[CRaceData::PART_WEAPON])
		{
			m_GraphicThingInstance.AttachWeapon(m_awPart[CRaceData::PART_WEAPON]);
			CItemData* pItemData;
			if (CItemManager::Instance().GetItemDataPointer(m_awPart[CRaceData::PART_WEAPON], &pItemData))
				__GetRefinedEffect(pItemData);
		}
	}
	else if (m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON))
	{
		m_GraphicThingInstance.AttachWeapon(0);
		__ClearWeaponRefineEffect();
	}
#endif

	if (IsMountingHorse())
	{
		m_kHorse.m_pkActor->HORSE_MotionProcess(FALSE);
	}

	__ComboProcess();

	ProcessDamage();
}

void CInstanceBase::Transform()
{
	if (__IsSyncing())
	{
		//OnSyncing();
	}
	else
	{
		if (IsWalking() || m_GraphicThingInstance.IsUsingMovingSkill())
		{
			const D3DXVECTOR3& c_rv3Movment = m_GraphicThingInstance.GetMovementVectorRef();

			float len = (c_rv3Movment.x * c_rv3Movment.x) + (c_rv3Movment.y * c_rv3Movment.y);
			if (len > 1.0f)
				OnMoving();
			else
				OnWaiting();
		}
	}

	m_GraphicThingInstance.INSTANCEBASE_Transform();
}

void CInstanceBase::Deform()
{
	// 2004.07.17.levites.isShow�� ViewFrustumCheck�� ����
	if (!__CanRender())
		return;

	++ms_dwDeformCounter;

	m_GraphicThingInstance.INSTANCEBASE_Deform();

	m_kHorse.Deform();
}

void CInstanceBase::RenderTrace()
{
	if (!__CanRender())
		return;

	m_GraphicThingInstance.RenderTrace();
}

void CInstanceBase::Render()
{
	// 2004.07.17.levites.isShow�� ViewFrustumCheck�� ����
	if (!__CanRender())
		return;

	++ms_dwRenderCounter;

	m_kHorse.Render();
	m_GraphicThingInstance.Render();

	if (CActorInstance::IsDirLine())
	{
		if (NEW_GetDstPixelPositionRef().x != 0.0f)
		{
			static CScreen s_kScreen;

			STATEMANAGER.SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
			STATEMANAGER.SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			STATEMANAGER.SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			STATEMANAGER.SaveRenderState(D3DRS_ZENABLE, FALSE);
			STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, FALSE);
			STATEMANAGER.SetRenderState(D3DRS_LIGHTING, FALSE);

			TPixelPosition px;
			m_GraphicThingInstance.GetPixelPosition(&px);
			D3DXVECTOR3 kD3DVt3Cur(px.x, px.y, px.z);
			//D3DXVECTOR3 kD3DVt3Cur(NEW_GetSrcPixelPositionRef().x, -NEW_GetSrcPixelPositionRef().y, NEW_GetSrcPixelPositionRef().z);
			D3DXVECTOR3 kD3DVt3Dest(NEW_GetDstPixelPositionRef().x, -NEW_GetDstPixelPositionRef().y, NEW_GetDstPixelPositionRef().z);

			//printf("%s %f\n", GetNameString(), kD3DVt3Cur.y - kD3DVt3Dest.y);
			//float fdx = NEW_GetDstPixelPositionRef().x - NEW_GetSrcPixelPositionRef().x;
			//float fdy = NEW_GetDstPixelPositionRef().y - NEW_GetSrcPixelPositionRef().y;

			s_kScreen.SetDiffuseColor(0.0f, 0.0f, 1.0f);
			s_kScreen.RenderLine3d(kD3DVt3Cur.x, kD3DVt3Cur.y, px.z, kD3DVt3Dest.x, kD3DVt3Dest.y, px.z);
			STATEMANAGER.RestoreRenderState(D3DRS_ZENABLE);
			STATEMANAGER.SetRenderState(D3DRS_FOGENABLE, TRUE);
			STATEMANAGER.SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}
}

void CInstanceBase::RenderToShadowMap()
{
	if (IsDoor())
		return;

	if (IsBuilding())
		return;

	if (!__CanRender())
		return;

	if (!__IsExistMainInstance())
		return;

	CInstanceBase* pkInstMain = __GetMainInstancePtr();

	const float SHADOW_APPLY_DISTANCE = 2500.0f;

	float fDistance = NEW_GetDistanceFromDestInstance(*pkInstMain);
	if (fDistance >= SHADOW_APPLY_DISTANCE)
		return;

	m_GraphicThingInstance.RenderToShadowMap();
}

void CInstanceBase::RenderCollision()
{
	m_GraphicThingInstance.RenderCollisionData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Setting & Getting Data

void CInstanceBase::SetVirtualID(DWORD dwVirtualID)
{
	m_GraphicThingInstance.SetVirtualID(dwVirtualID);
}

void CInstanceBase::SetVirtualNumber(DWORD dwVirtualNumber)
{
	m_dwVirtualNumber = dwVirtualNumber;
}

void CInstanceBase::SetInstanceType(int iInstanceType)
{
	m_GraphicThingInstance.SetActorType(iInstanceType);
}

void CInstanceBase::SetAlignment(short sAlignment)
{
	m_sAlignment = sAlignment;
	RefreshTextTailTitle();
}

void CInstanceBase::SetLevel(DWORD dwLevel)
{
	m_dwLevel = dwLevel;
	RefreshTextTail();
}

void CInstanceBase::SetLevelText(DWORD dwLevel)
{
	m_dwLevel = dwLevel;
	UpdateTextTailLevel(dwLevel);
}

#if defined(ENABLE_CONQUEROR_LEVEL)
void CInstanceBase::SetConquerorLevelText(DWORD dwLevel)
{
	m_dwConquerorLevel = dwLevel;
	UpdateTextTailConquerorLevel(dwLevel);
}
#endif

void CInstanceBase::SetPKMode(BYTE byPKMode)
{
	if (m_byPKMode == byPKMode)
		return;

	m_byPKMode = byPKMode;

	if (__IsMainInstance())
	{
		IAbstractPlayer& rPlayer = IAbstractPlayer::GetSingleton();
		rPlayer.NotifyChangePKMode();
	}
}

void CInstanceBase::SetKiller(bool bFlag)
{
	if (m_isKiller == bFlag)
		return;

	m_isKiller = bFlag;
	RefreshTextTail();
}

void CInstanceBase::SetPartyMemberFlag(bool bFlag)
{
	m_isPartyMember = bFlag;
}

void CInstanceBase::SetStateFlags(DWORD dwStateFlags)
{
	if (dwStateFlags & ADD_CHARACTER_STATE_KILLER)
		SetKiller(TRUE);
	else
		SetKiller(FALSE);

	if (dwStateFlags & ADD_CHARACTER_STATE_PARTY)
		SetPartyMemberFlag(TRUE);
	else
		SetPartyMemberFlag(FALSE);
}

void CInstanceBase::SetComboType(UINT uComboType)
{
	m_GraphicThingInstance.SetComboType(uComboType);
}

const char* CInstanceBase::GetNameString()
{
	return m_stName.c_str();
}

int CInstanceBase::GetLevel()
{
	return m_dwLevel;
}

DWORD CInstanceBase::GetRace()
{
	return m_dwRace;
}

bool CInstanceBase::IsConflictAlignmentInstance(CInstanceBase& rkInstVictim)
{
	if (PK_MODE_PROTECT == rkInstVictim.GetPKMode())
		return false;

	switch (GetAlignmentType())
	{
	case ALIGNMENT_TYPE_NORMAL:
	case ALIGNMENT_TYPE_WHITE:
		if (ALIGNMENT_TYPE_DARK == rkInstVictim.GetAlignmentType())
			return true;
		break;
	case ALIGNMENT_TYPE_DARK:
		if (GetAlignmentType() != rkInstVictim.GetAlignmentType())
			return true;
		break;
	}

	return false;
}

void CInstanceBase::SetDuelMode(DWORD type)
{
	m_dwDuelMode = type;
}

DWORD CInstanceBase::GetDuelMode()
{
	return m_dwDuelMode;
}

bool CInstanceBase::IsAttackableInstance(CInstanceBase& rkInstVictim)
{
	if (__IsMainInstance())
	{
		CPythonPlayer& rkPlayer = CPythonPlayer::Instance();
		if (rkPlayer.IsObserverMode())
			return false;
	}

	if (GetVirtualID() == rkInstVictim.GetVirtualID())
		return false;

	if (IsStone())
	{
		if (rkInstVictim.IsPC())
			return true;
	}
	else if (IsPC())
	{
		if (rkInstVictim.IsStone())
			return true;

		if (rkInstVictim.IsPC())
		{
			if (GetDuelMode())
			{
				switch (GetDuelMode())
				{
				case DUEL_CANNOTATTACK:
					return false;
				case DUEL_START:
					if (__FindDUELKey(GetVirtualID(), rkInstVictim.GetVirtualID()))
						return true;
					else
						return false;
				}
			}
			if (PK_MODE_GUILD == GetPKMode())
				if (GetGuildID() == rkInstVictim.GetGuildID())
					return false;

			if (rkInstVictim.IsKiller())
				if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
					return true;

			if (PK_MODE_PROTECT != GetPKMode())
			{
				if (PK_MODE_FREE == GetPKMode())
				{
					if (PK_MODE_PROTECT != rkInstVictim.GetPKMode())
						if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
							return true;
				}
				if (PK_MODE_GUILD == GetPKMode())
				{
					if (PK_MODE_PROTECT != rkInstVictim.GetPKMode())
						if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
							if (GetGuildID() != rkInstVictim.GetGuildID())
								return true;
				}
			}

			if (IsSameEmpire(rkInstVictim))
			{
				if (IsPVPInstance(rkInstVictim))
					return true;

				if (PK_MODE_REVENGE == GetPKMode())
					if (!IAbstractPlayer::GetSingleton().IsSamePartyMember(GetVirtualID(), rkInstVictim.GetVirtualID()))
						if (IsConflictAlignmentInstance(rkInstVictim))
							return true;
			}
			else
			{
				return true;
			}
		}

		if (rkInstVictim.IsEnemy())
			return true;

		if (rkInstVictim.IsWoodenDoor())
			return true;
	}
	else if (IsEnemy())
	{
		if (rkInstVictim.IsPC())
			return true;

		if (rkInstVictim.IsBuilding())
			return true;

	}
	else if (IsPoly())
	{
		if (rkInstVictim.IsPC())
			return true;

		if (rkInstVictim.IsEnemy())
			return true;
	}
	return false;
}

bool CInstanceBase::IsTargetableInstance(CInstanceBase& rkInstVictim)
{
	return rkInstVictim.CanPickInstance();
}

// 2004. 07. 07. [levites] - ��ų ����� Ÿ���� �ٲ�� ���� �ذ��� ���� �ڵ�
bool CInstanceBase::CanChangeTarget()
{
	return m_GraphicThingInstance.CanChangeTarget();
}

// 2004.07.17.levites.isShow�� ViewFrustumCheck�� ����
bool CInstanceBase::CanPickInstance()
{
	if (!__IsInViewFrustum())
		return false;

	if (IsDoor())
	{
		if (IsDead())
			return false;
	}

	if (IsPC())
	{
		if (IsAffect(AFFECT_EUNHYEONG))
		{
			if (!__MainCanSeeHiddenThing())
				return false;
		}
		if (IsAffect(AFFECT_REVIVE_INVISIBILITY))
			return false;
		if (IsAffect(AFFECT_INVISIBILITY))
			return false;
	}

	if (IsDead())
		return false;

	return true;
}

bool CInstanceBase::CanViewTargetHP(CInstanceBase& rkInstVictim)
{
	if (rkInstVictim.IsStone())
		return true;

	if (rkInstVictim.IsWoodenDoor())
		return true;

	if (rkInstVictim.IsEnemy())
		return true;

#ifdef ENABLE_VIEW_TARGET_PLAYER_HP
	if (rkInstVictim.IsPC())
		return true;
#endif

	return false;
}

BOOL CInstanceBase::IsPoly()
{
	return m_GraphicThingInstance.IsPoly();
}

BOOL CInstanceBase::IsPC()
{
	return m_GraphicThingInstance.IsPC();
}

BOOL CInstanceBase::IsNPC()
{
	return m_GraphicThingInstance.IsNPC();
}

BOOL CInstanceBase::IsEnemy()
{
	return m_GraphicThingInstance.IsEnemy();
}

BOOL CInstanceBase::IsStone()
{
	return m_GraphicThingInstance.IsStone();
}

BOOL CInstanceBase::IsHorse()
{
	return m_GraphicThingInstance.IsHorse();
}

BOOL CInstanceBase::IsPet()
{
	return m_GraphicThingInstance.IsPet();
}

BOOL CInstanceBase::IsPetPay()
{
	return m_GraphicThingInstance.IsPetPay();
}

BOOL CInstanceBase::IsShop()
{
	if (GetRace() >= 30000 && GetRace() <= 30009)
		return true;
	return false;
}

BOOL CInstanceBase::IsGuildWall() //IsBuilding ���ǹ���ü IsGuildWall�� ���庮��(���� ����)
{
	return IsWall(m_dwRace);
}

BOOL CInstanceBase::IsResource()
{
	switch (m_dwVirtualNumber)
	{
	case 20047:
	case 20048:
	case 20049:
	case 20050:
	case 20051:
	case 20052:
	case 20053:
	case 20054:
	case 20055:
	case 20056:
	case 20057:
	case 20058:
	case 20059:
	case 30301:
	case 30302:
	case 30303:
	case 30304:
	case 30305:
	case 30306:
	case 30309:
	case 30310:
	case 30311:
	case 30312:
	case 30313:
		return TRUE;
	}

	return FALSE;
}

BOOL CInstanceBase::IsWarp()
{
	return m_GraphicThingInstance.IsWarp();
}

BOOL CInstanceBase::IsGoto()
{
	return m_GraphicThingInstance.IsGoto();
}

BOOL CInstanceBase::IsObject()
{
	return m_GraphicThingInstance.IsObject();
}

BOOL CInstanceBase::IsBuilding()
{
	return m_GraphicThingInstance.IsBuilding();
}

BOOL CInstanceBase::IsDoor()
{
	return m_GraphicThingInstance.IsDoor();
}

BOOL CInstanceBase::IsWoodenDoor()
{
	if (m_GraphicThingInstance.IsDoor())
	{
		int vnum = GetVirtualNumber();
		if (vnum == 13000) // ������
			return true;
		else if (vnum >= 30111 && vnum <= 30119) // ��͹�
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

BOOL CInstanceBase::IsStoneDoor()
{
	return m_GraphicThingInstance.IsDoor() && 13001 == GetVirtualNumber();
}

BOOL CInstanceBase::IsFlag()
{
	if (GetRace() == 20035)
		return TRUE;
	if (GetRace() == 20036)
		return TRUE;
	if (GetRace() == 20037)
		return TRUE;

	return FALSE;
}

#if defined(ENABLE_EVENT_BANNER_FLAG)
BOOL CInstanceBase::IsBannerFlag()
{
	switch(GetRace())
	{
	case 20127: // Anniversary Banner 1
	case 20128: // Anniversary Banner 2
	case 20129: // Anniversary Banner 3
	case 20130: // Anniversary Banner 4
	case 20131: // Anniversary Banner 5
	case 20132: // Anniversary Banner 6
	case 20133: // Anniversary Banner 7
	case 20134: // Solar Banner
	case 20135: // Christmas Banner
	case 20138: // Ramadan Banner
	case 20142: // Halloween Banner
	case 20143: // Easter Banner
		return TRUE;
	}
	return FALSE;
}
#endif

BOOL CInstanceBase::IsForceVisible()
{
	if (IsAffect(AFFECT_SHOW_ALWAYS))
		return TRUE;

	if (IsObject() || IsBuilding() || IsDoor())
		return TRUE;

	return FALSE;
}

int CInstanceBase::GetInstanceType()
{
	return m_GraphicThingInstance.GetActorType();
}

DWORD CInstanceBase::GetVirtualID()
{
	return m_GraphicThingInstance.GetVirtualID();
}

DWORD CInstanceBase::GetVirtualNumber()
{
	return m_dwVirtualNumber;
}

// 2004.07.17.levites.isShow�� ViewFrustumCheck�� ����
bool CInstanceBase::__IsInViewFrustum()
{
	return m_GraphicThingInstance.isShow();
}

bool CInstanceBase::__CanRender()
{
#if defined(RENDER_TARGET)
	if (IsAlwaysRender())
		return true;
#endif

	if (!__IsInViewFrustum())
		return false;

	if (IsAffect(AFFECT_INVISIBILITY))
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Graphic Control

bool CInstanceBase::IntersectBoundingBox()
{
	float u, v, t;
	return m_GraphicThingInstance.Intersect(&u, &v, &t);
}

bool CInstanceBase::IntersectDefendingSphere()
{
	return m_GraphicThingInstance.IntersectDefendingSphere();
}

float CInstanceBase::GetDistance(CInstanceBase* pkTargetInst)
{
	TPixelPosition TargetPixelPosition;
	pkTargetInst->m_GraphicThingInstance.GetPixelPosition(&TargetPixelPosition);
	return GetDistance(TargetPixelPosition);
}

float CInstanceBase::GetDistance(const TPixelPosition& c_rPixelPosition)
{
	TPixelPosition PixelPosition;
	m_GraphicThingInstance.GetPixelPosition(&PixelPosition);

	float fdx = PixelPosition.x - c_rPixelPosition.x;
	float fdy = PixelPosition.y - c_rPixelPosition.y;

	return sqrtf((fdx * fdx) + (fdy * fdy));
}

float CInstanceBase::GetBaseHeight()
{
	CActorInstance* pkHorse = m_kHorse.GetActorPtr();
	if (!m_kHorse.IsMounting() || !pkHorse)
		return 0.0f;

	DWORD dwHorseVnum = m_kHorse.m_pkActor->GetRace();
	if ((dwHorseVnum >= 20101 && dwHorseVnum <= 20109) ||
		(dwHorseVnum == 20029 || dwHorseVnum == 20030))
		return 100.0f;

	float fRaceHeight = CRaceManager::instance().GetRaceHeight(dwHorseVnum);
	if (fRaceHeight == 0.0f)
		return 100.0f;
	else
		return fRaceHeight;
}

CActorInstance& CInstanceBase::GetGraphicThingInstanceRef()
{
	return m_GraphicThingInstance;
}

CActorInstance* CInstanceBase::GetGraphicThingInstancePtr()
{
	return &m_GraphicThingInstance;
}

void CInstanceBase::RefreshActorInstance()
{
	m_GraphicThingInstance.RefreshActorInstance();
}

void CInstanceBase::Refresh(DWORD dwMotIndex, bool isLoop)
{
	RefreshState(dwMotIndex, isLoop);
}

void CInstanceBase::RestoreRenderMode()
{
	m_GraphicThingInstance.RestoreRenderMode();
}

void CInstanceBase::SetAddRenderMode()
{
	m_GraphicThingInstance.SetAddRenderMode();
}

void CInstanceBase::SetModulateRenderMode()
{
	m_GraphicThingInstance.SetModulateRenderMode();
}

void CInstanceBase::SetRenderMode(int iRenderMode)
{
	m_GraphicThingInstance.SetRenderMode(iRenderMode);
}

void CInstanceBase::SetAddColor(const D3DXCOLOR& c_rColor)
{
	m_GraphicThingInstance.SetAddColor(c_rColor);
}

void CInstanceBase::__SetBlendRenderingMode()
{
	m_GraphicThingInstance.SetBlendRenderMode();
}

void CInstanceBase::__SetAlphaValue(float fAlpha)
{
	m_GraphicThingInstance.SetAlphaValue(fAlpha);
}

float CInstanceBase::__GetAlphaValue()
{
	return m_GraphicThingInstance.GetAlphaValue();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Part

void CInstanceBase::SetHair(DWORD eHair)
{
	if (!HAIR_COLOR_ENABLE)
		return;

	if (IsPC() == false)
		return;

	m_awPart[CRaceData::PART_HAIR] = eHair;
	m_GraphicThingInstance.SetHair(eHair);
}

void CInstanceBase::ChangeHair(DWORD eHair)
{
	if (!HAIR_COLOR_ENABLE)
		return;

	if (IsPC() == false)
		return;

	if (GetPart(CRaceData::PART_HAIR) == eHair)
		return;

	SetHair(eHair);

	//int type = m_GraphicThingInstance.GetMotionMode();

	RefreshState(CRaceMotionData::NAME_WAIT, true);
	//RefreshState(type, true);
}

#if defined(ENABLE_AURA_COSTUME_SYSTEM)
bool CInstanceBase::SetAura(DWORD eAura)
{
	/*
	if (!IsPC() || IsPoly() || __IsShapeAnimalWear())
		return false;

	m_GraphicThingInstance.ChangePart(CRaceData::PART_AURA, eAura);
	if (!eAura)
	{
		if (m_auraRefineEffect)
		{
			__DetachEffect(m_auraRefineEffect);
			m_auraRefineEffect = 0;
		}
		m_adwPart[CRaceData::PART_AURA] = 0;
		return true;
	}

	CItemData* pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(eAura, &pItemData))
	{
		if (m_auraRefineEffect)
		{
			__DetachEffect(m_auraRefineEffect);
			m_auraRefineEffect = 0;
		}
		m_adwPart[CRaceData::PART_AURA] = 0;
		return true;
	}

	BYTE byRace = (BYTE)GetRace();
	BYTE byJob = (BYTE)RaceToJob(byRace);
	BYTE bySex = (BYTE)RaceToSex(byRace);

	D3DXVECTOR3 scalePos = pItemData->GetItemScalePosition(byJob, bySex);
	if (IsMountingHorseOnly() && byJob != NRaceData::JOB_WOLFMAN)
		scalePos.z += 15.0f;

	m_auraRefineEffect = m_GraphicThingInstance.AttachEffectByID(NULL, "Bip01 Spine2", pItemData->GetAuraEffectID(), NULL, 0, FALSE, pItemData->GetItemScale(byJob, bySex).z, &scalePos);
	m_adwPart[CRaceData::PART_AURA] = eAura;
	*/

	return true;
}
#endif

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
bool CInstanceBase::SetAcce(DWORD eAcce)
{
	if (!IsPC() || IsPoly() || __IsShapeAnimalWear())
		return false;

	__ClearAcceRefineEffect();
	float fSpecular = 65.0f;

	eAcce += 85000;
	if (eAcce > 86500)
	{
		eAcce -= 1500;
		fSpecular += 35;

		m_acceRefineEffect = EFFECT_ACCE_BACK;
		__EffectContainer_AttachEffect(m_acceRefineEffect);
	}

	fSpecular /= 100.0f;
	m_awPart[CRaceData::PART_ACCE] = eAcce;

	CItemData* pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(eAcce, &pItemData))
	{
		__ClearAcceRefineEffect();
		m_GraphicThingInstance.SetAcceScale(1.0f, 1.0f, 1.0f);
		m_awPart[CRaceData::PART_ACCE] = 0;
		m_GraphicThingInstance.AttachAcce(NULL, 0.0f);
		return true;
	}

	m_GraphicThingInstance.AttachAcce(pItemData, fSpecular);

	BYTE bRace = (BYTE)GetRace();
	BYTE bJob = (BYTE)RaceToJob(bRace);
	BYTE bSex = (BYTE)RaceToSex(bRace);

	m_GraphicThingInstance.SetAcceScale(pItemData->GetItemScaleVector(bJob, bSex), bRace);
	m_awPart[CRaceData::PART_ACCE] = eAcce;

	return true;
}

void CInstanceBase::ChangeAcce(DWORD eAcce)
{
	if (!IsPC())
		return;

	SetAcce(eAcce);
}

void CInstanceBase::__ClearAcceRefineEffect()
{
	if (!m_acceRefineEffect)
		return;

	__EffectContainer_DetachEffect(m_acceRefineEffect);
	m_acceRefineEffect = 0;
}
#endif

void CInstanceBase::SetArmor(DWORD dwArmor)
{
	DWORD dwShape;
	if (__ArmorVnumToShape(dwArmor, &dwShape))
	{
		CItemData* pItemData;
		if (CItemManager::Instance().GetItemDataPointer(dwArmor, &pItemData))
		{
			float fSpecularPower = pItemData->GetSpecularPowerf();
			SetShape(dwShape, fSpecularPower);
			__GetRefinedEffect(pItemData);
			return;
		}
		else
			__ClearArmorRefineEffect();
	}

	SetShape(dwArmor);
}

void CInstanceBase::SetShape(DWORD eShape, float fSpecular)
{
	if (IsPoly())
	{
		m_GraphicThingInstance.SetShape(0);
	}
	else
	{
		m_GraphicThingInstance.SetShape(eShape, fSpecular);
	}

	m_eShape = eShape;
}

DWORD CInstanceBase::GetWeaponType()
{
	DWORD dwWeapon = GetPart(CRaceData::PART_WEAPON);
	CItemData* pItemData;
	if (!CItemManager::Instance().GetItemDataPointer(dwWeapon, &pItemData))
		return CItemData::WEAPON_NONE;

	return pItemData->GetWeaponType();
}

/*
void CInstanceBase::SetParts(const WORD * c_pParts)
{
	if (IsPoly())
		return;

	if (__IsShapeAnimalWear())
		return;

	UINT eWeapon=c_pParts[CRaceData::PART_WEAPON];

	if (__IsChangableWeapon(eWeapon) == false)
			eWeapon = 0;

	if (eWeapon != m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON))
	{
		m_GraphicThingInstance.AttachPart(CRaceData::PART_MAIN, CRaceData::PART_WEAPON, eWeapon);
		m_awPart[CRaceData::PART_WEAPON] = eWeapon;
	}

	__AttachHorseSaddle();
}
*/

void CInstanceBase::__ClearWeaponRefineEffect()
{
	if (m_swordRefineEffectRight)
	{
		__DetachEffect(m_swordRefineEffectRight);
		m_swordRefineEffectRight = 0;
	}

	if (m_swordRefineEffectLeft)
	{
		__DetachEffect(m_swordRefineEffectLeft);
		m_swordRefineEffectLeft = 0;
	}
}

void CInstanceBase::__ClearArmorRefineEffect()
{
	if (m_armorRefineEffect)
	{
		__DetachEffect(m_armorRefineEffect);
		m_armorRefineEffect = 0;
	}
}

UINT CInstanceBase::__GetRefinedEffect(CItemData* pItem)
{
	DWORD refine = max(pItem->GetRefine() + pItem->GetSocketCount(), CItemData::ITEM_SOCKET_MAX_NUM) - CItemData::ITEM_SOCKET_MAX_NUM;

	switch (pItem->GetType())
	{
	case CItemData::ITEM_TYPE_COSTUME:
	{
		switch (pItem->GetSubType())
		{
		case CItemData::COSTUME_WEAPON:
			__ClearWeaponRefineEffect();
			break;
		}
		break;
	}

	case CItemData::ITEM_TYPE_WEAPON:
	{
		__ClearWeaponRefineEffect();

		if (refine >= 7) // ���� ���õ� 7 �̻� ����Ʈ�� �ֽ��ϴ�.
		{
			switch (pItem->GetSubType())
			{
			case CItemData::WEAPON_DAGGER:
				m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7 + refine - 7;
				m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7_LEFT + refine - 7;
				break;

			case CItemData::WEAPON_FAN:
				m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_FANBELL_REFINED7 + refine - 7;
				break;

			case CItemData::WEAPON_BELL:
				m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED7 + refine - 7;
				break;

			case CItemData::WEAPON_BOW:
				m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_BOW_REFINED7 + refine - 7;
				break;

			case CItemData::WEAPON_CLAW:
				m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_CLAW_REFINED7 + refine - 7;
				m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_CLAW_REFINED7_LEFT + refine - 7;
				break;

			default:
				m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SWORD_REFINED7 + refine - 7;
			} // end switch
		}
		else
		{
			DWORD vnum = pItem->GetIndex();

			switch (pItem->GetSubType())
			{
			case CItemData::WEAPON_DAGGER:
			{
				if (vnum >= 1200 && vnum <= 1205 ||
					vnum >= 1220 && vnum <= 1225)
				{
					m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED10;
					m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED10_LEFT;
				}
			} break;

			case CItemData::WEAPON_FAN:
			{
				if (vnum >= 7320 && vnum <= 7325 ||
					vnum >= 7340 && vnum <= 7345)
				{
					m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_FANBELL_REFINED10;
				}
			} break;

			case CItemData::WEAPON_BELL:
			{
				if (vnum >= 5180 && vnum <= 5185 ||
					vnum >= 5210 && vnum <= 5215)
				{
					m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SMALLSWORD_REFINED10;
				}
			} break;

			case CItemData::WEAPON_BOW:
			{
				if (vnum >= 2220 && vnum <= 2225 ||
					vnum >= 2240 && vnum <= 2245)
				{
					m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_BOW_REFINED10;
				}
			} break;

			case CItemData::WEAPON_CLAW:
			{
				if (vnum >= 6140 && vnum <= 6145 ||
					vnum >= 6160 && vnum <= 6165)
				{
					m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_CLAW_REFINED10;
					m_swordRefineEffectLeft = EFFECT_REFINED + EFFECT_CLAW_REFINED10_LEFT;
				}
			} break;

			default:
			{
				if (vnum >= 330 && vnum <= 335 ||
					vnum >= 350 && vnum <= 355 ||
					vnum >= 370 && vnum <= 375 ||
					vnum >= 390 && vnum <= 395 ||
					vnum >= 3240 && vnum <= 3245 ||
					vnum >= 3260 && vnum <= 3265)
				{
					m_swordRefineEffectRight = EFFECT_REFINED + EFFECT_SWORD_REFINED10;
				}
			}
			} // end switch
		}

		if (m_swordRefineEffectRight)
			m_swordRefineEffectRight = __AttachEffect(m_swordRefineEffectRight);

		if (m_swordRefineEffectLeft)
			m_swordRefineEffectLeft = __AttachEffect(m_swordRefineEffectLeft);
	} break;

	case CItemData::ITEM_TYPE_ARMOR:
	{
		__ClearArmorRefineEffect();

		// ���� Ưȭ ����Ʈ
		if (pItem->GetSubType() == CItemData::ARMOR_BODY)
		{
			DWORD vnum = pItem->GetIndex();

			if (vnum >= 12010 && vnum <= 12049 ||
				vnum >= 21080 && vnum <= 21089)
			{
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL);
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL2);
			}

			if (vnum >= 20760 && vnum <= 20809 ||
				vnum >= 20810 && vnum <= 20859 ||
				vnum >= 20860 && vnum <= 20909 ||
				vnum >= 20910 && vnum <= 20959)
			{
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL3);
			}

			if (vnum >= 12050 && vnum <= 12099)
			{
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_SPECIAL4);
			}

			// 7th
			if (vnum > 21219 && vnum <= 21225 ||
				vnum > 21239 && vnum <= 21245 ||
				vnum > 21259 && vnum <= 21265 ||
				vnum > 21279 && vnum <= 21285 ||
				vnum > 21299 && vnum <= 21305)
			{
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_REFINED10);
			}

			// Serpent Armour
			if (vnum > 21319 && vnum <= 21325 ||
				vnum > 21339 && vnum <= 21345 ||
				vnum > 21359 && vnum <= 21365 ||
				vnum > 21379 && vnum <= 21385 ||
				vnum > 21399 && vnum <= 21405)
			{
				__AttachEffect(EFFECT_REFINED + EFFECT_BODYARMOR_REFINED10);
			}
		}

		if (refine < 7) //���� ���õ� 7 �̻� ����Ʈ�� �ֽ��ϴ�.
			return 0;

		if (pItem->GetSubType() == CItemData::ARMOR_BODY)
		{
			m_armorRefineEffect = EFFECT_REFINED + EFFECT_BODYARMOR_REFINED7 + refine - 7;
			__AttachEffect(m_armorRefineEffect);

			/*
			// Multiple refined effect
			for (int iCount = 0; iCount < 9; iCount++)
				__AttachEffect(m_armorRefineEffect);
			*/
		}
	} break;
	} // end switch

	return 0;
}

bool CInstanceBase::SetWeapon(DWORD eWeapon)
{
	if (IsPoly())
		return false;

	if (__IsShapeAnimalWear())
		return false;

	if (__IsChangableWeapon(eWeapon) == false)
		eWeapon = 0;

	m_GraphicThingInstance.AttachWeapon(eWeapon);
	m_awPart[CRaceData::PART_WEAPON] = eWeapon;

	// Weapon Effect
	CItemData* pItemData;
	if (CItemManager::Instance().GetItemDataPointer(eWeapon, &pItemData))
		__GetRefinedEffect(pItemData);
	else
		__ClearWeaponRefineEffect();

	return true;
}

#ifdef ENABLE_QUIVER_SYSTEM
bool CInstanceBase::SetArrow(DWORD eArrow)
{
	if (IsPoly())
		return false;
	if (__IsShapeAnimalWear())
		return false;

	if (eArrow)
	{
		CItemData* pItemData;
		if (!CItemManager::Instance().GetItemDataPointer(eArrow, &pItemData))
			return false;

		if (pItemData->GetType() == CItemData::ITEM_TYPE_WEAPON)
		{
			if (pItemData->GetSubType() == CItemData::WEAPON_ARROW)
			{
				m_GraphicThingInstance.SetQuiverEquipped(false);
				m_GraphicThingInstance.SetQuiverEffectID(0);
				return true;
			}

			if (pItemData->GetSubType() == CItemData::WEAPON_QUIVER)
			{
				m_GraphicThingInstance.SetQuiverEquipped(true);
				m_GraphicThingInstance.SetQuiverEffectID(pItemData->GetValue(0));
				return true;
			}
		}
	}

	m_GraphicThingInstance.SetQuiverEquipped(false);
	m_GraphicThingInstance.SetQuiverEffectID(0);
	return false;
}
#endif

void CInstanceBase::ChangeWeapon(DWORD eWeapon)
{
	if (eWeapon == m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON))
		return;

	if (SetWeapon(eWeapon))
		RefreshState(CRaceMotionData::NAME_WAIT, true);
}

bool CInstanceBase::ChangeArmor(DWORD dwArmor)
{
	DWORD eShape;
	__ArmorVnumToShape(dwArmor, &eShape);

	if (GetShape() == eShape)
		return false;

	CAffectFlagContainer kAffectFlagContainer;
	kAffectFlagContainer.CopyInstance(m_kAffectFlagContainer);

	DWORD dwVID = GetVirtualID();
	DWORD dwRace = GetRace();
	DWORD eHair = GetPart(CRaceData::PART_HAIR);
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	DWORD dwAcce = GetPart(CRaceData::PART_ACCE);
#endif
	DWORD eWeapon = GetPart(CRaceData::PART_WEAPON);
	float fRot = GetRotation();
	float fAdvRot = GetAdvancingRotation();

	if (IsWalking())
		EndWalking();

	// 2004.07.25.myevan.����Ʈ �� �ٴ� ����
	//////////////////////////////////////////////////////
	__ClearAffects();
	//////////////////////////////////////////////////////

	if (!SetRace(dwRace))
	{
		TraceError("CPythonCharacterManager::ChangeArmor - SetRace VID[%d] Race[%d] ERROR", dwVID, dwRace);
		return false;
	}

	SetArmor(dwArmor);
	SetHair(eHair);
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	SetAcce(dwAcce);
#endif
	SetWeapon(eWeapon);

	SetRotation(fRot);
	SetAdvancingRotation(fAdvRot);

	__AttachHorseSaddle();

	RefreshState(CRaceMotionData::NAME_WAIT, TRUE);

	// 2004.07.25.myevan.����Ʈ �� �ٴ� ����
	/////////////////////////////////////////////////
	SetAffectFlagContainer(kAffectFlagContainer);
	/////////////////////////////////////////////////

	CActorInstance::IEventHandler& rkEventHandler = GetEventHandlerRef();
	rkEventHandler.OnChangeShape();

	return true;
}

bool CInstanceBase::__IsShapeAnimalWear()
{
	if (100 == GetShape() ||
		101 == GetShape() ||
		102 == GetShape() ||
		103 == GetShape())
		return true;

	return false;
}

DWORD CInstanceBase::__GetRaceType()
{
	return m_eRaceType;
}

void CInstanceBase::RefreshState(DWORD dwMotIndex, bool isLoop)
{
	DWORD dwPartItemID = m_GraphicThingInstance.GetPartItemID(CRaceData::PART_WEAPON);

	BYTE byItemType = 0xff;
	BYTE bySubType = 0xff;

	CItemManager& rkItemMgr = CItemManager::Instance();
	CItemData* pItemData;

	if (rkItemMgr.GetItemDataPointer(dwPartItemID, &pItemData))
	{
		byItemType = pItemData->GetType();
		bySubType = pItemData->GetWeaponType();
	}

	if (IsPoly())
	{
		SetMotionMode(CRaceMotionData::MODE_GENERAL);
	}
	else if (IsWearingDress())
	{
		SetMotionMode(CRaceMotionData::MODE_WEDDING_DRESS);
	}
	else if (IsHoldingPickAxe())
	{
		if (m_kHorse.IsMounting())
		{
			SetMotionMode(CRaceMotionData::MODE_HORSE);
		}
		else
		{
			SetMotionMode(CRaceMotionData::MODE_GENERAL);
		}
	}
	else if (CItemData::ITEM_TYPE_ROD == byItemType)
	{
		if (m_kHorse.IsMounting())
		{
			SetMotionMode(CRaceMotionData::MODE_HORSE);
		}
		else
		{
			SetMotionMode(CRaceMotionData::MODE_FISHING);
		}
	}
	else if (m_kHorse.IsMounting())
	{
		switch (bySubType)
		{
		case CItemData::WEAPON_SWORD:
			SetMotionMode(CRaceMotionData::MODE_HORSE_ONEHAND_SWORD);
			break;

		case CItemData::WEAPON_TWO_HANDED:
			SetMotionMode(CRaceMotionData::MODE_HORSE_TWOHAND_SWORD); // Only Warrior
			break;

		case CItemData::WEAPON_DAGGER:
			SetMotionMode(CRaceMotionData::MODE_HORSE_DUALHAND_SWORD); // Only Assassin
			break;

		case CItemData::WEAPON_FAN:
			SetMotionMode(CRaceMotionData::MODE_HORSE_FAN); // Only Shaman
			break;

		case CItemData::WEAPON_BELL:
			SetMotionMode(CRaceMotionData::MODE_HORSE_BELL); // Only Shaman
			break;

		case CItemData::WEAPON_BOW:
			SetMotionMode(CRaceMotionData::MODE_HORSE_BOW); // Only Shaman
			break;

		case CItemData::WEAPON_CLAW:
			SetMotionMode(CRaceMotionData::MODE_HORSE_CLAW); // Only Wolfman
			break;

		default:
			SetMotionMode(CRaceMotionData::MODE_HORSE);
			break;
		}
	}
	else
	{
		switch (bySubType)
		{
		case CItemData::WEAPON_SWORD:
			SetMotionMode(CRaceMotionData::MODE_ONEHAND_SWORD);
			break;

		case CItemData::WEAPON_TWO_HANDED:
			SetMotionMode(CRaceMotionData::MODE_TWOHAND_SWORD); // Only Warrior
			break;

		case CItemData::WEAPON_DAGGER:
			SetMotionMode(CRaceMotionData::MODE_DUALHAND_SWORD); // Only Assassin
			break;

		case CItemData::WEAPON_BOW:
			SetMotionMode(CRaceMotionData::MODE_BOW); // Only Assassin
			break;

		case CItemData::WEAPON_FAN:
			SetMotionMode(CRaceMotionData::MODE_FAN); // Only Shaman
			break;

		case CItemData::WEAPON_BELL:
			SetMotionMode(CRaceMotionData::MODE_BELL); // Only Shaman
			break;

		case CItemData::WEAPON_CLAW:
			SetMotionMode(CRaceMotionData::MODE_CLAW); // Only Wolfman
			break;

		case CItemData::WEAPON_ARROW:
		default:
			SetMotionMode(CRaceMotionData::MODE_GENERAL);
			break;
		}
	}

	if (isLoop)
		m_GraphicThingInstance.InterceptLoopMotion(dwMotIndex);
	else
		m_GraphicThingInstance.InterceptOnceMotion(dwMotIndex);

	RefreshActorInstance();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Device

void CInstanceBase::RegisterBoundingSphere()
{
	// Stone �� ��� DeforomNoSkin �� �ϸ�
	// �����ϴ� �ִϸ��̼� ���� ��� �ִϸ��̼���
	// �ٿ�� �ڽ��� ������ ���� �ø��� ����� �̷������ �ʴ´�.
	if (!IsStone())
	{
		m_GraphicThingInstance.DeformNoSkin();
	}

	m_GraphicThingInstance.RegisterBoundingSphere();
}

bool CInstanceBase::CreateDeviceObjects()
{
	return m_GraphicThingInstance.CreateDeviceObjects();
}

void CInstanceBase::DestroyDeviceObjects()
{
	m_GraphicThingInstance.DestroyDeviceObjects();
}

void CInstanceBase::Destroy()
{
	DetachTextTail();

	DismountHorse();

	m_kQue_kCmdNew.clear();

	__EffectContainer_Destroy();
	__StoneSmoke_Destroy();

	if (__IsMainInstance())
		__ClearMainInstance();

	m_GraphicThingInstance.Destroy();

	__Initialize();
}

void CInstanceBase::__InitializeRotationSpeed()
{
	SetRotationSpeed(c_fDefaultRotationSpeed);
}

void CInstanceBase::__Warrior_Initialize()
{
	m_kWarrior.m_dwGeomgyeongEffect = 0;
}

void CInstanceBase::__Initialize()
{
	__Warrior_Initialize();
	__StoneSmoke_Inialize();
	__EffectContainer_Initialize();
	__InitializeRotationSpeed();

	SetEventHandler(CActorInstance::IEventHandler::GetEmptyPtr());

	m_kAffectFlagContainer.Clear();

	m_dwLevel = 0;
#if defined(ENABLE_CONQUEROR_LEVEL)
	m_dwConquerorLevel = 0; // fix refresh after user
#endif
#ifdef WJ_SHOW_MOB_INFO
	m_dwAIFlag = 0;
#endif
	m_dwGuildID = 0;
#ifdef ENABLE_GUILD_LEADER_GRADE_NAME
	m_bGuildLeaderGrade = 0;
#endif
	m_dwEmpireID = 0;

	m_eType = 0;
	m_eRaceType = 0;
	m_eShape = 0;
	m_dwRace = 0;
#ifdef ENABLE_GENDER_ALIGNMENT
	m_bJob = 0;
#endif
	m_dwVirtualNumber = 0;

	m_dwBaseCmdTime = 0;
	m_dwBaseChkTime = 0;
	m_dwSkipTime = 0;

	m_GraphicThingInstance.Initialize();

	m_dwAdvActorVID = 0;
	m_dwLastDmgActorVID = 0;

	m_nAverageNetworkGap = 0;
	m_dwNextUpdateHeightTime = 0;

	// Moving by keyboard
	m_iRotatingDirection = DEGREE_DIRECTION_SAME;

	// Moving by mouse
	m_isTextTail = FALSE;
	m_isGoing = FALSE;
	NEW_SetSrcPixelPosition(TPixelPosition(0, 0, 0));
	NEW_SetDstPixelPosition(TPixelPosition(0, 0, 0));

	m_kPPosDust = TPixelPosition(0, 0, 0);

	m_kQue_kCmdNew.clear();

	m_dwLastComboIndex = 0;

	m_swordRefineEffectRight = 0;
	m_swordRefineEffectLeft = 0;
	m_armorRefineEffect = 0;
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	m_acceRefineEffect = 0;
#endif

	m_sAlignment = 0;
	m_byPKMode = 0;
	m_isKiller = false;
	m_isPartyMember = false;

	m_bEnableTCPState = TRUE;

	m_stName = "";

	memset(m_awPart, 0, sizeof(m_awPart));
	memset(m_adwCRCAffectEffect, 0, sizeof(m_adwCRCAffectEffect));
	//memset(m_adwCRCEmoticonEffect, 0, sizeof(m_adwCRCEmoticonEffect));
	memset(&m_kMovAfterFunc, 0, sizeof(m_kMovAfterFunc));

	m_bDamageEffectType = false;
	m_dwDuelMode = DUEL_NONE;
	m_dwEmoticonTime = 0;
#if defined(RENDER_TARGET)
	m_IsAlwaysRender = false;
#endif
}

CInstanceBase::CInstanceBase()
{
	__Initialize();
}

CInstanceBase::~CInstanceBase()
{
	Destroy();
}

#if defined(RENDER_TARGET)
bool CInstanceBase::IsAlwaysRender()
{
	return m_IsAlwaysRender;
}

void CInstanceBase::SetAlwaysRender(bool val)
{
	m_IsAlwaysRender = val;
}
#endif

void CInstanceBase::GetBoundBox(D3DXVECTOR3* vtMin, D3DXVECTOR3* vtMax)
{
	m_GraphicThingInstance.GetBoundBox(vtMin, vtMax);
}
