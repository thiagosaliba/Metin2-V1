#include "StdAfx.h"
#include "PythonApplication.h"
#include "ProcessScanner.h"
#include "PythonExceptionSender.h"
#include "resource.h"
#include "Version.h"

#ifdef _DEBUG
//#include <crtdbg.h>
#endif

#include "../EterPack/EterPackManager.h"
#include "../EterLib/Util.h"
#include "CefWebBrowser.h"
#include "../EterBase/CPostIt.h"

#include "CheckLatestFiles.h"

#ifdef ENABLE_MD5_FILE_CHECK
#include "md5.h"
#include <Windows.h>
#endif

#include "HWIDManager.h"

#ifdef ENABLE_CONFIG_MODULE
#include "PythonConfig.h"
#endif

extern "C"
{
	extern int _fltused;
	volatile int _AVOID_FLOATING_POINT_LIBRARY_BUG = _fltused;
};
extern "C" { FILE __iob_func[3] = { *stdin, *stdout, *stderr }; }

#pragma comment(linker, "/NODEFAULTLIB:libci.lib")

#pragma comment(lib, "version.lib")
#pragma comment(lib, "python27.lib")
#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "devil.lib")
#pragma comment(lib, "granny2.lib")
#pragma comment(lib, "mss32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "imm32.lib")
#pragma comment(lib, "oldnames.lib")
#pragma comment(lib, "SpeedTreeRT.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dmoguids.lib")
//#pragma comment(lib, "wsock32.lib")

#if defined(ENABLE_DISCORD_RPC)
#ifdef _DEBUG
#pragma comment(lib, "DiscordRPCd.lib")
#else
#pragma comment(lib, "DiscordRPC.lib")
#endif
#endif

#pragma comment (lib, "libcef.lib")
#ifdef _DEBUG
#pragma comment (lib, "libcef_dll_wrapperd.lib")
#else
#pragma comment (lib, "libcef_dll_wrapper.lib")
#endif

#include <stdlib.h>
#include <cryptopp/cryptoppLibLink.h>
bool __IS_TEST_SERVER_MODE__ = false;

#if defined(ENABLE_CYTHON)
#include "PythonrootlibManager.h"
#endif

extern bool SetDefaultCodePage(DWORD codePage);

static const char* sc_apszPythonLibraryFilenames[] =
{
	"UserDict.pyc",
	"__future__.pyc",
	"copy_reg.pyc",
	"linecache.pyc",
	"ntpath.pyc",
	"os.pyc",
	"site.pyc",
	"stat.pyc",
	"string.pyc",
	"traceback.pyc",
	"types.pyc",
	"\n",
};

