import ui
import net
import item
import skill
import localeInfo
import wndMgr
import player
import constInfo
import osfInfo
import mouseModule
import uiScriptLocale
import app
import guild

MOUSE_SETTINGS = [0, 0]

def InitMouseButtonSettings(left, right):
	global MOUSE_SETTINGS
	MOUSE_SETTINGS = [left, right]

def SetMouseButtonSetting(dir, event):
	global MOUSE_SETTINGS
	MOUSE_SETTINGS[dir] = event

def GetMouseButtonSettings():
	global MOUSE_SETTINGS
	return MOUSE_SETTINGS

def SaveMouseButtonSettings():
	global MOUSE_SETTINGS
	open("mouse.cfg", "w").write("%s\t%s" % tuple(MOUSE_SETTINGS))

def LoadMouseButtonSettings():
	global MOUSE_SETTINGS
	tokens = open("mouse.cfg", "r").read().split()

	if len(tokens) != 2:
		raise RuntimeError, "MOUSE_SETTINGS_FILE_ERROR"

	MOUSE_SETTINGS[0] = int(tokens[0])
	MOUSE_SETTINGS[1] = int(tokens[1])

def unsigned32(n):
	return n & 0xFFFFFFFFL

#-------------------Giftbox Begin------------------------------

class GiftBox(ui.ScriptWindow):
	class TextToolTip(ui.Window):
		def __init__(self):
			ui.Window.__init__(self, "TOP_MOST")
			self.SetWindowName("GiftBox")
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
			self.textLine.SetPosition(mouseX, mouseY - 15)

	def __init__(self):
		# print "NEW TASKBAR -----------------------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)
		self.tooltipGift = self.TextToolTip()
		self.tooltipGift.Show()

	def __del__(self):
		# print "---------------------------------------------------------------------------- DELETE TASKBAR"
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "giftbox.py")
		except:
			import exception
			exception.Abort("GiftBox.LoadWindow.LoadObject")

		self.giftBoxIcon = self.GetChild("GiftBox_Icon")
		self.giftBoxIcon.SetEvent(ui.__mem_func__(self.OpenMall), "mouse_click")
		self.giftBoxToolTip = self.GetChild("GiftBox_ToolTip")

	def Destroy(self):
		self.giftBoxIcon = None
		self.giftBoxToolTip = None

	def OpenMall(self):
		net.SendChatPacket("/click_mall")
		self.Hide()

#-------------------Giftbox End------------------------------

class EnergyBar(ui.ScriptWindow):
	class TextToolTip(ui.Window):
		def __init__(self):
			ui.Window.__init__(self, "TOP_MOST")
			self.SetWindowName("EnergyBar")
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
			self.textLine.SetPosition(mouseX, mouseY - 15)

	def __init__(self):
		#print "NEW TASKBAR -----------------------------------------------------------------------------"
		ui.ScriptWindow.__init__(self)
		self.tooltipEnergy = self.TextToolTip()
		self.tooltipEnergy.Show()

	def __del__(self):
		#print "---------------------------------------------------------------------------- DELETE TASKBAR"
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "EnergyBar.py")
		except:
			import exception
			exception.Abort("EnergyBar.LoadWindow.LoadObject")

		self.energyEmpty = self.GetChild("EnergyGauge_Empty")
		self.energyHungry = self.GetChild("EnergyGauge_Hungry")
		self.energyFull = self.GetChild("EnergyGauge_Full")

		self.energyGaugeBoard = self.GetChild("EnergyGauge_Board")
		self.energyGaugeToolTip = self.GetChild("EnergyGauge_ToolTip")

	def Destroy(self):
		self.tooltipEnergy = None
		self.energyEmpty = None
		self.energyHungry = None
		self.energyFull = None

		self.energyGaugeBoard = None
		self.energyGaugeToolTip = None

	## Gauge
	def RefreshStatus(self):
		pointEnergy = player.GetStatus(player.ENERGY)
		if app.ENABLE_EXTENDED_BLEND_AFFECT:
			if pointEnergy != 0:
				leftTimeEnergy = 7200
			else:
				leftTimeEnergy = player.GetStatus(player.ENERGY_END_TIME) - app.GetGlobalTimeStamp()
		else:
			leftTimeEnergy = player.GetStatus(player.ENERGY_END_TIME) - app.GetGlobalTimeStamp()
		# ���ȯ ���� �ð� = 2�ð�.
		self.SetEnergy(pointEnergy, leftTimeEnergy, 7200)

	def SetEnergy(self, point, leftTime, maxTime):
		leftTime = max(leftTime, 0)
		maxTime = max(maxTime, 0)

		self.energyEmpty.Hide()
		self.energyHungry.Hide()
		self.energyFull.Hide()

		if leftTime == 0:
			self.energyEmpty.Show()
		elif ((leftTime * 100) / maxTime) < 15:
			self.energyHungry.Show()
		else:
			self.energyFull.Show()

		self.tooltipEnergy.SetText("%s" % (localeInfo.TOOLTIP_ENERGY(point)))

	def OnUpdate(self):
		if True == self.energyGaugeToolTip.IsIn():
			self.RefreshStatus()
			self.tooltipEnergy.Show()
		else:
			self.tooltipEnergy.Hide()

if app.ENABLE_GEM_SYSTEM:
	class ExpandedMoneyTaskBar(ui.ScriptWindow):
		def __init__(self):
			ui.Window.__init__(self)

			self.wndMoney = None
			self.wndMoneySlot = None
			self.wndMoneyIcon = None

			if app.ENABLE_CHEQUE_SYSTEM:
				self.wndCheque = None
				self.wndChequeSlot = None
				self.wndChequeIcon = None
			if app.ENABLE_GEM_SYSTEM:
				self.wndGem = None
				self.wndGemSlot = None
				self.wndGemIcon = None

		def __del__(self):
			ui.Window.__del__(self)

		def Destroy(self):
			self.wndMoney = None
			self.wndMoneySlot = None
			self.wndMoneyIcon = None
			if app.ENABLE_CHEQUE_SYSTEM:
				self.wndCheque = None
				self.wndChequeSlot = None
				self.wndChequeIcon = None
			if app.ENABLE_GEM_SYSTEM:
				self.wndGem = None
				self.wndGemSlot = None
				self.wndGemIcon = None

		def LoadWindow(self):
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "UIScript/ExpandedMoneyTaskbar.py")

				self.wndMoney = self.GetChild("Money")
				self.wndMoneyIcon = self.GetChild("Money_Icon")
				self.wndMoneySlot = self.GetChild("Money_Slot")

				if app.ENABLE_CHEQUE_SYSTEM:
					self.wndCheque = self.GetChild("Cheque")
					self.wndChequeIcon = self.GetChild("Cheque_Icon")
					self.wndChequeSlot = self.GetChild("Cheque_Slot")

				if app.ENABLE_GEM_SYSTEM:
					self.wndGem = self.GetChild("Gem")
					self.wndGemIcon = self.GetChild("Gem_Icon")
					self.wndGemSlot = self.GetChild("Gem_Slot")

			except:
				import exception
				exception.Abort("ExpandedMoneyTaskBar.LoadWindow.LoadObject")

		def GetMoneySlot(self):
			if self.wndMoneySlot:
				return self.wndMoneySlot
			else:
				return None

		def GetMoney(self):
			if self.wndMoney:
				return self.wndMoney
			else:
				return None

		def GetMoneyIcon(self):
			if self.wndMoneyIcon:
				return self.wndMoneyIcon
			else:
				return None

		if app.ENABLE_CHEQUE_SYSTEM:
			def GetCheque(self):
				if self.wndCheque:
					return self.wndCheque
				else:
					return None

			def GetChequeSlot(self):
				if self.wndChequeSlot:
					return self.wndChequeSlot
				else:
					return None

			def GetChequeIcon(self):
				if self.wndChequeIcon:
					return self.wndChequeIcon
				else:
					return None

		if app.ENABLE_GEM_SYSTEM:
			def GetGem(self):
				if self.wndGem:
					return self.wndGem
				else:
					return None

			def GetGemSlot(self):
				if self.wndGemSlot:
					return self.wndGemSlot
				else:
					return None

			def GetGemIcon(self):
				if self.wndGemIcon:
					return self.wndGemIcon
				else:
					return None

