#pragma once

#include "Packet.h"

/*
* ��ȯ â ����
*/
class CPythonExchange : public CSingleton<CPythonExchange>
{
public:
	enum
	{
		EXCHANGE_ITEM_MAX_NUM = 12,
	};

	typedef struct trade
	{
		char name[CHARACTER_NAME_MAX_LEN + 1];

		DWORD item_vnum[EXCHANGE_ITEM_MAX_NUM];
		WORD item_count[EXCHANGE_ITEM_MAX_NUM];
		DWORD item_metin[EXCHANGE_ITEM_MAX_NUM][ITEM_SOCKET_SLOT_MAX_NUM];
#if defined(ENABLE_APPLY_RANDOM)
		TPlayerItemAttribute item_apply_random[EXCHANGE_ITEM_MAX_NUM][ITEM_APPLY_RANDOM_SLOT_MAX_NUM];
#endif
		TPlayerItemAttribute item_attr[EXCHANGE_ITEM_MAX_NUM][ITEM_ATTRIBUTE_SLOT_MAX_NUM];

		BYTE accept;
		DWORD elk;
		int level;
#if defined(ENABLE_CHEQUE_SYSTEM)
		DWORD cheque;
#endif
#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
		DWORD dwTransmutationVnum[EXCHANGE_ITEM_MAX_NUM];
#endif
	} TExchangeData;

public:
	CPythonExchange();
	virtual ~CPythonExchange();

	void Clear();

	void Start();
	void End();
	bool isTrading();

	// Interface

	void SetSelfName(const char* name);
	void SetTargetName(const char* name);

	char* GetNameFromSelf();
	char* GetNameFromTarget();

	void SetLevelToTarget(int level);
	int GetLevelFromTarget();

	void SetElkToTarget(DWORD elk);
	void SetElkToSelf(DWORD elk);

	DWORD GetElkFromTarget();
	DWORD GetElkFromSelf();

#if defined(ENABLE_CHEQUE_SYSTEM)
	void SetChequeToTarget(DWORD cheque);
	void SetChequeToSelf(DWORD cheque);

	DWORD GetChequeFromTarget();
	DWORD GetChequeFromSelf();
#endif

	void SetItemToTarget(DWORD pos, DWORD vnum, WORD count);
	void SetItemToSelf(DWORD pos, DWORD vnum, WORD count);

	void SetItemMetinSocketToTarget(int pos, int imetinpos, DWORD vnum);
	void SetItemMetinSocketToSelf(int pos, int imetinpos, DWORD vnum);

	void SetItemAttributeToTarget(int pos, int iattrpos, BYTE byType, short sValue);
	void SetItemAttributeToSelf(int pos, int iattrpos, BYTE byType, short sValue);

	void DelItemOfTarget(BYTE pos);
	void DelItemOfSelf(BYTE pos);

	DWORD GetItemVnumFromTarget(BYTE pos);
	DWORD GetItemVnumFromSelf(BYTE pos);

	WORD GetItemCountFromTarget(BYTE pos);
	WORD GetItemCountFromSelf(BYTE pos);

	DWORD GetItemMetinSocketFromTarget(BYTE pos, int iMetinSocketPos);
	DWORD GetItemMetinSocketFromSelf(BYTE pos, int iMetinSocketPos);

#if defined(ENABLE_APPLY_RANDOM)
	void SetItemApplyRandomToTarget(int pos, int iApplyPos, BYTE byType, short sValue);
	void SetItemApplyRandomToSelf(int pos, int iApplyPos, BYTE byType, short sValue);

	void GetItemApplyRandomFromTarget(BYTE pos, int iApplyPos, BYTE* pbyType, short* psValue);
	void GetItemApplyRandomFromSelf(BYTE pos, int iApplyPos, BYTE* pbyType, short* psValue);
#endif

	void GetItemAttributeFromTarget(BYTE pos, int iAttrPos, BYTE* pbyType, short* psValue);
	void GetItemAttributeFromSelf(BYTE pos, int iAttrPos, BYTE* pbyType, short* psValue);

#if defined(ENABLE_CHANGE_LOOK_SYSTEM)
	void SetItemChangeLookVnumToSelf(const int pos, const DWORD blVnum);
	void SetItemChangeLookVnumToTarget(const int pos, const DWORD blVnum);

	DWORD GetItemChangeLookVnumToSelf(const BYTE pos) const;
	DWORD GetItemChangeLookVnumToTarget(const BYTE pos) const;
#endif

	void SetAcceptToTarget(BYTE Accept);
	void SetAcceptToSelf(BYTE Accept);

	bool GetAcceptFromTarget();
	bool GetAcceptFromSelf();

	bool GetElkMode();
	void SetElkMode(bool value);

protected:
	bool m_isTrading;

	bool m_elk_mode; // ��ũ�� Ŭ���ؼ� ��ȯ�������� ���� ������.
	TExchangeData m_self;
	TExchangeData m_victim;
};
