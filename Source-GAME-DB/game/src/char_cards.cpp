#include "stdafx.h"
#include "char.h"
#include "utils.h"
#include "log.h"
#include "db.h"
#include "dev_log.h"
#include "locale_service.h"
#include <stdlib.h>
#include <sstream>
#include "config.h"
#include "desc.h"
#include "desc_manager.h"
#include "char_manager.h"
#include "item.h"
#include "item_manager.h"
#include "buffer_manager.h"
#include "packet.h"
#include "questmanager.h"
#include "monarch.h"
#include "mob_manager.h"
#include "desc_client.h"
#include "group_text_parse_tree.h"
#include <boost/algorithm/string/predicate.hpp>
#include <cctype>
#include "p2p.h"
#include "entity.h"
#include "sectree_manager.h"

void CHARACTER::Cards_open(DWORD safemode)
{
	if (PreventTradeWindow(WND_ALL))
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("Close all other trade windows first."));
		return;
	}

	if (character_cards.cards_left <= 0)
	{
		if (GetGold() < 30000)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have enough gold."));
			return;
		}

		if (CountSpecifyItem(79506) < 1)
		{
			ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have any card sets."));
			return;
		}

		PointChange(POINT_GOLD, -30000, true);
		RemoveSpecifyItem(79506, 1);
		Cards_clean_list();
		character_cards.cards_left = 24;
	}

	ChatPacket(CHAT_TYPE_COMMAND, "cards open %d", safemode);
	SendUpdatedInformations();
}

void CHARACTER::Cards_clean_list()
{
	memset(&character_cards, 0, sizeof(character_cards));
	memset(&randomized_cards, 0, sizeof(randomized_cards));
}

DWORD CHARACTER::GetEmptySpaceInHand()
{
	for (int i = 0; i < MAX_CARDS_IN_HAND; ++i)
	{
		if (character_cards.cards_in_hand[i].type == 0)
			return i;
	}

	return -1;
}

void CHARACTER::Cards_pullout()
{
	DWORD empty_space = GetEmptySpaceInHand();
	unsigned int i = -1;

	if (empty_space == i)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have anymore space in your hands."));
		return;
	}

	if (character_cards.cards_left < 1)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have any cards."));
		return;
	}

	if (GetAllCardsCount() >= 5)
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("You don't have space on the table."));
		return;
	}

	RandomizeCards();
	SendUpdatedInformations();
}

void CHARACTER::RandomizeCards()
{
	DWORD card_type = number(1, 3);
	DWORD card_value = number(1, 8);

	if (CardWasRandomized(card_type, card_value) != false)
	{
		RandomizeCards();
	}
	else
	{
		DWORD empty_space = GetEmptySpaceInHand();
		character_cards.cards_in_hand[empty_space].type = card_type;
		character_cards.cards_in_hand[empty_space].value = card_value;
		character_cards.cards_left -= 1;
	}
}

bool CHARACTER::CardWasRandomized(DWORD type, DWORD value)
{
	for (int i = 0; i < 24; ++i)
	{
		if (randomized_cards[i].type == type && randomized_cards[i].value == value)
		{
			return true;
		}
	}

	for (int i = 0; i < 24; ++i)
	{
		if (randomized_cards[i].type == 0)
		{
			randomized_cards[i].type = type;
			randomized_cards[i].value = value;
			return false;
		}
	}

	return false;
}

void CHARACTER::SendUpdatedInformations()
{
	ChatPacket(CHAT_TYPE_COMMAND, "cards info %d %d %d %d %d %d %d %d %d %d %d %d", character_cards.cards_in_hand[0].type, character_cards.cards_in_hand[0].value,
		character_cards.cards_in_hand[1].type, character_cards.cards_in_hand[1].value, character_cards.cards_in_hand[2].type,
		character_cards.cards_in_hand[2].value, character_cards.cards_in_hand[3].type, character_cards.cards_in_hand[3].value,
		character_cards.cards_in_hand[4].type, character_cards.cards_in_hand[4].value,
		character_cards.cards_left, character_cards.points);

	ChatPacket(CHAT_TYPE_COMMAND, "cards finfo %d %d %d %d %d %d %d", character_cards.cards_in_field[0].type, character_cards.cards_in_field[0].value, character_cards.cards_in_field[1].type,
		character_cards.cards_in_field[1].value, character_cards.cards_in_field[2].type, character_cards.cards_in_field[2].value,
		character_cards.field_points);
}

void CHARACTER::SendReward()
{
	ChatPacket(CHAT_TYPE_COMMAND, "cards reward %d %d %d %d %d %d %d", character_cards.cards_in_field[0].type, character_cards.cards_in_field[0].value, character_cards.cards_in_field[1].type,
		character_cards.cards_in_field[1].value, character_cards.cards_in_field[2].type, character_cards.cards_in_field[2].value,
		character_cards.field_points);
}

