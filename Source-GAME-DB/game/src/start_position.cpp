#include "stdafx.h"
#include "start_position.h"

char g_nation_name[4][32] =
{
	"",
	"�ż���", // Shinsoo
	"õ����", // Chunjo
	"���뱹", // Jinno
};

// LC_TEXT("�ż���")
// LC_TEXT("õ����")
// LC_TEXT("���뱹")

long g_start_map[4] =
{
	0, // reserved
	1, // �ż���
	21, // õ����
	41 // ���뱹
};

DWORD g_start_position[4][2] =
{
	{ 0, 0 }, // reserved
	{ 475000, 966100 }, // �ż���
	{ 60000, 156000 }, // õ����
	{ 963400, 278200 } // ���뱹
};

DWORD arena_return_position[4][2] =
{
	{ 0, 0 },
	{ 347600, 882700 }, // �ھ���
	{ 138600, 236600 }, // ������
	{ 857200, 251800 } // �ڶ���
};

DWORD g_create_position[4][2] =
{
	{ 0, 0 }, // reserved,
	{ 475000, 966100 }, // �ż���
	{ 60000, 156000 }, // õ����
	{ 963400, 278200 } // ���뱹
};

DWORD g_wolfman_create_position[4][2] =
{
	{ 0, 0 }, // reserved,
	{ 808500, 936000 }, // �ż���
	{ 859700, 936000 }, // õ����
	{ 910900, 936000 }, // ���뱹
};
