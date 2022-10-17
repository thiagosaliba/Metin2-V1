﻿#include "stdafx.h"
#include "StringCodec_Vietnamese.h"

#pragma warning(disable: 4310) // char 짤림 경고 무시

static wchar_t cp1258_to_unicode[256] = {
	0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
	0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e, 0x000f,
	0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
	0x0018, 0x0019, 0x001a, 0x001b, 0x001c, 0x001d, 0x001e, 0x001f,
	0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
	0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
	0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
	0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
	0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
	0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
	0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
	0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
	0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
	0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
	0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x007f,
	0x20ac, 0x0081, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021, //0x80
	0x02c6, 0x2030, 0x008a, 0x2039, 0x0152, 0x008d, 0x008e, 0x008f, //
	0x0090, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014, //0x90
	0x02dc, 0x2122, 0x009a, 0x203a, 0x0153, 0x009d, 0x009e, 0x0178, //
	0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7, //0xa0
	0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af, //
	0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7, //0xb0
	0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf, //
	0x00c0, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x00c5, 0x00c6, 0x00c7, //0xc0
	0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x0300, 0x00cd, 0x00ce, 0x00cf, //
	0x0110, 0x00d1, 0x0309, 0x00d3, 0x00d4, 0x01a0, 0x00d6, 0x00d7, //0xd0
	0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x01af, 0x0303, 0x00df, //
	0x00e0, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x00e5, 0x00e6, 0x00e7, //0xe0
	0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x0301, 0x00ed, 0x00ee, 0x00ef, //
	0x0111, 0x00f1, 0x0323, 0x00f3, 0x00f4, 0x01a1, 0x00f6, 0x00f7, //0xf0
	0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x01b0, 0x20ab, 0x00ff, //
};

static wchar_t cp1258_composed_table[][5] = {
	{ 0x00c1, 0x00c0, 0x1ea2, 0x00c3, 0x1ea0 },
	{ 0x00e1, 0x00e0, 0x1ea3, 0x00e3, 0x1ea1 },
	{ 0x1eae, 0x1eb0, 0x1eb2, 0x1eb4, 0x1eb6 },
	{ 0x1eaf, 0x1eb1, 0x1eb3, 0x1eb5, 0x1eb7 },
	{ 0x1ea4, 0x1ea6, 0x1ea8, 0x1eaa, 0x1eac },
	{ 0x1ea5, 0x1ea7, 0x1ea9, 0x1eab, 0x1ead },
	{ 0x00c9, 0x00c8, 0x1eba, 0x1ebc, 0x1eb8 },
	{ 0x00e9, 0x00e8, 0x1ebb, 0x1ebd, 0x1eb9 },
	{ 0x1ebe, 0x1ec0, 0x1ec2, 0x1ec4, 0x1ec6 },
	{ 0x1ebf, 0x1ec1, 0x1ec3, 0x1ec5, 0x1ec7 },
	{ 0x00cd, 0x00cc, 0x1ec8, 0x0128, 0x1eca },
	{ 0x00ed, 0x00ec, 0x1ec9, 0x0129, 0x1ecb },
	{ 0x00d3, 0x00d2, 0x1ece, 0x00d5, 0x1ecc },
	{ 0x00f3, 0x00f2, 0x1ecf, 0x00f5, 0x1ecd },
	{ 0x1ed0, 0x1ed2, 0x1ed4, 0x1ed6, 0x1ed8 },
	{ 0x1ed1, 0x1ed3, 0x1ed5, 0x1ed7, 0x1ed9 },
	{ 0x1eda, 0x1edc, 0x1ede, 0x1ee0, 0x1ee2 },
	{ 0x1edb, 0x1edd, 0x1edf, 0x1ee1, 0x1ee3 },
	{ 0x00da, 0x00d9, 0x1ee6, 0x0168, 0x1ee4 },
	{ 0x00fa, 0x00f9, 0x1ee7, 0x0169, 0x1ee5 },
	{ 0x1ee8, 0x1eea, 0x1eec, 0x1eee, 0x1ef0 },
	{ 0x1ee9, 0x1eeb, 0x1eed, 0x1eef, 0x1ef1 },
	{ 0x00dd, 0x1ef2, 0x1ef6, 0x1ef8, 0x1ef4 },
	{ 0x00fd, 0x1ef3, 0x1ef7, 0x1ef9, 0x1ef5 },
};

