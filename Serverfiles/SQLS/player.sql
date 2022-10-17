/*
 Navicat Premium Data Transfer

 Source Server         : V5_TESTE
 Source Server Type    : MySQL
 Source Server Version : 80030
 Source Host           : 192.168.2.14:3306
 Source Schema         : player

 Target Server Type    : MySQL
 Target Server Version : 80030
 File Encoding         : 65001

 Date: 14/10/2022 06:45:36
*/

SET NAMES utf8mb4;
set @@sql_mode='';
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for affect
-- ----------------------------
DROP TABLE IF EXISTS `affect`;
CREATE TABLE `affect`  (
  `dwPID` int UNSIGNED NOT NULL DEFAULT 0,
  `bType` smallint UNSIGNED NOT NULL DEFAULT 0,
  `bApplyOn` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `lApplyValue` int NOT NULL DEFAULT 0,
  `dwFlag` int UNSIGNED NOT NULL DEFAULT 0,
  `lDuration` int NOT NULL DEFAULT 0,
  `lSPCost` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`dwPID`, `bType`, `bApplyOn`, `lApplyValue`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of affect
-- ----------------------------

-- ----------------------------
-- Table structure for banword
-- ----------------------------
DROP TABLE IF EXISTS `banword`;
CREATE TABLE `banword`  (
  `word` varbinary(24) NOT NULL DEFAULT '',
  PRIMARY KEY (`word`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of banword
-- ----------------------------
INSERT INTO `banword` VALUES (0x2F);
INSERT INTO `banword` VALUES (0x3B);
INSERT INTO `banword` VALUES (0x3F);
INSERT INTO `banword` VALUES (0x5C);

-- ----------------------------
-- Table structure for card
-- ----------------------------
DROP TABLE IF EXISTS `card`;
CREATE TABLE `card`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `cash` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `prices` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `no` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `pwd` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `lock` int NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of card
-- ----------------------------

-- ----------------------------
-- Table structure for change_empire
-- ----------------------------
DROP TABLE IF EXISTS `change_empire`;
CREATE TABLE `change_empire`  (
  `change_count` int NULL DEFAULT NULL,
  `account_id` int NULL DEFAULT NULL,
  `time` datetime(0) NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of change_empire
-- ----------------------------

-- ----------------------------
-- Table structure for dungeon_ranking
-- ----------------------------
DROP TABLE IF EXISTS `dungeon_ranking`;
CREATE TABLE `dungeon_ranking`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Index',
  `pid` int NOT NULL COMMENT 'Player ID',
  `dungeon_index` int NOT NULL COMMENT 'Dungeon Map Index',
  `completed` int NOT NULL COMMENT 'Dungeon Completed',
  `time` int NOT NULL COMMENT 'Dungeon Finish Time',
  `damage` int NOT NULL COMMENT 'Dungeon Highest Damage',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of dungeon_ranking
-- ----------------------------

-- ----------------------------
-- Table structure for guild
-- ----------------------------
DROP TABLE IF EXISTS `guild`;
CREATE TABLE `guild`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `sp` smallint NOT NULL DEFAULT 1000,
  `master` int UNSIGNED NOT NULL DEFAULT 0,
  `level` tinyint NULL DEFAULT NULL,
  `exp` int NULL DEFAULT NULL,
  `skill_point` tinyint NOT NULL DEFAULT 0,
  `skill` tinyblob NULL,
  `win` int NOT NULL DEFAULT 0,
  `draw` int NOT NULL DEFAULT 0,
  `loss` int NOT NULL DEFAULT 0,
  `ladder_point` int NOT NULL DEFAULT 0,
  `gold` int NOT NULL DEFAULT 0,
  `dungeon_ch` int NOT NULL DEFAULT 0,
  `dungeon_map` int UNSIGNED NOT NULL DEFAULT 0,
  `dungeon_cooldown` int UNSIGNED NOT NULL DEFAULT 0,
  `dungeon_start` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild
-- ----------------------------

-- ----------------------------
-- Table structure for guild_comment
-- ----------------------------
DROP TABLE IF EXISTS `guild_comment`;
CREATE TABLE `guild_comment`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `guild_id` int UNSIGNED NULL DEFAULT NULL,
  `name` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `notice` tinyint NULL DEFAULT NULL,
  `content` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `time` datetime(0) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `aaa`(`notice`, `id`, `guild_id`) USING BTREE,
  INDEX `guild_id`(`guild_id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_comment
-- ----------------------------

-- ----------------------------
-- Table structure for guild_grade
-- ----------------------------
DROP TABLE IF EXISTS `guild_grade`;
CREATE TABLE `guild_grade`  (
  `guild_id` int NOT NULL DEFAULT 0,
  `grade` tinyint NOT NULL DEFAULT 0,
  `name` varchar(8) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `auth` set('ADD_MEMBER','REMOVE_MEMBER','NOTICE','USE_SKILL') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT '',
  PRIMARY KEY (`guild_id`, `grade`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_grade
-- ----------------------------

-- ----------------------------
-- Table structure for guild_member
-- ----------------------------
DROP TABLE IF EXISTS `guild_member`;
CREATE TABLE `guild_member`  (
  `pid` int UNSIGNED NOT NULL DEFAULT 0,
  `guild_id` int UNSIGNED NOT NULL DEFAULT 0,
  `grade` tinyint NULL DEFAULT NULL,
  `is_general` tinyint(1) NOT NULL DEFAULT 0,
  `offer` int UNSIGNED NULL DEFAULT NULL,
  PRIMARY KEY (`guild_id`, `pid`) USING BTREE,
  UNIQUE INDEX `pid`(`pid`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of guild_member
-- ----------------------------

-- ----------------------------
-- Table structure for guild_war
-- ----------------------------
DROP TABLE IF EXISTS `guild_war`;
CREATE TABLE `guild_war`  (
  `id_from` int NOT NULL DEFAULT 0,
  `id_to` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`id_from`, `id_to`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of guild_war
-- ----------------------------

-- ----------------------------
-- Table structure for guild_war_bet
-- ----------------------------
DROP TABLE IF EXISTS `guild_war_bet`;
CREATE TABLE `guild_war_bet`  (
  `login` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `gold` int UNSIGNED NOT NULL DEFAULT 0,
  `guild` int NOT NULL DEFAULT 0,
  `war_id` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`war_id`, `login`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_war_bet
-- ----------------------------

-- ----------------------------
-- Table structure for guild_war_reservation
-- ----------------------------
DROP TABLE IF EXISTS `guild_war_reservation`;
CREATE TABLE `guild_war_reservation`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `guild1` int UNSIGNED NOT NULL DEFAULT 0,
  `guild2` int UNSIGNED NOT NULL DEFAULT 0,
  `time` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `type` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `warprice` int UNSIGNED NOT NULL DEFAULT 0,
  `initscore` int UNSIGNED NOT NULL DEFAULT 0,
  `started` tinyint(1) NOT NULL DEFAULT 0,
  `bet_from` int UNSIGNED NOT NULL DEFAULT 0,
  `bet_to` int UNSIGNED NOT NULL DEFAULT 0,
  `winner` int NOT NULL DEFAULT -1,
  `power1` int NOT NULL DEFAULT 0,
  `power2` int NOT NULL DEFAULT 0,
  `handicap` int NOT NULL DEFAULT 0,
  `result1` int NOT NULL DEFAULT 0,
  `result2` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of guild_war_reservation
-- ----------------------------

-- ----------------------------
-- Table structure for highscore
-- ----------------------------
DROP TABLE IF EXISTS `highscore`;
CREATE TABLE `highscore`  (
  `pid` int NULL DEFAULT NULL,
  `board` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `value` varchar(0) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of highscore
-- ----------------------------

-- ----------------------------
-- Table structure for horse_name
-- ----------------------------
DROP TABLE IF EXISTS `horse_name`;
CREATE TABLE `horse_name`  (
  `id` int UNSIGNED NOT NULL DEFAULT 0,
  `name` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NONAME',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of horse_name
-- ----------------------------

-- ----------------------------
-- Table structure for item
-- ----------------------------
DROP TABLE IF EXISTS `item`;
CREATE TABLE `item`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `owner_id` int UNSIGNED NOT NULL DEFAULT 0,
  `window` enum('INVENTORY','EQUIPMENT','SAFEBOX','MALL','DRAGON_SOUL_INVENTORY','BELT_INVENTORY','SKILL_BOOK_INVENTORY','UPGRADE_ITEMS_INVENTORY','STONE_INVENTORY','GIFT_BOX_INVENTORY','GROUND') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `pos` smallint UNSIGNED NOT NULL DEFAULT 0,
  `count` smallint UNSIGNED NOT NULL DEFAULT 0,
  `vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `soulbind` bigint NOT NULL DEFAULT 0,
  `transmutation` int UNSIGNED NOT NULL DEFAULT 0,
  `socket0` int UNSIGNED NOT NULL DEFAULT 0,
  `socket1` int UNSIGNED NOT NULL DEFAULT 0,
  `socket2` int UNSIGNED NOT NULL DEFAULT 0,
  `socket3` int UNSIGNED NOT NULL DEFAULT 0,
  `socket4` int UNSIGNED NOT NULL DEFAULT 0,
  `socket5` int UNSIGNED NOT NULL DEFAULT 0,
  `apply_type0` tinyint NOT NULL DEFAULT 0,
  `apply_value0` smallint NOT NULL DEFAULT 0,
  `apply_path0` tinyint NOT NULL DEFAULT 0,
  `apply_type1` tinyint NOT NULL DEFAULT 0,
  `apply_value1` smallint NOT NULL DEFAULT 0,
  `apply_path1` tinyint NOT NULL DEFAULT 0,
  `apply_type2` tinyint NOT NULL DEFAULT 0,
  `apply_value2` smallint NOT NULL DEFAULT 0,
  `apply_path2` tinyint NOT NULL DEFAULT 0,
  `apply_type3` tinyint NOT NULL DEFAULT 0,
  `apply_value3` smallint NOT NULL DEFAULT 0,
  `apply_path3` tinyint NOT NULL DEFAULT 0,
  `attrtype0` tinyint NOT NULL DEFAULT 0,
  `attrvalue0` smallint NOT NULL DEFAULT 0,
  `attrtype1` tinyint NOT NULL DEFAULT 0,
  `attrvalue1` smallint NOT NULL DEFAULT 0,
  `attrtype2` tinyint NOT NULL DEFAULT 0,
  `attrvalue2` smallint NOT NULL DEFAULT 0,
  `attrtype3` tinyint NOT NULL DEFAULT 0,
  `attrvalue3` smallint NOT NULL DEFAULT 0,
  `attrtype4` tinyint NOT NULL DEFAULT 0,
  `attrvalue4` smallint NOT NULL DEFAULT 0,
  `attrtype5` tinyint NOT NULL DEFAULT 0,
  `attrvalue5` smallint NOT NULL DEFAULT 0,
  `attrtype6` tinyint NOT NULL DEFAULT 0,
  `attrvalue6` smallint NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `owner_id_idx`(`owner_id`) USING BTREE,
  INDEX `item_vnum_index`(`vnum`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 2000000000 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of item
-- ----------------------------

-- ----------------------------
-- Table structure for item_attr
-- ----------------------------
DROP TABLE IF EXISTS `item_attr`;
CREATE TABLE `item_attr`  (
  `apply` enum('MAX_HP','MAX_SP','CON','INT','STR','DEX','ATT_SPEED','MOV_SPEED','CAST_SPEED','HP_REGEN','SP_REGEN','POISON_PCT','STUN_PCT','SLOW_PCT','CRITICAL_PCT','PENETRATE_PCT','ATTBONUS_HUMAN','ATTBONUS_ANIMAL','ATTBONUS_ORC','ATTBONUS_MILGYO','ATTBONUS_UNDEAD','ATTBONUS_DEVIL','STEAL_HP','STEAL_SP','MANA_BURN_PCT','DAMAGE_SP_RECOVER','BLOCK','DODGE','RESIST_SWORD','RESIST_TWOHAND','RESIST_DAGGER','RESIST_BELL','RESIST_FAN','RESIST_BOW','RESIST_FIRE','RESIST_ELEC','RESIST_MAGIC','RESIST_WIND','REFLECT_MELEE','REFLECT_CURSE','POISON_REDUCE','KILL_SP_RECOVER','EXP_DOUBLE_BONUS','GOLD_DOUBLE_BONUS','ITEM_DROP_BONUS','POTION_BONUS','KILL_HP_RECOVER','IMMUNE_STUN','IMMUNE_SLOW','IMMUNE_FALL','SKILL','BOW_DISTANCE','ATT_GRADE_BONUS','DEF_GRADE_BONUS','MAGIC_ATT_GRADE','MAGIC_DEF_GRADE','CURSE_PCT','MAX_STAMINA','ATTBONUS_WARRIOR','ATTBONUS_ASSASSIN','ATTBONUS_SURA','ATTBONUS_SHAMAN','ATTBONUS_MONSTER','MALL_ATTBONUS','MALL_DEFBONUS','MALL_EXPBONUS','MALL_ITEMBONUS','MALL_GOLDBONUS','MAX_HP_PCT','MAX_SP_PCT','SKILL_DAMAGE_BONUS','NORMAL_HIT_DAMAGE_BONUS','SKILL_DEFEND_BONUS','NORMAL_HIT_DEFEND_BONUS','PC_BANG_EXP_BONUS','PC_BANG_DROP_BONUS','EXTRACT_HP_PCT','RESIST_WARRIOR','RESIST_ASSASSIN','RESIST_SURA','RESIST_SHAMAN','ENERGY','DEF_GRADE','COSTUME_ATTR_BONUS','MAGIC_ATTBONUS_PER','MELEE_MAGIC_ATTBONUS_PER','RESIST_ICE','RESIST_EARTH','RESIST_DARK','RESIST_CRITICAL','RESIST_PENETRATE','BLEEDING_REDUCE','BLEEDING_PCT','ATTBONUS_WOLFMAN','RESIST_WOLFMAN','RESIST_CLAW','MOUNT','ACCEDRAIN_RATE','RESIST_MAGIC_REDUCTION','ENCHANT_ELECT','ENCHANT_FIRE','ENCHANT_ICE','ENCHANT_WIND','ENCHANT_EARTH','ENCHANT_DARK','ATTBONUS_CZ','ATTBONUS_INSECT','ATTBONUS_DESERT','ATTBONUS_SWORD','ATTBONUS_TWOHAND','ATTBONUS_DAGGER','ATTBONUS_BELL','ATTBONUS_FAN','ATTBONUS_BOW','ATTBONUS_CLAW','RESIST_HUMAN','ATTBONUS_STONE','SUNGMA_STR','SUNGMA_HP','SUNGMA_MOVE','SUNGMA_IMMUNE','INFINITE_AFFECT_DURATION','ATTACK_SPEED','MOVE_SPEED','ATT_BONUS','DEF_BONUS','ATT_BONUS_TO_WARRIOR','ATT_BONUS_TO_ASSASSIN','ATT_BONUS_TO_SURA','ATT_BONUS_TO_SHAMAN','ATT_BONUS_TO_WOLFMAN') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `prob` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv1` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv2` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv3` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv4` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv5` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `weapon` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `body` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `wrist` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `foots` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `neck` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `head` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `shield` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `ear` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `costume_body` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `costume_hair` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `costume_weapon` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `pendant` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `glove` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT ''
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of item_attr
-- ----------------------------
INSERT INTO `item_attr` VALUES ('ATTBONUS_MILGYO', '1', '2', '4', '6', '10', '20', '5', '0', '5', '0', '0', '5', '5', '5', '0', '5', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('ATTBONUS_SWORD', '1', '1', '2', '3', '4', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5');
INSERT INTO `item_attr` VALUES ('ATTBONUS_TWOHAND', '1', '1', '2', '3', '4', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5');
INSERT INTO `item_attr` VALUES ('ATTBONUS_DAGGER', '1', '1', '2', '3', '4', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5');
INSERT INTO `item_attr` VALUES ('ATTBONUS_BELL', '1', '1', '2', '3', '4', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5');
INSERT INTO `item_attr` VALUES ('ATTBONUS_FAN', '1', '1', '2', '3', '4', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5');
INSERT INTO `item_attr` VALUES ('ATTBONUS_BOW', '1', '1', '2', '3', '4', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5');
INSERT INTO `item_attr` VALUES ('ATTBONUS_CLAW', '1', '1', '2', '3', '4', '5', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '5', '5');
INSERT INTO `item_attr` VALUES ('ATTBONUS_CZ', '1', '10', '15', '20', '30', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '4');
INSERT INTO `item_attr` VALUES ('ATTBONUS_INSECT', '1', '10', '15', '20', '30', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '4');
INSERT INTO `item_attr` VALUES ('ATTBONUS_DESERT', '1', '10', '15', '20', '30', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '4');
INSERT INTO `item_attr` VALUES ('RESIST_HUMAN', '1', '2', '3', '6', '10', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '4');
INSERT INTO `item_attr` VALUES ('ATTBONUS_HUMAN', '1', '1', '2', '3', '5', '10', '5', '0', '5', '0', '0', '5', '5', '5', '0', '5', '5', '4', '4');
INSERT INTO `item_attr` VALUES ('ATTBONUS_ORC', '1', '2', '4', '6', '10', '20', '5', '0', '5', '0', '0', '5', '5', '5', '0', '5', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('ATTBONUS_DEVIL', '1', '2', '4', '6', '10', '20', '5', '0', '5', '0', '0', '5', '5', '5', '0', '5', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('ATTBONUS_ANIMAL', '1', '2', '4', '6', '10', '20', '5', '0', '5', '0', '0', '5', '5', '5', '0', '5', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('ATTBONUS_UNDEAD', '1', '2', '4', '6', '10', '20', '5', '0', '5', '0', '0', '5', '5', '5', '0', '5', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('ATTBONUS_CZ', '1', '10', '15', '20', '30', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '4');
INSERT INTO `item_attr` VALUES ('ATTBONUS_INSECT', '1', '10', '15', '20', '30', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '4');
INSERT INTO `item_attr` VALUES ('ATTBONUS_DESERT', '1', '10', '15', '20', '30', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '4', '4');
INSERT INTO `item_attr` VALUES ('RESIST_MAGIC', '1', '2', '4', '6', '10', '15', '0', '5', '5', '0', '0', '5', '0', '0', '5', '5', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_ELEC', '1', '2', '4', '6', '10', '15', '0', '5', '5', '0', '0', '5', '0', '0', '5', '5', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_FIRE', '1', '2', '4', '6', '10', '15', '0', '5', '5', '0', '0', '5', '0', '0', '5', '5', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_WIND', '1', '2', '4', '6', '10', '15', '0', '5', '5', '0', '0', '5', '0', '0', '5', '5', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_SWORD', '1', '2', '4', '6', '10', '15', '0', '5', '0', '5', '5', '0', '0', '5', '5', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_TWOHAND', '1', '2', '4', '6', '10', '15', '0', '5', '0', '5', '5', '0', '0', '5', '5', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_DAGGER', '1', '2', '4', '6', '10', '15', '0', '5', '0', '5', '5', '0', '0', '5', '5', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_BELL', '1', '2', '4', '6', '10', '15', '0', '5', '0', '5', '5', '0', '0', '5', '5', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_FAN', '1', '2', '4', '6', '10', '15', '0', '5', '0', '5', '5', '0', '0', '5', '5', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_BOW', '1', '2', '4', '6', '10', '15', '0', '5', '0', '5', '5', '0', '0', '5', '5', '5', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('RESIST_CLAW', '1', '2', '4', '6', '10', '15', '0', '5', '0', '5', '5', '0', '0', '5', '5', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('CON', '1', '2', '4', '6', '8', '12', '5', '0', '0', '0', '0', '0', '5', '0', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('INT', '1', '2', '4', '6', '8', '12', '5', '0', '0', '0', '0', '0', '5', '0', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('STR', '1', '2', '4', '6', '8', '12', '5', '0', '0', '0', '0', '0', '5', '0', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('DEX', '1', '2', '4', '6', '8', '12', '5', '0', '0', '0', '0', '0', '5', '0', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('IMMUNE_STUN', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('IMMUNE_SLOW', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('MAX_HP', '1', '500', '1000', '1500', '2000', '0', '0', '4', '4', '4', '4', '0', '0', '0', '4', '0', '4', '0', '0');
INSERT INTO `item_attr` VALUES ('MAX_SP', '1', '10', '20', '30', '50', '80', '0', '5', '5', '5', '5', '0', '0', '5', '5', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('CRITICAL_PCT', '1', '1', '2', '3', '5', '10', '5', '0', '0', '5', '5', '0', '0', '0', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('PENETRATE_PCT', '1', '1', '2', '3', '5', '10', '5', '0', '5', '0', '5', '0', '0', '0', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('ATT_SPEED', '1', '1', '2', '3', '5', '8', '0', '0', '0', '5', '0', '5', '0', '0', '0', '5', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('CAST_SPEED', '1', '2', '4', '6', '10', '20', '5', '5', '0', '0', '0', '0', '0', '0', '5', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('MOV_SPEED', '1', '2', '4', '6', '10', '20', '0', '0', '0', '0', '0', '0', '0', '5', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('STUN_PCT', '1', '1', '2', '3', '5', '8', '5', '0', '0', '5', '5', '0', '0', '0', '0', '0', '5', '5', '5');
INSERT INTO `item_attr` VALUES ('SLOW_PCT', '1', '1', '2', '3', '5', '8', '5', '0', '0', '5', '0', '0', '0', '0', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('POISON_PCT', '1', '1', '2', '3', '5', '8', '5', '0', '0', '0', '0', '5', '0', '0', '0', '5', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('STEAL_HP', '1', '1', '2', '3', '5', '10', '0', '5', '5', '0', '0', '0', '0', '0', '5', '0', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('STEAL_SP', '1', '1', '2', '3', '5', '10', '0', '5', '0', '0', '5', '5', '0', '0', '5', '5', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('HP_REGEN', '1', '4', '8', '12', '20', '30', '0', '0', '0', '0', '5', '5', '0', '0', '0', '5', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('SP_REGEN', '1', '4', '8', '12', '20', '30', '0', '0', '0', '0', '5', '5', '0', '0', '0', '5', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('DAMAGE_SP_RECOVER', '1', '1', '2', '3', '5', '10', '0', '5', '0', '0', '5', '5', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('ITEM_DROP_BONUS', '1', '2', '4', '6', '8', '20', '0', '0', '5', '0', '0', '0', '0', '5', '0', '0', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('GOLD_DOUBLE_BONUS', '1', '2', '4', '6', '8', '20', '0', '0', '0', '5', '5', '0', '5', '0', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('EXP_DOUBLE_BONUS', '1', '2', '4', '6', '8', '20', '0', '0', '0', '5', '5', '0', '5', '0', '0', '0', '5', '0', '0');
INSERT INTO `item_attr` VALUES ('REFLECT_MELEE', '1', '2', '3', '6', '10', '0', '0', '4', '0', '0', '0', '0', '4', '0', '4', '0', '0', '0', '0');
INSERT INTO `item_attr` VALUES ('BLOCK', '1', '2', '4', '6', '10', '15', '0', '0', '0', '0', '0', '0', '5', '0', '0', '0', '0', '0', '0');

-- ----------------------------
-- Table structure for item_attr_rare
-- ----------------------------
DROP TABLE IF EXISTS `item_attr_rare`;
CREATE TABLE `item_attr_rare`  (
  `apply` enum('MAX_HP','MAX_SP','CON','INT','STR','DEX','ATT_SPEED','MOV_SPEED','CAST_SPEED','HP_REGEN','SP_REGEN','POISON_PCT','STUN_PCT','SLOW_PCT','CRITICAL_PCT','PENETRATE_PCT','ATTBONUS_HUMAN','ATTBONUS_ANIMAL','ATTBONUS_ORC','ATTBONUS_MILGYO','ATTBONUS_UNDEAD','ATTBONUS_DEVIL','STEAL_HP','STEAL_SP','MANA_BURN_PCT','DAMAGE_SP_RECOVER','BLOCK','DODGE','RESIST_SWORD','RESIST_TWOHAND','RESIST_DAGGER','RESIST_BELL','RESIST_FAN','RESIST_BOW','RESIST_FIRE','RESIST_ELEC','RESIST_MAGIC','RESIST_WIND','REFLECT_MELEE','REFLECT_CURSE','POISON_REDUCE','KILL_SP_RECOVER','EXP_DOUBLE_BONUS','GOLD_DOUBLE_BONUS','ITEM_DROP_BONUS','POTION_BONUS','KILL_HP_RECOVER','IMMUNE_STUN','IMMUNE_SLOW','IMMUNE_FALL','SKILL','BOW_DISTANCE','ATT_GRADE_BONUS','DEF_GRADE_BONUS','MAGIC_ATT_GRADE','MAGIC_DEF_GRADE','CURSE_PCT','MAX_STAMINA','ATTBONUS_WARRIOR','ATTBONUS_ASSASSIN','ATTBONUS_SURA','ATTBONUS_SHAMAN','ATTBONUS_MONSTER','MALL_ATTBONUS','MALL_DEFBONUS','MALL_EXPBONUS','MALL_ITEMBONUS','MALL_GOLDBONUS','MAX_HP_PCT','MAX_SP_PCT','SKILL_DAMAGE_BONUS','NORMAL_HIT_DAMAGE_BONUS','SKILL_DEFEND_BONUS','NORMAL_HIT_DEFEND_BONUS','PC_BANG_EXP_BONUS','PC_BANG_DROP_BONUS','EXTRACT_HP_PCT','RESIST_WARRIOR','RESIST_ASSASSIN','RESIST_SURA','RESIST_SHAMAN','ENERGY','DEF_GRADE','COSTUME_ATTR_BONUS','MAGIC_ATTBONUS_PER','MELEE_MAGIC_ATTBONUS_PER','RESIST_ICE','RESIST_EARTH','RESIST_DARK','RESIST_CRITICAL','RESIST_PENETRATE','BLEEDING_REDUCE','BLEEDING_PCT','ATTBONUS_WOLFMAN','RESIST_WOLFMAN','RESIST_CLAW','MOUNT','ACCEDRAIN_RATE','RESIST_MAGIC_REDUCTION','ENCHANT_ELECT','ENCHANT_FIRE','ENCHANT_ICE','ENCHANT_WIND','ENCHANT_EARTH','ENCHANT_DARK','ATTBONUS_CZ','ATTBONUS_INSECT','ATTBONUS_DESERT','ATTBONUS_SWORD','ATTBONUS_TWOHAND','ATTBONUS_DAGGER','ATTBONUS_BELL','ATTBONUS_FAN','ATTBONUS_BOW','ATTBONUS_CLAW','RESIST_HUMAN','ATTBONUS_STONE','SUNGMA_STR','SUNGMA_HP','SUNGMA_MOVE','SUNGMA_IMMUNE','INFINITE_AFFECT_DURATION','ATTACK_SPEED','MOVE_SPEED','ATT_BONUS','DEF_BONUS','ATT_BONUS_TO_WARRIOR','ATT_BONUS_TO_ASSASSIN','ATT_BONUS_TO_SURA','ATT_BONUS_TO_SHAMAN','ATT_BONUS_TO_WOLFMAN') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `prob` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv1` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv2` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv3` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv4` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lv5` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `weapon` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `body` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `wrist` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `foots` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `neck` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `head` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `shield` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `ear` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `pendant` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT ''
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of item_attr_rare
-- ----------------------------
INSERT INTO `item_attr_rare` VALUES ('MAX_HP', '1', '2', '4', '5', '8', '10', '5', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item_attr_rare` VALUES ('INT', '1', '2', '4', '5', '8', '10', '5', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item_attr_rare` VALUES ('STR', '1', '2', '4', '5', '8', '10', '5', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item_attr_rare` VALUES ('DEX', '1', '2', '4', '5', '8', '10', '5', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item_attr_rare` VALUES ('ATTBONUS_MONSTER', '1', '2', '4', '5', '8', '10', '5', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item_attr_rare` VALUES ('MAX_SP', '1', '2', '4', '5', '8', '10', '5', '0', '0', '0', '0', '0', '0', '0', '0');
INSERT INTO `item_attr_rare` VALUES ('CON', '1', '2', '4', '5', '8', '10', '5', '0', '0', '0', '0', '0', '0', '0', '0');

-- ----------------------------
-- Table structure for item_award
-- ----------------------------
DROP TABLE IF EXISTS `item_award`;
CREATE TABLE `item_award`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `pid` int UNSIGNED NOT NULL DEFAULT 0,
  `login` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `transmutation` int UNSIGNED NOT NULL DEFAULT 0,
  `count` int UNSIGNED NOT NULL DEFAULT 0,
  `given_time` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `taken_time` datetime(0) NULL DEFAULT NULL,
  `item_id` int NULL DEFAULT NULL,
  `why` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `socket0` int NOT NULL DEFAULT 0,
  `socket1` int NOT NULL DEFAULT 0,
  `socket2` int NOT NULL DEFAULT 0,
  `socket3` int NOT NULL DEFAULT 0,
  `socket4` int NOT NULL DEFAULT 0,
  `socket5` int NOT NULL DEFAULT 0,
  `attrtype0` tinyint NOT NULL DEFAULT 0,
  `attrvalue0` smallint NOT NULL DEFAULT 0,
  `attrtype1` tinyint NOT NULL DEFAULT 0,
  `attrvalue1` smallint NOT NULL DEFAULT 0,
  `attrtype2` tinyint NOT NULL DEFAULT 0,
  `attrvalue2` smallint NOT NULL DEFAULT 0,
  `attrtype3` tinyint NOT NULL DEFAULT 0,
  `attrvalue3` smallint NOT NULL DEFAULT 0,
  `attrtype4` tinyint NOT NULL DEFAULT 0,
  `attrvalue4` smallint NOT NULL DEFAULT 0,
  `attrtype5` tinyint NOT NULL DEFAULT 0,
  `attrvalue5` smallint NOT NULL DEFAULT 0,
  `attrtype6` tinyint NOT NULL DEFAULT 0,
  `attrvalue6` smallint NOT NULL DEFAULT 0,
  `mall` tinyint(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `pid_idx`(`pid`) USING BTREE,
  INDEX `given_time_idx`(`given_time`) USING BTREE,
  INDEX `taken_time_idx`(`taken_time`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of item_award
-- ----------------------------

-- ----------------------------
-- Table structure for item_proto
-- ----------------------------
DROP TABLE IF EXISTS `item_proto`;
CREATE TABLE `item_proto`  (
  `vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `name` varbinary(200) NOT NULL DEFAULT 0x4E6F4E616D65,
  `locale_name` varbinary(30) NOT NULL DEFAULT 0x4E6F4E616D65,
  `type` tinyint NOT NULL DEFAULT 0,
  `subtype` tinyint NOT NULL DEFAULT 0,
  `weight` tinyint NULL DEFAULT 0,
  `size` tinyint NULL DEFAULT 0,
  `antiflag` int NULL DEFAULT 0,
  `flag` int NULL DEFAULT 0,
  `wearflag` int NULL DEFAULT 0,
  `immuneflag` set('PARA','CURSE','STUN','SLEEP','SLOW','POISON','TERROR') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `gold` int NULL DEFAULT 0,
  `shop_buy_price` int UNSIGNED NOT NULL DEFAULT 0,
  `refined_vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `refine_set` smallint UNSIGNED NOT NULL DEFAULT 0,
  `refine_set2` smallint UNSIGNED NOT NULL DEFAULT 0,
  `magic_pct` tinyint NOT NULL DEFAULT 0,
  `limittype0` tinyint NULL DEFAULT 0,
  `limitvalue0` int NULL DEFAULT 0,
  `limittype1` tinyint NULL DEFAULT 0,
  `limitvalue1` int NULL DEFAULT 0,
  `applytype0` tinyint NULL DEFAULT 0,
  `applyvalue0` int NULL DEFAULT 0,
  `applytype1` tinyint NULL DEFAULT 0,
  `applyvalue1` int NULL DEFAULT 0,
  `applytype2` tinyint NULL DEFAULT 0,
  `applyvalue2` int NULL DEFAULT 0,
  `applytype3` tinyint NULL DEFAULT 0,
  `applyvalue3` int NULL DEFAULT 0,
  `value0` int NULL DEFAULT 0,
  `value1` int NULL DEFAULT 0,
  `value2` int NULL DEFAULT 0,
  `value3` int NULL DEFAULT 0,
  `value4` int NULL DEFAULT 0,
  `value5` int NULL DEFAULT 0,
  `socket0` tinyint NULL DEFAULT -1,
  `socket1` tinyint NULL DEFAULT -1,
  `socket2` tinyint NULL DEFAULT -1,
  `socket3` tinyint NULL DEFAULT -1,
  `socket4` tinyint NULL DEFAULT -1,
  `socket5` tinyint NULL DEFAULT -1,
  `specular` tinyint NOT NULL DEFAULT 0,
  `socket_pct` tinyint NOT NULL DEFAULT 0,
  `addon_type` smallint NOT NULL DEFAULT 0,
  PRIMARY KEY (`vnum`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of item_proto
-- ----------------------------

-- ----------------------------
-- Table structure for land
-- ----------------------------
DROP TABLE IF EXISTS `land`;
CREATE TABLE `land`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `map_index` int NOT NULL DEFAULT 0,
  `x` int NOT NULL DEFAULT 0,
  `y` int NOT NULL DEFAULT 0,
  `width` int NOT NULL DEFAULT 0,
  `height` int NOT NULL DEFAULT 0,
  `guild_id` int UNSIGNED NOT NULL DEFAULT 0,
  `guild_level_limit` tinyint NOT NULL DEFAULT 0,
  `price` int UNSIGNED NOT NULL DEFAULT 0,
  `enable` enum('YES','NO') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NO',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 293 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of land
-- ----------------------------
INSERT INTO `land` VALUES (1, 1, 20200, 10000, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (2, 1, 23500, 13600, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (3, 1, 27600, 10900, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (4, 1, 55600, 9600, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (5, 1, 57100, 3800, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (6, 1, 60900, 5200, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (7, 1, 66100, 7900, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (8, 1, 81400, 16800, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (9, 1, 90700, 20400, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (10, 1, 94300, 23700, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (11, 1, 10800, 108500, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (12, 1, 5900, 108500, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (13, 1, 7700, 104300, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (14, 1, 23400, 46200, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (15, 1, 17200, 44400, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (16, 1, 20000, 40100, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (17, 1, 25800, 36900, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (18, 1, 16900, 35800, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (101, 21, 29400, 11400, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (102, 21, 33600, 12700, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (103, 21, 36300, 16300, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (104, 21, 55900, 17200, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (105, 21, 72800, 8500, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (106, 21, 76300, 5400, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (107, 21, 77600, 10200, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (108, 21, 84200, 47200, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (109, 21, 87800, 61000, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (110, 21, 32100, 113800, 3000, 3000, 0, 10, 45000000, 'YES');
INSERT INTO `land` VALUES (111, 21, 28800, 117200, 3000, 3000, 0, 10, 45000000, 'YES');
INSERT INTO `land` VALUES (112, 21, 24400, 118700, 3000, 3000, 0, 10, 45000000, 'YES');
INSERT INTO `land` VALUES (113, 21, 22600, 114400, 3000, 3000, 0, 10, 45000000, 'YES');
INSERT INTO `land` VALUES (114, 21, 34800, 92300, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (115, 21, 37500, 89000, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (116, 21, 35100, 73600, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (117, 21, 35400, 69200, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (118, 21, 35400, 55600, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (201, 41, 66100, 9400, 3000, 3000, 0, 10, 45000000, 'YES');
INSERT INTO `land` VALUES (202, 41, 70300, 7900, 3000, 3000, 0, 10, 45000000, 'YES');
INSERT INTO `land` VALUES (203, 41, 73600, 11500, 3000, 3000, 0, 10, 45000000, 'YES');
INSERT INTO `land` VALUES (204, 41, 70900, 15100, 3000, 3000, 0, 10, 45000000, 'YES');
INSERT INTO `land` VALUES (205, 41, 71800, 61900, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (206, 41, 77000, 64100, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (207, 41, 60700, 66800, 3000, 3000, 1, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (208, 41, 74500, 68200, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (209, 41, 67900, 110800, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (210, 41, 67900, 115700, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (211, 41, 62800, 114800, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (212, 41, 16600, 95300, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (213, 41, 5400, 94100, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (214, 41, 18600, 89300, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (215, 41, 4600, 89800, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (216, 41, 11200, 36000, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (217, 41, 8200, 31800, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (218, 41, 12000, 29200, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (75, 4, 9600, 13100, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (76, 4, 16100, 15600, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (77, 4, 9100, 19500, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (78, 4, 14900, 19500, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (79, 4, 27700, 19500, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (80, 4, 23600, 23000, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (81, 4, 31800, 24200, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (82, 4, 27900, 27300, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (83, 4, 35500, 35400, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (84, 4, 32800, 39300, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (85, 4, 39700, 39600, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (86, 4, 36100, 42700, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (87, 4, 24400, 39000, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (88, 4, 19900, 41600, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (89, 4, 14400, 41700, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (90, 4, 10200, 39100, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (91, 4, 7700, 34600, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (92, 4, 8400, 29500, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (175, 24, 7100, 5200, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (176, 24, 12800, 4800, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (177, 24, 9700, 9900, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (178, 24, 14500, 13300, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (179, 24, 36900, 26000, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (180, 24, 38600, 29800, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (181, 24, 36500, 33900, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (182, 24, 33600, 38600, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (183, 24, 30900, 33500, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (184, 24, 25700, 33500, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (185, 24, 25600, 37800, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (186, 24, 20900, 37800, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (187, 24, 20900, 33700, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (188, 24, 14200, 33700, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (189, 24, 10000, 37100, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (190, 24, 5800, 34800, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (191, 24, 8800, 29800, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (192, 24, 10500, 26100, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (275, 44, 5400, 7700, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (276, 44, 9700, 7300, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (277, 44, 5500, 11500, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (278, 44, 5100, 15500, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (279, 44, 8900, 17900, 3000, 3000, 0, 10, 40000000, 'YES');
INSERT INTO `land` VALUES (280, 44, 22200, 15600, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (281, 44, 43000, 3700, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (282, 44, 38900, 27200, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (283, 44, 30200, 29900, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (284, 44, 40000, 34500, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (285, 44, 32800, 37000, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (286, 44, 40600, 39300, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (287, 44, 35600, 40600, 3000, 3000, 0, 10, 50000000, 'YES');
INSERT INTO `land` VALUES (288, 44, 18500, 33600, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (289, 44, 17300, 38500, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (290, 44, 13500, 39400, 3000, 3000, 0, 10, 70000000, 'YES');
INSERT INTO `land` VALUES (291, 44, 9100, 38700, 3000, 3000, 0, 10, 65000000, 'YES');
INSERT INTO `land` VALUES (292, 44, 5000, 36300, 3000, 3000, 0, 10, 65000000, 'YES');

-- ----------------------------
-- Table structure for lotto_list
-- ----------------------------
DROP TABLE IF EXISTS `lotto_list`;
CREATE TABLE `lotto_list`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `server` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `pid` int UNSIGNED NULL DEFAULT NULL,
  `time` datetime(0) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of lotto_list
-- ----------------------------

-- ----------------------------
-- Table structure for mailbox
-- ----------------------------
DROP TABLE IF EXISTS `mailbox`;
CREATE TABLE `mailbox`  (
  `name` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `who` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `title` varchar(25) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `message` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `gm` tinyint NOT NULL DEFAULT 0,
  `confirm` tinyint NOT NULL DEFAULT 0,
  `send_time` bigint NOT NULL DEFAULT 0,
  `delete_time` bigint NOT NULL DEFAULT 0,
  `gold` int NOT NULL DEFAULT 0,
  `won` int NOT NULL DEFAULT 0,
  `ivnum` int UNSIGNED NOT NULL DEFAULT 0,
  `ichangelookvnum` int UNSIGNED NOT NULL DEFAULT 0,
  `icount` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `socket0` int UNSIGNED NOT NULL DEFAULT 0,
  `socket1` int UNSIGNED NOT NULL DEFAULT 0,
  `socket2` int UNSIGNED NOT NULL DEFAULT 0,
  `socket3` int UNSIGNED NOT NULL DEFAULT 0,
  `socket4` int UNSIGNED NOT NULL DEFAULT 0,
  `socket5` int UNSIGNED NOT NULL DEFAULT 0,
  `apply_type0` tinyint NOT NULL DEFAULT 0,
  `apply_value0` smallint NOT NULL DEFAULT 0,
  `apply_path0` tinyint NOT NULL DEFAULT 0,
  `apply_type1` tinyint NOT NULL DEFAULT 0,
  `apply_value1` smallint NOT NULL DEFAULT 0,
  `apply_path1` tinyint NOT NULL DEFAULT 0,
  `apply_type2` tinyint NOT NULL DEFAULT 0,
  `apply_value2` smallint NOT NULL DEFAULT 0,
  `apply_path2` tinyint NOT NULL DEFAULT 0,
  `apply_type3` tinyint NOT NULL DEFAULT 0,
  `apply_value3` smallint NOT NULL DEFAULT 0,
  `apply_path3` tinyint NOT NULL DEFAULT 0,
  `attrtype0` tinyint NOT NULL DEFAULT 0,
  `attrvalue0` smallint NOT NULL DEFAULT 0,
  `attrtype1` tinyint NOT NULL DEFAULT 0,
  `attrvalue1` smallint NOT NULL DEFAULT 0,
  `attrtype2` tinyint NOT NULL DEFAULT 0,
  `attrvalue2` smallint NOT NULL DEFAULT 0,
  `attrtype3` tinyint NOT NULL DEFAULT 0,
  `attrvalue3` smallint NOT NULL DEFAULT 0,
  `attrtype4` tinyint NOT NULL DEFAULT 0,
  `attrvalue4` smallint NOT NULL DEFAULT 0,
  `attrtype5` tinyint NOT NULL DEFAULT 0,
  `attrvalue5` smallint NOT NULL DEFAULT 0,
  `attrtype6` tinyint NOT NULL DEFAULT 0,
  `attrvalue6` smallint NOT NULL DEFAULT 0
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of mailbox
-- ----------------------------

-- ----------------------------
-- Table structure for marriage
-- ----------------------------
DROP TABLE IF EXISTS `marriage`;
CREATE TABLE `marriage`  (
  `is_married` tinyint NOT NULL DEFAULT 0,
  `pid1` int UNSIGNED NOT NULL DEFAULT 0,
  `pid2` int UNSIGNED NOT NULL DEFAULT 0,
  `love_point` int NULL DEFAULT NULL,
  `time` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`pid1`, `pid2`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of marriage
-- ----------------------------

-- ----------------------------
-- Table structure for messenger_block_list
-- ----------------------------
DROP TABLE IF EXISTS `messenger_block_list`;
CREATE TABLE `messenger_block_list`  (
  `account` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `companion` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `time` timestamp(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`account`, `companion`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of messenger_block_list
-- ----------------------------

-- ----------------------------
-- Table structure for messenger_list
-- ----------------------------
DROP TABLE IF EXISTS `messenger_list`;
CREATE TABLE `messenger_list`  (
  `account` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `companion` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`account`, `companion`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of messenger_list
-- ----------------------------

-- ----------------------------
-- Table structure for mob_proto
-- ----------------------------
DROP TABLE IF EXISTS `mob_proto`;
CREATE TABLE `mob_proto`  (
  `vnum` int NOT NULL DEFAULT 0,
  `name` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NoName',
  `locale_name` varbinary(40) NOT NULL DEFAULT 0x4E6F4E616D65,
  `rank` tinyint NOT NULL DEFAULT 0,
  `type` smallint NOT NULL DEFAULT 0,
  `battle_type` tinyint(1) NOT NULL DEFAULT 0,
  `level` smallint NOT NULL DEFAULT 1,
  `size` tinyint NOT NULL DEFAULT 100,
  `ai_flag` set('AGGR','NOMOVE','COWARD','NOATTSHINSU','NOATTCHUNJO','NOATTJINNO','ATTMOB','BERSERK','STONESKIN','GODSPEED','DEATHBLOW','REVIVE','HEALER','COUNT','NORECOVERY','REFLECT','FALL','VIT','RATTSPEED','RCASTSPEED','RHP_REGEN','TIMEVIT') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT '',
  `mount_capacity` tinyint NOT NULL DEFAULT 0,
  `setRaceFlag` set('ANIMAL','UNDEAD','DEVIL','HUMAN','ORC','MILGYO','INSECT','FIRE','ICE','DESERT','TREE','ELEC','WIND','EARTH','DARK','ATT_ELEC','ATT_FIRE','ATT_ICE','ATT_WIND','ATT_EARTH','ATT_DARK') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `setImmuneFlag` set('STUN','SLOW','FALL','CURSE','POISON','TERROR') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `empire` tinyint NOT NULL DEFAULT 0,
  `folder` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `on_click` tinyint NOT NULL DEFAULT 0,
  `st` smallint UNSIGNED NOT NULL DEFAULT 0,
  `dx` smallint UNSIGNED NOT NULL DEFAULT 0,
  `ht` smallint UNSIGNED NOT NULL DEFAULT 0,
  `iq` smallint UNSIGNED NOT NULL DEFAULT 0,
  `damage_min` smallint UNSIGNED NOT NULL DEFAULT 0,
  `damage_max` smallint UNSIGNED NOT NULL DEFAULT 0,
  `max_hp` bigint UNSIGNED NOT NULL DEFAULT 0,
  `regen_cycle` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `regen_percent` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `gold_min` int NOT NULL DEFAULT 0,
  `gold_max` int NOT NULL DEFAULT 0,
  `exp` int UNSIGNED NOT NULL DEFAULT 0,
  `def` smallint UNSIGNED NOT NULL DEFAULT 0,
  `attack_speed` smallint UNSIGNED NOT NULL DEFAULT 100,
  `move_speed` smallint UNSIGNED NOT NULL DEFAULT 100,
  `aggressive_hp_pct` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `aggressive_sight` smallint UNSIGNED NOT NULL DEFAULT 0,
  `attack_range` smallint UNSIGNED NOT NULL DEFAULT 0,
  `drop_item` int UNSIGNED NOT NULL DEFAULT 0,
  `resurrection_vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `enchant_curse` tinyint NOT NULL DEFAULT 0,
  `enchant_slow` tinyint NOT NULL DEFAULT 0,
  `enchant_poison` tinyint NOT NULL DEFAULT 0,
  `enchant_stun` tinyint NOT NULL DEFAULT 0,
  `enchant_critical` tinyint NOT NULL DEFAULT 0,
  `enchant_penetrate` tinyint NOT NULL DEFAULT 0,
  `resist_sword` tinyint NOT NULL DEFAULT 0,
  `resist_twohand` tinyint NOT NULL DEFAULT 0,
  `resist_dagger` tinyint NOT NULL DEFAULT 0,
  `resist_bell` tinyint NOT NULL DEFAULT 0,
  `resist_fan` tinyint NOT NULL DEFAULT 0,
  `resist_bow` tinyint NOT NULL DEFAULT 0,
  `resist_claw` tinyint NOT NULL DEFAULT 0,
  `resist_fire` tinyint NOT NULL DEFAULT 0,
  `resist_elect` tinyint NOT NULL DEFAULT 0,
  `resist_magic` tinyint NOT NULL DEFAULT 0,
  `resist_wind` tinyint NOT NULL DEFAULT 0,
  `resist_poison` tinyint NOT NULL DEFAULT 0,
  `resist_bleeding` tinyint NULL DEFAULT NULL,
  `att_elec` tinyint NOT NULL DEFAULT 0,
  `att_fire` tinyint NOT NULL DEFAULT 0,
  `att_ice` tinyint NOT NULL DEFAULT 0,
  `att_wind` tinyint NOT NULL DEFAULT 0,
  `att_earth` tinyint NOT NULL DEFAULT 0,
  `att_dark` tinyint NOT NULL DEFAULT 0,
  `dam_multiply` float NULL DEFAULT NULL,
  `summon` int NULL DEFAULT NULL,
  `drain_sp` int NULL DEFAULT NULL,
  `mob_color` int UNSIGNED NULL DEFAULT NULL,
  `polymorph_item` int UNSIGNED NOT NULL DEFAULT 0,
  `skill_level0` tinyint UNSIGNED NULL DEFAULT NULL,
  `skill_vnum0` int UNSIGNED NULL DEFAULT NULL,
  `skill_level1` tinyint UNSIGNED NULL DEFAULT NULL,
  `skill_vnum1` int UNSIGNED NULL DEFAULT NULL,
  `skill_level2` tinyint UNSIGNED NULL DEFAULT NULL,
  `skill_vnum2` int UNSIGNED NULL DEFAULT NULL,
  `skill_level3` tinyint UNSIGNED NULL DEFAULT NULL,
  `skill_vnum3` int UNSIGNED NULL DEFAULT NULL,
  `skill_level4` tinyint UNSIGNED NULL DEFAULT NULL,
  `skill_vnum4` int UNSIGNED NULL DEFAULT NULL,
  `sp_berserk` tinyint NOT NULL DEFAULT 0,
  `sp_stoneskin` tinyint NOT NULL DEFAULT 0,
  `sp_godspeed` tinyint NOT NULL DEFAULT 0,
  `sp_deathblow` tinyint NOT NULL DEFAULT 0,
  `sp_revive` tinyint NOT NULL DEFAULT 0,
  PRIMARY KEY (`vnum`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of mob_proto
-- ----------------------------

-- ----------------------------
-- Table structure for monarch
-- ----------------------------
DROP TABLE IF EXISTS `monarch`;
CREATE TABLE `monarch`  (
  `empire` int UNSIGNED NOT NULL DEFAULT 0,
  `pid` int UNSIGNED NULL DEFAULT NULL,
  `windate` datetime(0) NULL DEFAULT NULL,
  `money` bigint UNSIGNED NULL DEFAULT NULL,
  PRIMARY KEY (`empire`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of monarch
-- ----------------------------

-- ----------------------------
-- Table structure for monarch_candidacy
-- ----------------------------
DROP TABLE IF EXISTS `monarch_candidacy`;
CREATE TABLE `monarch_candidacy`  (
  `pid` int UNSIGNED NOT NULL DEFAULT 0,
  `name` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `date` datetime(0) NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`pid`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of monarch_candidacy
-- ----------------------------

-- ----------------------------
-- Table structure for monarch_election
-- ----------------------------
DROP TABLE IF EXISTS `monarch_election`;
CREATE TABLE `monarch_election`  (
  `pid` int UNSIGNED NOT NULL DEFAULT 0,
  `selectedpid` int UNSIGNED NULL DEFAULT 0,
  `electiondata` datetime(0) NULL DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`pid`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of monarch_election
-- ----------------------------

-- ----------------------------
-- Table structure for myshop_pricelist
-- ----------------------------
DROP TABLE IF EXISTS `myshop_pricelist`;
CREATE TABLE `myshop_pricelist`  (
  `owner_id` int UNSIGNED NOT NULL DEFAULT 0,
  `item_vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `price` int UNSIGNED NOT NULL DEFAULT 0,
  `price_cheque` int NOT NULL,
  UNIQUE INDEX `list_id`(`owner_id`, `item_vnum`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of myshop_pricelist
-- ----------------------------

-- ----------------------------
-- Table structure for object
-- ----------------------------
DROP TABLE IF EXISTS `object`;
CREATE TABLE `object`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `land_id` int NOT NULL DEFAULT 0,
  `vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `map_index` int NOT NULL DEFAULT 0,
  `x` int NOT NULL DEFAULT 0,
  `y` int NOT NULL DEFAULT 0,
  `x_rot` float NOT NULL DEFAULT 0,
  `y_rot` float NOT NULL DEFAULT 0,
  `z_rot` float NOT NULL DEFAULT 0,
  `life` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of object
-- ----------------------------

-- ----------------------------
-- Table structure for object_proto
-- ----------------------------
DROP TABLE IF EXISTS `object_proto`;
CREATE TABLE `object_proto`  (
  `vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `name` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `price` int UNSIGNED NOT NULL DEFAULT 0,
  `materials` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `upgrade_vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `upgrade_limit_time` int UNSIGNED NOT NULL DEFAULT 0,
  `life` int NOT NULL DEFAULT 0,
  `reg_1` int NOT NULL DEFAULT 0,
  `reg_2` int NOT NULL DEFAULT 0,
  `reg_3` int NOT NULL DEFAULT 0,
  `reg_4` int NOT NULL DEFAULT 0,
  `npc` int UNSIGNED NOT NULL DEFAULT 0,
  `group_vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `dependent_group` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`vnum`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of object_proto
-- ----------------------------
INSERT INTO `object_proto` VALUES (14003, 'Factory', 20000000, '90010,15/90011,25/90012,30', 0, 0, 0, -528, -411, 528, 411, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14013, 'Weaponsmith', 20000000, '90010,20/90011,30/90012,20', 0, 0, 0, -525, -525, 525, 525, 20044, 2, 1);
INSERT INTO `object_proto` VALUES (14014, 'Armoursmith', 20000000, '90010,20/90011,30/90012,20', 0, 0, 0, -525, -525, 525, 525, 20045, 2, 1);
INSERT INTO `object_proto` VALUES (14015, 'Jeweller', 20000000, '90010,20/90011,30/90012,20', 0, 0, 0, -525, -525, 525, 525, 20046, 2, 1);
INSERT INTO `object_proto` VALUES (14023, 'Headquarters', 20000000, '90010,30/90011,35/90012,35', 0, 0, 0, -457, -495, 457, 530, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14033, 'Training Centre', 20000000, '90010,25/90011,25/90012,25', 0, 0, 0, -833, -473, 833, 473, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14043, 'Diamond Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20060, 3, 1);
INSERT INTO `object_proto` VALUES (14044, 'Amber Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20061, 3, 1);
INSERT INTO `object_proto` VALUES (14045, 'Fossil Wood Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20062, 3, 1);
INSERT INTO `object_proto` VALUES (14046, 'Copper Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20063, 3, 1);
INSERT INTO `object_proto` VALUES (14047, 'Silver Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20064, 3, 1);
INSERT INTO `object_proto` VALUES (14048, 'Gold Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20065, 3, 1);
INSERT INTO `object_proto` VALUES (14049, 'Jade Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20066, 3, 1);
INSERT INTO `object_proto` VALUES (14050, 'Ebony Stone Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20067, 3, 1);
INSERT INTO `object_proto` VALUES (14051, 'Pearl Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20068, 3, 1);
INSERT INTO `object_proto` VALUES (14052, 'White Gold Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20069, 3, 1);
INSERT INTO `object_proto` VALUES (14053, 'Crystal Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20070, 3, 1);
INSERT INTO `object_proto` VALUES (14054, 'Amethyst Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20071, 3, 1);
INSERT INTO `object_proto` VALUES (14055, 'Heavens Tear Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20072, 3, 1);
INSERT INTO `object_proto` VALUES (14061, 'Altar of Power', 25000000, '90010,25/90011,25/90012,25', 0, 0, 0, -524, -996, 528, 751, 20077, 4, 1);
INSERT INTO `object_proto` VALUES (14062, 'Altar of Power', 500000000, '90010,50/90011,50/90012,50', 0, 0, 0, -586, -996, 587, 819, 20078, 4, 4);
INSERT INTO `object_proto` VALUES (14063, 'Altar of Power', 750000000, '90010,75/90011,75/90012,75', 0, 0, 0, -665, -996, 666, 897, 20079, 4, 4);
INSERT INTO `object_proto` VALUES (14071, 'Guild Warehouse', 25000000, '90010,25/90011,25/90012,25', 0, 0, 0, -524, -996, 528, 751, 60000, 6, 1);
INSERT INTO `object_proto` VALUES (14072, 'Guild Warehouse', 500000000, '90010,50/90011,50/90012,50', 0, 0, 0, -586, -996, 587, 819, 60001, 6, 6);
INSERT INTO `object_proto` VALUES (14073, 'Guild Warehouse', 750000000, '90010,75/90011,75/90012,75', 0, 0, 0, -665, -996, 666, 897, 60002, 6, 6);
INSERT INTO `object_proto` VALUES (14074, 'Soul Crystal Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20423, 3, 1);
INSERT INTO `object_proto` VALUES (14075, 'Ruby Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20424, 3, 1);
INSERT INTO `object_proto` VALUES (14076, 'Garnet Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20425, 3, 1);
INSERT INTO `object_proto` VALUES (14077, 'Emerald Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20426, 3, 1);
INSERT INTO `object_proto` VALUES (14078, 'Sapphire Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20427, 3, 1);
INSERT INTO `object_proto` VALUES (14079, 'Tourmaline Smelter', 20000000, '90010,15/90011,20/90012,25', 0, 0, 0, -180, -200, 180, 200, 20428, 3, 1);
INSERT INTO `object_proto` VALUES (14100, 'House (1)', 20000000, '90010,20/90011,30/90012,25', 0, 0, 0, -677, -894, 690, 749, 0, 1, 0);
INSERT INTO `object_proto` VALUES (14101, 'Stone Wall (1-1)', 2000000, '90010,15/90011,5/90012,5', 0, 0, 0, -400, -154, 156, 400, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14102, 'Stone Wall (1-2)', 1000000, '90010,15/90011,5/90012,5', 0, 0, 0, -200, -107, 300, 107, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14103, 'Stone Wall (1-3)', 1000000, '90010,15/90011,5/90012,5', 0, 0, 0, -500, -107, 500, 107, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14104, 'Gate (1)', 2500000, '90010,5/90011,15/90012,10', 0, 0, 0, -1000, -275, 1000, 275, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14105, 'Wall (1)', 2500000, '90010,5/90011,15/90012,10', 0, 0, 0, -1000, -275, 1000, 275, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14110, 'House (2)', 20000000, '90010,20/90011,30/90012,25', 0, 0, 0, -679, -894, 688, 749, 0, 1, 0);
INSERT INTO `object_proto` VALUES (14111, 'Stone Wall (2-1)', 2000000, '90010,15/90011,5/90012,5', 0, 0, 0, -400, -154, 156, 400, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14112, 'Stone Wall (2-2)', 1000000, '90010,15/90011,5/90012,5', 0, 0, 0, -200, -107, 300, 107, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14113, 'Stone Wall (2-3)', 1000000, '90010,15/90011,5/90012,5', 0, 0, 0, -500, -107, 500, 107, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14114, 'Gate (2)', 2500000, '90010,5/90011,15/90012,10', 0, 0, 0, -1000, -275, 1000, 275, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14115, 'Wall (2)', 2500000, '90010,5/90011,15/90012,10', 0, 0, 0, -1000, -275, 1000, 275, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14120, 'House (3)', 20000000, '90010,20/90011,30/90012,25', 0, 0, 0, -677, -894, 690, 749, 0, 1, 0);
INSERT INTO `object_proto` VALUES (14121, 'Stone Wall (3-1)', 2000000, '90010,15/90011,5/90012,5', 0, 0, 0, -400, -154, 156, 400, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14122, 'Stone Wall (3-2)', 1000000, '90010,15/90011,5/90012,5', 0, 0, 0, -200, -107, 300, 107, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14123, 'Stone Wall (3-3)', 1000000, '90010,15/90011,5/90012,5', 0, 0, 0, -500, -107, 500, 107, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14124, 'Gate (3)', 2500000, '90010,5/90011,15/90012,10', 0, 0, 0, -1000, -275, 1000, 275, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14125, 'Wall (3)', 2500000, '90010,5/90011,15/90012,10', 0, 0, 0, -1000, -275, 1000, 275, 0, 5, 0);
INSERT INTO `object_proto` VALUES (14140, 'Watch Tower', 3000000, '90010,20/90011,30/90012,35', 0, 0, 0, -188, -221, 188, 221, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14141, 'Wooden Wall (1-1)', 800000, '90010,5/90011,15/90012,15', 0, 0, 0, -4, -80, 349, 80, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14142, 'Wooden Wall (1-2)', 500000, '90010,5/90011,15/90012,15', 0, 0, 0, -72, -72, 72, 72, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14143, 'Wooden Wall (1-3)', 800000, '90010,5/90011,15/90012,15', 0, 0, 0, -391, -105, 4, 105, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14144, 'Gate (4)', 2000000, '90010,5/90011,20/90012,20', 0, 0, 0, -2, -192, 696, 192, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14145, 'Wall (4)', 2500000, '90010,5/90011,15/90012,10', 0, 0, 0, -1000, -275, 1000, 275, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14200, 'Guild Insignia', 3000000, '90011,5/90012,5', 0, 0, 0, -46, -9, 43, 40, 0, 0, 1);
INSERT INTO `object_proto` VALUES (14201, 'Wall Front', 3000000, '90010.50', 0, 0, 0, -1000, -54, 1000, 20, 0, 0, 1);
INSERT INTO `object_proto` VALUES (14202, 'Wall Back', 3000000, '90010.50', 0, 0, 0, -1000, -18, 1000, 18, 0, 0, 1);
INSERT INTO `object_proto` VALUES (14203, 'Left Side of Wall', 3000000, '90010.50', 0, 0, 0, -19, -1362, 19, 1362, 0, 0, 1);
INSERT INTO `object_proto` VALUES (14204, 'Right Side of Wall', 3000000, '90010.50', 0, 0, 0, -18, -1362, 18, 1362, 0, 0, 1);
INSERT INTO `object_proto` VALUES (14300, 'Stone (1)', 300000, '90010,5', 0, 0, 0, -116, -118, 116, 118, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14301, 'Stone (2)', 300000, '90010,5', 0, 0, 0, -118, -133, 118, 133, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14302, 'Stone (3)', 300000, '90010,7', 0, 0, 0, -133, -103, 133, 103, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14303, 'Stone (4)', 300000, '90010,7', 0, 0, 0, -104, -118, 104, 118, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14304, 'Stone (5)', 300000, '90010,7', 0, 0, 0, -172, -146, 172, 146, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14305, 'Stone (6)', 300000, '90010,8', 0, 0, 0, -193, -202, 193, 202, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14306, 'Stone (7)', 300000, '90010,8', 0, 0, 0, -184, -184, 184, 184, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14307, 'Stone (8)', 300000, '90010,8', 0, 0, 0, -67, -57, 67, 57, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14308, 'Stone (9)', 300000, '90010,9', 0, 0, 0, -66, -53, 66, 53, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14309, 'Stone (10)', 300000, '90010,9', 0, 0, 0, -182, -123, 182, 123, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14400, 'Tree (1)', 2000000, '90011,5', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14401, 'Tree (2)', 2000000, '90011,5', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14402, 'Tree (3)', 2000000, '90011,7', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14403, 'Tree (4)', 2000000, '90011,7', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14404, 'Tree (5)', 2000000, '90011,7', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14405, 'Tree (6)', 2000000, '90011,9', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14406, 'Tree (7)', 2000000, '90011,9', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14407, 'Tree (8)', 2000000, '90011,9', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `object_proto` VALUES (14408, 'Tree (9)', 2000000, '90011,9', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
INSERT INTO `object_proto` VALUES (20139, 'Guild Banner (Purple)', 1000000, '90011', 0, 0, 0, -100, -100, 100, 100, 0, 0, 0);
INSERT INTO `object_proto` VALUES (20140, 'Guild Banner (Blue/Green)', 1000000, '90011', 0, 0, 0, -100, -100, 100, 100, 0, 0, 0);
INSERT INTO `object_proto` VALUES (20141, 'Guild Banner (Red/Gold)', 1000000, '90011', 0, 0, 0, -100, -100, 100, 100, 0, 0, 0);

-- ----------------------------
-- Table structure for okey_event
-- ----------------------------
DROP TABLE IF EXISTS `okey_event`;
CREATE TABLE `okey_event`  (
  `pid` int NULL DEFAULT NULL,
  `name` varchar(11) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `points` int NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of okey_event
-- ----------------------------

-- ----------------------------
-- Table structure for pcbang_ip
-- ----------------------------
DROP TABLE IF EXISTS `pcbang_ip`;
CREATE TABLE `pcbang_ip`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `pcbang_id` int NOT NULL DEFAULT 0,
  `ip` varchar(15) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '000.000.000.000',
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `ip`(`ip`) USING BTREE,
  INDEX `pcbang_id`(`pcbang_id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of pcbang_ip
-- ----------------------------

-- ----------------------------
-- Table structure for player
-- ----------------------------
DROP TABLE IF EXISTS `player`;
CREATE TABLE `player`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `account_id` int NOT NULL DEFAULT 0,
  `name` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NONAME',
  `job` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `voice` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `dir` tinyint NOT NULL DEFAULT 0,
  `x` int NOT NULL DEFAULT 0,
  `y` int NOT NULL DEFAULT 0,
  `z` int NOT NULL DEFAULT 0,
  `map_index` int NOT NULL DEFAULT 0,
  `exit_x` int NOT NULL DEFAULT 0,
  `exit_y` int NOT NULL DEFAULT 0,
  `exit_map_index` int NOT NULL DEFAULT 0,
  `hp` bigint NOT NULL DEFAULT 0,
  `mp` bigint NOT NULL DEFAULT 0,
  `stamina` smallint NOT NULL DEFAULT 0,
  `random_hp` smallint UNSIGNED NOT NULL DEFAULT 0,
  `random_sp` smallint UNSIGNED NOT NULL DEFAULT 0,
  `playtime` int NOT NULL DEFAULT 0,
  `level` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `level_step` tinyint(1) NOT NULL DEFAULT 0,
  `st` smallint NOT NULL DEFAULT 0,
  `ht` smallint NOT NULL DEFAULT 0,
  `dx` smallint NOT NULL DEFAULT 0,
  `iq` smallint NOT NULL DEFAULT 0,
  `exp` int NOT NULL DEFAULT 0,
  `gold` bigint NOT NULL DEFAULT 0,
  `cheque` int NOT NULL DEFAULT 0,
  `gem` int UNSIGNED NOT NULL DEFAULT 0,
  `stat_point` smallint NOT NULL DEFAULT 0,
  `skill_point` smallint NOT NULL DEFAULT 0,
  `quickslot` tinyblob NULL,
  `ip` varchar(15) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT '0.0.0.0',
  `part_main` int UNSIGNED NOT NULL DEFAULT 0,
  `part_base` int NOT NULL DEFAULT 0,
  `part_hair` int NOT NULL DEFAULT 0,
  `part_acce` int NOT NULL DEFAULT 0,
  `skill_group` tinyint NOT NULL DEFAULT 0,
  `skill_level` blob NULL,
  `alignment` int NOT NULL DEFAULT 0,
  `conqueror_level` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `conqueror_level_step` tinyint(1) NOT NULL,
  `sungma_str` smallint NOT NULL,
  `sungma_hp` smallint NOT NULL,
  `sungma_move` smallint NOT NULL,
  `sungma_immune` smallint NOT NULL,
  `conqueror_exp` int NOT NULL,
  `conqueror_point` smallint NOT NULL,
  `last_play` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `change_name` tinyint(1) NOT NULL DEFAULT 0,
  `sub_skill_point` smallint NOT NULL DEFAULT 0,
  `stat_reset_count` tinyint NOT NULL DEFAULT 0,
  `horse_hp` smallint NOT NULL DEFAULT 0,
  `horse_stamina` smallint NOT NULL DEFAULT 0,
  `horse_level` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `horse_hp_droptime` int UNSIGNED NOT NULL DEFAULT 0,
  `horse_riding` tinyint(1) NOT NULL DEFAULT 0,
  `horse_skill_point` smallint NOT NULL DEFAULT 0,
  `shop_gold` bigint NOT NULL DEFAULT 0,
  `shop_cheque` int NOT NULL DEFAULT 0,
  `pin` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '0000',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `account_id_idx`(`account_id`) USING BTREE,
  INDEX `name_idx`(`name`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of player
-- ----------------------------

-- ----------------------------
-- Table structure for player_deleted
-- ----------------------------
DROP TABLE IF EXISTS `player_deleted`;
CREATE TABLE `player_deleted`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `account_id` int NOT NULL DEFAULT 0,
  `name` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NONAME',
  `job` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `voice` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `dir` tinyint NOT NULL DEFAULT 0,
  `x` int NOT NULL DEFAULT 0,
  `y` int NOT NULL DEFAULT 0,
  `z` int NOT NULL DEFAULT 0,
  `map_index` int NOT NULL DEFAULT 0,
  `exit_x` int NOT NULL DEFAULT 0,
  `exit_y` int NOT NULL DEFAULT 0,
  `exit_map_index` int NOT NULL DEFAULT 0,
  `hp` bigint NOT NULL DEFAULT 0,
  `mp` bigint NOT NULL DEFAULT 0,
  `stamina` smallint NOT NULL DEFAULT 0,
  `random_hp` smallint UNSIGNED NOT NULL DEFAULT 0,
  `random_sp` smallint UNSIGNED NOT NULL DEFAULT 0,
  `playtime` int NOT NULL DEFAULT 0,
  `level` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `level_step` tinyint(1) NOT NULL DEFAULT 0,
  `st` smallint NOT NULL DEFAULT 0,
  `ht` smallint NOT NULL DEFAULT 0,
  `dx` smallint NOT NULL DEFAULT 0,
  `iq` smallint NOT NULL DEFAULT 0,
  `exp` int NOT NULL DEFAULT 0,
  `gold` bigint NOT NULL DEFAULT 0,
  `cheque` int NOT NULL DEFAULT 0,
  `gem` int UNSIGNED NOT NULL DEFAULT 0,
  `stat_point` smallint NOT NULL DEFAULT 0,
  `skill_point` smallint NOT NULL DEFAULT 0,
  `quickslot` tinyblob NULL,
  `ip` varchar(15) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT '0.0.0.0',
  `part_main` smallint UNSIGNED NOT NULL DEFAULT 0,
  `part_base` tinyint NOT NULL DEFAULT 0,
  `part_hair` smallint NOT NULL DEFAULT 0,
  `part_acce` smallint NOT NULL DEFAULT 0,
  `skill_group` tinyint NOT NULL DEFAULT 0,
  `skill_level` blob NULL,
  `alignment` int NOT NULL DEFAULT 0,
  `conqueror_level` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `conqueror_level_step` tinyint(1) NOT NULL,
  `sungma_str` smallint NOT NULL,
  `sungma_hp` smallint NOT NULL,
  `sungma_move` smallint NOT NULL,
  `sungma_immune` smallint NOT NULL,
  `conqueror_exp` int NOT NULL,
  `conqueror_point` smallint NOT NULL,
  `last_play` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `change_name` tinyint(1) NOT NULL DEFAULT 0,
  `sub_skill_point` smallint NOT NULL DEFAULT 0,
  `stat_reset_count` tinyint NOT NULL DEFAULT 0,
  `horse_hp` smallint NOT NULL DEFAULT 0,
  `horse_stamina` smallint NOT NULL DEFAULT 0,
  `horse_level` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `horse_hp_droptime` int UNSIGNED NOT NULL DEFAULT 0,
  `horse_riding` tinyint(1) NOT NULL DEFAULT 0,
  `horse_skill_point` smallint NOT NULL DEFAULT 0,
  `shop_gold` bigint NOT NULL DEFAULT 0,
  `shop_cheque` int NOT NULL DEFAULT 0,
  `pin` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '0000',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `account_id_idx`(`account_id`) USING BTREE,
  INDEX `name_idx`(`name`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of player_deleted
-- ----------------------------

-- ----------------------------
-- Table structure for player_index
-- ----------------------------
DROP TABLE IF EXISTS `player_index`;
CREATE TABLE `player_index`  (
  `id` int NOT NULL DEFAULT 0,
  `pid1` int NOT NULL DEFAULT 0,
  `pid2` int NOT NULL DEFAULT 0,
  `pid3` int NOT NULL DEFAULT 0,
  `pid4` int NOT NULL DEFAULT 0,
  `pid5` int NOT NULL DEFAULT 0,
  `empire` tinyint NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `id unique`(`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of player_index
-- ----------------------------

-- ----------------------------
-- Table structure for quest
-- ----------------------------
DROP TABLE IF EXISTS `quest`;
CREATE TABLE `quest`  (
  `dwPID` int UNSIGNED NOT NULL DEFAULT 0,
  `szName` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szState` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `lValue` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`dwPID`, `szName`, `szState`) USING BTREE,
  INDEX `pid_idx`(`dwPID`) USING BTREE,
  INDEX `name_idx`(`szName`) USING BTREE,
  INDEX `state_idx`(`szState`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of quest
-- ----------------------------

-- ----------------------------
-- Table structure for refine_proto
-- ----------------------------
DROP TABLE IF EXISTS `refine_proto`;
CREATE TABLE `refine_proto`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `vnum0` int UNSIGNED NOT NULL DEFAULT 0,
  `count0` smallint NOT NULL DEFAULT 0,
  `vnum1` int UNSIGNED NOT NULL DEFAULT 0,
  `count1` smallint NOT NULL DEFAULT 0,
  `vnum2` int UNSIGNED NOT NULL DEFAULT 0,
  `count2` smallint NOT NULL DEFAULT 0,
  `vnum3` int UNSIGNED NOT NULL DEFAULT 0,
  `count3` smallint NOT NULL DEFAULT 0,
  `vnum4` int UNSIGNED NOT NULL DEFAULT 0,
  `count4` smallint NOT NULL DEFAULT 0,
  `cost` int NOT NULL DEFAULT 0,
  `src_vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `result_vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `prob` smallint NOT NULL DEFAULT 100,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1000 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of refine_proto
-- ----------------------------
INSERT INTO `refine_proto` VALUES (1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (6, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (7, 30053, 1, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (8, 30073, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (9, 30033, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (10, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (11, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (12, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (13, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (14, 30030, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (15, 30075, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (16, 30056, 2, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (17, 30022, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (18, 30067, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (19, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (20, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (21, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (22, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (23, 30079, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (24, 30015, 1, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (25, 30016, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (26, 30089, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (27, 30091, 1, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (28, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (29, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (30, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (31, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (32, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (33, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (34, 30034, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (35, 30011, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (36, 30035, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (37, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (38, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (39, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (40, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (41, 30052, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (42, 30105, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (43, 30045, 2, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (44, 30025, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (45, 30058, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 30);
INSERT INTO `refine_proto` VALUES (46, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 90);
INSERT INTO `refine_proto` VALUES (47, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 80);
INSERT INTO `refine_proto` VALUES (48, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (49, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (50, 30083, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (51, 30060, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (52, 30116, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (53, 30088, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (54, 30019, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (55, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (56, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (57, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (58, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (59, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (60, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (61, 30070, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (62, 30053, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (63, 30017, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (64, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (65, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (66, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (67, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (68, 30041, 1, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (69, 30008, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (70, 30057, 1, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (71, 30081, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (72, 30077, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (73, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (74, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (75, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (76, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (77, 30009, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (78, 30039, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (79, 30040, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (80, 30048, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (81, 30090, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (82, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (83, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (84, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (85, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (86, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (87, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (88, 30031, 1, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (89, 30032, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (90, 30033, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (91, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (92, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (93, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (94, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (95, 30008, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (96, 30075, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (97, 30006, 2, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (98, 30030, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (99, 30067, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (100, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (101, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (102, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (103, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (104, 30079, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (105, 30015, 1, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (106, 30086, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (107, 30050, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (108, 30091, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (109, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (110, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (111, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (112, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (113, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (114, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (115, 30071, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (116, 30018, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (117, 30017, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (118, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (119, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (120, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (121, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (122, 30005, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (123, 30076, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (124, 30051, 1, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (125, 30081, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (126, 30047, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (127, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (128, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (129, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (130, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (131, 30009, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (132, 30080, 1, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (133, 30040, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (134, 30049, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (135, 30090, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (136, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (137, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (138, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (139, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (140, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (141, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (142, 30038, 1, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (143, 30023, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (144, 30074, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (145, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (146, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (147, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (148, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (149, 30007, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (150, 30078, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (151, 30045, 2, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (152, 30055, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (153, 30047, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (154, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (155, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (156, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (157, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (158, 30059, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (159, 30080, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (160, 30085, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (161, 30014, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (162, 30042, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (163, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (164, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (165, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (166, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (167, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (168, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (169, 30010, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (170, 30031, 1, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (171, 30032, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (172, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (173, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (174, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (175, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (176, 30021, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (177, 30008, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (178, 30057, 2, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (179, 30006, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (180, 30022, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (181, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (182, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (183, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (184, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (185, 30082, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (186, 30079, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (187, 30087, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (188, 30086, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (189, 30050, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (190, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (191, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (192, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (193, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (194, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (195, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (196, 30038, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (197, 30011, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (198, 30074, 1, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (199, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (200, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (201, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (202, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (203, 30007, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (204, 30046, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (205, 30045, 2, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (206, 30055, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (207, 30058, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (208, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (209, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (210, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (211, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (212, 30059, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (213, 30060, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (214, 30116, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (215, 30014, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (216, 30042, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (217, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (218, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (219, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (220, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (221, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (222, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (223, 30071, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (224, 30023, 1, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (225, 30074, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (226, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (227, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (228, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (229, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (230, 30005, 1, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (231, 30078, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (232, 30051, 1, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (233, 30055, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (234, 30047, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (235, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (236, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (237, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (238, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (239, 30059, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (240, 30080, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (241, 30039, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (242, 30049, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (243, 30042, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (244, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (245, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (246, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (247, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (248, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (249, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (250, 30070, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (251, 30018, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (252, 30017, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (253, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (254, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (255, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (256, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (257, 30041, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (258, 30076, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (259, 30051, 2, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (260, 30081, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (261, 30077, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (262, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (263, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (264, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (265, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (266, 30009, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (267, 30039, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (268, 30040, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (269, 30048, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (270, 30090, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (271, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (272, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (273, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (274, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (275, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (276, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (277, 30010, 1, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (278, 30072, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (279, 30032, 1, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (280, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (281, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (282, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (283, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (284, 30021, 1, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (285, 30008, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (286, 30057, 1, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (287, 30006, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (288, 30077, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (289, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (290, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (291, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (292, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (293, 30082, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (294, 30039, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (295, 30087, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (296, 30086, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (297, 30050, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (298, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (299, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (300, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (301, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (302, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (303, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (304, 30053, 1, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (305, 30073, 1, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (306, 30035, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (307, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (308, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (309, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (310, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (311, 30030, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (312, 30075, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (313, 30056, 2, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (314, 30025, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (315, 30067, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (316, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (317, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (318, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (319, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (320, 30083, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (321, 30015, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (322, 30016, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (323, 30089, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (324, 30019, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (325, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 600, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (326, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (327, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (328, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (329, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (330, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (331, 30034, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (332, 30073, 2, 0, 0, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (333, 30035, 2, 0, 0, 0, 0, 0, 0, 0, 0, 75000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (334, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (335, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (336, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 4000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (337, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 8000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (338, 30052, 2, 0, 0, 0, 0, 0, 0, 0, 0, 13000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (339, 30046, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (340, 30056, 2, 27799, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (341, 30025, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 70000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (342, 30058, 2, 27987, 1, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (343, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (344, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (345, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (346, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (347, 30083, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (348, 30060, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (349, 30116, 2, 30193, 2, 0, 0, 0, 0, 0, 0, 45000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (350, 30088, 2, 30193, 4, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (351, 30019, 2, 30193, 8, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (352, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (353, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 6000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (354, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 12000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (355, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 25000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (356, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (357, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 60000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (358, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (359, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (360, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (361, 33030, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (362, 33030, 2, 30611, 1, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (363, 33030, 4, 30611, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (364, 33030, 8, 30611, 2, 0, 0, 0, 0, 0, 0, 80000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (365, 33030, 16, 30611, 2, 0, 0, 0, 0, 0, 0, 160000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (366, 33030, 32, 30611, 3, 0, 0, 0, 0, 0, 0, 320000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (367, 33030, 64, 30616, 1, 0, 0, 0, 0, 0, 0, 640000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (368, 33030, 128, 30617, 1, 0, 0, 0, 0, 0, 0, 1280000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (369, 33030, 200, 30618, 1, 0, 0, 0, 0, 0, 0, 2560000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (390, 33029, 1, 0, 0, 0, 0, 0, 0, 0, 0, 10000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (391, 33029, 2, 30612, 1, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (392, 33029, 4, 30612, 1, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (393, 33029, 8, 30612, 2, 0, 0, 0, 0, 0, 0, 80000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (394, 33029, 16, 30612, 2, 0, 0, 0, 0, 0, 0, 160000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (395, 33029, 32, 30612, 3, 0, 0, 0, 0, 0, 0, 320000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (396, 33029, 64, 30616, 1, 0, 0, 0, 0, 0, 0, 640000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (397, 33029, 128, 30617, 1, 0, 0, 0, 0, 0, 0, 1280000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (398, 33029, 200, 30618, 1, 0, 0, 0, 0, 0, 0, 2560000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (400, 30192, 2, 0, 0, 0, 0, 0, 0, 0, 0, 20000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (401, 30194, 2, 0, 0, 0, 0, 0, 0, 0, 0, 30000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (421, 30604, 2, 0, 0, 0, 0, 0, 0, 0, 0, 100000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (422, 30604, 2, 30612, 1, 0, 0, 0, 0, 0, 0, 125000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (423, 30604, 3, 30612, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (424, 30604, 3, 30612, 2, 0, 0, 0, 0, 0, 0, 180000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (425, 30604, 3, 30612, 2, 0, 0, 0, 0, 0, 0, 225000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (426, 30604, 4, 30612, 3, 0, 0, 0, 0, 0, 0, 270000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (427, 30610, 4, 30616, 1, 0, 0, 0, 0, 0, 0, 320000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (428, 30610, 4, 30617, 1, 0, 0, 0, 0, 0, 0, 400000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (429, 30610, 5, 30618, 1, 0, 0, 0, 0, 0, 0, 500000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (431, 30604, 2, 0, 0, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (432, 30604, 2, 30612, 1, 0, 0, 0, 0, 0, 0, 165000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (433, 30604, 3, 30612, 1, 0, 0, 0, 0, 0, 0, 190000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (434, 30604, 3, 30612, 2, 0, 0, 0, 0, 0, 0, 235000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (435, 30604, 3, 30612, 2, 0, 0, 0, 0, 0, 0, 280000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (436, 30604, 4, 30612, 3, 0, 0, 0, 0, 0, 0, 330000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (437, 30610, 4, 30616, 1, 0, 0, 0, 0, 0, 0, 380000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (438, 30610, 4, 30617, 1, 0, 0, 0, 0, 0, 0, 500000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (439, 30610, 5, 30618, 1, 0, 0, 0, 0, 0, 0, 600000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (440, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (441, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (442, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (443, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (444, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (445, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (446, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (447, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (448, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (449, 33031, 10, 9600, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (450, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (451, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (452, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (453, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (454, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (455, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (456, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (457, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (458, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (459, 33031, 10, 10750, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (460, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (461, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (462, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (463, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (464, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (465, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (466, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (467, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (468, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (469, 33031, 10, 9830, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (470, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (471, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (472, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (473, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (474, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (475, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (476, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (477, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (478, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (479, 33031, 10, 10520, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (480, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (481, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (482, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (483, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (484, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (485, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (486, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (487, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (488, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (489, 33031, 10, 10060, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (490, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (491, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (492, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (493, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (494, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (495, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (496, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (497, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (498, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (499, 33031, 10, 10290, 1, 30031, 1, 0, 0, 0, 0, 50000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (501, 25040, 2, 70031, 1, 0, 0, 0, 0, 0, 0, 1000000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (502, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (503, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 140000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (504, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 170000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (505, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 200000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (506, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 240000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (507, 30050, 2, 0, 0, 0, 0, 0, 0, 0, 0, 290000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (508, 30089, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 340000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (509, 30040, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 410000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (510, 30089, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 500000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (601, 51001, 10, 0, 0, 0, 0, 0, 0, 0, 0, 100000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (602, 51001, 15, 0, 0, 0, 0, 0, 0, 0, 0, 125000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (603, 51001, 20, 0, 0, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (604, 51001, 30, 0, 0, 0, 0, 0, 0, 0, 0, 180000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (605, 51001, 45, 0, 0, 0, 0, 0, 0, 0, 0, 225000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (606, 51001, 65, 0, 0, 0, 0, 0, 0, 0, 0, 270000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (607, 51001, 95, 0, 0, 0, 0, 0, 0, 0, 0, 320000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (608, 51001, 140, 0, 0, 0, 0, 0, 0, 0, 0, 400000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (609, 51001, 200, 0, 0, 0, 0, 0, 0, 0, 0, 500000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (610, 25040, 2, 70031, 3, 51001, 100, 0, 0, 0, 0, 5000000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (611, 30604, 1, 0, 0, 0, 0, 0, 0, 0, 0, 7500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (612, 30604, 1, 30610, 1, 0, 0, 0, 0, 0, 0, 15000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (613, 30604, 2, 30610, 1, 0, 0, 0, 0, 0, 0, 25000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (614, 30604, 2, 30610, 2, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (615, 30604, 3, 30610, 2, 0, 0, 0, 0, 0, 0, 65000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (616, 30604, 3, 30610, 3, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (617, 30616, 1, 30610, 4, 0, 0, 0, 0, 0, 0, 120000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (618, 30617, 1, 30610, 4, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (619, 30618, 1, 30610, 4, 0, 0, 0, 0, 0, 0, 250000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (620, 30623, 3, 30624, 2, 25040, 2, 0, 0, 0, 0, 2000000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (621, 30626, 1, 30629, 1, 25040, 2, 0, 0, 0, 0, 330000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (622, 30626, 1, 30629, 1, 25040, 2, 0, 0, 0, 0, 425000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (623, 30626, 2, 30629, 2, 25040, 2, 0, 0, 0, 0, 540000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (624, 30626, 2, 30629, 2, 25040, 2, 0, 0, 0, 0, 720000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (625, 30626, 3, 30629, 3, 25040, 2, 0, 0, 0, 0, 800000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (626, 30627, 2, 30629, 3, 25040, 2, 0, 0, 0, 0, 900000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (627, 30627, 2, 27992, 1, 0, 0, 0, 0, 0, 0, 950000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (628, 30627, 3, 27993, 1, 0, 0, 0, 0, 0, 0, 1300000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (629, 30624, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 1500000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (630, 30623, 3, 30624, 2, 25040, 2, 0, 0, 0, 0, 2000000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (530, 71123, 3, 71129, 4, 0, 0, 0, 0, 0, 0, 200000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (531, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 7500, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (532, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 15000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (533, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 25000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (534, 30196, 2, 0, 0, 0, 0, 0, 0, 0, 0, 40000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (535, 30090, 3, 0, 0, 0, 0, 0, 0, 0, 0, 65000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (536, 30195, 2, 0, 0, 0, 0, 0, 0, 0, 0, 90000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (537, 30197, 4, 27992, 1, 0, 0, 0, 0, 0, 0, 12000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (538, 30198, 2, 27993, 1, 0, 0, 0, 0, 0, 0, 150000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (539, 30199, 2, 27994, 1, 0, 0, 0, 0, 0, 0, 200000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (631, 30625, 1, 30629, 1, 0, 0, 0, 0, 0, 0, 330000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (632, 30625, 1, 30629, 1, 0, 0, 0, 0, 0, 0, 425000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (633, 30625, 2, 30629, 2, 0, 0, 0, 0, 0, 0, 540000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (634, 30625, 2, 30629, 2, 0, 0, 0, 0, 0, 0, 720000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (635, 30625, 3, 30629, 3, 0, 0, 0, 0, 0, 0, 800000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (636, 30628, 2, 30629, 3, 0, 0, 0, 0, 0, 0, 200000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (637, 30628, 2, 30616, 2, 0, 0, 0, 0, 0, 0, 950000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (638, 30628, 3, 30617, 2, 0, 0, 0, 0, 0, 0, 1300000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (639, 30624, 2, 30618, 2, 0, 0, 0, 0, 0, 0, 1500000, 0, 0, 100);
INSERT INTO `refine_proto` VALUES (999, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

-- ----------------------------
-- Table structure for safebox
-- ----------------------------
DROP TABLE IF EXISTS `safebox`;
CREATE TABLE `safebox`  (
  `account_id` int UNSIGNED NOT NULL DEFAULT 0,
  `size` smallint UNSIGNED NOT NULL DEFAULT 0,
  `password` varchar(6) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `gold` int NOT NULL DEFAULT 0,
  `buff_point` int NOT NULL DEFAULT 0,
  `buff_value` int NOT NULL DEFAULT 0,
  `buff_duration` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`account_id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of safebox
-- ----------------------------

-- ----------------------------
-- Table structure for shop
-- ----------------------------
DROP TABLE IF EXISTS `shop`;
CREATE TABLE `shop`  (
  `vnum` int NOT NULL DEFAULT 0,
  `name` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NoName',
  `npc_vnum` smallint NOT NULL DEFAULT 0,
  PRIMARY KEY (`vnum`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of shop
-- ----------------------------
INSERT INTO `shop` VALUES (1, 'weapon_shop', 9001);
INSERT INTO `shop` VALUES (2, 'fish_shop', 9009);
INSERT INTO `shop` VALUES (3, 'general_shop', 9003);
INSERT INTO `shop` VALUES (4, 'armor_shop', 9002);
INSERT INTO `shop` VALUES (5, 'weapon_shop2', 9007);
INSERT INTO `shop` VALUES (6, 'armor_shop2', 9008);
INSERT INTO `shop` VALUES (12, 'dragonsoul_shop', 20001);
INSERT INTO `shop` VALUES (7, 'gold_bar_shop', 9005);
INSERT INTO `shop` VALUES (8, 'firework_shop', 9004);
INSERT INTO `shop` VALUES (9, 'peddler_shop', 20042);
INSERT INTO `shop` VALUES (10, 'pick_shop', 20015);
INSERT INTO `shop` VALUES (11, 'stable_shop', 20349);

-- ----------------------------
-- Table structure for shop_class
-- ----------------------------
DROP TABLE IF EXISTS `shop_class`;
CREATE TABLE `shop_class`  (
  `classid` int NOT NULL AUTO_INCREMENT,
  `classname` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  PRIMARY KEY (`classid`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 14 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of shop_class
-- ----------------------------
INSERT INTO `shop_class` VALUES (1, 'esxKLIeJtiDtcrDpwzh/Bw');
INSERT INTO `shop_class` VALUES (2, 'e80aKduEuC7jcqb+wzh/Bw');
INSERT INTO `shop_class` VALUES (3, 'JcJBKoyE6yHqsrDl2IB/1Q');
INSERT INTO `shop_class` VALUES (4, 'JZtMII6PviH8ta5LzF1/Bw');
INSERT INTO `shop_class` VALUES (5, 'dctNLtrbuCbwK6pj2IB/1R4');
INSERT INTO `shop_class` VALUES (6, 'J8sbf4aJ6if1F8IS2IB/1R4');
INSERT INTO `shop_class` VALUES (7, 'IsoaIN2Jt3T9PasV2IB/1Q');
INSERT INTO `shop_class` VALUES (8, 'IJlIeNyPu3X/Hq5LyCc');
INSERT INTO `shop_class` VALUES (9, 'd8tBeIuE6XT0A672zF1/Bw');
INSERT INTO `shop_class` VALUES (10, 'd8gcKd3ctnX9G8HyzF1/Bw');
INSERT INTO `shop_class` VALUES (11, 'dstNLI2E6SDxb7jpoqlpGg');
INSERT INTO `shop_class` VALUES (12, 'JZhAetyFvnabjbEDzF1/Bw');
INSERT INTO `shop_class` VALUES (13, 'c8JIKouItiLrs8EQwSh7mQ');

-- ----------------------------
-- Table structure for shop_item
-- ----------------------------
DROP TABLE IF EXISTS `shop_item`;
CREATE TABLE `shop_item`  (
  `shop_vnum` int NOT NULL DEFAULT 0,
  `item_vnum` int NOT NULL DEFAULT 0,
  `count` smallint UNSIGNED NOT NULL DEFAULT 1,
  UNIQUE INDEX `vnum_unique`(`shop_vnum`, `item_vnum`, `count`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of shop_item
-- ----------------------------
INSERT INTO `shop_item` VALUES (1, 20, 1);
INSERT INTO `shop_item` VALUES (1, 40, 1);
INSERT INTO `shop_item` VALUES (1, 60, 1);
INSERT INTO `shop_item` VALUES (1, 80, 1);
INSERT INTO `shop_item` VALUES (1, 1040, 1);
INSERT INTO `shop_item` VALUES (1, 2010, 1);
INSERT INTO `shop_item` VALUES (1, 2030, 1);
INSERT INTO `shop_item` VALUES (1, 2050, 1);
INSERT INTO `shop_item` VALUES (1, 2070, 1);
INSERT INTO `shop_item` VALUES (1, 3010, 1);
INSERT INTO `shop_item` VALUES (1, 3030, 1);
INSERT INTO `shop_item` VALUES (1, 3050, 1);
INSERT INTO `shop_item` VALUES (1, 3070, 1);
INSERT INTO `shop_item` VALUES (1, 4000, 1);
INSERT INTO `shop_item` VALUES (1, 4010, 1);
INSERT INTO `shop_item` VALUES (1, 4020, 1);
INSERT INTO `shop_item` VALUES (1, 5010, 1);
INSERT INTO `shop_item` VALUES (1, 5030, 1);
INSERT INTO `shop_item` VALUES (1, 7010, 1);
INSERT INTO `shop_item` VALUES (1, 7030, 1);
INSERT INTO `shop_item` VALUES (1, 7050, 1);
INSERT INTO `shop_item` VALUES (1, 8000, 100);
INSERT INTO `shop_item` VALUES (1, 8000, 200);
INSERT INTO `shop_item` VALUES (2, 27400, 1);
INSERT INTO `shop_item` VALUES (2, 27600, 1);
INSERT INTO `shop_item` VALUES (2, 27610, 1);
INSERT INTO `shop_item` VALUES (2, 27800, 5);
INSERT INTO `shop_item` VALUES (2, 27800, 50);
INSERT INTO `shop_item` VALUES (2, 27801, 5);
INSERT INTO `shop_item` VALUES (2, 27801, 50);
INSERT INTO `shop_item` VALUES (3, 14000, 1);
INSERT INTO `shop_item` VALUES (3, 15000, 1);
INSERT INTO `shop_item` VALUES (3, 16000, 1);
INSERT INTO `shop_item` VALUES (3, 17000, 1);
INSERT INTO `shop_item` VALUES (3, 27001, 5);
INSERT INTO `shop_item` VALUES (3, 27001, 20);
INSERT INTO `shop_item` VALUES (3, 27001, 200);
INSERT INTO `shop_item` VALUES (3, 27002, 5);
INSERT INTO `shop_item` VALUES (3, 27002, 20);
INSERT INTO `shop_item` VALUES (3, 27002, 200);
INSERT INTO `shop_item` VALUES (3, 27003, 5);
INSERT INTO `shop_item` VALUES (3, 27003, 20);
INSERT INTO `shop_item` VALUES (3, 27003, 200);
INSERT INTO `shop_item` VALUES (3, 27004, 5);
INSERT INTO `shop_item` VALUES (3, 27004, 20);
INSERT INTO `shop_item` VALUES (3, 27004, 200);
INSERT INTO `shop_item` VALUES (3, 27005, 5);
INSERT INTO `shop_item` VALUES (3, 27005, 20);
INSERT INTO `shop_item` VALUES (3, 27005, 200);
INSERT INTO `shop_item` VALUES (3, 27006, 5);
INSERT INTO `shop_item` VALUES (3, 27006, 20);
INSERT INTO `shop_item` VALUES (3, 27006, 200);
INSERT INTO `shop_item` VALUES (3, 27007, 5);
INSERT INTO `shop_item` VALUES (3, 27007, 20);
INSERT INTO `shop_item` VALUES (3, 27007, 200);
INSERT INTO `shop_item` VALUES (3, 27008, 5);
INSERT INTO `shop_item` VALUES (3, 27008, 20);
INSERT INTO `shop_item` VALUES (3, 27008, 200);
INSERT INTO `shop_item` VALUES (3, 50200, 1);
INSERT INTO `shop_item` VALUES (3, 50263, 1);
INSERT INTO `shop_item` VALUES (3, 50264, 1);
INSERT INTO `shop_item` VALUES (3, 50901, 10);
INSERT INTO `shop_item` VALUES (3, 70063, 1);
INSERT INTO `shop_item` VALUES (3, 70064, 1);
INSERT INTO `shop_item` VALUES (3, 79501, 1);
INSERT INTO `shop_item` VALUES (3, 79502, 1);
INSERT INTO `shop_item` VALUES (4, 11200, 1);
INSERT INTO `shop_item` VALUES (4, 11220, 1);
INSERT INTO `shop_item` VALUES (4, 11230, 1);
INSERT INTO `shop_item` VALUES (4, 11400, 1);
INSERT INTO `shop_item` VALUES (4, 11420, 1);
INSERT INTO `shop_item` VALUES (4, 11430, 1);
INSERT INTO `shop_item` VALUES (4, 11600, 1);
INSERT INTO `shop_item` VALUES (4, 11620, 1);
INSERT INTO `shop_item` VALUES (4, 11630, 1);
INSERT INTO `shop_item` VALUES (4, 11800, 1);
INSERT INTO `shop_item` VALUES (4, 11820, 1);
INSERT INTO `shop_item` VALUES (4, 11830, 1);
INSERT INTO `shop_item` VALUES (4, 12200, 1);
INSERT INTO `shop_item` VALUES (4, 12340, 1);
INSERT INTO `shop_item` VALUES (4, 12480, 1);
INSERT INTO `shop_item` VALUES (4, 12620, 1);
INSERT INTO `shop_item` VALUES (4, 13000, 1);
INSERT INTO `shop_item` VALUES (5, 3100, 1);
INSERT INTO `shop_item` VALUES (5, 5020, 1);
INSERT INTO `shop_item` VALUES (5, 5030, 1);
INSERT INTO `shop_item` VALUES (5, 5040, 1);
INSERT INTO `shop_item` VALUES (5, 5050, 1);
INSERT INTO `shop_item` VALUES (5, 5070, 1);
INSERT INTO `shop_item` VALUES (5, 5080, 1);
INSERT INTO `shop_item` VALUES (5, 7020, 1);
INSERT INTO `shop_item` VALUES (5, 7050, 1);
INSERT INTO `shop_item` VALUES (5, 7060, 1);
INSERT INTO `shop_item` VALUES (5, 7090, 1);
INSERT INTO `shop_item` VALUES (5, 7100, 1);
INSERT INTO `shop_item` VALUES (6, 14000, 1);
INSERT INTO `shop_item` VALUES (6, 14020, 1);
INSERT INTO `shop_item` VALUES (6, 14040, 1);
INSERT INTO `shop_item` VALUES (6, 14060, 1);
INSERT INTO `shop_item` VALUES (6, 14080, 1);
INSERT INTO `shop_item` VALUES (6, 14140, 1);
INSERT INTO `shop_item` VALUES (6, 15000, 1);
INSERT INTO `shop_item` VALUES (6, 15020, 1);
INSERT INTO `shop_item` VALUES (6, 15040, 1);
INSERT INTO `shop_item` VALUES (6, 15060, 1);
INSERT INTO `shop_item` VALUES (6, 15080, 1);
INSERT INTO `shop_item` VALUES (6, 15160, 1);
INSERT INTO `shop_item` VALUES (6, 16000, 1);
INSERT INTO `shop_item` VALUES (6, 16020, 1);
INSERT INTO `shop_item` VALUES (6, 16040, 1);
INSERT INTO `shop_item` VALUES (6, 16060, 1);
INSERT INTO `shop_item` VALUES (6, 16080, 1);
INSERT INTO `shop_item` VALUES (6, 16180, 1);
INSERT INTO `shop_item` VALUES (6, 17000, 1);
INSERT INTO `shop_item` VALUES (6, 17020, 1);
INSERT INTO `shop_item` VALUES (6, 17040, 1);
INSERT INTO `shop_item` VALUES (6, 17060, 1);
INSERT INTO `shop_item` VALUES (6, 17080, 1);
INSERT INTO `shop_item` VALUES (6, 17180, 1);
INSERT INTO `shop_item` VALUES (7, 80003, 1);
INSERT INTO `shop_item` VALUES (7, 80004, 1);
INSERT INTO `shop_item` VALUES (7, 80005, 1);
INSERT INTO `shop_item` VALUES (7, 80006, 1);
INSERT INTO `shop_item` VALUES (7, 80007, 1);
INSERT INTO `shop_item` VALUES (8, 50100, 100);
INSERT INTO `shop_item` VALUES (8, 50101, 100);
INSERT INTO `shop_item` VALUES (8, 50102, 100);
INSERT INTO `shop_item` VALUES (8, 50103, 100);
INSERT INTO `shop_item` VALUES (8, 50104, 100);
INSERT INTO `shop_item` VALUES (8, 50105, 100);
INSERT INTO `shop_item` VALUES (9, 11901, 1);
INSERT INTO `shop_item` VALUES (9, 11902, 1);
INSERT INTO `shop_item` VALUES (9, 11903, 1);
INSERT INTO `shop_item` VALUES (9, 11904, 1);
INSERT INTO `shop_item` VALUES (9, 50201, 1);
INSERT INTO `shop_item` VALUES (9, 50202, 1);
INSERT INTO `shop_item` VALUES (10, 29101, 1);
INSERT INTO `shop_item` VALUES (11, 50054, 1);
INSERT INTO `shop_item` VALUES (11, 50055, 1);
INSERT INTO `shop_item` VALUES (11, 50056, 1);
INSERT INTO `shop_item` VALUES (12, 100100, 1);
INSERT INTO `shop_item` VALUES (12, 100200, 1);

-- ----------------------------
-- Table structure for skill_color
-- ----------------------------
DROP TABLE IF EXISTS `skill_color`;
CREATE TABLE `skill_color`  (
  `player_id` int(11) UNSIGNED ZEROFILL NOT NULL,
  `skillSlot1_Col1` int NOT NULL DEFAULT 0,
  `skillSlot1_Col2` int NOT NULL DEFAULT 0,
  `skillSlot1_Col3` int NOT NULL DEFAULT 0,
  `skillSlot1_Col4` int NOT NULL DEFAULT 0,
  `skillSlot1_Col5` int NOT NULL DEFAULT 0,
  `skillSlot2_Col1` int NOT NULL DEFAULT 0,
  `skillSlot2_Col2` int NOT NULL DEFAULT 0,
  `skillSlot2_Col3` int NOT NULL DEFAULT 0,
  `skillSlot2_Col4` int NOT NULL DEFAULT 0,
  `skillSlot2_Col5` int NOT NULL DEFAULT 0,
  `skillSlot3_Col1` int NOT NULL DEFAULT 0,
  `skillSlot3_Col2` int NOT NULL DEFAULT 0,
  `skillSlot3_Col3` int NOT NULL DEFAULT 0,
  `skillSlot3_Col4` int NOT NULL DEFAULT 0,
  `skillSlot3_Col5` int NOT NULL DEFAULT 0,
  `skillSlot4_Col1` int NOT NULL DEFAULT 0,
  `skillSlot4_Col2` int NOT NULL DEFAULT 0,
  `skillSlot4_Col3` int NOT NULL DEFAULT 0,
  `skillSlot4_Col4` int NOT NULL DEFAULT 0,
  `skillSlot4_Col5` int NOT NULL DEFAULT 0,
  `skillSlot5_Col1` int NOT NULL DEFAULT 0,
  `skillSlot5_Col2` int NOT NULL DEFAULT 0,
  `skillSlot5_Col3` int NOT NULL DEFAULT 0,
  `skillSlot5_Col4` int NOT NULL DEFAULT 0,
  `skillSlot5_Col5` int NOT NULL DEFAULT 0,
  `skillSlot6_Col1` int NOT NULL DEFAULT 0,
  `skillSlot6_Col2` int NOT NULL DEFAULT 0,
  `skillSlot6_Col3` int NOT NULL DEFAULT 0,
  `skillSlot6_Col4` int NOT NULL DEFAULT 0,
  `skillSlot6_Col5` int NOT NULL DEFAULT 0,
  `skillSlot7_Col1` int NOT NULL DEFAULT 0,
  `skillSlot7_Col2` int NOT NULL DEFAULT 0,
  `skillSlot7_Col3` int NOT NULL DEFAULT 0,
  `skillSlot7_Col4` int NOT NULL DEFAULT 0,
  `skillSlot7_Col5` int NOT NULL DEFAULT 0,
  `skillSlot8_Col1` int NOT NULL DEFAULT 0,
  `skillSlot8_Col2` int NOT NULL DEFAULT 0,
  `skillSlot8_Col3` int NOT NULL DEFAULT 0,
  `skillSlot8_Col4` int NOT NULL DEFAULT 0,
  `skillSlot8_Col5` int NOT NULL DEFAULT 0,
  `skillSlot9_Col1` int NOT NULL DEFAULT 0,
  `skillSlot9_Col2` int NOT NULL DEFAULT 0,
  `skillSlot9_Col3` int NOT NULL DEFAULT 0,
  `skillSlot9_Col4` int NOT NULL DEFAULT 0,
  `skillSlot9_Col5` int NOT NULL DEFAULT 0,
  `skillSlot10_Col1` int NOT NULL DEFAULT 0,
  `skillSlot10_Col2` int NOT NULL DEFAULT 0,
  `skillSlot10_Col3` int NOT NULL DEFAULT 0,
  `skillSlot10_Col4` int NOT NULL DEFAULT 0,
  `skillSlot10_Col5` int NOT NULL DEFAULT 0,
  `buffSkill1_Col1` int NOT NULL DEFAULT 0,
  `buffSkill1_Col2` int NOT NULL DEFAULT 0,
  `buffSkill1_Col3` int NOT NULL DEFAULT 0,
  `buffSkill1_Col4` int NOT NULL DEFAULT 0,
  `buffSkill1_Col5` int NOT NULL DEFAULT 0,
  `buffSkill2_Col1` int NOT NULL DEFAULT 0,
  `buffSkill2_Col2` int NOT NULL DEFAULT 0,
  `buffSkill2_Col3` int NOT NULL DEFAULT 0,
  `buffSkill2_Col4` int NOT NULL DEFAULT 0,
  `buffSkill2_Col5` int NOT NULL DEFAULT 0,
  `buffSkill3_Col1` int NOT NULL DEFAULT 0,
  `buffSkill3_Col2` int NOT NULL DEFAULT 0,
  `buffSkill3_Col3` int NOT NULL DEFAULT 0,
  `buffSkill3_Col4` int NOT NULL DEFAULT 0,
  `buffSkill3_Col5` int NOT NULL DEFAULT 0,
  `buffSkill4_Col1` int NOT NULL DEFAULT 0,
  `buffSkill4_Col2` int NOT NULL DEFAULT 0,
  `buffSkill4_Col3` int NOT NULL DEFAULT 0,
  `buffSkill4_Col4` int NOT NULL DEFAULT 0,
  `buffSkill4_Col5` int NOT NULL DEFAULT 0,
  `buffSkill5_Col1` int NOT NULL DEFAULT 0,
  `buffSkill5_Col2` int NOT NULL DEFAULT 0,
  `buffSkill5_Col3` int NOT NULL DEFAULT 0,
  `buffSkill5_Col4` int NOT NULL DEFAULT 0,
  `buffSkill5_Col5` int NOT NULL DEFAULT 0,
  `buffSkill6_Col1` int NOT NULL DEFAULT 0,
  `buffSkill6_Col2` int NOT NULL DEFAULT 0,
  `buffSkill6_Col3` int NOT NULL DEFAULT 0,
  `buffSkill6_Col4` int NOT NULL DEFAULT 0,
  `buffSkill6_Col5` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`player_id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of skill_color
-- ----------------------------

-- ----------------------------
-- Table structure for skill_proto
-- ----------------------------
DROP TABLE IF EXISTS `skill_proto`;
CREATE TABLE `skill_proto`  (
  `dwVnum` int NOT NULL DEFAULT 0,
  `szName` varbinary(32) NOT NULL DEFAULT '',
  `bType` tinyint NOT NULL DEFAULT 0,
  `bLevelStep` tinyint NOT NULL DEFAULT 0,
  `bMaxLevel` tinyint NOT NULL DEFAULT 0,
  `bLevelLimit` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `szPointOn` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '0',
  `szPointPoly` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szSPCostPoly` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szDurationPoly` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szDurationSPCostPoly` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szCooldownPoly` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szMasterBonusPoly` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szAttackGradePoly` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `setFlag` set('ATTACK','USE_MELEE_DAMAGE','COMPUTE_ATTGRADE','SELFONLY','USE_MAGIC_DAMAGE','USE_HP_AS_COST','COMPUTE_MAGIC_DAMAGE','SPLASH','GIVE_PENALTY','USE_ARROW_DAMAGE','PENETRATE','IGNORE_TARGET_RATING','ATTACK_SLOW','ATTACK_STUN','HP_ABSORB','SP_ABSORB','ATTACK_FIRE_CONT','REMOVE_BAD_AFFECT','REMOVE_GOOD_AFFECT','CRUSH','ATTACK_POISON','TOGGLE','DISABLE_BY_POINT_UP','CRUSH_LONG','WIND','ELEC','FIRE','ATTACK_BLEEDING','PARTY','KNOCKBACK') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `setAffectFlag` enum('YMIR','INVISIBILITY','SPAWN','POISON','SLOW','STUN','DUNGEON_READY','DUNGEON_UNIQUE','BUILDING_CONSTRUCTION_SMALL','BUILDING_CONSTRUCTION_LARGE','BUILDING_UPGRADE','MOV_SPEED_POTION','ATT_SPEED_POTION','FISH_MIND','JEONGWIHON','GEOMGYEONG','CHEONGEUN','GYEONGGONG','EUNHYUNG','GWIGUM','TERROR','JUMAGAP','HOSIN','BOHO','KWAESOK','MANASHIELD','MUYEONG','REVIVE_INVISIBLE','FIRE','GICHEON','JEUNGRYEOK','TANHWAN_DASH','PABEOP','CHEONGEUN_WITH_FALL','POLYMORPH','WAR_FLAG1','WAR_FLAG2','WAR_FLAG3','CHINA_FIREWORK','HAIR','GERMANY','DS','BLEEDING','RED_POSSESSION','BLUE_POSSESSION','STATUE1','STATUE2','STATUE3','STATUE4','SOUL_RED','SOUL_BLUE','SOUL_MIX','CHEONUN') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `szPointOn2` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NONE',
  `szPointPoly2` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szDurationPoly2` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `setAffectFlag2` enum('YMIR','INVISIBILITY','SPAWN','POISON','SLOW','STUN','DUNGEON_READY','DUNGEON_UNIQUE','BUILDING_CONSTRUCTION_SMALL','BUILDING_CONSTRUCTION_LARGE','BUILDING_UPGRADE','MOV_SPEED_POTION','ATT_SPEED_POTION','FISH_MIND','JEONGWIHON','GEOMGYEONG','CHEONGEUN','GYEONGGONG','EUNHYUNG','GWIGUM','TERROR','JUMAGAP','HOSIN','BOHO','KWAESOK','MANASHIELD','MUYEONG','REVIVE_INVISIBLE','FIRE','GICHEON','JEUNGRYEOK','TANHWAN_DASH','PABEOP','CHEONGEUN_WITH_FALL','POLYMORPH','WAR_FLAG1','WAR_FLAG2','WAR_FLAG3','CHINA_FIREWORK','HAIR','GERMANY','DS','BLEEDING','RED_POSSESSION','BLUE_POSSESSION','STATUE1','STATUE2','STATUE3','STATUE4','SOUL_RED','SOUL_BLUE','SOUL_MIX','CHEONUN') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `szPointOn3` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NONE',
  `szPointPoly3` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szDurationPoly3` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `szGrandMasterAddSPCostPoly` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `prerequisiteSkillVnum` int NOT NULL DEFAULT 0,
  `prerequisiteSkillLevel` int NOT NULL DEFAULT 0,
  `eSkillType` enum('NORMAL','MELEE','RANGE','MAGIC') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NORMAL',
  `iMaxHit` tinyint NOT NULL DEFAULT 0,
  `szSplashAroundDamageAdjustPoly` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '1',
  `dwTargetRange` int NOT NULL DEFAULT 1000,
  `dwSplashRange` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`dwVnum`) USING BTREE,
  UNIQUE INDEX `dwVnum unique`(`dwVnum`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of skill_proto
-- ----------------------------
INSERT INTO `skill_proto` VALUES (1, 0xBBEFBFACC2FC, 1, 1, 1, 0, 'HP', '-(1.1*atk+(0.5*atk+1.5*str)*k)', '40+100*k', '', '', '12', '-(1.1*atk+(0.5*atk+1.5*str)*k)', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '40+100*k', 0, 0, 'MELEE', 5, '1', 0, 200);
INSERT INTO `skill_proto` VALUES (2, 0xC6C8B9E6C7B3BFEC, 1, 1, 1, 0, 'HP', '-(3*atk+(0.8*atk+str*5+dex*3+con)*k)', '50+130*k', '', '', '15', '-(3*atk+(0.8*atk+str*5+dex*3+con)*k)', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '50+130*k', 0, 0, 'MELEE', 12, '1', 0, 200);
INSERT INTO `skill_proto` VALUES (3, 0xC0FCB1CDC8A5, 1, 1, 1, 0, 'ATT_SPEED', '50*k', '50+140*k', '60+90*k', '', '63+90*k', '50*k', '', 'SELFONLY', 'JEONGWIHON', 'MOV_SPEED', '20*k', '60+90*k', NULL, '', '', '', '50+140*k', 0, 0, 'NORMAL', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (4, 0xB0CBB0E6, 1, 1, 1, 0, 'ATT_GRADE', '(100+str+lv*3)*k', '100+200*k', '30+50*k', '', '33+50*k', '(100+str+lv*3)*k', '', 'SELFONLY', 'GEOMGYEONG', 'NONE', '', '', NULL, '', '', '', '100+200*k', 0, 0, 'NORMAL', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (16, 0xB1E2B0F8C2FC, 1, 1, 1, 0, 'HP', '-(2.3*atk+(4*atk+str*4+con)*k)', '60+120*k', '', '', '15', '-(2.3*atk+(4*atk+str*4+con)*k)', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '60+120*k', 0, 0, 'MELEE', 4, '1', 0, 100);
INSERT INTO `skill_proto` VALUES (17, 0xB0DDBBEAC5B8BFEC, 1, 1, 1, 0, 'HP', '-(2.3*atk+(3*atk+str*4+con*3)*k)', '60+150*k', '', '', '15', '-(2.3*atk+(3*atk+str*4+con*3)*k)', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '60+150*k', 0, 0, 'MELEE', 8, '1', 0, 200);
INSERT INTO `skill_proto` VALUES (18, 0xB4EBC1F8B0A2, 1, 1, 1, 0, 'HP', '-(2*atk+(2*atk+2*dex+2*con+str*4)*k)', '50+140*k', '', '', '25', '-(2*atk+(2*atk+2*dex+2*con+str*4)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,SELFONLY,SPLASH,ATTACK_STUN', NULL, 'NONE', '100+k*1000/6', '2', NULL, '', '', '', '50+140*k', 0, 0, 'MELEE', 10, '1', 0, 400);
INSERT INTO `skill_proto` VALUES (19, 0xC3B5B1D9C3DF, 1, 1, 1, 0, 'DEF_GRADE', '(200+str*0.2+con*0.5)*k', '80+220*k', '60+90*k', '', '63+90*k', '(200+str*0.2+con*0.5)*k', '', 'SELFONLY', 'CHEONGEUN', 'MOV_SPEED', '-(1+9*k)', '60+90*k', NULL, '', '', '', '80+220*k', 0, 0, 'NORMAL', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (47, 0xB0FCB0DDBCFA, 2, 1, 1, 0, 'HP', '-(atk+(1.7*atk+dex*5+str)*k)', '30+150*k', '', '', '15', '-(atk+(1.7*atk+dex*5+str)*k)', '', 'ATTACK,SPLASH,USE_ARROW_DAMAGE,ATTACK_SLOW', NULL, 'NONE', '50+dex*6*k', '1+iq*0.4*k*0.1', NULL, '', '', '', '30+150*k', 0, 0, 'RANGE', 8, '0.6', 2500, 200);
INSERT INTO `skill_proto` VALUES (48, 0xC8ADC1B6C6C4, 2, 1, 1, 0, 'HP', '-(1.5*atk+(2.6*atk+0.9*iq+number(100,300))*k)', '30+160*k', '', '', '15', '-(1.5*atk+(2.6*atk+0.9*iq+number(100,300))*k)', '', 'ATTACK,SPLASH,USE_ARROW_DAMAGE,ATTACK_FIRE_CONT', NULL, 'NONE', 'lv+5*iq*k', '(350+iq*4*k)/10', NULL, '', '', '', '30+160*k', 0, 0, 'RANGE', 12, '1', 2500, 300);
INSERT INTO `skill_proto` VALUES (46, 0xBFACBBE7, 2, 1, 1, 0, 'HP', '-(atk+0.2*atk*floor(2+k*6)+(0.8*atk+dex*8*ar+iq*2)*k)', '40+160*k', '', '', '10', '-(atk+0.2*atk*floor(2+k*6)+(0.8*atk+dex*8*ar+iq*2)*k)', '', 'ATTACK,USE_ARROW_DAMAGE,KNOCKBACK', NULL, 'NONE', '', '', NULL, '', '', '', '40+160*k', 0, 0, 'RANGE', 1, '1', 2500, 0);
INSERT INTO `skill_proto` VALUES (49, 0xB0E6B0F8BCFA, 2, 1, 1, 0, 'MOV_SPEED', '60*k', '60+220*k', '3+(90*k)/10', '', '4+8*k', '60*k', '', 'SELFONLY', 'GYEONGGONG', 'NONE', '(Lv*2+(3*dex+number(100,300))+str*2+iq*2)*k)', '', NULL, '', '', '', '60+220*k', 0, 0, 'NORMAL', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (31, 0xBECFBDC0, 2, 1, 1, 0, 'HP', '-(atk+(1.2*atk+number(500,700)+dex*4+str*4)*k)', '40+160*k', '', '', '15', '-(atk+(1.2*atk+number(500,700)+dex*4+str*4)*k)', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '40+160*k', 0, 0, 'MELEE', 6, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (32, 0xB1C3BDC5C5BABFB5, 2, 1, 1, 0, 'HP', '-(atk+(1.6*atk+number(200,300)+dex*7+str*7)*k)', '40+160*k', '', '', '20', '-(atk+(1.6*atk+number(200,300)+dex*7+str*7)*k)', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '40+160*k', 0, 0, 'MELEE', 6, '1', 800, 0);
INSERT INTO `skill_proto` VALUES (33, 0xC2F7B7FBBBEC, 2, 1, 1, 0, 'HP', '-(2*atk+(0.5*atk+dex*9+str*7)*k)', '50+140*k', '', '', '25', '-(2*atk+(0.5*atk+dex*9+str*7)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,ATTACK_POISON', NULL, 'NONE', '40*k', '', NULL, '', '', '', '50+140*k', 0, 0, 'MELEE', 12, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (34, 0xC0BAC7FCB9FD, 2, 1, 1, 0, 'NONE', '', '30+60*k', '15+30*k', '', '60', '', '', 'SELFONLY', 'EUNHYUNG', 'NONE', '', '', NULL, '', '', '', '30+60*k', 0, 0, 'NORMAL', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (76, 0xB8B6B7C9, 3, 1, 1, 0, 'HP', '-(40+5*lv+2*iq+(13*iq+6*mwep+number(50,1))*ar*k)', '30+140*k', '', '', '7', '-(40+5*lv+2*iq+(13*iq+6*mwep+number(50,1))*ar*k)', '', 'ATTACK,COMPUTE_MAGIC_DAMAGE,SPLASH', NULL, 'NONE', '', '', NULL, '', '', '', '30+140*k', 0, 0, 'MAGIC', 5, '1', 1500, 200);
INSERT INTO `skill_proto` VALUES (77, 0xC8ADBFB0C6F8, 3, 1, 1, 0, 'HP', '-(5*lv+2*iq+(10*iq+6*mwep+str*4+con*2+number(180,2))*k)', '60+140*k', '', '', '12', '-(5*lv+2*iq+(10*iq+6*mwep+str*4+con*2+number(180,2))*k)', '', 'ATTACK,SELFONLY,COMPUTE_MAGIC_DAMAGE,SPLASH', NULL, 'NONE', '', '', NULL, '', '', '', '60+140*k', 0, 0, 'MAGIC', 15, '0.8', 0, 500);
INSERT INTO `skill_proto` VALUES (78, 0xB9ABBFB5C1F8, 3, 1, 1, 0, 'HP', '-(30+2*lv+2*iq+(7*iq+6*mwep+number(200,5))*ar*k)', '20+30*k', '40+30*k', '5+40*k', '43+30*k', '-(30+2*lv+2*iq+(7*iq+6*mwep+number(200,5))*ar*k)', '', 'ATTACK,COMPUTE_MAGIC_DAMAGE,SPLASH,TOGGLE', NULL, 'NONE', '', '', NULL, '', '', '', '20+30*k', 0, 0, 'MAGIC', 1, '1', 800, 100);
INSERT INTO `skill_proto` VALUES (80, 0xC5F5BCD3B8B6B7C9, 3, 1, 1, 0, 'HP', '-(40+2*lv+2*iq+(2*con+2*dex+13*iq+6*mwep+number(180,2))*ar*k)', '40+120*k', '', '', '12', '-(40+2*lv+2*iq+(2*con+2*dex+13*iq+6*mwep+number(180,2))*ar*k)', '', 'ATTACK,COMPUTE_MAGIC_DAMAGE,SPLASH,ATTACK_SLOW', NULL, 'NONE', '333+300*k', '10+10*k', NULL, '', '', '', '40+120*k', 0, 0, 'MAGIC', 9, '1', 1200, 400);
INSERT INTO `skill_proto` VALUES (61, 0xBCE2B7C9C1F6, 3, 1, 1, 0, 'HP', '-(atk+2*lv+iq*2+(2*atk+str*4+iq*14)*k)', '30+140*k', '', '', '10', '-(atk+2*lv+iq*2+(2*atk+str*4+iq*14)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,PENETRATE', NULL, 'NONE', '1+k*9', '', NULL, '', '', '', '30+140*k', 0, 0, 'MELEE', 4, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (62, 0xBFEBB1C7C6C4, 3, 1, 1, 0, 'HP', '-(1.1*atk+2*lv+iq*2+(1.5*atk+str+iq*12)*k)', '50+150*k', '', '', '15', '-(1.1*atk+2*lv+iq*2+(1.5*atk+str+iq*12)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,SELFONLY,SPLASH,IGNORE_TARGET_RATING', NULL, 'NONE', '1+k*9', '', NULL, '', '', '', '50+150*k', 0, 0, 'MELEE', 12, '1', 0, 500);
INSERT INTO `skill_proto` VALUES (63, 0xB1CDB0CB, 3, 1, 1, 0, 'ATT_GRADE', '(3*iq+2*lv)*k', '20+240*k', '50+100*k', '2+23*k', '0', '(3*iq+2*lv)*k', '', 'SELFONLY,TOGGLE', 'GWIGUM', 'HIT_HP_RECOVER', '10*k', '50+100*k', NULL, '', '', '', '20+240*k', 0, 0, 'NORMAL', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (64, 0xB0F8C6F7, 3, 1, 1, 0, 'DODGE', '1+29*k', '60+120*k', '60+100*k', '', '100', '1+29*k', '', 'SELFONLY', 'TERROR', 'NONE', '', '', NULL, '', '', '', '60+120*k', 0, 0, 'NORMAL', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (65, 0xC1D6B8B6B0A9, 3, 1, 1, 0, 'DEF_GRADE', '(iq+30)*k', '70+170*k', '30+120*k', '', '33+140*k', '(iq+30)*k', '', 'SELFONLY', 'JUMAGAP', 'REFLECT_MELEE', '(iq/4+10)*k', '30+120*k', NULL, '', '', '', '70+170*k', 0, 0, 'NORMAL', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (92, 0xBFEBC6C4BBEA, 4, 1, 1, 0, 'HP', '-(60+5*lv+(16*iq+dex*6+6*mwep+120)*ar*k)', '50+280*k', '', '', '10', '-(60+5*lv+(16*iq+dex*6+6*mwep+120)*ar*k)', '', 'ATTACK,ATTACK_FIRE_CONT,KNOCKBACK', NULL, 'NONE', 'lv+5*iq*k', '(100+iq*5*k)/10', NULL, '', '', '', '50+280*k', 0, 0, 'MAGIC', 10, '1', 800, 0);
INSERT INTO `skill_proto` VALUES (93, 0xC6D0B7E6C6F8, 4, 1, 1, 0, 'HP', '-(70+3*lv+(20*iq+str*3+10*mwep+100)*ar*k)', '70+380*k', '', '', '12', '-(70+3*lv+(20*iq+str*3+10*mwep+100)*ar*k)', '', 'ATTACK,SPLASH,ATTACK_FIRE_CONT', NULL, 'NONE', 'lv+5*iq*k', '(100+iq*5*k)/10', NULL, '', '', '', '70+380*k', 0, 0, 'MAGIC', 15, '1', 2500, 500);
INSERT INTO `skill_proto` VALUES (94, 0xC8A3BDC5, 4, 1, 1, 0, 'RESIST_NORMAL', '((iq*0.3+5)*(2*k+0.5)+(dex*0.3))/(k+2.3)', '40+190*k', '50+80*k', '', '10', '((iq*0.3+5)*(2*k+0.5)+(dex*0.3))/(k+2.3)', '', 'PARTY', 'HOSIN', 'NONE', '', '', NULL, '', '', '', '40+190*k', 0, 0, 'NORMAL', 1, '1', 2000, 0);
INSERT INTO `skill_proto` VALUES (95, 0xB9DDBBE7, 4, 1, 1, 0, 'REFLECT_MELEE', '5+(iq*0.2+str*0.1)*k', '60+120*(k+k)', '60+200*k', '', '10', '5+(iq*0.2+str*0.1)*k', '', 'PARTY', 'BOHO', 'NONE', '', '', NULL, '', '', '', '60+120*(k+k)', 0, 0, 'NORMAL', 1, '1', 2000, 0);
INSERT INTO `skill_proto` VALUES (106, 0xB3FAC0FCB7C9, 4, 1, 1, 0, 'HP', '-(60+5*lv+(8*iq+dex*2+8*mwep+number(iq*5,iq*15))*ar*k)', '30+200*k', '', '', '8', '-(60+5*lv+(8*iq+dex*2+8*mwep+number(iq*5,iq*15))*ar*k)', '', 'ATTACK,SPLASH', NULL, 'NONE', '', '', NULL, '', '', '', '30+200*k', 0, 0, 'MAGIC', 5, '1', 1800, 400);
INSERT INTO `skill_proto` VALUES (107, 0xBAADB6F4, 4, 1, 1, 0, 'HP', '-(40+4*lv+(13*iq+str*2+10*mwep+number(iq*5,iq*16))*ar*k)', '50+300*k', '', '', '10', '-(40+4*lv+(13*iq+str*2+10*mwep+number(iq*5,iq*16))*ar*k)', '', 'ATTACK,SPLASH,ATTACK_STUN', NULL, 'NONE', '30+70*2*k', '1+str*0.2*k*0.1', NULL, '', '', '', '50+300*k', 0, 0, 'MAGIC', 15, '1', 1500, 400);
INSERT INTO `skill_proto` VALUES (108, 0xC6F8B7DAB0DD, 4, 1, 1, 0, 'HP', '-(50+5*lv+(8*iq+str*2+8*mwep+number(1,800))*ar*k)*(1-chain*0.13)', '40+200*k', '', '', '9', '-(50+5*lv+(8*iq+str*2+8*mwep+number(1,800))*ar*k)*(1-chain*0.13)', '', 'ATTACK,SPLASH,ATTACK_SLOW', NULL, 'NONE', '80+iq*5*k', '1+str*0.4*k*0.1', NULL, '', '', '', '40+200*k', 0, 0, 'MAGIC', 7, '1', 2500, 100);
INSERT INTO `skill_proto` VALUES (109, 0xC1A4BEF7C0CE, 4, 1, 1, 0, 'HP', '150+5*lv+(10*iq+6*mwep+number(600,800))*(k+0.15)', '40+250*k', '', '', '10', '150+5*lv+(10*iq+6*mwep+number(600,800))*(k+0.15)', '', 'REMOVE_BAD_AFFECT,PARTY', NULL, 'NONE', '20+80*k', '0', NULL, '', '', '', '40+250*k', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (110, 0xC4E8BCD3, 4, 1, 1, 0, 'MOV_SPEED', '(dex*0.2)+35*k', '60+120*k', '60+100*k', '', '10', '(dex*0.2)+35*k', '', 'PARTY', 'KWAESOK', 'CASTING_SPEED', '3+33*k', '60+100*k', NULL, '', '', '', '60+120*k', 0, 0, 'NORMAL', 1, '1', 2000, 0);
INSERT INTO `skill_proto` VALUES (79, 0xC8E6BDC5BCF6C8A3, 3, 1, 1, 0, 'DEF_GRADE', '(0.5*iq+15)*k', '20+30*k', '60+120*k', '5+10*k', '63+120*k', '(0.5*iq+15)*k', '', 'SELFONLY,TOGGLE', 'MANASHIELD', 'MANASHIELD', '100-((iq*0.84)*k)', '60+120*k', NULL, '', '', '', '20+30*k', 0, 0, 'MAGIC', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (157, 0xC1D6B9AEBCFA, 0, 1, 7, 0, 'CASTING_SPEED', '50*k', '150+150*k', '180', '', '480', '50*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '150+150*k', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (156, 0xBFEBBDC5C0C7BAD0B3EB, 0, 1, 7, 0, 'CRITICAL', '50*k', '150+150*k', '180', '', '480', '50*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '150+150*k', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (155, 0xB0A1BCD3C8AD, 0, 1, 7, 0, 'MOV_SPEED', '15*k', '150+150*k', '180', '', '480', '15*k', '', NULL, NULL, 'ATT_SPEED', '15*k', '180', NULL, '', '', '', '150+150*k', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (154, 0xBCBAC8D6B0A9, 0, 1, 7, 0, 'DEF_GRADE', 'odef*0.1*k', '150+150*k', '180', '', '480', 'odef*0.1*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '150+150*k', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (153, 0xBFEBBDC5C0C7C3E0BAB9, 0, 1, 7, 0, 'MAX_SP', 'maxsp*0.2*k', '150+150*k', '300', '', '600', 'maxsp*0.2*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '150+150*k', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (151, 0xBFEBBEC8, 0, 1, 7, 0, 'NONE', '', '', '', '', '', '', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (91, 0xBAF1C6C4BACE, 4, 1, 1, 0, 'HP', '-(70+5*lv+(18*iq+str*7+5*mwep+50)*ar*k)', '30+200*k', '', '', '9', '-(70+5*lv+(18*iq+str*7+5*mwep+50)*ar*k)', '', 'ATTACK,COMPUTE_MAGIC_DAMAGE,SPLASH,ATTACK_SLOW', NULL, 'NONE', '3+iq*7*k', '1+dex*0.4*k*0.1', NULL, '', '', '', '30+200*k', 0, 0, 'MAGIC', 5, '1', 1800, 400);
INSERT INTO `skill_proto` VALUES (5, 0xC5BAC8AFB0DD, 1, 1, 1, 0, 'HP', '-(2*atk+(atk+dex*3+str*7+con)*k)', '60+120*k', '', '', '12', '-(2*atk+(atk+dex*3+str*7+con)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,CRUSH', NULL, 'MOV_SPEED', '150', '3', NULL, '', '', '', '60+120*k', 0, 0, 'MELEE', 4, '1', 0, 200);
INSERT INTO `skill_proto` VALUES (20, 0xB0CBC7B3, 1, 1, 1, 0, 'HP', '-(2*atk+(atk+dex*3+str*5+con)*k)', '40+120*k', '', '', '20', '-(2*atk+(atk+dex*3+str*5+con)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,CRUSH', NULL, 'NONE', '', '', NULL, '', '', '', '40+120*k', 0, 0, 'MELEE', 10, '1', 1200, 200);
INSERT INTO `skill_proto` VALUES (35, 0xBBEAB0F8BAD0, 2, 1, 1, 0, 'HP', '-(lv*2+(atk+str*3+dex*18)*k)', '40+130*k', '', '', '25', '-(lv*2+(atk+str*3+dex*18)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_POISON', NULL, 'NONE', '60*k', '5+25*k', NULL, '', '', '', '40+130*k', 0, 0, 'MELEE', 0, '1', 800, 200);
INSERT INTO `skill_proto` VALUES (50, 0xB5B6B1E2B1C3, 2, 1, 1, 0, 'HP', '-(atk+(1.4*atk+number(100,200)+dex*7+str*4+iq*4)*k)', '40+160*k', '', '', '18', '-(atk+(1.4*atk+number(100,200)+dex*7+str*4+iq*4)*k)', '', 'ATTACK,SPLASH,USE_ARROW_DAMAGE,CRUSH,ATTACK_POISON', NULL, 'NONE', '(50+dex*6*k)/10', '15+30*k', NULL, '', '', '', '40+160*k', 0, 0, 'RANGE', 12, '1', 2500, 600);
INSERT INTO `skill_proto` VALUES (66, 0xC6C4B9FDBCFA, 3, 1, 1, 0, 'HP', '-(40+5*lv+2*iq+(10*iq+7*mwep+number(50,1))*ar*k)', '30+120*k', '', '', '12', '-(40+5*lv+2*iq+(10*iq+7*mwep+number(50,1))*ar*k)', '', 'ATTACK,COMPUTE_MAGIC_DAMAGE,SPLASH,REMOVE_GOOD_AFFECT', NULL, 'NONE', '10+40*k', '7+23*k', NULL, '', '', '', '30+120*k', 0, 0, 'NORMAL', 5, '1', 1800, 200);
INSERT INTO `skill_proto` VALUES (81, 0xB8B6C8AFB0DD, 3, 1, 1, 0, 'HP', '-(120+6*lv+(5*con+5*dex+29*iq+9*mwep)*ar*k)', '80+220*k', '', '', '24', '-(120+6*lv+(5*con+5*dex+29*iq+9*mwep)*ar*k)', '', 'ATTACK,COMPUTE_MAGIC_DAMAGE,SPLASH', NULL, 'NONE', '', '', NULL, '', '', '', '80+220*k', 0, 0, 'MAGIC', 9, '1', 1500, 200);
INSERT INTO `skill_proto` VALUES (96, 0xB1E2C3B5B4EBB0F8, 4, 1, 1, 0, 'CRITICAL', '(iq*0.3+5)*(2*k)/(k+3)', '50+180*k', '60+100*k', '', '10', '(iq*0.3+5)*(2*k)/(k+3)', '', 'PARTY', 'GICHEON', 'NONE', '', '', NULL, '', '', '', '50+180*k', 0, 0, 'NORMAL', 1, '1', 2000, 0);
INSERT INTO `skill_proto` VALUES (111, 0xC1F5B7C2BCFA, 4, 1, 1, 0, 'ATT_GRADE', '3+(iq*0.2+str*0.1+11)*k', '60+150*k', '60+100*k', '', '10', '3+(iq*0.2+str*0.1+11)*k', '', 'PARTY', 'JEUNGRYEOK', 'NONE', '', '', NULL, '', '', '', '60+150*k', 0, 0, 'NORMAL', 1, '1', 2000, 0);
INSERT INTO `skill_proto` VALUES (126, 0xBDC5BCF6BEEE, 0, 1, 20, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (122, 0xBFACB0E8B1E2, 0, 1, 2, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (121, 0xC5EBBCD6B7C2, 0, 1, 40, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (125, 0xBAAFBDC5, 0, 1, 40, 0, 'NONE', '', '', '10+1000*k', '', '', '', '', NULL, 'YMIR', 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (127, 0xC3B5C1B6BEEE, 0, 1, 20, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (128, 0xC1F8B3EBBEEE, 0, 1, 20, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (158, 0xB1E6B5E5C0CCB5BF, 0, 1, 3, 0, 'NONE', '', '', '', '', '', '', '', 'SELFONLY', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (159, 0xB0F8B0A3C0C7B9AE, 0, 1, 5, 0, 'NONE', '', '', '', '', '', '', '', 'SELFONLY', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (160, 0xB0A1BCD3C8AD, 0, 1, 5, 0, 'NONE', '3*k', '80-12*k', '300', '', '', '3*k', '', 'SELFONLY', NULL, 'NONE', '', '', NULL, '', '', '', '80-12*k', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (161, 0xB0F8B0A3C0C7B9AE, 0, 1, 2, 0, 'NONE', '', '50', '', '', '', '', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '50', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (162, 0xB0F8B0A3C0CCB5BF, 0, 1, 2, 0, 'NONE', '', '20', '', '', '', '', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '20', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (256, 0x4352555348323030BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '2', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,CRUSH', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 300);
INSERT INTO `skill_proto` VALUES (257, 0xC0CFB9DDB9FCC0A7333530BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '5', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 525);
INSERT INTO `skill_proto` VALUES (258, 0x4352555348333030BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '7', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,CRUSH', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 450);
INSERT INTO `skill_proto` VALUES (259, 0xC0CFB9DDB9FCC0A7323030BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '9', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 300);
INSERT INTO `skill_proto` VALUES (260, 0x4355525348343030BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '10', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,CRUSH', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 600);
INSERT INTO `skill_proto` VALUES (261, 0xB5B6323530BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '9', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_POISON', NULL, 'NONE', '80', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 375);
INSERT INTO `skill_proto` VALUES (262, 0x534C4F57333030BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '12', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH', NULL, 'MOV_SPEED', '-20', '10', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 450);
INSERT INTO `skill_proto` VALUES (130, 0xBDC2B8B6, 0, 1, 1, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (137, 0xB3ADB9AB, 5, 1, 1, 50, 'HP', '-(atk+(2*atk*k))', '60+80*k', '', '', '5-(4*k)', '-(atk+(2*atk*k))', '', 'ATTACK,USE_MELEE_DAMAGE,CRUSH', NULL, 'NONE', '', '', NULL, '', '', '', '60+80*k', 0, 0, 'MELEE', 10, '1', 300, 0);
INSERT INTO `skill_proto` VALUES (138, 0xB5B9B0DD, 5, 1, 1, 52, 'HP', '-(2.4*(200+1.5*lv)+(3*200*k))', '60+120*k', '', '', '15', '-(2.4*(200+1.5*lv)+(3*200*k))', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,CRUSH_LONG', NULL, 'MOV_SPEED', '50', '5', NULL, '', '', '', '60+120*k', 0, 0, 'MELEE', 6, '1', 400, 100);
INSERT INTO `skill_proto` VALUES (139, 0xC5BBC3E2, 5, 1, 1, 55, 'HP', '-(2*(200+1.5*lv)+(3*200*k))', '60+160*k', '', '', '20', '-(2*(200+1.5*lv)+(3*200*k))', '', 'ATTACK,USE_MELEE_DAMAGE,SELFONLY,SPLASH,CRUSH', NULL, 'NONE', '', '', NULL, '', '', '', '60+160*k', 0, 0, 'MELEE', 12, '1', 400, 250);
INSERT INTO `skill_proto` VALUES (140, 0xB3ADB9AB28C8B029, 5, 1, 1, 50, 'HP', '-(atk+(2*atk*k))', '60+80*k', '', '', '10', '-(atk+(2*atk*k))', '', 'ATTACK,USE_ARROW_DAMAGE,CRUSH', NULL, 'NONE', '', '', NULL, '', '', '', '60+80*k', 0, 0, 'RANGE', 5, '1', 2500, 0);
INSERT INTO `skill_proto` VALUES (131, 0xB8BBBCD2C8AF, 0, 1, 10, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (124, 0xC3A4B1A4BCFA, 0, 1, 40, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (152, 0xBFEBBDC5C0C7C7C7, 0, 1, 7, 0, 'MAX_HP', 'maxhp*0.2*k', '150+150*k', '300', '', '600', 'maxhp*0.2*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '150+150*k', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (129, 0xB5D0B0A9BCFA, 0, 1, 40, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (170, 0xC2F7BFAD, 7, 1, 1, 0, 'HP', '-(1.1*atk+(0.3*atk+1.5*str)*k)', '40+100*k', '', '', '12', '-(1.1*atk+(0.3*atk+1.5*str)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_BLEEDING', NULL, 'NONE', '60*k', '5+25*k', NULL, '', '', '', '40+100*k', 0, 0, 'MELEE', 5, '1', 300, 200);
INSERT INTO `skill_proto` VALUES (171, 0xBBECC7B3, 7, 1, 1, 0, 'HP', '-(2*atk+(atk+dex*3+str*5+con)*k)', '40+120*k', '', '', '20', '-(2*atk+(atk+dex*3+str*5+con)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_STUN,KNOCKBACK', NULL, 'NONE', '30+70*2*k', '2', NULL, '', '', '', '40+120*k', 0, 0, 'MELEE', 10, '1', 1000, 200);
INSERT INTO `skill_proto` VALUES (172, 0xB0F8B4E4C2FC, 7, 1, 1, 0, 'HP', '-(atk+(1.6*atk+200+dex*7+str*7)*k)', '40+100*k', '', '', '12', '-(atk+(1.6*atk+200+dex*7+str*7)*k)', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '40+100*k', 0, 0, 'MELEE', 5, '1', 800, 100);
INSERT INTO `skill_proto` VALUES (173, 0xC6C4BCE2, 7, 1, 1, 0, 'HP', '-(3*atk+(0.8*atk+str*6+dex*2+con)*k)', '40+100*k', '', '', '12', '-(3*atk+(0.8*atk+str*6+dex*2+con)*k)', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'DEF_GRADE', '-30*k', '5', NULL, '', '', '', '40+100*k', 0, 0, 'MELEE', 10, '1', 800, 100);
INSERT INTO `skill_proto` VALUES (174, 0xC0FBB6FBBAF9C0C7, 7, 1, 1, 0, 'ATT_GRADE', '7+(4*iq+13)*k', '20+240*k', '45+60*k', '2+23*k', '10+10*k', '7+(4*iq+13)*k', '', 'SELFONLY,TOGGLE', 'RED_POSSESSION', 'NONE', '', '', NULL, '', '', '', '20+240*k', 0, 0, 'NORMAL', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (175, 0xC3BBB6FBBAF9C0C7, 7, 1, 1, 0, 'ATT_SPEED', '20*k', '80+220*k', '200', '', '300', '20*k', '', 'PARTY', 'BLUE_POSSESSION', 'DODGE', '1+10*k', '200', NULL, '', '', '', '80+220*k', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (6, 0xB1E2C7F7C3EBB8ED, 1, 1, 1, 0, 'HP', '-(3*atk+(atk+1.5*str)*k)*1.07', '300+150*k', '', '', '60', '-(3*atk+(atk+1.5*str)*k)*1.07', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '300+150*k', 0, 0, 'MELEE', 5, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (21, 0xB3FAB0CBB5B5B7A1, 1, 1, 1, 0, 'HP', '-(2*atk+(2*atk+2*dex+2*con+str*4)*k)*1.1', '300+180*k', '', '', '60', '-(2*atk+(2*atk+2*dex+2*con+str*4)*k)*1.1', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '300+180*k', 0, 0, 'MELEE', 10, '1', 0, 400);
INSERT INTO `skill_proto` VALUES (36, 0xC8E6BCB6BCF6, 2, 1, 1, 0, 'HP', '-((lv*2+(atk+str*3+dex*18)*k)*1.1)', '300+180*k', '', '', '60', '-((lv*2+(atk+str*3+dex*18)*k)*1.1)', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_POISON', NULL, 'NONE', '60*k', '5+25*k', NULL, '', '', '', '300+180*k', 0, 0, 'MELEE', 6, '1', 800, 300);
INSERT INTO `skill_proto` VALUES (51, 0xBCB6B1A4C5BA, 2, 1, 1, 0, 'HP', '-((atk+(1.2*atk+number(100,200)+dex*6+str*3+iq*3)*k)*1.2)', '200+200*k', '1+iq*0.4*k*0.1', '', '60', '-((atk+(1.2*atk+number(100,200)+dex*6+str*3+iq*3)*k)*1.2)', '', 'ATTACK,SPLASH,USE_ARROW_DAMAGE,CRUSH,ATTACK_POISON', NULL, 'NONE', '(50+dex*6*k)/10', '15+30*k', NULL, '', '', '', '200+200*k', 0, 0, 'RANGE', 5, '1', 0, 400);
INSERT INTO `skill_proto` VALUES (221, 0xC6C8B9E6C7B3BFECC6C4C7D8, 6, 1, 1, 0, 'NONE', '24*k', '', '', '', '', '24*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (222, 0xBECFBDC0C6C4C7D8, 6, 1, 1, 0, 'NONE', '24*k', '', '', '', '', '24*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (223, 0xBCE2B7C9C1F6C6C4C7D8, 6, 1, 1, 0, 'NONE', '24*k', '', '', '', '', '24*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (224, 0xBFEBC6C4BBEAC6C4C7D8, 6, 1, 1, 0, 'NONE', '24*k', '', '', '', '', '24*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (225, 0xB1E2B0F8C2FCC6C4C7D8, 6, 1, 1, 0, 'NONE', '24*k', '', '', '', '', '24*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (226, 0xC8ADC1B6C6C4C6C4C7D8, 6, 1, 1, 0, 'NONE', '24*k', '', '', '', '', '24*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (227, 0xB8B6B7C9C6C4C7D8, 6, 1, 1, 0, 'NONE', '24*k', '', '', '', '', '24*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (228, 0xBAADB6F4C6C4C7D8, 6, 1, 1, 0, 'NONE', '24*k', '', '', '', '', '24*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (229, 0xBBECC7B3C6C4C7D8, 6, 1, 1, 0, 'NONE', '24*k', '', '', '', '', '24*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (236, 0xC6C8B9E6C7B3BFECB0ADC8AD, 6, 1, 1, 0, 'NONE', '16*k', '', '', '', '', '16*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (237, 0xBECFBDC0B0ADC8AD, 6, 1, 1, 0, 'NONE', '16*k', '', '', '', '', '16*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (238, 0xBCE2B7C9C1F6B0ADC8AD, 6, 1, 1, 0, 'NONE', '16*k', '', '', '', '', '16*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (239, 0xBFEBC6C4BBEAB0ADC8AD, 6, 1, 1, 0, 'NONE', '16*k', '', '', '', '', '16*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (240, 0xB1E2B0F8C2FCB0ADC8AD, 6, 1, 1, 0, 'NONE', '16*k', '', '', '', '', '16*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (241, 0xC8ADC1B6C6C4B0ADC8AD, 6, 1, 1, 0, 'NONE', '16*k', '', '', '', '', '16*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (242, 0xB8B6B7C9B0ADC8AD, 6, 1, 1, 0, 'NONE', '16*k', '', '', '', '', '16*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (243, 0xBAADB6F4B0ADC8AD, 6, 1, 1, 0, 'NONE', '16*k', '', '', '', '', '16*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (244, 0xBBECC7B3B0ADC8AD, 6, 1, 1, 0, 'NONE', '16*k', '', '', '', '', '16*k', '', NULL, NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (263, 0x534C4F5734303030BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '12', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH', NULL, 'MOV_SPEED', '-70', '3', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 2000, 6000);
INSERT INTO `skill_proto` VALUES (264, 0x5448554E444552BDBAC5B3, 0, 1, 1, 0, 'HP', '-maxhp*k', '', '', '', '12', '-maxhp*k', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,CRUSH', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 500);
INSERT INTO `skill_proto` VALUES (265, 0x4845414C34303030BDBAC5B3, 0, 1, 1, 0, 'HP', '0.1*maxhp*k', '', '', '', '5', '0.1*maxhp*k', '', 'PARTY', 'YMIR', 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 3000, 0);
INSERT INTO `skill_proto` VALUES (266, 0x41545441434B5F534C4F57, 0, 1, 1, 0, 'NONE', '', '', '', '', '', '', '', 'SELFONLY,SPLASH', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 1, '1', 0, 3000);
INSERT INTO `skill_proto` VALUES (132, 0x4175746F41747461636B, 0, 0, 0, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 1000, 0);
INSERT INTO `skill_proto` VALUES (267, 0xC0CFB9DD30BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '10', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'MELEE', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (268, 0x435255534830BDBAC5B3, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '10', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,CRUSH', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'MELEE', 1, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (269, 0x5354554E323030, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '10', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_STUN', NULL, 'NONE', '1000', '3', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 300);
INSERT INTO `skill_proto` VALUES (270, 0x6E6F726D616C5F72616E67655F61747461636B5F636F6E66757374696F6E, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '5', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH', NULL, 'NONE', '80*k', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 525);
INSERT INTO `skill_proto` VALUES (133, 0x526F6C6550726F66696369656E6379, 0, 1, 40, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (134, 0x496E5369676874, 0, 1, 40, 0, 'NONE', '', '', '', '', '', '', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (271, 0x6B6E6F636B6261636B5F68705F6162736F7262, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '7', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,HP_ABSORB,CRUSH', NULL, 'NONE', '10', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 350);
INSERT INTO `skill_proto` VALUES (272, 0x6B6E6F636B6261636B5F626C656564696E67, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '7', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,CRUSH,ATTACK_BLEEDING', NULL, 'NONE', '100', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 350);
INSERT INTO `skill_proto` VALUES (273, 0x776964655F617265615F64616D6167655F7374756E, 0, 1, 1, 0, 'HP', '-maxhp*k', '', '', '', '12', '-maxhp*k', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_STUN,CRUSH', NULL, 'NONE', '1000', '4', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 500);
INSERT INTO `skill_proto` VALUES (274, 0x776964655F617265615F64616D6167655F73756E676D615F737472, 0, 1, 1, 0, 'NONE', '-40*k', '', '10', '', '12', '-40*k', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,CRUSH', NULL, 'HP', '-maxhp*k', '0', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 1000, 300);
INSERT INTO `skill_proto` VALUES (275, 0x73706C6173683430305F626C656564696E67, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '7', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_BLEEDING', NULL, 'NONE', '30', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 400);
INSERT INTO `skill_proto` VALUES (276, 0x7374756E333030, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '7', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_STUN', NULL, 'NONE', '200', '3', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 300);
INSERT INTO `skill_proto` VALUES (277, 0x73706C6173683430305F706F69736F6E, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '5', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_POISON', NULL, 'NONE', '10', '', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 400);
INSERT INTO `skill_proto` VALUES (278, 0x7374756E343030, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '7', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,ATTACK_STUN', NULL, 'NONE', '200', '3', NULL, '', '', '', '', 0, 0, 'MELEE', 0, '1', 0, 400);
INSERT INTO `skill_proto` VALUES (279, 0x6D617A655F6B65696E676E6F6C6C5F626F73735F736B696C6C33, 0, 1, 1, 0, 'HP', '-5*k*atk', '', '', '', '20', '-5*k*atk', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,HP_ABSORB', NULL, 'NONE', '50', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 400);
INSERT INTO `skill_proto` VALUES (246, 0x686974, 0, 1, 40, 0, 'NONE', '0.4*k+(32*k/2)', '', '', '', '', '0.4*k+(32*k/2)', '', 'DISABLE_BY_POINT_UP', NULL, 'NONE', '', '', NULL, '', '', '', '', 0, 0, 'NORMAL', 0, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (181, 0xB8DEC5D7BFC0, 4, 1, 1, 0, 'HP', '-(120+6*lv+(5*con+5*dex+30*iq+number(1,1000)+9*mwep)*ar*k)', '200+200*k', '', '', '35+20*k', '-(120+6*lv+(5*con+5*dex+30*iq+number(1,1000)+9*mwep)*ar*k)', '', 'ATTACK,SPLASH,ATTACK_FIRE_CONT,DISABLE_BY_POINT_UP', NULL, 'NONE', '0.4*k+(32*k/2)', '20*k', NULL, '', '', '', '200+200*k', 0, 0, 'MAGIC', 5, '1', 2000, 500);
INSERT INTO `skill_proto` VALUES (182, 0xC3B5BFEE, 4, 1, 1, 0, 'NONE', '0.4*k+(18*k)', '200+200*k', '50+(8*k)', '', '386+(80*k)', '0.4*k+(18*k)', '', 'DISABLE_BY_POINT_UP,PARTY', 'CHEONUN', 'NONE', '4*k', '', NULL, '', '', '', '200+200*k', 0, 0, 'NORMAL', 1, '1', 2000, 0);
INSERT INTO `skill_proto` VALUES (183, 0xC0CCB8AEC7B3BFEC, 7, 1, 1, 0, 'HP', '-(1.8*atk+(atk+dex*6+number(1,1000)+str*3+lv)*k)', '200+200*k', '', '', '35+20*k', '-(1.8*atk+(atk+dex*6+number(1,1000)+str*3+lv)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,DISABLE_BY_POINT_UP', NULL, 'NONE', '0.4*k+(32*k/2)', '20*k', NULL, '', '', '', '200+200*k', 0, 0, 'MELEE', 8, '1', 0, 200);
INSERT INTO `skill_proto` VALUES (176, 0xC7C7B4CFBDAC, 1, 1, 1, 0, 'HP', '-(3*atk+(0.9*atk+number(1,1000)+str*5+dex*3+lv)*k)', '200+200*k', '', '', '35+20*k', '-(3*atk+(0.9*atk+number(1,1000)+str*5+dex*3+lv)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,DISABLE_BY_POINT_UP', NULL, 'NONE', '0.4*k+(32*k/2)', '20*k', NULL, '', '', '', '200+200*k', 0, 0, 'MELEE', 8, '1', 0, 200);
INSERT INTO `skill_proto` VALUES (177, 0xC0CFB1A4C7A5, 2, 1, 1, 0, 'HP', '-(atk+(1.7*atk+number(1,1000)+dex*6+lv*5)*k)', '200+200*k', '', '', '35+20*k', '-(atk+(1.7*atk+number(1,1000)+dex*6+lv*5)*k)', '', 'ATTACK,USE_MELEE_DAMAGE,SPLASH,DISABLE_BY_POINT_UP', NULL, 'NONE', '0.4*k+(32*k/2)', '20*k', NULL, '', '', '', '200+200*k', 0, 0, 'MELEE', 8, '1', 2000, 200);
INSERT INTO `skill_proto` VALUES (178, 0xC7B3B7DAC6F7, 2, 1, 1, 0, 'HP', '-(1.9*atk+(2.6*atk+number(1,1000))*k)', '200+200*k', '', '', '35+20*k', '-(1.9*atk+(2.6*atk+number(1,1000))*k)', '', 'ATTACK,SPLASH,USE_ARROW_DAMAGE,DISABLE_BY_POINT_UP', NULL, 'NONE', '0.4*k+(32*k/2)', '20*k', NULL, '', '', '', '200+200*k', 0, 0, 'RANGE', 8, '1', 2500, 500);
INSERT INTO `skill_proto` VALUES (179, 0xBEC7B1CD, 3, 1, 1, 0, 'HP', '-(1.9*atk+(2.6*atk+number(1,1000))*k)', '200+200*k', '', '', '35+20*k', '-(1.9*atk+(2.6*atk+number(1,1000))*k)', '', 'ATTACK,USE_MELEE_DAMAGE,DISABLE_BY_POINT_UP', NULL, 'NONE', '0.4*k+(32*k/2)', '20*k', NULL, '', '', '', '200+200*k', 0, 0, 'MELEE', 4, '1', 0, 0);
INSERT INTO `skill_proto` VALUES (180, 0xC8E6B8B6BEC7B1CD, 3, 1, 1, 0, 'HP', '-(120+6*lv+(5*con+5*dex+30*iq+number(1,1000)+9*mwep)*ar*k)', '200+200*k', '', '', '35+20*k', '-(120+6*lv+(5*con+5*dex+30*iq+number(1,1000)+9*mwep)*ar*k)', '', 'ATTACK,COMPUTE_MAGIC_DAMAGE,SPLASH,DISABLE_BY_POINT_UP', NULL, 'NONE', '0.4*k+(32*k/2)', '20*k', NULL, '', '', '', '200+200*k', 0, 0, 'MAGIC', 5, '1', 2000, 200);

-- ----------------------------
-- Table structure for string
-- ----------------------------
DROP TABLE IF EXISTS `string`;
CREATE TABLE `string`  (
  `name` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `text` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  PRIMARY KEY (`name`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of string
-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