#ifdef ENABLE_MD5_FILE_CHECK
static const char* ar_szMD5FileNames[][2] =
{
	// Miles
	{ "miles/mss32.dll", "6400e224b8b44ece59a992e6d8233719" },
	{ "miles/mssa3d.m3d", "e089ce52b0617a6530069f22e0bdba2a" },
	{ "miles/mssds3d.m3d", "85267776d45dbf5475c7d9882f08117c" },
	{ "miles/mssdsp.flt", "cb71b1791009eca618e9b1ad4baa4fa9" },
	{ "miles/mssdx7.m3d", "2727e2671482a55b2f1f16aa88d2780f" },
	{ "miles/msseax.m3d", "788bd950efe89fa5166292bd6729fa62" },
	{ "miles/mssmp3.asi", "189576dfe55af3b70db7e3e2312cd0fd" },
	{ "miles/mssrsx.m3d", "7fae15b559eb91f491a5f75cfa103cd4" },
	{ "miles/msssoft.m3d", "bdc9ad58ade17dbd939522eee447416f" },
	{ "miles/mssvoice.asi", "3d5342edebe722748ace78c930f4d8a5" },

	{ "artpclnt.dll", "26b7ef7142e838b886fed8159c891a10" },
	{ "bdvid32.dll", "d97c8fcd782bc2342a39937daf6605a0" },
	{ "chrome_100_percent.pak", "5d5a761197e22a4756063293385479e5" },
	{ "chrome_200_percent.pak", "834057a3f89568def061e37e2db94905" },
	{ "chrome_elf.dll", "3dfb45ef32231373e28aee9bbf6c788e" },
	{ "config.exe", "df9f6413dbffe48212c61ad93e228a7d" },
	{ "config.ini", "b65f6c497648564c3f21bba988800cb9" },
	{ "d3dcompiler_47.dll", "02e034cd47aa9a633f6aaef348dbbba0" },
	{ "devil.dll", "8df4d4324e5755f1a0567db3c5be4c58" },
	{ "DSETUP.dll", "4f5f399a970a921f883975a2228a1c8c" },
	{ "granny2.dll", "6f403e1359bbc6b5f23b2def240e4c3c" },
	{ "icudtl.dat", "80a7528515595d8b0bf99a477a7eff0d" },
	{ "ijl15.dll", "1aa06c81a0621e277e755b965b5e4b5f" },
	{ "ilu.dll", "bd4ff726291d98374fa6791c0589f288" },
	{ "libcef.dll", "ac1a9115b632556ed44c7a0de9e8bbaa" },
	{ "libEGL.dll", "7a95317ae31eec8cfea6d45c1330bbf7" },
	{ "libGLESv2.dll", "78d0764aa34c61555aa08756173e2f65" },
	{ "locale_list.txt", "391dfcc68d4a11472519fbe29fc4c54e" },
	//{ "logo1.wmv", "1e3095c51e38c67f58f81fd5632d4149" },
	{ "mscoree.dll", "ae30494155fc06b63a0410740a18e676" },
	{ "MSS32.DLL", "6400e224b8b44ece59a992e6d8233719" },
	{ "msvcp60.dll", "cb21d826d9c39aed19dd431c1880f5de" },
	{ "MSVCRTD.DLL", "01c72cfcaaed3a7564e5b893b7712d28" },
	{ "patchw32.dll", "1bc225e0c1c2f54cc877b654ccf04684" },
	{ "python27.dll", "a35fef665eec18e0bf4195359a106261" },
	{ "resources.pak", "7ba24dd690aadef8607fc46a98d431c4" },
	{ "snapshot_blob.bin", "cf4d8376cb8a10249310928a26f6d966" },
	{ "SpeedTreeRT.dll", "1ac3d612389fa679f5ca3c6bab855145" },
	{ "unicows.dll", "f8d176db5b14aed7c9b25e0640226bd1" },
	{ "v8_context_snapshot.bin", "bcfe5f1279dfea4dce05e79c9d87f015" },

	// Pack
	{ "pack/bgm.data", "a69d5eecdf47e1a58c4dad3461cf3e17" },
	{ "pack/bgm.idx", "6ea56734244a1670fc866893bb050f2c" },
	{ "pack/effect.data", "00d1032eeef161dacfea3aebaa58084c" },
	{ "pack/effect.idx", "f5460bdda7db3fbdf65c6180b88b73af" },
	{ "pack/environment.data", "e9417ec5c8f1657ac4f5008ddcd45248" },
	{ "pack/environment.idx", "5f9fa484e2c3c616fff836e9be2fe0b1" },
	{ "pack/guild.data", "b64de095ada166a1f172565640e30622" },
	{ "pack/guild.idx", "7c8f46c2f60a2e6a9da88dc6c08638d2" },
	{ "pack/icon.data", "314cfbc8e98af1b1aa151cd84ee0ae95" },
	{ "pack/icon.idx", "85fc93507d26e2ee91ceaa9acb1e329e" },
	{ "pack/item.data", "bc989cdb7dcbe22b8c66b595dbb28c71" },
	{ "pack/item.idx", "19292e42260a1ba5bd3f9ea365ec23a4" },
	{ "pack/locale.data", "e7d0fcb5012721a96b7922ee33157631" },
	{ "pack/locale.idx", "ade867e157cc299a5b2477da8c0cdb3e" },
	{ "pack/map.data", "f57ab557be45bf4c903eb241de018ca0" },
	{ "pack/map.idx", "835297997bd214d2f3e2867a81adddb0" },
	{ "pack/monster.data", "5540f4f34e76ca5a0ebc0e220b2ef517" },
	{ "pack/monster.idx", "3d3cb1d8209eef02e1c129218cdb0abd" },
	{ "pack/monster2.data", "5aa12ee19e69a539988ca08f9ef1ef0a" },
	{ "pack/monster2.idx", "7100c1b40d01f8e01233ee247328800b" },
	{ "pack/npc.data", "098a446d5f119a1fe07b347eaf19134d" },
	{ "pack/npc.idx", "5c42a6f178cfe74b39b38fed0afb20bc" },
	{ "pack/npc_mount.data", "cc19b023783b4b189d616cf83c171a29" },
	{ "pack/npc_mount.idx", "60c9ae895569aa4e5841228abc0a738b" },
	{ "pack/npc_pet.data", "c36875047c0b532a23bc989f4fc790ad" },
	{ "pack/npc_pet.idx", "c0001364498c89d97c8cffed79767bc5" },
	{ "pack/npc2.data", "59017b88cc1abd4015ca1578e0d41ae5" },
	{ "pack/npc2.idx", "45c98451125c03afc1d220787c6ae269" },
	{ "pack/patch_osf.data", "53e644d37a9d253430f111943500ee47" },
	{ "pack/patch_osf.idx", "770eef2f5bd983518deaf5cc6fd08096" },
	{ "pack/pc.data", "c182e2d1898183641bda68da82a7004e" },
	{ "pack/pc.idx", "d5ef4ae9831d1bf746ce0439085f9ba8" },
	{ "pack/pc2.data", "981aef73098a580cc570518032d2768d" },
	{ "pack/pc2.idx", "57c4f7b5683e29c462a1eae465b13c9e" },
	{ "pack/pc3.data", "b842cb67bf53dde14acfb238281f6ea2" },
	{ "pack/pc3.idx", "1a46d8f8325dca53ba858b4802570a89" },
	{ "pack/property.data", "f1f66862bf73f7cef9edea5896bdcd7b" },
	{ "pack/property.idx", "679387128430c1e82b425971db46ba4c" },
	{ "pack/root.data", "cfc675a4b63e2e770dfb68250c7827b3" },
	{ "pack/root.idx", "642e54459b8bfd591dc490fe150e8ba0" },
	{ "pack/season1.data", "cc814cd6397ff22c4c4c8fc426a58f3b" },
	{ "pack/season1.idx", "8ce399a8faa3f8fdd282b6f97cdac1b7" },
	{ "pack/season2.data", "fff00c67f656739e2de21b238ad4d6ab" },
	{ "pack/season2.idx", "1f1eb200ba158483b45179646d567c1c" },
	{ "pack/sound.data", "423a86b7cb85f37b1d954f5cfde462a8" },
	{ "pack/sound.idx", "389f4758662d96b9933df13a70b52076" },
	{ "pack/terrain.data", "6dae4f3fcf616eafc9f4ee834f4a26d6" },
	{ "pack/terrain.idx", "4eda5ae3d63084a1a099f1f5f9ecbb5c" },
	{ "pack/textureset.data", "d5122e1ba00af5f79e95b2536d0030ae" },
	{ "pack/textureset.idx", "73f41c1e13681653185ae4360c77b034" },
	{ "pack/tree.data", "df7fef18de778ef9b77a6f5bdea2e5d4" },
	{ "pack/tree.idx", "0a33390165dffe252db63ae066ba4781" },
	{ "pack/ui.data", "4b3551bf77eab7a18df41d6f97869719" },
	{ "pack/ui.idx", "91c8ece8a4fd970bd95047dbe3912985" },
	{ "pack/zone.data", "49c8ddfdb9be9b7cf699e57ec5bbfd41" },
	{ "pack/zone.idx", "ee32fe8b1f2c4dc774d217021ed961b6" },
};
#endif

