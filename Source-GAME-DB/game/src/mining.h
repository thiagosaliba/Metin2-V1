#ifndef __INC_MINING_H__
#define __INC_MINING_H__

namespace mining
{
	LPEVENT CreateMiningEvent(LPCHARACTER ch, LPCHARACTER load, int count);
	DWORD GetRawOreFromLoad(DWORD dwLoadVnum);
	bool OreRefine(LPCHARACTER ch, LPCHARACTER npc, LPITEM item, int cost, int pct, LPITEM metinstone_item);
	int GetFractionCount();

	// REFINE_PICK
	int RealRefinePick(LPCHARACTER ch, LPITEM item);
	void CHEAT_MAX_PICK(LPCHARACTER ch, LPITEM item);
	// END_OF_REFINE_PICK

	bool IsVeinOfOre(DWORD vnum);
}

#endif // __INC_MINING_H__
