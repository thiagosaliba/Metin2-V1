/**
* Title: Apply Random
* Description: Generate a random bonus path.
* Author: Owsap
* Date: 2022.03.25
**/

#include "stdafx.h"

#if defined(__ITEM_APPLY_RANDOM__)
#include "group_text_parse_tree.h"
#include "item_apply_random_table.h"
#include "constants.h"

const std::string c_stApplyListName = "applylist";
const std::string c_astPathName[APPLY_RANDOM_MAX_PATHS] = { "path1", "path2" };

bool CApplyRandomTable::ReadApplyRandomTableFile(const char* c_pszFileName)
{
	m_pLoader = new CGroupTextParseTreeLoader;
	CGroupTextParseTreeLoader& rkLoader = *m_pLoader;

	if (rkLoader.Load(c_pszFileName) == false)
		return false;

	if (!ReadApplyRandomMapper())
		return false;

	if (!ReadApplyRandomTypes())
		return false;

	m_pApplyRandomValueTableNode = rkLoader.GetGroup("applyrandomvalues");
	if (m_pApplyRandomValueTableNode)
		return true;
	
	return false;
}

bool CApplyRandomTable::ReadApplyRandomMapper()
{
	CGroupNode* pGroupNode = m_pLoader->GetGroup("applyrandommapper");
	if (pGroupNode == nullptr)
	{
		sys_err(0, "Group ApplyRandomMapper not found.");
		return false;
	}

	{
		std::size_t nSize = pGroupNode->GetRowCount();
		if (nSize == 0)
		{
			sys_err(0, "Group ApplyRandomMapper is Empty.");
			return false;
		}

		for (std::size_t n = 0; n < nSize; n++)
		{
			const CGroupNode::CGroupNodeRow* c_pRow;
			pGroupNode->GetRow(n, &c_pRow);

			std::string stGroupName;
			if (!c_pRow->GetValue("group_name", stGroupName))
			{
				sys_err(0, "In Group ApplyRandomMapper, Column Group_Name not found.");
				return false;
			}

			std::transform(stGroupName.begin(), stGroupName.end(), stGroupName.begin(), ::tolower);
			m_vecApplyRandomGroups.emplace_back(stGroupName);
		}
	}
	return true;
}

bool CApplyRandomTable::ReadApplyRandomTypes()
{
	CGroupNode* pGroupNode = m_pLoader->GetGroup("applyrandomtypes");
	if (pGroupNode == nullptr)
	{
		sys_err(0, "Table needs ApplyRandomTypes.");
		return false;
	}

	for (std::size_t n = 0; n < m_vecApplyRandomGroups.size(); n++)
	{
		CGroupNode* pChild;
		if (nullptr == (pChild = pGroupNode->GetChildNode(m_vecApplyRandomGroups[n])))
		{
			sys_err(0, "In Group ApplyRandomTypes, %s group is not defined.", m_vecApplyRandomGroups[n].c_str());
			return false;
		}

		ApplyRandomVector vecApplyRandom;
		std::size_t nSize = pChild->GetRowCount();
		for (int j = 1; j <= nSize; j++)
		{
			std::stringstream stStream;
			stStream << j;
			const CGroupNode::CGroupNodeRow* c_pRow = nullptr;

			pChild->GetRow(stStream.str(), &c_pRow);
			if (c_pRow == nullptr)
			{
				sys_err(0, "In Group ApplyRandomTypes, No %d row.", j);
				return false;
			}

			std::string stTypeName;
			if (!c_pRow->GetValue("apply_type", stTypeName))
			{
				sys_err(0, "In Group ApplyRandomTypes, %s group's Apply_Type is empty.", m_vecApplyRandomGroups[n].c_str());
				return false;
			}

			EApplyTypes ApplyType;
			if (!(ApplyType = (EApplyTypes)FN_get_apply_type(stTypeName.c_str())))
			{
				sys_err(0, "In Group ApplyRandomTypes, %s group's Apply_Type %s is invalid.", m_vecApplyRandomGroups[n].c_str(), stTypeName.c_str());
				return false;
			}

			std::string stApplyValueGroupName;
			if (!c_pRow->GetValue("apply_value_group_name", stApplyValueGroupName))
			{
				sys_err(0, "In Group ApplyRandomTypes, %s group's Apply_Value_Group_Name is empty.", m_vecApplyRandomGroups[n].c_str());
				return false;
			}

			vecApplyRandom.push_back(SApplyRandom(ApplyType, stApplyValueGroupName));
		}
		m_mapApplyRandomGroup.insert(ApplyRandomGroupMap::value_type(m_vecApplyRandomGroups[n].c_str(), vecApplyRandom));
	}
	return true;
}