char gs_szErrorString[512] = "";

void ApplicationSetErrorString(const char* szErrorString)
{
	strcpy(gs_szErrorString, szErrorString);
}

bool CheckPythonLibraryFilenames()
{
	for (int i = 0; *sc_apszPythonLibraryFilenames[i] != '\n'; ++i)
	{
		std::string stFilename = "lib\\";
		stFilename += sc_apszPythonLibraryFilenames[i];

		if (_access(stFilename.c_str(), 0) != 0)
		{
			return false;
		}

		MoveFile(stFilename.c_str(), stFilename.c_str());
	}

	return true;
}

struct ApplicationStringTable
{
	HINSTANCE m_hInstance;
	std::map<DWORD, std::string> m_kMap_dwID_stLocale;
} gs_kAppStrTable;

void ApplicationStringTable_Initialize(HINSTANCE hInstance)
{
	gs_kAppStrTable.m_hInstance = hInstance;
}

const std::string& ApplicationStringTable_GetString(DWORD dwID, LPCSTR szKey)
{
	char szBuffer[512];
	char szIniFileName[256];
	char szLocale[256];

	::GetCurrentDirectory(sizeof(szIniFileName), szIniFileName);
	if (szIniFileName[lstrlen(szIniFileName) - 1] != '\\')
		strcat(szIniFileName, "\\");
	strcat(szIniFileName, "metin2client.dat");

	strcpy(szLocale, LocaleService_GetLocalePath());
	if (strnicmp(szLocale, "locale/", strlen("locale/")) == 0)
		strcpy(szLocale, LocaleService_GetLocalePath() + strlen("locale/"));
	::GetPrivateProfileString(szLocale, szKey, NULL, szBuffer, sizeof(szBuffer) - 1, szIniFileName);
	if (szBuffer[0] == '\0')
		LoadString(gs_kAppStrTable.m_hInstance, dwID, szBuffer, sizeof(szBuffer) - 1);
	if (szBuffer[0] == '\0')
		::GetPrivateProfileString("en", szKey, NULL, szBuffer, sizeof(szBuffer) - 1, szIniFileName);
	if (szBuffer[0] == '\0')
		strcpy(szBuffer, szKey);

	std::string& rstLocale = gs_kAppStrTable.m_kMap_dwID_stLocale[dwID];
	rstLocale = szBuffer;

	return rstLocale;
}

const std::string& ApplicationStringTable_GetString(DWORD dwID)
{
	char szBuffer[512];

	LoadString(gs_kAppStrTable.m_hInstance, dwID, szBuffer, sizeof(szBuffer) - 1);
	std::string& rstLocale = gs_kAppStrTable.m_kMap_dwID_stLocale[dwID];
	rstLocale = szBuffer;

	return rstLocale;
}

const char* ApplicationStringTable_GetStringz(DWORD dwID, LPCSTR szKey)
{
	return ApplicationStringTable_GetString(dwID, szKey).c_str();
}

const char* ApplicationStringTable_GetStringz(DWORD dwID)
{
	return ApplicationStringTable_GetString(dwID).c_str();
}

////////////////////////////////////////////

int Setup(LPSTR lpCmdLine); // Internal function forward

