#ifndef __INC_MINI_GAME_H__
#define __INC_MINI_GAME_H__

#include <boost/unordered_map.hpp>

#include "../../common/stl.h"
#include "../../common/length.h"
#include "../../common/tables.h"

#include "packet.h"
#include "questmanager.h"

class CMiniGameManager : public singleton<CMiniGameManager>
{
public:
	CMiniGameManager();
	virtual ~CMiniGameManager();

	enum EMapIndex
	{
		EMPIRE_A = 1,
		EMPIRE_B = 21,
		EMPIRE_C = 41,
	};

	bool Initialize();
	void Destroy();

	void SpawnEventNPC(DWORD dwVnum);

#if defined(__EVENT_BANNER_FLAG__)
public:
	typedef std::map<DWORD, std::string> BannerMapType;
	bool InitializeBanners();
	bool SpawnBanners(int iEnable, const char* c_szBannerName = NULL);

protected:
	BannerMapType BannerMap;
	bool m_bIsLoadedBanners;
#endif

#if defined(__MINI_GAME_CATCH_KING__)
public:
	int MiniGameCatchKing(LPCHARACTER ch, const char* data, size_t uiBytes);

	void MiniGameCatchKingEventInfo(LPCHARACTER pkChar);
	void InitializeMiniGameCatchKing(int iEnable);
	void InitializeMiniGameCatchKingEndTime(int iEndTime) { iCatchKingEndTime = iEndTime; }
	void MiniGameCatchKingCheckEnd();

	void MiniGameCatchKingStartGame(LPCHARACTER pkChar, BYTE bSetCount);
	void MiniGameCatchKingDeckCardClick(LPCHARACTER pkChar);
	void MiniGameCatchKingFieldCardClick(LPCHARACTER pkChar, BYTE bFieldPos);
	void MiniGameCatchKingGetReward(LPCHARACTER pkChar);

	void MiniGameCatchKingRegisterScore(LPCHARACTER pkChar, DWORD dwScore);
	int MiniGameCatchKingGetScore(lua_State* L, bool isTotal);
	int MiniGameCatchKingGetMyScore(LPCHARACTER pkChar);

protected:
	int iCatchKingEndTime;
#endif

};

#endif // __INC_MINI_GAME_H__
