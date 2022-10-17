/*
 Navicat Premium Data Transfer

 Source Server         : V5_TESTE
 Source Server Type    : MySQL
 Source Server Version : 80030
 Source Host           : 192.168.2.14:3306
 Source Schema         : log

 Target Server Type    : MySQL
 Target Server Version : 80030
 File Encoding         : 65001

 Date: 14/10/2022 06:45:32
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;
set @@sql_mode='';

-- ----------------------------
-- Table structure for acce
-- ----------------------------
DROP TABLE IF EXISTS `acce`;
CREATE TABLE `acce`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `pid` int NOT NULL,
  `time` datetime(0) NOT NULL,
  `x` int NOT NULL,
  `y` int NOT NULL,
  `item_vnum` int NOT NULL,
  `item_uid` int NOT NULL,
  `item_count` int NOT NULL,
  `item_abs_chance` int NOT NULL,
  `success` int NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of acce
-- ----------------------------

-- ----------------------------
-- Table structure for bootlog
-- ----------------------------
DROP TABLE IF EXISTS `bootlog`;
CREATE TABLE `bootlog`  (
  `time` datetime(0) NULL DEFAULT NULL,
  `hostname` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `channel` int NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of bootlog
-- ----------------------------

-- ----------------------------
-- Table structure for catck_king_event
-- ----------------------------
DROP TABLE IF EXISTS `catck_king_event`;
CREATE TABLE `catck_king_event`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `empire` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `max_score` int UNSIGNED NOT NULL DEFAULT 0,
  `total_score` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of catck_king_event
-- ----------------------------

-- ----------------------------
-- Table structure for change_job
-- ----------------------------
DROP TABLE IF EXISTS `change_job`;
CREATE TABLE `change_job`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `pid` int NOT NULL,
  `old_job` int NOT NULL,
  `new_job` int NOT NULL,
  `datetime` datetime(0) NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of change_job
-- ----------------------------

-- ----------------------------
-- Table structure for change_name
-- ----------------------------
DROP TABLE IF EXISTS `change_name`;
CREATE TABLE `change_name`  (
  `pid` int NOT NULL DEFAULT 0,
  `old_name` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `new_name` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `time` datetime(0) NULL DEFAULT NULL,
  `ip` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  PRIMARY KEY (`pid`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of change_name
-- ----------------------------

-- ----------------------------
-- Table structure for command_log
-- ----------------------------
DROP TABLE IF EXISTS `command_log`;
CREATE TABLE `command_log`  (
  `userid` int NULL DEFAULT NULL,
  `server` int NULL DEFAULT NULL,
  `ip` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  `port` int NULL DEFAULT NULL,
  `username` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'NONAME',
  `command` varchar(60) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `date` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00'
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of command_log
-- ----------------------------

-- ----------------------------
-- Table structure for cube
-- ----------------------------
DROP TABLE IF EXISTS `cube`;
CREATE TABLE `cube`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `pid` int NULL DEFAULT NULL,
  `time` datetime(0) NULL DEFAULT NULL,
  `x` int NULL DEFAULT NULL,
  `y` int NULL DEFAULT NULL,
  `item_vnum` int NULL DEFAULT NULL,
  `item_uid` varchar(40) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `item_count` int NULL DEFAULT NULL,
  `succes` int NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of cube
-- ----------------------------

-- ----------------------------
-- Table structure for dungeon_ranking
-- ----------------------------
DROP TABLE IF EXISTS `dungeon_ranking`;
CREATE TABLE `dungeon_ranking`  (
  `dungeon_id` int NOT NULL COMMENT 'Dungeon id',
  `name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL COMMENT 'Player name',
  `level` int NOT NULL COMMENT 'Player level',
  `finished` int NOT NULL COMMENT 'Dungeons finished',
  `fastest_time` int NOT NULL COMMENT 'Fastest time',
  `highest_damage` int NOT NULL COMMENT 'Highest damage on boss'
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of dungeon_ranking
-- ----------------------------

-- ----------------------------
-- Table structure for fish_log
-- ----------------------------
DROP TABLE IF EXISTS `fish_log`;
CREATE TABLE `fish_log`  (
  `1` int NULL DEFAULT NULL,
  `2` int NULL DEFAULT NULL,
  `3` int NULL DEFAULT NULL,
  `4` int NULL DEFAULT NULL,
  `5` int NULL DEFAULT NULL,
  `6` int NULL DEFAULT NULL,
  `7` int NULL DEFAULT NULL,
  `8` int NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of fish_log
-- ----------------------------

-- ----------------------------
-- Table structure for gmhost
-- ----------------------------
DROP TABLE IF EXISTS `gmhost`;
CREATE TABLE `gmhost`  (
  `mIP` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`mIP`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of gmhost
-- ----------------------------

-- ----------------------------
-- Table structure for gmlist
-- ----------------------------
DROP TABLE IF EXISTS `gmlist`;
CREATE TABLE `gmlist`  (
  `mID` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `mAccount` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `mName` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `mContactIP` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `mServerIP` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'ALL',
  `mAuthority` enum('IMPLEMENTOR','HIGH_WIZARD','GOD','LOW_WIZARD','PLAYER') CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT 'PLAYER',
  PRIMARY KEY (`mID`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of gmlist
-- ----------------------------

-- ----------------------------
-- Table structure for goldlog
-- ----------------------------
DROP TABLE IF EXISTS `goldlog`;
CREATE TABLE `goldlog`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `date` date NULL DEFAULT NULL,
  `time` time(0) NULL DEFAULT NULL,
  `pid` int NULL DEFAULT NULL,
  `what` int NULL DEFAULT NULL,
  `how` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `hint` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of goldlog
-- ----------------------------

-- ----------------------------
-- Table structure for hack_crc_log
-- ----------------------------
DROP TABLE IF EXISTS `hack_crc_log`;
CREATE TABLE `hack_crc_log`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `time` datetime(0) NULL DEFAULT NULL,
  `login` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `name` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `ip` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `server` int NULL DEFAULT NULL,
  `why` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `crc` int NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hack_crc_log
-- ----------------------------

-- ----------------------------
-- Table structure for hack_log
-- ----------------------------
DROP TABLE IF EXISTS `hack_log`;
CREATE TABLE `hack_log`  (
  `time` time(0) NULL DEFAULT NULL,
  `login` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `name` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `ip` varchar(15) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `server` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `why` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hack_log
-- ----------------------------

-- ----------------------------
-- Table structure for hackshield_log
-- ----------------------------
DROP TABLE IF EXISTS `hackshield_log`;
CREATE TABLE `hackshield_log`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `time` datetime(0) NULL DEFAULT NULL,
  `account_id` int NULL DEFAULT NULL,
  `login` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `pid` int NULL DEFAULT NULL,
  `name` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `reason` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `ip` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hackshield_log
-- ----------------------------

-- ----------------------------
-- Table structure for levellog
-- ----------------------------
DROP TABLE IF EXISTS `levellog`;
CREATE TABLE `levellog`  (
  `name` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `level` int NULL DEFAULT NULL,
  `time` time(0) NULL DEFAULT NULL,
  `playtime` int NULL DEFAULT NULL,
  `account_id` int NULL DEFAULT NULL,
  `pid` int NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of levellog
-- ----------------------------

-- ----------------------------
-- Table structure for locale
-- ----------------------------
DROP TABLE IF EXISTS `locale`;
CREATE TABLE `locale`  (
  `mKey` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `mValue` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`mKey`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of locale
-- ----------------------------

-- ----------------------------
-- Table structure for locale_bug
-- ----------------------------
DROP TABLE IF EXISTS `locale_bug`;
CREATE TABLE `locale_bug`  (
  `mKey` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `mValue` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`mKey`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of locale_bug
-- ----------------------------

-- ----------------------------
-- Table structure for log
-- ----------------------------
DROP TABLE IF EXISTS `log`;
CREATE TABLE `log`  (
  `ID` int NOT NULL AUTO_INCREMENT,
  `type` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `time` time(0) NULL DEFAULT NULL,
  `who` int NULL DEFAULT NULL,
  `x` bigint NULL DEFAULT NULL,
  `y` bigint NULL DEFAULT NULL,
  `what` bigint NULL DEFAULT NULL,
  `how` varchar(80) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `hint` varchar(180) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `ip` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `vnum` int NULL DEFAULT NULL,
  `DELAYED` varchar(5) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of log
-- ----------------------------

-- ----------------------------
-- Table structure for loginlog
-- ----------------------------
DROP TABLE IF EXISTS `loginlog`;
CREATE TABLE `loginlog`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `type` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  `time` datetime(0) NULL DEFAULT NULL,
  `channel` int NULL DEFAULT NULL,
  `account_id` int NULL DEFAULT NULL,
  `pid` int NULL DEFAULT NULL,
  `level` int NULL DEFAULT NULL,
  `job` int NULL DEFAULT NULL,
  `playtime` int NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of loginlog
-- ----------------------------

-- ----------------------------
-- Table structure for loginlog2
-- ----------------------------
DROP TABLE IF EXISTS `loginlog2`;
CREATE TABLE `loginlog2`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `type` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  `is_gm` tinytext CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  `login_time` datetime(0) NULL DEFAULT NULL,
  `channel` int NULL DEFAULT NULL,
  `account_id` int NULL DEFAULT NULL,
  `pid` int NULL DEFAULT NULL,
  `client_version` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  `ip` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL,
  `logout_time` datetime(0) NULL DEFAULT NULL,
  `playtime` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of loginlog2
-- ----------------------------

-- ----------------------------
-- Table structure for mailbox_log
-- ----------------------------
DROP TABLE IF EXISTS `mailbox_log`;
CREATE TABLE `mailbox_log`  (
  `name` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `who` varchar(24) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `title` varchar(25) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `message` varchar(100) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `gm` tinyint NOT NULL DEFAULT 0,
  `gold` int NOT NULL DEFAULT 0,
  `won` int NOT NULL DEFAULT 0,
  `ivnum` int UNSIGNED NOT NULL DEFAULT 0,
  `icount` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `date` datetime(0) NOT NULL DEFAULT CURRENT_TIMESTAMP(0)
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of mailbox_log
-- ----------------------------

-- ----------------------------
-- Table structure for meley_dungeon
-- ----------------------------
DROP TABLE IF EXISTS `meley_dungeon`;
CREATE TABLE `meley_dungeon`  (
  `guild_id` int UNSIGNED NOT NULL DEFAULT 0,
  `participants` int UNSIGNED NOT NULL DEFAULT 0,
  `time` int UNSIGNED NOT NULL DEFAULT 0,
  `date` datetime(0) NOT NULL,
  PRIMARY KEY (`guild_id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of meley_dungeon
-- ----------------------------

-- ----------------------------
-- Table structure for money_log
-- ----------------------------
DROP TABLE IF EXISTS `money_log`;
CREATE TABLE `money_log`  (
  `time` datetime(0) NULL DEFAULT NULL,
  `type` int NULL DEFAULT NULL,
  `vnum` int NULL DEFAULT NULL,
  `gold` int NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of money_log
-- ----------------------------

-- ----------------------------
-- Table structure for pcbang_loginlog
-- ----------------------------
DROP TABLE IF EXISTS `pcbang_loginlog`;
CREATE TABLE `pcbang_loginlog`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `time` datetime(0) NULL DEFAULT NULL,
  `pcbang_id` int NULL DEFAULT NULL,
  `ip` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `pid` int NULL DEFAULT NULL,
  `play_time` int NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of pcbang_loginlog
-- ----------------------------

-- ----------------------------
-- Table structure for quest_reward_log
-- ----------------------------
DROP TABLE IF EXISTS `quest_reward_log`;
CREATE TABLE `quest_reward_log`  (
  `1` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `2` int NULL DEFAULT NULL,
  `3` int NULL DEFAULT NULL,
  `4` int NULL DEFAULT NULL,
  `5` int NULL DEFAULT NULL,
  `6` int NULL DEFAULT NULL,
  `7` time(0) NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of quest_reward_log
-- ----------------------------

-- ----------------------------
-- Table structure for refinelog
-- ----------------------------
DROP TABLE IF EXISTS `refinelog`;
CREATE TABLE `refinelog`  (
  `Id` int NULL DEFAULT NULL,
  `pid` int NULL DEFAULT NULL,
  `item_name` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `item_id` int NULL DEFAULT NULL,
  `step` int NULL DEFAULT NULL,
  `time` time(0) NULL DEFAULT NULL,
  `is_success` int NULL DEFAULT NULL,
  `setType` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of refinelog
-- ----------------------------

-- ----------------------------
-- Table structure for shout_log
-- ----------------------------
DROP TABLE IF EXISTS `shout_log`;
CREATE TABLE `shout_log`  (
  `1` time(0) NULL DEFAULT NULL,
  `2` int NULL DEFAULT NULL,
  `3` int NULL DEFAULT NULL,
  `4` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of shout_log
-- ----------------------------

-- ----------------------------
-- Table structure for speed_hack
-- ----------------------------
DROP TABLE IF EXISTS `speed_hack`;
CREATE TABLE `speed_hack`  (
  `pid` int NULL DEFAULT NULL,
  `time` time(0) NULL DEFAULT NULL,
  `x` int NULL DEFAULT NULL,
  `y` int NULL DEFAULT NULL,
  `hack_count` varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of speed_hack
-- ----------------------------

-- ----------------------------
-- Table structure for whisper_log
-- ----------------------------
DROP TABLE IF EXISTS `whisper_log`;
CREATE TABLE `whisper_log`  (
  `time` datetime(0) NOT NULL,
  `from` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `to` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `message` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT ''
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of whisper_log
-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
