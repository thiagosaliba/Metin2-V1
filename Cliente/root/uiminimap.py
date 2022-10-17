import ui
import uiScriptLocale
import wndMgr
import player
import miniMap
import localeInfo
import net
import app
import colorInfo
import constInfo
import background

if app.ENABLE_MAILBOX:
	import mail
	import uiToolTip

if app.WJ_SHOW_PARTY_ON_MINIMAP:
	QUEST_TOOLTIP_COLOR = 0xfff2cb61

class MapTextToolTip(ui.Window):
	def __init__(self):
		ui.Window.__init__(self)

		textLine = ui.TextLine()
		textLine.SetParent(self)
		textLine.SetOutline()
		if app.WJ_SHOW_PARTY_ON_MINIMAP:
			if localeInfo.IsARABIC():
				textLine.SetHorizontalAlignLeft()
			else:
				textLine.SetHorizontalAlignRight()
		else:
			textLine.SetHorizontalAlignRight()
		textLine.Show()
		self.textLine = textLine

	def __del__(self):
		ui.Window.__del__(self)

	def SetText(self, text):
		self.textLine.SetText(text)

	def SetTooltipPosition(self, PosX, PosY):
		if app.WJ_SHOW_PARTY_ON_MINIMAP:
			self.textLine.SetPosition(PosX - 5, PosY)
		else:
			if localeInfo.IsARABIC():
				w, h = self.textLine.GetTextSize()
				self.textLine.SetPosition(PosX - w - 5, PosY)
			else:
				self.textLine.SetPosition(PosX - 5, PosY)

	def SetTextColor(self, TextColor):
		self.textLine.SetPackedFontColor(TextColor)

	def GetTextSize(self):
		return self.textLine.GetTextSize()

	def SetHorizontalAlignLeft(self):
		if self.textLine:
			self.textLine.SetHorizontalAlignLeft()

