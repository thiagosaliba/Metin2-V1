#ifndef __INC_ITEM_APPLY_RANDOM_H__
#define __INC_ITEM_APPLY_RANDOM_H__

/**
* Title: Apply Random (Base Bonus)
* Author: Owsap
* Date: 2022.03.25
**/

#if defined(__ITEM_APPLY_RANDOM__)
enum EApplyRandom
{
	APPLY_RANDOM_PATH1 = 1,
	APPLY_RANDOM_PATH2,
	APPLY_RANDOM_MAX_PATHS,
	ITEM_REFINE_MAX_LEVEL = 15,
};

struct SApplyRandom
{
	SApplyRandom(EApplyTypes _ApplyType, std::string _ApplyValueGroupName) : ApplyType(_ApplyType), ApplyValueGroupName(_ApplyValueGroupName) {}
	EApplyTypes ApplyType;
	std::string ApplyValueGroupName;
};

class CGroupNode;
class CGroupTextParseTreeLoader;

class CApplyRandomTable
{
public:
	CApplyRandomTable();
	virtual ~CApplyRandomTable();

	bool ReadApplyRandomTableFile(const char* c_pszFileName);
	bool GetApplyRandom(uint8_t uiIndex, uint8_t uiLevel, uint8_t& uiApplyType, int32_t& iApplyValue, uint8_t& uiPath);
	int32_t GetNextApplyRandomValue(uint8_t uiIndex, uint8_t uiLevel, uint8_t uiPath, uint8_t uiApplyType);

public:
	using ApplyRandomVector = std::vector<SApplyRandom>;
	using ApplyRandomGroupMap = std::map<std::string, ApplyRandomVector>;

private:
	bool ReadApplyRandomMapper();
	bool ReadApplyRandomTypes();
	bool GetApplyRandomValue(std::string stApplyValueGroupName, uint8_t uiLevel, int32_t& iPath1Value, int32_t& iPath2Value);

private:
	CGroupTextParseTreeLoader* m_pLoader;
	std::string stFileName;

	CGroupNode* m_pApplyRandomValueTableNode;

	std::vector<std::string> m_vecApplyRandomGroups;
	ApplyRandomGroupMap m_mapApplyRandomGroup;
};

#endif // __INC_ITEM_APPLY_RANDOM_H__
#endif // __ITEM_APPLY_RANDOM__
