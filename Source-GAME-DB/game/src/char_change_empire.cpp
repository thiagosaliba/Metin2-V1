#include "stdafx.h"
#include "config.h"
#include "char.h"
#include "char_manager.h"
#include "db.h"
#include "guild_manager.h"
#include "marriage.h"
#include "../../common/service.h"

/*
Return Value
	0 : �� �� ���� ���� or ���� ����
	1 : ������ �������� �ٲٷ�����
	2 : ��� ������ ĳ���Ͱ� ����
	3 : ��ȥ�� ĳ���Ͱ� ����

	999 : ���� �̵� ����
*/

int CHARACTER::ChangeEmpire(BYTE empire)
{
	if (GetEmpire() == empire)
		return 1;

	char szQuery[1024 + 1];
	DWORD dwAID;
	DWORD dwPID[PLAYER_PER_ACCOUNT];
	memset(dwPID, 0, sizeof(dwPID));

	{
		// 1. �� ������ ��� pid�� ��� �´�
		snprintf(szQuery, sizeof(szQuery), "SELECT "
			"id, "
			"pid1, "
			"pid2, "
			"pid3, "
			"pid4"
#if defined(__PLAYER_PER_ACCOUNT5__)
			", pid5"
#endif
			" FROM player_index%s WHERE "
			"pid1=%u OR "
			"pid2=%u OR "
			"pid3=%u OR "
			"pid4=%u"
#if defined(__PLAYER_PER_ACCOUNT5__)
			" OR pid5=%u"
#endif
			" AND empire=%u", get_table_postfix(),
			GetPlayerID(),
			GetPlayerID(),
			GetPlayerID(),
			GetPlayerID(),
#if defined(__PLAYER_PER_ACCOUNT5__)
			GetPlayerID(),
#endif
			GetEmpire()
		);

		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
		if (pMsg->Get()->uiNumRows == 0)
			return 0;

		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

		str_to_number(dwAID, row[0]);
		str_to_number(dwPID[0], row[1]);
		str_to_number(dwPID[1], row[2]);
		str_to_number(dwPID[2], row[3]);
		str_to_number(dwPID[3], row[4]);
#if defined(__PLAYER_PER_ACCOUNT5__)
		str_to_number(dwPID[4], row[5]);
#endif
	}

	const int loop = 4;

	{
		// 2. �� ĳ������ ��� ������ ���´�.
		// �� ĳ���Ͷ� ��忡 ���� �Ǿ� �ִٸ�, ���� �̵��� �� �� ����.
		DWORD dwGuildID[4];
		CGuild* pGuild[4];

		for (int i = 0; i < loop; ++i)
		{
			snprintf(szQuery, sizeof(szQuery), "SELECT guild_id FROM guild_member%s WHERE pid=%u", get_table_postfix(), dwPID[i]);
			std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));

			if (pMsg->Get()->uiNumRows > 0)
			{
				MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);

				str_to_number(dwGuildID[i], row[0]);

				pGuild[i] = CGuildManager::instance().FindGuild(dwGuildID[i]);

				if (pGuild[i] != NULL)
					return 2;
			}
			else
			{
				dwGuildID[i] = 0;
				pGuild[i] = NULL;
			}
		}
	}

	{
		// 3. �� ĳ������ ��ȥ ������ ���´�.
		// �� ĳ���Ͷ� ��ȥ ���¶�� ���� �̵��� �� �� ����.
		for (int i = 0; i < loop; ++i)
		{
			if (marriage::CManager::instance().IsEngagedOrMarried(dwPID[i]) == true)
				return 3;
		}
	}

	{
		// 4. db�� ���� ������ ������Ʈ �Ѵ�.
		snprintf(szQuery, sizeof(szQuery), "UPDATE player_index%s SET empire=%u WHERE "
			"pid1=%u OR "
			"pid2=%u OR "
			"pid3=%u OR "
			"pid4=%u"
#if defined(__PLAYER_PER_ACCOUNT5__)
			" OR pid5=%u"
#endif
			" AND empire=%u",
			get_table_postfix(),
			empire,
			GetPlayerID(),
			GetPlayerID(),
			GetPlayerID(),
			GetPlayerID(),
#if defined(__PLAYER_PER_ACCOUNT5__)
			GetPlayerID(),
#endif
			GetEmpire());

		std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));

		if (pMsg->Get()->uiAffectedRows > 0)
		{
			// 5. ���� ���� �̷��� �߰��Ѵ�.
			SetChangeEmpireCount();

			return 999;
		}
	}

	return 0;
}

int CHARACTER::GetChangeEmpireCount() const
{
	char szQuery[1024 + 1];
	DWORD dwAID = GetAID();

	if (dwAID == 0)
		return 0;

	snprintf(szQuery, sizeof(szQuery), "SELECT change_count FROM change_empire WHERE account_id = %u", dwAID);

	DWORD dwCount = 0;

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
	if (pMsg->Get()->uiNumRows > 0)
	{
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		str_to_number(dwCount, row[0]);
		return dwCount;
	}

	return dwCount;
}

void CHARACTER::SetChangeEmpireCount()
{
	char szQuery[1024 + 1];

	DWORD dwAID = GetAID();

	if (dwAID == 0) return;

	int count = GetChangeEmpireCount();

	if (count == 0)
	{
		count++;
		snprintf(szQuery, sizeof(szQuery), "INSERT INTO change_empire VALUES(%u, %d, NOW())", dwAID, count);
	}
	else
	{
		count++;
		snprintf(szQuery, sizeof(szQuery), "UPDATE change_empire SET change_count=%d WHERE account_id=%u", count, dwAID);
	}

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
}

DWORD CHARACTER::GetAID() const
{
	char szQuery[1024 + 1];
	DWORD dwAID = 0;

	snprintf(szQuery, sizeof(szQuery), "SELECT id FROM player_index%s WHERE "
		"pid1=%u OR "
		"pid2=%u OR "
		"pid3=%u OR "
		"pid4=%u"
#if defined(__PLAYER_PER_ACCOUNT5__)
		" OR pid5=%u"
#endif
		" AND empire=%u",
		get_table_postfix(),
		GetPlayerID(),
		GetPlayerID(),
		GetPlayerID(),
		GetPlayerID(),
#if defined(__PLAYER_PER_ACCOUNT5__)
		GetPlayerID(),
#endif
		GetEmpire());

	std::unique_ptr<SQLMsg> pMsg(DBManager::instance().DirectQuery(szQuery));
	if (pMsg->Get()->uiNumRows > 0)
	{
		MYSQL_ROW row = mysql_fetch_row(pMsg->Get()->pSQLResult);
		str_to_number(dwAID, row[0]);
		return dwAID;
	}

	return 0;
}
