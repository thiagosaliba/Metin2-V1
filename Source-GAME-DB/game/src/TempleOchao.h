#ifndef __INC_TEMPLE_OCHAO_H__
#define __INC_TEMPLE_OCHAO_H__

namespace TempleOchao
{
	enum
	{
		MAP_INDEX = 354,
		ROOMS = 11,
		GUARDIAN = 6405,
		PORTAL = 20415,
		PORTAL_VANISH_TIME = 60,
		CHECK_ACTIVITY = 30,
		NO_ACTIVITY = 180, // 3 min
		ATTACKED = 600, // 10 min
	};

	class CMgr : public singleton<CMgr>
	{
	public:
		void Initialize();
		void Destroy();
		void Prepare();
		void Spawn();
		void ClearPrepare();
		void SetRoom(int iRoom) { iTempleRoom = iRoom; }
		int GetRoom() { return iTempleRoom; }
		void RandomRoom(int& iRoom, int& iX, int& iY, int& iZ);
		void ChangeRoom();
		void SetGuardianVID(DWORD dwGuardianVID) { dwGuardianVID = dwGuardianVID; }
		DWORD GetGuardianVID() { return dwGuardianVID; }
		void SetPortalVID(DWORD dwPortalVID) { dwPortalVID = dwPortalVID; }
		DWORD GetPortalVID() { return dwPortalVID; }
		void OnGuardianKilled(int iX, int iY, int iZ);
		void AttackedGuardian(LPCHARACTER ch);

	private:
		int iTempleRoom;
		DWORD dwGuardianVID, dwPortalVID;
		bool bOnGuardianKilled;

	protected:
		LPEVENT s_pkGuardianKilledEvent, s_pkGuardianActivityEvent;
	};
}
#endif // __INC_TEMPLE_OCHAO_H__
