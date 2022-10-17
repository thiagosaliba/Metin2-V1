#ifndef __INC_SHOP_H__
#define __INC_SHOP_H__

enum
{
	SHOP_MAX_DISTANCE = 1000,
};

class CGrid;

/* ---------------------------------------------------------------------------------- */
class CShop
{
public:
	typedef struct shop_item
	{
		DWORD vnum; // ������ ��ȣ
		long price; // ����
#if defined(__CHEQUE_SYSTEM__)
		long price_cheque;
#endif
		WORD count; // ������ ����

		LPITEM pkItem;
		int itemid; // ������ �������̵�

#if defined(__SHOPEX_RENEWAL__)
		long alSockets[ITEM_SOCKET_MAX_NUM];
		TPlayerItemAttribute aAttr[ITEM_ATTRIBUTE_MAX_NUM];
		DWORD price_type;
		DWORD price_vnum;
#endif

		shop_item()
		{
			vnum = 0;
			price = 0;
#if defined(__CHEQUE_SYSTEM__)
			price_cheque = 0;
#endif
			count = 0;
			itemid = 0;
			pkItem = NULL;

#if defined(__SHOPEX_RENEWAL__)
			price_type = 1;
			price_vnum = 0;
			memset(&alSockets, 0, sizeof(alSockets));
			memset(&aAttr, 0, sizeof(aAttr));
#endif
		}
	} SHOP_ITEM;

	CShop();
	virtual ~CShop();

	bool Create(DWORD dwVnum, DWORD dwNPCVnum, TShopItemTable* pItemTable);
	void SetShopItems(TShopItemTable* pItemTable, WORD wItemCount);

	virtual void SetPCShop(LPCHARACTER ch);
	virtual bool IsPCShop() { return m_pkPC ? true : false; }
#if defined(__SHOPEX_RENEWAL__)
	virtual bool IsShopEx() const { return false; };
#endif

	// �Խ�Ʈ �߰�/����
	virtual bool AddGuest(LPCHARACTER ch, DWORD owner_vid, bool bOtherEmpire);
	void RemoveGuest(LPCHARACTER ch);

	// ���� ����
#if defined(__PRIVATESHOP_SEARCH_SYSTEM__)
	const std::vector<SHOP_ITEM>& GetItemVector() const { return m_itemVector; }
	LPCHARACTER GetShopOwner() { return m_pkPC; }
	virtual int Buy(LPCHARACTER ch, BYTE pos, bool bIsShopSearch = false);
#else
	virtual int Buy(LPCHARACTER ch, BYTE pos);
#endif

	// �Խ�Ʈ���� ��Ŷ�� ����
	void BroadcastUpdateItem(BYTE pos);

	// �Ǹ����� �������� ������ �˷��ش�.
	int GetNumberByVnum(DWORD dwVnum);

	// �������� ������ ��ϵǾ� �ִ��� �˷��ش�.
	virtual bool IsSellingItem(DWORD itemID);

	DWORD GetVnum() { return m_dwVnum; }
	DWORD GetNPCVnum() { return m_dwNPCVnum; }
	bool IsSoldOut() const;

protected:
	void Broadcast(const void* data, int bytes);

protected:
	DWORD m_dwVnum;
	DWORD m_dwNPCVnum;

	CGrid* m_pGrid;

	typedef TR1_NS::unordered_map<LPCHARACTER, bool> GuestMapType;
	GuestMapType m_map_guest;
	std::vector<SHOP_ITEM> m_itemVector; // �� �������� ����ϴ� ���ǵ�

	LPCHARACTER m_pkPC;
};

#endif // __INC_SHOP_H__
