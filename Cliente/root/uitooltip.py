import dbg
import player
import item
import grp
import wndMgr
import skill
import shop
import exchange
import grpText
import safebox
import localeInfo
import app
import background
import nonplayer
import chr

import ui
import mouseModule
import constInfo
import osfInfo

if app.ENABLE_ACCE_COSTUME_SYSTEM:
	import acce

if app.ENABLE_DS_SET:
	import uiDragonSoul

if app.ENABLE_MAILBOX:
	import mail

WARP_SCROLLS = [22011, 22000, 22010]
BELT_SLOTS = [0, 1, 2, 4, 6, 9, 12, 16]

DESC_DEFAULT_MAX_COLS = 26 
DESC_WESTERN_MAX_COLS = 35
DESC_WESTERN_MAX_WIDTH = 220

def chop(n):
	return round(n - 0.5, 1)

def SplitDescription(desc, limit):
	total_tokens = desc.split()
	line_tokens = []
	line_len = 0
	lines = []

	for token in total_tokens:
		if "|" in token:
			sep_pos = token.find("|")
			line_tokens.append(token[:sep_pos])
			lines.append(" ".join(line_tokens))
			line_len = len(token) - (sep_pos + 1)
			line_tokens = [token[sep_pos+1:]]
		elif app.WJ_MULTI_TEXTLINE and "\\n" in token:
			sep_pos = token.find("\\n")
			line_tokens.append(token[:sep_pos])

			lines.append(" ".join(line_tokens))
			line_len = len(token) - (sep_pos + 2)
			line_tokens = [token[sep_pos+2:]]
		else:
			line_len += len(token)
			if len(line_tokens) + line_len > limit:
				lines.append(" ".join(line_tokens))
				line_len = len(token)
				line_tokens = [token]
			else:
				line_tokens.append(token)

	if line_tokens:
		lines.append(" ".join(line_tokens))

	return lines

###################################################################################################
## ToolTip
##
##	NOTE : 현재는 Item과 Skill을 상속으로 특화 시켜두었음
##			하지만 그다지 의미가 없어 보임
##
class ToolTip(ui.ThinBoard):
	TOOL_TIP_WIDTH = 190
	TOOL_TIP_HEIGHT = 10

	TEXT_LINE_HEIGHT = 17

	TITLE_COLOR = grp.GenerateColor(0.9490, 0.9058, 0.7568, 1.0)
	SPECIAL_TITLE_COLOR = grp.GenerateColor(1.0, 0.7843, 0.0, 1.0)
	NORMAL_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
	FONT_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
	PRICE_COLOR = 0xffFFB96D

	HIGH_PRICE_COLOR = SPECIAL_TITLE_COLOR
	MIDDLE_PRICE_COLOR = grp.GenerateColor(0.85, 0.85, 0.85, 1.0)
	LOW_PRICE_COLOR = grp.GenerateColor(0.7, 0.7, 0.7, 1.0)

	ENABLE_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)
	DISABLE_COLOR = grp.GenerateColor(0.9, 0.4745, 0.4627, 1.0)

	NEGATIVE_COLOR = grp.GenerateColor(0.9, 0.4745, 0.4627, 1.0)
	POSITIVE_COLOR = grp.GenerateColor(0.5411, 0.7254, 0.5568, 1.0)
	SPECIAL_POSITIVE_COLOR = grp.GenerateColor(0.6911, 0.8754, 0.7068, 1.0)
	SPECIAL_POSITIVE_COLOR2 = grp.GenerateColor(0.8824, 0.9804, 0.8824, 1.0)

	if app.ENABLE_DS_SET:
		TEXTLINE_2ND_COLOR_DEFAULT = grp.GenerateColor(1.0, 1.0, 0.6078, 1.0)

	CONDITION_COLOR = 0xffBEB47D
	CAN_LEVEL_UP_COLOR = 0xff8EC292
	CANNOT_LEVEL_UP_COLOR = DISABLE_COLOR
	NEED_SKILL_POINT_COLOR = 0xff9A9CDB

	if app.ENABLE_CHANGE_LOOK_SYSTEM:
		CHANGELOOK_TITLE_COLOR = 0xff8BBDFF
		CHANGELOOK_ITEMNAME_COLOR = 0xffBCE55C

	if app.ENABLE_APPLY_RANDOM:
		APPLY_RANDOM_TEXT_COLOR = 0xff7AF6D4

	def __init__(self, width = TOOL_TIP_WIDTH, isPickable=False):
		ui.ThinBoard.__init__(self, "TOP_MOST")

		if isPickable:
			pass
		else:
			self.AddFlag("not_pick")

		self.AddFlag("float")

		self.followFlag = True
		self.toolTipWidth = width

		self.xPos = -1
		self.yPos = -1

		if app.ENABLE_DS_SET:
			self.window_type = player.INVENTORY

		self.defFontName = localeInfo.UI_DEF_FONT
		self.ClearToolTip()

	def __del__(self):
		ui.ThinBoard.__del__(self)

	def ClearToolTip(self):
		self.toolTipHeight = 12
		self.childrenList = []

	def SetFollow(self, flag):
		self.followFlag = flag

	def SetDefaultFontName(self, fontName):
		self.defFontName = fontName

	def AppendSpace(self, size):
		self.toolTipHeight += size
		self.ResizeToolTip()

	def AppendHorizontalLine(self):
		for i in xrange(2):
			horizontalLine = ui.Line()
			horizontalLine.SetParent(self)
			horizontalLine.SetPosition(0, self.toolTipHeight + 3 + i)
			horizontalLine.SetWindowHorizontalAlignCenter()
			horizontalLine.SetSize(150, 0)
			horizontalLine.Show()

			if 0 == i:
				horizontalLine.SetColor(0xff555555)
			else:
				horizontalLine.SetColor(0xff000000)

			self.childrenList.append(horizontalLine)

		self.toolTipHeight += 11
		self.ResizeToolTip()

	def AlignHorizonalCenter(self):
		for child in self.childrenList:
			(x, y) = child.GetLocalPosition()
			child.SetPosition(self.toolTipWidth/2, y)

		self.ResizeToolTip()

	def AlignTextLineHorizonalCenter(self):
		for child in self.childrenList:
			if type(child).__name__ == "TextLine":
				(x, y) = child.GetLocalPosition()
				child.SetPosition(self.toolTipWidth / 2, y)

		self.ResizeToolTip()

	def AutoAppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.Show()

		if centerAlign:
			textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
			textLine.SetHorizontalAlignCenter()
		else:
			textLine.SetPosition(10, self.toolTipHeight)

		self.childrenList.append(textLine)

		(textWidth, textHeight)=textLine.GetTextSize()

		textWidth += 40
		textHeight += 5

		if self.toolTipWidth < textWidth:
			self.toolTipWidth = textWidth

		self.toolTipHeight += textHeight

		return textLine

	def SetThinBoardSize(self, width, height = 12):
		self.toolTipWidth = width 
		self.toolTipHeight = height

	def AppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPackedFontColor(color)
		textLine.SetText(text)
		textLine.SetOutline()
		textLine.SetFeather(False)
		if app.WJ_MULTI_TEXTLINE:
			textLine.SetLineHeight(self.TEXT_LINE_HEIGHT)
		textLine.Show()

		if centerAlign:
			textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
			textLine.SetHorizontalAlignCenter()

		else:
			textLine.SetPosition(10, self.toolTipHeight)

		self.childrenList.append(textLine)

		if app.WJ_MULTI_TEXTLINE:
			lineCount = textLine.GetTextLineCount()
			self.toolTipHeight += self.TEXT_LINE_HEIGHT * lineCount
		else:
			self.toolTipHeight += self.TEXT_LINE_HEIGHT

		self.ResizeToolTip()

		return textLine

	if app.ENABLE_DS_SET:
		def AppendTwoColorTextLine(self, text, color, text2, color2 = TEXTLINE_2ND_COLOR_DEFAULT, centerAlign = True):
			textLine = ui.TextLine()
			textLine.SetParent(self)
			textLine.SetFontName(self.defFontName)
			textLine.SetPackedFontColor(color)
			textLine.SetText(text)
			textLine.SetOutline()
			textLine.SetFeather(False)
			w, h = textLine.GetTextSize()

			textLine2 = ui.TextLine()
			textLine2.SetParent(textLine)
			textLine2.SetFontName(self.defFontName)
			textLine2.SetPackedFontColor(color2)
			textLine2.SetText(text2)
			textLine2.SetOutline()
			textLine2.SetFeather(False)
			textLine2.Show()

			w2, h2 = textLine2.GetTextSize()

			textLine.SetLineHeight(self.TEXT_LINE_HEIGHT)
			textLine.Show()

			if centerAlign:
				if localeInfo.IsARABIC():
					textLine.SetPosition(self.toolTipWidth / 2 + w2 / 2, self.toolTipHeight)
					textLine.SetHorizontalAlignCenter()
					textLine2.SetPosition(-w / 2, 0)
				else:
					textLine.SetPosition(self.toolTipWidth / 2 -w2 / 2, self.toolTipHeight)
					textLine.SetHorizontalAlignCenter()
					textLine2.SetPosition(w / 2, 0)
			else:
				textLine.SetPosition(10, self.toolTipHeight)

			self.childrenList.append(textLine)
			self.childrenList.append(textLine2)

			lineCount = textLine.GetTextLineCount()
			self.toolTipHeight += self.TEXT_LINE_HEIGHT * lineCount

			self.ResizeToolTip()

			return textLine

	def AppendDescription(self, desc, limit, color = FONT_COLOR):
		if localeInfo.IsEUROPE():
			self.__AppendDescription_WesternLanguage(desc, color)
		else:
			self.__AppendDescription_EasternLanguage(desc, limit, color)

	def __AppendDescription_EasternLanguage(self, description, characterLimitation, color=FONT_COLOR):
		length = len(description)
		if 0 == length:
			return

		lineCount = grpText.GetSplitingTextLineCount(description, characterLimitation)
		for i in xrange(lineCount):
			if 0 == i:
				self.AppendSpace(5)
			self.AppendTextLine(grpText.GetSplitingTextLine(description, characterLimitation, i), color)

	def __AppendDescription_WesternLanguage(self, desc, color=FONT_COLOR):
		lines = SplitDescription(desc, DESC_WESTERN_MAX_COLS)
		if not lines:
			return

		self.AppendSpace(5)
		for line in lines:
			self.AppendTextLine(line, color)

	def ResizeToolTip(self):
		self.SetSize(self.toolTipWidth, self.TOOL_TIP_HEIGHT + self.toolTipHeight)

	def SetTitle(self, name):
		self.AppendTextLine(name, self.TITLE_COLOR)

	def GetLimitTextLineColor(self, curValue, limitValue):
		if curValue < limitValue:
			return self.DISABLE_COLOR

		return self.ENABLE_COLOR

	def GetChangeTextLineColor(self, value, isSpecial=False):
		if value > 0:
			if isSpecial:
				return self.SPECIAL_POSITIVE_COLOR
			else:
				return self.POSITIVE_COLOR

		if 0 == value:
			return self.NORMAL_COLOR

		return self.NEGATIVE_COLOR

	def SetToolTipPosition(self, x = -1, y = -1):
		self.xPos = x
		self.yPos = y

	def ShowToolTip(self):
		self.SetTop()
		self.Show()

		self.OnUpdate()

	def HideToolTip(self):
		self.Hide()

	if app.ENABLE_EXPRESSING_EMOTION:
		def AppendLastTime(self, endTime):
			leftSec = max(0, endTime - app.GetGlobalTimeStamp())
			if leftSec > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(leftSec), self.NORMAL_COLOR)

	def OnUpdate(self):
		if not self.followFlag:
			return

		x = 0
		y = 0
		width = self.GetWidth()
		height = self.toolTipHeight

		if -1 == self.xPos and -1 == self.yPos:
			(mouseX, mouseY) = wndMgr.GetMousePosition()

			if mouseY < wndMgr.GetScreenHeight() - 300:
				y = mouseY + 40
			else:
				y = mouseY - height - 30

			x = mouseX - width/2

		else:
			x = self.xPos - width/2
			y = self.yPos - height

		x = max(x, 0)
		y = max(y, 0)
		x = min(x + width/2, wndMgr.GetScreenWidth() - width/2) - width/2
		y = min(y + self.GetHeight(), wndMgr.GetScreenHeight()) - self.GetHeight()

		parentWindow = self.GetParentProxy()
		if parentWindow:
			(gx, gy) = parentWindow.GetGlobalPosition()
			x -= gx
			y -= gy

		self.SetPosition(x, y)

