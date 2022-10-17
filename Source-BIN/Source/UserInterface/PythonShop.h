#pragma once

#include "Packet.h"

/**
* ���� ó��
*
* 2003-01-16 anoa ���� �Ϸ�
* 2003-12-26 levites ����
*
* 2012-10-29 rtsummit ���ο� ȭ�� ���� �� tab ��� �߰��� ���� shop Ȯ��.
*
**/

typedef enum
{
	SHOP_COIN_TYPE_GOLD, // DEFAULT VALUE
	SHOP_COIN_TYPE_SECONDARY_COIN,
} EShopCoinType;

class CPythonShop : public CSingleton<CPythonShop>
{
public:
	CPythonShop(void);
	virtual ~CPythonShop(void);

	void Clear();

	void SetItemData(DWORD dwIndex, const TShopItemData& c_rShopItemData);
	BOOL GetItemData(DWORD dwIndex, const TShopItemData** c_ppItemData);

	void SetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData& c_rShopItemData);
	BOOL GetItemData(BYTE tabIdx, DWORD dwSlotPos, const TShopItemData** c_ppItemData);

	void SetTabCount(BYTE bTabCount) { m_bTabCount = bTabCount; }
	BYTE GetTabCount() { return m_bTabCount; }

	void SetTabCoinType(BYTE tabIdx, BYTE coinType);
	BYTE GetTabCoinType(BYTE tabIdx);

	void SetTabName(BYTE tabIdx, const char* name);
	const char* GetTabName(BYTE tabIdx);

	void SetNameDialogOpen(bool bOpen) { m_isNameDialogOpen = bOpen; }
	bool GetNameDialogOpen() { return m_isNameDialogOpen; }

	//BOOL GetSlotItemID(DWORD dwSlotPos, DWORD* pdwItemID);

	void Open(BOOL isPrivateShop, BOOL isMainPrivateShop);
	void Close();
	BOOL IsOpen();
	BOOL IsPrivateShop();
	BOOL IsMainPlayerPrivateShop();

	void ClearPrivateShopStock();
#if defined(ENABLE_CHEQUE_SYSTEM)
	void AddPrivateShopItemStock(TItemPos ItemPos, BYTE byDisplayPos, DWORD dwPrice, DWORD dwPriceCheque);
#else
	void AddPrivateShopItemStock(TItemPos ItemPos, BYTE byDisplayPos, DWORD dwPrice);
#endif
	void DelPrivateShopItemStock(TItemPos ItemPos);
	int GetPrivateShopItemPrice(TItemPos ItemPos);
#if defined(ENABLE_CHEQUE_SYSTEM)
	int GetPrivateShopItemPriceCheque(TItemPos ItemPos);
#endif
	void BuildPrivateShop(const char* c_szName);

#if defined(ENABLE_PRIVATESHOP_SEARCH_SYSTEM)
	void SetShopSearchItemData(ShopSearchData* sShopData);
	ShopSearchData* GetShopSearchItemData(DWORD dwIndex);

	void ClearShopSearchData();
	void SortShopSearchData();
	void ShopSearchChangePage(int iPage);

	size_t GetShopSearchResultCount() const { return vShopSearch.size(); }
	int GetShopSearchPage() const { return iShopSearchPage; }
#endif

protected:
	BOOL CheckSlotIndex(DWORD dwIndex);

protected:
	BOOL m_isShoping;
	BOOL m_isPrivateShop;
	BOOL m_isMainPlayerPrivateShop;
	bool m_isNameDialogOpen;

	struct ShopTab
	{
		ShopTab()
		{
			coinType = SHOP_COIN_TYPE_GOLD;
		}
		BYTE coinType;
		std::string name;
		TShopItemData items[SHOP_HOST_ITEM_MAX_NUM];
	};

	BYTE m_bTabCount;
	ShopTab m_aShoptabs[SHOP_TAB_COUNT_MAX];

	typedef std::map<TItemPos, TShopItemTable> TPrivateShopItemStock;
	TPrivateShopItemStock m_PrivateShopItemStock;

#if defined(ENABLE_PRIVATESHOP_SEARCH_SYSTEM)
	std::vector<ShopSearchData*> vShopSearch;
	int iShopSearchPage;
#endif
};
