#
# OSF Info
# Child of constInfo.py
#
# General Options
#

import app

DEBUG_MODE = False # Debug mode (debug info and console)

USE_NEW_WHISPER_LETTER_CATEGORY = True # Use the new letter category for normal and gm players

WHISPER_ON_CHAT_LINE = True # Show whisper in chat line
PAST_HEALTH_ON_TARGET = True # Show past health on target (yellow gauge)
SHOW_LOADING_PROGRESS = True # Show loading progress
EXTENDED_LOADING_GUAGE = True # Loading guage extended with description
AFFECT_DISPLAY_IN_THINBOARD = False # Show align affect's using a ThinBoard
ALIGN_AFFECT_IMG_LIST = True # Align affect image list
FILTER_EMOJI_NOTICE = True # Filter emojis on tip notice or big notice to chat line

SHOW_EMOJI_IN_CHAT_LINE = True # Show emoji in chat line
SHOW_EMOJI_IN_WHISPER_LINE = False # Show emoji in whisper line
SHOW_USE_IMAGE_TEXT_LINE = True # Show use item image text line in tooltip
SHOW_SELL_IMAGE_TEXT_LINE = False # Show sell item image text line in tooltip

SHOW_REFINE_ITEM_DESC = True # Show refine item description
ENABLE_OLD_QUEST_LETTER_LIST = True # Enable the old quest letter list

EVENT_BANNER = True # Show event banner instead of mini-game rumi button

SHOW_REFINE_PERCENTAGE = True # Show refine success percentage

MOUNT_ITEM_ATTR_TOOLTIP = True # Show quest item attributes on tooltip
PET_ITEM_ATTR_TOOLTIP = True # Show quest item attributes on tooltip
SHOW_BELT_SLOTS_TOOLTIP = False # Show maximum slots of belt item on tooltip
SHOW_ITEM_VNUM_TOOTIP = True # Show the item vnum

LOGIN_UI_MODIFY = False # Login countdown
NEW_SELECT_UI = True # New select character interface
NEW_CREATE_UI = True # New create character interface
SELECT_CHARACTER_ROTATION = True # Enables character rotation in select phase.

if app.ENABLE_SKILL_COLOR_SYSTEM:
	SKILL_COLOR_BUFF_ONLY = False # Enable skill color button on buffs only

##################################################################################

if app.ENABLE_EXPRESSING_EMOTION:
	import emotion
	EXPRESSING_EMOTIONS = {}

if app.ENABLE_HIDE_COSTUME_SYSTEM:
	HIDDEN_BODY_COSTUME = 0
	HIDDEN_HAIR_COSTUME = 0
	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		HIDDEN_ACCE_COSTUME = 0
	if app.ENABLE_WEAPON_COSTUME_SYSTEM:
		HIDDEN_WEAPON_COSTUME = 0

if app.ENABLE_MESSENGER_BLOCK:
	MESSENGER_KEY = 0

INPUT_IGNORE = 0

if app.ENABLE_DUNGEON_INFO_SYSTEM:
	dungeonInfo = []
	dungeonRanking = {
		"ranking_type" : 0,
		"ranking_list" : [],
		"my_ranking" : [],
	}
	dungeonData = {
		"quest_index" : 0,
		"quest_cmd" : "",
	}

# MINI_GAME
if app.ENABLE_MINI_GAME_OKEY:
	RUMI_GAME_EVENT = False
if app.ENABLE_MINI_GAME_CATCH_KING:
	CATCH_KING_GAME_EVENT = False
# END_OF_MINI_GAME

if app.ENABLE_AUTO_REFINE:
	AUTO_REFINE = False
	IS_AUTO_REFINE = False
	AUTO_REFINE_TYPE = 0
	AUTO_REFINE_DATA = {
		"ITEM" : [-1, -1],
		"NPC" : [0, -1, -1, 0]
	}

def IsDebugMode():
	if DEBUG_MODE == True:
		return True

	return False

def IS_ANTI_EXP_RING(itemVnum):
	if itemVnum == 72501:
		return 1

	return 0

if app.ENABLE_EXTENDED_BLEND_AFFECT:
	def IS_BLEND_POTION(itemVnum):
		if itemVnum >= 50821 and itemVnum <= 50826:
			return 1
		elif itemVnum == 51002:
			return 1

		return 0

	def IS_EXTENDED_BLEND_POTION(itemVnum):
		if itemVnum >= 950821 and itemVnum <= 950826: # Dews
			return 1
		elif itemVnum == 951002: # Cristal Energy
			return 1
		elif itemVnum >= 939017 and itemVnum <= 939020: # Dragon God Medals
			return 1
		elif itemVnum == 939024 or itemVnum == 939025: # Critical & Penetration
			return 1
		elif itemVnum == 927209 or itemVnum == 927212: # Attack Speed & Move Speed
			return 1

		return 0

if app.ENABLE_EXPRESSING_EMOTION:
	def GET_SPECIAL_EMOTION_NAME(idx):
		if idx >= app.SPECIAL_ACTION_START_INDEX and idx <= emotion.EMOTION_WHIRL:
			return emotion.SPECIAL_EMOTION_DICT[idx]["name"]

def StripColor(text):
	import re

	regex = '\|c([a-zA-Z0-9]){0,8}|'
	search = re.search(regex, text)
	if search:
		text = re.sub(regex, '', text)

	return text

def IsEmojiFilter(text):
	import re

	regex = '\|I.*\|i'
	search = re.search(regex, text)
	if search:
		return True

	return False

def IsHyperLinkFilter(text):
	import re

	regex = '\|H.*\|h'
	search = re.search(regex, text)
	if search:
		return True

	return False

def EmojiFilter(text):
	import re

	EMOJI_DIC = {
		':o' : 'emoji/amazed',
		':)' : 'emoji/happiness',
		':D' : 'emoji/happy',
		':\\' : 'emoji/indifferent',
		':p' : 'emoji/tongue',
		':P' : 'emoji/tongue',
		'xP' : 'emoji/dead',
		':X' : 'emoji/muted',
		':x' : 'emoji/muted',
		'>:D' : 'emoji/evil',
		':3' : 'emoji/in-love',
		'<3' : 'emoji/heart',
		':*' : 'emoji/kiss',
		':O' : 'emoji/surprised',
		'>:(' : 'emoji/angry',
		':||' : 'emoji/quiet',
	}

	for emoji_str, emoji_img in EMOJI_DIC.iteritems():
		text = text.replace(emoji_str, "|I" + emoji_img + "|i")

	return text
