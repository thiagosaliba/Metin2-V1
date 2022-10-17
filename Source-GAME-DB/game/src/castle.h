/**
* File : castle.h
* Date : 2007.04.07
* Author : mhh
* Description :
**/

#ifndef __INC_CASTLE_SIEGE_H__
#define __INC_CASTLE_SIEGE_H__

#define MAX_CASTLE_GUARD_REGION 4 // ��� ��ġ ����
#define MAX_CASTLE_GUARD_PER_REGION 10 // �������� ��ġ�Ҽ��ִ� ��񺴱׷�
#define MAX_CASTLE_FROG 20 // Ȳ�� �β���
#define MAX_CASTLE_TOWER 10 // ��ȭ �ִ� ��ȯ ����
#define MIN_CASTLE_TOWER 5 // ��ȭ �ּ� ��ȯ ����

enum CASTLE_MAP
{
	CASTLE_RED = 181,
	CASTLE_YELLOW = 183,
	CASTLE_BLUE = 182
};

#define CASTLE_FROG_PRICE 100000000 // Ȳ�ݵβ��� ���� (1��)
#define CASTLE_FROG_VNUM 11505 // Ȳ�ݵβ��� ��ȣ
//#define CASTLE_TOWER_VNUM 11506 // ��ȭ ��ȣ
#define IS_CASTLE_MAP(map) (CASTLE_RED == (map) || CASTLE_YELLOW == (map) || CASTLE_BLUE == (map))
//#define IS_CASTLE_TOWER(vnum) (11506 == (vnum) || 11507 == (vnum) || 11508 == (vnum) || 11509 == (vnum) || 11510 == (vnum))

enum CASTLE_STATE
{
	CASTLE_SIEGE_NONE, // ��ȭ���
	CASTLE_SIEGE_STRUGGLE, // ������
	CASTLE_SIEGE_END // ������ �����ߴٸ� 1�ð����� ��ȭ�� �� �� �ִ�.
};

struct CASTLE_DATA
{
	LPCHARACTER frog[MAX_CASTLE_FROG]; // Ȳ�ݵβ���

	LPCHARACTER guard[MAX_CASTLE_GUARD_REGION][MAX_CASTLE_GUARD_PER_REGION]; // ��� ����
	DWORD guard_group[MAX_CASTLE_GUARD_REGION][MAX_CASTLE_GUARD_PER_REGION]; // ��� ����

	LPCHARACTER tower[MAX_CASTLE_TOWER]; // ��ȭ

	LPEVENT siege_event;
	LPEVENT stone_event;
};

/* extern functions */
bool castle_boot();
void castle_save();
int castle_siege(int empire, int tower_count);
void castle_start_siege(int empire, int tower_count);
void castle_end_siege();

LPCHARACTER castle_spawn_frog(int empire);
LPCHARACTER castle_spawn_guard(int empire, DWORD group_vnum, int region_index);
bool castle_spawn_tower(int empire, int tower_count);

void castle_guard_die(LPCHARACTER ch, LPCHARACTER killer);
void castle_frog_die(LPCHARACTER ch, LPCHARACTER killer);
void castle_tower_die(LPCHARACTER ch, LPCHARACTER killer);

int castle_guard_count(int empire, int region_index);
int castle_frog_count(int empire);

bool castle_is_guard_vnum(DWORD vnum);
int castle_cost_of_hiring_guard(DWORD vnum);
bool castle_can_attack(LPCHARACTER ch, LPCHARACTER victim);

bool castle_frog_to_empire_money(LPCHARACTER ch);
bool castle_is_my_castle(int empire, int map_index);
bool castle_is_tower_vnum(DWORD vnum);

#endif // __INC_CASTLE_SIEGE_H__