#if defined(DISABLE_INDEX_FILE)
bool PackInitialize(const char* c_pszFolder)
{
	std::vector<std::vector<std::string>> indexVec
	{
		{ "pack/", "patch_osf" },
		{ "d:/ymir work/pc/", "pc" },
		{ "d:/ymir work/pc2/", "pc2" },
		{ "d:/ymir work/pc3/", "pc3" },
		{ "d:/ymir work/monster/", "monster" },
		{ "d:/ymir work/monster2/", "monster2" },
		{ "d:/ymir work/effect/", "effect" },
		{ "d:/ymir work/zone/", "zone" },
		{ "d:/ymir work/terrainmaps/", "terrain" },
		{ "d:/ymir work/npc/", "npc" },
		{ "d:/ymir work/npc2/", "npc2" },
		{ "d:/ymir work/npc_mount/", "npc_mount" },
		{ "d:/ymir work/npc_pet/", "npc_pet" },
		{ "d:/ymir work/tree/", "tree" },
		{ "d:/ymir work/guild/", "guild" },
		{ "d:/ymir work/item/", "item" },
		{ "textureset/", "textureset" },
		{ "property/", "property" },
		{ "icon/", "icon" },
		// Season
		{ "season1/", "season1" },
		{ "season2/", "season2" },
		// Maps
		{ "gm_guild_build/", "map" },
		{ "map_a2/", "map" },
		{ "map_n_snowm_01/", "map" },
		{ "metin2_12zi_stage/", "map" },
		{ "metin2_guild_pve/", "map" },
		{ "metin2_guild_village/", "map" },
		{ "metin2_guild_village_01/", "map" },
		{ "metin2_guild_village_02/", "map" },
		{ "metin2_guild_village_03/", "map" },
		{ "metin2_guild_war1/", "map" },
		{ "metin2_guild_war2/", "map" },
		{ "metin2_guild_war3/", "map" },
		{ "metin2_guild_war4/", "map" },
		{ "metin2_map_a1/", "map" },
		{ "metin2_map_a3/", "map" },
		{ "metin2_map_b1/", "map" },
		{ "metin2_map_b3/", "map" },
		{ "metin2_map_battlefied/", "map" },
		{ "metin2_map_bayblacksand/", "map" },
		{ "metin2_map_boss_awaken_dawnmist/", "map" },
		{ "metin2_map_boss_awaken_flame/", "map" },
		{ "metin2_map_boss_awaken_skipia/", "map" },
		{ "metin2_map_boss_awaken_snow/", "map" },
		{ "metin2_map_boss_crack_dawnmist/", "map" },
		{ "metin2_map_boss_crack_flame/", "map" },
		{ "metin2_map_boss_crack_skipia/", "map" },
		{ "metin2_map_boss_crack_snow/", "map" },
		{ "metin2_map_c1/", "map" },
		{ "metin2_map_c3/", "map" },
		{ "metin2_map_capedragonhead/", "map" },
		{ "metin2_map_dawnmistwood/", "map" },
		{ "metin2_map_dawnmist_dungeon_01/", "map" },
		{ "metin2_map_defensewave/", "map" },
		{ "metin2_map_defensewave_port/", "map" },
		{ "metin2_map_devilscatacomb/", "map" },
		{ "metin2_map_deviltower1/", "map" },
		{ "metin2_map_duel/", "map" },
		{ "metin2_map_e1/", "map" },
		{ "metin2_map_elemental_01/", "map" },
		{ "metin2_map_elemental_02/", "map" },
		{ "metin2_map_elemental_03/", "map" },
		{ "metin2_map_elemental_04/", "map" },
		{ "metin2_map_guild_01/", "map" },
		{ "metin2_map_guild_02/", "map" },
		{ "metin2_map_guild_03/", "map" },
		{ "metin2_map_labyrinth/", "map" },
		{ "metin2_map_milgyo/", "map" },
		{ "metin2_map_miniboss_01/", "map" },
		{ "metin2_map_miniboss_02/", "map" },
		{ "metin2_map_mists_of_island/", "map" },
		{ "metin2_map_monkeydungeon/", "map" },
		{ "metin2_map_monkeydungeon_02/", "map" },
		{ "metin2_map_monkeydungeon_03/", "map" },
		{ "metin2_map_mt_thunder/", "map" },
		{ "metin2_map_mt_th_dungeon_01/", "map" },
		{ "metin2_map_n_desert_01/", "map" },
		{ "metin2_map_n_flame_01/", "map" },
		{ "metin2_map_n_flame_dragon/", "map" },
		{ "metin2_map_n_flame_dungeon_01/", "map" },
		{ "metin2_map_n_snow_dungeon_01/", "map" },
		{ "metin2_map_pvp_arena/", "map" },
		{ "metin2_map_skipia_dungeon_02/", "map" },
		{ "metin2_map_skipia_dungeon_boss/", "map" },
		{ "metin2_map_spiderdungeon/", "map" },
		{ "metin2_map_spiderdungeon_02/", "map" },
		{ "metin2_map_spiderdungeon_03/", "map" },
		{ "metin2_map_t1/", "map" },
		{ "metin2_map_t2/", "map" },
		{ "metin2_map_t3/", "map" },
		{ "metin2_map_t4/", "map" },
		{ "metin2_map_trent/", "map" },
		{ "metin2_map_trent02/", "map" },
		{ "metin2_map_wedding_01/", "map" },
		// Sound
		{ "sound/ambience/", "sound" },
		{ "sound/common/", "sound" },
		{ "sound/effect/", "sound" },
		{ "sound/monster/", "sound" },
		{ "sound/npc/", "sound" },
		{ "sound/pc/", "sound" },
		{ "sound/ui/", "sound" },
		{ "sound/ambience/", "sound" },
		{ "sound/common/", "sound" },
		{ "sound/effect/", "sound" },
		{ "sound/monster/", "sound" },
		{ "sound/npc/", "sound" },
		{ "sound/npc_mount/", "sound" },
		{ "sound/pc/", "sound" },
		{ "sound/ui/", "sound" },
		{ "sound/monster2/", "sound" },
		{ "sound/pc2/", "sound" },
		{ "sound/pc3/", "sound" },
		// BGM
		{ "bgm/", "bgm" },
		// ETC
		{ "d:/ymir work/special/", "special" },
		{ "d:/ymir work/environment/", "environment" },
		// LOCALE
		{ "locale/common/", "locale" },
		{ "locale/ca/", "locale" },
		{ "locale/ae/", "locale" },
		{ "locale/de/", "locale" },
		{ "locale/es/", "locale" },
		{ "locale/fr/", "locale" },
		{ "locale/gr/", "locale" },
		{ "locale/it/", "locale" },
		{ "locale/nl/", "locale" },
		{ "locale/pl/", "locale" },
		{ "locale/pt/", "locale" },
		{ "locale/tr/", "locale" },
		{ "locale/uk/", "locale" },
		{ "locale/bg/", "locale" },
		{ "locale/en/", "locale" },
		{ "locale/mx/", "locale" },
		{ "locale/ro/", "locale" },
		{ "locale/ru/", "locale" },
		{ "locale/dk/", "locale" },
		{ "locale/cz/", "locale" },
		{ "locale/hu/", "locale" },
		{ "locale/us/", "locale" },
		{ "locale/pa/", "locale" },
		{ "uiscript/", "uiscript" },
		{ "d:/ymir work/ui/", "ui" },
		{ "d:/ymir work/uiloading/", "uiloading" },
	};

	if (_access(c_pszFolder, 0) != 0)
		return true;

	std::string stFolder(c_pszFolder);
	stFolder += "/";

	const bool bPackFirst = false;

#if defined(USE_RELATIVE_PATH)
	CEterPackManager::Instance().SetRelativePathMode();
#endif
	CEterPackManager::Instance().SetCacheMode();
	CEterPackManager::Instance().SetSearchMode(bPackFirst);

	CSoundData::SetPackMode();

	std::string strPackName, strTexCachePackName;

	for (auto& elem : indexVec)
	{
		const std::string& c_rstFolder = elem[0];
		const std::string& c_rstName = elem[1];

		strPackName = stFolder + c_rstName;
		strTexCachePackName = strPackName + "_texcache";

		CEterPackManager::Instance().RegisterPack(strPackName.c_str(), c_rstFolder.c_str());
		CEterPackManager::Instance().RegisterPack(strTexCachePackName.c_str(), c_rstFolder.c_str());
	}

	CEterPackManager::Instance().RegisterRootPack((stFolder + std::string("root")).c_str());

	NANOEND

		return true;
}
#else
bool PackInitialize(const char* c_pszFolder)
{
	NANOBEGIN

		if (_access(c_pszFolder, 0) != 0)
			return true;

	std::string stFolder(c_pszFolder);
	stFolder += "/";

	std::string stFileName(stFolder);
	stFileName += "Index";

	CMappedFile file;
	LPCVOID pvData;

	if (!file.Create(stFileName.c_str(), &pvData, 0, 0))
	{
		LogBoxf("FATAL ERROR! File not exist: %s", stFileName.c_str());
		TraceError("FATAL ERROR! File not exist: %s", stFileName.c_str());
		return true;
	}

	CMemoryTextFileLoader TextLoader;
	TextLoader.Bind(file.Size(), pvData);

	bool bPackFirst = true;

	const std::string& strPackType = TextLoader.GetLineString(0);

	if (strPackType.compare("FILE") && strPackType.compare("PACK"))
	{
		TraceError("Pack/Index has invalid syntax. First line must be 'PACK' or 'FILE'");
		return false;
	}

#ifdef _DISTRIBUTE
	Tracef("알림: 팩 모드입니다.\n");

	/*
	if (0 == strPackType.compare("FILE"))
	{
		bPackFirst = FALSE;
		Tracef("알림: 파일 모드입니다.\n");
	}
	else
	{
		Tracef("알림: 팩 모드입니다.\n");
	}
	*/
#else
	bPackFirst = false;
	Tracef("알림: 파일 모드입니다.\n");
#endif

	CTextFileLoader::SetCacheMode();
#if defined(USE_RELATIVE_PATH)
	CEterPackManager::Instance().SetRelativePathMode();
#endif
	CEterPackManager::Instance().SetCacheMode();
	CEterPackManager::Instance().SetSearchMode(bPackFirst);

	CSoundData::SetPackMode(); // Miles 파일 콜백을 셋팅

	std::string strPackName, strTexCachePackName;
	for (DWORD i = 1; i < TextLoader.GetLineCount() - 1; i += 2)
	{
		const std::string& c_rstFolder = TextLoader.GetLineString(i);
		const std::string& c_rstName = TextLoader.GetLineString(i + 1);

		strPackName = stFolder + c_rstName;
		strTexCachePackName = strPackName + "_texcache";

		CEterPackManager::Instance().RegisterPack(strPackName.c_str(), c_rstFolder.c_str());
		CEterPackManager::Instance().RegisterPack(strTexCachePackName.c_str(), c_rstFolder.c_str());
	}

	CEterPackManager::Instance().RegisterRootPack((stFolder + std::string("root")).c_str());

	NANOEND

		return true;
}
#endif