void CHARACTER::CardsDestroy(DWORD reject_index)
{
	if (reject_index + 1 > MAX_CARDS_IN_HAND)
		return;

	if (character_cards.cards_in_hand[reject_index].type == 0)
		return;

	character_cards.cards_in_hand[reject_index].type = 0;
	character_cards.cards_in_hand[reject_index].value = 0;
	SendUpdatedInformations();
}

void CHARACTER::CardsAccept(DWORD accept_index)
{
	if (accept_index + 1 > MAX_CARDS_IN_HAND)
		return;

	if (character_cards.cards_in_hand[accept_index].type == 0)
		return;

	DWORD empty_space = GetEmptySpaceInField();
	unsigned int i = -1;

	if (empty_space != i)
	{
		character_cards.cards_in_field[empty_space].type = character_cards.cards_in_hand[accept_index].type;
		character_cards.cards_in_field[empty_space].value = character_cards.cards_in_hand[accept_index].value;
		character_cards.cards_in_hand[accept_index].type = 0;
		character_cards.cards_in_hand[accept_index].value = 0;
	}

	if (GetEmptySpaceInField() == i)
	{
		if (CheckReward())
		{
			SendReward();
			ResetField();
		}
		else
		{
			RestoreField();
		}
	}

	SendUpdatedInformations();
}

void CHARACTER::CardsRestore(DWORD restore_index)
{
	if (restore_index + 1 > MAX_CARDS_IN_FIELD)
		return;

	if (character_cards.cards_in_field[restore_index].type == 0)
		return;

	DWORD empty_space = GetEmptySpaceInHand();

	character_cards.cards_in_hand[empty_space].type = character_cards.cards_in_field[restore_index].type;
	character_cards.cards_in_hand[empty_space].value = character_cards.cards_in_field[restore_index].value;
	character_cards.cards_in_field[restore_index].type = 0;
	character_cards.cards_in_field[restore_index].value = 0;

	SendUpdatedInformations();
}

DWORD CHARACTER::GetEmptySpaceInField()
{
	for (int i = 0; i < MAX_CARDS_IN_FIELD; ++i)
	{
		if (character_cards.cards_in_field[i].type == 0)
			return i;
	}

	return -1;
}

DWORD CHARACTER::GetAllCardsCount()
{
	DWORD count = 0;

	for (int i = 0; i < MAX_CARDS_IN_HAND; ++i)
	{
		if (character_cards.cards_in_hand[i].type != 0)
			count += 1;
	}

	for (int i = 0; i < MAX_CARDS_IN_FIELD; ++i)
	{
		if (character_cards.cards_in_field[i].type != 0)
			count += 1;
	}

	return count;
}

bool CHARACTER::TypesAreSame()
{
	if (character_cards.cards_in_field[0].type == character_cards.cards_in_field[1].type && character_cards.cards_in_field[1].type == character_cards.cards_in_field[2].type)
		return true;

	return false;
}

bool CHARACTER::ValuesAreSame()
{
	if (character_cards.cards_in_field[0].value == character_cards.cards_in_field[1].value && character_cards.cards_in_field[1].value == character_cards.cards_in_field[2].value)
		return true;

	return false;
}

bool CHARACTER::CardsMatch()
{
	if (character_cards.cards_in_field[0].value == character_cards.cards_in_field[1].value - 1 && character_cards.cards_in_field[1].value == character_cards.cards_in_field[2].value - 1)
		return true;
	else if (character_cards.cards_in_field[0].value == character_cards.cards_in_field[2].value - 1 && character_cards.cards_in_field[2].value == character_cards.cards_in_field[1].value - 1)
		return true;
	else if (character_cards.cards_in_field[1].value == character_cards.cards_in_field[0].value - 1 && character_cards.cards_in_field[0].value == character_cards.cards_in_field[2].value - 1)
		return true;
	else if (character_cards.cards_in_field[0].value == character_cards.cards_in_field[1].value - 1 && character_cards.cards_in_field[2].value == character_cards.cards_in_field[0].value - 1)
		return true;
	else if (character_cards.cards_in_field[1].value == character_cards.cards_in_field[0].value - 1 && character_cards.cards_in_field[2].value == character_cards.cards_in_field[1].value - 1)
		return true;
	else if (character_cards.cards_in_field[1].value == character_cards.cards_in_field[2].value - 1 && character_cards.cards_in_field[2].value == character_cards.cards_in_field[0].value - 1)
		return true;
	else
		return false;
}

DWORD CHARACTER::GetLowestCard()
{
	return MIN(character_cards.cards_in_field[0].value, MIN(character_cards.cards_in_field[1].value, character_cards.cards_in_field[2].value));
}

