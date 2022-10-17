#ifndef __INC_SHOP_SECONDARY_COIN_H__
#define __INC_SHOP_SECONDARY_COIN_H__

#include "typedef.h"
#include "shop.h"

struct SShopTable;
typedef struct SShopTableEx : SShopTable
{
	std::string name;
	EShopCoinType coinType;
} TShopTableEx;

class CGroupNode;

// Ȯ�� shop.
// ������ ȭ��� �� �� �ְ�, �������� ���� �ǿ� ���� ��ġ�� �� �ִ�.
// ��, pc ���� �������� ����.
// Ŭ��� ����� ���� ���� pos 45 ������ ����.
// ���� ���� m_itemVector�� ������� �ʴ´�.

typedef std::vector <TShopTableEx> ShopTableExVector;

class CShopEx : public CShop
{
public:
	bool Create(DWORD dwVnum, DWORD dwNPCVnum);
	bool AddShopTable(TShopTableEx& shopTable);

	virtual bool AddGuest(LPCHARACTER ch, DWORD owner_vid, bool bOtherEmpire);
	virtual void SetPCShop(LPCHARACTER ch) { return; }
	virtual bool IsPCShop() { return false; }
#if defined(__SHOPEX_RENEWAL__)
	virtual bool IsShopEx() const { return true; };
#endif
#if defined(__PRIVATESHOP_SEARCH_SYSTEM__)
	virtual int Buy(LPCHARACTER ch, BYTE pos, bool bIsShopSearch = false);
#else
	virtual int Buy(LPCHARACTER ch, BYTE pos);
#endif
	virtual bool IsSellingItem(DWORD itemID) { return false; }

	size_t GetTabCount() { return m_vec_shopTabs.size(); }

private:
	ShopTableExVector m_vec_shopTabs;
};
typedef CShopEx* LPSHOPEX;

#endif // __INC_SHOP_SECONDARY_COIN_H__
