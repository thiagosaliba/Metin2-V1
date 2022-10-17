#include "stdafx.h"
#include "questlua.h"
#include "questmanager.h"
#include "desc_client.h"
#include "char.h"
#include "item_manager.h"
#include "item.h"
#include "cmd.h"
#include "packet.h"
#include "utils.h"
#include "config.h"

#if defined(__DICE_SYSTEM__)
#include "party.h"
#endif

#if defined(__MINI_GAME_CATCH_KING__)
#include "minigame.h"
#endif

#undef sys_err
#ifndef __WIN32__
#define sys_err(fmt, args...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, ##args)
#else
#define sys_err(fmt, ...) quest::CQuestManager::instance().QuestError(__FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#endif

extern ACMD(do_in_game_mall);

namespace quest
{
	int game_set_event_flag(lua_State* L)
	{
		CQuestManager& q = CQuestManager::instance();

		if (lua_isstring(L, 1) && lua_isnumber(L, 2))
			q.RequestSetEventFlag(lua_tostring(L, 1), (int)lua_tonumber(L, 2));

		return 0;
	}

	int game_get_event_flag(lua_State* L)
	{
		CQuestManager& q = CQuestManager::instance();

		if (lua_isstring(L, 1))
			lua_pushnumber(L, q.GetEventFlag(lua_tostring(L, 1)));
		else
			lua_pushnumber(L, 0);

		return 1;
	}

	int game_request_make_guild(lua_State* L)
	{
		CQuestManager& q = CQuestManager::instance();
		LPDESC d = q.GetCurrentCharacterPtr()->GetDesc();
		if (d)
		{
			BYTE header = HEADER_GC_REQUEST_MAKE_GUILD;
			d->Packet(&header, 1);
		}
		return 0;
	}

	int game_get_safebox_level(lua_State* L)
	{
		CQuestManager& q = CQuestManager::instance();
		lua_pushnumber(L, q.GetCurrentCharacterPtr()->GetSafeboxSize() / SAFEBOX_PAGE_SIZE);
		return 1;
	}

	int game_set_safebox_level(lua_State* L)
	{
		CQuestManager& q = CQuestManager::instance();

		//q.GetCurrentCharacterPtr()->ChangeSafeboxSize(3*(int)lua_tonumber(L,-1));
		TSafeboxChangeSizePacket p;
		p.dwID = q.GetCurrentCharacterPtr()->GetDesc()->GetAccountTable().id;
		p.bSize = (int)lua_tonumber(L, -1);
		db_clientdesc->DBPacket(HEADER_GD_SAFEBOX_CHANGE_SIZE, q.GetCurrentCharacterPtr()->GetDesc()->GetHandle(), &p, sizeof(p));

		q.GetCurrentCharacterPtr()->SetSafeboxSize(SAFEBOX_PAGE_SIZE * (int)lua_tonumber(L, -1));
		return 0;
	}

	int game_open_safebox(lua_State* /*L*/)
	{
		CQuestManager& q = CQuestManager::instance();
		LPCHARACTER ch = q.GetCurrentCharacterPtr();
		ch->SetSafeboxOpenPosition();
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeSafeboxPassword");
		return 0;
	}

	int game_open_mall(lua_State* /*L*/)
	{
		CQuestManager& q = CQuestManager::instance();
		LPCHARACTER ch = q.GetCurrentCharacterPtr();
		ch->SetSafeboxOpenPosition();
		ch->ChatPacket(CHAT_TYPE_COMMAND, "ShowMeMallPassword");
		return 0;
	}

	int game_drop_item(lua_State* L)
	{
		//
		// Syntax: game.drop_item(50050, 1)
		//
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		DWORD item_vnum = (DWORD)lua_tonumber(L, 1);
		int count = (int)lua_tonumber(L, 2);
		long x = ch->GetX();
		long y = ch->GetY();

		LPITEM item = ITEM_MANAGER::instance().CreateItem(item_vnum, count);

		if (!item)
		{
			sys_err("cannot create item vnum %d count %d", item_vnum, count);
			return 0;
		}

		PIXEL_POSITION pos;
		pos.x = x + number(-200, 200);
		pos.y = y + number(-200, 200);

		item->AddToGround(ch->GetMapIndex(), pos);
		item->StartDestroyEvent();

		return 0;
	}

