#pragma once

#ifndef __INC_HWID_MANAGER_H__
#define __INC_HWID_MANAGER_H__

#include <string>
//#include "../EterBase/Singleton.h"
#include <windows.h>

class CHWIDManager : public CSingleton<CHWIDManager>
{
public:
	CHWIDManager();
	~CHWIDManager();

	std::string GetHWID() { return m_strHWID; };

private:
	long GetStringRegKey(HKEY hKey, const std::wstring& strValueName, std::wstring& strValue, const std::wstring& strDefaultValue);
	std::string m_strHWID;

};

#endif // __INC_HWID_MANAGER_H__