##		def SetTop(self):
##			super(ExpandedTaskBar, self).SetTop()
##			for button in self.toggleButtonDict.values():
##				button.SetTop()

		def Show(self):
			self.SetTop()
			ui.ScriptWindow.Show(self)

		def Close(self):
			self.Hide()

		def OnPressEscapeKey(self):
			self.Close()
			return True

class ExpandedTaskBar(ui.ScriptWindow):
	BUTTON_DRAGON_SOUL = 0

	if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
		BUTTON_SPECIAL_INVENTORY = 1

	def __init__(self):
		ui.Window.__init__(self)
		self.SetWindowName("ExpandedTaskBar")

		self.exclusion_list = []

	def __del__(self):
		ui.Window.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "ExpandedTaskBar.py")
		except:
			import exception
			exception.Abort("ExpandedTaskBar.LoadWindow.LoadObject")

		self.expandedTaskBarBoard = self.GetChild("ExpanedTaskBar_Board")

		self.toggleButtonDict = {}
		self.toggleButtonDict[ExpandedTaskBar.BUTTON_DRAGON_SOUL] = self.GetChild("DragonSoulButton")
		self.toggleButtonDict[ExpandedTaskBar.BUTTON_DRAGON_SOUL].SetParent(self)

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.toggleButtonDict[ExpandedTaskBar.BUTTON_SPECIAL_INVENTORY] = self.GetChild("SpecialInventoryButton")
			self.toggleButtonDict[ExpandedTaskBar.BUTTON_SPECIAL_INVENTORY].SetParent(self)

		# ��ư�� ��ġ�� �ٽ� ������ �ִ� �Լ�
		# �������� ȣ�� �Ǿ�� �Ѵ�.
		self.RePositionButton()

	## �ػ󵵸� ��� ������ ��ư�� ������ ��쿡 ���ؼ��� ����ó�� ���� �ʾ���.
	## ��ư�� �ʹ� ������ ���� ���ο� ������ �ʿ��ϴٰ� ������.
	def RePositionButton(self):
		# ������ ������� append �� �ش�
		button_order = []
		if not ExpandedTaskBar.BUTTON_DRAGON_SOUL in self.exclusion_list:
			button_order.append(ExpandedTaskBar.BUTTON_DRAGON_SOUL)

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if not ExpandedTaskBar.BUTTON_SPECIAL_INVENTORY in self.exclusion_list:
				button_order.append(ExpandedTaskBar.BUTTON_SPECIAL_INVENTORY)

		## �θ� window �� position �� �ٽ� ���
		total_count = len( button_order ) # ������ �� ����
		quotient = total_count /2 # ��
		remainder = total_count % 2 # ������

		window_x = (wndMgr.GetScreenWidth()/2 + 33) + (-36 * quotient)
		if 0 == remainder: ## ¦���϶��� �߰� ��ġ ������ ���ش�
			window_x = window_x + 18

		window_y = wndMgr.GetScreenHeight() - 74
		self.SetPosition( window_x, window_y )

		## ��ư���� ��ġ�� �ٽ� ����Ѵ�.
		pos_x = 0
		pos_y = 0
		for key in button_order:
			if not self.toggleButtonDict[key]:
				continue

			if key in self.exclusion_list:
				continue

			self.toggleButtonDict[key].SetPosition( pos_x, pos_y)
			pos_x = pos_x + 36

	def SetTop(self):
		super(ExpandedTaskBar, self).SetTop()
		for button in self.toggleButtonDict.values():
			button.SetTop()

	def Show(self):
		ui.ScriptWindow.Show(self)

	def Close(self):
		self.Hide()

	def SetToolTipText(self, eButton, text):
		self.toggleButtonDict[eButton].SetToolTipText(text)

	def SetToggleButtonEvent(self, eButton, kEventFunc):
		self.toggleButtonDict[eButton].SetEvent(kEventFunc)

	def OnPressEscapeKey(self):
		self.Close()
		return True