	int game_drop_item_with_ownership(lua_State* L)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		LPITEM item = NULL;
		switch (lua_gettop(L))
		{
		case 1:
			item = ITEM_MANAGER::instance().CreateItem((DWORD)lua_tonumber(L, 1));
			break;
		case 2:
		case 3:
			item = ITEM_MANAGER::instance().CreateItem((DWORD)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));
			break;
		default:
			return 0;
		}

		if (item == NULL)
		{
			return 0;
		}

		if (lua_isnumber(L, 3))
		{
			int sec = (int)lua_tonumber(L, 3);
			if (sec <= 0)
			{
				item->SetOwnership(ch);
			}
			else
			{
				item->SetOwnership(ch, sec);
			}
		}
		else
			item->SetOwnership(ch);

		PIXEL_POSITION pos;
		pos.x = ch->GetX() + number(-200, 200);
		pos.y = ch->GetY() + number(-200, 200);

		item->AddToGround(ch->GetMapIndex(), pos);
		item->StartDestroyEvent();

		return 0;
	}

#if defined(__DICE_SYSTEM__)
	int game_drop_item_with_ownership_and_dice(lua_State* L)
	{
		LPITEM item = NULL;
		switch (lua_gettop(L))
		{
		case 1:
			item = ITEM_MANAGER::instance().CreateItem((DWORD)lua_tonumber(L, 1));
			break;
		case 2:
		case 3:
			item = ITEM_MANAGER::instance().CreateItem((DWORD)lua_tonumber(L, 1), (int)lua_tonumber(L, 2));
			break;
		default:
			return 0;
		}

		if (item == NULL)
		{
			return 0;
		}

		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();
		if (ch->GetParty())
		{
			FPartyDropDiceRoll f(item, ch);
			f.Process(NULL);
		}

		if (lua_isnumber(L, 3))
		{
			int sec = (int)lua_tonumber(L, 3);
			if (sec <= 0)
			{
				item->SetOwnership(ch);
			}
			else
			{
				item->SetOwnership(ch, sec);
			}
		}
		else
			item->SetOwnership(ch);

		PIXEL_POSITION pos;
		pos.x = ch->GetX() + number(-200, 200);
		pos.y = ch->GetY() + number(-200, 200);

		item->AddToGround(ch->GetMapIndex(), pos);
		item->StartDestroyEvent();

		return 0;
	}
#endif

	int game_web_mall(lua_State* L)
	{
		LPCHARACTER ch = CQuestManager::instance().GetCurrentCharacterPtr();

		if (ch != NULL)
		{
			do_in_game_mall(ch, const_cast<char*>(""), 0, 0);
		}
		return 0;
	}

#if defined(__GEM_MARKET_SYSTEM__)
	int game_open_gem_c(lua_State*)
	{
		CQuestManager& q = CQuestManager::instance();
		LPCHARACTER ch = q.GetCurrentCharacterPtr();
		ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenGuiGem");
		return 0;
	}

	int game_open_gem_m(lua_State*)
	{
		CQuestManager& q = CQuestManager::instance();
		LPCHARACTER ch = q.GetCurrentCharacterPtr();

		if (ch->CheckItemsFull() == false)
		{
			//ch->SetGemState("gem_refresh_time", init_gemTime() + (60*60*5));
			ch->UpdateItemsGemMarker();
			ch->InfoGemMarker();
			ch->StartCheckTimeMarket();
		}
		else
		{
			ch->InfoGemMarker();
			ch->StartCheckTimeMarket();
		}

		ch->ChatPacket(CHAT_TYPE_COMMAND, "OpenGuiGemMarket");

		return 0;
	}
