import app
import net
import player
import item
import ui
import uiToolTip
import mouseModule
import localeInfo
import uiCommon
import constInfo
import osfInfo

if app.WJ_ENABLE_TRADABLE_ICON:
	INVENTORY_PAGE_SIZE = player.INVENTORY_PAGE_SIZE

if osfInfo.SHOW_REFINE_ITEM_DESC == True:
	TOOLTIP_DATA = {
		"materials" : [],
		"slot_count" : 0
	}

class RefineDialog(ui.ScriptWindow):
	makeSocketSuccessPercentage = ( 100, 33, 20, 15, 10, 5, 0 )
	upgradeStoneSuccessPercentage = ( 30, 29, 28, 27, 26, 25, 24, 23, 22 )
	upgradeArmorSuccessPercentage = ( 99, 66, 33, 33, 33, 33, 33, 33, 33 )
	upgradeAccessorySuccessPercentage = ( 99, 88, 77, 66, 33, 33, 33, 33, 33 )
	upgradeSuccessPercentage = ( 99, 66, 33, 33, 33, 33, 33, 33, 33 )

	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__LoadScript()

		self.scrollItemPos = 0
		self.targetItemPos = 0
		self.IsShow = False

	def __LoadScript(self):
		self.__LoadQuestionDialog()

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/refinedialog.py")

		except:
			import exception
			exception.Abort("RefineDialog.__LoadScript.LoadObject")

		try:
			self.board = self.GetChild("Board")
			self.titleBar = self.GetChild("TitleBar")
			self.successPercentage = self.GetChild("SuccessPercentage")
			self.GetChild("AcceptButton").SetEvent(self.OpenQuestionDialog)
			self.GetChild("CancelButton").SetEvent(self.Close)
		except:
			import exception
			exception.Abort("RefineDialog.__LoadScript.BindObject")

		## 936 : 개량 확률 표시 안함
		##if 936 == app.GetDefaultCodePage():
		if osfInfo.SHOW_REFINE_PERCENTAGE == True:
			self.successPercentage.Show()
		else:
			self.successPercentage.Hide()

		toolTip = uiToolTip.ItemToolTip()
		toolTip.SetParent(self)
		toolTip.SetPosition(15, 38)
		toolTip.SetFollow(False)
		toolTip.Show()
		self.toolTip = toolTip

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.Close))

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def __LoadQuestionDialog(self):
		self.dlgQuestion = ui.ScriptWindow()

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self.dlgQuestion, "uiscript/questiondialog2.py")
		except:
			import exception
			exception.Abort("RefineDialog.__LoadQuestionDialog.LoadScript")

		try:
			GetObject=self.dlgQuestion.GetChild
			GetObject("message1").SetText(localeInfo.REFINE_DESTROY_WARNING)
			GetObject("message2").SetText(localeInfo.REFINE_WARNING2)
			GetObject("accept").SetEvent(ui.__mem_func__(self.Accept))
			GetObject("cancel").SetEvent(ui.__mem_func__(self.dlgQuestion.Hide))
		except:
			import exception
			exception.Abort("SelectCharacterWindow.__LoadQuestionDialog.BindObject")

	def Destroy(self):
		self.ClearDictionary()
		self.board = 0
		self.successPercentage = 0
		self.titleBar = 0
		self.toolTip = 0
		self.dlgQuestion = 0

	def GetRefineSuccessPercentage(self, scrollSlotIndex, itemSlotIndex):
		if -1 != scrollSlotIndex:
			if player.IsRefineGradeScroll(scrollSlotIndex):
				curGrade = player.GetItemGrade(itemSlotIndex)
				itemIndex = player.GetItemIndex(itemSlotIndex)

				item.SelectItem(itemIndex)
				itemType = item.GetItemType()
				itemSubType = item.GetItemSubType()

				if item.ITEM_TYPE_METIN == itemType:
					if curGrade >= len(self.upgradeStoneSuccessPercentage):
						return 0
					return self.upgradeStoneSuccessPercentage[curGrade]

				elif item.ITEM_TYPE_ARMOR == itemType:
					if item.ARMOR_BODY == itemSubType:
						if curGrade >= len(self.upgradeArmorSuccessPercentage):
							return 0
						return self.upgradeArmorSuccessPercentage[curGrade]
					else:
						if curGrade >= len(self.upgradeAccessorySuccessPercentage):
							return 0
						return self.upgradeAccessorySuccessPercentage[curGrade]
				else:
					if curGrade >= len(self.upgradeSuccessPercentage):
						return 0
					return self.upgradeSuccessPercentage[curGrade]

		for i in xrange(player.METIN_SOCKET_MAX_NUM+1):
			if 0 == player.GetItemMetinSocket(itemSlotIndex, i):
				break

		return self.makeSocketSuccessPercentage[i]

	def Open(self, scrollItemPos, targetItemPos):
		self.scrollItemPos = scrollItemPos
		self.targetItemPos = targetItemPos

		percentage = self.GetRefineSuccessPercentage(scrollItemPos, targetItemPos)
		if 0 == percentage:
			return
		self.successPercentage.SetText(localeInfo.REFINE_SUCCESS_PROBALITY % (percentage))

		itemIndex = player.GetItemIndex(targetItemPos)
		self.toolTip.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(targetItemPos, i))
		self.toolTip.AddItemData(itemIndex, metinSlot)

		self.UpdateDialog()
		self.SetTop()
		self.Show()
		self.IsShow = True

	def UpdateDialog(self):
		newWidth = self.toolTip.GetWidth() + 30
		newHeight = self.toolTip.GetHeight() + 98
		self.board.SetSize(newWidth, newHeight)
		self.titleBar.SetWidth(newWidth-15)
		self.SetSize(newWidth, newHeight)

		(x, y) = self.GetLocalPosition()
		self.SetPosition(x, y)

	def OpenQuestionDialog(self):
		percentage = self.GetRefineSuccessPercentage(-1, self.targetItemPos)
		if 100 == percentage:
			self.Accept()
			return

		self.dlgQuestion.SetTop()
		self.dlgQuestion.Show()

	def Accept(self):
		net.SendItemUseToItemPacket(self.scrollItemPos, self.targetItemPos)
		self.Close()

	def Close(self):
		self.dlgQuestion.Hide()
		self.Hide()
		self.IsShow = False

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def IsShow(self):
		return self.IsShow

