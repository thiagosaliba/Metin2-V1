#pragma once

#include "../EterBase/FileLoader.h"
#include "../EterBase/MappedFile.h"
#include "../EterLib/Util.h"

class CGroupNode
{
public:
	class CGroupNodeRow
	{
	public:
		CGroupNodeRow(CGroupNode* pGroupNode, CTokenVector& vec_values);
		virtual ~CGroupNodeRow();

		template <typename T>
		bool GetValue(const std::string& stColKey, OUT T& value) const;
		template <typename T>
		bool GetValue(int idx, OUT T& value) const;

		int GetSize() const;

	private:
		CGroupNode* m_pOwnerGroupNode;
		CTokenVector m_vec_values;
	};

public:
	CGroupNode();
	virtual ~CGroupNode();

	bool Load(const char* c_szFileName);
	const char* GetFileName();

	DWORD GetChildNodeCount();
	bool SetChildNode(const char* c_szKey, CGroupNode* pChildNode);
	CGroupNode* GetChildNode(const std::string& c_rstrKey) const;
	std::string GetNodeName() const;

	bool IsToken(const std::string& c_rstrKey) const;

	int GetRowCount();

	template <typename T>
	bool GetValue(size_t i, const std::string& c_rstrColKey, T& tValue) const; // n��°(map�� ����ִ� ������ ��, txt�� �����ʹ� ���� ����) row�� Ư�� �÷��� ���� ��ȯ�ϴ� �Լ�.
	// �������̱� ������, ���Ǹ� ���� �Լ�.

	template <typename T>
	bool GetValue(const std::string& c_rstrRowKey, const std::string& c_rstrColKey, T& tValue) const;
	template <typename T>
	bool GetValue(const std::string& c_rstrRowKey, int index, T& tValue) const;

	bool GetRow(const std::string& c_rstrKey, OUT const CGroupNodeRow** ppRow) const;
	// �����, idx�� txt�� ������ ������ ���� ����.
	bool GetRow(int idx, OUT const CGroupNodeRow** ppRow) const;
	bool GetGroupRow(const std::string& stGroupName, const std::string& stRow, OUT const CGroupNode::CGroupNodeRow** ppRow) const;

	template <typename T>
	bool GetGroupValue(const std::string& stGroupName, const std::string& stRow, int iCol, OUT T& tValue) const;
	template <typename T>
	bool GetGroupValue(const std::string& stGroupName, const std::string& stRow, const std::string& stCol, OUT T& tValue) const;

	int	GetColumnIndexFromName(const std::string& stName) const;

private:
	typedef std::map <std::string, CGroupNode*> TMapGroup;
	typedef std::map <std::string, CGroupNode::CGroupNodeRow> TMapRow;
	TMapGroup m_mapChildNodes;
	std::string strGroupName;

	CMapNameToIndex m_map_columnNameToIndex;
	TMapRow m_map_rows;
	friend class CGroupTextParseTreeLoader;

};

class CGroupTextParseTreeLoader
{
public:
	CGroupTextParseTreeLoader();
	virtual ~CGroupTextParseTreeLoader();

	bool Load(const char* c_szFileName/*, DWORD dwSize, LPCVOID *pData*/);
	const char* GetFileName();

	CGroupNode* GetGroup(const char* c_szGroupName);

private:
	bool LoadGroup(CGroupNode* pGroupNode);

	CGroupNode* m_pRootGroupNode;
	std::string m_strFileName;
	DWORD m_dwcurLineIndex;

	CMemoryTextFileLoader m_fileLoader;
};

template <typename T>
bool from_string(OUT T& t, IN const std::string& s)
{
	std::istringstream iss(s);
	return !(iss >> t).fail();
}

template <>
inline bool from_string <BYTE>(OUT BYTE& t, IN const std::string& s)
{
	std::istringstream iss(s);
	int temp;
	bool b = !(iss >> temp).fail();
	t = (BYTE)temp;
	return b;
}

template <typename T>
bool CGroupNode::GetValue(size_t i, const std::string& c_rstrColKey, T& tValue) const
{
	if (i > m_map_rows.size())
		return false;

	TMapRow::const_iterator row_it = m_map_rows.begin();
	std::advance(row_it, i);

	CMapNameToIndex::const_iterator col_idx_it = m_map_columnNameToIndex.find(c_rstrColKey);
	if (m_map_columnNameToIndex.end() == col_idx_it)
	{
		return false;
	}

	int index = col_idx_it->second;
	if (row_it->second.GetSize() <= index)
	{
		return false;
	}

	return row_it->second.GetValue(index, tValue);
}

template <typename T>
bool CGroupNode::GetValue(const std::string& c_rstrRowKey, const std::string& c_rstrColKey, T& tValue) const
{
	TMapRow::const_iterator row_it = m_map_rows.find(c_rstrRowKey);
	if (m_map_rows.end() == row_it)
	{
		return false;
	}

	CMapNameToIndex::const_iterator col_idx_it = m_map_columnNameToIndex.find(c_rstrColKey);
	if (m_map_columnNameToIndex.end() == col_idx_it)
	{
		return false;
	}

	int index = col_idx_it->second;
	if (row_it->second.GetSize() <= index)
	{
		return false;
	}

	return row_it->second.GetValue(index, tValue);
}

template <typename T>
bool CGroupNode::GetValue(const std::string& c_rstrRowKey, int index, T& tValue) const
{
	TMapRow::const_iterator row_it = m_map_rows.find(c_rstrRowKey);
	if (m_map_rows.end() == row_it)
	{
		return false;
	}

	if (row_it->second.GetSize() <= index)
	{
		return false;
	}
	return row_it->second.GetValue(index, tValue);
}

template <typename T>
bool CGroupNode::GetGroupValue(const std::string& stGroupName, const std::string& stRow, int iCol, OUT T& tValue) const
{
	CGroupNode* pChildGroup = GetChildNode(stGroupName);
	if (NULL != pChildGroup)
	{
		if (pChildGroup->GetValue(stRow, iCol, tValue))
			return true;
	}
	// default group�� ���캽.
	pChildGroup = GetChildNode("default");
	if (NULL != pChildGroup)
	{
		if (pChildGroup->GetValue(stRow, iCol, tValue))
			return true;
	}
	return false;
}

template <typename T>
bool CGroupNode::GetGroupValue(const std::string& stGroupName, const std::string& stRow, const std::string& stCol, OUT T& tValue) const
{
	CGroupNode* pChildGroup = GetChildNode(stGroupName);
	if (NULL != pChildGroup)
	{
		if (pChildGroup->GetValue(stRow, stCol, tValue))
			return true;
	}
	// default group�� ���캽.
	pChildGroup = GetChildNode("default");
	if (NULL != pChildGroup)
	{
		if (pChildGroup->GetValue(stRow, stCol, tValue))
			return true;
	}
	return false;
}

template <typename T>
bool CGroupNode::CGroupNodeRow::GetValue(const std::string& stColKey, OUT T& value) const
{
	int idx = m_pOwnerGroupNode->GetColumnIndexFromName(stColKey);
	if (idx < 0 || (CTokenVectorMap::size_type)idx >= m_vec_values.size())
		return false;
	return from_string(value, m_vec_values[idx]);
}

template <typename T>
bool CGroupNode::CGroupNodeRow::GetValue(int idx, OUT T& value) const
{
	if (idx < 0 || (CTokenVectorMap::size_type)idx >= m_vec_values.size())
		return false;
	return from_string(value, m_vec_values[idx]);
}
