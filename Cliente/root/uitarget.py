import app
import ui
import player
import net
import wndMgr
import messenger
import guild
import chr
import nonplayer
import localeInfo
import constInfo
import osfInfo

if app.ENABLE_SEND_TARGET_INFO:
	import item
	import uiToolTip

if app.ENABLE_MESSENGER_BLOCK:
	import uiCommon

if app.ENABLE_SEND_TARGET_INFO:
	def HAS_FLAG(value, flag):
		return (value & flag) == flag

class TargetBoard(ui.ThinBoard):
	if app.ENABLE_SEND_TARGET_INFO:
		class InfoBoard(ui.ThinBoard):
			class ItemGrid(object):
				def __init__(self, width, height):
					self.grid = {}
					self.gridWidth = width
					self.gridHeight = height
					self.gridSize = width * height
					self.Clear()

				def __del__(self):
					self.grid = {}
					self.gridWidth = 0
					self.gridHeight = 0
					self.gridSize = 0

				def Clear(self):
					for pos in range(self.gridSize):
						self.grid[pos] = False

				def IsEmpty(self, pos, width, height):
					row = pos / self.gridWidth

					if row + height > self.gridHeight:
						return False

					if pos + width > row * self.gridWidth + self.gridWidth:
						return False

					for y in range(height):
						start = pos + (y * self.gridWidth)
						if self.grid[start] == True:
							return False

						x = 1
						while x < width:
							x =+ 1
							if self.grid[start + x] == True:
								return False

					return True

				def FindBlank(self, width, height):
					if width > self.gridWidth or height > self.gridHeight:
						return -1

					for row in range(self.gridHeight):
						for col in range(self.gridWidth):
							index = row * self.gridWidth + col
							if self.IsEmpty(index, width, height):
								return index

					return -1

				def Put(self, pos, width, height):
					if not self.IsEmpty(pos, width, height):
						return False

					for y in range(height):
						start = pos + (y * self.gridWidth)
						self.grid[start] = True

						x = 1
						while x < width:
							x += 1
							self.grid[start + x] = True

					return True

			EXP_BASE_LVDELTA = [
				1,		# -15 0
				5,		# -14 1
				10,		# -13 2
				20,		# -12 3
				30,		# -11 4
				50,		# -10 5
				70,		# -9 6
				80,		# -8 7
				85,		# -7 8
				90,		# -6 9
				92,		# -5 10
				94,		# -4 11
				96,		# -3 12
				98,		# -2 13
				100,	# -1 14
				100,	# 0 15
				105,	# 1 16
				110,	# 2 17
				115,	# 3 18
				120,	# 4 19
				125,	# 5 20
				130,	# 6 21
				135,	# 7 22
				140,	# 8 23
				145,	# 9 24
				150,	# 10 25
				155,	# 11 26
				160,	# 12 27
				165,	# 13 28
				170,	# 14 29
				180,	# 15 30
			]

			RACE_FLAG_TO_NAME = {
				nonplayer.RACE_FLAG_ANIMAL : localeInfo. TARGET_INFO_RACE_ANIMAL,
				nonplayer.RACE_FLAG_UNDEAD : localeInfo. TARGET_INFO_RACE_UNDEAD,
				nonplayer.RACE_FLAG_DEVIL : localeInfo. TARGET_INFO_RACE_DEVIL,
				nonplayer.RACE_FLAG_HUMAN : localeInfo. TARGET_INFO_RACE_HUMAN,
				nonplayer.RACE_FLAG_ORC : localeInfo. TARGET_INFO_RACE_ORC,
				nonplayer.RACE_FLAG_MILGYO : localeInfo. TARGET_INFO_RACE_MILGYO,
			}

			SUB_RACE_FLAG_TO_NAME = {
				nonplayer.RACE_FLAG_INSECT : localeInfo.TARGET_INFO_RACE_INSECT,
				nonplayer.RACE_FLAG_FIRE : localeInfo.TARGET_INFO_RACE_FIRE,
				nonplayer.RACE_FLAG_ICE : localeInfo.TARGET_INFO_RACE_ICE,
				nonplayer.RACE_FLAG_DESERT : localeInfo.TARGET_INFO_RACE_DESERT,
				nonplayer.RACE_FLAG_ATT_ELEC : localeInfo.TARGET_INFO_RACE_ELEC,
				nonplayer.RACE_FLAG_ATT_FIRE : localeInfo.TARGET_INFO_RACE_FIRE,
				nonplayer.RACE_FLAG_ATT_ICE : localeInfo.TARGET_INFO_RACE_ICE,
				nonplayer.RACE_FLAG_ATT_WIND : localeInfo.TARGET_INFO_RACE_WIND,
				nonplayer.RACE_FLAG_ATT_EARTH : localeInfo.TARGET_INFO_RACE_EARTH,
				nonplayer.RACE_FLAG_ATT_DARK : localeInfo.TARGET_INFO_RACE_DARK,
				nonplayer.RACE_FLAG_ZODIAC : localeInfo.TARGET_INFO_RACE_CZ,
			}

			BOARD_WIDTH = 280

			def __init__(self):
				ui.ThinBoard.__init__(self)

				self.HideCorners(self.LT)
				self.HideCorners(self.RT)
				self.HideLine(self.T)

				self.toolTipItem = uiToolTip.ItemToolTip()

				self.raceVnum = 0

				self.itemGrid = self.ItemGrid(nonplayer.MAX_DROP_INFO_GRID_SLOT_X, nonplayer.MAX_DROP_INFO_GRID_SLOT_Y)
				self.itemList = {}
				self.hasItemList = False

				self.yPos = 0
				self.children = []
				self.scrollPos = 0

				self.SetSize(self.BOARD_WIDTH, 0)

			def __del__(self):
				ui.ThinBoard.__del__(self)
				self.toolTipItem = None
				self.itemGrid = {}

			def __UpdatePosition(self, targetBoard):
				self.SetPosition(targetBoard.GetLeft() + (targetBoard.GetWidth() - self.GetWidth()) / 2, targetBoard.GetBottom() - 17)

			def Open(self, targetBoard, raceVnum):
				self.__LoadInformation(raceVnum)

				self.SetSize(self.BOARD_WIDTH, self.yPos + 10)
				self.__UpdatePosition(targetBoard)

				self.Show()

			def Refresh(self):
				self.__LoadInformation(self.raceVnum)

				self.SetSize(self.BOARD_WIDTH, self.yPos + 10)

			def Close(self):
				if self.toolTipItem:
					self.toolTipItem.HideToolTip()

				if self.hasItemList:
					nonplayer.ClearMonsterDrop()

				self.Hide()

			def __LoadInformation(self, raceVnum):
				self.yPos = 7
				self.children = []
				self.raceVnum = raceVnum

				wndItem = ui.GridSlotWindow()
				wndItem.SetParent(self)
				wndItem.SetPosition(15, 265)
				wndItem.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
				wndItem.SetOverInItemEvent(ui.__mem_func__(self.__SlotOverInItem))
				wndItem.SetOverOutItemEvent(ui.__mem_func__(self.__SlotOverOutItem))
				wndItem.ArrangeSlot(0, nonplayer.MAX_DROP_INFO_GRID_SLOT_X, nonplayer.MAX_DROP_INFO_GRID_SLOT_Y, 32, 32, 0, 0)
				wndItem.RefreshSlot()
				wndItem.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
				wndItem.Hide()
				self.wndItem = wndItem

				scrollBar = ui.ScrollBar()
				scrollBar.SetParent(self)
				scrollBar.SetPosition(250, 265)
				scrollBar.SetScrollBarSize(100)
				scrollBar.SetMiddleBarSize(0.5)
				scrollBar.SetScrollEvent(self.__OnScroll)
				scrollBar.Hide()
				self.scrollBar = scrollBar

				self.hasItemList = False
				self.scrollPos = 0

				self.__OnLoadDefaultInformation(raceVnum)
				self.__OnLoadRaceInformation(raceVnum)
				self.__OnLoadResistInformation(raceVnum)
				self.__OnLoadItemDropInformation(raceVnum)

			def __GetDefaultHitRate(self, raceVnum):
				attacker_dx = nonplayer.GetMonsterDX(raceVnum)
				attacker_level = nonplayer.GetMonsterLevel(raceVnum)

				self_dx = player.GetStatus(player.DX)
				self_level = player.GetStatus(player.LEVEL)

				iARSrc = min(90, (attacker_dx * 4 + attacker_level * 2) / 6)
				iERSrc = min(90, (self_dx * 4 + self_level * 2) / 6)

				fAR = (float(iARSrc) + 210.0) / 300.0
				fER = (float(iERSrc) * 2 + 5) / (float(iERSrc) + 95) * 3.0 / 10.0

				return fAR - fER

			def __OnLoadDefaultInformation(self, raceVnum):
				self.AppendSeperator()
				self.AppendTextLine(localeInfo.TARGET_INFO_MAX_HP % localeInfo.NumberToDecimal(nonplayer.GetMonsterMaxHP(raceVnum)))

				# Calc attack damage
				myLevel = player.GetStatus(player.LEVEL)
				mobLevel = nonplayer.GetMonsterLevel(raceVnum)
				fHitRate = self.__GetDefaultHitRate(raceVnum)
				iDamMin, iDamMax = nonplayer.GetMonsterDamage(raceVnum)
				iDamMin = int((iDamMin + nonplayer.GetMonsterST(raceVnum)) * 2 * fHitRate) + mobLevel * 2
				iDamMax = int((iDamMax + nonplayer.GetMonsterST(raceVnum)) * 2 * fHitRate) + mobLevel * 2
				iDef = player.GetStatus(player.DEF_GRADE) * (100 + player.GetStatus(player.DEF_BONUS)) / 100
				fDamMulti = nonplayer.GetMonsterDamageMultiply(raceVnum)

				#iDamMin = int(max(0, iDamMin - iDef) * fDamMulti)
				#iDamMax = int(max(0, iDamMax - iDef) * fDamMulti)
				iDamMin = int(max(0, iDamMin) * fDamMulti)
				iDamMax = int(max(0, iDamMax) * fDamMulti)

				if iDamMin < 1:
					iDamMin = 1
				if iDamMax < 5:
					iDamMax = 5
				self.AppendTextLine(localeInfo.TARGET_INFO_DAMAGE % (str(iDamMin), str(iDamMax)))

				iExp = 0
				minGold = 0
				maxGold = 0

				if (abs(mobLevel - myLevel) < len(self.EXP_BASE_LVDELTA) / 2):
					idx = min(len(self.EXP_BASE_LVDELTA) - 1, max(0, (myLevel + 15) - myLevel))
					iExp = nonplayer.GetMonsterExp(raceVnum) * self.EXP_BASE_LVDELTA[idx] / 100

					minGold = nonplayer.GetMonsterGoldMin(raceVnum)
					maxGold = nonplayer.GetMonsterGoldMax(raceVnum)

				self.AppendTextLine(localeInfo.TARGET_INFO_EXP % localeInfo.NumberToDecimal(iExp)) # EXP
				self.AppendTextLine(localeInfo.TARGET_INFO_GOLD_MIN_MAX % (localeInfo.NumberToMoneyString(minGold), localeInfo.NumberToMoneyString(maxGold))) # MinGold, MaxGold
				self.AppendTextLine(localeInfo.TARGET_INFO_REGEN_INFO % (nonplayer.GetMonsterRegenPercent(raceVnum), nonplayer.GetMonsterRegenCycle(raceVnum))) # Regen PCT & Cycle

			def __OnLoadRaceInformation(self, raceVnum):
				self.AppendSeperator()

				mainRaceName = localeInfo.TARGET_INFO_NO_RACE
				subRaceName = "" # ,

				for key, value in self.RACE_FLAG_TO_NAME.iteritems():
					if nonplayer.IsMonsterRaceFlag(raceVnum, key):
						mainRaceName = value
						break

				subRaceNameList = []
				for key, value in self.SUB_RACE_FLAG_TO_NAME.iteritems():
					if nonplayer.IsMonsterRaceFlag(raceVnum, key):
						subRaceNameList.append(value)

				for value in list(set(subRaceNameList)):
					subRaceName += value + ", "

				if len(subRaceNameList) > 0:
					subRaceName = subRaceName[:-2]
				else:
					subRaceName = localeInfo.TARGET_INFO_NO_RACE

				if nonplayer.IsMonsterStone(raceVnum):
					mainRaceName = localeInfo.TARGET_INFO_RACE_METIN
					subRaceName = localeInfo.TARGET_INFO_NO_RACE

				self.AppendTextLine(localeInfo.TARGET_INFO_MAINRACE % mainRaceName)
				self.AppendTextLine(localeInfo.TARGET_INFO_SUBRACE % subRaceName)

			def __OnLoadResistInformation(self, raceVnum):
				self.AppendSeperator()
				self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS)
				self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS_LINE0 % (nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_SWORD), nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_TWOHAND), nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_BELL)))
				if not app.DISABLE_WOLFMAN_CREATION:
					self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS_LINE2 % (nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_DAGGER), nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_FAN), nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_BOW), nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_CLAW)))
				else:
					self.AppendTextLine(localeInfo.TARGET_INFO_RESISTS_LINE1 % (nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_DAGGER), nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_FAN), nonplayer.GetMonsterResist(raceVnum, nonplayer.MOB_RESIST_BOW)))

			def __OnLoadItemDropInformation(self, raceVnum):
				self.AppendSeperator()

				if nonplayer.GetMonsterDropCount(raceVnum) > 0:
					self.hasItemList = True
					self.wndItem.Show()
					self.yPos += 100

					self.__OnLoadDrop()
				else:
					self.hasItemList = False
					self.AppendTextLine(localeInfo.TARGET_INFO_NO_ITEM_TEXT)

			def __OnClearItems(self):
				if not self.wndItem:
					return

				if self.itemGrid:
					self.itemGrid.Clear()

				for slotPos in xrange(self.wndItem.GetSlotCount()):
					self.wndItem.ClearSlot(slotPos)
					self.wndItem.ShowSlotBaseImage(slotPos)

			def __OnLoadDrop(self):
				self.__OnClearItems()

				for slotPos in xrange(self.wndItem.GetSlotCount()):
					try:
						globalPos = self.__GetGridGlobalSlotPos(slotPos)
						itemVnum = nonplayer.GetMonsterDropItemVnum(self.raceVnum, globalPos)
						itemCount = nonplayer.GetMonsterDropItemCount(self.raceVnum, globalPos)
						if itemCount <= 1:
							itemCount = 0
					except IndexError:
						return

					item.SelectItem(itemVnum)
					itemIcon = item.GetIconImage()
					(width, height) = item.GetItemSize()

					pos = self.itemGrid.FindBlank(width, height)
					if pos == -1:
						break

					self.itemGrid.Put(pos, width, height)
					self.itemList.update({ pos : [ itemVnum, itemCount] })
					self.wndItem.SetItemSlot(pos, itemVnum, itemCount)

				if nonplayer.GetMonsterDropCount(self.raceVnum) > nonplayer.MAX_DROP_INFO_GRID_SIZE:
					scrollBarHeight = min(float(nonplayer.MAX_DROP_INFO_GRID_SLOT_Y) / float(len(self.itemList)), 1.0)
					self.scrollBar.SetMiddleBarSize(scrollBarHeight)
					self.scrollBar.Show()
				else:
					self.scrollBar.Hide()

				self.wndItem.RefreshSlot()

			def __SlotOverInItem(self, slotPos):
				if self.toolTipItem:
					if self.itemList and self.itemList[slotPos] > 0:
						self.toolTipItem.SetItemToolTip(self.itemList[slotPos][0])

			def __SlotOverOutItem(self):
				if self.toolTipItem:
					self.toolTipItem.HideToolTip()

			def __GetGridGlobalSlotPos(self, slotPos):
				return self.scrollPos + slotPos

			def __OnScroll(self):
				if not self.raceVnum:
					return

				count = nonplayer.GetMonsterDropCount(self.raceVnum)
				self.scrollPos = int(count * self.scrollBar.GetPos())

				self.__OnLoadDrop()

			def OnRunMouseWheel(self, pos):
				if self.scrollBar.IsShow():
					if pos > 0:
						self.scrollBar.OnUp()
					else:
						self.scrollBar.OnDown()

			def AppendTextLine(self, text):
				textLine = ui.TextLine()
				textLine.SetParent(self)
				textLine.SetWindowHorizontalAlignCenter()
				textLine.SetHorizontalAlignCenter()
				textLine.SetText(text)
				textLine.SetPosition(0, self.yPos)
				textLine.Show()

				self.children.append(textLine)
				self.yPos += 17

			def AppendSeperator(self):
				img = ui.ImageBox()
				img.LoadImage("d:/ymir work/ui/pattern/seperator.tga")
				self.AppendWindow(img)
				img.SetPosition(img.GetLeft(), img.GetTop() - 15)
				self.yPos -= 15

			def AppendWindow(self, wnd, x = 0, width = 0, height = 0):
				if width == 0:
					width = wnd.GetWidth()
				if height == 0:
					height = wnd.GetHeight()

				wnd.SetParent(self)
				if x == 0:
					wnd.SetPosition((self.GetWidth() - width) / 2, self.yPos)
				else:
					wnd.SetPosition(x, self.yPos)
				wnd.Show()

				self.children.append(wnd)
				self.yPos += height + 5

	if app.ENABLE_MESSENGER_BLOCK:
		BUTTON_NAME_LIST = [
			localeInfo.TARGET_BUTTON_WHISPER,
			localeInfo.TARGET_BUTTON_EXCHANGE,
			localeInfo.TARGET_BUTTON_FIGHT,
			localeInfo.TARGET_BUTTON_ACCEPT_FIGHT,
			localeInfo.TARGET_BUTTON_AVENGE,
			localeInfo.TARGET_BUTTON_FRIEND,
			localeInfo.TARGET_BUTTON_INVITE_PARTY,
			localeInfo.TARGET_BUTTON_LEAVE_PARTY,
			localeInfo.TARGET_BUTTON_EXCLUDE,
			localeInfo.TARGET_BUTTON_INVITE_GUILD,
			localeInfo.TARGET_BUTTON_DISMOUNT,
			localeInfo.TARGET_BUTTON_EXIT_OBSERVER,
			localeInfo.TARGET_BUTTON_VIEW_EQUIPMENT,
			localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY,
			localeInfo.TARGET_BUTTON_BUILDING_DESTROY,
			localeInfo.TARGET_BUTTON_EMOTION_ALLOW,
			localeInfo.TARGET_BUTTON_BLOCK,
			localeInfo.TARGET_BUTTON_BLOCK_REMOVE,
			"VOTE_BLOCK_CHAT",
		]
	else:
		BUTTON_NAME_LIST = [
			localeInfo.TARGET_BUTTON_WHISPER,
			localeInfo.TARGET_BUTTON_EXCHANGE,
			localeInfo.TARGET_BUTTON_FIGHT,
			localeInfo.TARGET_BUTTON_ACCEPT_FIGHT,
			localeInfo.TARGET_BUTTON_AVENGE,
			localeInfo.TARGET_BUTTON_FRIEND,
			localeInfo.TARGET_BUTTON_INVITE_PARTY,
			localeInfo.TARGET_BUTTON_LEAVE_PARTY,
			localeInfo.TARGET_BUTTON_EXCLUDE,
			localeInfo.TARGET_BUTTON_INVITE_GUILD,
			localeInfo.TARGET_BUTTON_DISMOUNT,
			localeInfo.TARGET_BUTTON_EXIT_OBSERVER,
			localeInfo.TARGET_BUTTON_VIEW_EQUIPMENT,
			localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY,
			localeInfo.TARGET_BUTTON_BUILDING_DESTROY,
			localeInfo.TARGET_BUTTON_EMOTION_ALLOW,
			"VOTE_BLOCK_CHAT",
		]

	GRADE_NAME = {
		nonplayer.PAWN : localeInfo.TARGET_LEVEL_PAWN,
		nonplayer.S_PAWN : localeInfo.TARGET_LEVEL_S_PAWN,
		nonplayer.KNIGHT : localeInfo.TARGET_LEVEL_KNIGHT,
		nonplayer.S_KNIGHT : localeInfo.TARGET_LEVEL_S_KNIGHT,
		nonplayer.BOSS : localeInfo.TARGET_LEVEL_BOSS,
		nonplayer.KING : localeInfo.TARGET_LEVEL_KING,
	}

	EXCHANGE_LIMIT_RANGE = 3000

	if app.ENABLE_ELEMENT_ADD:
		ELEMENT_IMG_PATH = {
			0: 'd:/ymir work/ui/game/12zi/element/elect.sub',
			1: 'd:/ymir work/ui/game/12zi/element/fire.sub',
			2: 'd:/ymir work/ui/game/12zi/element/ice.sub',
			3: 'd:/ymir work/ui/game/12zi/element/wind.sub',
			4: 'd:/ymir work/ui/game/12zi/element/earth.sub',
			5: 'd:/ymir work/ui/game/12zi/element/dark.sub'
		}
		ELEMENT_ENCHANT_ICON_SCALE = 0.8

	def __init__(self):
		ui.ThinBoard.__init__(self)

		if app.ENABLE_MESSENGER_BLOCK:
			self.AddFlag("float")

		name = ui.TextLine()
		name.SetParent(self)
		name.SetDefaultFontName()
		name.SetOutline()
		name.Show()
		self.name = name

		hpGauge = ui.Gauge()
		hpGauge.SetParent(self)
		hpGauge.MakeGauge(130, "red")
		hpGauge.Hide()
		self.hpGauge = hpGauge

		if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
			hpDecimal = ui.TextLine()
			hpDecimal.SetParent(hpGauge)
			hpDecimal.SetDefaultFontName()
			hpDecimal.SetPosition(5, 5)
			hpDecimal.SetOutline()
			hpDecimal.Hide()
			self.hpDecimal = hpDecimal

		if app.ENABLE_SEND_TARGET_INFO:
			infoButton = ui.Button()
			infoButton.SetParent(self)
			infoButton.SetUpVisual("d:/ymir work/ui/pattern/q_mark_01.tga")
			infoButton.SetOverVisual("d:/ymir work/ui/pattern/q_mark_02.tga")
			infoButton.SetDownVisual("d:/ymir work/ui/pattern/q_mark_01.tga")
			infoButton.SetEvent(ui.__mem_func__(self.OnPressedInfoButton))
			infoButton.Hide()
			self.infoButton = infoButton

			infoBoard = self.InfoBoard()
			infoBoard.Hide()
			self.infoBoard = infoBoard

		closeButton = ui.Button()
		closeButton.SetParent(self)
		closeButton.SetUpVisual("d:/ymir work/ui/public/close_button_01.sub")
		closeButton.SetOverVisual("d:/ymir work/ui/public/close_button_02.sub")
		closeButton.SetDownVisual("d:/ymir work/ui/public/close_button_03.sub")
		closeButton.SetPosition(30, 13)

		if localeInfo.IsARABIC():
			hpGauge.SetPosition(55, 17)
			hpGauge.SetWindowHorizontalAlignLeft()
			closeButton.SetWindowHorizontalAlignLeft()
		else:
			hpGauge.SetPosition(175, 17)
			hpGauge.SetWindowHorizontalAlignRight()
			closeButton.SetWindowHorizontalAlignRight()

		closeButton.SetEvent(ui.__mem_func__(self.OnPressedCloseButton))
		closeButton.Show()
		self.closeButton = closeButton

		self.buttonDict = {}
		self.showingButtonList = []
		for buttonName in self.BUTTON_NAME_LIST:
			button = ui.Button()
			button.SetParent(self)

			if localeInfo.IsARABIC():
				button.SetUpVisual("d:/ymir work/ui/public/Small_Button_01.sub")
				button.SetOverVisual("d:/ymir work/ui/public/Small_Button_02.sub")
				button.SetDownVisual("d:/ymir work/ui/public/Small_Button_03.sub")
			else:
				button.SetUpVisual("d:/ymir work/ui/public/small_thin_button_01.sub")
				button.SetOverVisual("d:/ymir work/ui/public/small_thin_button_02.sub")
				button.SetDownVisual("d:/ymir work/ui/public/small_thin_button_03.sub")

			button.SetWindowHorizontalAlignCenter()
			button.SetText(buttonName)
			button.Hide()
			self.buttonDict[buttonName] = button
			self.showingButtonList.append(button)

		self.buttonDict[localeInfo.TARGET_BUTTON_WHISPER].SetEvent(ui.__mem_func__(self.OnWhisper))
		self.buttonDict[localeInfo.TARGET_BUTTON_EXCHANGE].SetEvent(ui.__mem_func__(self.OnExchange))
		self.buttonDict[localeInfo.TARGET_BUTTON_FIGHT].SetEvent(ui.__mem_func__(self.OnPVP))
		self.buttonDict[localeInfo.TARGET_BUTTON_ACCEPT_FIGHT].SetEvent(ui.__mem_func__(self.OnPVP))
		self.buttonDict[localeInfo.TARGET_BUTTON_AVENGE].SetEvent(ui.__mem_func__(self.OnPVP))
		self.buttonDict[localeInfo.TARGET_BUTTON_FRIEND].SetEvent(ui.__mem_func__(self.OnAppendToMessenger))
		self.buttonDict[localeInfo.TARGET_BUTTON_FRIEND].SetEvent(ui.__mem_func__(self.OnAppendToMessenger))
		self.buttonDict[localeInfo.TARGET_BUTTON_INVITE_PARTY].SetEvent(ui.__mem_func__(self.OnPartyInvite))
		self.buttonDict[localeInfo.TARGET_BUTTON_LEAVE_PARTY].SetEvent(ui.__mem_func__(self.OnPartyExit))
		self.buttonDict[localeInfo.TARGET_BUTTON_EXCLUDE].SetEvent(ui.__mem_func__(self.OnPartyRemove))

		self.buttonDict[localeInfo.TARGET_BUTTON_INVITE_GUILD].SAFE_SetEvent(self.__OnGuildAddMember)
		self.buttonDict[localeInfo.TARGET_BUTTON_DISMOUNT].SAFE_SetEvent(self.__OnDismount)
		self.buttonDict[localeInfo.TARGET_BUTTON_EXIT_OBSERVER].SAFE_SetEvent(self.__OnExitObserver)
		self.buttonDict[localeInfo.TARGET_BUTTON_VIEW_EQUIPMENT].SAFE_SetEvent(self.__OnViewEquipment)
		self.buttonDict[localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY].SAFE_SetEvent(self.__OnRequestParty)
		self.buttonDict[localeInfo.TARGET_BUTTON_BUILDING_DESTROY].SAFE_SetEvent(self.__OnDestroyBuilding)
		self.buttonDict[localeInfo.TARGET_BUTTON_EMOTION_ALLOW].SAFE_SetEvent(self.__OnEmotionAllow)
		if app.ENABLE_MESSENGER_BLOCK:
			self.buttonDict[localeInfo.TARGET_BUTTON_BLOCK].SAFE_SetEvent(self.__OnBlock)
			self.buttonDict[localeInfo.TARGET_BUTTON_BLOCK_REMOVE].SAFE_SetEvent(self.__OnBlockRemove)

		self.buttonDict["VOTE_BLOCK_CHAT"].SetEvent(ui.__mem_func__(self.__OnVoteBlockChat))

		self.__Initialize()
		self.ResetTargetBoard()

	def __del__(self):
		ui.ThinBoard.__del__(self)

		print "===================================================== DESTROYED TARGET BOARD"

	def __Initialize(self):
		self.nameString = ""
		self.nameLength = 0
		self.vid = 0
		if app.ENABLE_SEND_TARGET_INFO:
			self.raceVnum = 0

		self.isShowButton = False

		if app.ENABLE_ELEMENT_ADD:
			self.elementImgList = []

		if osfInfo.PAST_HEALTH_ON_TARGET:
			self.oldPercentage = 100
			self.newPercentage = 100

	def Destroy(self):
		self.__Initialize()

		self.name = None
		self.hpGauge = None
		if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
			self.hpDecimal = None

		if app.ENABLE_SEND_TARGET_INFO:
			self.infoButton = None
			if self.infoBoard:
				self.infoBoard.Close()
				self.infoBoard = None

		self.closeButton = None
		self.buttonDict = None
		self.showingButtonList = None

		if app.ENABLE_ELEMENT_ADD:
			self.elementImgList = None

	if app.ENABLE_SEND_TARGET_INFO:
		def RefreshMonsterInfoBoard(self):
			if not self.infoBoard.IsShow():
				return

			self.infoBoard.Refresh()

		def OnPressedInfoButton(self):
			net.SendTargetInfoLoad(player.GetTargetVID())
			if self.infoBoard.IsShow():
				self.infoBoard.Close()
			elif self.raceVnum != 0:
				self.infoBoard.Open(self, self.raceVnum)

	def OnPressedCloseButton(self):
		player.ClearTarget()
		self.Close()

	def Close(self):
		self.__Initialize()

		if app.ENABLE_SEND_TARGET_INFO:
			if self.infoBoard:
				self.infoBoard.Close()

		if app.ENABLE_ELEMENT_ADD:
			self.__HideAllElementImg()

		self.Hide()

	def Open(self, vid, name):
		if vid:
			if not constInfo.GET_VIEW_OTHER_EMPIRE_PLAYER_TARGET_BOARD():
				if not player.IsSameEmpire(vid):
					self.Hide()
					return

			if vid != self.GetTargetVID():
				self.ResetTargetBoard()
				self.SetTargetVID(vid)
				self.SetTargetName(name)

			if player.IsMainCharacterIndex(vid):
				self.__ShowMainCharacterMenu()
			elif chr.INSTANCE_TYPE_BUILDING == chr.GetInstanceType(self.vid):
				self.Hide()
			else:
				self.RefreshButton()
				self.Show()
		else:
			self.HideAllButton()
			self.__ShowButton(localeInfo.TARGET_BUTTON_WHISPER)
			self.__ShowButton("VOTE_BLOCK_CHAT")
			self.__ArrangeButtonPosition()
			self.SetTargetName(name)
			self.Show()

	def Refresh(self):
		if self.IsShow():
			if self.IsShowButton():
				self.RefreshButton()

	def RefreshByVID(self, vid):
		if vid == self.GetTargetVID():
			self.Refresh()

	def RefreshByName(self, name):
		if name == self.GetTargetName():
			self.Refresh()

	def __ShowMainCharacterMenu(self):
		canShow = 0

		self.HideAllButton()

		if player.IsMountingHorse():
			self.__ShowButton(localeInfo.TARGET_BUTTON_DISMOUNT)
			canShow = 1

		if player.IsObserverMode():
			self.__ShowButton(localeInfo.TARGET_BUTTON_EXIT_OBSERVER)
			canShow = 1

		if canShow:
			self.__ArrangeButtonPosition()
			self.Show()
		else:
			self.Hide()

	def __ShowNameOnlyMenu(self):
		self.HideAllButton()

	def SetWhisperEvent(self, event):
		self.eventWhisper = event

	def UpdatePosition(self):
		self.SetPosition(wndMgr.GetScreenWidth() / 2 - self.GetWidth() / 2, 10)
		if app.ENABLE_ELEMENT_ADD:
			self.AdjustPositionElementImage()

	def ResetTargetBoard(self):
		for btn in self.buttonDict.values():
			btn.Hide()

		self.__Initialize()

		self.name.SetPosition(0, 13)
		self.name.SetHorizontalAlignCenter()
		self.name.SetWindowHorizontalAlignCenter()

		self.hpGauge.Hide()
		if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
			self.hpDecimal.Hide()

		if app.ENABLE_SEND_TARGET_INFO:
			self.infoButton.Hide()
			self.infoBoard.Close()

		self.SetSize(250, 40)

	def SetTargetVID(self, vid):
		self.vid = vid
		if app.ENABLE_SEND_TARGET_INFO:
			self.raceVnum = 0

	def SetEnemyVID(self, vid):
		self.SetTargetVID(vid)

		if app.ENABLE_SEND_TARGET_INFO:
			raceVnum = nonplayer.GetRaceNumByVID(vid)
		name = chr.GetNameByVID(vid)
		level = nonplayer.GetLevelByVID(vid)
		grade = nonplayer.GetGradeByVID(vid)

		nameFront = ""
		if -1 != level:
			nameFront += "Lv." + str(level) + " "
		if self.GRADE_NAME.has_key(grade):
			nameFront += "(" + self.GRADE_NAME[grade] + ") "

		self.SetTargetName(nameFront + name)

		if app.ENABLE_SEND_TARGET_INFO:
			if raceVnum >= 101:
				(textWidth, textHeight) = self.name.GetTextSize()

				self.infoButton.SetPosition(textWidth + 25, 12)
				self.infoButton.SetWindowHorizontalAlignLeft()

				self.raceVnum = raceVnum
				self.infoButton.Show()

	def GetTargetVID(self):
		return self.vid

	def GetTargetName(self):
		return self.nameString

	def SetTargetName(self, name):
		self.nameString = name
		self.nameLength = len(name)
		self.name.SetText(name)

	if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
		def SetHP(self, hpPercentage, iMinHP, iMaxHP):
			if not self.hpGauge.IsShow():
				if app.ENABLE_VIEW_TARGET_PLAYER_HP:
					showingButtonCount = len(self.showingButtonList)
					if showingButtonCount < len(self.BUTTON_NAME_LIST):
						if chr.GetInstanceType(self.vid) == chr.INSTANCE_TYPE_PLAYER:
							self.SetSize(max(150 + 75 * 3, showingButtonCount * 75), self.GetHeight())
						else:
							self.SetSize(200 + 7 * self.nameLength, self.GetHeight())
					else:
						self.SetSize(200 + 7 * self.nameLength, self.GetHeight())
				else:
					self.SetSize(200 + 7 * self.nameLength, self.GetHeight())

				if localeInfo.IsARABIC():
					self.name.SetPosition(self.GetWidth() - 23, 13)
				else:
					self.name.SetPosition(23, 13)

				self.name.SetWindowHorizontalAlignLeft()
				self.name.SetHorizontalAlignLeft()
				self.hpGauge.Show()
				self.UpdatePosition()

			self.hpGauge.SetPercentage(hpPercentage, 100)

			if osfInfo.PAST_HEALTH_ON_TARGET:
				self.oldPercentage = float(self.newPercentage)
				self.newPercentage = float(hpPercentage)

				self.hpGauge.SetPercentageBack(self.oldPercentage, 100)

			if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
				iMinHPText = '.'.join([i - 3 < 0 and str(iMinHP)[:i] or str(iMinHP)[i-3:i] for i in range(len(str(iMinHP)) % 3, len(str(iMinHP))+1, 3) if i])
				iMaxHPText = '.'.join([i - 3 < 0 and str(iMaxHP)[:i] or str(iMaxHP)[i-3:i] for i in range(len(str(iMaxHP)) % 3, len(str(iMaxHP))+1, 3) if i])
				self.hpDecimal.SetText(str(iMinHPText) + "/" + str(iMaxHPText))
				(textWidth, textHeight) = self.hpDecimal.GetTextSize()
				if localeInfo.IsARABIC():
					self.hpDecimal.SetPosition(120 / 2 + textWidth / 2, -15)
				else:
					self.hpDecimal.SetPosition(130 / 2 - textWidth / 2, -15)

				self.hpDecimal.Show()
	else:
		def SetHP(self, hpPercentage):
			if not self.hpGauge.IsShow():
				if app.ENABLE_VIEW_TARGET_PLAYER_HP:
					showingButtonCount = len(self.showingButtonList)
					if showingButtonCount > 0:
						if chr.GetInstanceType(self.GetTargetVID) != chr.INSTANCE_TYPE_PLAYER:
							if showingButtonCount != 1:
								self.SetSize(max(150, showingButtonCount * 75), self.GetHeight())
							else:
								self.SetSize(max(150, 2 * 75), self.GetHeight())
						else:
							self.SetSize(200 + 7 * self.nameLength, self.GetHeight())
					else:
						self.SetSize(200 + 7 * self.nameLength, self.GetHeight())
				else:
					self.SetSize(200 + 7 * self.nameLength, self.GetHeight())

				if localeInfo.IsARABIC():
					self.name.SetPosition(self.GetWidth() - 23, 13)
				else:
					self.name.SetPosition(23, 13)

				self.name.SetWindowHorizontalAlignLeft()
				self.name.SetHorizontalAlignLeft()
				self.hpGauge.Show()
				self.UpdatePosition()

			if osfInfo.PAST_HEALTH_ON_TARGET:
				if self.oldPercentage > self.newPercentage:
					self.oldPercentage = self.oldPercentage - 0.5
					self.hpGauge.SetPercentageBack(self.oldPercentage, 100)
				else:
					self.hpGauge.SetPercentage(hpPercentage, 100)
			else:
				self.hpGauge.SetPercentage(hpPercentage, 100)

	def ShowDefaultButton(self):
		self.isShowButton = True
		self.showingButtonList.append(self.buttonDict[localeInfo.TARGET_BUTTON_WHISPER])
		self.showingButtonList.append(self.buttonDict[localeInfo.TARGET_BUTTON_EXCHANGE])
		self.showingButtonList.append(self.buttonDict[localeInfo.TARGET_BUTTON_FIGHT])
		self.showingButtonList.append(self.buttonDict[localeInfo.TARGET_BUTTON_EMOTION_ALLOW])
		for button in self.showingButtonList:
			button.Show()

	def HideAllButton(self):
		self.isShowButton = False
		for button in self.showingButtonList:
			button.Hide()
		self.showingButtonList = []

	def __ShowButton(self, name):
		if not self.buttonDict.has_key(name):
			return

		self.buttonDict[name].Show()
		self.showingButtonList.append(self.buttonDict[name])

	def __HideButton(self, name):
		if not self.buttonDict.has_key(name):
			return

		button = self.buttonDict[name]
		button.Hide()

		for btnInList in self.showingButtonList:
			if btnInList == button:
				self.showingButtonList.remove(button)
				break

	def OnWhisper(self):
		if None != self.eventWhisper:
			self.eventWhisper(self.nameString)

	def OnExchange(self):
		net.SendExchangeStartPacket(self.vid)

	def OnPVP(self):
		net.SendChatPacket("/pvp %d" % (self.vid))

	def OnAppendToMessenger(self):
		net.SendMessengerAddByVIDPacket(self.vid)

	def OnPartyInvite(self):
		net.SendPartyInvitePacket(self.vid)

	def OnPartyExit(self):
		net.SendPartyExitPacket()

	def OnPartyRemove(self):
		pid = player.PartyMemberVIDToPID(self.vid)
		if pid:
			net.SendPartyRemovePacket(pid)

	def __OnGuildAddMember(self):
		net.SendGuildAddMemberPacket(self.vid)

	def __OnDismount(self):
		net.SendChatPacket("/unmount")

	def __OnExitObserver(self):
		net.SendChatPacket("/observer_exit")

	def __OnViewEquipment(self):
		net.SendChatPacket("/view_equip " + str(self.vid))

	def __OnRequestParty(self):
		net.SendChatPacket("/party_request " + str(self.vid))

	def __OnDestroyBuilding(self):
		net.SendChatPacket("/build d %d" % (self.vid))

	def __OnEmotionAllow(self):
		net.SendChatPacket("/emotion_allow %d" % (self.vid))

	def __OnVoteBlockChat(self):
		cmd = "/vote_block_chat %s" % (self.nameString)
		net.SendChatPacket(cmd)

	def OnPressEscapeKey(self):
		self.OnPressedCloseButton()
		return True

	def IsShowButton(self):
		return self.isShowButton

	def RefreshButton(self):
		self.HideAllButton()

		if chr.INSTANCE_TYPE_BUILDING == chr.GetInstanceType(self.vid):
			#self.__ShowButton(localeInfo.TARGET_BUTTON_BUILDING_DESTROY)
			#self.__ArrangeButtonPosition()
			return

		if player.IsPVPInstance(self.vid) or player.IsObserverMode():
			# PVP_INFO_SIZE_BUG_FIX
			self.SetSize(200 + 7 * self.nameLength, 40)
			self.UpdatePosition()
			# END_OF_PVP_INFO_SIZE_BUG_FIX
			return

		self.ShowDefaultButton()

		if guild.MainPlayerHasAuthority(guild.AUTH_ADD_MEMBER):
			if not guild.IsMemberByName(self.nameString):
				if 0 == chr.GetGuildID(self.vid):
					self.__ShowButton(localeInfo.TARGET_BUTTON_INVITE_GUILD)

		if not messenger.IsFriendByName(self.nameString):
			self.__ShowButton(localeInfo.TARGET_BUTTON_FRIEND)

		if app.ENABLE_MESSENGER_BLOCK:
			if not messenger.IsBlockFriendByName(self.nameString):
				self.__ShowButton(localeInfo.TARGET_BUTTON_BLOCK)
			else:
				self.__ShowButton(localeInfo.TARGET_BUTTON_BLOCK_REMOVE)

		if player.IsPartyMember(self.vid):
			self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)

			if player.IsPartyLeader(self.vid):
				self.__ShowButton(localeInfo.TARGET_BUTTON_LEAVE_PARTY)
			elif player.IsPartyLeader(player.GetMainCharacterIndex()):
				self.__ShowButton(localeInfo.TARGET_BUTTON_EXCLUDE)
		else:
			if player.IsPartyMember(player.GetMainCharacterIndex()):
				if player.IsPartyLeader(player.GetMainCharacterIndex()):
					self.__ShowButton(localeInfo.TARGET_BUTTON_INVITE_PARTY)
			else:
				if chr.IsPartyMember(self.vid):
					self.__ShowButton(localeInfo.TARGET_BUTTON_REQUEST_ENTER_PARTY)
				else:
					self.__ShowButton(localeInfo.TARGET_BUTTON_INVITE_PARTY)

			if player.IsRevengeInstance(self.vid):
				self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)
				self.__ShowButton(localeInfo.TARGET_BUTTON_AVENGE)
			elif player.IsChallengeInstance(self.vid):
				self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)
				self.__ShowButton(localeInfo.TARGET_BUTTON_ACCEPT_FIGHT)
			elif player.IsCantFightInstance(self.vid):
				self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)

			if not player.IsSameEmpire(self.vid):
				self.__HideButton(localeInfo.TARGET_BUTTON_INVITE_PARTY)
				self.__HideButton(localeInfo.TARGET_BUTTON_FRIEND)
				self.__HideButton(localeInfo.TARGET_BUTTON_FIGHT)

		distance = player.GetCharacterDistance(self.vid)
		if distance > self.EXCHANGE_LIMIT_RANGE:
			self.__HideButton(localeInfo.TARGET_BUTTON_EXCHANGE)
			self.__ArrangeButtonPosition()

		self.__ArrangeButtonPosition()

	def __ArrangeButtonPosition(self):
		showingButtonCount = len(self.showingButtonList)

		pos = -(showingButtonCount / 2) * 68
		if 0 == showingButtonCount % 2:
			pos += 34

		for button in self.showingButtonList:
			button.SetPosition(pos, 33)
			pos += 68

		if app.ENABLE_VIEW_TARGET_PLAYER_HP:
			if showingButtonCount <= 2:
				self.SetSize(max(150 + 125, showingButtonCount * 75), 65)
			else:
				self.SetSize(max(150, showingButtonCount * 75), 65)
		else:
			self.SetSize(max(150, showingButtonCount * 75), 65)

		self.UpdatePosition()

	def OnUpdate(self):
		if self.isShowButton:
			exchangeButton = self.buttonDict[localeInfo.TARGET_BUTTON_EXCHANGE]
			distance = player.GetCharacterDistance(self.vid)

			if distance < 0:
				if app.WJ_NEW_USER_CARE:
					player.ClearTarget()
					self.Close()
				return

			if exchangeButton.IsShow():
				if distance > self.EXCHANGE_LIMIT_RANGE:
					self.RefreshButton()
			else:
				if distance < self.EXCHANGE_LIMIT_RANGE:
					self.RefreshButton()

	if app.ENABLE_MESSENGER_BLOCK:
		def __OnBlock(self):
			net.SendMessengerBlockAddByVIDPacket(self.vid)

		def __OnBlockRemove(self):
			self.questionDialog = uiCommon.QuestionDialog()
			self.questionDialog.SetText(localeInfo.MESSENGER_DO_YOU_DELETE)
			self.questionDialog.SetAcceptEvent(ui.__mem_func__(self.OnBlockRemove))
			self.questionDialog.SetCancelEvent(ui.__mem_func__(self.OnBlockRemoveClose))
			self.questionDialog.Open()

		def OnBlockRemove(self):
			messenger.RemoveBlock(osfInfo.MESSENGER_KEY)
			net.SendMessengerBlockRemoveByVIDPacket(osfInfo.MESSENGER_KEY, chr.GetNameByVID(self.vid))
			self.OnBlockRemoveClose()

		def OnBlockRemoveClose(self):
			self.questionDialog.Close()
			self.questionDialog = None
			return True

	if app.ENABLE_ELEMENT_ADD:
		def AdjustPositionElementImage(self):
			if not self.elementImgList:
				return

			isShowButton = False
			if self.IsShowButton():
				isShowButton = True

			for button in self.showingButtonList:
				if button.IsShow():
					isShowButton = True
					break

			for element, elementImg in enumerate(self.elementImgList):
				elementImg.SetPosition((32 * element - 1), 65 if isShowButton else 40)
				elementImg.SetScale(self.ELEMENT_ENCHANT_ICON_SCALE, self.ELEMENT_ENCHANT_ICON_SCALE)
				elementImg.Show()

		def ShowElementImg(self, element_enchants_dict):
			if all(value == 0 for value in element_enchants_dict.values()) or not element_enchants_dict:
				self.__HideAllElementImg()
				return

			elementDict = sorted(element_enchants_dict.items(), key = lambda item : item[1])
			elementDict.reverse()

			self.elementImgList = []
			for key, value in elementDict:
				if value <= 0:
					continue

				elementImg = ui.ExpandedImageBox()
				elementImg.SetParent(self)
				elementImg.LoadImage(self.ELEMENT_IMG_PATH[key])
				elementImg.Hide()

				self.elementImgList.append(elementImg)

			self.AdjustPositionElementImage()

		def __HideAllElementImg(self):
			if self.elementImgList:
				for elementImg in self.elementImgList:
					elementImg.Hide()
				self.elementImgList = []