static bool IsTone(wchar_t tone)
{
	switch (tone)
	{
	case 0x0300:
	case 0x0301:
	case 0x0309:
	case 0x0303:
	case 0x0323:
		return true;
	default:
		return false;
	}
}

static wchar_t ComposeTone(wchar_t prev, wchar_t tone)
{
	int col, row;

	switch (tone)
	{
	case 0x0301: col = 0; break;
	case 0x0300: col = 1; break;
	case 0x0309: col = 2; break;
	case 0x0303: col = 3; break;
	case 0x0323: col = 4; break;
	default:
		return prev;
	}

	switch (prev)
	{
	case 0x0041: row = 0; break;
	case 0x0061: row = 1; break;
	case 0x0102: row = 2; break;
	case 0x0103: row = 3; break;
	case 0x00C2: row = 4; break;
	case 0x00E2: row = 5; break;
	case 0x0045: row = 6; break;
	case 0x0065: row = 7; break;
	case 0x00CA: row = 8; break;
	case 0x00EA: row = 9; break;
	case 0x0049: row = 10; break;
	case 0x0069: row = 11; break;
	case 0x004F: row = 12; break;
	case 0x006F: row = 13; break;
	case 0x00D4: row = 14; break;
	case 0x00F4: row = 15; break;
	case 0x01A0: row = 16; break;
	case 0x01A1: row = 17; break;
	case 0x0055: row = 18; break;
	case 0x0075: row = 19; break;
	case 0x01AF: row = 20; break;
	case 0x01B0: row = 21; break;
	case 0x0059: row = 22; break;
	case 0x0079: row = 23; break;
	default:
		return prev;
	}

	return cp1258_composed_table[row][col];
}

int EL_String_Decode_Vietnamese(const char* multi, int multiLen, wchar_t* wide, int wideLen)
{
	if (multiLen < 0)
		multiLen = (int)strlen(multi) + 1;

	int src = 0;
	int dest = 0;

	if (multiLen > 0)
	{
		/* 첫글자는 무조건 변경 */
		wchar_t prev = cp1258_to_unicode[(BYTE)multi[src++]];

		while (src < multiLen)
		{
			wchar_t unicode = cp1258_to_unicode[(BYTE)multi[src]];

			/* 다음 문자가 Tone 인가? */
			if (IsTone(unicode))
			{
				/* 앞의 문자와 합하자. */
				prev = ComposeTone(prev, unicode);
			}
			else
			{
				/* 일반 문자가 왔다. 앞 문자를 변환 */
				if (dest < wideLen)
					wide[dest++] = prev;
				prev = unicode;
			}
			++src;
		}

		if (dest < wideLen)
			wide[dest++] = prev;
	}

	return dest;
}

