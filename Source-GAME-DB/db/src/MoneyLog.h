#ifndef __INC_MONEY_LOG
#define __INC_MONEY_LOG

#include <map>

typedef std::map<DWORD, int> MoneyLogMap;

class CMoneyLog : public singleton<CMoneyLog>
{
public:
	CMoneyLog();
	virtual ~CMoneyLog();

	void Save();
	void AddLog(BYTE bType, DWORD dwVnum, int iGold);

private:
	MoneyLogMap m_MoneyLogContainer[MONEY_LOG_TYPE_MAX_NUM];
};

#endif