class TaskBar(ui.ScriptWindow):
	BUTTON_CHARACTER = 0
	BUTTON_INVENTORY = 1
	BUTTON_MESSENGER = 2
	BUTTON_SYSTEM = 3
	BUTTON_CHAT = 4
	BUTTON_EXPAND = 4
	if app.ENABLE_GEM_SYSTEM:
		BUTTON_EXPAND_MONEY = 5
	IS_EXPANDED = False

	MOUSE_BUTTON_LEFT = 0
	MOUSE_BUTTON_RIGHT = 1
	NONE = 255

	EVENT_MOVE = 0
	EVENT_ATTACK = 1
	EVENT_MOVE_AND_ATTACK = 2
	EVENT_CAMERA = 3
	EVENT_SKILL = 4
	EVENT_AUTO = 5

	GAUGE_WIDTH = 95
	GAUGE_HEIGHT = 13

	QUICKPAGE_NUMBER_FILENAME = [
		"d:/ymir work/ui/game/taskbar/1.sub",
		"d:/ymir work/ui/game/taskbar/2.sub",
		"d:/ymir work/ui/game/taskbar/3.sub",
		"d:/ymir work/ui/game/taskbar/4.sub",
	]

	# Gift icon show and hide
	def ShowGift(self):
		if not localeInfo.IsBRAZIL():
			self.wndGiftBox.Show()

	def HideGift(self):
		self.wndGiftBox.Hide()

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
			self.textLine = None

		def SetText(self, text):
			self.textLine.SetText(text)

		def OnRender(self):
			(mouseX, mouseY) = wndMgr.GetMousePosition()
			self.textLine.SetPosition(mouseX, mouseY - 15)

	class SkillButton(ui.SlotWindow):
		def __init__(self):
			ui.SlotWindow.__init__(self)

			self.event = None
			self.arg = None

			self.slotIndex = 0
			self.skillIndex = 0

			slotIndex = 0
			wndMgr.SetSlotBaseImage(self.hWnd, "d:/ymir work/ui/public/slot_base.sub", 1.0, 1.0, 1.0, 1.0)
			wndMgr.AppendSlot(self.hWnd, slotIndex, 0, 0, 32, 32)
			self.SetCoverButton(slotIndex, "d:/ymir work/ui/public/slot_cover_button_01.sub",\
				"d:/ymir work/ui/public/slot_cover_button_02.sub",\
				"d:/ymir work/ui/public/slot_cover_button_03.sub",\
				"d:/ymir work/ui/public/slot_cover_button_04.sub", True, False)
			self.SetSize(32, 32)

		def __del__(self):
			ui.SlotWindow.__del__(self)

		def Destroy(self):
			if 0 != self.tooltipSkill:
				self.tooltipSkill.HideToolTip()

		def RefreshSkill(self):
			if 0 != self.slotIndex:
				self.SetSkill(self.slotIndex)

		def SetSkillToolTip(self, tooltip):
			self.tooltipSkill = tooltip

		def SetSkill(self, skillSlotNumber):
			slotNumber = 0
			skillIndex = player.GetSkillIndex(skillSlotNumber)
			skillGrade = player.GetSkillGrade(skillSlotNumber)
			skillLevel = player.GetSkillLevel(skillSlotNumber)
			skillType = skill.GetSkillType(skillIndex)

			self.skillIndex = skillIndex
			if 0 == self.skillIndex:
				self.ClearSlot(slotNumber)
				return

			self.slotIndex = skillSlotNumber

			self.SetSkillSlotNew(slotNumber, skillIndex, skillGrade, skillLevel)
			self.SetSlotCountNew(slotNumber, skillGrade, skillLevel)

			## NOTE : CoolTime üũ
			if player.IsSkillCoolTime(skillSlotNumber):
				(coolTime, elapsedTime) = player.GetSkillCoolTime(skillSlotNumber)
				self.SetSlotCoolTime(slotNumber, coolTime, elapsedTime)

			## NOTE : Activate �Ǿ� �ִٸ� �����ܵ� ������Ʈ
			if player.IsSkillActive(skillSlotNumber):
				self.ActivateSlot(slotNumber)

		def SetSkillEvent(self, event, arg=0):
			self.event = event
			self.arg = arg

		def GetSkillIndex(self):
			return self.skillIndex

		def GetSlotIndex(self):
			return self.slotIndex

		def Activate(self, coolTime):
			self.SetSlotCoolTime(0, coolTime)

			if skill.IsToggleSkill(self.skillIndex):
				self.ActivateSlot(0)

		def Deactivate(self):
			if skill.IsToggleSkill(self.skillIndex):
				self.DeactivateSlot(0)

		def OnOverInItem(self, dummy):
			self.tooltipSkill.SetSkill(self.skillIndex)

		def OnOverOutItem(self):
			self.tooltipSkill.HideToolTip()

		def OnSelectItemSlot(self, dummy):
			if 0 != self.event:
				if 0 != self.arg:
					self.event(self.arg)
				else:
					self.event()

	def __init__(self):
		#print "NEW TASKBAR -----------------------------------------------------------------------------"

		ui.ScriptWindow.__init__(self, "TOP_MOST")

		self.quickPageNumImageBox = None
		self.tooltipItem = None
		self.tooltipSkill = None
		self.mouseModeButtonList = [ ui.ScriptWindow("TOP_MOST"), ui.ScriptWindow("TOP_MOST") ]

		if app.ENABLE_HEART_BEAT_MALL:
			self.heartBeat = False

		self.tooltipHP = self.TextToolTip()
		self.tooltipHP.Show()
		self.tooltipSP = self.TextToolTip()
		self.tooltipSP.Show()
		self.tooltipST = self.TextToolTip()
		self.tooltipST.Show()
		self.tooltipEXP = self.TextToolTip()
		self.tooltipEXP.Show()

		self.skillCategoryNameList = [ "ACTIVE_1", "ACTIVE_2", "ACTIVE_3" ]
		self.skillPageStartSlotIndexDict = {
			"ACTIVE_1" : 1,
			"ACTIVE_2" : 21,
			"ACTIVE_3" : 41,
		}

		self.selectSkillButtonList = []

		self.lastUpdateQuickSlot = 0
		self.SetWindowName("TaskBar")

	def __del__(self):
		#print "---------------------------------------------------------------------------- DELETE TASKBAR"
		ui.ScriptWindow.__del__(self)

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()

			if constInfo.IN_GAME_SHOP_ENABLE:
				if app.ENABLE_GEM_SYSTEM:
					pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "newtaskbar.py")
				else:
					pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "TaskBar.py")
			else:
				if app.ENABLE_GEM_SYSTEM:
					pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "newtaskbar.py")
				else:
					pyScrLoader.LoadScriptFile(self, "UIScript/TaskBar.py")

			pyScrLoader.LoadScriptFile(self.mouseModeButtonList[self.MOUSE_BUTTON_LEFT], "UIScript/MouseButtonWindow.py")
			pyScrLoader.LoadScriptFile(self.mouseModeButtonList[self.MOUSE_BUTTON_RIGHT], "UIScript/RightMouseButtonWindow.py")
		except:
			import exception
			exception.Abort("TaskBar.LoadWindow.LoadObject")

		self.quickslot = []
		self.quickslot.append(self.GetChild("quick_slot_1"))
		self.quickslot.append(self.GetChild("quick_slot_2"))
		for slot in self.quickslot:
			slot.SetSlotStyle(wndMgr.SLOT_STYLE_NONE)
			slot.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptyQuickSlot))
			slot.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemQuickSlot))
			slot.SetUnselectItemSlotEvent(ui.__mem_func__(self.UnselectItemQuickSlot))
			slot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			slot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))

		toggleButtonDict = {}
		toggleButtonDict[TaskBar.BUTTON_CHARACTER] = self.GetChild("CharacterButton")
		toggleButtonDict[TaskBar.BUTTON_INVENTORY] = self.GetChild("InventoryButton")
		toggleButtonDict[TaskBar.BUTTON_MESSENGER] = self.GetChild("MessengerButton")
		toggleButtonDict[TaskBar.BUTTON_SYSTEM] = self.GetChild("SystemButton")

		# ChatButton, ExpandButton �� �� �ϳ��� �ݵ�� �����Ѵ�.
		try:
			toggleButtonDict[TaskBar.BUTTON_CHAT]=self.GetChild("ChatButton")
		except:
			toggleButtonDict[TaskBar.BUTTON_EXPAND]=self.GetChild("ExpandButton")
			TaskBar.IS_EXPANDED = True

		if app.ENABLE_GEM_SYSTEM:
			toggleButtonDict[TaskBar.BUTTON_EXPAND_MONEY]=self.GetChild("ExpandMoneyButton")

		if localeInfo.IsARABIC():
			systemButton = toggleButtonDict[TaskBar.BUTTON_SYSTEM]
			if systemButton.ToolTipText:
				tx, ty = systemButton.ToolTipText.GetLocalPosition()
				tw = systemButton.ToolTipText.GetWidth() 
				systemButton.ToolTipText.SetPosition(-tw/2, ty)

		expGauge = []
		expGauge.append(self.GetChild("EXPGauge_01"))
		expGauge.append(self.GetChild("EXPGauge_02"))
		expGauge.append(self.GetChild("EXPGauge_03"))
		expGauge.append(self.GetChild("EXPGauge_04"))

		for exp in expGauge:
			exp.SetSize(0, 0)

		if app.ENABLE_CONQUEROR_LEVEL:
			newExpGauge = []
			newExpGauge.append(self.GetChild("NewExpGauge_01"))
			newExpGauge.append(self.GetChild("NewExpGauge_02"))
			newExpGauge.append(self.GetChild("NewExpGauge_03"))
			newExpGauge.append(self.GetChild("NewExpGauge_04"))

			for newExp in newExpGauge:
				newExp.SetSize(0, 0)
				newExp.Hide()

		self.quickPageNumImageBox=self.GetChild("QuickPageNumber")

		self.GetChild("QuickPageUpButton").SetEvent(ui.__mem_func__(self.__OnClickQuickPageUpButton))
		self.GetChild("QuickPageDownButton").SetEvent(ui.__mem_func__(self.__OnClickQuickPageDownButton))

		mouseLeftButtonModeButton = self.GetChild("LeftMouseButton")
		mouseRightButtonModeButton = self.GetChild("RightMouseButton")
		mouseLeftButtonModeButton.SetEvent(ui.__mem_func__(self.ToggleLeftMouseButtonModeWindow))
		mouseRightButtonModeButton.SetEvent(ui.__mem_func__(self.ToggleRightMouseButtonModeWindow))
		self.curMouseModeButton = [ mouseLeftButtonModeButton, mouseRightButtonModeButton ]

		(xLocalRight, yLocalRight) = mouseRightButtonModeButton.GetLocalPosition()
		self.curSkillButton = self.SkillButton()
		self.curSkillButton.SetParent(self)
		self.curSkillButton.SetPosition(xLocalRight, 3)
		self.curSkillButton.SetSkillEvent(ui.__mem_func__(self.ToggleRightMouseButtonModeWindow))
		self.curSkillButton.Hide()

		(xLeft, yLeft) = mouseLeftButtonModeButton.GetGlobalPosition()
		(xRight, yRight) = mouseRightButtonModeButton.GetGlobalPosition()
		leftModeButtonList = self.mouseModeButtonList[self.MOUSE_BUTTON_LEFT]
		leftModeButtonList.SetPosition(xLeft, yLeft - leftModeButtonList.GetHeight()-5)
		rightModeButtonList = self.mouseModeButtonList[self.MOUSE_BUTTON_RIGHT]
		rightModeButtonList.SetPosition(xRight - rightModeButtonList.GetWidth() + 32, yRight - rightModeButtonList.GetHeight()-5)
		rightModeButtonList.GetChild("button_skill").SetEvent(lambda adir=self.MOUSE_BUTTON_RIGHT, aevent=self.EVENT_SKILL: self.SelectMouseButtonEvent(adir, aevent))
		rightModeButtonList.GetChild("button_skill").Hide()

		mouseImage = ui.ImageBox("TOP_MOST")
		mouseImage.AddFlag("float")
		mouseImage.LoadImage("d:/ymir work/ui/game/taskbar/mouse_button_camera_01.sub")
		mouseImage.SetPosition(xRight, wndMgr.GetScreenHeight() - 34)
		mouseImage.Hide()
		self.mouseImage = mouseImage

		dir = self.MOUSE_BUTTON_LEFT
		wnd = self.mouseModeButtonList[dir]
		wnd.GetChild("button_move_and_attack").SetEvent(lambda adir=dir, aevent=self.EVENT_MOVE_AND_ATTACK: self.SelectMouseButtonEvent(adir, aevent))
		wnd.GetChild("button_auto_attack").SetEvent(lambda adir=dir, aevent=self.EVENT_AUTO: self.SelectMouseButtonEvent(adir, aevent))
		wnd.GetChild("button_camera").SetEvent(lambda adir=dir, aevent=self.EVENT_CAMERA: self.SelectMouseButtonEvent(adir, aevent))

		dir = self.MOUSE_BUTTON_RIGHT
		wnd = self.mouseModeButtonList[dir]
		wnd.GetChild("button_move_and_attack").SetEvent(lambda adir=dir, aevent=self.EVENT_MOVE_AND_ATTACK: self.SelectMouseButtonEvent(adir, aevent))
		wnd.GetChild("button_camera").SetEvent(lambda adir=dir, aevent=self.EVENT_CAMERA: self.SelectMouseButtonEvent(adir, aevent))

		self.toggleButtonDict = toggleButtonDict
		self.expGauge = expGauge
		if app.ENABLE_CONQUEROR_LEVEL:
			self.newExpGauge = newExpGauge

		if constInfo.IN_GAME_SHOP_ENABLE:
			self.rampageGauge1 = self.GetChild("RampageGauge")
			self.rampageGauge1.OnMouseOverIn = ui.__mem_func__(self.__RampageGauge_OverIn)
			self.rampageGauge2 = self.GetChild("RampageGauge2")
			self.rampageGauge2.OnMouseOverOut = ui.__mem_func__(self.__RampageGauge_OverOut)
			self.rampageGauge2.OnMouseLeftButtonUp = ui.__mem_func__(self.__RampageGauge_Click)
			if app.ENABLE_HEART_BEAT_MALL:
				self.rampageGauge1.SetDelay(10)
				self.rampageGauge2.SetDelay(10)

				self.rampageGauge3 = self.GetChild("RampageGauge3")
				self.rampageGauge3.OnMouseOverIn = ui.__mem_func__(self.__RampageGauge_OverIn)
				self.rampageGauge3.SetDelay(10)

				self.rampageGauge4 = self.GetChild("RampageGauge4")
				self.rampageGauge4.OnMouseOverOut = ui.__mem_func__(self.__RampageGauge_OverOut)
				self.rampageGauge4.OnMouseLeftButtonUp = ui.__mem_func__(self.__RampageGauge_Click)
				self.rampageGauge4.SetDelay(10)

			print "[DEBUG]: constInfo.IN_GAME_SHOP_ENABLE / self.rampageGauge1",constInfo.IN_GAME_SHOP_ENABLE, self.rampageGauge1
			self.__RampageGauge_OverOut()

		self.hpGauge = self.GetChild("HPGauge")
		self.mpGauge = self.GetChild("SPGauge")
		self.stGauge = self.GetChild("STGauge")
		self.hpRecoveryGaugeBar = self.GetChild("HPRecoveryGaugeBar")
		self.spRecoveryGaugeBar = self.GetChild("SPRecoveryGaugeBar")

		self.hpGaugeBoard = self.GetChild("HPGauge_Board")
		self.mpGaugeBoard = self.GetChild("SPGauge_Board")
		self.stGaugeBoard = self.GetChild("STGauge_Board")
		self.expGaugeBoard = self.GetChild("EXP_Gauge_Board")

		# Giftbox object
		wndGiftBox = GiftBox()
		wndGiftBox.LoadWindow()
		self.wndGiftBox = wndGiftBox

		self.__LoadMouseSettings()
		self.RefreshStatus()
		self.RefreshQuickSlot()

	def __RampageGauge_OverIn(self):
		print "rampage_over_in"
		if app.ENABLE_HEART_BEAT_MALL:
			if self.heartBeat:
				self.rampageGauge4.Show()
				self.rampageGauge3.Hide()

				self.rampageGauge1.Hide()
				self.rampageGauge2.Hide()
			else:
				self.rampageGauge2.Show()
				self.rampageGauge1.Hide()

				self.rampageGauge3.Hide()
				self.rampageGauge4.Hide()
		else:
			self.rampageGauge2.Show()
			self.rampageGauge1.Hide()

	def __RampageGauge_OverOut(self):
		print "rampage_over_out"
		if app.ENABLE_HEART_BEAT_MALL:
			if self.heartBeat:
				self.rampageGauge4.Hide()
				self.rampageGauge3.Show()

				self.rampageGauge1.Hide()
				self.rampageGauge2.Hide()
			else:
				self.rampageGauge2.Hide()
				self.rampageGauge1.Show()

				self.rampageGauge3.Hide()
				self.rampageGauge4.Hide()
		else:
			self.rampageGauge2.Hide()
			self.rampageGauge1.Show()

	def __RampageGauge_Click(self):
		print "rampage_up"
		net.SendChatPacket("/in_game_mall")
		# gift icon hide when click mall icon
		self.wndGiftBox.Hide()

	def __LoadMouseSettings(self):
		try:
			LoadMouseButtonSettings()
			(mouseLeftButtonEvent, mouseRightButtonEvent) = GetMouseButtonSettings()
			if not self.__IsInSafeMouseButtonSettingRange(mouseLeftButtonEvent) or not self.__IsInSafeMouseButtonSettingRange(mouseRightButtonEvent):
				raise RuntimeError, "INVALID_MOUSE_BUTTON_SETTINGS"
		except:
			InitMouseButtonSettings(self.EVENT_AUTO, self.EVENT_CAMERA)
			(mouseLeftButtonEvent, mouseRightButtonEvent) = GetMouseButtonSettings()

		try:
			self.SelectMouseButtonEvent(self.MOUSE_BUTTON_LEFT, mouseLeftButtonEvent)
			self.SelectMouseButtonEvent(self.MOUSE_BUTTON_RIGHT, mouseRightButtonEvent)
		except:
			InitMouseButtonSettings(self.EVENT_AUTO, self.EVENT_CAMERA)
			(mouseLeftButtonEvent, mouseRightButtonEvent) = GetMouseButtonSettings()

			self.SelectMouseButtonEvent(self.MOUSE_BUTTON_LEFT, mouseLeftButtonEvent)
			self.SelectMouseButtonEvent(self.MOUSE_BUTTON_RIGHT, mouseRightButtonEvent)

	def __IsInSafeMouseButtonSettingRange(self, arg):
		return arg >= self.EVENT_MOVE and arg <= self.EVENT_AUTO

	def Destroy(self):
		SaveMouseButtonSettings()

		self.ClearDictionary()
		self.mouseModeButtonList[0].ClearDictionary()
		self.mouseModeButtonList[1].ClearDictionary()
		self.mouseModeButtonList = []
		self.curMouseModeButton = []
		self.curSkillButton = None
		self.selectSkillButtonList = []

		self.expGauge = None
		if app.ENABLE_CONQUEROR_LEVEL:
			self.newExpGauge = None
		self.hpGauge = None
		self.mpGauge = None
		self.stGauge = None
		self.hpRecoveryGaugeBar = None
		self.spRecoveryGaugeBar = None

		self.rampageGauge1 = None
		self.rampageGauge2 = None
		if app.ENABLE_HEART_BEAT_MALL:
			self.heartBeat = False

			self.rampageGauge3 = None
			self.rampageGauge4 = None

		self.tooltipItem = None
		self.tooltipSkill = None
		self.quickslot = []
		self.toggleButtonDict = {}

		self.hpGaugeBoard = None
		self.mpGaugeBoard = None
		self.stGaugeBoard = None
		self.expGaugeBoard = None

		self.tooltipHP = None
		self.tooltipSP = None
		self.tooltipST = None
		self.tooltipEXP = None

		self.mouseImage = None

	def __OnClickQuickPageUpButton(self):
		player.SetQuickPage(player.GetQuickPage()-1)

	def __OnClickQuickPageDownButton(self):
		player.SetQuickPage(player.GetQuickPage()+1)

	def SetToggleButtonEvent(self, eButton, kEventFunc):
		self.toggleButtonDict[eButton].SetEvent(kEventFunc)

	def SetItemToolTip(self, tooltipItem):
		self.tooltipItem = tooltipItem

	def SetSkillToolTip(self, tooltipSkill):
		self.tooltipSkill = tooltipSkill
		self.curSkillButton.SetSkillToolTip(self.tooltipSkill)

	## Mouse Image
	def ShowMouseImage(self):
		self.mouseImage.SetTop()
		self.mouseImage.Show()

	def HideMouseImage(self):
		player.SetQuickCameraMode(False)
		self.mouseImage.Hide()

	## Gauge
	def RefreshStatus(self):
		curHP = player.GetStatus(player.HP)
		maxHP = player.GetStatus(player.MAX_HP)
		curSP = player.GetStatus(player.SP)
		maxSP = player.GetStatus(player.MAX_SP)
		if app.ENABLE_CONQUEROR_LEVEL:
			if player.GetStatus(player.POINT_CONQUEROR_LEVEL) > 0:
				curEXP = unsigned32(player.GetStatus(player.POINT_CONQUEROR_EXP))
				nextEXP = unsigned32(player.GetStatus(player.POINT_CONQUEROR_NEXT_EXP))
			else:
				curEXP = unsigned32(player.GetStatus(player.EXP))
				nextEXP = unsigned32(player.GetStatus(player.NEXT_EXP))
		else:
			curEXP = unsigned32(player.GetStatus(player.EXP))
			nextEXP = unsigned32(player.GetStatus(player.NEXT_EXP))
		recoveryHP = player.GetStatus(player.HP_RECOVERY)
		recoverySP = player.GetStatus(player.SP_RECOVERY)

		self.RefreshStamina()

		self.SetHP(curHP, recoveryHP, maxHP)
		self.SetSP(curSP, recoverySP, maxSP)
		self.SetExperience(curEXP, nextEXP)

	def RefreshStamina(self):
		curST = player.GetStatus(player.STAMINA)
		maxST = player.GetStatus(player.MAX_STAMINA)
		self.SetST(curST, maxST)

	def RefreshSkill(self):
		self.curSkillButton.RefreshSkill()
		for button in self.selectSkillButtonList:
			button.RefreshSkill()

	if app.ENABLE_SKILL_COOLTIME_UPDATE:
		def SkillClearCoolTime(self, usedSlotIndex):
			QUICK_SLOT_SLOT_COUNT = 4
			slotIndex = 0
			for slotWindow in self.quickslot:
				for i in xrange(QUICK_SLOT_SLOT_COUNT):
					(Type, Position) = player.GetLocalQuickSlot(slotIndex)
					if Type == player.SLOT_TYPE_SKILL:
						if usedSlotIndex == Position:
							slotWindow.SetSlotCoolTime(slotIndex, 0)
							return
					slotIndex += 1

	def SetHP(self, curPoint, recoveryPoint, maxPoint):
		curPoint = min(curPoint, maxPoint)
		if maxPoint > 0:
			self.hpGauge.SetPercentage(curPoint, maxPoint)
			self.tooltipHP.SetText("%s : %d / %d" % (localeInfo.TASKBAR_HP, curPoint, maxPoint))

			if 0 == recoveryPoint:
				self.hpRecoveryGaugeBar.Hide()
			else:
				destPoint = min(maxPoint, curPoint + recoveryPoint)
				newWidth = int(self.GAUGE_WIDTH * (float(destPoint) / float(maxPoint)))
				self.hpRecoveryGaugeBar.SetSize(newWidth, self.GAUGE_HEIGHT)
				self.hpRecoveryGaugeBar.Show()

		if app.ENABLE_HEART_BEAT_MALL:
			self.heartBeat = True
			curPointPct = int(100 * float(curPoint) / float(maxPoint))
			heartBeats = [
				[ [0, 30], 1 ],
				[ [30, 40], 2 ],
				[ [40, 50], 3 ],
			]

			if curPointPct >= heartBeats[0][0][0] and curPointPct <= heartBeats[0][0][1]:
				self.rampageGauge1.SetDelay(heartBeats[0][1])
				self.rampageGauge2.SetDelay(heartBeats[0][1])
				self.rampageGauge3.SetDelay(heartBeats[0][1])
				self.rampageGauge4.SetDelay(heartBeats[0][1])
			elif curPointPct >= heartBeats[1][0][0] and curPointPct <= heartBeats[1][0][1]:
				self.rampageGauge1.SetDelay(heartBeats[1][1])
				self.rampageGauge2.SetDelay(heartBeats[1][1])
				self.rampageGauge3.SetDelay(heartBeats[1][1])
				self.rampageGauge4.SetDelay(heartBeats[1][1])
			elif curPointPct >= heartBeats[2][0][0] and curPointPct <= heartBeats[2][0][1]:
				self.rampageGauge1.SetDelay(heartBeats[2][1])
				self.rampageGauge2.SetDelay(heartBeats[2][1])
				self.rampageGauge3.SetDelay(heartBeats[2][1])
				self.rampageGauge4.SetDelay(heartBeats[2][1])
			else:
				if curPointPct <= 0:
					self.rampageGauge1.SetDelay(99)
					self.rampageGauge2.SetDelay(99)
					self.rampageGauge3.SetDelay(99)
					self.rampageGauge4.SetDelay(99)
				else:
					self.heartBeat = False

					self.rampageGauge1.SetDelay(10)
					self.rampageGauge2.SetDelay(10)
					self.rampageGauge3.SetDelay(10)
					self.rampageGauge4.SetDelay(10)

			if self.heartBeat:
				self.rampageGauge1.Hide()
				self.rampageGauge2.Hide()
				self.rampageGauge3.Show()
				self.rampageGauge4.Show()
			else:
				self.rampageGauge1.Show()
				self.rampageGauge2.Show()
				self.rampageGauge3.Hide()
				self.rampageGauge4.Hide()

	def SetSP(self, curPoint, recoveryPoint, maxPoint):
		curPoint = min(curPoint, maxPoint)
		if maxPoint > 0:
			self.mpGauge.SetPercentage(curPoint, maxPoint)
			self.tooltipSP.SetText("%s : %d / %d" % (localeInfo.TASKBAR_SP, curPoint, maxPoint))

			if 0 == recoveryPoint:
				self.spRecoveryGaugeBar.Hide()
			else:
				destPoint = min(maxPoint, curPoint + recoveryPoint)
				newWidth = int(self.GAUGE_WIDTH * (float(destPoint) / float(maxPoint)))
				self.spRecoveryGaugeBar.SetSize(newWidth, self.GAUGE_HEIGHT)
				self.spRecoveryGaugeBar.Show()

	def SetST(self, curPoint, maxPoint):
		curPoint = min(curPoint, maxPoint)
		if maxPoint > 0:
			self.stGauge.SetPercentage(curPoint, maxPoint)
			self.tooltipST.SetText("%s : %d / %d" % (localeInfo.TASKBAR_ST, curPoint, maxPoint))

	def SetExperience(self, curPoint, maxPoint):
		curPoint = min(curPoint, maxPoint)
		curPoint = max(curPoint, 0)
		maxPoint = max(maxPoint, 0)

		quarterPoint = maxPoint / 4
		FullCount = 0

		if 0 != quarterPoint:
			FullCount = min(4, curPoint / quarterPoint)

		if app.ENABLE_CONQUEROR_LEVEL:
			if player.GetStatus(player.POINT_CONQUEROR_LEVEL) > 0:
				for i in xrange(4):
					self.expGauge[i].Hide()
					self.newExpGauge[i].Hide()

				for i in xrange(FullCount):
					self.newExpGauge[i].SetRenderingRect(0.0, 0.0, 0.0, 0.0)
					self.newExpGauge[i].Show()

				if 0 != quarterPoint:
					if FullCount < 4:
						Percentage = float(curPoint % quarterPoint) / quarterPoint - 1.0
						self.newExpGauge[FullCount].SetRenderingRect(0.0, Percentage, 0.0, 0.0)
						self.newExpGauge[FullCount].Show()

				self.tooltipEXP.SetText("%s : %.2f%%" % (localeInfo.TASKBAR_EXP, float(curPoint) / max(1, float(maxPoint)) * 100))
				return
			else:
				for i in xrange(4):
					self.newExpGauge[i].Hide()

		for i in xrange(4):
			self.expGauge[i].Hide()

		for i in xrange(FullCount):
			self.expGauge[i].SetRenderingRect(0.0, 0.0, 0.0, 0.0)
			self.expGauge[i].Show()

		if 0 != quarterPoint:
			if FullCount < 4:
				Percentage = float(curPoint % quarterPoint) / quarterPoint - 1.0
				self.expGauge[FullCount].SetRenderingRect(0.0, Percentage, 0.0, 0.0)
				self.expGauge[FullCount].Show()

		#####
		self.tooltipEXP.SetText("%s : %.2f%%" % (localeInfo.TASKBAR_EXP, float(curPoint) / max(1, float(maxPoint)) * 100))

	## QuickSlot
	def RefreshQuickSlot(self):
		pageNum = player.GetQuickPage()

		try:
			self.quickPageNumImageBox.LoadImage(TaskBar.QUICKPAGE_NUMBER_FILENAME[pageNum])
		except:
			pass

		startNumber = 0
		for slot in self.quickslot:

			for i in xrange(4):
				slotNumber = i+startNumber

				(Type, Position) = player.GetLocalQuickSlot(slotNumber)

				if player.SLOT_TYPE_NONE == Type:
					slot.ClearSlot(slotNumber)
					continue

				if player.SLOT_TYPE_INVENTORY == Type:
					itemIndex = player.GetItemIndex(Position)
					itemCount = player.GetItemCount(Position)
					if itemCount <= 1:
						itemCount = 0

					## �ڵ����� (#72723, #72724) Ư��ó�� - �������ε��� ���Կ� Ȱ��ȭ/��Ȱ��ȭ ǥ�ø� ���� �۾��� - [hyo]
					if constInfo.IS_AUTO_POTION(itemIndex):
						# metinSocket - [0] : Ȱ��ȭ ����, [1] : ����� ��, [2] : �ִ� �뷮
						metinSocket = [player.GetItemMetinSocket(Position, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]

						if 0 != int(metinSocket[0]):
							slot.ActivateSlot(slotNumber)
						else:
							slot.DeactivateSlot(slotNumber)

					if item.IsPetPay(itemIndex):
						metinSocket = [player.GetItemMetinSocket(Position, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]

						if 0 != int(metinSocket[1]):
							slot.ActivateSlot(slotNumber)
						else:
							slot.DeactivateSlot(slotNumber)

					if app.ENABLE_SOUL_SYSTEM:
						if item.IsSoulItem(itemIndex):
							metinSocket = [player.GetItemMetinSocket(window, Position, j) for j in xrange(player.METIN_SOCKET_MAX_NUM)]

							if 0 != int(metinSocket[1]):
								slot.ActivateSlot(slotNumber)
							else:
								slot.DeactivateSlot(slotNumber)

					slot.SetItemSlot(slotNumber, itemIndex, itemCount)

				elif player.SLOT_TYPE_SKILL == Type:
					skillIndex = player.GetSkillIndex(Position)
					if 0 == skillIndex:
						slot.ClearSlot(slotNumber)
						continue

					skillType = skill.GetSkillType(skillIndex)
					if skill.SKILL_TYPE_GUILD == skillType:
						skillGrade = 0
						skillLevel = guild.GetSkillLevel(Position)
					else:
						skillGrade = player.GetSkillGrade(Position)
						skillLevel = player.GetSkillLevel(Position)

					slot.SetSkillSlotNew(slotNumber, skillIndex, skillGrade, skillLevel)
					slot.SetSlotCountNew(slotNumber, skillGrade, skillLevel)
					slot.SetCoverButton(slotNumber)

					## NOTE : CoolTime üũ
					if player.IsSkillCoolTime(Position):
						(coolTime, elapsedTime) = player.GetSkillCoolTime(Position)
						slot.SetSlotCoolTime(slotNumber, coolTime, elapsedTime)

					## NOTE : Activate �Ǿ� �ִٸ� �����ܵ� ������Ʈ
					if player.IsSkillActive(Position):
						slot.ActivateSlot(slotNumber)

				elif player.SLOT_TYPE_EMOTION == Type:

					emotionIndex = Position
					slot.SetEmotionSlot(slotNumber, emotionIndex)
					slot.SetCoverButton(slotNumber)
					slot.SetSlotCount(slotNumber, 0)

			slot.RefreshSlot()
			startNumber += 4

	def canAddQuickSlot(self, Type, slotNumber):
		if player.SLOT_TYPE_INVENTORY == Type:
			itemIndex = player.GetItemIndex(slotNumber)
			return item.CanAddToQuickSlotItem(itemIndex)

		return True

	def AddQuickSlot(self, localSlotIndex):
		AttachedSlotType = mouseModule.mouseController.GetAttachedType()
		AttachedSlotNumber = mouseModule.mouseController.GetAttachedSlotNumber()
		AttachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()

		if player.SLOT_TYPE_QUICK_SLOT == AttachedSlotType:
			player.RequestMoveGlobalQuickSlotToLocalQuickSlot(AttachedSlotNumber, localSlotIndex)

		elif player.SLOT_TYPE_EMOTION == AttachedSlotType:
			player.RequestAddLocalQuickSlot(localSlotIndex, AttachedSlotType, AttachedItemIndex)

		elif True == self.canAddQuickSlot(AttachedSlotType, AttachedSlotNumber):
			## Online Code
			player.RequestAddLocalQuickSlot(localSlotIndex, AttachedSlotType, AttachedSlotNumber)

		mouseModule.mouseController.DeattachObject()
		self.RefreshQuickSlot()

	def SelectEmptyQuickSlot(self, slotIndex):
		if True == mouseModule.mouseController.isAttached():
			self.AddQuickSlot(slotIndex)

	def SelectItemQuickSlot(self, localQuickSlotIndex):
		if True == mouseModule.mouseController.isAttached():
			self.AddQuickSlot(localQuickSlotIndex)
		else:
			globalQuickSlotIndex=player.LocalQuickSlotIndexToGlobalQuickSlotIndex(localQuickSlotIndex)
			mouseModule.mouseController.AttachObject(self, player.SLOT_TYPE_QUICK_SLOT, globalQuickSlotIndex, globalQuickSlotIndex)

	def UnselectItemQuickSlot(self, localSlotIndex):
		if False == mouseModule.mouseController.isAttached():
			player.RequestUseLocalQuickSlot(localSlotIndex)
			return
		elif mouseModule.mouseController.isAttached():
			mouseModule.mouseController.DeattachObject()
			return

	def OnUseSkill(self, usedSlotIndex, coolTime):
		QUICK_SLOT_SLOT_COUNT = 4
		slotIndex = 0

		## Current Skill Button
		if usedSlotIndex == self.curSkillButton.GetSlotIndex():
			self.curSkillButton.Activate(coolTime)

		## Quick Slot
		for slotWindow in self.quickslot:
			for i in xrange(QUICK_SLOT_SLOT_COUNT):
				(Type, Position) = player.GetLocalQuickSlot(slotIndex)

				if Type == player.SLOT_TYPE_SKILL:
					if usedSlotIndex == Position:
						slotWindow.SetSlotCoolTime(slotIndex, coolTime)
						return

				slotIndex += 1

	def OnActivateSkill(self, usedSlotIndex):
		slotIndex = 0

		## Current Skill Button
		if usedSlotIndex == self.curSkillButton.GetSlotIndex():
			self.curSkillButton.Deactivate()

		## Quick Slot
		for slotWindow in self.quickslot:
			for i in xrange(4):
				(Type, Position) = player.GetLocalQuickSlot(slotIndex)

				if Type == player.SLOT_TYPE_SKILL:
					if usedSlotIndex == Position:
						slotWindow.ActivateSlot(slotIndex)
						return

				slotIndex += 1

	def OnDeactivateSkill(self, usedSlotIndex):
		slotIndex = 0

		## Current Skill Button
		if usedSlotIndex == self.curSkillButton.GetSlotIndex():
			self.curSkillButton.Deactivate()

		## Quick Slot
		for slotWindow in self.quickslot:
			for i in xrange(4):
				(Type, Position) = player.GetLocalQuickSlot(slotIndex)

				if Type == player.SLOT_TYPE_SKILL:
					if usedSlotIndex == Position:
						slotWindow.DeactivateSlot(slotIndex)
						return

				slotIndex += 1

	## ToolTip
	def OverInItem(self, slotNumber):
		if mouseModule.mouseController.isAttached():
			return

		(Type, Position) = player.GetLocalQuickSlot(slotNumber)

		if player.SLOT_TYPE_INVENTORY == Type:
			self.tooltipItem.SetInventoryItem(Position)
			self.tooltipSkill.HideToolTip()

		elif player.SLOT_TYPE_SKILL == Type:
			skillIndex = player.GetSkillIndex(Position)
			skillType = skill.GetSkillType(skillIndex)

			if skill.SKILL_TYPE_GUILD == skillType:
				skillGrade = 0
				skillLevel = guild.GetSkillLevel(Position)

			else:
				skillGrade = player.GetSkillGrade(Position)
				skillLevel = player.GetSkillLevel(Position)

			self.tooltipSkill.SetSkillNew(Position, skillIndex, skillGrade, skillLevel)
			self.tooltipItem.HideToolTip()

	def OverOutItem(self):
		if 0 != self.tooltipItem:
			self.tooltipItem.HideToolTip()
		if 0 != self.tooltipSkill:
			self.tooltipSkill.HideToolTip()

	def OnUpdate(self):
		if app.GetGlobalTime() - self.lastUpdateQuickSlot > 500:
			self.lastUpdateQuickSlot = app.GetGlobalTime()
			self.RefreshQuickSlot()

		if True == self.hpGaugeBoard.IsIn():
			self.tooltipHP.Show()
		else:
			self.tooltipHP.Hide()

		if True == self.mpGaugeBoard.IsIn():
			self.tooltipSP.Show()
		else:
			self.tooltipSP.Hide()

		if True == self.stGaugeBoard.IsIn():
			self.tooltipST.Show()
		else:
			self.tooltipST.Hide()

		if True == self.expGaugeBoard.IsIn():
			self.tooltipEXP.Show()
		else:
			self.tooltipEXP.Hide()

	## Skill
	def ToggleLeftMouseButtonModeWindow(self):
		wndMouseButtonMode = self.mouseModeButtonList[self.MOUSE_BUTTON_LEFT]

		if True == wndMouseButtonMode.IsShow():
			wndMouseButtonMode.Hide()
		else:
			wndMouseButtonMode.Show()

	def ToggleRightMouseButtonModeWindow(self):
		wndMouseButtonMode = self.mouseModeButtonList[self.MOUSE_BUTTON_RIGHT]

		if True == wndMouseButtonMode.IsShow():
			wndMouseButtonMode.Hide()
			self.CloseSelectSkill()
		else:
			wndMouseButtonMode.Show()
			self.OpenSelectSkill()

	def OpenSelectSkill(self):
		PAGE_SLOT_COUNT = 6

		(xSkillButton, y) = self.curSkillButton.GetGlobalPosition()
		y -= (37 + 32 + 1)

		for key in self.skillCategoryNameList:
			appendCount = 0
			startNumber = self.skillPageStartSlotIndexDict[key]
			x = xSkillButton

			getSkillIndex=player.GetSkillIndex
			getSkillLevel=player.GetSkillLevel

			for i in xrange(PAGE_SLOT_COUNT):
				skillIndex = getSkillIndex(startNumber+i)
				skillLevel = getSkillLevel(startNumber+i)

				if 0 == skillIndex:
					continue
				if 0 == skillLevel:
					continue
				if skill.IsStandingSkill(skillIndex):
					continue

				## FIXME : ��ų �ϳ��� ���� �ϳ��� �Ҵ��ϴ°� �ƹ��� ���� ���ϰ� ũ��.
				##		 �� �κ��� �ð��� ���� ��ġ����. - [levites]
				skillButton = self.SkillButton()
				skillButton.SetSkill(startNumber+i)
				skillButton.SetPosition(x, y)
				skillButton.SetSkillEvent(ui.__mem_func__(self.CloseSelectSkill), startNumber+i+1)
				skillButton.SetSkillToolTip(self.tooltipSkill)
				skillButton.SetTop()
				skillButton.Show()
				self.selectSkillButtonList.append(skillButton)

				appendCount += 1
				x -= 32

			if appendCount > 0:
				y -= 32

	def CloseSelectSkill(self, slotIndex=-1):
		self.mouseModeButtonList[self.MOUSE_BUTTON_RIGHT].Hide()
		for button in self.selectSkillButtonList:
			button.Destroy()

		self.selectSkillButtonList = []

		if -1 != slotIndex:
			self.curSkillButton.Show()
			self.curMouseModeButton[self.MOUSE_BUTTON_RIGHT].Hide()
			player.SetMouseFunc(player.MBT_RIGHT, player.MBF_SKILL)
			player.ChangeCurrentSkillNumberOnly(slotIndex-1)
		else:
			self.curSkillButton.Hide()
			self.curMouseModeButton[self.MOUSE_BUTTON_RIGHT].Show()

	def SelectMouseButtonEvent(self, dir, event):
		SetMouseButtonSetting(dir, event)

		self.CloseSelectSkill()
		self.mouseModeButtonList[dir].Hide()

		btn = 0
		type = self.NONE
		func = self.NONE
		tooltip_text = ""

		if self.MOUSE_BUTTON_LEFT == dir:
			type = player.MBT_LEFT

		elif self.MOUSE_BUTTON_RIGHT == dir:
			type = player.MBT_RIGHT

		if self.EVENT_MOVE == event:
			btn = self.mouseModeButtonList[dir].GetChild("button_move")
			func = player.MBF_MOVE
			tooltip_text = localeInfo.TASKBAR_MOVE
		elif self.EVENT_ATTACK == event:
			btn = self.mouseModeButtonList[dir].GetChild("button_attack")
			func = player.MBF_ATTACK
			tooltip_text = localeInfo.TASKBAR_ATTACK
		elif self.EVENT_AUTO == event:
			btn = self.mouseModeButtonList[dir].GetChild("button_auto_attack")
			func = player.MBF_AUTO
			tooltip_text = localeInfo.TASKBAR_AUTO
		elif self.EVENT_MOVE_AND_ATTACK == event:
			btn = self.mouseModeButtonList[dir].GetChild("button_move_and_attack")
			func = player.MBF_SMART
			tooltip_text = localeInfo.TASKBAR_ATTACK
		elif self.EVENT_CAMERA == event:
			btn = self.mouseModeButtonList[dir].GetChild("button_camera")
			func = player.MBF_CAMERA
			tooltip_text = localeInfo.TASKBAR_CAMERA
		elif self.EVENT_SKILL == event:
			btn = self.mouseModeButtonList[dir].GetChild("button_skill")
			func = player.MBF_SKILL
			tooltip_text = localeInfo.TASKBAR_SKILL

		if 0 != btn:
			self.curMouseModeButton[dir].SetToolTipText(tooltip_text, 0, -18)
			self.curMouseModeButton[dir].SetUpVisual(btn.GetUpVisualFileName())
			self.curMouseModeButton[dir].SetOverVisual(btn.GetOverVisualFileName())
			self.curMouseModeButton[dir].SetDownVisual(btn.GetDownVisualFileName())
			self.curMouseModeButton[dir].Show()

		player.SetMouseFunc(type, func)

	def OnChangeCurrentSkill(self, skillSlotNumber):
		self.curSkillButton.SetSkill(skillSlotNumber)
		self.curSkillButton.Show()
		self.curMouseModeButton[self.MOUSE_BUTTON_RIGHT].Hide()