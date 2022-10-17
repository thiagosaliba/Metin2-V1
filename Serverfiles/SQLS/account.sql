/*
 Navicat Premium Data Transfer

 Source Server         : V5_TESTE
 Source Server Type    : MySQL
 Source Server Version : 80030
 Source Host           : 192.168.2.14:3306
 Source Schema         : account

 Target Server Type    : MySQL
 Target Server Version : 80030
 File Encoding         : 65001

 Date: 14/10/2022 06:45:23
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;
set @@sql_mode='';

-- ----------------------------
-- Table structure for account
-- ----------------------------
DROP TABLE IF EXISTS `account`;
CREATE TABLE `account`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `login` varchar(30) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `password` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `real_name` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT '',
  `social_id` varchar(13) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `email` varchar(64) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT '',
  `create_time` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `question1` varchar(48) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `answer1` varchar(48) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `question2` varchar(48) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `answer2` varchar(48) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `is_testor` tinyint(1) NOT NULL DEFAULT 0,
  `status` varchar(8) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL DEFAULT 'OK',
  `securitycode` varchar(192) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT '',
  `newsletter` tinyint(1) NULL DEFAULT 0,
  `empire` tinyint NOT NULL DEFAULT 0,
  `name_checked` tinyint(1) NOT NULL DEFAULT 0,
  `availDt` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `mileage` int NOT NULL DEFAULT 0,
  `cash` int NOT NULL DEFAULT 0,
  `gold_expire` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `silver_expire` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `safebox_expire` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `autoloot_expire` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `fish_mind_expire` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `marriage_fast_expire` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `money_drop_rate_expire` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `last_play` datetime(0) NOT NULL DEFAULT '0000-00-00 00:00:00',
  `language` tinyint NOT NULL DEFAULT 1,
  `web_admin` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `login`(`login`) USING BTREE,
  INDEX `social_id`(`social_id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 2 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of account
-- ----------------------------
INSERT INTO `account` VALUES (1, 'turkmmo', '471544d398c996530ff417012ef4cede420a0713', 'turkmmo', '1234567', '', '0000-00-00 00:00:00', 'Are you the master?', 'Yes', NULL, NULL, 1, 'OK', '', 0, 0, 1, '0000-00-00 00:00:00', 99999, 99999, '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00', 1, 10);

-- ----------------------------
-- Table structure for admin
-- ----------------------------
DROP TABLE IF EXISTS `admin`;
CREATE TABLE `admin`  (
  `id` int NOT NULL DEFAULT 0,
  `admin` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `password` varchar(50) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of admin
-- ----------------------------
INSERT INTO `admin` VALUES (1, 'turkmmo', '471544d398c996530ff417012ef4cede420a0713');

-- ----------------------------
-- Table structure for ban_log
-- ----------------------------
DROP TABLE IF EXISTS `ban_log`;
CREATE TABLE `ban_log`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `admin_id` int UNSIGNED NOT NULL,
  `account_id` int UNSIGNED NOT NULL,
  `time` datetime(0) NOT NULL,
  `reason` varchar(200) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `typ` varchar(5) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of ban_log
-- ----------------------------

-- ----------------------------
-- Table structure for block_exception
-- ----------------------------
DROP TABLE IF EXISTS `block_exception`;
CREATE TABLE `block_exception`  (
  `login` int NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of block_exception
-- ----------------------------
INSERT INTO `block_exception` VALUES (1);

-- ----------------------------
-- Table structure for hwid
-- ----------------------------
DROP TABLE IF EXISTS `hwid`;
CREATE TABLE `hwid`  (
  `account_id` int NOT NULL AUTO_INCREMENT COMMENT 'Account ID',
  `hwid` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT '' COMMENT 'HWID',
  `old_hwid` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT '' COMMENT 'Old HWID',
  `valid1` tinyint(1) NULL DEFAULT 0 COMMENT 'Valid PIN Index 1',
  `valid2` tinyint(1) NULL DEFAULT 0 COMMENT 'Valid PIN Index 2',
  `valid3` tinyint(1) NULL DEFAULT 0 COMMENT 'Valid PIN Index 3',
  `valid4` tinyint(1) NULL DEFAULT 0 COMMENT 'Valid PIN Index 4',
  `valid5` tinyint(1) NULL DEFAULT 0 COMMENT 'Valid PIN Index 5',
  `ban` tinyint(1) NULL DEFAULT 0 COMMENT 'Banned',
  PRIMARY KEY (`account_id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hwid
-- ----------------------------

-- ----------------------------
-- Table structure for iptocountry
-- ----------------------------
DROP TABLE IF EXISTS `iptocountry`;
CREATE TABLE `iptocountry`  (
  `IP_FROM` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `IP_TO` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `COUNTRY_NAME` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of iptocountry
-- ----------------------------
INSERT INTO `iptocountry` VALUES ('127.0.0.1', '127.0.0.1', 'localhost');

-- ----------------------------
-- Table structure for monarch
-- ----------------------------
DROP TABLE IF EXISTS `monarch`;
CREATE TABLE `monarch`  (
  `empire` int UNSIGNED NOT NULL DEFAULT 0,
  `pid` int UNSIGNED NULL DEFAULT 0,
  `name` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `windate` datetime(0) NULL DEFAULT '0000-00-00 00:00:00',
  `money` bigint UNSIGNED NULL DEFAULT 0,
  PRIMARY KEY (`empire`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of monarch
-- ----------------------------

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