static bool DecomposeLetter(wchar_t input, char* letter)
{
	switch (input)
	{
	case 0x00c1: // L'Á'
	case 0x00c0: // L'À'
	case 0x1ea2: // L'Ả'
	case 0x00c3: // L'Ã'
	case 0x1ea0: // L'Ạ'
		*letter = 'A';
		return true;
	case 0x00e1: // L'á'
	case 0x00e0: // L'à'
	case 0x1ea3: // L'ả'
	case 0x00e3: // L'ã'
	case 0x1ea1: // L'ạ'
		*letter = 'a';
		return true;
	case 0x1eae: // L'Ắ'
	case 0x1eb0: // L'Ằ'
	case 0x1eb2: // L'Ẳ'
	case 0x1eb4: // L'Ẵ'
	case 0x1eb6: // L'Ặ'
		*letter = (char)0xc3;
		return true;
	case 0x1eaf: // L'ắ'
	case 0x1eb1: // L'ằ'
	case 0x1eb3: // L'ẳ'
	case 0x1eb5: // L'ẵ'
	case 0x1eb7: // L'ặ'
		*letter = (char)0xe3;
		return true;
	case 0x1ea4: // L'Ấ'
	case 0x1ea6: // L'Ầ'
	case 0x1ea8: // L'Ẩ'
	case 0x1eaa: // L'Ẫ'
	case 0x1eac: // L'Ậ'
		*letter = (char)0xc2;
		return true;
	case 0x1ea5: // L'ấ'
	case 0x1ea7: // L'ầ'
	case 0x1ea9: // L'ẩ'
	case 0x1eab: // L'ẫ'
	case 0x1ead: // L'ậ'
		*letter = (char)0xe2;
		return true;
	case 0x00c9: // L'É'
	case 0x00c8: // L'È'
	case 0x1eba: // L'Ẻ'
	case 0x1ebc: // L'Ẽ'
	case 0x1eb8: // L'Ẹ'
		*letter = (char)'E';
		return true;
	case 0x00e9: // L'é'
	case 0x00e8: // L'è'
	case 0x1ebb: // L'ẻ'
	case 0x1ebd: // L'ẽ'
	case 0x1eb9: // L'ẹ'
		*letter = (char)'e';
		return true;
	case 0x1ebe: // L'Ế'
	case 0x1ec0: // L'Ề'
	case 0x1ec2: // L'Ể'
	case 0x1ec4: // L'Ễ'
	case 0x1ec6: // L'Ệ'
		*letter = (char)0xca;
		return true;
	case 0x1ebf: // L'ế'
	case 0x1ec1: // L'ề'
	case 0x1ec3: // L'ể'
	case 0x1ec5: // L'ễ'
	case 0x1ec7: // L'ệ'
		*letter = (char)0xea;
		return true;
	case 0x00cd: // L'Í'
	case 0x00cc: // L'Ì'
	case 0x1ec8: // L'Ỉ'
	case 0x0128: // L'Ĩ'
	case 0x1eca: // L'Ị'
		*letter = (char)'I';
		return true;
	case 0x00ed: // L'í'
	case 0x00ec: // L'ì'
	case 0x1ec9: // L'ỉ'
	case 0x0129: // L'ĩ'
	case 0x1ecb: // L'ị'
		*letter = (char)'i';
		return true;
	case 0x00d3: // L'Ó'
	case 0x00d2: // L'Ò'
	case 0x1ece: // L'Ỏ'
	case 0x00d5: // L'Õ'
	case 0x1ecc: // L'Ọ'
		*letter = (char)'O';
		return true;
	case 0x00f3: // L'ó'
	case 0x00f2: // L'ò'
	case 0x1ecf: // L'ỏ'
	case 0x00f5: // L'õ'
	case 0x1ecd: // L'ọ'
		*letter = (char)'o';
		return true;
	case 0x1ed0: // L'Ố'
	case 0x1ed2: // L'Ồ'
	case 0x1ed4: // L'Ổ'
	case 0x1ed6: // L'Ỗ'
	case 0x1ed8: // L'Ộ'
		*letter = (char)0xd4;
		return true;
	case 0x1ed1: // L'ố'
	case 0x1ed3: // L'ồ'
	case 0x1ed5: // L'ổ'
	case 0x1ed7: // L'ỗ'
	case 0x1ed9: // L'ộ'
		*letter = (char)0xf4;
		return true;
	case 0x1eda: // L'Ớ'
	case 0x1edc: // L'Ờ'
	case 0x1ede: // L'Ở'
	case 0x1ee0: // L'Ỡ'
	case 0x1ee2: // L'Ợ'
		*letter = (char)0xd5;
		return true;
	case 0x1edb: // L'ớ'
	case 0x1edd: // L'ờ'
	case 0x1edf: // L'ở'
	case 0x1ee1: // L'ỡ'
	case 0x1ee3: // L'ợ'
		*letter = (char)0xf5;
		return true;
	case 0x00da: // L'Ú'
	case 0x00d9: // L'Ù'
	case 0x1ee6: // L'Ủ'
	case 0x0168: // L'Ũ'
	case 0x1ee4: // L'Ụ'
		*letter = (char)'U';
		return true;
	case 0x00fa: // L'ú'
	case 0x00f9: // L'ù'
	case 0x1ee7: // L'ủ'
	case 0x0169: // L'ũ'
	case 0x1ee5: // L'ụ'
		*letter = (char)'u';
		return true;
	case 0x1ee8: // L'Ứ'
	case 0x1eea: // L'Ừ'
	case 0x1eec: // L'Ử'
	case 0x1eee: // L'Ữ'
	case 0x1ef0: // L'Ự'
		*letter = (char)0xdd;
		return true;
	case 0x1ee9: // L'ứ'
	case 0x1eeb: // L'ừ'
	case 0x1eed: // L'ử'
	case 0x1eef: // L'ữ'
	case 0x1ef1: // L'ự'
		*letter = (char)0xfd;
		return true;
	case 0x1ef2: // L'Ỳ'
	case 0x00dd: // L'Ý'
	case 0x1ef6: // L'Ỷ'
	case 0x1ef8: // L'Ỹ'
	case 0x1ef4: // L'Ỵ'
		*letter = (char)'Y';
		return true;
	case 0x1ef3: // L'ỳ'
	case 0x00fd: // L'ý'
	case 0x1ef7: // L'ỷ'
	case 0x1ef9: // L'ỹ'
	case 0x1ef5: // L'ỵ'
		*letter = (char)'y';
		return true;
	case 0x0102: // L'Ă'
		*letter = (char)0xc3;
		return true;
	case 0x0103: // L'ă'
		*letter = (char)0xe3;
		return true;
	case 0x0110: // L'Đ'
		*letter = (char)0xd0;
		return true;
	case 0x0111: // L'đ'
		*letter = (char)0xf0;
		return true;
	case 0x01a0: // L'Ơ'
		*letter = (char)0xd5;
		return true;
	case 0x01a1: // L'ơ'
		*letter = (char)0xf5;
		return true;
	case 0x01af: // L'Ư'
		*letter = (char)0xdd;
		return true;
	case 0x01b0: // L'ư'
		*letter = (char)0xfd;
		return true;
	case 0x20ab: // L'₫'
		*letter = (char)0xfe;
		return true;
	case 0x201c: // L'“'
		*letter = (char)'"';
		return true;
	case 0x201d: // L'”'
		*letter = (char)'"';
		return true;
	}

	if (input < 256)
	{
		*letter = (char)input;
		return true;
	}
	return false;
}