class AtlasWindow(ui.ScriptWindow):
	class AtlasRenderer(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)
			self.AddFlag("not_pick")

		def __del__(self):
			ui.Window.__del__(self)

		def OnUpdate(self):
			miniMap.UpdateAtlas()

		def OnRender(self):
			(x, y) = self.GetGlobalPosition()
			fx = float(x)
			fy = float(y)
			miniMap.RenderAtlas(fx, fy)

		def HideAtlas(self):
			miniMap.HideAtlas()

		def ShowAtlas(self):
			miniMap.ShowAtlas()

	def __init__(self):
		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Hide()
		self.infoGuildMark = ui.MarkBox()
		self.infoGuildMark.Hide()
		self.AtlasMainWindow = None
		self.mapName = ""
		self.board = 0
		self.IsShowWindowValue = False

		ui.ScriptWindow.__init__(self)

	def __del__(self):
		ui.ScriptWindow.__del__(self)

	def SetMapName(self, mapName):
		if 949 == app.GetDefaultCodePage():
			try:
				self.board.SetTitleName(localeInfo.MINIMAP_ZONE_NAME_DICT[mapName])
			except:
				pass

	def LoadWindow(self):
		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/AtlasWindow.py")
		except:
			import exception
			exception.Abort("AtlasWindow.LoadWindow.LoadScript")

		try:
			self.board = self.GetChild("board")

		except:
			import exception
			exception.Abort("AtlasWindow.LoadWindow.BindObject")

		self.AtlasMainWindow = self.AtlasRenderer()
		self.board.SetCloseEvent(self.Close)
		self.AtlasMainWindow.SetParent(self.board)
		self.AtlasMainWindow.SetPosition(7, 30)
		self.tooltipInfo.SetParent(self.board)
		self.infoGuildMark.SetParent(self.board)
		self.SetPosition(wndMgr.GetScreenWidth() - 136 - 256 - 10 - 165, 78)
		self.Hide()

		miniMap.RegisterAtlasWindow(self)

	def Destroy(self):
		miniMap.UnregisterAtlasWindow()
		self.ClearDictionary()
		self.AtlasMainWindow = None
		self.tooltipAtlasClose = 0
		self.tooltipInfo = None
		self.infoGuildMark = None
		self.board = None

	def OnUpdate(self):
		if not self.tooltipInfo:
			return

		if not self.infoGuildMark:
			return

		self.infoGuildMark.Hide()
		self.tooltipInfo.Hide()

		if False == self.board.IsIn():
			return

		(mouseX, mouseY) = wndMgr.GetMousePosition()
		(bFind, sName, iPosX, iPosY, dwTextColor, dwGuildID) = miniMap.GetAtlasInfo(mouseX, mouseY)

		if False == bFind:
			return

		if "empty_guild_area" == sName:
			sName = localeInfo.GUILD_EMPTY_AREA

		if localeInfo.IsARABIC() and sName[-1].isalnum():
			self.tooltipInfo.SetText("(%s)%d, %d" % (sName, iPosX, iPosY))
		else:
			self.tooltipInfo.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))

		(x, y) = self.GetGlobalPosition()
		self.tooltipInfo.SetTooltipPosition(mouseX - x, mouseY - y)
		self.tooltipInfo.SetTextColor(dwTextColor)
		self.tooltipInfo.Show()
		self.tooltipInfo.SetTop()

		if 0 != dwGuildID:
			textWidth, textHeight = self.tooltipInfo.GetTextSize()
			self.infoGuildMark.SetIndex(dwGuildID)
			self.infoGuildMark.SetPosition(mouseX - x - textWidth - 18 - 5, mouseY - y)
			self.infoGuildMark.Show()

	def Hide(self):
		if self.AtlasMainWindow:
			self.AtlasMainWindow.HideAtlas()
			self.AtlasMainWindow.Hide()
		ui.ScriptWindow.Hide(self)

	def Show(self):
		if self.AtlasMainWindow:
			(bGet, iSizeX, iSizeY) = miniMap.GetAtlasSize()
			if bGet:
				self.SetSize(iSizeX + 15, iSizeY + 38)

				if localeInfo.IsARABIC():
					self.board.SetPosition(iSizeX+15, 0)

				self.board.SetSize(iSizeX + 15, iSizeY + 38)
				self.SetPosition(wndMgr.GetScreenWidth() - 136 - iSizeX - 25 - 200, 78)
				#self.AtlasMainWindow.SetSize(iSizeX, iSizeY)
				self.AtlasMainWindow.ShowAtlas()
				self.AtlasMainWindow.Show()
		ui.ScriptWindow.Show(self)
		self.IsShowWindowValue = True

	def Close(self):
		self.IsShowWindowValue = False
		self.Hide()

	def IsShowWindow(self):
		return self.IsShowWindowValue

	def SetCenterPositionAdjust(self, x, y):
		self.SetPosition((wndMgr.GetScreenWidth() - self.GetWidth()) / 2 + x, (wndMgr.GetScreenHeight() - self.GetHeight()) / 2 + y)

	def OnPressEscapeKey(self):
		self.Close()
		return True

def __RegisterMiniMapColor(type, rgb):
	miniMap.RegisterColor(type, rgb[0], rgb[1], rgb[2])