class ItemToolTip(ToolTip):
	if not app.DISABLE_WOLFMAN_CREATION:
		CHARACTER_NAMES = (
			localeInfo.TOOLTIP_WARRIOR,
			localeInfo.TOOLTIP_ASSASSIN,
			localeInfo.TOOLTIP_SURA,
			localeInfo.TOOLTIP_SHAMAN,
			localeInfo.TOOLTIP_WOLFMAN
		)
	else:
		CHARACTER_NAMES = (
			localeInfo.TOOLTIP_WARRIOR,
			localeInfo.TOOLTIP_ASSASSIN,
			localeInfo.TOOLTIP_SURA,
			localeInfo.TOOLTIP_SHAMAN,
		)

	CHARACTER_COUNT = len(CHARACTER_NAMES)
	WEAR_NAMES = ( 
		localeInfo.TOOLTIP_ARMOR, 
		localeInfo.TOOLTIP_HELMET, 
		localeInfo.TOOLTIP_SHOES, 
		localeInfo.TOOLTIP_WRISTLET, 
		localeInfo.TOOLTIP_WEAPON, 
		localeInfo.TOOLTIP_NECK,
		localeInfo.TOOLTIP_EAR,
		localeInfo.TOOLTIP_UNIQUE,
		localeInfo.TOOLTIP_SHIELD,
		localeInfo.TOOLTIP_ARROW,
	)
	WEAR_COUNT = len(WEAR_NAMES)

	if not app.DISABLE_WOLFMAN_CREATION:
		AFFECT_DICT = {
			item.APPLY_MAX_HP : localeInfo.TOOLTIP_MAX_HP,
			item.APPLY_MAX_SP : localeInfo.TOOLTIP_MAX_SP,
			item.APPLY_CON : localeInfo.TOOLTIP_CON,
			item.APPLY_INT : localeInfo.TOOLTIP_INT,
			item.APPLY_STR : localeInfo.TOOLTIP_STR,
			item.APPLY_DEX : localeInfo.TOOLTIP_DEX,
			item.APPLY_ATT_SPEED : localeInfo.TOOLTIP_ATT_SPEED,
			item.APPLY_MOV_SPEED : localeInfo.TOOLTIP_MOV_SPEED,
			item.APPLY_CAST_SPEED : localeInfo.TOOLTIP_CAST_SPEED,
			item.APPLY_HP_REGEN : localeInfo.TOOLTIP_HP_REGEN,
			item.APPLY_SP_REGEN : localeInfo.TOOLTIP_SP_REGEN,
			item.APPLY_POISON_PCT : localeInfo.TOOLTIP_APPLY_POISON_PCT,
			item.APPLY_BLEEDING_PCT : localeInfo.TOOLTIP_APPLY_BLEEDING_PCT,
			item.APPLY_STUN_PCT : localeInfo.TOOLTIP_APPLY_STUN_PCT,
			item.APPLY_SLOW_PCT : localeInfo.TOOLTIP_APPLY_SLOW_PCT,
			item.APPLY_CRITICAL_PCT : localeInfo.TOOLTIP_APPLY_CRITICAL_PCT,
			item.APPLY_PENETRATE_PCT : localeInfo.TOOLTIP_APPLY_PENETRATE_PCT,

			item.APPLY_ATTBONUS_WARRIOR : localeInfo.TOOLTIP_APPLY_ATTBONUS_WARRIOR,
			item.APPLY_ATTBONUS_ASSASSIN : localeInfo.TOOLTIP_APPLY_ATTBONUS_ASSASSIN,
			item.APPLY_ATTBONUS_SURA : localeInfo.TOOLTIP_APPLY_ATTBONUS_SURA,
			item.APPLY_ATTBONUS_SHAMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_SHAMAN,
			item.APPLY_ATTBONUS_MONSTER : localeInfo.TOOLTIP_APPLY_ATTBONUS_MONSTER,

			item.APPLY_ATTBONUS_HUMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_HUMAN,
			item.APPLY_ATTBONUS_ANIMAL : localeInfo.TOOLTIP_APPLY_ATTBONUS_ANIMAL,
			item.APPLY_ATTBONUS_ORC : localeInfo.TOOLTIP_APPLY_ATTBONUS_ORC,
			item.APPLY_ATTBONUS_MILGYO : localeInfo.TOOLTIP_APPLY_ATTBONUS_MILGYO,
			item.APPLY_ATTBONUS_UNDEAD : localeInfo.TOOLTIP_APPLY_ATTBONUS_UNDEAD,
			item.APPLY_ATTBONUS_DEVIL : localeInfo.TOOLTIP_APPLY_ATTBONUS_DEVIL,
			item.APPLY_STEAL_HP : localeInfo.TOOLTIP_APPLY_STEAL_HP,
			item.APPLY_STEAL_SP : localeInfo.TOOLTIP_APPLY_STEAL_SP,
			item.APPLY_MANA_BURN_PCT : localeInfo.TOOLTIP_APPLY_MANA_BURN_PCT,
			item.APPLY_DAMAGE_SP_RECOVER : localeInfo.TOOLTIP_APPLY_DAMAGE_SP_RECOVER,
			item.APPLY_BLOCK : localeInfo.TOOLTIP_APPLY_BLOCK,
			item.APPLY_DODGE : localeInfo.TOOLTIP_APPLY_DODGE,
			item.APPLY_RESIST_SWORD : localeInfo.TOOLTIP_APPLY_RESIST_SWORD,
			item.APPLY_RESIST_TWOHAND : localeInfo.TOOLTIP_APPLY_RESIST_TWOHAND,
			item.APPLY_RESIST_DAGGER : localeInfo.TOOLTIP_APPLY_RESIST_DAGGER,
			item.APPLY_RESIST_BELL : localeInfo.TOOLTIP_APPLY_RESIST_BELL,
			item.APPLY_RESIST_FAN : localeInfo.TOOLTIP_APPLY_RESIST_FAN,
			item.APPLY_RESIST_BOW : localeInfo.TOOLTIP_RESIST_BOW,
			item.APPLY_RESIST_FIRE : localeInfo.TOOLTIP_RESIST_FIRE,
			item.APPLY_RESIST_ELEC : localeInfo.TOOLTIP_RESIST_ELEC,
			item.APPLY_RESIST_MAGIC : localeInfo.TOOLTIP_RESIST_MAGIC,
			item.APPLY_RESIST_WIND : localeInfo.TOOLTIP_APPLY_RESIST_WIND,
			item.APPLY_REFLECT_MELEE : localeInfo.TOOLTIP_APPLY_REFLECT_MELEE,
			item.APPLY_REFLECT_CURSE : localeInfo.TOOLTIP_APPLY_REFLECT_CURSE,
			item.APPLY_POISON_REDUCE : localeInfo.TOOLTIP_APPLY_POISON_REDUCE,
			item.APPLY_BLEEDING_REDUCE : localeInfo.TOOLTIP_APPLY_BLEEDING_REDUCE,
			item.APPLY_KILL_SP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_SP_RECOVER,
			item.APPLY_EXP_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_EXP_DOUBLE_BONUS,
			item.APPLY_GOLD_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_GOLD_DOUBLE_BONUS,
			item.APPLY_ITEM_DROP_BONUS : localeInfo.TOOLTIP_APPLY_ITEM_DROP_BONUS,
			item.APPLY_POTION_BONUS : localeInfo.TOOLTIP_APPLY_POTION_BONUS,
			item.APPLY_KILL_HP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_HP_RECOVER,
			item.APPLY_IMMUNE_STUN : localeInfo.TOOLTIP_APPLY_IMMUNE_STUN,
			item.APPLY_IMMUNE_SLOW : localeInfo.TOOLTIP_APPLY_IMMUNE_SLOW,
			item.APPLY_IMMUNE_FALL : localeInfo.TOOLTIP_APPLY_IMMUNE_FALL,
			item.APPLY_BOW_DISTANCE : localeInfo.TOOLTIP_BOW_DISTANCE,
			item.APPLY_DEF_GRADE_BONUS : localeInfo.TOOLTIP_DEF_GRADE,
			item.APPLY_ATT_GRADE_BONUS : localeInfo.TOOLTIP_ATT_GRADE,
			item.APPLY_MAGIC_ATT_GRADE : localeInfo.TOOLTIP_MAGIC_ATT_GRADE,
			item.APPLY_MAGIC_DEF_GRADE : localeInfo.TOOLTIP_MAGIC_DEF_GRADE,
			item.APPLY_MAX_STAMINA : localeInfo.TOOLTIP_MAX_STAMINA,
			item.APPLY_MALL_ATTBONUS : localeInfo.TOOLTIP_MALL_ATTBONUS,
			item.APPLY_MALL_DEFBONUS : localeInfo.TOOLTIP_MALL_DEFBONUS,
			item.APPLY_MALL_EXPBONUS : localeInfo.TOOLTIP_MALL_EXPBONUS,
			item.APPLY_MALL_ITEMBONUS : localeInfo.TOOLTIP_MALL_ITEMBONUS,
			item.APPLY_MALL_GOLDBONUS : localeInfo.TOOLTIP_MALL_GOLDBONUS,
			item.APPLY_SKILL_DAMAGE_BONUS : localeInfo.TOOLTIP_SKILL_DAMAGE_BONUS,
			item.APPLY_NORMAL_HIT_DAMAGE_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DAMAGE_BONUS,
			item.APPLY_SKILL_DEFEND_BONUS : localeInfo.TOOLTIP_SKILL_DEFEND_BONUS,
			item.APPLY_NORMAL_HIT_DEFEND_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DEFEND_BONUS,
			item.APPLY_PC_BANG_EXP_BONUS : localeInfo.TOOLTIP_MALL_EXPBONUS_P_STATIC,
			item.APPLY_PC_BANG_DROP_BONUS : localeInfo.TOOLTIP_MALL_ITEMBONUS_P_STATIC,
			item.APPLY_RESIST_WARRIOR : localeInfo.TOOLTIP_APPLY_RESIST_WARRIOR,
			item.APPLY_RESIST_ASSASSIN : localeInfo.TOOLTIP_APPLY_RESIST_ASSASSIN,
			item.APPLY_RESIST_SURA : localeInfo.TOOLTIP_APPLY_RESIST_SURA,
			item.APPLY_RESIST_SHAMAN : localeInfo.TOOLTIP_APPLY_RESIST_SHAMAN,
			item.APPLY_MAX_HP_PCT : localeInfo.TOOLTIP_APPLY_MAX_HP_PCT,
			item.APPLY_MAX_SP_PCT : localeInfo.TOOLTIP_APPLY_MAX_SP_PCT,
			item.APPLY_ENERGY : localeInfo.TOOLTIP_ENERGY,
			item.APPLY_COSTUME_ATTR_BONUS : localeInfo.TOOLTIP_COSTUME_ATTR_BONUS,

			item.APPLY_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MAGIC_ATTBONUS_PER,
			item.APPLY_MELEE_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MELEE_MAGIC_ATTBONUS_PER,
			item.APPLY_RESIST_ICE : localeInfo.TOOLTIP_RESIST_ICE,
			item.APPLY_RESIST_EARTH : localeInfo.TOOLTIP_RESIST_EARTH,
			item.APPLY_RESIST_DARK : localeInfo.TOOLTIP_RESIST_DARK,
			item.APPLY_ANTI_CRITICAL_PCT : localeInfo.TOOLTIP_ANTI_CRITICAL_PCT,
			item.APPLY_ANTI_PENETRATE_PCT : localeInfo.TOOLTIP_ANTI_PENETRATE_PCT,

			item.APPLY_ATTBONUS_WOLFMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_WOLFMAN,
			item.APPLY_RESIST_WOLFMAN : localeInfo.TOOLTIP_APPLY_RESIST_WOLFMAN,
			item.APPLY_RESIST_CLAW : localeInfo.TOOLTIP_APPLY_RESIST_CLAW,
			#item.APPLY_ACCEDRAIN_RATE : localeInfo.TOOLTIP_APPLY_ACCEDRAIN_RATE,
		}
	else:
		AFFECT_DICT = {
			item.APPLY_MAX_HP : localeInfo.TOOLTIP_MAX_HP,
			item.APPLY_MAX_SP : localeInfo.TOOLTIP_MAX_SP,
			item.APPLY_CON : localeInfo.TOOLTIP_CON,
			item.APPLY_INT : localeInfo.TOOLTIP_INT,
			item.APPLY_STR : localeInfo.TOOLTIP_STR,
			item.APPLY_DEX : localeInfo.TOOLTIP_DEX,
			item.APPLY_ATT_SPEED : localeInfo.TOOLTIP_ATT_SPEED,
			item.APPLY_MOV_SPEED : localeInfo.TOOLTIP_MOV_SPEED,
			item.APPLY_CAST_SPEED : localeInfo.TOOLTIP_CAST_SPEED,
			item.APPLY_HP_REGEN : localeInfo.TOOLTIP_HP_REGEN,
			item.APPLY_SP_REGEN : localeInfo.TOOLTIP_SP_REGEN,
			item.APPLY_POISON_PCT : localeInfo.TOOLTIP_APPLY_POISON_PCT,
			item.APPLY_STUN_PCT : localeInfo.TOOLTIP_APPLY_STUN_PCT,
			item.APPLY_SLOW_PCT : localeInfo.TOOLTIP_APPLY_SLOW_PCT,
			item.APPLY_CRITICAL_PCT : localeInfo.TOOLTIP_APPLY_CRITICAL_PCT,
			item.APPLY_PENETRATE_PCT : localeInfo.TOOLTIP_APPLY_PENETRATE_PCT,

			item.APPLY_ATTBONUS_WARRIOR : localeInfo.TOOLTIP_APPLY_ATTBONUS_WARRIOR,
			item.APPLY_ATTBONUS_ASSASSIN : localeInfo.TOOLTIP_APPLY_ATTBONUS_ASSASSIN,
			item.APPLY_ATTBONUS_SURA : localeInfo.TOOLTIP_APPLY_ATTBONUS_SURA,
			item.APPLY_ATTBONUS_SHAMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_SHAMAN,
			item.APPLY_ATTBONUS_MONSTER : localeInfo.TOOLTIP_APPLY_ATTBONUS_MONSTER,

			item.APPLY_ATTBONUS_HUMAN : localeInfo.TOOLTIP_APPLY_ATTBONUS_HUMAN,
			item.APPLY_ATTBONUS_ANIMAL : localeInfo.TOOLTIP_APPLY_ATTBONUS_ANIMAL,
			item.APPLY_ATTBONUS_ORC : localeInfo.TOOLTIP_APPLY_ATTBONUS_ORC,
			item.APPLY_ATTBONUS_MILGYO : localeInfo.TOOLTIP_APPLY_ATTBONUS_MILGYO,
			item.APPLY_ATTBONUS_UNDEAD : localeInfo.TOOLTIP_APPLY_ATTBONUS_UNDEAD,
			item.APPLY_ATTBONUS_DEVIL : localeInfo.TOOLTIP_APPLY_ATTBONUS_DEVIL,
			item.APPLY_STEAL_HP : localeInfo.TOOLTIP_APPLY_STEAL_HP,
			item.APPLY_STEAL_SP : localeInfo.TOOLTIP_APPLY_STEAL_SP,
			item.APPLY_MANA_BURN_PCT : localeInfo.TOOLTIP_APPLY_MANA_BURN_PCT,
			item.APPLY_DAMAGE_SP_RECOVER : localeInfo.TOOLTIP_APPLY_DAMAGE_SP_RECOVER,
			item.APPLY_BLOCK : localeInfo.TOOLTIP_APPLY_BLOCK,
			item.APPLY_DODGE : localeInfo.TOOLTIP_APPLY_DODGE,
			item.APPLY_RESIST_SWORD : localeInfo.TOOLTIP_APPLY_RESIST_SWORD,
			item.APPLY_RESIST_TWOHAND : localeInfo.TOOLTIP_APPLY_RESIST_TWOHAND,
			item.APPLY_RESIST_DAGGER : localeInfo.TOOLTIP_APPLY_RESIST_DAGGER,
			item.APPLY_RESIST_BELL : localeInfo.TOOLTIP_APPLY_RESIST_BELL,
			item.APPLY_RESIST_FAN : localeInfo.TOOLTIP_APPLY_RESIST_FAN,
			item.APPLY_RESIST_BOW : localeInfo.TOOLTIP_RESIST_BOW,
			item.APPLY_RESIST_FIRE : localeInfo.TOOLTIP_RESIST_FIRE,
			item.APPLY_RESIST_ELEC : localeInfo.TOOLTIP_RESIST_ELEC,
			item.APPLY_RESIST_MAGIC : localeInfo.TOOLTIP_RESIST_MAGIC,
			item.APPLY_RESIST_WIND : localeInfo.TOOLTIP_APPLY_RESIST_WIND,
			item.APPLY_REFLECT_MELEE : localeInfo.TOOLTIP_APPLY_REFLECT_MELEE,
			item.APPLY_REFLECT_CURSE : localeInfo.TOOLTIP_APPLY_REFLECT_CURSE,
			item.APPLY_POISON_REDUCE : localeInfo.TOOLTIP_APPLY_POISON_REDUCE,
			item.APPLY_KILL_SP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_SP_RECOVER,
			item.APPLY_EXP_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_EXP_DOUBLE_BONUS,
			item.APPLY_GOLD_DOUBLE_BONUS : localeInfo.TOOLTIP_APPLY_GOLD_DOUBLE_BONUS,
			item.APPLY_ITEM_DROP_BONUS : localeInfo.TOOLTIP_APPLY_ITEM_DROP_BONUS,
			item.APPLY_POTION_BONUS : localeInfo.TOOLTIP_APPLY_POTION_BONUS,
			item.APPLY_KILL_HP_RECOVER : localeInfo.TOOLTIP_APPLY_KILL_HP_RECOVER,
			item.APPLY_IMMUNE_STUN : localeInfo.TOOLTIP_APPLY_IMMUNE_STUN,
			item.APPLY_IMMUNE_SLOW : localeInfo.TOOLTIP_APPLY_IMMUNE_SLOW,
			item.APPLY_IMMUNE_FALL : localeInfo.TOOLTIP_APPLY_IMMUNE_FALL,
			item.APPLY_BOW_DISTANCE : localeInfo.TOOLTIP_BOW_DISTANCE,
			item.APPLY_DEF_GRADE_BONUS : localeInfo.TOOLTIP_DEF_GRADE,
			item.APPLY_ATT_GRADE_BONUS : localeInfo.TOOLTIP_ATT_GRADE,
			item.APPLY_MAGIC_ATT_GRADE : localeInfo.TOOLTIP_MAGIC_ATT_GRADE,
			item.APPLY_MAGIC_DEF_GRADE : localeInfo.TOOLTIP_MAGIC_DEF_GRADE,
			item.APPLY_MAX_STAMINA : localeInfo.TOOLTIP_MAX_STAMINA,
			item.APPLY_MALL_ATTBONUS : localeInfo.TOOLTIP_MALL_ATTBONUS,
			item.APPLY_MALL_DEFBONUS : localeInfo.TOOLTIP_MALL_DEFBONUS,
			item.APPLY_MALL_EXPBONUS : localeInfo.TOOLTIP_MALL_EXPBONUS,
			item.APPLY_MALL_ITEMBONUS : localeInfo.TOOLTIP_MALL_ITEMBONUS,
			item.APPLY_MALL_GOLDBONUS : localeInfo.TOOLTIP_MALL_GOLDBONUS,
			item.APPLY_SKILL_DAMAGE_BONUS : localeInfo.TOOLTIP_SKILL_DAMAGE_BONUS,
			item.APPLY_NORMAL_HIT_DAMAGE_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DAMAGE_BONUS,
			item.APPLY_SKILL_DEFEND_BONUS : localeInfo.TOOLTIP_SKILL_DEFEND_BONUS,
			item.APPLY_NORMAL_HIT_DEFEND_BONUS : localeInfo.TOOLTIP_NORMAL_HIT_DEFEND_BONUS,
			item.APPLY_PC_BANG_EXP_BONUS : localeInfo.TOOLTIP_MALL_EXPBONUS_P_STATIC,
			item.APPLY_PC_BANG_DROP_BONUS : localeInfo.TOOLTIP_MALL_ITEMBONUS_P_STATIC,
			item.APPLY_RESIST_WARRIOR : localeInfo.TOOLTIP_APPLY_RESIST_WARRIOR,
			item.APPLY_RESIST_ASSASSIN : localeInfo.TOOLTIP_APPLY_RESIST_ASSASSIN,
			item.APPLY_RESIST_SURA : localeInfo.TOOLTIP_APPLY_RESIST_SURA,
			item.APPLY_RESIST_SHAMAN : localeInfo.TOOLTIP_APPLY_RESIST_SHAMAN,
			item.APPLY_MAX_HP_PCT : localeInfo.TOOLTIP_APPLY_MAX_HP_PCT,
			item.APPLY_MAX_SP_PCT : localeInfo.TOOLTIP_APPLY_MAX_SP_PCT,
			item.APPLY_ENERGY : localeInfo.TOOLTIP_ENERGY,
			item.APPLY_COSTUME_ATTR_BONUS : localeInfo.TOOLTIP_COSTUME_ATTR_BONUS,

			item.APPLY_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MAGIC_ATTBONUS_PER,
			item.APPLY_MELEE_MAGIC_ATTBONUS_PER : localeInfo.TOOLTIP_MELEE_MAGIC_ATTBONUS_PER,
			item.APPLY_RESIST_ICE : localeInfo.TOOLTIP_RESIST_ICE,
			item.APPLY_RESIST_EARTH : localeInfo.TOOLTIP_RESIST_EARTH,
			item.APPLY_RESIST_DARK : localeInfo.TOOLTIP_RESIST_DARK,
			item.APPLY_ANTI_CRITICAL_PCT : localeInfo.TOOLTIP_ANTI_CRITICAL_PCT,
			item.APPLY_ANTI_PENETRATE_PCT : localeInfo.TOOLTIP_ANTI_PENETRATE_PCT,
		}

	if app.ENABLE_MAGIC_REDUCTION:
		AFFECT_DICT[item.APPLY_RESIST_MAGIC_REDUCTION] = localeInfo.TOOLTIP_RESIST_MAGIC_REDUCTION

	# AFFECT_DICT[item.APPLY_ACCEDRAIN_RATE] = localeInfo.TOOLTIP_APPLY_ACCEDRAIN_RATE

	if app.ENABLE_ELEMENT_ADD:
		AFFECT_DICT[item.APPLY_ENCHANT_ELEC] = localeInfo.TOOLTIP_APPLY_ENCHANT_ELECT
		AFFECT_DICT[item.APPLY_ENCHANT_FIRE] = localeInfo.TOOLTIP_APPLY_ENCHANT_FIRE
		AFFECT_DICT[item.APPLY_ENCHANT_ICE] = localeInfo.TOOLTIP_APPLY_ENCHANT_ICE
		AFFECT_DICT[item.APPLY_ENCHANT_WIND] = localeInfo.TOOLTIP_APPLY_ENCHANT_WIND
		AFFECT_DICT[item.APPLY_ENCHANT_EARTH] = localeInfo.TOOLTIP_APPLY_ENCHANT_EARTH
		AFFECT_DICT[item.APPLY_ENCHANT_DARK] = localeInfo.TOOLTIP_APPLY_ENCHANT_DARK
		AFFECT_DICT[item.APPLY_ATTBONUS_CZ] = localeInfo.TOOLTIP_APPLY_ATTBONUS_CZ
		AFFECT_DICT[item.APPLY_ATTBONUS_INSECT] = localeInfo.TOOLTIP_APPLY_ATTBONUS_INSECT
		AFFECT_DICT[item.APPLY_ATTBONUS_DESERT] = localeInfo.TOOLTIP_APPLY_ATTBONUS_DESERT

		AFFECT_DICT[item.APPLY_RESIST_SWORD_REDUCTION] = localeInfo.TOOLTIP_APPLY_ATTBONUS_SWORD
		AFFECT_DICT[item.APPLY_RESIST_TWOHAND_REDUCTION]= localeInfo.TOOLTIP_APPLY_ATTBONUS_TWOHAND
		AFFECT_DICT[item.APPLY_RESIST_DAGGER_REDUCTION] = localeInfo.TOOLTIP_APPLY_ATTBONUS_DAGGER
		AFFECT_DICT[item.APPLY_RESIST_BELL_REDUCTION] = localeInfo.TOOLTIP_APPLY_ATTBONUS_BELL
		AFFECT_DICT[item.APPLY_RESIST_FAN_REDUCTION] = localeInfo.TOOLTIP_APPLY_ATTBONUS_FAN
		AFFECT_DICT[item.APPLY_RESIST_BOW_REDUCTION] = localeInfo.TOOLTIP_APPLY_ATTBONUS_BOW
		if not app.DISABLE_WOLFMAN_CREATION:
			AFFECT_DICT[item.APPLY_RESIST_CLAW_REDUCTION] = localeInfo.TOOLTIP_APPLY_ATTBONUS_CLAW
		AFFECT_DICT[item.APPLY_RESIST_HUMAN] = localeInfo.TOOLTIP_APPLY_RESIST_HUMAN

	AFFECT_DICT[item.APPLY_ATTBONUS_STONE] = localeInfo.TOOLTIP_ATTBONUS_STONE
	if app.ENABLE_CONQUEROR_LEVEL:
		AFFECT_DICT[item.APPLY_SUNGMA_STR] = localeInfo.TOOLTIP_SUNGMA_STR
		AFFECT_DICT[item.APPLY_SUNGMA_HP] = localeInfo.TOOLTIP_SUNGMA_HP
		AFFECT_DICT[item.APPLY_SUNGMA_MOVE] = localeInfo.TOOLTIP_SUNGMA_MOVE
		AFFECT_DICT[item.APPLY_SUNGMA_IMMUNE] = localeInfo.TOOLTIP_SUNGMA_IMMUNE

	ATTRIBUTE_NEED_WIDTH = {
		23 : 230,
		24 : 230,
		25 : 230,
		26 : 220,
		27 : 210,

		35 : 210,
		36 : 210,
		37 : 210,
		38 : 210,
		39 : 210,
		40 : 210,
		41 : 210,

		42 : 220,
		43 : 230,
		45 : 230,
	}

	if not app.DISABLE_WOLFMAN_CREATION:
		ANTI_FLAG_DICT = {
			0 : item.ITEM_ANTIFLAG_WARRIOR,
			1 : item.ITEM_ANTIFLAG_ASSASSIN,
			2 : item.ITEM_ANTIFLAG_SURA,
			3 : item.ITEM_ANTIFLAG_SHAMAN,
			4 : item.ITEM_ANTIFLAG_WOLFMAN,
		}
	else:
		ANTI_FLAG_DICT = {
			0 : item.ITEM_ANTIFLAG_WARRIOR,
			1 : item.ITEM_ANTIFLAG_ASSASSIN,
			2 : item.ITEM_ANTIFLAG_SURA,
			3 : item.ITEM_ANTIFLAG_SHAMAN,
		}
	FONT_COLOR = grp.GenerateColor(0.7607, 0.7607, 0.7607, 1.0)

	def __init__(self, *args, **kwargs):
		ToolTip.__init__(self, *args, **kwargs)
		self.itemVnum = 0
		self.metinSlot = []
		self.isShopItem = False

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			self.isPrivateSearchItem = False

		if app.ENABLE_DS_SET:
			self.interface = None

		# 아이템 툴팁을 표시할 때 현재 캐릭터가 착용할 수 없는 아이템이라면 강제로 Disable Color로 설정 (이미 그렇게 작동하고 있으나 꺼야 할 필요가 있어서)
		self.bCannotUseItemForceSetDisableColor = True 

	def __del__(self):
		ToolTip.__del__(self)
		self.metinSlot = None

	if app.ENABLE_DS_SET:
		def BindInterface(self, interface):
			from _weakref import proxy
			self.interface = proxy(interface)

	def SetCannotUseItemForceSetDisableColor(self, enable):
		self.bCannotUseItemForceSetDisableColor = enable

	def CanEquip(self):
		if not item.IsEquipmentVID(self.itemVnum):
			return True

		race = player.GetRace()
		job = chr.RaceToJob(race)
		if not self.ANTI_FLAG_DICT.has_key(job):
			return False

		if item.IsAntiFlag(self.ANTI_FLAG_DICT[job]):
			return False

		sex = chr.RaceToSex(race)

		MALE = 1
		FEMALE = 0

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE) and sex == MALE:
			return False

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE) and sex == FEMALE:
			return False

		for i in xrange(item.LIMIT_MAX_NUM):
			(limitType, limitValue) = item.GetLimit(i)

			if item.LIMIT_LEVEL == limitType:
				if player.GetStatus(player.LEVEL) < limitValue:
					return False
			"""
			elif item.LIMIT_STR == limitType:
				if player.GetStatus(player.ST) < limitValue:
					return False
			elif item.LIMIT_DEX == limitType:
				if player.GetStatus(player.DX) < limitValue:
					return False
			elif item.LIMIT_INT == limitType:
				if player.GetStatus(player.IQ) < limitValue:
					return False
			elif item.LIMIT_CON == limitType:
				if player.GetStatus(player.HT) < limitValue:
					return False
			"""

		return True

	def AppendTextLineAcce(self, text, color = FONT_COLOR, centerAlign = True):
		return ToolTip.AppendTextLine(self, text, color, centerAlign)

	def AppendTextLine(self, text, color = FONT_COLOR, centerAlign = True):
		if not self.CanEquip() and self.bCannotUseItemForceSetDisableColor:
			color = self.DISABLE_COLOR

		return ToolTip.AppendTextLine(self, text, color, centerAlign)

	def AppendTextLineDontCheckColor(self, text, color = FONT_COLOR, centerAlign = True):
		return ToolTip.AppendTextLine(self, text, color, centerAlign)

	def ClearToolTip(self):
		self.isShopItem = False
		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			self.isPrivateSearchItem = False
		self.toolTipWidth = self.TOOL_TIP_WIDTH
		ToolTip.ClearToolTip(self)

	def SetInventoryItem(self, slotIndex, window_type = player.INVENTORY):
		if app.ENABLE_DS_SET:
			self.window_type = window_type

		itemVnum = player.GetItemIndex(window_type, slotIndex)

		if 0 == itemVnum:
			return

		self.ClearToolTip()
		if shop.IsOpen():
			if not shop.IsPrivateShop():
				item.SelectItem(itemVnum)
				self.AppendSellingPrice(player.GetISellItemPrice(window_type, slotIndex))

		metinSlot = [player.GetItemMetinSocket(window_type, slotIndex, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
		attrSlot = [player.GetItemAttribute(window_type, slotIndex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
		apply_random_list = []
		if app.ENABLE_APPLY_RANDOM:
			apply_random_list = [player.GetItemApplyRandom(window_type, slotIndex, i) for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM)]

		self.AddItemData(itemVnum, metinSlot, attrSlot, None, 0, 0, window_type, slotIndex, None, apply_random_list, 0)

	def SetResulItemAttrMove(self, baseSlotIndex, materialSlotIndex, window_type = player.INVENTORY):
		baseItemVnum = player.GetItemIndex(window_type, baseSlotIndex)

		if 0 == baseItemVnum:
			return

		materialItemVnum = player.GetItemIndex(window_type, materialSlotIndex)

		if 0 == materialItemVnum:
			return

		self.ClearToolTip()

		metinSlot = [player.GetItemMetinSocket(window_type, baseSlotIndex, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
		attrSlot = [player.GetItemAttribute(window_type, materialSlotIndex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]

		self.AddItemData(baseItemVnum, metinSlot, attrSlot)

	if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
		def SetPrivateSearchItem(self, slotIndex):
			itemVnum = shop.GetPrivateShopSelectItemVnum(slotIndex)

			if 0 == itemVnum:
				return

			self.ClearToolTip()
			self.isPrivateSearchItem = True

			metinSlot = []
			for i in xrange(player.METIN_SOCKET_MAX_NUM):
				metinSlot.append(shop.GetPrivateShopSelectItemMetinSocket(slotIndex, i))
			attrSlot = []
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				attrSlot.append(shop.GetPrivateShopSelectItemAttribute(slotIndex, i))
			apply_random_list = []
			if app.ENABLE_APPLY_RANDOM:
				for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
					apply_random_list.append(shop.GetPrivateShopItemApplyRandom(slotIndex, i))

			self.AddItemData(itemVnum, metinSlot, attrSlot, None, 0, 0, player.INVENTORY, slotIndex, None, apply_random_list, 0)
			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				self.AppendChangeLookInfoPrivateShopWIndow(slotIndex)

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			def AppendChangeLookInfoPrivateShopWIndow(self, slotIndex):
				changelookvnum = shop.GetPrivateShopItemChangeLookVnum(slotIndex)
				if not changelookvnum == 0:
					self.AppendSpace(5)
					self.AppendTextLine("[ " + localeInfo.CHANGE_LOOK_TITLE + " ]", self.CHANGELOOK_TITLE_COLOR)
					itemName = item.GetItemNameByVnum(changelookvnum)

					if item.GetItemType() == item.ITEM_TYPE_COSTUME:
						if item.GetItemSubType() == item.COSTUME_TYPE_BODY:
							malefemale = ""
							if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
								malefemale = localeInfo.FOR_FEMALE

							if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
								malefemale = localeInfo.FOR_MALE
							itemName += " ( " + malefemale + " )"

					textLine = self.AppendTextLine(itemName, self.CHANGELOOK_ITEMNAME_COLOR, True)
					textLine.SetFeather()

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			def AppendChangeLookInfoShopWindow(self, slotIndex):
				changelookvnum = shop.GetItemChangeLookVnum(slotIndex)
				if not changelookvnum == 0:
					self.AppendSpace(5)
					self.AppendTextLine("[ " + localeInfo.CHANGE_LOOK_TITLE + " ]", self.CHANGELOOK_TITLE_COLOR)
					itemName = item.GetItemNameByVnum(changelookvnum)

					if item.GetItemType() == item.ITEM_TYPE_COSTUME:
						if item.GetItemSubType() == item.COSTUME_TYPE_BODY:
							malefemale = ""
							if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
								malefemale = localeInfo.FOR_FEMALE

							if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
								malefemale = localeInfo.FOR_MALE
							itemName += " ( " + malefemale + " )"

					textLine = self.AppendTextLine(itemName, self.CHANGELOOK_ITEMNAME_COLOR, True)
					textLine.SetFeather()

	if app.ENABLE_CHANGE_LOOK_SYSTEM:
		def AppendChangeLookInfoItemVnum(self, changelookvnum):
			if not changelookvnum == 0:
				self.AppendSpace(5)
				self.AppendTextLine("[ " + localeInfo.CHANGE_LOOK_TITLE + " ]", self.CHANGELOOK_TITLE_COLOR)
				itemName = item.GetItemNameByVnum(changelookvnum)

				if item.GetItemType() == item.ITEM_TYPE_COSTUME:
					if item.GetItemSubType() == item.COSTUME_TYPE_BODY:
						malefemale = ""
						if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
							malefemale = localeInfo.FOR_FEMALE

						if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
							malefemale = localeInfo.FOR_MALE
						itemName += " ( " + malefemale + " )"

				textLine = self.AppendTextLine(itemName, self.CHANGELOOK_ITEMNAME_COLOR, True)
				textLine.SetFeather()

	def SetShopItem(self, slotIndex):
		itemVnum = shop.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		price = shop.GetItemPrice(slotIndex)
		self.ClearToolTip()
		self.isShopItem = True

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetItemAttribute(slotIndex, i))
		apply_random_list = []
		if app.ENABLE_APPLY_RANDOM:
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				apply_random_list.append(shop.GetItemApplyRandom(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, None, 0, 0, player.INVENTORY, slotIndex, None, apply_random_list, 0)

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			self.AppendChangeLookInfoShopWindow(slotIndex)

		if app.ENABLE_CHEQUE_SYSTEM:
			cheque = shop.GetItemPriceCheque(slotIndex)
			self.AppendPrice(price, cheque)
		else:
			self.AppendPrice(price)

	def SetShopItemBySecondaryCoin(self, slotIndex):
		itemVnum = shop.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		price = shop.GetItemPrice(slotIndex)
		self.ClearToolTip()
		self.isShopItem = True

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(shop.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(shop.GetItemAttribute(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot)
		self.AppendPriceBySecondaryCoin(price)

	if app.ENABLE_SHOPEX_RENEWAL:
		def SetShopItemByShopEx(self, slotIndex, type):
			itemVnum = shop.GetItemID(slotIndex)
			PriceVnum = shop.GetItemPriceVnum(slotIndex)

			if 0 == itemVnum:
				return

			if PriceVnum == 0 and type == 3:
				return

			price = shop.GetItemPrice(slotIndex)
			self.ClearToolTip()
			self.isShopItem = True

			metinSlot = []
			for i in xrange(player.METIN_SOCKET_MAX_NUM):
				metinSlot.append(shop.GetItemMetinSocket(slotIndex, i))
			attrSlot = []
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				attrSlot.append(shop.GetItemAttribute(slotIndex, i))

			self.AddItemData(itemVnum, metinSlot, attrSlot)
			self.AppendSpace(5)

			if type == 3:
				self.AddShopEXItem(PriceVnum, price)
			else:
				self.AppendTextLineDontCheckColor(localeInfo.TOOLTIP_BUYPRICE % (localeInfo.NumberToShopEXP(price)), self.GetPriceColor(price))

		def AddShopEXItem(self, itemVnum, price):
			self.AppendTextLine(localeInfo.MONEY_INPUT_DIALOG_SELLPRICE)
			item.SelectItem(itemVnum)
			name = "%d. %s" % (price, item.GetItemName())
			self.AutoAppendTextLine(name, self.SPECIAL_TITLE_COLOR)

			itemImage = ui.ImageBox()
			itemImage.SetParent(self)
			itemImage.Show()
			itemImage.LoadImage(item.GetIconImageFileName())
			itemImage.SetPosition(self.toolTipWidth / 2 - 16, self.toolTipHeight + 2.5)

			self.toolTipHeight += itemImage.GetHeight() + 2.5
			self.childrenList.append(itemImage)
			self.ResizeToolTip()

	def SetExchangeOwnerItem(self, slotIndex):
		itemVnum = exchange.GetItemVnumFromSelf(slotIndex)

		if 0 == itemVnum:
			return

		self.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(exchange.GetItemMetinSocketFromSelf(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(exchange.GetItemAttributeFromSelf(slotIndex, i))
		apply_random_list = []
		if app.ENABLE_APPLY_RANDOM:
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				apply_random_list.append(exchange.GetItemApplyRandomFromSelf(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, None, 0, 0, player.INVENTORY, slotIndex, None, apply_random_list, 0)

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			self.AppendChangeLookInfoExchangeWIndow(0, slotIndex)

	if app.ENABLE_SEND_TARGET_INFO:
		def SetItemToolTipBook(self, itemVnum):
			item.SelectItem(itemVnum)

			self.ClearToolTip()

			self.SetTitle(item.GetItemName())
			self.AppendDescription(item.GetItemDescription(), 26)
			self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
			self.ShowToolTip()

	def SetExchangeTargetItem(self, slotIndex):
		itemVnum = exchange.GetItemVnumFromTarget(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(exchange.GetItemMetinSocketFromTarget(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(exchange.GetItemAttributeFromTarget(slotIndex, i))
		apply_random_list = []
		if app.ENABLE_APPLY_RANDOM:
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				apply_random_list.append(exchange.GetItemApplyRandomFromTarget(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, None, 0, 0, player.INVENTORY, slotIndex, None, apply_random_list, 0)

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			self.AppendChangeLookInfoExchangeWIndow(1, slotIndex)

	if app.ENABLE_CHANGE_LOOK_SYSTEM:
		def AppendChangeLookInfoExchangeWIndow(self, type, slotIndex):
			if type == 0:
				changelookvnum = exchange.GetChangeLookVnumFromSelf(slotIndex)
			elif type == 1:
				changelookvnum = exchange.GetChangeLookVnumFromTarget(slotIndex)
			if not changelookvnum == 0:
				self.AppendSpace(5)
				self.AppendTextLine("[ " + localeInfo.CHANGE_LOOK_TITLE + " ]", self.CHANGELOOK_TITLE_COLOR)
				itemName = item.GetItemNameByVnum(changelookvnum)

				if item.GetItemType() == item.ITEM_TYPE_COSTUME:
					if item.GetItemSubType() == item.COSTUME_TYPE_BODY:
						malefemale = ""
						if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
							malefemale = localeInfo.FOR_FEMALE

						if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
							malefemale = localeInfo.FOR_MALE
						itemName += " ( " + malefemale + " )"

				textLine = self.AppendTextLine(itemName, self.CHANGELOOK_ITEMNAME_COLOR, True)
				textLine.SetFeather()

	def SetPrivateShopBuilderItem(self, invenType, invenPos, privateShopSlotIndex):
		itemVnum = player.GetItemIndex(invenType, invenPos)
		if 0 == itemVnum:
			return

		item.SelectItem(itemVnum)
		self.ClearToolTip()

		if app.ENABLE_CHEQUE_SYSTEM:
			self.AppendSellingPrice(shop.GetPrivateShopItemPrice(invenType, invenPos), shop.GetPrivateShopItemPriceCheque(invenType, invenPos), True)
		else:
			self.AppendSellingPrice(shop.GetPrivateShopItemPrice(invenType, invenPos))

		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(invenPos, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(invenPos, i))
		apply_random_list = []
		if app.ENABLE_APPLY_RANDOM:
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				apply_random_list.append(player.GetItemApplyRandom(invenPos, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, None, 0, 0, invenType, invenPos, None, apply_random_list, 0)

	if app.ENABLE_CHANGE_LOOK_SYSTEM:
		def SetChangeLookWindowItem(self, slotIndex):
			inventoryslotindex = player.GetChangeLookItemInvenSlot(slotIndex)
			if inventoryslotindex == player.ITEM_SLOT_COUNT:
				return

			itemVnum = player.GetItemIndex(player.INVENTORY, inventoryslotindex)
			if 0 == itemVnum:
				return

			self.ClearToolTip()

			metinSlot = [player.GetItemMetinSocket(player.INVENTORY, inventoryslotindex, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [player.GetItemAttribute(player.INVENTORY, inventoryslotindex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			apply_random_list = []
			if app.ENABLE_APPLY_RANDOM:
				for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
					apply_random_list.append(player.GetItemApplyRandom(inventoryslotindex, i))

			self.AddItemData(itemVnum, metinSlot, attrSlot, None, 0, 0, player.INVENTORY, inventoryslotindex, None, apply_random_list, 0)

	def SetSafeBoxItem(self, slotIndex):
		itemVnum = safebox.GetItemID(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(safebox.GetItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(safebox.GetItemAttribute(slotIndex, i))
		apply_random_list = []
		if app.ENABLE_APPLY_RANDOM:
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				apply_random_list.append(safebox.GetItemApplyRandom(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, None, safebox.GetItemFlags(slotIndex), 0, player.SAFEBOX, slotIndex, None, apply_random_list, 0)

	def SetMallItem(self, slotIndex):
		itemVnum = safebox.GetMallItemID(slotIndex)
		if 0 == itemVnum:
			return

		self.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(safebox.GetMallItemMetinSocket(slotIndex, i))
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(safebox.GetMallItemAttribute(slotIndex, i))
		apply_random_list = []
		if app.ENABLE_APPLY_RANDOM:
			for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
				apply_random_list.append(safebox.GetMallItemApplyRandom(slotIndex, i))

		self.AddItemData(itemVnum, metinSlot, attrSlot, None, 0, 0, player.MALL, slotIndex, None, apply_random_list, 0)

	def SetItemToolTip(self, itemVnum):
		self.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(0)
		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append((0, 0))

		self.AddItemData(itemVnum, metinSlot, attrSlot)

	if app.ENABLE_MAILBOX:
		def SetMailBoxItem(self, index):
			item_data = mail.GetMailItemData(index)
			if None == item_data:
				return

			(vnum, count) = item_data

			self.ClearToolTip()
			metinSlot = []
			for i in xrange(player.METIN_SOCKET_MAX_NUM):
				metinSlot.append(mail.GetMailItemMetinSocket(index, i))
			attrSlot = []
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				attrSlot.append(mail.GetMailItemAttribute(index, i))
			apply_random_list = []
			if app.ENABLE_APPLY_RANDOM:
				for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
					apply_random_list.append(mail.GetItemApplyRandom(index, i))

			self.AddItemData(vnum, metinSlot, attrSlot, None, 0, 0, player.INVENTORY, index, None, apply_random_list, 0)

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				changelookvnum = mail.GetItemChangeLookVnum(index)
				self.AppendChangeLookInfoItemVnum(changelookvnum)

	def __AppendAttackSpeedInfo(self, item):
		atkSpd = item.GetValue(0)

		if atkSpd < 80:
			stSpd = localeInfo.TOOLTIP_ITEM_VERY_FAST
		elif atkSpd <= 95:
			stSpd = localeInfo.TOOLTIP_ITEM_FAST
		elif atkSpd <= 105:
			stSpd = localeInfo.TOOLTIP_ITEM_NORMAL
		elif atkSpd <= 120:
			stSpd = localeInfo.TOOLTIP_ITEM_SLOW
		else:
			stSpd = localeInfo.TOOLTIP_ITEM_VERY_SLOW

		self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_SPEED % stSpd, self.NORMAL_COLOR)

	def __AppendAttackGradeInfo(self):
		atkGrade = item.GetValue(1)
		self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_GRADE % atkGrade, self.GetChangeTextLineColor(atkGrade))

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def CalcAcceValue(self, value, abs):
			if not value:
				return 0

			valueCalc = int((round(value * abs) / 100) - .5) + int(int((round(value * abs) / 100) - .5) > 0)
			if valueCalc <= 0 and value > 0:
				value = 1
			else:
				value = valueCalc

			return value

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def __AppendAttackPowerInfo(self, itemAbsChance = 0):
			minPower = item.GetValue(3)
			maxPower = item.GetValue(4)
			addPower = item.GetValue(5)

			if itemAbsChance:
				minPower = self.CalcAcceValue(minPower, itemAbsChance)
				maxPower = self.CalcAcceValue(maxPower, itemAbsChance)
				addPower = self.CalcAcceValue(addPower, itemAbsChance)

			if maxPower > minPower:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_POWER % (minPower+addPower, maxPower+addPower), self.POSITIVE_COLOR)
			else:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_POWER_ONE_ARG % (minPower+addPower), self.POSITIVE_COLOR)
	else:
		def __AppendAttackPowerInfo(self):
			minPower = item.GetValue(3)
			maxPower = item.GetValue(4)
			addPower = item.GetValue(5)
			if maxPower > minPower:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_POWER % (minPower+addPower, maxPower+addPower), self.POSITIVE_COLOR)
			else:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_ATT_POWER_ONE_ARG % (minPower+addPower), self.POSITIVE_COLOR)

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def __AppendMagicAttackInfo(self, itemAbsChance = 0):
			minMagicAttackPower = item.GetValue(1)
			maxMagicAttackPower = item.GetValue(2)
			addPower = item.GetValue(5)

			if itemAbsChance:
				minMagicAttackPower = self.CalcAcceValue(minMagicAttackPower, itemAbsChance)
				maxMagicAttackPower = self.CalcAcceValue(maxMagicAttackPower, itemAbsChance)
				addPower = self.CalcAcceValue(addPower, itemAbsChance)

			if minMagicAttackPower > 0 or maxMagicAttackPower > 0:
				if maxMagicAttackPower > minMagicAttackPower:
					self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_ATT_POWER % (minMagicAttackPower + addPower, maxMagicAttackPower + addPower), self.POSITIVE_COLOR)
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_ATT_POWER_ONE_ARG % (minMagicAttackPower + addPower), self.POSITIVE_COLOR)
	else:
		def __AppendMagicAttackInfo(self):
			minMagicAttackPower = item.GetValue(1)
			maxMagicAttackPower = item.GetValue(2)
			addPower = item.GetValue(5)

			if minMagicAttackPower > 0 or maxMagicAttackPower > 0:
				if maxMagicAttackPower > minMagicAttackPower:
					self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_ATT_POWER % (minMagicAttackPower+addPower, maxMagicAttackPower+addPower), self.POSITIVE_COLOR)
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_ATT_POWER_ONE_ARG % (minMagicAttackPower+addPower), self.POSITIVE_COLOR)

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def __AppendMagicDefenceInfo(self, itemAbsChance = 0):
			magicDefencePower = item.GetValue(0)

			if itemAbsChance:
				magicDefencePower = self.CalcAcceValue(magicDefencePower, itemAbsChance)

			if magicDefencePower > 0:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_DEF_POWER % magicDefencePower, self.GetChangeTextLineColor(magicDefencePower))
	else:
		def __AppendMagicDefenceInfo(self):
			magicDefencePower = item.GetValue(0)

			if magicDefencePower > 0:
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_MAGIC_DEF_POWER % magicDefencePower, self.GetChangeTextLineColor(magicDefencePower))

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def __AppendAttributeInformation(self, attrSlot, itemAbsChance = 0):
			if 0 != attrSlot:
				for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
					type = attrSlot[i][0]
					value = attrSlot[i][1]

					if 0 == value:
						continue

					affectString = self.__GetAffectString(type, value)

					if item.GetItemType() == item.ITEM_TYPE_COSTUME and item.GetItemSubType() == item.COSTUME_TYPE_ACCE and itemAbsChance:
						value = self.CalcAcceValue(value, itemAbsChance)
						affectString = self.__GetAffectString(type, value)

					if affectString:
						affectColor = self.__GetAttributeColor(i, value)
						self.AppendTextLine(affectString, affectColor)
	else:
		def __AppendAttributeInformation(self, attrSlot):
			if 0 != attrSlot:
				for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
					type = attrSlot[i][0]
					value = attrSlot[i][1]

					if 0 == value:
						continue

					affectString = self.__GetAffectString(type, value)
					if affectString:
						affectColor = self.__GetAttributeColor(i, value)
						self.AppendTextLine(affectString, affectColor)

	if app.ENABLE_67TH_BONUS:
		def __AppendRareAttributeInformation(self, attrSlot):
			if 0 != attrSlot:
				attrCount = 0
				for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
					if 0 == attrSlot[i][1]:
						continue

					attrCount += 1

				if attrCount < player.ATTRIBUTE_SLOT_MAX_NUM and attrCount >= (player.ATTRIBUTE_SLOT_MAX_NUM - 2):
					self.AppendTextLine(localeInfo.ATTR_6TH_7TH_POSSIBILITY, self.TEXTLINE_2ND_COLOR_DEFAULT)

	if app.ENABLE_APPLY_RANDOM:
		def __AppendDefaultItemApplyInformation(self, apply_random_list):
			if apply_random_list != None:
				for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
					type = apply_random_list[i][0]
					value = apply_random_list[i][1]

					if 0 == value:
						continue

					affectString = self.__GetAffectString(type, value)
					if affectString:
						self.AppendTextLine(affectString, self.APPLY_RANDOM_TEXT_COLOR)

		if app.ENABLE_ACCE_COSTUME_SYSTEM:
			def __AppendAcceDefaultItemApplyInformation(self, apply_random_list, socketPercentage):
				if apply_random_list != None:
					for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM):
						type = apply_random_list[i][0]
						value = apply_random_list[i][1]

						if 0 == value:
							continue

						affectValue = self.CalcAcceValue(value, socketPercentage)
						affectString = self.__GetAffectString(type, affectValue)
						if affectString:
							self.AppendTextLine(affectString, self.APPLY_RANDOM_TEXT_COLOR)

	if app.ENABLE_DS_SET:
		def __AppendDragonSoulAttributeInformation(self, attrSlot, dsType = 0, grade = 0):
			if 0 != attrSlot:
				if grade != 0:
					setWeightValue = item.GetDSSetWeight(dsType, grade)
					basicApplyCount = item.GetDSBasicApplyCount(dsType, grade)

					for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
						type = attrSlot[i][0]
						value = attrSlot[i][1]

						if 0 == value:
							continue

						affectString = self.__GetAffectString(type, value)
						if affectString:
							affectColor = self.__GetAttributeColor(i, value)

							setValue = 0
							if i < basicApplyCount:
								setValue = item.GetDSBasicApplyValue(dsType, type)
							else:
								setValue = item.GetDSAdditionalApplyValue(dsType, type)

							if setValue != 0:
								setValue = (setValue * setWeightValue - 1) / 100 + 1
								if affectString.find('%') == -1:
									self.AppendTwoColorTextLine(affectString, affectColor, " (+%d)" % (setValue))
								else:
									self.AppendTwoColorTextLine(affectString, affectColor, " (+%d%%)" % (setValue))
							else:
								self.AppendTextLine(affectString, affectColor)
				else:
					for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
						type = attrSlot[i][0]
						value = attrSlot[i][1]

						if 0 == value:
							continue

						affectString = self.__GetAffectString(type, value)
						if affectString:
							affectColor = self.__GetAttributeColor(i, value)
							self.AppendTextLine(affectString, affectColor)

	def __GetAttributeColor(self, index, value):
		if value > 0:
			if index >= 5:
				return self.SPECIAL_POSITIVE_COLOR2
			else:
				return self.SPECIAL_POSITIVE_COLOR
		elif value == 0:
			return self.NORMAL_COLOR
		else:
			return self.NEGATIVE_COLOR

	def __IsPolymorphItem(self, itemVnum):
		item.SelectItem(itemVnum)
		if item.GetItemType() == item.ITEM_TYPE_POLYMORPH:
			return 1
		return 0

	def __SetPolymorphItemTitle(self, monsterVnum):
		if localeInfo.IsVIETNAM():
			itemName =item.GetItemName()
			itemName+=" "
			itemName+=nonplayer.GetMonsterName(monsterVnum)
		else:
			itemName =nonplayer.GetMonsterName(monsterVnum)
			itemName+=" "
			itemName+=item.GetItemName()
		self.SetTitle(itemName)

	def __SetNormalItemTitle(self):
		self.SetTitle(item.GetItemName())

	def __SetSpecialItemTitle(self):
		self.AppendTextLine(item.GetItemName(), self.SPECIAL_TITLE_COLOR)

	def __SetItemTitle(self, itemVnum, metinSlot, attrSlot):
		if localeInfo.IsCANADA():
			if 72726 == itemVnum or 72730 == itemVnum:
				self.AppendTextLine(item.GetItemName(), grp.GenerateColor(1.0, 0.7843, 0.0, 1.0))
				return

		if self.__IsPolymorphItem(itemVnum):
			self.__SetPolymorphItemTitle(metinSlot[0])
		else:
			if self.__IsAttr(attrSlot):
				self.__SetSpecialItemTitle()
				return

			self.__SetNormalItemTitle()

	def __IsAttr(self, attrSlot):
		if not attrSlot:
			return False

		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			type = attrSlot[i][0]
			if 0 != type:
				return True

		return False

	def AddRefineItemData(self, item_vnum, sockets, attributes, rare_attr_slot = None, flags = 0, refine_element = None, apply_random_list = None, set_value = 0):
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlotData = sockets[i]
			if self.GetMetinItemIndex(metinSlotData) == constInfo.ERROR_METIN_STONE:
				sockets[i] = player.METIN_SOCKET_TYPE_SILVER

		self.AddItemData(item_vnum, sockets, attributes, rare_attr_slot, 0, 0, player.INVENTORY, -1, refine_element, apply_random_list, set_value)

	def AddItemData_Offline(self, itemVnum, itemDesc, itemSummary, metinSlot, attrSlot):
		self.__AdjustMaxWidth(attrSlot, itemDesc)
		self.__SetItemTitle(itemVnum, metinSlot, attrSlot)

		if self.__IsHair(itemVnum):
			self.__AppendHairIcon(itemVnum)

		### Description ###
		self.AppendDescription(itemDesc, 26)
		self.AppendDescription(itemSummary, 26, self.CONDITION_COLOR)

	if app.ENABLE_SOUL_SYSTEM:
		def __SoulItemToolTip(self, itemVnum, metinSlot, flags):
			self.itemVnum = itemVnum
			item.SelectItem(itemVnum)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()

			(limit_Type, limit_Value) = item.GetLimit(1)
			max_time = limit_Value

			if 0 != flags:
				metinSlot[2] = item.GetValue(2)

			data = metinSlot[2]
			keep_time = data / 10000
			remain_count = data % 10000

			## value3,4,5
			value_index = 2 + keep_time / 60
			if value_index < 3:
				value_index = 3
			if value_index > 5:
				value_index = 5
			damage_value = float(item.GetValue(value_index) / 10.0)

			self.ClearToolTip()
			## board size
			soul_desc = ""
			if item.RED_SOUL == itemSubType:
				soul_desc = localeInfo.SOUL_ITEM_TOOLTIP_RED1 % (keep_time, max_time)
				soul_desc += localeInfo.SOUL_ITEM_TOOLTIP_RED2 % (damage_value)
			elif item.BLUE_SOUL == itemSubType:
				soul_desc = localeInfo.SOUL_ITEM_TOOLTIP_BLUE1 % (keep_time, max_time)
				soul_desc += localeInfo.SOUL_ITEM_TOOLTIP_BLUE2 % (damage_value)

			self.__AdjustMaxWidth(0, soul_desc)

			## 아이템 이름
			self.__SetNormalItemTitle()

			## desc
			board_width = self.__AdjustDescMaxWidth(soul_desc)
			if keep_time < 60:
				desc_color = 0xfff15f5f # RGB(241,95,95)
			else:
				desc_color = 0xff86e57f # RGB(134,229,127)

			self.AppendDescription(soul_desc, 26, desc_color)

			## PVP,변신,마운트 미적용
			self.AppendDescription(localeInfo.SOUL_ITEM_TOOLTIP_COMMON, 26, 0xfff15f5f) # RGB(241,95,95)
			self.AppendSpace(10)

			## 효과 적용/미적용
			if metinSlot[1] and keep_time >= 60 and remain_count > 0:
				self.AppendTextLine(localeInfo.SOUL_ITEM_TOOLTIP_APPLY, 0xff86e57f ) # RGB(134,229,127)
			else:
				self.AppendTextLine(localeInfo.SOUL_ITEM_TOOLTIP_UNAPPLIED, 0xfff15f5f ) # RGB(241,95,95)
			self.AppendSpace(5)

			## 남은 횟수 
			self.AppendTextLine(localeInfo.SOUL_ITEM_TOOLTIP_REMAIN_COUNT % remain_count)
			self.AppendSpace(5)

			## 남은 시간
			if 0 == flags:
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)
					if item.LIMIT_REAL_TIME == limitType:
						self.AppendSoulItemLastTime(metinSlot[0])
			else:
				(limitType, limitValue) = item.GetLimit(0)
				self.AppendSoulItemLastTime(limitValue + app.GetGlobalTimeStamp())

			self.ShowToolTip()

		def AppendSoulItemLastTime(self, endTime):
			leftSec = max(0, endTime - app.GetGlobalTimeStamp())
			if leftSec > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.SOUL_ITEM_TOOLTIP_REMOVE_TIME + " : " + localeInfo.SecondToDHM(leftSec), self.NORMAL_COLOR)

	def AddItemData(self, itemVnum, metinSlot, attrSlot = 0, rare_attr_slot = None, flags = 0, unbindTime = 0, window_type = player.INVENTORY, slotIndex = -1, refine_element = None, apply_random_list = None, set_value = 0):
		self.itemVnum = itemVnum
		self.metinSlot = metinSlot

		item.SelectItem(itemVnum)
		itemType = item.GetItemType()
		itemSubType = item.GetItemSubType()

		if not item.GetItemDescription():
			self.__CalculateToolTipWidth()

		if 50026 == itemVnum:
			if 0 != metinSlot:
				name = item.GetItemName()
				if metinSlot[0] > 0:
					name += " "
					name += localeInfo.NumberToMoneyString(metinSlot[0])
				self.SetTitle(name)
				if app.ENABLE_SOUL_BIND_SYSTEM:
					self.__AppendSealInformation(window_type, slotIndex) ## cyh itemseal 2013 11 11
				self.ShowToolTip()
			return

		if app.ENABLE_SEND_TARGET_INFO:
			if itemType == item.ITEM_TYPE_SKILLBOOK or itemType == item.ITEM_TYPE_SKILLFORGET:
				if metinSlot != 0 and metinSlot[0] == 0:
					self.SetItemToolTipBook(itemVnum)
					return

		### Skill Book ###
		if 50300 == itemVnum:
			if 0 != metinSlot:
				self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILLBOOK_NAME, 1)
				if app.ENABLE_SOUL_BIND_SYSTEM:
					self.__AppendSealInformation(window_type, slotIndex) ## cyh itemseal 2013 11 11
				self.ShowToolTip()
			return
		elif 70037 == itemVnum:
			if 0 != metinSlot:
				self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
				self.AppendDescription(item.GetItemDescription(), 26)
				self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
				if app.ENABLE_SOUL_BIND_SYSTEM:
					self.__AppendSealInformation(window_type, slotIndex) ## cyh itemseal 2013 11 11
				self.ShowToolTip()
			return
		elif 70055 == itemVnum:
			if 0 != metinSlot:
				self.__SetSkillBookToolTip(metinSlot[0], localeInfo.TOOLTIP_SKILL_FORGET_BOOK_NAME, 0)
				self.AppendDescription(item.GetItemDescription(), 26)
				self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
				if app.ENABLE_SOUL_BIND_SYSTEM:
					self.__AppendSealInformation(window_type, slotIndex) ## cyh itemseal 2013 11 11
				self.ShowToolTip()
			return
		###########################################################################################

		if app.ENABLE_SOUL_SYSTEM:
			if item.ITEM_TYPE_SOUL == itemType:
				self.__SoulItemToolTip(itemVnum, metinSlot, flags)
				return

		if slotIndex == -1:
			itemDesc = item.GetItemDescription()
		else:
			itemDesc = item.GetItemDescription(slotIndex, window_type)

		itemSummary = item.GetItemSummary()

		isCostumeItem = 0
		isCostumeHair = 0
		isCostumeBody = 0
		isCostumeMount = 0
		isCostumeAcce = 0
		isCostumeWeapon = 0

		if item.ITEM_TYPE_COSTUME == itemType:
			isCostumeItem = 1
			isCostumeHair = item.COSTUME_TYPE_HAIR == itemSubType
			isCostumeBody = item.COSTUME_TYPE_BODY == itemSubType
			if app.ENABLE_MOUNT_COSTUME_SYSTEM:
				isCostumeMount = item.COSTUME_TYPE_MOUNT == itemSubType
			if app.ENABLE_ACCE_COSTUME_SYSTEM:
				isCostumeAcce = item.COSTUME_TYPE_ACCE == itemSubType
			if app.ENABLE_WEAPON_COSTUME_SYSTEM:
				isCostumeWeapon = item.COSTUME_TYPE_WEAPON == itemSubType

		self.__AdjustMaxWidth(attrSlot, itemDesc)
		self.__SetItemTitle(itemVnum, metinSlot, attrSlot)

		### Hair Preview Image ###
		if self.__IsHair(itemVnum):
			self.__AppendHairIcon(itemVnum)

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			if self.isPrivateSearchItem:
				if not self.__IsHair(itemVnum):
					self.__AppendPrivateSearchItemIcon(itemVnum)

		### Description ###
		self.AppendDescription(itemDesc, 26)
		self.AppendDescription(itemSummary, 26, self.CONDITION_COLOR)

		### Weapon ###
		if item.ITEM_TYPE_WEAPON == itemType:
			self.__AppendLimitInformation()

			self.AppendSpace(5)

			## 부채일 경우 마공을 먼저 표시한다.
			if item.WEAPON_FAN == itemSubType:
				self.__AppendMagicAttackInfo()
				self.__AppendAttackPowerInfo()
			else:
				self.__AppendAttackPowerInfo()
				self.__AppendMagicAttackInfo()

			self.__AppendAffectInformation()
			if app.ENABLE_APPLY_RANDOM:
				self.__AppendDefaultItemApplyInformation(apply_random_list)
			self.__AppendAttributeInformation(attrSlot)
			if app.ENABLE_67TH_BONUS:
				self.__AppendRareAttributeInformation(attrSlot)

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				self.AppendChangeLookInformation(window_type, slotIndex)

			self.AppendWearableInformation()

			bHasRealtimeFlag = 0
			if app.ENABLE_QUIVER_SYSTEM:
				if item.WEAPON_QUIVER == itemSubType:
					if not self.isShopItem:
						## 사용가능 시간 제한이 있는지 찾아보고
						for i in xrange(item.LIMIT_MAX_NUM):
							(limitType, limitValue) = item.GetLimit(i)

							if item.LIMIT_REAL_TIME == limitType:
								bHasRealtimeFlag = 1

						## 있다면 관련 정보를 표시함. ex) 남은 시간 : 6일 6시간 58분 
						if 1 == bHasRealtimeFlag:
							self.AppendMallItemLastTime(metinSlot[0])
				else:
					for i in xrange(item.LIMIT_MAX_NUM):
						(limitType, limitValue) = item.GetLimit(i)

						if item.LIMIT_REAL_TIME == limitType:
							bHasRealtimeFlag = 1

					if 1 != bHasRealtimeFlag:
						self.__AppendMetinSlotInfo(metinSlot)
			else:
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1

				if 1 != bHasRealtimeFlag:
					self.__AppendMetinSlotInfo(metinSlot)

			if app.WJ_NEW_USER_CARE:
				if item.WEAPON_QUIVER != itemSubType:
					self.__AppendRealTimeToolTip(itemVnum, metinSlot[0])

		### Armor ###
		elif item.ITEM_TYPE_ARMOR == itemType:
			self.__AppendLimitInformation()

			## 방어력
			defGrade = item.GetValue(1)
			defBonus = item.GetValue(5)*2 ## 방어력 표시 잘못 되는 문제를 수정
			if defGrade > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade+defBonus), self.GetChangeTextLineColor(defGrade))

			self.__AppendMagicDefenceInfo()
			self.__AppendAffectInformation()
			if app.ENABLE_APPLY_RANDOM:
				self.__AppendDefaultItemApplyInformation(apply_random_list)
			self.__AppendAttributeInformation(attrSlot)
			if app.ENABLE_67TH_BONUS:
				if itemSubType != item.ARMOR_GLOVE:
					self.__AppendRareAttributeInformation(attrSlot)

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				self.AppendChangeLookInformation(window_type, slotIndex)

			self.AppendWearableInformation()

			if itemSubType in (item.ARMOR_WRIST, item.ARMOR_NECK, item.ARMOR_EAR):
				self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_ACCESSORY_MATERIAL_VNUM(itemVnum, itemSubType))
			else:
				self.__AppendMetinSlotInfo(metinSlot)

		### Ring Slot Item (Not UNIQUE) ###
		elif item.ITEM_TYPE_RING == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			if app.ENABLE_APPLY_RANDOM:
				self.__AppendDefaultItemApplyInformation(apply_random_list)
			self.__AppendAttributeInformation(attrSlot)

			#반지 소켓 시스템 관련해선 아직 기획 미정
			#self.__AppendAccessoryMetinSlotInfo(metinSlot, 99001)

		### Belt Item ###
		elif item.ITEM_TYPE_BELT == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			if app.ENABLE_APPLY_RANDOM:
				self.__AppendDefaultItemApplyInformation(apply_random_list)
			self.__AppendAttributeInformation(attrSlot)
			if app.ENABLE_67TH_BONUS:
				self.__AppendRareAttributeInformation(attrSlot)

			if osfInfo.SHOW_BELT_SLOTS_TOOLTIP == True:
				self.AppendTextLine("%s: %s " % (localeInfo.TOOLTIP_ITEM_SLOTS, BELT_SLOTS[item.GetValue(0)]), self.SPECIAL_POSITIVE_COLOR)

			self.__AppendAccessoryMetinSlotInfo(metinSlot, constInfo.GET_BELT_MATERIAL_VNUM(itemVnum))

		## 코스츔 아이템 ##
		elif 0 != isCostumeItem:
			self.__AppendLimitInformation()

			if app.ENABLE_ACCE_COSTUME_SYSTEM:
				if item.GetItemSubType() == item.COSTUME_TYPE_ACCE:
					## ABSORPTION RATE
					absChance = int(metinSlot[acce.ABSORPTION_SOCKET])
					self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ACCEDRAIN_RATE(absChance), self.CONDITION_COLOR)
					## END ABSOPRTION RATE

					itemAbsorbedVnum = int(metinSlot[acce.ABSORBED_SOCKET])
					if itemAbsorbedVnum:
						## ATTACK / DEFENCE
						item.SelectItem(itemAbsorbedVnum)
						if item.GetItemType() == item.ITEM_TYPE_WEAPON:
							if item.GetItemSubType() == item.WEAPON_FAN:
								self.__AppendMagicAttackInfo(metinSlot[acce.ABSORPTION_SOCKET])
								item.SelectItem(itemAbsorbedVnum)
								self.__AppendAttackPowerInfo(metinSlot[acce.ABSORPTION_SOCKET])
							else:
								self.__AppendAttackPowerInfo(metinSlot[acce.ABSORPTION_SOCKET])
								item.SelectItem(itemAbsorbedVnum)
								self.__AppendMagicAttackInfo(metinSlot[acce.ABSORPTION_SOCKET])
						elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
							defGrade = item.GetValue(1)
							defBonus = item.GetValue(5) * 2
							defGrade = self.CalcAcceValue(defGrade, metinSlot[acce.ABSORPTION_SOCKET])
							defBonus = self.CalcAcceValue(defBonus, metinSlot[acce.ABSORPTION_SOCKET])

							if defGrade > 0:
								self.AppendSpace(5)
								self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))

							item.SelectItem(itemAbsorbedVnum)
							self.__AppendMagicDefenceInfo(metinSlot[acce.ABSORPTION_SOCKET])
						## END ATTACK / DEFENCE

						## EFFECT
						item.SelectItem(itemAbsorbedVnum)
						for i in xrange(item.ITEM_APPLY_MAX_NUM):
							(affectType, affectValue) = item.GetAffect(i)
							affectValue = self.CalcAcceValue(affectValue, metinSlot[acce.ABSORPTION_SOCKET])
							affectString = self.__GetAffectString(affectType, affectValue)
							if affectString and affectValue > 0:
								self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))

							item.SelectItem(itemAbsorbedVnum)
						# END EFFECT

						item.SelectItem(itemVnum)
						if app.ENABLE_APPLY_RANDOM:
							self.__AppendAcceDefaultItemApplyInformation(apply_random_list, metinSlot[acce.ABSORPTION_SOCKET])
						self.__AppendAttributeInformation(attrSlot, metinSlot[acce.ABSORPTION_SOCKET])
					else:
						self.__AppendAttributeInformation(attrSlot)
				else:
					self.__AppendAffectInformation()
					self.__AppendAttributeInformation(attrSlot)
			else:
				self.__AppendAffectInformation()
				self.__AppendAttributeInformation(attrSlot)

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				if item.GetItemSubType() == item.COSTUME_TYPE_WEAPON or item.GetItemSubType() == item.COSTUME_TYPE_BODY:
					self.AppendChangeLookInformation(window_type, slotIndex)

				if item.GetItemSubType() == item.COSTUME_TYPE_MOUNT:
					self.AppendChangeLookInformation(window_type, slotIndex)

			self.AppendWearableInformation()

			bHasRealtimeFlag = 0
			bHasRealtimeFirstFlag = 0

			if app.ENABLE_ACCE_COSTUME_SYSTEM:
				if item.GetItemSubType() != item.COSTUME_TYPE_ACCE:
					## 사용가능 시간 제한이 있는지 찾아보고
					for i in xrange(item.LIMIT_MAX_NUM):
						(limitType, limitValue) = item.GetLimit(i)

						if item.LIMIT_REAL_TIME == limitType:
							bHasRealtimeFlag = 1

						if item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
							bHasRealtimeFirstFlag = 1

					if app.ENABLE_MOUNT_COSTUME_SYSTEM:
						if item.GetItemSubType() == item.COSTUME_TYPE_MOUNT:
							if not bHasRealtimeFirstFlag:
								if 1 == bHasRealtimeFlag:
									if app.ENABLE_COSTUME_EXTENDED_RECHARGE:
										self.AppendMallItemLastTimeEx(metinSlot[0])
									else:
										self.AppendMallItemLastTime(metinSlot[0])
								else:
									time = metinSlot[player.METIN_SOCKET_MAX_NUM-1]

									if 1 == item.GetValue(2): ## 실시간 이용 Flag / 장착 안해도 준다
										self.AppendMallItemLastTime(time)
									else:
										self.AppendUniqueItemLastTime(time)
						else:
							if 1 == bHasRealtimeFlag:
								if app.ENABLE_COSTUME_EXTENDED_RECHARGE:
									self.AppendMallItemLastTimeEx(metinSlot[0])
								else:
									self.AppendMallItemLastTime(metinSlot[0])
					else:
						if 1 == bHasRealtimeFlag:
							if app.ENABLE_COSTUME_EXTENDED_RECHARGE:
								self.AppendMallItemLastTimeEx(metinSlot[0])
							else:
								self.AppendMallItemLastTime(metinSlot[0])
			else:
				## 사용가능 시간 제한이 있는지 찾아보고
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1

					if item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
						bHasRealtimeFirstFlag = 1

				if app.ENABLE_MOUNT_COSTUME_SYSTEM:
					if item.GetItemSubType() == item.COSTUME_TYPE_MOUNT:
						if not bHasRealtimeFirstFlag:
							if 1 == bHasRealtimeFlag:
								if app.ENABLE_COSTUME_EXTENDED_RECHARGE:
									self.AppendMallItemLastTimeEx(metinSlot[0])
								else:
									self.AppendMallItemLastTime(metinSlot[0])
							else:
								time = metinSlot[player.METIN_SOCKET_MAX_NUM-1]

								if 1 == item.GetValue(2): ## 실시간 이용 Flag / 장착 안해도 준다
									self.AppendMallItemLastTime(time)
								else:
									self.AppendUniqueItemLastTime(time)
					else:
						if 1 == bHasRealtimeFlag:
							if app.ENABLE_COSTUME_EXTENDED_RECHARGE:
								self.AppendMallItemLastTimeEx(metinSlot[0])
							else:
								self.AppendMallItemLastTime(metinSlot[0])
				else:
					if 1 == bHasRealtimeFlag:
						if app.ENABLE_COSTUME_EXTENDED_RECHARGE:
							self.AppendMallItemLastTimeEx(metinSlot[0])
						else:
							self.AppendMallItemLastTime(metinSlot[0])

			if app.ENABLE_MOUNT_COSTUME_SYSTEM:
				if item.GetItemSubType() == item.COSTUME_TYPE_MOUNT:
					if osfInfo.SHOW_USE_IMAGE_TEXT_LINE == True:
						self.AppendSpace(5)
						self.AppendTextLine("|Ikey/mouse_right|i - %s" % localeInfo.TOOLTIP_ITEM_MOUNT)

		## Rod ##
		elif item.ITEM_TYPE_ROD == itemType:
			if 0 != metinSlot:
				curLevel = item.GetValue(0) / 10
				curEXP = metinSlot[0]
				maxEXP = item.GetValue(2)
				self.__AppendLimitInformation()
				self.__AppendRodInformation(curLevel, curEXP, maxEXP)

		## Pick ##
		elif item.ITEM_TYPE_PICK == itemType:
			if 0 != metinSlot:
				curLevel = item.GetValue(0) / 10
				curEXP = metinSlot[0]
				maxEXP = item.GetValue(2)
				self.__AppendLimitInformation()
				self.__AppendPickInformation(curLevel, curEXP, maxEXP)

		## Lottery ##
		elif item.ITEM_TYPE_LOTTERY == itemType:
			if 0 != metinSlot:
				ticketNumber = int(metinSlot[0])
				stepNumber = int(metinSlot[1])

				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_LOTTERY_STEP_NUMBER % (stepNumber), self.NORMAL_COLOR)
				self.AppendTextLine(localeInfo.TOOLTIP_LOTTO_NUMBER % (ticketNumber), self.NORMAL_COLOR);

		### Metin ###
		elif item.ITEM_TYPE_METIN == itemType:
			self.AppendMetinInformation()
			self.AppendMetinWearInformation()

		### Fish ###
		elif item.ITEM_TYPE_FISH == itemType:
			if 0 != metinSlot:
				self.__AppendFishInfo(metinSlot[0])

		## item.ITEM_TYPE_BLEND
		elif item.ITEM_TYPE_BLEND == itemType:
			self.__AppendLimitInformation()

			if metinSlot:
				affectType = metinSlot[0]
				affectValue = metinSlot[1]
				time = metinSlot[2]
				self.AppendSpace(5)
				affectText = self.__GetAffectString(affectType, affectValue)

				self.AppendTextLine(affectText, self.NORMAL_COLOR)

				if app.ENABLE_EXTENDED_BLEND_AFFECT:
					status = metinSlot[3]

					self.AppendSpace(10)
					if status != 0:
						self.AppendTextLine(localeInfo.BLEND_ITEM_TOOLTIP_APPLY, 0xff86e57f) # RGB(134, 229, 127)
					else:
						self.AppendTextLine(localeInfo.BLEND_ITEM_TOOLTIP_UNAPPLIED, 0xfff15f5f) # RGB(241, 95, 95)

					self.AppendSpace(5)
					if time > item.MIN_INFINITE_DURATION:
						self.AppendTextLine(localeInfo.INFINITE_USE, self.CONDITION_COLOR)
					else:
						if time > 0:
							endTime = app.GetGlobalTimeStamp() + time
							leftSec = max(0, endTime - app.GetGlobalTimeStamp())
							self.AppendTextLine(localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHMS(leftSec), self.NORMAL_COLOR)
						else:
							self.AppendTextLine(localeInfo.BLEND_ITEM_TOOLTIP_NO_TIME)
				else:
					if time > 0:
						minute = (time / 60)
						second = (time % 60)
						timeString = localeInfo.TOOLTIP_POTION_TIME

						if minute > 0:
							timeString += str(minute) + localeInfo.TOOLTIP_POTION_MIN
						if second > 0:
							timeString += " " + str(second) + localeInfo.TOOLTIP_POTION_SEC

						self.AppendTextLine(timeString)
					# [MT-489] 제조창에 알수 없는 툴팁
					#else:
					#	self.AppendTextLine(localeInfo.BLEND_POTION_NO_TIME)
			else:
				self.AppendTextLine("BLEND_POTION_NO_INFO")

		elif item.ITEM_TYPE_UNIQUE == itemType:
			if 0 != metinSlot:
				bHasRealtimeFlag = 0

				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1

				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])
				else:
					time = metinSlot[player.METIN_SOCKET_MAX_NUM-1]

					if 1 == item.GetValue(2): ## 실시간 이용 Flag / 장착 안해도 준다
						self.AppendMallItemLastTime(time)
					else:
						self.AppendUniqueItemLastTime(time)

		### Use ###
		elif item.ITEM_TYPE_USE == itemType:
			self.__AppendLimitInformation()

			if item.USE_POTION == itemSubType or item.USE_POTION_NODELAY == itemSubType:
				self.__AppendPotionInformation()

			elif item.USE_ABILITY_UP == itemSubType:
				self.__AppendAbilityPotionInformation()

			## 영석 감지기
			if 27989 == itemVnum or 76006 == itemVnum:
				if 0 != metinSlot:
					useCount = int(metinSlot[0])

					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT % (6 - useCount), self.NORMAL_COLOR)

			## 이벤트 감지기
			elif 50004 == itemVnum:
				if 0 != metinSlot:
					useCount = int(metinSlot[0])

					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT % (10 - useCount), self.NORMAL_COLOR)

			## 자동물약
			elif constInfo.IS_AUTO_POTION(itemVnum):
				if 0 != metinSlot:
					## 0: 활성화, 1: 사용량, 2: 총량
					isActivated = int(metinSlot[0])
					usedAmount = float(metinSlot[1])
					totalAmount = float(metinSlot[2])

					if 0 == totalAmount:
						totalAmount = 1

					self.AppendSpace(5)

					if 0 != isActivated:
						self.AppendTextLine("(%s)" % (localeInfo.TOOLTIP_AUTO_POTION_USING), self.SPECIAL_POSITIVE_COLOR)
						self.AppendSpace(5)

					self.AppendTextLine(localeInfo.TOOLTIP_AUTO_POTION_REST % (100.0 - ((usedAmount / totalAmount) * 100.0)), self.POSITIVE_COLOR)

			## 귀환 기억부
			elif itemVnum in WARP_SCROLLS:
				if 0 != metinSlot:
					xPos = int(metinSlot[0])
					yPos = int(metinSlot[1])

					if xPos != 0 and yPos != 0:
						(mapName, xBase, yBase) = background.GlobalPositionToMapInfo(xPos, yPos)

						localeMapName = localeInfo.MINIMAP_ZONE_NAME_DICT.get(mapName, "")

						self.AppendSpace(5)

						if localeMapName != "":
							self.AppendTextLine(localeInfo.TOOLTIP_MEMORIZED_POSITION % (localeMapName, int(xPos-xBase)/100, int(yPos-yBase)/100), self.NORMAL_COLOR)
						else:
							self.AppendTextLine(localeInfo.TOOLTIP_MEMORIZED_POSITION_ERROR % (int(xPos)/100, int(yPos)/100), self.NORMAL_COLOR)
							dbg.TraceError("NOT_EXIST_IN_MINIMAP_ZONE_NAME_DICT: %s" % mapName)

			#####
			if item.USE_SPECIAL == itemSubType:
				bHasRealtimeFlag = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1

				## 있다면 관련 정보를 표시함. ex) 남은 시간 : 6일 6시간 58분 
				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])
				else:
					# ... 이거... 서버에는 이런 시간 체크 안되어 있는데...
					# 왜 이런게 있는지 알지는 못하나 그냥 두자...
					if 0 != metinSlot:
						time = metinSlot[player.METIN_SOCKET_MAX_NUM-1]

						## 실시간 이용 Flag
						if 1 == item.GetValue(2):
							self.AppendMallItemLastTime(time)

			elif item.USE_TIME_CHARGE_PER == itemSubType:
				bHasRealtimeFlag = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1

				if metinSlot[2]:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_PER(metinSlot[2]))
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_PER(item.GetValue(0)))

				## 있다면 관련 정보를 표시함. ex) 남은 시간 : 6일 6시간 58분 
				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])

			elif item.USE_TIME_CHARGE_FIX == itemSubType:
				bHasRealtimeFlag = 0
				for i in xrange(item.LIMIT_MAX_NUM):
					(limitType, limitValue) = item.GetLimit(i)

					if item.LIMIT_REAL_TIME == limitType:
						bHasRealtimeFlag = 1

				if metinSlot[2]:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_FIX(metinSlot[2]))
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_TIME_CHARGER_FIX(item.GetValue(0)))

				## 있다면 관련 정보를 표시함. ex) 남은 시간 : 6일 6시간 58분 
				if 1 == bHasRealtimeFlag:
					self.AppendMallItemLastTime(metinSlot[0])

		elif item.ITEM_TYPE_QUEST == itemType:
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if item.LIMIT_REAL_TIME == limitType:
					self.AppendMallItemLastTime(metinSlot[0])

		# app.ENABLE_PET_SYSTEM
		elif item.ITEM_TYPE_PET == itemType and item.PET_PAY == itemSubType:
			if osfInfo.PET_ITEM_ATTR_TOOLTIP == True:
				if item.IsPetPay(itemVnum):
					self.__AppendAffectInformation()
					if app.ENABLE_APPLY_RANDOM:
						self.__AppendDefaultItemApplyInformation(apply_random_list)
					self.__AppendAttributeInformation(attrSlot)

			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if item.LIMIT_REAL_TIME == limitType:
					self.AppendMallItemLastTime(metinSlot[0])

			if osfInfo.SHOW_USE_IMAGE_TEXT_LINE == True:
				if item.IsPetPay(itemVnum):
					self.AppendSpace(5)
					self.AppendTextLine("|Ikey/mouse_right|i - %s" % localeInfo.TOOLTIP_ITEM_SUMMON)
				else:
					pass

		elif item.ITEM_TYPE_DS == itemType:
			self.AppendTextLine(self.__DragonSoulInfoString(itemVnum))

			if app.ENABLE_DS_SET:
				if window_type == player.EQUIPMENT and self.interface and self.interface.wndDragonSoul:
					self.__AppendDragonSoulAttributeInformation(attrSlot, itemVnum / 10000, self.interface.wndDragonSoul.GetDSSetGrade())
				else:
					self.__AppendDragonSoulAttributeInformation(attrSlot)
			else:
				self.__AppendAttributeInformation(attrSlot)

		else:
			self.__AppendLimitInformation()

		# if item.ITEM_TYPE_GIFTBOX == itemType:
			# for i in xrange(item.LIMIT_MAX_NUM):
				# (limitType, limitValue) = item.GetLimit(i)

				# if item.LIMIT_REAL_TIME == limitType:
					# self.AppendMallItemLastTime(metinSlot[0])

		for i in xrange(item.LIMIT_MAX_NUM):
			(limitType, limitValue) = item.GetLimit(i)
			# dbg.TraceError("LimitType : %d, limitValue : %d" % (limitType, limitValue))

			if item.LIMIT_REAL_TIME_START_FIRST_USE == limitType:
				self.AppendRealTimeStartFirstUseLastTime(item, metinSlot, i)
				# dbg.TraceError("2) REAL_TIME_START_FIRST_USE flag On ")

			elif item.LIMIT_TIMER_BASED_ON_WEAR == limitType:
				self.AppendTimerBasedOnWearLastTime(metinSlot)
				# dbg.TraceError("1) REAL_TIME flag On ")

		## 2015 크리스마스 이벤트 아이템으로 2개의 아이템은 REAL_TIME 이 아님에도 기간제 아이템이다
		if self.itemVnum in [79505, 79506]:
			if 0 != metinSlot[0]:
				self.AppendMallItemLastTime(metinSlot[0])

		# if app.ENABLE_10TH_EVENT:
		if self.itemVnum in [25101, 25103]:
			if 0 != metinSlot[0]:
				self.AppendMallItemLastTime(metinSlot[0])

		# if app.ENABLE_2017_RAMADAN:
		if self.itemVnum in [30315, 30317, 30318, 50183]:
			if 0 != metinSlot[0]:
				self.AppendMallItemLastTime(metinSlot[0])

		# if app.ENABLE_MINI_GAME_YUTNORI:
		if self.itemVnum in [79507, 79508]:
			if 0 != metinSlot[0]:
				self.AppendMallItemLastTime(metinSlot[0])

		if app.ENABLE_BOSS_BOX:
			if 0 != metinSlot and item.ITEM_TYPE_GACHA == itemType:
				useCount = int(metinSlot[0])

				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_REST_USABLE_COUNT % useCount, grp.GenerateColor(0.6705, 0.9490, 0.0, 1.0))

		if app.ENABLE_SOUL_BIND_SYSTEM:
			self.__AppendSealInformation(window_type, slotIndex) ## cyh itemseal 2013 11 11

		if osfInfo.SHOW_SELL_IMAGE_TEXT_LINE == True:
			if slotIndex < player.EQUIPMENT_SLOT_START:
				if not player.IsAntiFlagBySlot(slotIndex, item.ANTIFLAG_SELL):
					self.AppendSpace(5)
					self.AppendTextLine("|Ikey/ctrl|i + |Ikey/x|i + |Ikey/mouse_left|i - Remover")

		if osfInfo.SHOW_ITEM_VNUM_TOOTIP == True:
			self.AppendSpace(5)
			self.AppendTextLine("VNUM %d" % itemVnum, grp.GenerateColor(0.6705, 0.9490, 0.0, 1.0))

		self.ShowToolTip()

	def __DragonSoulInfoString (self, dwVnum):
		step = (dwVnum / 100) % 10
		refine = (dwVnum / 10) % 10
		if 0 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL1 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 1 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL2 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 2 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL3 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 3 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL4 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		elif 4 == step:
			return localeInfo.DRAGON_SOUL_STEP_LEVEL5 + " " + localeInfo.DRAGON_SOUL_STRENGTH(refine)
		else:
			return ""

	## 헤어인가?
	def __IsHair(self, itemVnum):
		item.SelectItem(itemVnum)
		itemType = item.GetItemType()
		if itemType != item.ITEM_TYPE_COSTUME:
			return 0 # hot fix

		return (
			self.__IsOldHair(itemVnum) or 
			self.__IsNewHair(itemVnum) or
			self.__IsNewHair2(itemVnum) or
			self.__IsNewHair3(itemVnum) or
			self.__IsCostumeHair(itemVnum)
		)

	def __IsOldHair(self, itemVnum):
		return itemVnum > 73000 and itemVnum < 74000

	def __IsNewHair(self, itemVnum):
		return itemVnum > 74000 and itemVnum < 75000

	def __IsNewHair2(self, itemVnum):
		return itemVnum > 75000 and itemVnum < 76000

	def __IsNewHair3(self, itemVnum):
		return ((74012 < itemVnum and itemVnum < 74022) or
			(74262 < itemVnum and itemVnum < 74272) or
			(74512 < itemVnum and itemVnum < 74522) or
			(74762 < itemVnum and itemVnum < 74772) or
			(45000 < itemVnum and itemVnum < 47000))

	def __IsCostumeHair(self, itemVnum):
		return self.__IsNewHair3(itemVnum - 100000)

	if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
		def __AppendPrivateSearchItemIcon(self, itemVnum):
			itemImage = ui.ImageBox()
			itemImage.SetParent(self)
			itemImage.Show()
			item.SelectItem(itemVnum)
			itemImage.LoadImage(item.GetIconImageFileName())
			itemImage.SetPosition((self.toolTipWidth / 2) - 16, self.toolTipHeight)
			self.toolTipHeight += itemImage.GetHeight()
			self.childrenList.append(itemImage)
			self.ResizeToolTip()

	def __AppendHairIcon(self, itemVnum):
		itemImage = ui.ImageBox()
		itemImage.SetParent(self)
		itemImage.Show()

		if self.__IsOldHair(itemVnum):
			itemImage.LoadImage("d:/ymir work/item/quest/" + str(itemVnum) + ".tga")
		elif self.__IsNewHair3(itemVnum):
			itemImage.LoadImage("icon/hair/%d.sub" % (itemVnum))
		elif self.__IsNewHair(itemVnum): # 기존 헤어 번호를 연결시켜서 사용한다. 새로운 아이템은 1000만큼 번호가 늘었다.
			if itemVnum > 74520 and itemVnum < 74751:
				itemImage.LoadImage("icon/hair/%d.sub" % (itemVnum))
			else:
				itemImage.LoadImage("d:/ymir work/item/quest/" + str(itemVnum-1000) + ".tga")
		elif self.__IsNewHair2(itemVnum):
			itemImage.LoadImage("icon/hair/%d.sub" % (itemVnum))
		elif self.__IsCostumeHair(itemVnum):
			itemImage.LoadImage("icon/hair/%d.sub" % (itemVnum - 100000))

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			if self.isPrivateSearchItem:
				itemImage.SetPosition((self.toolTipWidth / 2) - 48, self.toolTipHeight)
			else:
				itemImage.SetPosition((self.toolTipWidth / 2) - 48, self.toolTipHeight)
		else:
			itemImage.SetPosition(itemImage.GetWidth() / 2, self.toolTipHeight)

		self.toolTipHeight += itemImage.GetHeight()
		#self.toolTipWidth += itemImage.GetWidth() / 2
		self.childrenList.append(itemImage)
		self.ResizeToolTip()

	def ResizeToolTipWidth(self, width):
		self.toolTipWidth = width

	def __CalculateToolTipWidth(self):
		affectTextLineLenList = []

		metinSocket = self.metinSlot
		if metinSocket:
			for socketIndex in metinSocket:
				if socketIndex:
					item.SelectItem(socketIndex)

					affectType, affectValue = item.GetAffect(0)
					affectString = self.__GetAffectString(affectType, affectValue)
					if affectString:
						affectTextLineLenList.append(len(affectString))

			if self.itemVnum:
				item.SelectItem(self.itemVnum)
			self.metinSlot = None

		if self.toolTipWidth == self.TOOL_TIP_WIDTH:
			if affectTextLineLenList:
				self.toolTipWidth += max(affectTextLineLenList) + 10

		self.AlignTextLineHorizonalCenter()

	## 사이즈가 큰 Description 일 경우 툴팁 사이즈를 조정한다
	def __AdjustMaxWidth(self, attrSlot, desc):
		newToolTipWidth = self.toolTipWidth
		newToolTipWidth = max(self.__AdjustAttrMaxWidth(attrSlot), newToolTipWidth)
		newToolTipWidth = max(self.__AdjustDescMaxWidth(desc), newToolTipWidth)
		if newToolTipWidth > self.toolTipWidth:
			self.toolTipWidth = newToolTipWidth
			self.ResizeToolTip()

	def __AdjustAttrMaxWidth(self, attrSlot):
		if 0 == attrSlot:
			return self.toolTipWidth

		maxWidth = self.toolTipWidth
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			type = attrSlot[i][0]
			value = attrSlot[i][1]
			if self.ATTRIBUTE_NEED_WIDTH.has_key(type):
				if value > 0:
					maxWidth = max(self.ATTRIBUTE_NEED_WIDTH[type], maxWidth)

					# ATTR_CHANGE_TOOLTIP_WIDTH
					#self.toolTipWidth = max(self.ATTRIBUTE_NEED_WIDTH[type], self.toolTipWidth)
					#self.ResizeToolTip()
					# END_OF_ATTR_CHANGE_TOOLTIP_WIDTH

		return maxWidth

	def __AdjustDescMaxWidth(self, desc):
		if len(desc) < DESC_DEFAULT_MAX_COLS:
			return self.toolTipWidth

		return DESC_WESTERN_MAX_WIDTH

	def __SetSkillBookToolTip(self, skillIndex, bookName, skillGrade):
		skillName = skill.GetSkillName(skillIndex)

		if not skillName:
			return

		if localeInfo.IsVIETNAM():
			itemName = bookName + " " + skillName
		else:
			itemName = skillName + " " + bookName
		self.SetTitle(itemName)

	def __AppendPickInformation(self, curLevel, curEXP, maxEXP):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_PICK_LEVEL % (curLevel), self.NORMAL_COLOR)

		self.AppendTextLine(localeInfo.TOOLTIP_PICK_EXP % (curEXP, maxEXP), self.NORMAL_COLOR)

		if curEXP == maxEXP:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_PICK_UPGRADE1, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_PICK_UPGRADE2, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_PICK_UPGRADE3, self.NORMAL_COLOR)

	def __AppendRodInformation(self, curLevel, curEXP, maxEXP):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_LEVEL % (curLevel), self.NORMAL_COLOR)

		self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_EXP % (curEXP, maxEXP), self.NORMAL_COLOR)

		if curEXP == maxEXP:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_UPGRADE1, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_UPGRADE2, self.NORMAL_COLOR)
			self.AppendTextLine(localeInfo.TOOLTIP_FISHINGROD_UPGRADE3, self.NORMAL_COLOR)

	def __AppendLimitInformation(self):
		appendSpace = False

		for i in xrange(item.LIMIT_MAX_NUM):
			(limitType, limitValue) = item.GetLimit(i)

			if limitValue > 0:
				if False == appendSpace:
					self.AppendSpace(5)
					appendSpace = True
			else:
				continue

			if item.LIMIT_LEVEL == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.LEVEL), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_LEVEL % (limitValue), color)
			"""
			elif item.LIMIT_STR == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.ST), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_STR % (limitValue), color)
			elif item.LIMIT_DEX == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.DX), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_DEX % (limitValue), color)
			elif item.LIMIT_INT == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.IQ), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_INT % (limitValue), color)
			elif item.LIMIT_CON == limitType:
				color = self.GetLimitTextLineColor(player.GetStatus(player.HT), limitValue)
				self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_CON % (limitValue), color)
			"""

	if app.ENABLE_SOUL_BIND_SYSTEM:
		## cyh itemseal 2013 11 11
		def __AppendSealInformation(self, window_type, slotIndex):
			itemSealDate = player.GetItemSealDate(window_type, slotIndex)
			if itemSealDate == item.GetUnlimitedSealDate():
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SEALED, self.NEGATIVE_COLOR)
			elif itemSealDate > 0:
				self.AppendSpace(5)
				hours, minutes = player.GetItemUnSealLeftTime(window_type, slotIndex)
				self.AppendTextLine(localeInfo.TOOLTIP_UNSEAL_LEFT_TIME % (hours, minutes), self.NEGATIVE_COLOR )

	def __GetAffectString(self, affectType, affectValue):
		if 0 == affectType:
			return None

		if 0 == affectValue:
			return None

		if app.ENABLE_APPLY_RANDOM:
			if affectType == item.APPLY_RANDOM:
				return None

		try:
			return self.AFFECT_DICT[affectType](affectValue)
		except TypeError:
			return "UNKNOWN_VALUE[%s] %s" % (affectType, affectValue)
		except KeyError:
			return "UNKNOWN_TYPE[%s] %s" % (affectType, affectValue)

	def __AppendAffectInformation(self):
		for i in xrange(item.ITEM_APPLY_MAX_NUM):
			(affectType, affectValue) = item.GetAffect(i)
			affectString = self.__GetAffectString(affectType, affectValue)

			if affectString:
				#if affectType == item.APPLY_ACCEDRAIN_RATE:
				#	self.AppendTextLine(affectString, self.CONDITION_COLOR)
				#else:
				#	self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
				self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))

	if app.ENABLE_CHANGE_LOOK_SYSTEM:
		def AppendChangeLookInformation(self, window_type, slotIndex):
			if window_type == player.SAFEBOX:
				changelookvnum = safebox.GetItemChangeLookVnum(slotIndex)
			elif window_type == player.MALL:
				changelookvnum = safebox.GetMallItemChangeLookVnum(slotIndex)
			#elif window_type == player.ACCEREFINE:
			#	changelookvnum = player.GetAcceWindowChangeLookVnum(slotIndex)
			else:
				changelookvnum = player.GetChangeLookVnum(window_type, slotIndex)

			if not changelookvnum == 0:
				self.AppendSpace(5)
				self.AppendTextLine("[ " + localeInfo.CHANGE_LOOK_TITLE + " ]", self.CHANGELOOK_TITLE_COLOR)
				itemName = item.GetItemNameByVnum(changelookvnum)

				if item.GetItemType() == item.ITEM_TYPE_COSTUME:
					if item.GetItemSubType() == item.COSTUME_TYPE_BODY:
						malefemale = ""
						if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
							malefemale = localeInfo.FOR_FEMALE

						if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
							malefemale = localeInfo.FOR_MALE
						itemName += " ( " + malefemale + " )"

				textLine = self.AppendTextLine(itemName, self.CHANGELOOK_ITEMNAME_COLOR, True)
				textLine.SetFeather()

	def AppendWearableInformation(self):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_ITEM_WEARABLE_JOB, self.NORMAL_COLOR)

		if not app.DISABLE_WOLFMAN_CREATION:
			flagList = (
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN),
				)
		else:
			flagList = (
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN),
				)

		characterNames = ""
		for i in xrange(self.CHARACTER_COUNT):
			name = self.CHARACTER_NAMES[i]
			flag = flagList[i]

			if flag:
				characterNames += " "
				characterNames += name

		textLine = self.AppendTextLine(characterNames, self.NORMAL_COLOR, True)
		textLine.SetFeather()

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
			textLine = self.AppendTextLine(localeInfo.FOR_FEMALE, self.NORMAL_COLOR, True)
			textLine.SetFeather()

		if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
			textLine = self.AppendTextLine(localeInfo.FOR_MALE, self.NORMAL_COLOR, True)
			textLine.SetFeather()

	def __AppendPotionInformation(self):
		self.AppendSpace(5)

		healHP = item.GetValue(0)
		healSP = item.GetValue(1)
		healStatus = item.GetValue(2)
		healPercentageHP = item.GetValue(3)
		healPercentageSP = item.GetValue(4)

		if healHP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_HP_POINT % healHP, self.GetChangeTextLineColor(healHP))
		if healSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_SP_POINT % healSP, self.GetChangeTextLineColor(healSP))
		if healStatus != 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_CURE)
		if healPercentageHP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_HP_PERCENT % healPercentageHP, self.GetChangeTextLineColor(healPercentageHP))
		if healPercentageSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_SP_PERCENT % healPercentageSP, self.GetChangeTextLineColor(healPercentageSP))

	def __AppendAbilityPotionInformation(self):
		self.AppendSpace(5)

		abilityType = item.GetValue(0)
		time = item.GetValue(1)
		point = item.GetValue(2)

		if abilityType == item.APPLY_ATT_SPEED:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_ATTACK_SPEED % point, self.GetChangeTextLineColor(point))
		elif abilityType == item.APPLY_MOV_SPEED:
			self.AppendTextLine(localeInfo.TOOLTIP_POTION_PLUS_MOVING_SPEED % point, self.GetChangeTextLineColor(point))

		if time > 0:
			minute = (time / 60)
			second = (time % 60)
			timeString = localeInfo.TOOLTIP_POTION_TIME

			if minute > 0:
				timeString += str(minute) + localeInfo.TOOLTIP_POTION_MIN
			if second > 0:
				timeString += " " + str(second) + localeInfo.TOOLTIP_POTION_SEC

			self.AppendTextLine(timeString)

	def GetPriceColor(self, price):
		if price >= constInfo.HIGH_PRICE:
			return self.HIGH_PRICE_COLOR
		if price >= constInfo.MIDDLE_PRICE:
			return self.MIDDLE_PRICE_COLOR
		else:
			return self.LOW_PRICE_COLOR

	if app.ENABLE_CHEQUE_SYSTEM:
		def AppendPrice(self, price, cheque = 0):
			self.AppendSpace(5)
			self.AppendTextLineDontCheckColor(localeInfo.CHEQUE_SYSTEM_SELL_PRICE, grp.GenerateColor(1.0, 0.9686, 0.3098, 1.0))

			if cheque > 0:
				self.AppendTextLineDontCheckColor(localeInfo.CHEQUE_SYSTEM_WON % (str(cheque)), grp.GenerateColor(0.0, 0.8470, 1.0, 1.0))

			if price >= 0:
				self.AppendTextLineDontCheckColor(localeInfo.CHEQUE_SYSTEM_YANG % (localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))
	else:
		def AppendPrice(self, price):
			self.AppendSpace(5)
			self.AppendTextLineDontCheckColor(localeInfo.TOOLTIP_BUYPRICE % (localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))

	def AppendPriceBySecondaryCoin(self, price):
		self.AppendSpace(5)
		self.AppendTextLineDontCheckColor(localeInfo.TOOLTIP_BUYPRICE % (localeInfo.NumberToSecondaryCoinString(price)), self.GetPriceColor(price))

	if app.ENABLE_CHEQUE_SYSTEM:
		def AppendSellingPrice(self, price, cheque = 0, isPrivateShopBuilder = False):
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_SELL) and \
				not isPrivateShopBuilder:
				self.AppendTextLineDontCheckColor(localeInfo.TOOLTIP_ANTI_SELL, self.DISABLE_COLOR)
				self.AppendSpace(5)
			else:
				self.AppendTextLineDontCheckColor(localeInfo.CHEQUE_SYSTEM_SELL_PRICE, grp.GenerateColor(1.0, 0.9686, 0.3098, 1.0))

				if cheque > 0:
					self.AppendTextLineDontCheckColor(localeInfo.CHEQUE_SYSTEM_WON % (str(cheque)), grp.GenerateColor(0.0, 0.8470, 1.0, 1.0))

				if price >= 0:
					self.AppendTextLineDontCheckColor(localeInfo.CHEQUE_SYSTEM_YANG % (localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))

				self.AppendSpace(5)
	else:
		def AppendSellingPrice(self, price):
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_SELL):
				self.AppendTextLineDontCheckColor(localeInfo.TOOLTIP_ANTI_SELL, self.DISABLE_COLOR)
				self.AppendSpace(5)
			else:
				self.AppendTextLineDontCheckColor(localeInfo.TOOLTIP_SELLPRICE % (localeInfo.NumberToMoneyString(price)), self.GetPriceColor(price))
				self.AppendSpace(5)

	def AppendMetinInformation(self):
		affectType, affectValue = item.GetAffect(0)
		#affectType = item.GetValue(0)
		#affectValue = item.GetValue(1)

		affectString = self.__GetAffectString(affectType, affectValue)

		if affectString:
			self.AppendSpace(5)
			self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))

	def AppendMetinWearInformation(self):
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_SOCKET_REFINABLE_ITEM, self.NORMAL_COLOR)

		flagList = (
			item.IsWearableFlag(item.WEARABLE_BODY),
			item.IsWearableFlag(item.WEARABLE_HEAD),
			item.IsWearableFlag(item.WEARABLE_FOOTS),
			item.IsWearableFlag(item.WEARABLE_WRIST),
			item.IsWearableFlag(item.WEARABLE_WEAPON),
			item.IsWearableFlag(item.WEARABLE_NECK),
			item.IsWearableFlag(item.WEARABLE_EAR),
			item.IsWearableFlag(item.WEARABLE_UNIQUE),
			item.IsWearableFlag(item.WEARABLE_SHIELD),
			item.IsWearableFlag(item.WEARABLE_ARROW)
		)

		wearNames = ""
		for i in xrange(self.WEAR_COUNT):
			name = self.WEAR_NAMES[i]
			flag = flagList[i]

			if flag:
				wearNames += " "
				wearNames += name

		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetFontName(self.defFontName)
		textLine.SetPosition(self.toolTipWidth/2, self.toolTipHeight)
		textLine.SetHorizontalAlignCenter()
		textLine.SetPackedFontColor(self.NORMAL_COLOR)
		textLine.SetText(wearNames)
		textLine.Show()
		self.childrenList.append(textLine)

		self.toolTipHeight += self.TEXT_LINE_HEIGHT
		self.ResizeToolTip()

	def GetMetinSocketType(self, number):
		if player.METIN_SOCKET_TYPE_NONE == number:
			return player.METIN_SOCKET_TYPE_NONE
		elif player.METIN_SOCKET_TYPE_SILVER == number:
			return player.METIN_SOCKET_TYPE_SILVER
		elif player.METIN_SOCKET_TYPE_GOLD == number:
			return player.METIN_SOCKET_TYPE_GOLD
		else:
			if app.WJ_NEW_USER_CARE:
				if item.SelectItem(number) == False:
					return player.METIN_SOCKET_TYPE_NONE
				else:
					if item.METIN_NORMAL == item.GetItemSubType():
						return player.METIN_SOCKET_TYPE_SILVER
					elif item.METIN_GOLD == item.GetItemSubType():
						return player.METIN_SOCKET_TYPE_GOLD
					elif "USE_PUT_INTO_ACCESSORY_SOCKET" == item.GetUseType(number):
						return player.METIN_SOCKET_TYPE_SILVER
					elif "USE_PUT_INTO_RING_SOCKET" == item.GetUseType(number):
						return player.METIN_SOCKET_TYPE_SILVER
					elif "USE_PUT_INTO_BELT_SOCKET" == item.GetUseType(number):
						return player.METIN_SOCKET_TYPE_SILVER
			else:
				item.SelectItem(number)
				if item.METIN_NORMAL == item.GetItemSubType():
					return player.METIN_SOCKET_TYPE_SILVER
				elif item.METIN_GOLD == item.GetItemSubType():
					return player.METIN_SOCKET_TYPE_GOLD
				elif "USE_PUT_INTO_ACCESSORY_SOCKET" == item.GetUseType(number):
					return player.METIN_SOCKET_TYPE_SILVER
				elif "USE_PUT_INTO_RING_SOCKET" == item.GetUseType(number):
					return player.METIN_SOCKET_TYPE_SILVER
				elif "USE_PUT_INTO_BELT_SOCKET" == item.GetUseType(number):
					return player.METIN_SOCKET_TYPE_SILVER

		return player.METIN_SOCKET_TYPE_NONE

	def GetMetinItemIndex(self, number):
		if player.METIN_SOCKET_TYPE_SILVER == number:
			return 0
		if player.METIN_SOCKET_TYPE_GOLD == number:
			return 0

		return number

	def __AppendAccessoryMetinSlotInfo(self, metinSlot, mtrlVnum):
		ACCESSORY_SOCKET_MAX_SIZE = 3

		cur = min(metinSlot[0], ACCESSORY_SOCKET_MAX_SIZE)
		end = min(metinSlot[1], ACCESSORY_SOCKET_MAX_SIZE)

		affectType1, affectValue1 = item.GetAffect(0)
		affectList1 = [0, max(1, affectValue1 * 10 / 100), max(2, affectValue1 * 20 / 100), max(3, affectValue1 * 40 / 100)]

		affectType2, affectValue2 = item.GetAffect(1)
		affectList2 = [0, max(1, affectValue2 * 10 / 100), max(2, affectValue2 * 20 / 100), max(3, affectValue2 * 40 / 100)]

		affectType3, affectValue3 = item.GetAffect(2)
		affectList3 = [0, max(1, affectValue3 * 10 / 100), max(2, affectValue3 * 20 / 100), max(3, affectValue3 * 40 / 100)]

		mtrlPos = 0
		mtrlList = [mtrlVnum] * cur + [player.METIN_SOCKET_TYPE_SILVER] * (end - cur)
		for mtrl in mtrlList:
			affectString1 = self.__GetAffectString(affectType1, affectList1[mtrlPos + 1] - affectList1[mtrlPos])
			affectString2 = self.__GetAffectString(affectType2, affectList2[mtrlPos + 1] - affectList2[mtrlPos])
			affectString3 = self.__GetAffectString(affectType3, affectList3[mtrlPos + 1] - affectList3[mtrlPos])

			leftTime = 0
			if cur == mtrlPos + 1:
				leftTime = metinSlot[2]

			self.__AppendMetinSlotInfo_AppendMetinSocketData(mtrlPos, mtrl, affectString1, affectString2, affectString3, leftTime)
			mtrlPos += 1

	def __AppendMetinSlotInfo(self, metinSlot):
		if self.__AppendMetinSlotInfo_IsEmptySlotList(metinSlot):
			return

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			if app.ENABLE_ITEM_SOCKET5:
				if metinSlot[i] == -1:
					continue

			self.__AppendMetinSlotInfo_AppendMetinSocketData(i, metinSlot[i])

	def __AppendMetinSlotInfo_IsEmptySlotList(self, metinSlot):
		if 0 == metinSlot:
			return 1

		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlotData = metinSlot[i]
			if app.ENABLE_ITEM_SOCKET5:
				if metinSlotData == -1:
					continue

			if 0 != self.GetMetinSocketType(metinSlotData):
				if 0 != self.GetMetinItemIndex(metinSlotData):
					return 0

		return 1

	def __AppendMetinSlotInfo_AppendMetinSocketData(self, index, metinSlotData, custumAffectString = "", custumAffectString2 = "", custumAffectString3 = "", leftTime = 0):
		slotType = self.GetMetinSocketType(metinSlotData)
		itemIndex = self.GetMetinItemIndex(metinSlotData)

		# 2019.12.17.owsap.Only show socket items
		# if app.ENABLE_ITEM_SOCKET5:
			# if itemIndex:
				# item.SelectItem(itemIndex)
				# if item.GetItemType() != item.ITEM_TYPE_METIN and\
					# item.GetUseType(itemIndex) != "USE_PUT_INTO_ACCESSORY_SOCKET" and\
					# item.GetUseType(itemIndex) != "USE_PUT_INTO_RING_SOCKET" and\
					# item.GetUseType(itemIndex) != "USE_PUT_INTO_BELT_SOCKET":
					# return

		if 0 == slotType:
			return

		self.AppendSpace(5)

		slotImage = ui.ImageBox()
		slotImage.SetParent(self)
		slotImage.Show()

		## Name
		nameTextLine = ui.TextLine()
		nameTextLine.SetParent(self)
		nameTextLine.SetFontName(self.defFontName)
		nameTextLine.SetPackedFontColor(self.NORMAL_COLOR)
		nameTextLine.SetOutline()
		nameTextLine.SetFeather()
		nameTextLine.Show()

		self.childrenList.append(nameTextLine)

		if player.METIN_SOCKET_TYPE_SILVER == slotType:
			slotImage.LoadImage("d:/ymir work/ui/game/windows/metin_slot_silver.sub")
		elif player.METIN_SOCKET_TYPE_GOLD == slotType:
			slotImage.LoadImage("d:/ymir work/ui/game/windows/metin_slot_gold.sub")

		self.childrenList.append(slotImage)

		if localeInfo.IsARABIC():
			slotImage.SetPosition(self.toolTipWidth - slotImage.GetWidth() - 9, self.toolTipHeight - 1)
			nameTextLine.SetPosition(self.toolTipWidth - 50, self.toolTipHeight + 2)
		else:
			slotImage.SetPosition(9, self.toolTipHeight - 1)
			nameTextLine.SetPosition(50, self.toolTipHeight + 2)

		metinImage = ui.ImageBox()
		metinImage.SetParent(self)
		metinImage.Show()
		self.childrenList.append(metinImage)

		if itemIndex:
			item.SelectItem(itemIndex)

			## Image
			try:
				metinImage.LoadImage(item.GetIconImageFileName())
			except:
				dbg.TraceError("ItemToolTip.__AppendMetinSocketData() - Failed to find image file %d:%s" % 
					(itemIndex, item.GetIconImageFileName())
				)

			nameTextLine.SetText(item.GetItemName())

			## Affect
			affectTextLine = ui.TextLine()
			affectTextLine.SetParent(self)
			affectTextLine.SetFontName(self.defFontName)
			affectTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
			affectTextLine.SetOutline()
			affectTextLine.SetFeather()
			affectTextLine.Show()

			if localeInfo.IsARABIC():
				metinImage.SetPosition(self.toolTipWidth - metinImage.GetWidth() - 10, self.toolTipHeight)
				affectTextLine.SetPosition(self.toolTipWidth - 50, self.toolTipHeight + 16 + 2)
			else:
				metinImage.SetPosition(10, self.toolTipHeight)
				affectTextLine.SetPosition(50, self.toolTipHeight + 16 + 2)

			if custumAffectString:
				affectTextLine.SetText(custumAffectString)
			elif itemIndex != constInfo.ERROR_METIN_STONE:
				affectType, affectValue = item.GetAffect(0)
				affectString = self.__GetAffectString(affectType, affectValue)
				if affectString:
					affectTextLine.SetText(affectString)
			else:
				affectTextLine.SetText(localeInfo.TOOLTIP_APPLY_NOAFFECT)

			self.childrenList.append(affectTextLine)

			if custumAffectString2:
				affectTextLine = ui.TextLine()
				affectTextLine.SetParent(self)
				affectTextLine.SetFontName(self.defFontName)
				affectTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				affectTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
				affectTextLine.SetOutline()
				affectTextLine.SetFeather()
				affectTextLine.Show()
				affectTextLine.SetText(custumAffectString2)
				self.childrenList.append(affectTextLine)
				self.toolTipHeight += 16 + 2

			if custumAffectString3:
				affectTextLine = ui.TextLine()
				affectTextLine.SetParent(self)
				affectTextLine.SetFontName(self.defFontName)
				affectTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				affectTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
				affectTextLine.SetOutline()
				affectTextLine.SetFeather()
				affectTextLine.Show()
				affectTextLine.SetText(custumAffectString3)
				self.childrenList.append(affectTextLine)
				self.toolTipHeight += 16 + 2

			if 0 != leftTime:
				timeText = (localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(leftTime))
				timeTextLine = ui.TextLine()
				timeTextLine.SetParent(self)
				timeTextLine.SetFontName(self.defFontName)
				timeTextLine.SetPackedFontColor(self.POSITIVE_COLOR)
				timeTextLine.SetPosition(50, self.toolTipHeight + 16 + 2 + 16 + 2)
				timeTextLine.SetOutline()
				timeTextLine.SetFeather()
				timeTextLine.Show()
				timeTextLine.SetText(timeText)
				self.childrenList.append(timeTextLine)
				self.toolTipHeight += 16 + 2

		else:
			nameTextLine.SetText(localeInfo.TOOLTIP_SOCKET_EMPTY)

		self.toolTipHeight += 35
		self.ResizeToolTip()

	def __AppendFishInfo(self, size):
		if size > 0:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_FISH_LEN % (float(size) / 100.0), self.NORMAL_COLOR)

	def AppendUniqueItemLastTime(self, restMin):
		restSecond = restMin * 60
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(restSecond), self.NORMAL_COLOR)

	def AppendMallItemLastTime(self, endTime):
		leftSec = max(0, endTime - app.GetGlobalTimeStamp())

		if leftSec > 0:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(leftSec), self.NORMAL_COLOR)

	if app.ENABLE_COSTUME_EXTENDED_RECHARGE:
		def AppendMallItemLastTimeEx(self, endTime):
			leftSec = max(0, endTime - app.GetGlobalTimeStamp())

			if leftSec > item.MIN_INFINITE_DURATION:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.INFINITE_USE, self.CONDITION_COLOR)
				return

			if leftSec > 0:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(leftSec), self.NORMAL_COLOR)

	def AppendTimerBasedOnWearLastTime(self, metinSlot):
		if 0 == metinSlot[0]:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.CANNOT_USE, self.DISABLE_COLOR)
		elif app.ENABLE_DSS_EXTENDED_RECHARGE and metinSlot[0] > item.MIN_INFINITE_DURATION:
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.INFINITE_USE, self.CONDITION_COLOR)
		else:
			endTime = app.GetGlobalTimeStamp() + metinSlot[0]
			self.AppendMallItemLastTime(endTime)

	def AppendRealTimeStartFirstUseLastTime(self, item, metinSlot, limitIndex):
		useCount = metinSlot[1]
		endTime = metinSlot[0]

		# 한 번이라도 사용했다면 Socket0에 종료 시간(2012년 3월 1일 13시 01분 같은..) 이 박혀있음.
		# 사용하지 않았다면 Socket0에 이용가능시간(이를테면 600 같은 값. 초단위)이 들어있을 수 있고, 0이라면 Limit Value에 있는 이용가능시간을 사용한다.
		if 0 == useCount:
			if 0 == endTime:
				(limitType, limitValue) = item.GetLimit(limitIndex)
				endTime = limitValue

			endTime += app.GetGlobalTimeStamp()

		self.AppendMallItemLastTime(endTime)

	if app.WJ_NEW_USER_CARE:
		def __AppendRealTimeToolTip(self, itemVnum, endTime):
			item.SelectItem(itemVnum)

			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if item.LIMIT_REAL_TIME == limitType and limitValue > 0:
					self.AppendSpace(5)
					leftSec = max(0, endTime - app.GetGlobalTimeStamp())
					if leftSec > 0:
						self.AppendTextLine(localeInfo.LEFT_TIME + " : " + localeInfo.SecondToDHM(leftSec), self.NORMAL_COLOR)
					return
				else:
					continue

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def SetAcceResultItem(self, slotIndex, window_type = player.INVENTORY):
			(itemVnum, MinAbs, MaxAbs) = acce.GetResultItem()
			if not itemVnum:
				return

			self.ClearToolTip()

			metinSlot = [player.GetItemMetinSocket(window_type, slotIndex, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [player.GetItemAttribute(window_type, slotIndex, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			apply_random_list = []
			if app.ENABLE_APPLY_RANDOM:
				apply_random_list = [player.GetItemApplyRandom(window_type, slotIndex, i) for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM)]

			item.SelectItem(itemVnum)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()
			if itemType != item.ITEM_TYPE_COSTUME and itemSubType != item.COSTUME_TYPE_ACCE:
				return

			absChance = MaxAbs
			itemDesc = item.GetItemDescription()
			self.__AdjustMaxWidth(attrSlot, itemDesc)
			self.__SetItemTitle(itemVnum, metinSlot, attrSlot)
			self.AppendDescription(itemDesc, 26)
			self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
			self.__AppendLimitInformation()

			## ABSORPTION RATE
			if MinAbs == MaxAbs:
				self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ACCEDRAIN_RATE(MinAbs), self.CONDITION_COLOR)
			else:
				self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ACCEDRAIN_RATE(MinAbs) + " ~ %d%%" % (MaxAbs), self.CONDITION_COLOR)
			## END ABSOPRTION RATE

			itemAbsorbedVnum = int(metinSlot[acce.ABSORBED_SOCKET])
			if itemAbsorbedVnum:
				## ATTACK / DEFENCE
				item.SelectItem(itemAbsorbedVnum)
				if item.GetItemType() == item.ITEM_TYPE_WEAPON:
					if item.GetItemSubType() == item.WEAPON_FAN:
						self.__AppendMagicAttackInfo(absChance)
						item.SelectItem(itemAbsorbedVnum)
						self.__AppendAttackPowerInfo(absChance)
					else:
						self.__AppendAttackPowerInfo(absChance)
						item.SelectItem(itemAbsorbedVnum)
						self.__AppendMagicAttackInfo(absChance)
				elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
					defGrade = item.GetValue(1)
					defBonus = item.GetValue(5) * 2
					defGrade = self.CalcAcceValue(defGrade, absChance)
					defBonus = self.CalcAcceValue(defBonus, absChance)

					if defGrade > 0:
						self.AppendSpace(5)
						self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))

					item.SelectItem(itemAbsorbedVnum)
					self.__AppendMagicDefenceInfo(absChance)
				## END ATTACK / DEFENCE

				## EFFECT
				item.SelectItem(itemAbsorbedVnum)
				for i in xrange(item.ITEM_APPLY_MAX_NUM):
					(affectType, affectValue) = item.GetAffect(i)
					affectValue = self.CalcAcceValue(affectValue, absChance)
					affectString = self.__GetAffectString(affectType, affectValue)
					if affectString and affectValue > 0:
						self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))

					item.SelectItem(itemAbsorbedVnum)
				# END EFFECT

				if app.ENABLE_APPLY_RANDOM:
					self.__AppendAcceDefaultItemApplyInformation(apply_random_list, metinSlot[acce.ABSORPTION_SOCKET])

			item.SelectItem(itemVnum)
			## ATTR
			self.__AppendAttributeInformation(attrSlot, MaxAbs)
			# END ATTR

			self.AppendWearableInformation()
			self.ShowToolTip()

		def SetAcceResultAbsItem(self, slotIndex1, slotIndex2, window_type = player.INVENTORY):
			itemVnumAcce = player.GetItemIndex(window_type, slotIndex1)
			itemVnumTarget = player.GetItemIndex(window_type, slotIndex2)
			if not itemVnumAcce or not itemVnumTarget:
				return

			self.ClearToolTip()

			item.SelectItem(itemVnumAcce)
			itemType = item.GetItemType()
			itemSubType = item.GetItemSubType()
			if itemType != item.ITEM_TYPE_COSTUME and itemSubType != item.COSTUME_TYPE_ACCE:
				return

			metinSlot = [player.GetItemMetinSocket(window_type, slotIndex1, i) for i in xrange(player.METIN_SOCKET_MAX_NUM)]
			attrSlot = [player.GetItemAttribute(window_type, slotIndex2, i) for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM)]
			apply_random_list = []
			if app.ENABLE_APPLY_RANDOM:
				apply_random_list = [player.GetItemApplyRandom(window_type, slotIndex2, i) for i in xrange(player.APPLY_RANDOM_SLOT_MAX_NUM)]

			itemDesc = item.GetItemDescription()
			self.__AdjustMaxWidth(attrSlot, itemDesc)
			self.__SetItemTitle(itemVnumAcce, metinSlot, attrSlot)
			self.AppendDescription(itemDesc, 26)
			self.AppendDescription(item.GetItemSummary(), 26, self.CONDITION_COLOR)
			item.SelectItem(itemVnumAcce)
			self.__AppendLimitInformation()

			## ABSORPTION RATE
			self.AppendTextLine(localeInfo.TOOLTIP_APPLY_ACCEDRAIN_RATE(metinSlot[acce.ABSORPTION_SOCKET]), self.CONDITION_COLOR)
			## END ABSOPRTION RATE

			## ATTACK / DEFENCE
			itemAbsorbedVnum = itemVnumTarget
			item.SelectItem(itemAbsorbedVnum)
			if item.GetItemType() == item.ITEM_TYPE_WEAPON:
				if item.GetItemSubType() == item.WEAPON_FAN:
					self.__AppendMagicAttackInfo(metinSlot[acce.ABSORPTION_SOCKET])
					self.__AppendAttackPowerInfo(metinSlot[acce.ABSORPTION_SOCKET])
				else:
					self.__AppendAttackPowerInfo(metinSlot[acce.ABSORPTION_SOCKET])
					self.__AppendMagicAttackInfo(metinSlot[acce.ABSORPTION_SOCKET])
			elif item.GetItemType() == item.ITEM_TYPE_ARMOR:
				defGrade = item.GetValue(1)
				defBonus = item.GetValue(5) * 2
				defGrade = self.CalcAcceValue(defGrade, metinSlot[acce.ABSORPTION_SOCKET])
				defBonus = self.CalcAcceValue(defBonus, metinSlot[acce.ABSORPTION_SOCKET])

				if defGrade > 0:
					self.AppendSpace(5)
					self.AppendTextLine(localeInfo.TOOLTIP_ITEM_DEF_GRADE % (defGrade + defBonus), self.GetChangeTextLineColor(defGrade))

				self.__AppendMagicDefenceInfo(metinSlot[acce.ABSORPTION_SOCKET])
			## END ATTACK / DEFENCE

			## EFFECT
			item.SelectItem(itemAbsorbedVnum)
			for i in xrange(item.ITEM_APPLY_MAX_NUM):
				(affectType, affectValue) = item.GetAffect(i)
				affectValue = self.CalcAcceValue(affectValue, metinSlot[acce.ABSORPTION_SOCKET])
				affectString = self.__GetAffectString(affectType, affectValue)
				if affectString and affectValue > 0:
					self.AppendTextLine(affectString, self.GetChangeTextLineColor(affectValue))
			## END EFFECT

			if app.ENABLE_APPLY_RANDOM:
				self.__AppendAcceDefaultItemApplyInformation(apply_random_list, metinSlot[acce.ABSORPTION_SOCKET])

			## ATTR
			item.SelectItem(itemAbsorbedVnum)
			for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
				type = attrSlot[i][0]
				value = attrSlot[i][1]
				if not value:
					continue

				value = self.CalcAcceValue(value, metinSlot[acce.ABSORPTION_SOCKET])
				affectString = self.__GetAffectString(type, value)
				if affectString and value > 0:
					affectColor = self.__GetAttributeColor(i, value)
					self.AppendTextLine(affectString, affectColor)
			## END ATTR

			## WEARABLE
			item.SelectItem(itemVnumAcce)
			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_WEARABLE_JOB, self.NORMAL_COLOR)

			item.SelectItem(itemVnumAcce)
			flagList = (
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_WARRIOR),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_ASSASSIN),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_SURA),
				not item.IsAntiFlag(item.ITEM_ANTIFLAG_SHAMAN)
			)

			if not app.DISABLE_WOLFMAN_CREATION:
				flagList += (not item.IsAntiFlag(item.ITEM_ANTIFLAG_WOLFMAN),)

			characterNames = ""
			for i in xrange(self.CHARACTER_COUNT):
				name = self.CHARACTER_NAMES[i]
				flag = flagList[i]
				if flag:
					characterNames += " "
					characterNames += name

			textLine = self.AppendTextLine(characterNames, self.NORMAL_COLOR, True)
			textLine.SetFeather()

			item.SelectItem(itemVnumAcce)
			if item.IsAntiFlag(item.ITEM_ANTIFLAG_MALE):
				textLine = self.AppendTextLine(localeInfo.FOR_FEMALE, self.NORMAL_COLOR, True)
				textLine.SetFeather()

			if item.IsAntiFlag(item.ITEM_ANTIFLAG_FEMALE):
				textLine = self.AppendTextLine(localeInfo.FOR_MALE, self.NORMAL_COLOR, True)
				textLine.SetFeather()
			## END WEARABLE

			self.ShowToolTip()

