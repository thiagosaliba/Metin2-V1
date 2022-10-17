#include "StdAfx.h"

class CPythonPrivateShopSearch : public CSingleton<CPythonPrivateShopSearch>
{
public:
	struct TSearchItemData : TItemData
	{
		DWORD vid;
		DWORD price;
#if defined(ENABLE_CHEQUE_SYSTEM)
		DWORD price_cheque;
#endif
		BYTE Cell;
	};

	using TItemInstanceVector = std::vector<TSearchItemData>;

public:
	CPythonPrivateShopSearch();
	virtual ~CPythonPrivateShopSearch();

#if defined(ENABLE_CHEQUE_SYSTEM)
	void AddItemData(DWORD vid, DWORD price, DWORD price_cheque, const TSearchItemData& rItemData);
#else
	void AddItemData(DWORD vid, DWORD price, const TSearchItemData& rItemData);
#endif
	void ClearItemData();

	DWORD GetItemDataCount()
	{
		return m_ItemInstanceVector.size();
	}
	DWORD GetItemDataPtr(DWORD index, TSearchItemData** ppInstance);

protected:
	TItemInstanceVector m_ItemInstanceVector;
};