class MiniMap(ui.ScriptWindow):
	CANNOT_SEE_INFO_MAP_DICT = {
		"metin2_map_monkeydungeon" : False,
		"metin2_map_monkeydungeon_02" : False,
		"metin2_map_monkeydungeon_03" : False,
		"metin2_map_devilsCatacomb" : False,
		"metin2_12zi_stage" : False,
		"metin2_map_maze_dungeon1" : False,
		"metin2_map_maze_dungeon2" : False,
		"metin2_map_maze_dungeon3" : False,
	}

	def __init__(self):
		ui.ScriptWindow.__init__(self)

		self.__Initialize()

		miniMap.Create()
		miniMap.SetScale(2.0)

		self.AtlasWindow = AtlasWindow()
		self.AtlasWindow.LoadWindow()
		self.AtlasWindow.Hide()

		self.tooltipMiniMapOpen = MapTextToolTip()
		self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)
		self.tooltipMiniMapOpen.Show()
		self.tooltipMiniMapClose = MapTextToolTip()
		self.tooltipMiniMapClose.SetText(localeInfo.UI_CLOSE)
		self.tooltipMiniMapClose.Show()
		self.tooltipScaleUp = MapTextToolTip()
		self.tooltipScaleUp.SetText(localeInfo.MINIMAP_INC_SCALE)
		self.tooltipScaleUp.Show()
		self.tooltipScaleDown = MapTextToolTip()
		self.tooltipScaleDown.SetText(localeInfo.MINIMAP_DEC_SCALE)
		self.tooltipScaleDown.Show()
		self.tooltipAtlasOpen = MapTextToolTip()
		self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		self.tooltipAtlasOpen.Show()
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.tooltipDungeonInfoOpen = MapTextToolTip()
			self.tooltipDungeonInfoOpen.SetText(localeInfo.MINIMAP_SHOW_DUNGEON_INFO)
			self.tooltipDungeonInfoOpen.Show()

		self.tooltipInfo = MapTextToolTip()
		self.tooltipInfo.Show()

		if miniMap.IsAtlas():
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_SHOW_AREAMAP)
		else:
			self.tooltipAtlasOpen.SetText(localeInfo.MINIMAP_CAN_NOT_SHOW_AREAMAP)

		self.tooltipAtlasOpen.SetHorizontalAlignLeft()
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.tooltipDungeonInfoOpen.SetHorizontalAlignLeft()

		if app.ENABLE_MAILBOX:
			self.MailBoxGMButton = None
			self.MailBoxButton = None
			self.MailBoxEffect = None
			self.tooltipMailBoxGM = uiToolTip.ToolTip()
			self.tooltipMailBoxGM.Hide()
			self.tooptipMailBox = uiToolTip.ToolTip()
			self.tooptipMailBox.Hide()

		self.mapName = ""

		self.isLoaded = 0
		self.canSeeInfo = True

		# AUTOBAN
		self.imprisonmentDuration = 0
		self.imprisonmentEndTime = 0
		self.imprisonmentEndTimeText = ""
		# END_OF_AUTOBAN

	def __del__(self):
		if app.ENABLE_MAILBOX:
			if self.MailBoxGMButton:
				del self.MailBoxGMButton
				self.MailBoxGMButton = None

			if self.tooltipMailBoxGM:
				del self.tooltipMailBoxGM
				self.tooltipMailBoxGM = None

			if self.tooptipMailBox:
				del self.tooptipMailBox
				self.tooptipMailBox = None

		miniMap.Destroy()
		ui.ScriptWindow.__del__(self)

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.interface = None

	def __Initialize(self):
		self.positionInfo = 0
		self.observerCount = 0

		self.OpenWindow = 0
		self.CloseWindow = 0
		self.ScaleUpButton = 0
		self.ScaleDownButton = 0
		self.MiniMapHideButton = 0
		self.MiniMapShowButton = 0
		self.AtlasShowButton = 0
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.DungeonInfoShowButton = 0

		self.tooltipMiniMapOpen = 0
		self.tooltipMiniMapClose = 0
		self.tooltipScaleUp = 0
		self.tooltipScaleDown = 0
		self.tooltipAtlasOpen = 0
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.tooltipDungeonInfoOpen = 0
		self.tooltipInfo = None
		self.serverInfo = None

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.interface = None

		if app.ENABLE_MAILBOX:
			self.MailBoxGMButton = None
			self.MailBoxButton = None
			self.MailBoxEffect = None
			self.tooltipMailBoxGM = None
			self.tooptipMailBox = None

	def SetMapName(self, mapName):
		self.mapName = mapName
		self.AtlasWindow.SetMapName(mapName)

		if self.CANNOT_SEE_INFO_MAP_DICT.has_key(mapName):
			self.canSeeInfo = False
			self.HideMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP_CANNOT_SEE)
		else:
			self.canSeeInfo = True
			self.ShowMiniMap()
			self.tooltipMiniMapOpen.SetText(localeInfo.MINIMAP)

	# AUTOBAN
	def SetImprisonmentDuration(self, duration):
		self.imprisonmentDuration = duration
		self.imprisonmentEndTime = app.GetGlobalTimeStamp() + duration

		self.__UpdateImprisonmentDurationText()

	def __UpdateImprisonmentDurationText(self):
		restTime = max(self.imprisonmentEndTime - app.GetGlobalTimeStamp(), 0)

		imprisonmentEndTimeText = localeInfo.SecondToDHM(restTime)
		if imprisonmentEndTimeText != self.imprisonmentEndTimeText:
			self.imprisonmentEndTimeText = imprisonmentEndTimeText
			self.serverInfo.SetText("%s: %s" % (uiScriptLocale.AUTOBAN_QUIZ_REST_TIME, self.imprisonmentEndTimeText))
	# END_OF_AUTOBAN

	def Show(self):
		self.__LoadWindow()

		ui.ScriptWindow.Show(self)

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			if localeInfo.IsARABIC():
				pyScrLoader.LoadScriptFile(self, uiScriptLocale.LOCALE_UISCRIPT_PATH + "Minimap.py")
			else:
				pyScrLoader.LoadScriptFile(self, "UIScript/MiniMap.py")
		except:
			import exception
			exception.Abort("MiniMap.LoadWindow.LoadScript")

		try:
			self.OpenWindow = self.GetChild("OpenWindow")
			self.OpenWindowBGI = self.GetChild("OpenWindowBGI")
			self.MiniMapWindow = self.GetChild("MiniMapWindow")
			self.ScaleUpButton = self.GetChild("ScaleUpButton")
			self.ScaleDownButton = self.GetChild("ScaleDownButton")
			self.MiniMapHideButton = self.GetChild("MiniMapHideButton")
			self.AtlasShowButton = self.GetChild("AtlasShowButton")
			self.CloseWindow = self.GetChild("CloseWindow")
			self.MiniMapShowButton = self.GetChild("MiniMapShowButton")
			self.positionInfo = self.GetChild("PositionInfo")
			self.observerCount = self.GetChild("ObserverCount")
			self.serverInfo = self.GetChild("ServerInfo")
			if app.ENABLE_DUNGEON_INFO_SYSTEM:
				self.DungeonInfoShowButton = self.GetChild("DungeonInfoShowButton")

			if localeInfo.IsARABIC():
				self.OpenWindowBGI.LeftRightReverse()

			if app.ENABLE_OSF_MINIMAP_INFO:
				self.dateTimeTextLine = self.GetChild("DateTimeTextLine")
				self.dateTimeTextLine.SetOutline()

				self.timeZoneTextLine = self.GetChild("TimeZoneTextLine")
				self.timeZoneTextLine.SetOutline()

				self.patchTextLine = self.GetChild("PatchTextLine")
				self.patchTextLine.SetOutline()

			if app.ENABLE_RTT_STATISTICS:
				self.rttTextLine = self.GetChild("RTTTextLine")
				self.rttTextLine.SetOutline()

				self.pktLossTextLine = self.GetChild("PacketLossTextLine")
				self.pktLossTextLine.SetOutline()

			if app.ENABLE_MAILBOX:
				self.MakeGmMailButton()
				self.MailBoxButton = self.GetChild("MailBoxButton")
				self.MailBoxButton.Hide()
				self.MailBoxEffect = self.GetChild("MailBoxEffect")
				self.MailBoxEffect.Hide()

				if localeInfo.IsARABIC():
					(mailbox_effect_x, mailbox_effect_y) = self.MailBoxEffect.GetLocalPosition()
					self.MailBoxEffect.SetPosition(mailbox_effect_x + 26, mailbox_effect_y)
		except:
			import exception
			exception.Abort("MiniMap.LoadWindow.Bind")

		if constInfo.MINIMAP_POSITIONINFO_ENABLE == 0:
			self.positionInfo.Hide()

		self.serverInfo.SetText(net.GetServerInfo())
		self.ScaleUpButton.SetEvent(ui.__mem_func__(self.ScaleUp))
		self.ScaleDownButton.SetEvent(ui.__mem_func__(self.ScaleDown))
		self.MiniMapHideButton.SetEvent(ui.__mem_func__(self.HideMiniMap))
		self.MiniMapShowButton.SetEvent(ui.__mem_func__(self.ShowMiniMap))

		if miniMap.IsAtlas():
			self.AtlasShowButton.SetEvent(ui.__mem_func__(self.ShowAtlas))

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.DungeonInfoShowButton.SetEvent(ui.__mem_func__(self.ShowDungeonInfo))

		(ButtonPosX, ButtonPosY) = self.MiniMapShowButton.GetGlobalPosition()
		self.tooltipMiniMapOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.MiniMapHideButton.GetGlobalPosition()
		self.tooltipMiniMapClose.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleUpButton.GetGlobalPosition()
		self.tooltipScaleUp.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.ScaleDownButton.GetGlobalPosition()
		self.tooltipScaleDown.SetTooltipPosition(ButtonPosX, ButtonPosY)

		(ButtonPosX, ButtonPosY) = self.AtlasShowButton.GetGlobalPosition()
		self.tooltipAtlasOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)

		if app.ENABLE_MAILBOX:
			if self.MailBoxButton and self.tooptipMailBox:
				(ButtonPosX, ButtonPosY) = self.MailBoxButton.GetGlobalPosition()
				self.tooptipMailBox.SetToolTipPosition(ButtonPosX, ButtonPosY)

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			(ButtonPosX, ButtonPosY) = self.DungeonInfoShowButton.GetGlobalPosition()
			self.tooltipDungeonInfoOpen.SetTooltipPosition(ButtonPosX, ButtonPosY)

		self.ShowMiniMap()

	def Destroy(self):
		self.HideMiniMap()

		self.AtlasWindow.Destroy()
		self.AtlasWindow = None

		self.ClearDictionary()

		self.__Initialize()

	def UpdateObserverCount(self, observerCount):
		if observerCount>0:
			self.observerCount.Show()
		elif observerCount<=0:
			self.observerCount.Hide()

		self.observerCount.SetText(localeInfo.MINIMAP_OBSERVER_COUNT % observerCount)

	def OnUpdate(self):
		(x, y, z) = player.GetMainCharacterPosition()
		miniMap.Update(x, y)

		self.positionInfo.SetText("(%.0f, %.0f)" % (x/100, y/100))

		if app.ENABLE_OSF_MINIMAP_INFO:
			if self.dateTimeTextLine and self.timeZoneTextLine:
				self.dateTimeTextLine.SetText(app.GetCurrentTime())
				self.timeZoneTextLine.SetText(app.GetTimeZone())

		if app.ENABLE_RTT_STATISTICS:
			if self.rttTextLine and self.pktLossTextLine:
				rtt = app.GetRTT()
				pktLossPct = app.GetPacketLossPct()

				if rtt > 300:
					self.rttTextLine.SetFontColor((255.00 / 255), (0.00 / 255), (0.00 / 255)) # r, g, b
				elif rtt > 100 and rtt < 300:
					self.rttTextLine.SetFontColor((255.00 / 255), (255.00 / 255), (0.00 / 255)) # r, g, b
				else:
					self.rttTextLine.SetFontColor((0.00 / 255), (255.00 / 255), (0.00 / 255)) # r, g, b

				self.rttTextLine.SetText("Ping: %s%dms" % ("+" if rtt > 999 else "", 999 if rtt > 999 else rtt))
				self.pktLossTextLine.SetText("(%.2f%% pkt loss)" % (pktLossPct))

		if self.tooltipInfo:
			if 1 == self.MiniMapWindow.IsIn():
				(mouseX, mouseY) = wndMgr.GetMousePosition()
				(bFind, sName, iPosX, iPosY, dwTextColor) = miniMap.GetInfo(mouseX, mouseY)
				if bFind == 0:
					self.tooltipInfo.Hide()
				elif not self.canSeeInfo:
					self.tooltipInfo.SetText("%s(%s)" % (sName, localeInfo.UI_POS_UNKNOWN))
					self.tooltipInfo.SetTooltipPosition(mouseX - 5, mouseY)
					self.tooltipInfo.SetTextColor(dwTextColor)
					self.tooltipInfo.Show()
				else:
					if localeInfo.IsARABIC() and sName[-1].isalnum():
						self.tooltipInfo.SetText("(%s)%d, %d" % (sName, iPosX, iPosY))
					else:
						self.tooltipInfo.SetText("%s(%d, %d)" % (sName, iPosX, iPosY))
					self.tooltipInfo.SetTooltipPosition(mouseX - 5, mouseY)
					self.tooltipInfo.SetTextColor(dwTextColor)
					self.tooltipInfo.Show()
			else:
				self.tooltipInfo.Hide()

			# AUTOBAN
			if self.imprisonmentDuration:
				self.__UpdateImprisonmentDurationText()
			# END_OF_AUTOBAN

		if True == self.MiniMapShowButton.IsIn():
			self.tooltipMiniMapOpen.Show()
		else:
			self.tooltipMiniMapOpen.Hide()

		if True == self.MiniMapHideButton.IsIn():
			self.tooltipMiniMapClose.Show()
		else:
			self.tooltipMiniMapClose.Hide()

		if True == self.ScaleUpButton.IsIn():
			self.tooltipScaleUp.Show()
		else:
			self.tooltipScaleUp.Hide()

		if True == self.ScaleDownButton.IsIn():
			self.tooltipScaleDown.Show()
		else:
			self.tooltipScaleDown.Hide()

		if True == self.AtlasShowButton.IsIn():
			self.tooltipAtlasOpen.Show()
		else:
			self.tooltipAtlasOpen.Hide()

		if app.ENABLE_MAILBOX:
			if self.MailBoxGMButton:
				if True == self.MailBoxGMButton.IsIn():
					self.tooltipMailBoxGM.Show()
				else:
					self.tooltipMailBoxGM.Hide()

			if self.MailBoxButton:
				if True == self.MailBoxButton.IsIn():
					if self.MailBoxEffect:
						self.MailBoxEffect.Hide()
					self.tooptipMailBox.Show()
				else:
					self.tooptipMailBox.Hide()

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if True == self.DungeonInfoShowButton.IsIn():
				self.tooltipDungeonInfoOpen.Show()
			else:
				self.tooltipDungeonInfoOpen.Hide()

	def OnRender(self):
		(x, y) = self.GetGlobalPosition()
		fx = float(x)
		fy = float(y)
		miniMap.Render(fx + 4.0, fy + 5.0)

	def Close(self):
		self.HideMiniMap()

	def HideMiniMap(self):
		miniMap.Hide()
		self.OpenWindow.Hide()
		self.CloseWindow.Show()

	def ShowMiniMap(self):
		if not self.canSeeInfo:
			return

		miniMap.Show()
		self.OpenWindow.Show()
		self.CloseWindow.Hide()

	def isShowMiniMap(self):
		return miniMap.isShow()

	def ScaleUp(self):
		miniMap.ScaleUp()

	def ScaleDown(self):
		miniMap.ScaleDown()

	def ShowAtlas(self):
		if not miniMap.IsAtlas():
			return

		if not self.AtlasWindow.IsShow():
			self.AtlasWindow.Show()

	def ToggleAtlasWindow(self):
		if not miniMap.IsAtlas():
			return

		if self.AtlasWindow.IsShow():
			self.AtlasWindow.Hide()
		else:
			self.AtlasWindow.Show()

	if app.ENABLE_MOVE_CHANNEL:
		def RefreshServerInfo(self):
			self.serverInfo.SetText(net.GetServerInfo())

	if app.ENABLE_DUNGEON_INFO_SYSTEM:
		def ShowDungeonInfo(self):
			self.interface.ToggleDungeonInfoWindow()

		def BindInterfaceClass(self, interface):
			from _weakref import proxy
			self.interface = proxy(interface)

	if app.ENABLE_MAILBOX:
		def MiniMapMailProcess(self, type, data):
			if mail.MAILBOX_GC_UNREAD_DATA == type:
				self.MiniMapMailRefresh(data)

			elif mail.MAILBOX_GC_SYSTEM_CLOSE == type:
				self.MiniMapMailSystemClose()

		def MiniMapMailRefresh(self, data):
			(is_flash, total_count, item_count, common_count, is_gm_post_visible) = data

			if 0 == total_count:
				if self.MailBoxButton:
					self.MailBoxButton.Hide()
				if self.MailBoxEffect:
					self.MailBoxEffect.Hide()
			else:
				if self.MailBoxButton:
					self.MailBoxButton.Show()

				if True == is_flash and self.MailBoxEffect:
					self.MailBoxEffect.ResetFrame()
					self.MailBoxEffect.Show()
				else:
					self.MailBoxEffect.Hide()

				if self.tooptipMailBox:
					text1 = localeInfo.MAILBOX_POST_NOT_CONFIRM_INFO_1 % (total_count)
					text2 = localeInfo.MAILBOX_POST_NOT_CONFIRM_INFO_2 % (common_count, item_count)
					self.tooptipMailBox.ClearToolTip()
					self.tooptipMailBox.SetThinBoardSize(11 * len(text1))
					self.tooptipMailBox.AppendTextLine(text1)
					self.tooptipMailBox.AppendTextLine(text2)
			# gm ¿ìÆí Ç¥½Ã
			self.MailBoxGMButtonVisible(is_gm_post_visible)

		def	MiniMapMailSystemClose(self):
			if self.MailBoxButton:
				self.MailBoxButton.Hide()
			if self.MailBoxEffect:
				self.MailBoxEffect.Hide()
			if self.tooptipMailBox:
				self.tooptipMailBox.Hide()
			if self.MailBoxGMButton:
				self.MailBoxGMButton.Hide()
			if self.tooltipMailBoxGM:
				self.tooltipMailBoxGM.Hide()

		def MakeGmMailButton(self):
			SCREEN_WIDTH = wndMgr.GetScreenWidth()
			# create button
			self.MailBoxGMButton = ui.ExpandedImageBox()
			self.MailBoxGMButton.LoadImage("d:/ymir work/ui/game/mailbox/mailbox_icon_gm.sub")
			self.MailBoxGMButton.SetScale(0.8, 0.8)
			self.MailBoxGMButton.SetPosition(SCREEN_WIDTH - 136 - 30, 0)
			self.MailBoxGMButton.Hide()
			self.MailBoxGMButton.SetEvent(ui.__mem_func__(self.MailBoxGMButtonOverInEvent), "mouse_over_in", 0)
			self.MailBoxGMButton.SetEvent(ui.__mem_func__(self.MailBoxGMButtonOverOutEvent), "mouse_over_out", 0)

			if localeInfo.IsARABIC():
				self.MailBoxGMButton.SetPosition(SCREEN_WIDTH-136-30-10, 0)

			# tooltip setting
			text = localeInfo.MAILBOX_POST_GM_ARRIVE
			self.tooltipMailBoxGM.ClearToolTip()
			self.tooltipMailBoxGM.SetThinBoardSize(11 * len(text))
			self.tooltipMailBoxGM.AppendTextLine(text)

		def MailBoxGMButtonOverInEvent(self):
			if self.tooltipMailBoxGM:
				self.tooltipMailBoxGM.Show()

		def MailBoxGMButtonOverOutEvent(self):
			if self.tooltipMailBoxGM:
				self.tooltipMailBoxGM.Hide()

		def MailBoxGMButtonVisible(self, visible):
			if not self.MailBoxGMButton:
				return
			if not self.tooltipMailBoxGM:
				return

			if True == visible:
				self.MailBoxGMButton.Show()
				self.MailBoxGMButton.SetTop()
			else:
				self.MailBoxGMButton.Hide()
				self.tooltipMailBoxGM.Hide()