class HyperlinkItemToolTip(ItemToolTip):
	def __init__(self):
		ItemToolTip.__init__(self, isPickable=True)

	def __del__(self):
		ItemToolTip.__del__(self)

	def SetHyperlinkItem(self, tokens):
		import chat

		minTokenCount = chat.HYPER_LINK_ITEM_KEYWORD + chat.HYPER_LINK_ITEM_VNUM + chat.HYPER_LINK_ITEM_FLAGS
		minTokenCount += player.METIN_SOCKET_MAX_NUM

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			minTokenCount += 1

		if app.ENABLE_APPLY_RANDOM:
			minTokenCount += 2 * player.APPLY_RANDOM_SLOT_MAX_NUM

		maxTokenCount = minTokenCount + 2 * player.ATTRIBUTE_SLOT_MAX_NUM

		if tokens and len(tokens) >= minTokenCount and len(tokens) <= maxTokenCount:
			head, vnum, flag = tokens[:chat.HYPER_LINK_ITEM_KEYWORD + chat.HYPER_LINK_ITEM_VNUM + chat.HYPER_LINK_ITEM_FLAGS]
			itemVnum = int(vnum, 16)
			metinSlot = [int(metin, 16) for metin in tokens[chat.HYPER_LINK_ITEM_SOCKET0:chat.HYPER_LINK_ITEM_SOCKET0 + player.METIN_SOCKET_MAX_NUM + 1]]

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				changelookvnum = int(tokens[chat.HYPER_LINK_ITEM_CHANGE_ITEM_VNUM], 16)

			if app.ENABLE_APPLY_RANDOM:
				apply_random = tokens[chat.HYPER_LINK_ITEM_APPLY_RANDOM_TYPE0:chat.HYPER_LINK_ITEM_APPLY_RANDOM_VALUE3 + 1]
				print apply_random
				if apply_random:
					apply_random_list = []

					apply_random.reverse()
					while apply_random:
						key = int(apply_random.pop(), 16)
						if apply_random:
							val = int(apply_random.pop())
							apply_random_list.append((key, val))

					apply_random_list += [(0, 0)] * (player.APPLY_RANDOM_SLOT_MAX_NUM - len(apply_random_list))
				else:
					apply_random_list = [(0, 0)] * player.APPLY_RANDOM_SLOT_MAX_NUM

			rests = tokens[minTokenCount:]
			if rests:
				attrSlot = []

				rests.reverse()
				while rests:
					key = int(rests.pop(), 16)
					if rests:
						val = int(rests.pop())
						attrSlot.append((key, val))

				attrSlot += [(0, 0)] * (player.ATTRIBUTE_SLOT_MAX_NUM - len(attrSlot))
			else:
				attrSlot = [(0, 0)] * player.ATTRIBUTE_SLOT_MAX_NUM

			self.ClearToolTip()
			self.AddItemData(itemVnum, metinSlot, attrSlot, None, 0, 0, player.INVENTORY, -1, None, apply_random_list)

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				self.AppendChangeLookInfoItemVnum(changelookvnum)

			ItemToolTip.OnUpdate(self)

	def OnUpdate(self):
		pass

	def OnMouseLeftButtonDown(self):
		self.Hide()