bool RunMainScript(CPythonLauncher& pyLauncher, const char* lpCmdLine)
{
	initpack();
	initdbg();
	initime();
	initgrp();
	initgrpImage();
	initgrpText();
	initwndMgr();
	/////////////////////////////////////////////
	initudp();
	initapp();
#if defined(ENABLE_CYTHON)
	initsystemSetting();
#else
	initsystem();
#endif
	initchr();
	initchrmgr();
	initPlayer();
	initItem();
	initNonPlayer();
	initTrade();
	initChat();
	initTextTail();
	initnet();
	initMiniMap();
	initProfiler();
	initEvent();
	initeffect();
	initfly();
	initsnd();
	initeventmgr();
	initshop();
	initskill();
	initquest();
	initBackground();
	initMessenger();
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	initAcce();
#endif
#ifdef ENABLE_CONFIG_MODULE
	initcfg();
#endif
#if defined(ENABLE_MAILBOX)
	initmail();
#endif
	initsafebox();
	initguild();
	initServerStateChecker();
#if defined(RENDER_TARGET)
	initRenderTarget();
#endif
#if defined(ENABLE_CYTHON)
	initrootlibManager();
#endif

	NANOBEGIN

#define __BUILT_IN__
#if defined(__BUILT_IN__)
		PyObject* builtins = PyImport_ImportModule("__builtin__");

	// RegisterDebugFlag
#ifdef _DEBUG
	PyModule_AddIntConstant(builtins, "__DEBUG__", 1);
#else
	PyModule_AddIntConstant(builtins, "__DEBUG__", 0);
#endif

	// RegisterCythonFlag
#if defined(ENABLE_CYTHON)
	PyModule_AddIntConstant(builtins, "__USE_CYTHON__", 1);
#else
	PyModule_AddIntConstant(builtins, "__USE_CYTHON__", 0);
#endif
#else
		// RegisterDebugFlag
	{
		std::string stRegisterDebugFlag;

#ifdef _DEBUG
		stRegisterDebugFlag = "__DEBUG__ = 1";
#else
		stRegisterDebugFlag = "__DEBUG__ = 0";
#endif

		if (!pyLauncher.RunLine(stRegisterDebugFlag.c_str()))
		{
			TraceError("RegisterDebugFlag Error");
			return false;
		}
	}
#endif

	// RegisterCommandLine
	{
		std::string stRegisterCmdLine;

		const char* loginMark = "-cs";
		const char* loginMark_NonEncode = "-ncs";
		const char* seperator = " ";

		std::string stCmdLine;
		const int CmdSize = 3;
		std::vector<std::string> stVec;
		SplitLine(lpCmdLine, seperator, &stVec);
		if (CmdSize == stVec.size() && stVec[0] == loginMark)
		{
			char buf[MAX_PATH]; // TODO 아래 함수 string 형태로 수정
			base64_decode(stVec[2].c_str(), buf);
			stVec[2] = buf;
			string_join(seperator, stVec, &stCmdLine);
		}
		else if (CmdSize <= stVec.size() && stVec[0] == loginMark_NonEncode)
		{
			stVec[0] = loginMark;
			string_join(" ", stVec, &stCmdLine);
		}
		else
			stCmdLine = lpCmdLine;

#if defined(ENABLE_CYTHON)
		PyModule_AddStringConstant(builtins, "__COMMAND_LINE__", stCmdLine.c_str());
#else
		stRegisterCmdLine = "__COMMAND_LINE__ = ";
		stRegisterCmdLine += '"';
		stRegisterCmdLine += stCmdLine;
		stRegisterCmdLine += '"';

		const CHAR* c_szRegisterCmdLine = stRegisterCmdLine.c_str();
		if (!pyLauncher.RunLine(c_szRegisterCmdLine))
		{
			TraceError("RegisterCommandLine Error");
			return false;
		}
#endif
	}
	{
		std::vector<std::string> stVec;
		SplitLine(lpCmdLine, " ", &stVec);

		if (stVec.size() != 0 && "--pause-before-create-window" == stVec[0])
		{
			system("pause");
		}

#if defined(ENABLE_CYTHON)
		if (!pyLauncher.RunLine("import rootlib\nrootlib.moduleImport('system')"))
#else
		if (!pyLauncher.RunFile("system.py"))
#endif
		{
			TraceError("RunMain Error");
			return false;
		}
	}

	NANOEND
		return true;
}