class RefineDialogNew(ui.ScriptWindow):
	def __init__(self):
		ui.ScriptWindow.__init__(self)
		self.__Initialize()
		self.isLoaded = False

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.interface = None
			self.inven = None

	def __Initialize(self):
		self.dlgQuestion = None
		self.children = []
		self.vnum = 0
		self.targetItemPos = 0
		self.dialogHeight = 0
		self.cost = 0
		self.percentage = 0
		self.type = 0
		self.apply_random_list = None
		self.src_vnum = 0
		self.IsShow = False

	def __LoadScript(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "uiscript/refinedialog.py")

		except:
			import exception
			exception.Abort("RefineDialog.__LoadScript.LoadObject")

		try:
			self.board = self.GetChild("Board")
			self.titleBar = self.GetChild("TitleBar")
			self.probText = self.GetChild("SuccessPercentage")
			self.costText = self.GetChild("Cost")
			self.successPercentage = self.GetChild("SuccessPercentage")
			self.GetChild("AcceptButton").SetEvent(self.OpenQuestionDialog)
			self.GetChild("CancelButton").SetEvent(self.CancelRefine)
		except:
			import exception
			exception.Abort("RefineDialog.__LoadScript.BindObject")

		## 936 : 개량 확률 표시 안함
		##if 936 == app.GetDefaultCodePage():
		if osfInfo.SHOW_REFINE_PERCENTAGE == True:
			self.successPercentage.Show()
		else:
			self.successPercentage.Hide()

		toolTip = uiToolTip.ItemToolTip()
		toolTip.SetParent(self)
		toolTip.SetFollow(False)
		toolTip.SetPosition(15, 38)
		toolTip.Show()
		self.toolTip = toolTip

		if osfInfo.SHOW_REFINE_ITEM_DESC == True:
			self.toolTipItem = uiToolTip.ItemToolTip()
			self.toolTipItem.Hide()

		self.slotList = []
		for i in xrange(3):
			slot = self.__MakeSlot()
			slot.SetParent(toolTip)
			slot.SetWindowVerticalAlignCenter()
			self.slotList.append(slot)

		itemImage = self.__MakeItemImage()
		itemImage.SetParent(toolTip)
		itemImage.SetWindowVerticalAlignCenter()
		itemImage.SetPosition(-35, 0)
		self.itemImage = itemImage

		self.titleBar.SetCloseEvent(ui.__mem_func__(self.CancelRefine))
		self.isLoaded = True

	def __del__(self):
		ui.ScriptWindow.__del__(self)

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.inven = None

	def __MakeSlot(self):
		slot = ui.ImageBox()
		slot.LoadImage("d:/ymir work/ui/public/slot_base.sub")
		slot.Show()
		self.children.append(slot)
		return slot

	if osfInfo.SHOW_REFINE_ITEM_DESC == True:
		def __MakeItemSlot(self, slotIndex):
			slot = ui.SlotWindow()
			slot.SetParent(self)
			slot.SetSize(32, 32)
			slot.SetSlotBaseImage("d:/ymir work/ui/public/Slot_Base.sub", 1.0, 1.0, 1.0, 1.0)
			slot.AppendSlot(slotIndex, 0, 0, 32, 32)
			slot.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			slot.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			slot.RefreshSlot()
			slot.Show()
			self.children.append(slot)
			return slot

		def OverInItem(self, slotIndex):
			if slotIndex > len(TOOLTIP_DATA["materials"]):
				return

			itemVnum = TOOLTIP_DATA["materials"][slotIndex]
			metinSlot = []
			attrSlot = []
			rare_attr_slot = None
			flags = 0
			unbindTime = 0
			window_type = player.INVENTORY

			if self.toolTipItem:
				self.toolTipItem.ClearToolTip()
				self.toolTipItem.AddItemData(itemVnum, metinSlot, attrSlot, rare_attr_slot, flags, unbindTime, window_type)
				self.toolTipItem.AlignHorizonalCenter()
				self.toolTipItem.ShowToolTip()

		def OverOutItem(self):
			if self.toolTipItem:
				self.toolTipItem.HideToolTip()

	def __MakeItemImage(self):
		itemImage = ui.ImageBox()
		itemImage.Show()
		self.children.append(itemImage)
		return itemImage

	def __MakeThinBoard(self):
		thinBoard = ui.ThinBoard()
		thinBoard.SetParent(self)
		thinBoard.Show()
		self.children.append(thinBoard)
		return thinBoard

	def Destroy(self):
		self.ClearDictionary()
		self.dlgQuestion = None
		self.board = 0
		self.probText = 0
		self.costText = 0
		self.titleBar = 0
		self.toolTip = 0
		self.successPercentage = None
		self.slotList = []
		self.children = []

	def Open(self, targetItemPos, nextGradeItemVnum, cost, prob, type, apply_random_list, src_vnum):
		if False == self.isLoaded:
			self.__LoadScript()

		self.__Initialize()

		self.targetItemPos = targetItemPos
		self.vnum = nextGradeItemVnum
		self.cost = cost
		self.percentage = prob
		self.type = type
		self.apply_random_list = apply_random_list
		self.src_vnum = src_vnum

		self.probText.SetText(localeInfo.REFINE_SUCCESS_PROBALITY % (self.percentage))
		self.costText.SetText(localeInfo.REFINE_COST % (self.cost))
		if app.ENABLE_AUTO_REFINE:
			self.checkBox = ui.CheckBox()
			self.checkBox.SetParent(self)
			self.checkBox.SetPosition(0, 90)
			self.checkBox.SetWindowHorizontalAlignCenter()
			self.checkBox.SetWindowVerticalAlignBottom()
			self.checkBox.SetEvent(ui.__mem_func__(self.AutoRefine), "ON_CHECK", True)
			self.checkBox.SetEvent(ui.__mem_func__(self.AutoRefine), "ON_UNCKECK", False)
			self.checkBox.SetCheckStatus(osfInfo.IS_AUTO_REFINE)
			self.checkBox.SetTextInfo(localeInfo.REFINE_AUTO)
			self.checkBox.Show()

		self.toolTip.ClearToolTip()
		metinSlot = []
		for i in xrange(player.METIN_SOCKET_MAX_NUM):
			metinSlot.append(player.GetItemMetinSocket(targetItemPos, i))

		attrSlot = []
		for i in xrange(player.ATTRIBUTE_SLOT_MAX_NUM):
			attrSlot.append(player.GetItemAttribute(targetItemPos, i))

		#def AddRefineItemData(self, item_vnum, sockets, attributes, rare_attr_slot = None, flags = 0, refine_element = None, apply_random_list = None, set_value = 0):
		if app.ENABLE_APPLY_RANDOM:
			self.toolTip.AddRefineItemData(nextGradeItemVnum, metinSlot, attrSlot, None, type, None, apply_random_list, 0)
		else:
			self.toolTip.AddRefineItemData(nextGradeItemVnum, metinSlot, attrSlot, type)

		item.SelectItem(nextGradeItemVnum)
		self.itemImage.LoadImage(item.GetIconImageFileName())
		xSlotCount, ySlotCount = item.GetItemSize()
		for slot in self.slotList:
			slot.Hide()
		for i in xrange(min(3, ySlotCount)):
			self.slotList[i].SetPosition(-35, i*32 - (ySlotCount-1)*16)
			self.slotList[i].Show()

		self.dialogHeight = self.toolTip.GetHeight() + 46
		self.UpdateDialog()

		if osfInfo.IS_AUTO_REFINE == True and osfInfo.AUTO_REFINE == True:
			pass
		else:
			self.SetTop()

		self.Show()
		self.IsShow = True

	def Close(self):
		self.dlgQuestion = None
		self.Hide()
		self.IsShow = False
		if app.ENABLE_AUTO_REFINE:
			osfInfo.AUTO_REFINE = False

	def IsShow(self):
		return self.IsShow

	def AppendMaterial(self, vnum, count):
		if osfInfo.SHOW_REFINE_ITEM_DESC == True:
			slotIndex = len(TOOLTIP_DATA["materials"])

			slot = self.__MakeItemSlot(slotIndex)
			slot.SetPosition(15, self.dialogHeight)
			slot.SetItemSlot(slotIndex, vnum, count)

			TOOLTIP_DATA["materials"].append(vnum)
		else:
			slot = self.__MakeSlot()
			slot.SetParent(self)
			slot.SetPosition(15, self.dialogHeight)

			itemImage = self.__MakeItemImage()
			itemImage.SetParent(slot)
			item.SelectItem(vnum)
			itemImage.LoadImage(item.GetIconImageFileName())

		thinBoard = self.__MakeThinBoard()
		thinBoard.SetPosition(50, self.dialogHeight)
		thinBoard.SetSize(self.toolTip.GetWidth(), 20)

		textLine = ui.TextLine()
		textLine.SetParent(thinBoard)
		textLine.SetFontName(localeInfo.UI_DEF_FONT)
		textLine.SetPackedFontColor(0xffdddddd)
		textLine.SetText("%s x %02d" % (item.GetItemName(), count))
		textLine.SetOutline()
		textLine.SetFeather(False)
		textLine.SetWindowVerticalAlignCenter()
		textLine.SetVerticalAlignCenter()

		if localeInfo.IsARABIC():
			(x,y) = textLine.GetTextSize()
			textLine.SetPosition(x, 0)
		else:
			textLine.SetPosition(15, 0)

		textLine.Show()
		self.children.append(textLine)

		self.dialogHeight += 34
		self.UpdateDialog()

	def UpdateDialog(self):
		newWidth = self.toolTip.GetWidth() + 60
		if app.ENABLE_AUTO_REFINE:
			newHeight = self.dialogHeight + 69 + 40
		else:
			newHeight = self.dialogHeight + 69

		## 936 : 개량 확률 표시 안함
		##if 936 == app.GetDefaultCodePage():
		newHeight -= 8

		if localeInfo.IsARABIC():
			self.board.SetPosition(newWidth, 0)

			(x, y) = self.titleBar.GetLocalPosition()
			self.titleBar.SetPosition(newWidth - 15, y)

		self.board.SetSize(newWidth, newHeight)
		self.toolTip.SetPosition(15 + 35, 38)
		self.titleBar.SetWidth(newWidth-15)
		self.SetSize(newWidth, newHeight)

		(x, y) = self.GetLocalPosition()
		self.SetPosition(x, y)

	def OpenQuestionDialog(self):
		if 100 == self.percentage:
			self.Accept()
			return

		if 5 == self.type: ## 무신의 축복서
			self.Accept()
			return

		if app.ENABLE_STONE_OF_BLESS:
			if 7 == self.type:
				self.Accept()
				return

		if app.ENABLE_SOUL_SYSTEM:
			if 8 == self.type or 9 == self.type:
				self.Accept()
				return

		dlgQuestion = uiCommon.QuestionDialog2()
		dlgQuestion.SetText2(localeInfo.REFINE_WARNING2)
		dlgQuestion.SetAcceptEvent(ui.__mem_func__(self.Accept))
		dlgQuestion.SetCancelEvent(ui.__mem_func__(dlgQuestion.Close))

		if 3 == self.type: ## 현철
			if localeInfo.IsEUROPE():
				if dlgQuestion:
					del dlgQuestion

				dlgQuestion = uiCommon.QuestionDialog()
				dlgQuestion.SetAcceptEvent(ui.__mem_func__(self.Accept))
				dlgQuestion.SetCancelEvent(ui.__mem_func__(dlgQuestion.Close))
				dlgQuestion.SetText(localeInfo.REFINE_DESTROY_WARNING_WITH_BONUS_PERCENT_1)
			else:
				dlgQuestion.SetText1(localeInfo.REFINE_DESTROY_WARNING_WITH_BONUS_PERCENT_1)
				dlgQuestion.SetText2(localeInfo.REFINE_DESTROY_WARNING_WITH_BONUS_PERCENT_2)
		elif 2 == self.type: ## 축복서
			dlgQuestion.SetText1(localeInfo.REFINE_DOWN_GRADE_WARNING)
		else:
			dlgQuestion.SetText1(localeInfo.REFINE_DESTROY_WARNING)

		dlgQuestion.Open()
		self.dlgQuestion = dlgQuestion

	def Accept(self):
		if app.ENABLE_AUTO_REFINE:
			net.SendRefinePacket(self.targetItemPos, self.type)
		else:
			net.SendRefinePacket(self.targetItemPos, self.type)
			self.Close()

	def CancelRefine(self):
		net.SendRefinePacket(255, 255)
		self.Close()

		if app.WJ_ENABLE_TRADABLE_ICON:
			self.SetCanMouseEventSlot(self.targetItemPos)

		if app.ENABLE_AUTO_REFINE:
			osfInfo.AUTO_REFINE = False
			osfInfo.AUTO_REFINE_TYPE = 0
			osfInfo.AUTO_REFINE_DATA = {
				"ITEM" : [-1, -1],
				"NPC" : [0, -1, -1, 0]
			}

		if osfInfo.SHOW_REFINE_ITEM_DESC:
			TOOLTIP_DATA["materials"] = []

	if app.ENABLE_AUTO_REFINE:
		def AutoRefine(self, checkType, autoFlag):
			osfInfo.IS_AUTO_REFINE = autoFlag

		def CheckRefine(self, isFail):
			if osfInfo.IS_AUTO_REFINE == True:
				if osfInfo.AUTO_REFINE_TYPE == 1:
					if osfInfo.AUTO_REFINE_DATA["ITEM"][0] != -1 and osfInfo.AUTO_REFINE_DATA["ITEM"][1] != -1:
						scrollIndex = player.GetItemIndex(osfInfo.AUTO_REFINE_DATA["ITEM"][0])
						itemIndex = player.GetItemIndex(osfInfo.AUTO_REFINE_DATA["ITEM"][1])

						if int(itemIndex % 10) == 9:
							self.Close()

						if scrollIndex <= 0 or (itemIndex % 10 == 8 and not isFail):
							self.Close()
						else:
							osfInfo.AUTO_REFINE = True
							net.SendItemUseToItemPacket(osfInfo.AUTO_REFINE_DATA["ITEM"][0], osfInfo.AUTO_REFINE_DATA["ITEM"][1])

				elif osfInfo.AUTO_REFINE_TYPE == 2:
					npcData = osfInfo.AUTO_REFINE_DATA["NPC"]
					if npcData[0] != 0 and npcData[1] != -1 and npcData[2] != -1 and npcData[3] != 0:
						itemIndex = player.GetItemIndex(npcData[1], npcData[2])

						if int(itemIndex %10) == 9:
							self.Close()

						if (itemIndex % 10 == 8 and not isFail) or isFail:
							self.Close()
						else:
							osfInfo.AUTO_REFINE = True
							net.SendGiveItemPacket(npcData[0], npcData[1], npcData[2], npcData[3])
				else:
					self.Close()
			else:
				self.Close()

	def OnPressEscapeKey(self):
		self.CancelRefine()
		return True

	if app.WJ_ENABLE_TRADABLE_ICON:
		def BindInterface(self, interface):
			from _weakref import proxy
			self.interface = proxy(interface)

		def SetInven(self, inven):
			self.inven = inven

		def SetCanMouseEventSlot(self, idx):
			if idx >= INVENTORY_PAGE_SIZE:
				if app.ENABLE_EXTEND_INVEN_SYSTEM:
					page = self.inven.GetInventoryPageIndex() # 0,1,2,3
					idx -= (page * INVENTORY_PAGE_SIZE)
				else:
					idx -= INVENTORY_PAGE_SIZE

			self.inven.wndItem.SetCanMouseEventSlot(idx)

		def OnUpdate(self):
			if not self.inven:
				return

			targetItemPos = self.targetItemPos
			if targetItemPos <= 0:
				return

			page = self.inven.GetInventoryPageIndex() # range 0 ~ 1

			if (page * INVENTORY_PAGE_SIZE) <= targetItemPos < ((page + 1) * INVENTORY_PAGE_SIZE): # range 0 ~ 44, 45 ~ 89
				lock_idx = targetItemPos - (page * INVENTORY_PAGE_SIZE)
				self.inven.wndItem.SetCantMouseEventSlot(lock_idx)