class SkillToolTip(ToolTip):
	POINT_NAME_DICT = {
		player.LEVEL : localeInfo.SKILL_TOOLTIP_LEVEL,
		player.IQ : localeInfo.SKILL_TOOLTIP_INT,
	}

	SKILL_TOOL_TIP_WIDTH = 200
	PARTY_SKILL_TOOL_TIP_WIDTH = 340

	PARTY_SKILL_EXPERIENCE_AFFECT_LIST = (
		( 2, 2, 10,),
		( 8, 3, 20,),
		(14, 4, 30,),
		(22, 5, 45,),
		(28, 6, 60,),
		(34, 7, 80,),
		(38, 8, 100,),
	)

	PARTY_SKILL_PLUS_GRADE_AFFECT_LIST = (
		( 4, 2, 1, 0,),
		(10, 3, 2, 0,),
		(16, 4, 2, 1,),
		(24, 5, 2, 2,),
	)

	PARTY_SKILL_ATTACKER_AFFECT_LIST = (
		( 36, 3, ),
		( 26, 1, ),
		( 32, 2, ),
	)

	SKILL_GRADE_NAME = {
		player.SKILL_GRADE_MASTER : localeInfo.SKILL_GRADE_NAME_MASTER,
		player.SKILL_GRADE_GRAND_MASTER : localeInfo.SKILL_GRADE_NAME_GRAND_MASTER,
		player.SKILL_GRADE_PERFECT_MASTER : localeInfo.SKILL_GRADE_NAME_PERFECT_MASTER,
	}

	AFFECT_NAME_DICT = {
		"HP" : localeInfo.TOOLTIP_SKILL_AFFECT_ATT_POWER,
		"ATT_GRADE" : localeInfo.TOOLTIP_SKILL_AFFECT_ATT_GRADE,
		"DEF_GRADE" : localeInfo.TOOLTIP_SKILL_AFFECT_DEF_GRADE,
		"ATT_SPEED" : localeInfo.TOOLTIP_SKILL_AFFECT_ATT_SPEED,
		"MOV_SPEED" : localeInfo.TOOLTIP_SKILL_AFFECT_MOV_SPEED,
		"DODGE" : localeInfo.TOOLTIP_SKILL_AFFECT_DODGE,
		"RESIST_NORMAL" : localeInfo.TOOLTIP_SKILL_AFFECT_RESIST_NORMAL,
		"REFLECT_MELEE" : localeInfo.TOOLTIP_SKILL_AFFECT_REFLECT_MELEE,
	}

	AFFECT_APPEND_TEXT_DICT = {
		"DODGE" : "%",
		"RESIST_NORMAL" : "%",
		"REFLECT_MELEE" : "%",
	}

	def __init__(self):
		ToolTip.__init__(self, self.SKILL_TOOL_TIP_WIDTH)
	def __del__(self):
		ToolTip.__del__(self)

	def SetSkill(self, skillIndex, skillLevel = -1):
		if 0 == skillIndex:
			return

		if skill.SKILL_TYPE_GUILD == skill.GetSkillType(skillIndex):
			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			self.AppendDefaultData(skillIndex)
			self.AppendSkillConditionData(skillIndex)
			self.AppendGuildSkillData(skillIndex, skillLevel)

		else:
			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			slotIndex = player.GetSkillSlotIndex(skillIndex)
			skillGrade = player.GetSkillGrade(slotIndex)
			skillLevel = player.GetSkillLevel(slotIndex)
			skillCurrentPercentage = player.GetSkillCurrentEfficientPercentage(slotIndex)
			skillNextPercentage = player.GetSkillNextEfficientPercentage(slotIndex)

			self.AppendDefaultData(skillIndex)
			self.AppendSkillConditionData(skillIndex)
			self.AppendSkillDataNew(slotIndex, skillIndex, skillGrade, skillLevel, skillCurrentPercentage, skillNextPercentage)
			self.AppendSkillRequirement(skillIndex, skillLevel)

		self.ShowToolTip()

	def SetSkillNew(self, slotIndex, skillIndex, skillGrade, skillLevel):
		if 0 == skillIndex:
			return

		if player.SKILL_INDEX_TONGSOL == skillIndex:
			slotIndex = player.GetSkillSlotIndex(skillIndex)
			skillLevel = player.GetSkillLevel(slotIndex)

			self.AppendDefaultData(skillIndex)
			self.__AppendLeaderShipSkillData(skillGrade, skillLevel)

		elif player.SKILL_INDEX_INSIGHT == skillIndex:
			slotIndex = player.GetSkillSlotIndex(skillIndex)
			skillLevel = player.GetSkillLevel(slotIndex)

			self.AppendDefaultData(skillIndex)
			self.__AppendInSightSkillData(skillGrade, skillLevel)

		elif player.SKILL_INDEX_RIDING == skillIndex:
			slotIndex = player.GetSkillSlotIndex(skillIndex)
			self.AppendSupportSkillDefaultData(skillIndex, skillGrade, skillLevel, 30)

		elif player.SKILL_INDEX_SUMMON == skillIndex:
			maxLevel = 10

			self.ClearToolTip()
			self.__SetSkillTitle(skillIndex, skillGrade)

			## Description
			description = skill.GetSkillDescription(skillIndex)
			self.AppendDescription(description, 25)

			if skillLevel == 10:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
				self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (skillLevel * 10), self.NORMAL_COLOR)

			else:
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)
				self.__AppendSummonDescription(skillLevel, self.NORMAL_COLOR)

				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel + 1), self.NEGATIVE_COLOR)
				self.__AppendSummonDescription(skillLevel + 1, self.NEGATIVE_COLOR)

		elif skill.SKILL_TYPE_GUILD == skill.GetSkillType(skillIndex):
			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			self.AppendDefaultData(skillIndex)
			self.AppendSkillConditionData(skillIndex)
			self.AppendGuildSkillData(skillIndex, skillLevel)

		elif player.SKILL_INDEX_ROLE_PROFICIENCY == skillIndex:
			slotIndex = player.GetSkillSlotIndex(skillIndex)
			skillLevel = player.GetSkillLevel(slotIndex)

			self.AppendDefaultData(skillIndex)
			self.__AppendRoleProficiencySkillData(skillGrade, skillLevel)

		else:
			if self.SKILL_TOOL_TIP_WIDTH != self.toolTipWidth:
				self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
				self.ResizeToolTip()

			slotIndex = player.GetSkillSlotIndex(skillIndex)

			skillCurrentPercentage = player.GetSkillCurrentEfficientPercentage(slotIndex)
			skillNextPercentage = player.GetSkillNextEfficientPercentage(slotIndex)

			self.AppendDefaultData(skillIndex, skillGrade)
			self.AppendSkillConditionData(skillIndex)
			self.AppendSkillDataNew(slotIndex, skillIndex, skillGrade, skillLevel, skillCurrentPercentage, skillNextPercentage)
			self.AppendSkillRequirement(skillIndex, skillLevel)

		self.ShowToolTip()

	def __SetSkillTitle(self, skillIndex, skillGrade):
		self.SetTitle(skill.GetSkillName(skillIndex, skillGrade))
		self.__AppendSkillGradeName(skillIndex, skillGrade)

	def __AppendSkillGradeName(self, skillIndex, skillGrade):
		if self.SKILL_GRADE_NAME.has_key(skillGrade):
			self.AppendSpace(5)
			self.AppendTextLine(self.SKILL_GRADE_NAME[skillGrade] % (skill.GetSkillName(skillIndex, 0)), self.CAN_LEVEL_UP_COLOR)

	def SetSkillOnlyName(self, slotIndex, skillIndex, skillGrade):
		if 0 == skillIndex:
			return

		slotIndex = player.GetSkillSlotIndex(skillIndex)

		self.toolTipWidth = self.SKILL_TOOL_TIP_WIDTH
		self.ResizeToolTip()

		self.ClearToolTip()
		self.__SetSkillTitle(skillIndex, skillGrade)
		self.AppendDefaultData(skillIndex, skillGrade)
		self.AppendSkillConditionData(skillIndex)
		self.ShowToolTip()

	def AppendDefaultData(self, skillIndex, skillGrade = 0):
		self.ClearToolTip()
		self.__SetSkillTitle(skillIndex, skillGrade)

		## Level Limit
		levelLimit = skill.GetSkillLevelLimit(skillIndex)
		if levelLimit > 0:
			color = self.NORMAL_COLOR
			if player.GetStatus(player.LEVEL) < levelLimit:
				color = self.NEGATIVE_COLOR

			self.AppendSpace(5)
			self.AppendTextLine(localeInfo.TOOLTIP_ITEM_LIMIT_LEVEL % (levelLimit), color)

		## Description
		description = skill.GetSkillDescription(skillIndex)
		self.AppendDescription(description, 25)

	def AppendSupportSkillDefaultData(self, skillIndex, skillGrade, skillLevel, maxLevel):
		self.ClearToolTip()
		self.__SetSkillTitle(skillIndex, skillGrade)

		if 1 == skillGrade:
			skillLevel += 19
		elif 2 == skillGrade:
			skillLevel += 29
		elif 3 == skillGrade:
			skillLevel = 40

		## Description
		description = skill.GetSkillDescription(skillIndex)
		self.AppendDescription(description, 25)
		self.AppendSpace(5)
		self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_WITH_MAX % (skillLevel, maxLevel), self.NORMAL_COLOR)

	def AppendSkillConditionData(self, skillIndex):
		conditionDataCount = skill.GetSkillConditionDescriptionCount(skillIndex)
		if conditionDataCount > 0:
			self.AppendSpace(5)
			for i in xrange(conditionDataCount):
				self.AppendTextLine(skill.GetSkillConditionDescription(skillIndex, i), self.CONDITION_COLOR)

	def AppendGuildSkillData(self, skillIndex, skillLevel):
		skillMaxLevel = 7
		skillCurrentPercentage = float(skillLevel) / float(skillMaxLevel)
		skillNextPercentage = float(skillLevel+1) / float(skillMaxLevel)
		## Current Level
		if skillLevel > 0:
			if self.HasSkillLevelDescription(skillIndex, skillLevel):
				self.AppendSpace(5)
				if skillLevel == skillMaxLevel:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)

				#####

				for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
					self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillCurrentPercentage), self.ENABLE_COLOR)

				## Cooltime
				coolTime = skill.GetSkillCoolTime(skillIndex, skillCurrentPercentage)
				if coolTime > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_COOL_TIME + str(coolTime), self.ENABLE_COLOR)

				## SP
				needGSP = skill.GetSkillNeedSP(skillIndex, skillCurrentPercentage)
				if needGSP > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_NEED_GSP % (needGSP), self.ENABLE_COLOR)

		## Next Level
		if skillLevel < skillMaxLevel:
			if self.HasSkillLevelDescription(skillIndex, skillLevel+1):
				self.AppendSpace(5)
				self.AppendTextLine(localeInfo.TOOLTIP_NEXT_SKILL_LEVEL_1 % (skillLevel+1, skillMaxLevel), self.DISABLE_COLOR)

				#####

				for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
					self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillNextPercentage), self.DISABLE_COLOR)

				## Cooltime
				coolTime = skill.GetSkillCoolTime(skillIndex, skillNextPercentage)
				if coolTime > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_COOL_TIME + str(coolTime), self.DISABLE_COLOR)

				## SP
				needGSP = skill.GetSkillNeedSP(skillIndex, skillNextPercentage)
				if needGSP > 0:
					self.AppendTextLine(localeInfo.TOOLTIP_NEED_GSP % (needGSP), self.DISABLE_COLOR)

	def AppendSkillDataNew(self, slotIndex, skillIndex, skillGrade, skillLevel, skillCurrentPercentage, skillNextPercentage):
		self.skillMaxLevelStartDict = { 0 : 17, 1 : 7, 2 : 10, }
		self.skillMaxLevelEndDict = { 0 : 20, 1 : 10, 2 : 10, }

		skillLevelUpPoint = 1
		realSkillGrade = player.GetSkillGrade(slotIndex)
		skillMaxLevelStart = self.skillMaxLevelStartDict.get(realSkillGrade, 15)
		skillMaxLevelEnd = self.skillMaxLevelEndDict.get(realSkillGrade, 20)

		## Current Level
		if skillLevel > 0:
			if self.HasSkillLevelDescription(skillIndex, skillLevel):
				self.AppendSpace(5)
				if skillGrade == skill.SKILL_GRADE_COUNT:
					pass
				elif skillLevel == skillMaxLevelEnd:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL_MASTER % (skillLevel), self.NORMAL_COLOR)
				else:
					self.AppendTextLine(localeInfo.TOOLTIP_SKILL_LEVEL % (skillLevel), self.NORMAL_COLOR)
				self.AppendSkillLevelDescriptionNew(skillIndex, skillCurrentPercentage, self.ENABLE_COLOR)

		## Next Level
		if skillGrade != skill.SKILL_GRADE_COUNT:
			if skillLevel < skillMaxLevelEnd:
				if self.HasSkillLevelDescription(skillIndex, skillLevel+skillLevelUpPoint):
					self.AppendSpace(5)
					## HP보강, 관통회피 보조스킬의 경우
					if skillIndex == 141 or skillIndex == 142:
						self.AppendTextLine(localeInfo.TOOLTIP_NEXT_SKILL_LEVEL_3 % (skillLevel+1), self.DISABLE_COLOR)
					else:
						self.AppendTextLine(localeInfo.TOOLTIP_NEXT_SKILL_LEVEL_1 % (skillLevel+1, skillMaxLevelEnd), self.DISABLE_COLOR)
					self.AppendSkillLevelDescriptionNew(skillIndex, skillNextPercentage, self.DISABLE_COLOR)

	def AppendSkillLevelDescriptionNew(self, skillIndex, skillPercentage, color):
		affectDataCount = skill.GetNewAffectDataCount(skillIndex)
		if affectDataCount > 0:
			for i in xrange(affectDataCount):
				type, minValue, maxValue = skill.GetNewAffectData(skillIndex, i, skillPercentage)

				if not self.AFFECT_NAME_DICT.has_key(type):
					continue

				minValue = int(minValue)
				maxValue = int(maxValue)
				affectText = self.AFFECT_NAME_DICT[type]

				if "HP" == type:
					if minValue < 0 and maxValue < 0:
						minValue *= -1
						maxValue *= -1

					else:
						affectText = localeInfo.TOOLTIP_SKILL_AFFECT_HEAL

				affectText += str(minValue)
				if minValue != maxValue:
					affectText += " - " + str(maxValue)
				affectText += self.AFFECT_APPEND_TEXT_DICT.get(type, "")

				#import debugInfo
				#if debugInfo.IsDebugMode():
				#	affectText = "!!" + affectText

				self.AppendTextLine(affectText, color)

		else:
			for i in xrange(skill.GetSkillAffectDescriptionCount(skillIndex)):
				self.AppendTextLine(skill.GetSkillAffectDescription(skillIndex, i, skillPercentage), color)

		## Duration
		duration = skill.GetDuration(skillIndex, skillPercentage)
		if duration > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_SKILL_DURATION % (duration), color)

		## Cooltime
		coolTime = skill.GetSkillCoolTime(skillIndex, skillPercentage)
		if coolTime > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_SKILL_COOL_TIME + str(coolTime), color)

		## SP
		needSP = skill.GetSkillNeedSP(skillIndex, skillPercentage)
		if needSP != 0:
			continuationSP = skill.GetSkillContinuationSP(skillIndex, skillPercentage)

			if skill.IsUseHPSkill(skillIndex):
				self.AppendNeedHP(needSP, continuationSP, color)
			else:
				self.AppendNeedSP(needSP, continuationSP, color)

	def AppendSkillRequirement(self, skillIndex, skillLevel):
		skillMaxLevel = skill.GetSkillMaxLevel(skillIndex)

		if skillLevel >= skillMaxLevel:
			return

		isAppendHorizontalLine = False

		## Requirement
		if skill.IsSkillRequirement(skillIndex):
			if not isAppendHorizontalLine:
				isAppendHorizontalLine = True
				self.AppendHorizontalLine()

			requireSkillName, requireSkillLevel = skill.GetSkillRequirementData(skillIndex)

			color = self.CANNOT_LEVEL_UP_COLOR
			if skill.CheckRequirementSueccess(skillIndex):
				color = self.CAN_LEVEL_UP_COLOR
			self.AppendTextLine(localeInfo.TOOLTIP_REQUIREMENT_SKILL_LEVEL % (requireSkillName, requireSkillLevel), color)

		## Require Stat
		requireStatCount = skill.GetSkillRequireStatCount(skillIndex)
		if requireStatCount > 0:
			for i in xrange(requireStatCount):
				type, level = skill.GetSkillRequireStatData(skillIndex, i)
				if self.POINT_NAME_DICT.has_key(type):
					if not isAppendHorizontalLine:
						isAppendHorizontalLine = True
						self.AppendHorizontalLine()

					name = self.POINT_NAME_DICT[type]
					color = self.CANNOT_LEVEL_UP_COLOR
					if player.GetStatus(type) >= level:
						color = self.CAN_LEVEL_UP_COLOR
					self.AppendTextLine(localeInfo.TOOLTIP_REQUIREMENT_STAT_LEVEL % (name, level), color)

	def HasSkillLevelDescription(self, skillIndex, skillLevel):
		if skill.GetSkillAffectDescriptionCount(skillIndex) > 0:
			return True
		if skill.GetSkillCoolTime(skillIndex, skillLevel) > 0:
			return True
		if skill.GetSkillNeedSP(skillIndex, skillLevel) > 0:
			return True

		return False

	def AppendMasterAffectDescription(self, index, desc, color):
		self.AppendTextLine(desc, color)

	def AppendNextAffectDescription(self, index, desc):
		self.AppendTextLine(desc, self.DISABLE_COLOR)

	def AppendNeedHP(self, needSP, continuationSP, color):
		self.AppendTextLine(localeInfo.TOOLTIP_NEED_HP % (needSP), color)

		if continuationSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_HP_PER_SEC % (continuationSP), color)

	def AppendNeedSP(self, needSP, continuationSP, color):
		if -1 == needSP:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_ALL_SP, color)
		else:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_SP % (needSP), color)

		if continuationSP > 0:
			self.AppendTextLine(localeInfo.TOOLTIP_NEED_SP_PER_SEC % (continuationSP), color)

	def __AppendRoleProficiencySkillData(self, skill_grade, skill_level):
		if 1 == skill_grade:
			skill_level += 19
		elif 2 == skill_grade:
			skill_level += 29
		elif 3 == skill_grade:
			skill_level = 40

		if skill_level <= 0:
			return

		skillIndex = player.SKILL_INDEX_ROLE_PROFICIENCY
		slotIndex = player.GetSkillSlotIndex(skillIndex)

		self.AppendSpace(5)
		self.AutoAppendTextLine(localeInfo.TOOLTIP_ROLE_PROFICIENCY_SKILL_LEVEL % skill_level, self.NORMAL_COLOR)
		self.AutoAppendTextLine(localeInfo.TOOLTIP_ROLE_PROFICIENCY_LEADER_EFFECT % int(skill_level))

		self.AlignHorizonalCenter()

	def __AppendInSightSkillData(self, skill_grade, skill_level):
		if 1 == skill_grade:
			skill_level += 19
		elif 2 == skill_grade:
			skill_level += 29
		elif 3 == skill_grade:
			skill_level = 40

		if skill_level <= 0:
			return

		skillIndex = player.SKILL_INDEX_INSIGHT
		slotIndex = player.GetSkillSlotIndex(skillIndex)
		skillPower = player.GetSkillCurrentEfficientPercentage(slotIndex)

		if localeInfo.IsBRAZIL() or localeInfo.IsNEWCIBN():
			k = skillPower
		else:
			# k = float(skill.GetSkillPowerByLevel(skill_level) ) /100
			k = player.GetSkillLevel(skillIndex) / 100.0

		self.AppendSpace(5)
		self.AutoAppendTextLine(localeInfo.TOOLTIP_INSIGHT_SKILL_LEVEL % skill_level, self.NORMAL_COLOR)

		if skill_level >= 10:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_ATTACKER % int(10 + 60 * k))

		if skill_level >= 20:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_BERSERKER % int(1 + 5 * k))
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_TANKER % int(50 + 1450 * k))

		if skill_level >= 25:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_BUFFER % int(5 + 45 * k))

		if skill_level >= 35:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_SKILL_MASTER % int(25 + 600 * k))

		if skill_level >= 40:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_DEFENDER % int(5 + 30 * k))

		self.AlignHorizonalCenter()

	def __AppendLeaderShipSkillData(self, skill_grade, skill_level):
		if 1 == skill_grade:
			skill_level += 19
		elif 2 == skill_grade:
			skill_level += 29
		elif 3 == skill_grade:
			skill_level = 40

		if skill_level <= 0:
			return

		skillIndex = player.SKILL_INDEX_TONGSOL
		slotIndex = player.GetSkillSlotIndex(skillIndex)
		skillPower = player.GetSkillCurrentEfficientPercentage(slotIndex)

		if localeInfo.IsBRAZIL() or localeInfo.IsNEWCIBN():
			k = skillPower
		else:
			# k = float(skill.GetSkillPowerByLevel(skill_level) ) /100
			k = player.GetSkillLevel(skillIndex) / 100.0

		self.AppendSpace(5)
		self.AutoAppendTextLine(localeInfo.TOOLTIP_PARTY_SKILL_LEVEL % skill_level, self.NORMAL_COLOR)

		if skill_level >= 10:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_ATTACKER % int(10 + 60 * k))

		if skill_level >= 20:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_BERSERKER % int(1 + 5 * k))
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_TANKER % int(50 + 1450 * k))

		if skill_level >= 25:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_BUFFER % int(5 + 45 * k))

		if skill_level >= 35:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_SKILL_MASTER % int(25 + 600 * k))

		if skill_level >= 40:
			self.AutoAppendTextLine(localeInfo.PARTY_SKILL_DEFENDER % int(5 + 30 * k))

		self.AlignHorizonalCenter()

	def __AppendSummonDescription(self, skillLevel, color):
		if skillLevel > 1:
			self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (skillLevel * 10), color)
		elif 1 == skillLevel:
			self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (15), color)
		elif 0 == skillLevel:
			self.AppendTextLine(localeInfo.SKILL_SUMMON_DESCRIPTION % (10), color)

if __name__ == "__main__":
	import app
	import wndMgr
	import systemSetting
	import mouseModule
	import grp
	import ui

	# wndMgr.SetOutlineFlag(True)

	app.SetMouseHandler(mouseModule.mouseController)
	app.SetHairColorEnable(True)
	wndMgr.SetMouseHandler(mouseModule.mouseController)
	wndMgr.SetScreenSize(systemSetting.GetWidth(), systemSetting.GetHeight())
	app.Create("METIN2 CLOSED BETA", systemSetting.GetWidth(), systemSetting.GetHeight(), 1)
	mouseModule.mouseController.Create()

	toolTip = ItemToolTip()
	toolTip.ClearToolTip()
	# toolTip.AppendTextLine("Test")
	desc = "Item descriptions:|increase of width of display to 35 digits per row AND installation of function that the displayed words are not broken up in two parts, but instead if one word is too long to be displayed in this row, this word will start in the next row."
	summ = ""

	toolTip.AddItemData_Offline(10, desc, summ, 0, 0) 
	toolTip.Show()

	app.Loop()