static bool DecomposeTone(wchar_t input, char* tone)
{
	switch (input)
	{
	case 0x00c1: // L'Á'
	case 0x00e1: // L'á'
	case 0x1eae: // L'Ắ'
	case 0x1eaf: // L'ắ'
	case 0x1ea4: // L'Ấ'
	case 0x1ea5: // L'ấ'
	case 0x00c9: // L'É'
	case 0x00e9: // L'é'
	case 0x1ebe: // L'Ế'
	case 0x1ebf: // L'ế'
	case 0x00cd: // L'Í'
	case 0x00ed: // L'í'
	case 0x00d3: // L'Ó'
	case 0x00f3: // L'ó'
	case 0x1ed0: // L'Ố'
	case 0x1ed1: // L'ố'
	case 0x1eda: // L'Ớ'
	case 0x1edb: // L'ớ'
	case 0x00da: // L'Ú'
	case 0x00fa: // L'ú'
	case 0x1ee8: // L'Ứ'
	case 0x1ee9: // L'ứ'
	case 0x00dd: // L'Ý'
	case 0x00fd: // L'ý'
		*tone = (char)0xec;
		return true;
	case 0x00c0: // L'À'
	case 0x00e0: // L'à'
	case 0x1eb0: // L'Ằ'
	case 0x1eb1: // L'ằ'
	case 0x1ea6: // L'Ầ'
	case 0x1ea7: // L'ầ'
	case 0x00c8: // L'È'
	case 0x00e8: // L'è'
	case 0x1ec0: // L'Ề'
	case 0x1ec1: // L'ề'
	case 0x00cc: // L'Ì'
	case 0x00ec: // L'ì'
	case 0x00d2: // L'Ò'
	case 0x00f2: // L'ò'
	case 0x1ed2: // L'Ồ'
	case 0x1ed3: // L'ồ'
	case 0x1edc: // L'Ờ'
	case 0x1edd: // L'ờ'
	case 0x00d9: // L'Ù'
	case 0x00f9: // L'ù'
	case 0x1eea: // L'Ừ'
	case 0x1eeb: // L'ừ'
	case 0x1ef2: // L'Ỳ'
	case 0x1ef3: // L'ỳ'
		*tone = (char)0xcc;
		return true;
	case 0x1ea2: // L'Ả'
	case 0x1ea3: // L'ả'
	case 0x1eb2: // L'Ẳ'
	case 0x1eb3: // L'ẳ'
	case 0x1ea8: // L'Ẩ'
	case 0x1ea9: // L'ẩ'
	case 0x1eba: // L'Ẻ'
	case 0x1ebb: // L'ẻ'
	case 0x1ec2: // L'Ể'
	case 0x1ec3: // L'ể'
	case 0x1ec8: // L'Ỉ'
	case 0x1ec9: // L'ỉ'
	case 0x1ece: // L'Ỏ'
	case 0x1ecf: // L'ỏ'
	case 0x1ed4: // L'Ổ'
	case 0x1ed5: // L'ổ'
	case 0x1ede: // L'Ở'
	case 0x1edf: // L'ở'
	case 0x1ee6: // L'Ủ'
	case 0x1ee7: // L'ủ'
	case 0x1eec: // L'Ử'
	case 0x1eed: // L'ử'
	case 0x1ef6: // L'Ỷ'
	case 0x1ef7: // L'ỷ'
		*tone = (char)0xd2;
		return true;
	case 0x00c3: // L'Ã'
	case 0x00e3: // L'ã'
	case 0x1eb4: // L'Ẵ'
	case 0x1eb5: // L'ẵ'
	case 0x1eaa: // L'Ẫ'
	case 0x1eab: // L'ẫ'
	case 0x1ebc: // L'Ẽ'
	case 0x1ebd: // L'ẽ'
	case 0x1ec4: // L'Ễ'
	case 0x1ec5: // L'ễ'
	case 0x0128: // L'Ĩ'
	case 0x0129: // L'ĩ'
	case 0x00d5: // L'Õ'
	case 0x00f5: // L'õ'
	case 0x1ed6: // L'Ỗ'
	case 0x1ed7: // L'ỗ'
	case 0x1ee0: // L'Ỡ'
	case 0x1ee1: // L'ỡ'
	case 0x0169: // L'ũ'
	case 0x0168: // L'Ũ'
	case 0x1eee: // L'Ữ'
	case 0x1eef: // L'ữ'
	case 0x1ef8: // L'Ỹ'
	case 0x1ef9: // L'ỹ'
		*tone = (char)0xde;
		return true;
	case 0x1ea0: // L'Ạ'
	case 0x1ea1: // L'ạ'
	case 0x1eb6: // L'Ặ'
	case 0x1eb7: // L'ặ'
	case 0x1eac: // L'Ậ'
	case 0x1ead: // L'ậ'
	case 0x1eb8: // L'Ẹ'
	case 0x1eb9: // L'ẹ'
	case 0x1ec6: // L'Ệ'
	case 0x1ec7: // L'ệ'
	case 0x1eca: // L'Ị'
	case 0x1ecb: // L'ị'
	case 0x1ecc: // L'Ọ'
	case 0x1ecd: // L'ọ'
	case 0x1ed8: // L'Ộ'
	case 0x1ed9: // L'ộ'
	case 0x1ee2: // L'Ợ'
	case 0x1ee3: // L'ợ'
	case 0x1ee4: // L'Ụ'
	case 0x1ee5: // L'ụ'
	case 0x1ef0: // L'Ự'
	case 0x1ef1: // L'ự'
	case 0x1ef4: // L'Ỵ'
	case 0x1ef5: // L'ỵ'
		*tone = (char)0xf2;
		return true;
	}

	return false;
}

int EL_String_Encode_Vietnamese(const wchar_t* wide, int wideLen, char* multi, int multiLen)
{
	if (wideLen < 0)
		wideLen = (int)wcslen(wide) + 1;

	int src = 0;
	int dest = 0;

	if (wideLen > 0)
	{
		while (src < wideLen && dest < multiLen)
		{
			char letter;
			if (DecomposeLetter(wide[src], &letter))
			{
				multi[dest++] = letter;
			}

			char tone;
			if (DecomposeTone(wide[src], &tone) && dest < multiLen)
			{
				multi[dest++] = tone;
			}

			++src;
		}
	}

	return dest;
}
