#ifndef __INC_CHECK_SERVER_KEY_H__
#define __INC_CHECK_SERVER_KEY_H__

#include <string>
#include <vector>
#include "CheckServerKey.h"

class CheckServer
{
public:
	static FORCEINLINE void AddServerKey(const char* serverKey)
	{
		keys_.push_back(serverKey);
	}

	static FORCEINLINE bool CheckIp(const char* ip)
	{
		// ������ �Ȱɸ��� üũ ����
// #ifndef _USE_SERVER_KEY_
		// fail_ = false;
		// return true;
// #endif

		for (int i = 0; i < keys_.size(); i++)
		{
			// �ϳ��� �´� ����Ű�� ������ ok
			std::string errorString;
			if (CheckServerKey(keys_[i].c_str(), ip, "", errorString))
			{
				fail_ = false;
				break;
			}
		}

		return !IsFail();
	}

	static FORCEINLINE bool IsFail()
	{
		return fail_;
	}

private:
	static std::vector<std::string> keys_;
	static bool fail_;
};

#endif // __INC_CHECK_SERVER_KEY_H__