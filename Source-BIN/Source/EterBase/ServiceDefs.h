#ifndef __INC_SERVICE_DEFS_H__
#define __INC_SERVICE_DEFS_H__

#define __IMPROVED_PACKET_ENCRYPTION__
//#define __SEND_SEQUENCE__
#define __DOUBLE_RECV_BUFFER__
#define __VERTEX_BUFFER_PERFORMANCE__
#define __ANI_IMAGE_PERFORMANCE__

enum EEterPackKeySet
{
	COMPRESS_EIX = 1,
	COMPRESS_EPK = 2,
	COMPRESS_ITEM = 3,
	COMPRESS_MOB = 4,

	COMPRESS_BRICK_SIZE = 4,
	COMPRESS_BIRCK_LEN = 10,
	COMPRESS_KEY_SIZE = COMPRESS_BIRCK_LEN + COMPRESS_BRICK_SIZE + 2,
};

static const char* s_strEIX = ".idx"; // EterPack Index
static const char* s_strEPK = ".data"; // EterPack

static std::string s_strEterPackSecurityKey[COMPRESS_BRICK_SIZE] = {
	std::string(AY_OBFUSCATE("1041748338")),
	std::string(AY_OBFUSCATE("1557672469")),
	std::string(AY_OBFUSCATE("1649914760")),
	std::string(AY_OBFUSCATE("619526752"))
};

static std::string s_strEterPackKey[COMPRESS_BRICK_SIZE] = {
	std::string(AY_OBFUSCATE("1183607424")),
	std::string(AY_OBFUSCATE("1452461834")),
	std::string(AY_OBFUSCATE("840044373")),
	std::string(AY_OBFUSCATE("1566759064"))
};

static std::string s_strItemProtoKey[COMPRESS_BRICK_SIZE] = {
	std::string(AY_OBFUSCATE("173217")),
	std::string(AY_OBFUSCATE("72619434")),
	std::string(AY_OBFUSCATE("408587239")),
	std::string(AY_OBFUSCATE("27973291"))
};

static std::string s_strMobProtoKey[COMPRESS_BRICK_SIZE] = {
	std::string(AY_OBFUSCATE("4813894")),
	std::string(AY_OBFUSCATE("18955")),
	std::string(AY_OBFUSCATE("552631")),
	std::string(AY_OBFUSCATE("6822045"))
};

#endif // __INC_SERVICE_DEFS_H__
