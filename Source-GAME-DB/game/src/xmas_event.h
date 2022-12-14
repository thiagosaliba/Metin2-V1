#ifndef __INC_XMAS_EVENT_H__
#define __INC_XMAS_EVENT_H__

namespace xmas
{
	enum
	{
		MOB_SANTA_VNUM = 20031, // ??Ÿ
		//MOB_SANTA_VNUM = 20095, // ????
		MOB_XMAS_TREE_VNUM = 20032,
		MOB_XMAS_FIRWORK_SELLER_VNUM = 9004,
	};

	void ProcessEventFlag(const std::string& name, int prev_value, int value);
	void SpawnSanta(long lMapIndex, int iTimeGapSec);
	void SpawnEventHelper(bool spawn);
}
#endif // __INC_XMAS_EVENT_H__