if app.ENABLE_SHIP_DEFENSE:
	class AllianceTargetBoard(ui.ThinBoard):
		class TextToolTip(ui.Window):
			def __init__(self):
				ui.Window.__init__(self, "TOP_MOST")

				textLine = ui.TextLine()
				textLine.SetParent(self)
				textLine.SetHorizontalAlignCenter()
				textLine.SetOutline()
				textLine.Show()
				self.textLine = textLine

			def __del__(self):
				ui.Window.__del__(self)

			def SetText(self, text):
				self.textLine.SetText(text)

			def OnRender(self):
				(mouseX, mouseY) = wndMgr.GetMousePosition()
				self.textLine.SetPosition(mouseX, mouseY + 30)

		def __init__(self):
			ui.ThinBoard.__init__(self)

			name = ui.TextLine()
			name.SetParent(self)
			name.SetDefaultFontName()
			name.SetOutline()
			name.Show()

			hpGauge = ui.Gauge()
			hpGauge.SetParent(self)
			hpGauge.MakeGauge(80, "red")
			hpGauge.SetPosition(10, 25)
			hpGauge.SetOverEvent(ui.__mem_func__(self.IsIn))
			hpGauge.SetOverOutEvent(ui.__mem_func__(self.IsOut))
			hpGauge.Hide()

			self.name = name
			self.hpGauge = hpGauge

			self.toolTipHP = self.TextToolTip()
			self.toolTipHP.Hide()

			self.Initialize()
			self.ResetTargetBoard()

		def __del__(self):
			ui.ThinBoard.__del__(self)

		def Initialize(self):
			self.nameLength = 0
			self.vid = 0

		def Destroy(self):
			self.name = None
			self.hpGauge = None
			self.tooltipHP = None

			self.Initialize()

		def Close(self):
			self.Initialize()
			self.tooltipHP.Hide()
			self.Hide()

		def ResetTargetBoard(self):
			self.Initialize()

			self.name.SetPosition(0, 13)
			self.name.SetHorizontalAlignCenter()
			self.name.SetWindowHorizontalAlignCenter()

			self.hpGauge.Hide()
			self.SetSize(100, 40)

		def SetTargetVID(self, vid):
			self.vid = vid

		def SetTarget(self, vid):
			self.SetTargetVID(vid)

			name = chr.GetNameByVID(vid)
			self.SetTargetName(name)

		def GetTargetVID(self):
			return self.vid

		def SetTargetName(self, name):
			self.nameLength = len(name)
			self.name.SetText(name)

		def SetHP(self, hp, hpMax):
			hp = min(hp, hpMax)
			if hp > 0:
				self.SetSize(100, self.GetHeight())

				if localeInfo.IsARABIC():
					self.name.SetPosition(self.GetWidth() - 10, 10)
				else:
					self.name.SetPosition(10, 10)

				self.name.SetWindowHorizontalAlignLeft()
				self.name.SetHorizontalAlignLeft()
				self.hpGauge.Show()
				self.UpdatePosition()

			self.hpGauge.SetPercentage(hp, hpMax)
			self.toolTipHP.SetText("%s : %d / %d" % (localeInfo.TASKBAR_HP, hp, hpMax))

		def UpdatePosition(self):
			# NOTE : y = miniMap + serverInfo Height
			self.SetPosition(wndMgr.GetScreenWidth() - self.GetWidth() - 18, 250)

		def IsOut(self):
			if self.toolTipHP:
				self.toolTipHP.Hide()

		def IsIn(self):
			if self.toolTipHP:
				self.toolTipHP.Show()

		# NOTE : Unused.
		def SetMouseEvent(self):
			pass