bool CApplyRandomTable::GetApplyRandom(uint8_t uiIndex, uint8_t uiLevel, uint8_t& uiApplyType, int32_t& iApplyValue, uint8_t& uiPath)
{
	std::string stApplyListName(c_stApplyListName + std::to_string(uiIndex));
	std::transform(stApplyListName.begin(), stApplyListName.end(), stApplyListName.begin(), ::tolower);

	ApplyRandomGroupMap::const_iterator it = m_mapApplyRandomGroup.find(stApplyListName);
	if (m_mapApplyRandomGroup.end() != it)
	{
		if (uiLevel >= ITEM_REFINE_MAX_LEVEL)
			uiLevel = ITEM_REFINE_MAX_LEVEL;

		ApplyRandomVector vecApplyRandom = it->second;

		uint8_t uiRandom = number(0, vecApplyRandom.size() - 1);

		uiApplyType = vecApplyRandom[uiRandom].ApplyType;
		uiPath = number(APPLY_RANDOM_PATH1, APPLY_RANDOM_MAX_PATHS);

		int32_t iPath1Value, iPath2Value;
		if (GetApplyRandomValue(vecApplyRandom[uiRandom].ApplyValueGroupName, uiLevel, iPath1Value, iPath2Value))
		{
			switch (uiPath)
			{
			default:
			case APPLY_RANDOM_PATH1:
				iApplyValue = iPath1Value;
				break;
			case APPLY_RANDOM_PATH2:
				iApplyValue = iPath2Value;
				break;
			}
			return true;
		}
	}
	else
	{
		uiApplyType = APPLY_NONE;
		iApplyValue = 0;
		uiPath = 0;
	}

	return false;
}

int32_t CApplyRandomTable::GetNextApplyRandomValue(uint8_t uiIndex, uint8_t uiLevel, uint8_t uiPath, uint8_t uiApplyType)
{
	std::string stApplyListName(c_stApplyListName + std::to_string(uiIndex));
	std::transform(stApplyListName.begin(), stApplyListName.end(), stApplyListName.begin(), ::tolower);

	ApplyRandomGroupMap::const_iterator it = m_mapApplyRandomGroup.find(stApplyListName.c_str());
	if (m_mapApplyRandomGroup.end() != it)
	{
		for (const SApplyRandom& vecApplyRandom : it->second)
		{
			if (vecApplyRandom.ApplyType == uiApplyType)
			{
				int32_t iPath1Value = 0, iPath2Value = 0;
				if (GetApplyRandomValue(vecApplyRandom.ApplyValueGroupName, uiLevel, iPath1Value, iPath2Value))
				{
					switch (uiPath)
					{
					default:
					case APPLY_RANDOM_PATH1: return iPath1Value;
					case APPLY_RANDOM_PATH2: return iPath2Value;
					}
				}
				break;
			}
		}
	}
	return 0;
}

bool CApplyRandomTable::GetApplyRandomValue(std::string stApplyValueGroupName, uint8_t uiLevel, int32_t& iPath1Value, int32_t& iPath2Value)
{
	std::transform(stApplyValueGroupName.begin(), stApplyValueGroupName.end(), stApplyValueGroupName.begin(), ::tolower);

	if (!m_pApplyRandomValueTableNode->GetGroupValue(stApplyValueGroupName, c_astPathName[0], uiLevel, iPath1Value))
	{
		sys_err(0, "Invalid path value. ApplyRandomValues Group(%s) Level(%d)", stApplyValueGroupName.c_str(), uiLevel);
		return false;
	}

	if (!m_pApplyRandomValueTableNode->GetGroupValue(stApplyValueGroupName, c_astPathName[1], uiLevel, iPath2Value))
	{
		sys_err(0, "Invalid path value. ApplyRandomValues Group(%s) Level(%d)", stApplyValueGroupName.c_str(), uiLevel);
		return false;
	}

	return true;
}

CApplyRandomTable::CApplyRandomTable() : m_pLoader(nullptr), m_pApplyRandomValueTableNode(nullptr) {}
CApplyRandomTable::~CApplyRandomTable()
{
	if (m_pLoader)
		delete m_pLoader;
}
#endif
