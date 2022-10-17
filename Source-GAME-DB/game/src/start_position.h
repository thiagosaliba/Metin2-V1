#ifndef __INC_START_POSITION_H__
#define __INC_START_POSITION_H__

#include "locale_service.h"

extern char g_nation_name[4][32];
extern DWORD g_start_position[4][2];
extern long g_start_map[4];
extern DWORD g_create_position[4][2];
extern DWORD arena_return_position[4][2];
extern DWORD g_wolfman_create_position[4][2];

inline const char* EMPIRE_NAME(BYTE e)
{
	return LC_TEXT(g_nation_name[e]);
}

inline DWORD EMPIRE_START_MAP(BYTE e)
{
	return g_start_map[e];
}

inline DWORD EMPIRE_START_X(BYTE e)
{
	if (1 <= e && e <= 3)
		return g_start_position[e][0];

	return 0;
}

inline DWORD EMPIRE_START_Y(BYTE e)
{
	if (1 <= e && e <= 3)
		return g_start_position[e][1];

	return 0;
}

inline DWORD ARENA_RETURN_POINT_X(BYTE e)
{
	if (1 <= e && e <= 3)
		return arena_return_position[e][0];

	return 0;
}

inline DWORD ARENA_RETURN_POINT_Y(BYTE e)
{
	if (1 <= e && e <= 3)
		return arena_return_position[e][1];

	return 0;
}

inline DWORD CREATE_START_X(BYTE e, BYTE j)
{
	if (1 <= e && e <= 3)
	{
		return (j == JOB_WOLFMAN) ? g_wolfman_create_position[e][0] : g_create_position[e][0];
	}

	return 0;
}

inline DWORD CREATE_START_Y(BYTE e, BYTE j)
{
	if (1 <= e && e <= 3)
	{
		return (j == JOB_WOLFMAN) ? g_wolfman_create_position[e][1] : g_create_position[e][1];
	}

	return 0;
}

#endif // __INC_START_POSITION_H__