bool CHARACTER::CheckReward()
{
	if (TypesAreSame() && ValuesAreSame())
	{
		character_cards.field_points = 150;
		character_cards.points += 150;
		return true;
	}
	else if (TypesAreSame() && CardsMatch())
	{
		character_cards.field_points = 100;
		character_cards.points += 100;
		return true;
	}
	else if (ValuesAreSame())
	{
		character_cards.field_points = GetLowestCard() * 10 + 10;
		character_cards.points += GetLowestCard() * 10 + 10;
		return true;
	}
	else if (CardsMatch())
	{
		character_cards.field_points = GetLowestCard() * 10;
		character_cards.points += GetLowestCard() * 10;
		return true;
	}
	else
	{
		RestoreField();
		return false;
	}
}

void CHARACTER::RestoreField()
{
	for (int i = 0; i < MAX_CARDS_IN_FIELD; ++i)
	{
		DWORD empty_space = GetEmptySpaceInHand();

		character_cards.cards_in_hand[empty_space].type = character_cards.cards_in_field[i].type;
		character_cards.cards_in_hand[empty_space].value = character_cards.cards_in_field[i].value;
		character_cards.cards_in_field[i].type = 0;
		character_cards.cards_in_field[i].value = 0;

		SendUpdatedInformations();
	}
}

void CHARACTER::ResetField()
{
	for (int i = 0; i < MAX_CARDS_IN_FIELD; ++i)
	{
		character_cards.cards_in_field[i].type = 0;
		character_cards.cards_in_field[i].value = 0;
	}
}

void CHARACTER::CardsEnd()
{
	if (character_cards.points >= 400)
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		AutoGiveItem(50267, 1, -1, true, true);
#else
		AutoGiveItem(50267, 1, -1, true);
#endif
	else if (character_cards.points >= 300 && character_cards.points < 400)
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		AutoGiveItem(50268, 1, -1, true, true);
#else
		AutoGiveItem(50268, 1, -1, true);
#endif
	else if (character_cards.points < 300)
#if defined(__WJ_PICKUP_ITEM_EFFECT__)
		AutoGiveItem(50269, 1, -1, true, true);
#else
		AutoGiveItem(50269, 1, -1, true);
#endif

	//LogManager::instance().OkeyEventLog(GetPlayerID(), GetName(), character_cards.points);
	DBManager::instance().DirectQuery("INSERT INTO okey_event%s (pid, name, points) VALUES (%d, '%s', %d)", get_table_postfix(), GetPlayerID(), GetName(), character_cards.points);
	Cards_clean_list();
	SendUpdatedInformations();
}

void CHARACTER::GetGlobalRank(char* buffer, size_t buflen)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery("SELECT name, SUM(points) as spoints FROM okey_event%s GROUP BY pid ORDER BY spoints DESC LIMIT 10", get_table_postfix()));

	if (pMsg->Get()->uiNumRows == 0)
	{
		return;
	}

	MYSQL_ROW row;
	int pos = 1;
	int len = 0, len2;
	*buffer = '\0';

	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		int points = 0;
		str_to_number(points, row[1]);
		len2 = snprintf(buffer + len, buflen - len, "[ENTER]");

		if (len2 < 0 || len2 >= (int)buflen - len)
			len += (buflen - len) - 1;
		else
			len += len2;

		len2 = snprintf(buffer + len, buflen - len, LC_TEXT("%d. Place - %s - %d points."), pos, row[0], points);
		++pos;

		if (len2 < 0 || len2 >= (int)buflen - len)
			len += (buflen - len) - 1;
		else
			len += len2;
	}
}

void CHARACTER::GetRundRank(char* buffer, size_t buflen)
{
	std::unique_ptr<SQLMsg> pMsg(DBManager::Instance().DirectQuery("SELECT name, points FROM okey_event%s ORDER BY points DESC LIMIT 10", get_table_postfix()));

	if (pMsg->Get()->uiNumRows == 0)
	{
		return;
	}

	MYSQL_ROW row;
	int pos = 1;
	int len = 0, len2;
	*buffer = '\0';

	while (NULL != (row = mysql_fetch_row(pMsg->Get()->pSQLResult)))
	{
		int points = 0;
		str_to_number(points, row[1]);
		len2 = snprintf(buffer + len, buflen - len, "[ENTER]");

		if (len2 < 0 || len2 >= (int)buflen - len)
			len += (buflen - len) - 1;
		else
			len += len2;

		len2 = snprintf(buffer + len, buflen - len, LC_TEXT("%d. Place - %s - %d points."), pos, row[0], points);
		++pos;

		if (len2 < 0 || len2 >= (int)buflen - len)
			len += (buflen - len) - 1;
		else
			len += len2;
	}
}