#endif

#if defined(__MINI_GAME_CATCH_KING__)
	int mini_game_catch_king_get_score(lua_State* L)
	{
		DWORD dwArg = (DWORD)lua_tonumber(L, 1);
		bool isTotal = dwArg ? true : false;

		CMiniGameManager::instance().MiniGameCatchKingGetScore(L, isTotal);
		return 1;
	}
#endif

	int game_get_config(lua_State* L)
	{
		if (lua_isstring(L, 1))
		{
			bool bValue = 0;
			const char* szGameConfig = lua_tostring(L, 1);

			/* config_init */
			if (strcmp(szGameConfig, "create_with_full_set") == 0)
				bValue = g_bCreateWithFullSet;

			lua_pushnumber(L, bValue);
		}
		else
			lua_pushnumber(L, 0);

		return 1;
	}

#if defined(__MAILBOX__)
	int game_open_mailbox(lua_State* L)
	{
		CMailBox::Open(CQuestManager::instance().GetCurrentCharacterPtr());
		return 0;
	}

	int game_send_gm_mail(lua_State* L)
	{
		if (false == (lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3)
			&& lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6) && lua_isnumber(L, 7)))
		{
			sys_err("Wrong argument.");
			return 0;
		}

		const char* const cPlayerName = lua_tostring(L, 1);
		const char* const cTitle = lua_tostring(L, 2);
		const char* const cMessage = lua_tostring(L, 3);
		const DWORD dwItemVnum = static_cast<DWORD>(lua_tonumber(L, 4));
		const DWORD dwItemCount = static_cast<DWORD>(lua_tonumber(L, 5));
		const int iYang = static_cast<int>(lua_tonumber(L, 6));
		const int iWon = static_cast<int>(lua_tonumber(L, 7));

		CMailBox::SendGMMail(cPlayerName, cTitle, cMessage, dwItemVnum, dwItemCount, iYang, iWon);
		return 0;
	}
#endif

#if defined(__CHANGE_LOOK_SYSTEM__)
	int game_open_changelook(lua_State* L)
	{
		if (lua_isnumber(L, 1))
		{
			const LPCHARACTER c_lpCh = CQuestManager::instance().GetCurrentCharacterPtr();
			const BYTE c_bType = lua_tonumber(L, 1);
			CChangeLook::Open(c_lpCh, c_bType);
		}
		return 0;
	}
#endif

	void RegisterGameFunctionTable()
	{
		luaL_reg game_functions[] =
		{
			{ "get_safebox_level", game_get_safebox_level },
			{ "request_make_guild", game_request_make_guild },
			{ "set_safebox_level", game_set_safebox_level },
			{ "open_safebox", game_open_safebox },
			{ "open_mall", game_open_mall },
			{ "get_event_flag", game_get_event_flag },
			{ "set_event_flag", game_set_event_flag },
			{ "drop_item", game_drop_item },
			{ "drop_item_with_ownership", game_drop_item_with_ownership },
			{ "open_web_mall", game_web_mall },
#if defined(__DICE_SYSTEM__)
			{ "drop_item_with_ownership_and_dice", game_drop_item_with_ownership_and_dice },
#endif
#if defined(__GEM_MARKET_SYSTEM__)
			{ "open_gem", game_open_gem_c },
			{ "open_gem_market", game_open_gem_m },
#endif
#if defined(__MINI_GAME_CATCH_KING__)
			{ "mini_game_catch_king_get_score", mini_game_catch_king_get_score },
#endif
			{ "get_config", game_get_config },
#if defined(__MAILBOX__)
			{ "open_mailbox", game_open_mailbox },
			{ "send_gm_mail", game_send_gm_mail },
#endif
#if defined(__CHANGE_LOOK_SYSTEM__)
			{ "open_changelook", game_open_changelook },
#endif
			{ NULL, NULL }
		};

		CQuestManager::instance().AddLuaFunctionTable("game", game_functions);
	}
}