bool Main(HINSTANCE hInstance, LPSTR lpCmdLine)
{
#if !defined(_DEBUG) && defined(ENABLE_PATCH_CHECK)
	if (strstr(lpCmdLine, "--bTIub3dzYXA=") == 0)
		return 0;
#endif

#ifdef LOCALE_SERVICE_YMIR
	extern bool g_isScreenShotKey;
	g_isScreenShotKey = true;
#endif

	DWORD dwRandSeed = time(NULL) + DWORD(GetCurrentProcess());
	srandom(dwRandSeed);
	srand(random());

	SetLogLevel(1);

#ifdef LOCALE_SERVICE_VIETNAM_MILD
	extern BOOL USE_VIETNAM_CONVERT_WEAPON_VNUM;
	USE_VIETNAM_CONVERT_WEAPON_VNUM = true;
#endif

	if (_access("perf_game_update.txt", 0) == 0)
	{
		DeleteFile("perf_game_update.txt");
	}

	if (_access("newpatch.exe", 0) == 0)
	{
		system("patchupdater.exe");
		return false;
	}

#ifndef __VTUNE__
	ilInit();
#endif

	if (!Setup(lpCmdLine))
		return false;

#ifdef _DEBUG
	OpenConsoleWindow();
	OpenLogFile(true); // true == uses syserr.txt and log.txt
#else
	OpenLogFile(false); // false == uses syserr.txt only
#endif

	static CLZO lzo;
	static CEterPackManager EterPackManager;
	static CHWIDManager HWIDManager;

#ifdef ENABLE_CONFIG_MODULE
	static CPythonConfig m_pyConfig;
	m_pyConfig.Initialize("config.cfg");
#endif

	if (!PackInitialize("pack"))
	{
		LogBox("Pack Initialization failed. Check log.txt file..");
		return false;
	}

	if (LocaleService_LoadGlobal(hInstance))
		SetDefaultCodePage(LocaleService_GetCodePage());

	CPythonApplication* app = new CPythonApplication;

	app->Initialize(hInstance);

	bool ret = false;
	{
		CPythonLauncher pyLauncher;
		CPythonExceptionSender pyExceptionSender;
		SetExceptionSender(&pyExceptionSender);

		if (pyLauncher.Create())
		{
			ret = RunMainScript(pyLauncher, lpCmdLine); // 게임 실행중엔 함수가 끝나지 않는다.
		}

		// ProcessScanner_ReleaseQuitEvent();

		// 게임 종료시.
		app->Clear();

		timeEndPeriod(1);
		pyLauncher.Clear();
	}

	app->Destroy();
	delete app;

	return ret;
}

HANDLE CreateMetin2GameMutex()
{
	SECURITY_ATTRIBUTES sa;
	ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = FALSE;

	return CreateMutex(&sa, FALSE, "Metin2GameMutex");
}

void DestroyMetin2GameMutex(HANDLE hMutex)
{
	if (hMutex)
	{
		ReleaseMutex(hMutex);
		hMutex = NULL;
	}
}

void __ErrorPythonLibraryIsNotExist()
{
	LogBoxf("FATAL ERROR!! Python Library file not exist!");
}

bool __IsTimeStampOption(LPSTR lpCmdLine)
{
	const char* TIMESTAMP = "/timestamp";
	return (strncmp(lpCmdLine, TIMESTAMP, strlen(TIMESTAMP)) == 0);
}

void __PrintTimeStamp()
{
#ifdef _DEBUG
	if (__IS_TEST_SERVER_MODE__)
		LogBoxf("METIN2 BINARY TEST DEBUG VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
	else
		LogBoxf("METIN2 BINARY DEBUG VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);

#else
	if (__IS_TEST_SERVER_MODE__)
		LogBoxf("METIN2 BINARY TEST VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
	else
		LogBoxf("METIN2 BINARY DISTRIBUTE VERSION %s ( MS C++ %d Compiled )", __TIMESTAMP__, _MSC_VER);
#endif
}

bool __IsLocaleOption(LPSTR lpCmdLine)
{
	return (strcmp(lpCmdLine, "--locale") == 0);
}

bool __IsLocaleVersion(LPSTR lpCmdLine)
{
	return (strcmp(lpCmdLine, "--perforce-revision") == 0);
}

#ifdef ENABLE_MD5_FILE_CHECK
void CheckMD5Filenames()
{
	MD5 md5;
	for (int i = 0; i < std::size(ar_szMD5FileNames); i++)
	{
		if (strcmp(md5.digestFile((char*)ar_szMD5FileNames[i][0]), ar_szMD5FileNames[i][1]))
		{
			char szBuf[512 + 1];
			snprintf(szBuf, sizeof(szBuf), "%s has been modified, please run the patcher.", ar_szMD5FileNames[i][0]);
			MessageBoxA(NULL, szBuf, ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
			exit(0);
		}
	}
}
#endif

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(ENABLE_MD5_FILE_CHECK) && !defined(__DEV_CLIENT__)
	CheckMD5Filenames();
#endif

	ApplicationStringTable_Initialize(hInstance);

	LocaleService_LoadConfig("locale.cfg");
	SetDefaultCodePage(LocaleService_GetCodePage());

#if defined(CHECK_LATEST_DATA_FILES)
	if (!CheckLatestFiles())
		return 0;
#endif

	bool bQuit = false;
	int nArgc = 0;
	PCHAR* szArgv = CommandLineToArgv(lpCmdLine, &nArgc);

	for (int i = 0; i < nArgc; i++)
	{
		if (szArgv[i] == 0)
			continue;

		if (__IsLocaleVersion(szArgv[i])) // #0000829: [M2EU] 버전 파일이 항상 생기지 않도록 수정 
		{
			char szModuleName[MAX_PATH];
			char szVersionPath[MAX_PATH];
			GetModuleFileName(NULL, szModuleName, sizeof(szModuleName));
			sprintf(szVersionPath, "%s.version", szModuleName);
			FILE* fp = fopen(szVersionPath, "wt");
			if (fp)
			{
				fprintf(fp, "r%d\n", VERSION);
				fclose(fp);
			}
			bQuit = true;
		}
		else if (__IsLocaleOption(szArgv[i]))
		{
			FILE* fp = fopen("locale.txt", "wt");
			fprintf(fp, "service[%s] code_page[%d]",
				LocaleService_GetName(), LocaleService_GetCodePage());
			fclose(fp);
			bQuit = true;
		}
		else if (__IsTimeStampOption(szArgv[i]))
		{
			__PrintTimeStamp();
			bQuit = true;
		}
		else if ((strcmp(szArgv[i], "--force-set-locale") == 0))
		{
			// locale 설정엔 인자가 두 개 더 필요함 (로케일 명칭, 데이터 경로)
			if (nArgc <= i + 2)
			{
				MessageBox(NULL, "Invalid arguments", ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
				goto Clean;
			}

			const char* localeName = szArgv[++i];
			const char* localePath = szArgv[++i];

			LocaleService_ForceSetLocale(localeName, localePath);
		}
	}

	if (bQuit)
		goto Clean;

#if defined(NEEDED_COMMAND_ARGUMENT)
	// 옵션이 없으면 비정상 실행으로 간주, 프로그램 종료
	if (strstr(lpCmdLine, NEEDED_COMMAND_ARGUMENT) == 0) {
		MessageBox(NULL, ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"), ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
		goto Clean;
	}
#endif

#if defined(NEEDED_COMMAND_CLIPBOARD)
	{
		CHAR szSecKey[256];
		CPostIt cPostIt("VOLUME1");

		if (cPostIt.Get("SEC_KEY", szSecKey, sizeof(szSecKey)) == FALSE) {
			MessageBox(NULL, ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"), ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
			goto Clean;
		}
		if (strstr(szSecKey, NEEDED_COMMAND_CLIPBOARD) == 0) {
			MessageBox(NULL, ApplicationStringTable_GetStringz(IDS_ERR_MUST_LAUNCH_FROM_PATCHER, "ERR_MUST_LAUNCH_FROM_PATCHER"), ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);
			goto Clean;
		}
		cPostIt.Empty();
	}
#endif

	CefWebBrowser_Startup(hInstance);

	if (!CheckPythonLibraryFilenames())
	{
		__ErrorPythonLibraryIsNotExist();
		goto Clean;
	}

	Main(hInstance, lpCmdLine);

	CefWebBrowser_Cleanup();

	::CoUninitialize();

	if (gs_szErrorString[0])
		MessageBox(NULL, gs_szErrorString, ApplicationStringTable_GetStringz(IDS_APP_NAME, "APP_NAME"), MB_ICONSTOP);

Clean:
	SAFE_FREE_GLOBAL(szArgv);

	return 0;
}

static void GrannyError(granny_log_message_type Type,
	granny_log_message_origin Origin,
	char const* File,
	granny_int32x Line,
	char const* Error,
	void* UserData)
{
	if (Origin == GrannyFileReadingLogMessage || Origin == GrannyControlLogMessage || Origin == GrannyMeshBindingLogMessage)
		return;

#ifdef _DEBUG
	TraceError("GRANNY: %s(%d): ERROR: %s --- [%d] %s --- [%d] %s", File, Line, Error, Type, GrannyGetLogMessageTypeString(Type), Origin, GrannyGetLogMessageOriginString(Origin));
#endif
}

int Setup(LPSTR lpCmdLine)
{
	/*
	* 타이머 정밀도를 올린다.
	*/
	TIMECAPS tc;
	UINT wTimerRes;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR)
		return 0;

	wTimerRes = MINMAX(tc.wPeriodMin, 1, tc.wPeriodMax);
	timeBeginPeriod(wTimerRes);

	/*
	* 그래니 에러 핸들링
	*/

	granny_log_callback Callback;
	Callback.Function = GrannyError;
	Callback.UserData = 0;
	GrannySetLogCallback(&Callback);
	return 1;
}
