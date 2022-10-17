import os
import app
import dbg
import grp
import item
import background
import chr
import chrmgr
import player
import snd
import chat
import textTail
import snd
import net
import effect
import wndMgr
#import fly
import systemSetting
import quest
import guild
import skill
import messenger
import localeInfo
import constInfo
import osfInfo
import exchange
import ime

import ui
import uiCommon
import uiPhaseCurtain
import uiMapNameShower
import uiAffectShower
import uiPlayerGauge
import uiCharacter
import uiTarget

# PRIVATE_SHOP_PRICE_LIST
import uiPrivateShopBuilder
# END_OF_PRIVATE_SHOP_PRICE_LIST

import mouseModule
import consoleModule
import localeInfo

import playerSettingModule
import interfaceModule

import musicInfo
import debugInfo
import stringCommander

if app.ENABLE_KEYCHANGE_SYSTEM:
	import uiKeyChange

from _weakref import proxy

# TEXTTAIL_LIVINGTIME_CONTROL
#if localeInfo.IsJAPAN():
#	app.SetTextTailLivingTime(8.0)
# END_OF_TEXTTAIL_LIVINGTIME_CONTROL

# SCREENSHOT_CWDSAVE
SCREENSHOT_CWDSAVE = False
SCREENSHOT_DIR = None

if localeInfo.IsEUROPE():
	SCREENSHOT_CWDSAVE = True

if localeInfo.IsCIBN10():
	SCREENSHOT_CWDSAVE = False
	SCREENSHOT_DIR = "YT2W"

cameraDistance = 1550.0
cameraPitch = 27.0
cameraRotation = 0.0
cameraHeight = 100.0

testAlignment = 0

class GameWindow(ui.ScriptWindow):
	def __init__(self, stream):
		ui.ScriptWindow.__init__(self, "GAME")
		self.SetWindowName("game")
		net.SetPhaseWindow(net.PHASE_WINDOW_GAME, self)
		player.SetGameWindow(self)

		self.quickSlotPageIndex = 0
		self.lastPKModeSendedTime = 0
		self.pressNumber = None

		self.guildWarQuestionDialog = None
		self.interface = None
		self.targetBoard = None
		if app.ENABLE_SHIP_DEFENSE:
			self.allyTargetBoard = None
		self.console = None
		self.mapNameShower = None
		self.affectShower = None
		self.playerGauge = None
		if app.ENABLE_KEYCHANGE_SYSTEM:
			self.wndKeyChange = None

		self.stream=stream
		self.interface = interfaceModule.Interface()
		self.interface.MakeInterface()
		self.interface.ShowDefaultWindows()

		self.curtain = uiPhaseCurtain.PhaseCurtain()
		self.curtain.speed = 0.03
		self.curtain.Hide()

		self.targetBoard = uiTarget.TargetBoard()
		self.targetBoard.SetWhisperEvent(ui.__mem_func__(self.interface.OpenWhisperDialog))
		self.targetBoard.Hide()
		self.interface.SettargetBoard(self.targetBoard)

		if app.ENABLE_SHIP_DEFENSE:
			self.allyTargetBoard = uiTarget.AllianceTargetBoard()
			self.allyTargetBoard.Hide()
			self.interface.SetAllianceTargetBoard(self.allyTargetBoard)

		self.console = consoleModule.ConsoleWindow()
		self.console.BindGameClass(self)
		self.console.SetConsoleSize(wndMgr.GetScreenWidth(), 200)
		self.console.Hide()

		self.mapNameShower = uiMapNameShower.MapNameShower()
		self.affectShower = uiAffectShower.AffectShower()
		self.interface.SetAffectShower(self.affectShower)

		self.playerGauge = uiPlayerGauge.PlayerGauge(self)
		self.playerGauge.Hide()

		#wj 2014.1.2. ESCŰ�� ���� �� �켱������ DropQuestionDialog�� ������ �������. ������ ó���� itemDropQuestionDialog�� ����Ǿ� ���� �ʾ� ERROR�� �߻��Ͽ� init���� ����� ���ÿ� �ʱ�ȭ ��Ŵ.
		self.itemDropQuestionDialog = None

		self.__SetQuickSlotMode()

		self.__ServerCommand_Build()
		self.__ProcessPreservedServerCommand()

		if app.ENABLE_KEYCHANGE_SYSTEM:
			self.wndKeyChange = uiKeyChange.KeyChangeWindow(self, self.interface)
			self.ADDKEYBUFFERCONTROL = player.KEY_ADDKEYBUFFERCONTROL
			self.ADDKEYBUFFERALT = player.KEY_ADDKEYBUFFERALT
			self.ADDKEYBUFFERSHIFT = player.KEY_ADDKEYBUFFERSHIFT

		self.partyRequestQuestionDialog = None
		self.partyInviteQuestionDialog = None

	def __del__(self):
		player.SetGameWindow(0)
		net.ClearPhaseWindow(net.PHASE_WINDOW_GAME, self)
		ui.ScriptWindow.__del__(self)

	def Open(self):
		app.SetFrameSkip(1)

		self.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())

		self.quickSlotPageIndex = 0
		self.PickingCharacterIndex = -1
		self.PickingItemIndex = -1
		self.consoleEnable = False
		self.isShowDebugInfo = False
		self.ShowNameFlag = False

		self.enableXMasBoom = False
		self.startTimeXMasBoom = 0.0
		self.indexXMasBoom = 0

		global cameraDistance, cameraPitch, cameraRotation, cameraHeight

		app.SetCamera(cameraDistance, cameraPitch, cameraRotation, cameraHeight)

		constInfo.SET_DEFAULT_CAMERA_MAX_DISTANCE()
		constInfo.SET_DEFAULT_CHRNAME_COLOR()
		constInfo.SET_DEFAULT_FOG_LEVEL()
		constInfo.SET_DEFAULT_CONVERT_EMPIRE_LANGUAGE_ENABLE()
		constInfo.SET_DEFAULT_USE_ITEM_WEAPON_TABLE_ATTACK_BONUS()
		constInfo.SET_DEFAULT_USE_SKILL_EFFECT_ENABLE()

		# TWO_HANDED_WEAPON_ATTACK_SPEED_UP
		constInfo.SET_TWO_HANDED_WEAPON_ATT_SPEED_DECREASE_VALUE()
		# END_OF_TWO_HANDED_WEAPON_ATTACK_SPEED_UP

		import event
		event.SetLeftTimeString(localeInfo.UI_LEFT_TIME)

		textTail.EnablePKTitle(constInfo.PVPMODE_ENABLE)

		#if constInfo.PVPMODE_TEST_ENABLE:
			#self.testPKMode = ui.TextLine()
			#self.testPKMode.SetFontName(localeInfo.UI_DEF_FONT)
			#self.testPKMode.SetPosition(0, 15)
			#self.testPKMode.SetWindowHorizontalAlignCenter()
			#self.testPKMode.SetHorizontalAlignCenter()
			#self.testPKMode.SetFeather()
			#self.testPKMode.SetOutline()
			#self.testPKMode.Show()

			#self.testAlignment = ui.TextLine()
			#self.testAlignment.SetFontName(localeInfo.UI_DEF_FONT)
			#self.testAlignment.SetPosition(0, 35)
			#self.testAlignment.SetWindowHorizontalAlignCenter()
			#self.testAlignment.SetHorizontalAlignCenter()
			#self.testAlignment.SetFeather()
			#self.testAlignment.SetOutline()
			#self.testAlignment.Show()

		if app.ENABLE_KEYCHANGE_SYSTEM:
			pass
		else:
			self.__BuildKeyDict()

		self.__BuildDebugInfo()

		# PRIVATE_SHOP_PRICE_LIST
		uiPrivateShopBuilder.Clear()
		# END_OF_PRIVATE_SHOP_PRICE_LIST

		# UNKNOWN_UPDATE
		exchange.InitTrading()
		# END_OF_UNKNOWN_UPDATE

		if debugInfo.IsDebugMode() or osfInfo.IsDebugMode():
			self.ToggleDebugInfo()

		## Sound
		snd.SetMusicVolume(systemSetting.GetMusicVolume()*net.GetFieldMusicVolume())
		snd.SetSoundVolume(systemSetting.GetSoundVolume())

		if app.ENABLE_FOG_FIX:
			if systemSetting.GetFogMode():
				background.SetFogMode(True)
			else:
				background.SetFogMode(False)

		netFieldMusicFileName = net.GetFieldMusicFileName()
		if netFieldMusicFileName:
			snd.FadeInMusic("BGM/" + netFieldMusicFileName)
		elif musicInfo.fieldMusic != "":
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

		self.__SetQuickSlotMode()
		self.__SelectQuickPage(self.quickSlotPageIndex)

		self.SetFocus()
		self.Show()
		app.ShowCursor()

		net.SendEnterGamePacket()

		#if app.ENABLE_MYSHOP_DECO:
		#	app.MyShopDecoBGCreate()

		# START_GAME_ERROR_EXIT
		try:
			self.StartGame()
		except:
			import exception
			exception.Abort("GameWindow.Open")
		# END_OF_START_GAME_ERROR_EXIT

		# NPC�� ť��ý������� ���� �� �ִ� �����۵��� ����� ĳ��
		# ex) cubeInformation[20383] = [ {"rewordVNUM": 72723, "rewordCount": 1, "materialInfo": "101,1&102,2", "price": 999 }, ... ]
		self.cubeInformation = {}
		self.currentCubeNPC = 0

		mouseModule.mouseController.CreateNumberLine()

		self.__LateSummerEvent()

	def Close(self):
		self.Hide()

		global cameraDistance, cameraPitch, cameraRotation, cameraHeight
		(cameraDistance, cameraPitch, cameraRotation, cameraHeight) = app.GetCamera()

		if musicInfo.fieldMusic != "":
			snd.FadeOutMusic("BGM/"+ musicInfo.fieldMusic)

		self.onPressKeyDict = None
		self.onClickKeyDict = None

		chat.Close()
		snd.StopAllSound()
		grp.InitScreenEffect()
		chr.Destroy()
		textTail.Clear()
		quest.Clear()
		background.Destroy()
		guild.Destroy()
		messenger.Destroy()
		skill.ClearSkillData()
		wndMgr.Unlock()
		mouseModule.mouseController.DeattachObject()

		if self.guildWarQuestionDialog:
			self.guildWarQuestionDialog.Close()

		self.guildNameBoard = None
		self.partyRequestQuestionDialog = None
		self.partyInviteQuestionDialog = None
		self.guildInviteQuestionDialog = None
		self.guildWarQuestionDialog = None
		self.messengerAddFriendQuestion = None

		# UNKNOWN_UPDATE
		self.itemDropQuestionDialog = None
		# END_OF_UNKNOWN_UPDATE

		# QUEST_CONFIRM
		self.confirmDialog = None
		# END_OF_QUEST_CONFIRM

		self.PrintCoord = None
		self.FrameRate = None
		self.Pitch = None
		self.Splat = None
		self.TextureNum = None
		self.ObjectNum = None
		self.ViewDistance = None
		self.PrintMousePos = None

		self.ClearDictionary()

		self.playerGauge = None
		self.mapNameShower = None
		self.affectShower = None

		if self.console:
			self.console.BindGameClass(0)
			self.console.Close()
			self.console = None

		if self.interface:
			self.interface.HideAllWindows()
			self.interface.Close()
			del self.interface
			self.interface = None

		if self.targetBoard:
			self.targetBoard.Destroy()
			self.targetBoard = None

		if app.ENABLE_SHIP_DEFENSE:
			if self.allyTargetBoard:
				self.allyTargetBoard.Destroy()
				self.allyTargetBoard = None

		player.ClearSkillDict()
		player.ResetCameraRotation()

		self.KillFocus()
		app.HideCursor()

		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

		if app.ENABLE_KEYCHANGE_SYSTEM:
			if self.wndKeyChange:
				self.wndKeyChange.KeyChangeWindowClose(True)
				self.wndKeyChange = None

		mouseModule.mouseController.Destroy()
		#print "---------------------------------------------------------------------------- CLOSE GAME WINDOW"

	def __BuildKeyDict(self):
		onPressKeyDict = {}

		# PressKey �� ������ �ִ� ���� ��� ����Ǵ� Ű�̴�.

		## ���� ����Ű �����Կ� �̿�ȴ�.(���� ���ڵ鵵 �� ���Կ� ����)
		## F12 �� Ŭ�� ����׿� Ű�̹Ƿ� ���� �ʴ� �� ����.
		onPressKeyDict[app.DIK_1] = lambda : self.__PressNumKey(1)
		onPressKeyDict[app.DIK_2] = lambda : self.__PressNumKey(2)
		onPressKeyDict[app.DIK_3] = lambda : self.__PressNumKey(3)
		onPressKeyDict[app.DIK_4] = lambda : self.__PressNumKey(4)
		onPressKeyDict[app.DIK_5] = lambda : self.__PressNumKey(5)
		onPressKeyDict[app.DIK_6] = lambda : self.__PressNumKey(6)
		onPressKeyDict[app.DIK_7] = lambda : self.__PressNumKey(7)
		onPressKeyDict[app.DIK_8] = lambda : self.__PressNumKey(8)
		onPressKeyDict[app.DIK_9] = lambda : self.__PressNumKey(9)
		onPressKeyDict[app.DIK_F1] = lambda : self.__PressQuickSlot(4)
		onPressKeyDict[app.DIK_F2] = lambda : self.__PressQuickSlot(5)
		onPressKeyDict[app.DIK_F3] = lambda : self.__PressQuickSlot(6)
		onPressKeyDict[app.DIK_F4] = lambda : self.__PressQuickSlot(7)

		onPressKeyDict[app.DIK_LALT] = lambda : self.ShowName()
		onPressKeyDict[app.DIK_LCONTROL] = lambda : self.ShowMouseImage()
		onPressKeyDict[app.DIK_SYSRQ] = lambda : self.SaveScreen()
		onPressKeyDict[app.DIK_SPACE] = lambda : self.StartAttack()

		# ĳ���� �̵�Ű
		onPressKeyDict[app.DIK_UP] = lambda : self.MoveUp()
		onPressKeyDict[app.DIK_DOWN] = lambda : self.MoveDown()
		onPressKeyDict[app.DIK_LEFT] = lambda : self.MoveLeft()
		onPressKeyDict[app.DIK_RIGHT] = lambda : self.MoveRight()
		onPressKeyDict[app.DIK_W] = lambda : self.MoveUp()
		onPressKeyDict[app.DIK_S] = lambda : self.MoveDown()
		onPressKeyDict[app.DIK_A] = lambda : self.MoveLeft()
		onPressKeyDict[app.DIK_D] = lambda : self.MoveRight()

		onPressKeyDict[app.DIK_E] = self.__PressEKey
		onPressKeyDict[app.DIK_R] = lambda: app.ZoomCamera(app.CAMERA_TO_NEGATIVE)
		#onPressKeyDict[app.DIK_F] = lambda: app.ZoomCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_T] = lambda: app.PitchCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_G] = self.__PressGKey
		onPressKeyDict[app.DIK_Q] = self.__PressQKey

		onPressKeyDict[app.DIK_NUMPAD9] = lambda: app.MovieResetCamera()
		onPressKeyDict[app.DIK_NUMPAD4] = lambda: app.MovieRotateCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_NUMPAD6] = lambda: app.MovieRotateCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_PGUP] = lambda: app.MovieZoomCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_PGDN] = lambda: app.MovieZoomCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_NUMPAD8] = lambda: app.MoviePitchCamera(app.CAMERA_TO_NEGATIVE)
		onPressKeyDict[app.DIK_NUMPAD2] = lambda: app.MoviePitchCamera(app.CAMERA_TO_POSITIVE)
		onPressKeyDict[app.DIK_GRAVE] = lambda : self.PickUpItem()
		if app.ENABLE_QUICK_PICKUP:
			onPressKeyDict[app.DIK_Z] = lambda : self.PickUpAllItems()
		else:
			onPressKeyDict[app.DIK_Z] = lambda : self.PickUpItem()
		onPressKeyDict[app.DIK_C] = lambda state = "STATUS": self.interface.ToggleCharacterWindow(state)
		onPressKeyDict[app.DIK_V] = lambda state = "SKILL": self.interface.ToggleCharacterWindow(state)
		#onPressKeyDict[app.DIK_B] = lambda state = "EMOTICON": self.interface.ToggleCharacterWindow(state)
		onPressKeyDict[app.DIK_N] = lambda state = "QUEST": self.interface.ToggleCharacterWindow(state)
		onPressKeyDict[app.DIK_I] = lambda : self.interface.ToggleInventoryWindow()
		onPressKeyDict[app.DIK_O] = lambda : self.__PressOKey()

		onPressKeyDict[app.DIK_M] = lambda : self.interface.PressMKey()
		#onPressKeyDict[app.DIK_H] = lambda : self.interface.OpenHelpWindow()
		onPressKeyDict[app.DIK_ADD] = lambda : self.interface.MiniMapScaleUp()
		onPressKeyDict[app.DIK_SUBTRACT] = lambda : self.interface.MiniMapScaleDown()
		onPressKeyDict[app.DIK_L] = lambda : self.interface.ToggleChatLogWindow()
		onPressKeyDict[app.DIK_COMMA] = lambda : self.ShowConsole() # "`" key
		onPressKeyDict[app.DIK_LSHIFT] = lambda : self.__SetQuickPageMode()

		onPressKeyDict[app.DIK_J] = lambda : self.__PressJKey()
		onPressKeyDict[app.DIK_H] = lambda : self.__PressHKey()
		onPressKeyDict[app.DIK_B] = lambda : self.__PressBKey()
		onPressKeyDict[app.DIK_F] = lambda : self.__PressFKey()

		# CUBE_TEST
		#onPressKeyDict[app.DIK_K] = lambda : self.interface.OpenCubeWindow()
		# CUBE_TEST_END

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			onPressKeyDict[app.DIK_K] = lambda : self.interface.ToggleSpecialInventoryWindow()

		self.onPressKeyDict = onPressKeyDict

		onClickKeyDict = {}
		onClickKeyDict[app.DIK_UP] = lambda : self.StopUp()
		onClickKeyDict[app.DIK_DOWN] = lambda : self.StopDown()
		onClickKeyDict[app.DIK_LEFT] = lambda : self.StopLeft()
		onClickKeyDict[app.DIK_RIGHT] = lambda : self.StopRight()
		onClickKeyDict[app.DIK_SPACE] = lambda : self.EndAttack()

		onClickKeyDict[app.DIK_W] = lambda : self.StopUp()
		onClickKeyDict[app.DIK_S] = lambda : self.StopDown()
		onClickKeyDict[app.DIK_A] = lambda : self.StopLeft()
		onClickKeyDict[app.DIK_D] = lambda : self.StopRight()
		onClickKeyDict[app.DIK_Q] = lambda: app.RotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_E] = lambda: app.RotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_R] = lambda: app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_F] = lambda: app.ZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_T] = lambda: app.PitchCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_G] = lambda: self.__ReleaseGKey()
		onClickKeyDict[app.DIK_NUMPAD4] = lambda: app.MovieRotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_NUMPAD6] = lambda: app.MovieRotateCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_PGUP] = lambda: app.MovieZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_PGDN] = lambda: app.MovieZoomCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_NUMPAD8] = lambda: app.MoviePitchCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_NUMPAD2] = lambda: app.MoviePitchCamera(app.CAMERA_STOP)
		onClickKeyDict[app.DIK_LALT] = lambda: self.HideName()
		onClickKeyDict[app.DIK_LCONTROL] = lambda: self.HideMouseImage()
		onClickKeyDict[app.DIK_LSHIFT] = lambda: self.__SetQuickSlotMode()

		#if constInfo.PVPMODE_ACCELKEY_ENABLE:
		#	onClickKeyDict[app.DIK_B] = lambda: self.ChangePKMode()

		self.onClickKeyDict=onClickKeyDict

	def __PressNumKey(self,num):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):

			if num >= 1 and num <= 9:
				if (chrmgr.IsPossibleEmoticon(-1)):
					chrmgr.SetEmoticon(-1, int(num)-1)
					net.SendEmoticon(int(num)-1)
		else:
			if num >= 1 and num <= 4:
				self.pressNumber(num-1)

	def __ClickBKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			return
		else:
			if constInfo.PVPMODE_ACCELKEY_ENABLE:
				self.ChangePKMode()

	def __PressJKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if player.IsMountingHorse():
				net.SendChatPacket("/unmount")
			else:
				#net.SendChatPacket("/user_horse_ride")
				if not uiPrivateShopBuilder.IsBuildingPrivateShop():
					for i in xrange(player.INVENTORY_PAGE_SIZE):
						if player.GetItemIndex(i) in (71114, 71116, 71118, 71120):
							net.SendItemUsePacket(i)
							break

	def __PressHKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/user_horse_ride")
		else:
			self.interface.OpenHelpWindow()

	def __PressOKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if self.interface.wndDragonSoul:
				self.interface.wndDragonSoul.ActivateButtonClick()
		else:
			self.interface.ToggleDragonSoulWindowWithNoInfo()

	def __PressBKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/user_horse_back")
		else:
			state = "EMOTICON"
			self.interface.ToggleCharacterWindow(state)

	def __PressFKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/user_horse_feed")
		else:
			app.ZoomCamera(app.CAMERA_TO_POSITIVE)

	def __PressGKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			net.SendChatPacket("/ride")
		else:
			if self.ShowNameFlag:
				self.interface.ToggleGuildWindow()
			else:
				app.PitchCamera(app.CAMERA_TO_POSITIVE)

	def __ReleaseGKey(self):
		app.PitchCamera(app.CAMERA_STOP)

	def __PressQKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if 0 == interfaceModule.IsQBHide:
				interfaceModule.IsQBHide = 1
				self.interface.HideAllQuestButton()
			else:
				interfaceModule.IsQBHide = 0
				self.interface.ShowAllQuestButton()
		else:
			app.RotateCamera(app.CAMERA_TO_NEGATIVE)

	def __PressEKey(self):
		if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
			if 0 == interfaceModule.IsWisperHide:
				interfaceModule.IsWisperHide = 1
				self.interface.HideAllWhisperButton()
			else:
				interfaceModule.IsWisperHide= 0
				self.interface.ShowAllWhisperButton()
		else:
			app.RotateCamera(app.CAMERA_TO_POSITIVE)

	def __SetQuickSlotMode(self):
		self.pressNumber = ui.__mem_func__(self.__PressQuickSlot)

	def __SetQuickPageMode(self):
		self.pressNumber = ui.__mem_func__(self.__SelectQuickPage)

	def __PressQuickSlot(self, localSlotIndex):
		if localeInfo.IsARABIC():
			if 0 <= localSlotIndex and localSlotIndex < 4:
				player.RequestUseLocalQuickSlot(3-localSlotIndex)
			else:
				player.RequestUseLocalQuickSlot(11-localSlotIndex)
		else:
			player.RequestUseLocalQuickSlot(localSlotIndex)

	if app.ENABLE_KEYCHANGE_SYSTEM:
		def OpenKeyChangeWindow(self):
			self.wndKeyChange.Open()

		def OpenWindow(self, type, state):
			if type == player.KEY_OPEN_STATE:
				self.interface.ToggleCharacterWindow(state)
			elif type == player.KEY_OPEN_INVENTORY:
				self.interface.ToggleInventoryWindow()
			elif type == player.KEY_OPEN_DDS:
				self.interface.ToggleDragonSoulWindowWithNoInfo()
			elif type == player.KEY_OPEN_MINIMAP:
				self.interface.ToggleMiniMap()
			elif type == player.KEY_OPEN_LOGCHAT:
				self.interface.ToggleChatLogWindow()
			elif type == player.KEY_OPEN_GUILD:
				self.interface.ToggleGuildWindow()
			elif type == player.KEY_OPEN_MESSENGER:
				self.interface.ToggleMessenger()
			elif type == player.KEY_OPEN_HELP:
				self.interface.ToggleHelpWindow()

			if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
				if type == player.KEY_SPECIAL_INVENTORY:
					self.interface.ToggleSpecialInventoryWindow()

		def ScrollOnOff(self):
			if 0 == interfaceModule.IsQBHide:
				interfaceModule.IsQBHide = 1
				self.interface.HideAllQuestButton()
			else:
				interfaceModule.IsQBHide = 0
				self.interface.ShowAllQuestButton()

	def __SelectQuickPage(self, pageIndex):
		self.quickSlotPageIndex = pageIndex
		player.SetQuickPage(pageIndex)

	def ToggleDebugInfo(self):
		self.isShowDebugInfo = not self.isShowDebugInfo

		if self.isShowDebugInfo:
			self.PrintCoord.Show()
			self.FrameRate.Show()
			self.Pitch.Show()
			self.Splat.Show()
			self.TextureNum.Show()
			self.ObjectNum.Show()
			self.ViewDistance.Show()
			self.PrintMousePos.Show()
		else:
			self.PrintCoord.Hide()
			self.FrameRate.Hide()
			self.Pitch.Hide()
			self.Splat.Hide()
			self.TextureNum.Hide()
			self.ObjectNum.Hide()
			self.ViewDistance.Hide()
			self.PrintMousePos.Hide()

	def __BuildDebugInfo(self):
		## Character Position Coordinate
		self.PrintCoord = ui.TextLine()
		self.PrintCoord.SetFontName(localeInfo.UI_DEF_FONT)
		self.PrintCoord.SetPosition(wndMgr.GetScreenWidth() - 270, 0)

		## Frame Rate
		self.FrameRate = ui.TextLine()
		self.FrameRate.SetFontName(localeInfo.UI_DEF_FONT)
		self.FrameRate.SetPosition(wndMgr.GetScreenWidth() - 270, 20)

		## Camera Pitch
		self.Pitch = ui.TextLine()
		self.Pitch.SetFontName(localeInfo.UI_DEF_FONT)
		self.Pitch.SetPosition(wndMgr.GetScreenWidth() - 270, 40)

		## Splat
		self.Splat = ui.TextLine()
		self.Splat.SetFontName(localeInfo.UI_DEF_FONT)
		self.Splat.SetPosition(wndMgr.GetScreenWidth() - 270, 60)

		##
		self.PrintMousePos = ui.TextLine()
		self.PrintMousePos.SetFontName(localeInfo.UI_DEF_FONT)
		self.PrintMousePos.SetPosition(wndMgr.GetScreenWidth() - 270, 80)

		# TextureNum
		self.TextureNum = ui.TextLine()
		self.TextureNum.SetFontName(localeInfo.UI_DEF_FONT)
		self.TextureNum.SetPosition(wndMgr.GetScreenWidth() - 270, 100)

		# ������Ʈ �׸��� ����
		self.ObjectNum = ui.TextLine()
		self.ObjectNum.SetFontName(localeInfo.UI_DEF_FONT)
		self.ObjectNum.SetPosition(wndMgr.GetScreenWidth() - 270, 120)

		# �þ߰Ÿ�
		self.ViewDistance = ui.TextLine()
		self.ViewDistance.SetFontName(localeInfo.UI_DEF_FONT)
		self.ViewDistance.SetPosition(0, 0)

	def __NotifyError(self, msg):
		chat.AppendChat(chat.CHAT_TYPE_INFO, msg)

	def ChangePKMode(self):
		if not app.IsPressed(app.DIK_LCONTROL):
			return

		if player.GetStatus(player.LEVEL) < constInfo.PVPMODE_PROTECTED_LEVEL:
			self.__NotifyError(localeInfo.OPTION_PVPMODE_PROTECT % (constInfo.PVPMODE_PROTECTED_LEVEL))
			return

		curTime = app.GetTime()
		if curTime - self.lastPKModeSendedTime < constInfo.PVPMODE_ACCELKEY_DELAY:
			return

		self.lastPKModeSendedTime = curTime

		curPKMode = player.GetPKMode()
		nextPKMode = curPKMode + 1
		if nextPKMode == player.PK_MODE_PROTECT:
			if 0 == player.GetGuildID():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPTION_PVPMODE_CANNOT_SET_GUILD_MODE)
				nextPKMode = 0
			else:
				nextPKMode = player.PK_MODE_GUILD

		elif nextPKMode == player.PK_MODE_MAX_NUM:
			nextPKMode = 0

		net.SendChatPacket("/PKMode " + str(nextPKMode))
		print "/PKMode " + str(nextPKMode)

	def OnChangePKMode(self):
		self.interface.OnChangePKMode()

		try:
			if player.GetStatus(player.LEVEL) < constInfo.PVPMODE_PROTECTED_LEVEL:
				self.__NotifyError(localeInfo.OPTION_PVPMODE_MESSAGE_DICT[player.GetPKMode()])
				return
		except KeyError:
			print "UNKNOWN PVPMode[%d]" % (player.GetPKMode())

		#if constInfo.PVPMODE_TEST_ENABLE:
		#	curPKMode = player.GetPKMode()
		#	alignment, grade = chr.testGetPKData()
		#	self.pkModeNameDict = { 0 : "PEACE", 1 : "REVENGE", 2 : "FREE", 3 : "PROTECT", }
		#	self.testPKMode.SetText("Current PK Mode : " + self.pkModeNameDict.get(curPKMode, "UNKNOWN"))
		#	self.testAlignment.SetText("Current Alignment : " + str(alignment) + " (" + localeInfo.TITLE_NAME_LIST[grade] + ")")

	###############################################################################################
	###############################################################################################
	## Game Callback Functions

	# Start
	def StartGame(self):
		self.RefreshInventory()
		self.RefreshEquipment()
		self.RefreshCharacter()
		self.RefreshSkill()

	if app.ENABLE_MAP_LOCATION_APP_NAME:
		def SetApplicationText(self):
			try:
				mapName = background.GetCurrentMapName()
				if mapName in localeInfo.MINIMAP_ZONE_NAME_DICT:
					app.SetText(localeInfo.APP_TITLE + " - " + localeInfo.MINIMAP_ZONE_NAME_DICT[background.GetCurrentMapName()])
			except:
				pass

	# Refresh
	def CheckGameButton(self):
		if self.interface:
			self.interface.CheckGameButton()

	def RefreshAlignment(self):
		self.interface.RefreshAlignment()

	def RefreshStatus(self):
		self.CheckGameButton()

		if self.interface:
			self.interface.RefreshStatus()

		if self.playerGauge:
			self.playerGauge.RefreshGauge()

	def RefreshStamina(self):
		self.interface.RefreshStamina()

	def RefreshSkill(self):
		self.CheckGameButton()
		if self.interface:
			self.interface.RefreshSkill()

	def RefreshQuest(self):
		self.interface.RefreshQuest()

	def RefreshMessenger(self):
		self.interface.RefreshMessenger()

	def RefreshGuildInfoPage(self):
		self.interface.RefreshGuildInfoPage()

	def RefreshGuildBoardPage(self):
		self.interface.RefreshGuildBoardPage()

	def RefreshGuildMemberPage(self):
		self.interface.RefreshGuildMemberPage()

	def RefreshGuildMemberPageGradeComboBox(self):
		self.interface.RefreshGuildMemberPageGradeComboBox()

	def RefreshGuildSkillPage(self):
		self.interface.RefreshGuildSkillPage()

	def RefreshGuildGradePage(self):
		self.interface.RefreshGuildGradePage()

	def OnBlockMode(self, mode):
		self.interface.OnBlockMode(mode)

	def OpenQuestWindow(self, skin, idx):
		if osfInfo.INPUT_IGNORE == 1:
			return

		self.interface.OpenQuestWindow(skin, idx)

	def AskGuildName(self):
		guildNameBoard = uiCommon.InputDialog()
		guildNameBoard.SetTitle(localeInfo.GUILD_NAME)
		guildNameBoard.SetAcceptEvent(ui.__mem_func__(self.ConfirmGuildName))
		guildNameBoard.SetCancelEvent(ui.__mem_func__(self.CancelGuildName))
		guildNameBoard.Open()

		self.guildNameBoard = guildNameBoard

	def ConfirmGuildName(self):
		guildName = self.guildNameBoard.GetText()
		if not guildName:
			return

		if net.IsInsultIn(guildName):
			self.PopupMessage(localeInfo.GUILD_CREATE_ERROR_INSULT_NAME)
			return

		net.SendAnswerMakeGuildPacket(guildName)
		self.guildNameBoard.Close()
		self.guildNameBoard = None
		return True

	def CancelGuildName(self):
		self.guildNameBoard.Close()
		self.guildNameBoard = None
		return True

	## Refine
	def PopupMessage(self, msg):
		self.stream.popupWindow.Close()
		self.stream.popupWindow.Open(msg, 0, localeInfo.UI_OK)

	def OpenRefineDialog(self, targetItemPos, nextGradeItemVnum, cost, prob, type, apply_random_list = None, src_vnum = 0):
		self.interface.OpenRefineDialog(targetItemPos, nextGradeItemVnum, cost, prob, type, apply_random_list, src_vnum)

	def AppendMaterialToRefineDialog(self, vnum, count):
		self.interface.AppendMaterialToRefineDialog(vnum, count)

	def RunUseSkillEvent(self, slotIndex, coolTime):
		self.interface.OnUseSkill(slotIndex, coolTime)

	def ClearAffects(self):
		self.affectShower.ClearAffects()

	def SetAffect(self, affect):
		self.affectShower.SetAffect(affect)

	def ResetAffect(self, affect):
		self.affectShower.ResetAffect(affect)

	# UNKNOWN_UPDATE
	def BINARY_NEW_AddAffect(self, type, pointIdx, value, duration):
		self.affectShower.BINARY_NEW_AddAffect(type, pointIdx, value, duration)
		if chr.NEW_AFFECT_DRAGON_SOUL_DECK1 == type or chr.NEW_AFFECT_DRAGON_SOUL_DECK2 == type:
			self.interface.DragonSoulActivate(type - chr.NEW_AFFECT_DRAGON_SOUL_DECK1)
		elif chr.NEW_AFFECT_DRAGON_SOUL_QUALIFIED == type:
			self.BINARY_DragonSoulGiveQuilification()
		elif app.ENABLE_DS_SET and chr.NEW_AFFECT_DS_SET == type:
			self.interface.DragonSoulSetGrade(value)

	def BINARY_NEW_RemoveAffect(self, type, pointIdx):
		self.affectShower.BINARY_NEW_RemoveAffect(type, pointIdx)
		if chr.NEW_AFFECT_DRAGON_SOUL_DECK1 == type or chr.NEW_AFFECT_DRAGON_SOUL_DECK2 == type:
			self.interface.DragonSoulDeactivate()
		elif app.ENABLE_DS_SET and chr.NEW_AFFECT_DS_SET == type:
			self.interface.DragonSoulSetGrade(0)
	# END_OF_UNKNOWN_UPDATE

	if app.ENABLE_SKILL_COOLTIME_UPDATE:
		def SkillClearCoolTime(self, slotIndex):
			self.interface.SkillClearCoolTime(slotIndex)

	def ActivateSkillSlot(self, slotIndex):
		if self.interface:
			self.interface.OnActivateSkill(slotIndex)

	def DeactivateSkillSlot(self, slotIndex):
		if self.interface:
			self.interface.OnDeactivateSkill(slotIndex)

	def RefreshEquipment(self):
		if self.interface:
			self.interface.RefreshInventory()

	def RefreshInventory(self):
		if self.interface:
			self.interface.RefreshInventory()

	def RefreshCharacter(self):
		if self.interface:
			self.interface.RefreshCharacter()

	def OnGameOver(self):
		self.CloseTargetBoard()
		self.OpenRestartDialog()

	def OpenRestartDialog(self):
		self.interface.OpenRestartDialog()

	def ChangeCurrentSkill(self, skillSlotNumber):
		self.interface.OnChangeCurrentSkill(skillSlotNumber)

	## TargetBoard
	def SetPCTargetBoard(self, vid, name):
		if self.interface.IsHideUiMode == True:
			return

		self.targetBoard.Open(vid, name)

		if app.IsPressed(app.DIK_LCONTROL):
			if not player.IsSameEmpire(vid):
				return

			if player.IsMainCharacterIndex(vid):
				return
			elif chr.INSTANCE_TYPE_BUILDING == chr.GetInstanceType(vid):
				return

			self.interface.OpenWhisperDialog(name)

	def RefreshTargetBoardByVID(self, vid):
		if self.targetBoard:
			self.targetBoard.RefreshByVID(vid)

	def RefreshTargetBoardByName(self, name):
		if self.targetBoard:
			self.targetBoard.RefreshByName(name)

	def __RefreshTargetBoard(self):
		if self.targetBoard:
			self.targetBoard.Refresh()

	if app.ENABLE_VIEW_TARGET_DECIMAL_HP:
		def SetHPTargetBoard(self, vid, hpPercentage, iMinHP, iMaxHP):
			if self.interface.IsHideUiMode == True:
				return

			if vid != self.targetBoard.GetTargetVID():
				self.targetBoard.ResetTargetBoard()
				self.targetBoard.SetEnemyVID(vid)

			self.targetBoard.SetHP(hpPercentage, iMinHP, iMaxHP)
			self.targetBoard.Show()
	else:
		def SetHPTargetBoard(self, vid, hpPercentage):
			if self.interface.IsHideUiMode == True:
				return

			if vid != self.targetBoard.GetTargetVID():
				self.targetBoard.ResetTargetBoard()
				self.targetBoard.SetEnemyVID(vid)

			self.targetBoard.SetHP(hpPercentage)
			self.targetBoard.Show()

	if app.ENABLE_SHIP_DEFENSE:
		def SetHPAllianceTargetBoard(self, vid, hp, hpMax):
			if self.interface.IsHideUiMode == True:
				return

			if not vid:
				self.allyTargetBoard.Close()
				return

			if vid != self.allyTargetBoard.GetTargetVID():
				self.allyTargetBoard.ResetTargetBoard()
				self.allyTargetBoard.SetTarget(vid)

			self.allyTargetBoard.SetHP(hp, hpMax)
			self.allyTargetBoard.Show()

	def CloseTargetBoardIfDifferent(self, vid):
		if vid != self.targetBoard.GetTargetVID():
			self.targetBoard.Close()

	def CloseTargetBoard(self):
		self.targetBoard.Close()

	if app.ENABLE_ELEMENT_ADD:
		def ShowTargetElementEnchant(self, element_enchants_dict):
			if self.targetBoard:
				self.targetBoard.ShowElementImg(element_enchants_dict)

	## View Equipment
	def OpenEquipmentDialog(self, vid):
		self.interface.OpenEquipmentDialog(vid)

	if app.ENABLE_CHANGE_LOOK_SYSTEM:
		def SetEquipmentDialogItem(self, vid, slotIndex, vnum, count, dwChangeLookVnum):
			self.interface.SetEquipmentDialogItem(vid, slotIndex, vnum, count, dwChangeLookVnum)
	else:
		def SetEquipmentDialogItem(self, vid, slotIndex, vnum, count):
			self.interface.SetEquipmentDialogItem(vid, slotIndex, vnum, count)

	def SetEquipmentDialogSocket(self, vid, slotIndex, socketIndex, value):
		self.interface.SetEquipmentDialogSocket(vid, slotIndex, socketIndex, value)

	def SetEquipmentDialogAttr(self, vid, slotIndex, attrIndex, type, value):
		self.interface.SetEquipmentDialogAttr(vid, slotIndex, attrIndex, type, value)

	# SHOW_LOCAL_MAP_NAME
	def ShowMapName(self, mapName, x, y):
		if self.mapNameShower:
			self.mapNameShower.ShowMapName(mapName, x, y)

		if self.interface:
			self.interface.SetMapName(mapName)
	# END_OF_SHOW_LOCAL_MAP_NAME

	def BINARY_OpenAtlasWindow(self):
		self.interface.BINARY_OpenAtlasWindow()

	## Chat
	def OnRecvWhisper(self, mode, name, line):
		if mode == chat.WHISPER_TYPE_GM:
			self.interface.RegisterGameMasterName(name)
		chat.AppendWhisper(mode, name, line)
		self.interface.RecvWhisper(name)

	def OnRecvWhisperSystemMessage(self, mode, name, line):
		chat.AppendWhisper(chat.WHISPER_TYPE_SYSTEM, name, line)
		self.interface.RecvWhisper(name)

	def OnRecvWhisperError(self, mode, name, line):
		if localeInfo.WHISPER_ERROR.has_key(mode):
			chat.AppendWhisper(chat.WHISPER_TYPE_SYSTEM, name, localeInfo.WHISPER_ERROR[mode](name))
		else:
			chat.AppendWhisper(chat.WHISPER_TYPE_SYSTEM, name, "Whisper Unknown Error(mode=%d, name=%s)" % (mode, name))
		self.interface.RecvWhisper(name)

	def RecvWhisper(self, name):
		self.interface.RecvWhisper(name)

	def OnPickMoney(self, money):
		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			chat.AppendChat(chat.CHAT_TYPE_MONEY_INFO, localeInfo.GAME_PICK_MONEY % (money))
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_PICK_MONEY % (money))

	if app.ENABLE_CHEQUE_SYSTEM:
		def OnPickCheque(self, cheque):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CHEQUE_SYSTEM_PICK_WON % (cheque))

	if app.ENABLE_GEM_SYSTEM:
		def OnPickGem(self, gem):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GEM_SYSTEM_PICK_GEM % (gem))

		def OpenGuiGem(self):
			self.interface.OpenGuiGem()

		def GemCheck(self):
			self.interface.GemCheck()

		def OpenGuiGemMarket(self):
			self.interface.OpenGuiGemMarket()

		def GemMarketItems(self, vnums, gem, count):
			self.interface.GemMarketItems(vnums, gem, count)

		def GemMarketSlotsDesblock(self, slot0, slot1, slot2, slot3, slot4, slot5):
			self.interface.GemMarketSlotsDesblock(slot0, slot1, slot2, slot3, slot4, slot5)

		def GemMarketClear(self):
			self.interface.GemMarketClear()

		def GemTimeMarket(self, time):
			self.interface.GemTime(time)

	if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
		def OpenSpecialInventoryWindow(self, category):
			self.interface.OpenSpecialInventoryWindow(int(category))

	def OpenCostumeWindow(self):
		self.interface.OpenCostumeWindow()

	if app.ENABLE_MOVE_CHANNEL:
		# 2019.09.03.owsap - Load server info (server name)
		def __LoadChannelInfo(self):
			try:
				file = open("channel.inf")
				lines = file.readlines()

				if len(lines) > 0:
					tokens = lines[0].split()

					selServerID = int(tokens[0])
					selChannelID = int(tokens[1])

					if len(tokens) == 3:
						regionID = int(tokens[2])

					return regionID, selServerID, selChannelID
			except:
				print "GameWindow.__LoadChannelInfo - OpenError"
				return -1, -1, -1

		def ChannelInfo(self, channelNumber, mapIndex):
			_chNum = int(channelNumber.strip())
			_mapIdx = int(mapIndex.strip())

			if _chNum == 99 or _mapIdx >= 10000:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_CHANNEL_NOTICE % 0)
			else:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.MOVE_CHANNEL_NOTICE % _chNum)

			net.SetChannelNumber(_chNum) # 2019.09.02.owsap
			net.SetMapIndex(_mapIdx)

			# 2019.09.02.owsap - update server info
			import serverInfo
			loadRegionID, loadServerID, loadChannelID = self.__LoadChannelInfo()
			_svName = serverInfo.REGION_DICT[loadRegionID][loadServerID]["name"]
			_chName = serverInfo.REGION_DICT[loadRegionID][loadServerID]["channel"]
			net.SetServerInfo(("%s, %s " % (_svName, "CH0" if _chNum == 99 else _chName[_chNum]["name"])).strip())

			self.interface.RefreshServerInfo()

	if app.ENABLE_EXPRESSING_EMOTION:
		def ClearSpecialEmotions(self):
			osfInfo.EXPRESSING_EMOTIONS = {}
			self.interface.ClearSpecialEmotions()

		def RegisterSpecialEmotion(self, emotionIdx, leftTime, notice):
			emotionIdx, leftTime, notice = int(emotionIdx), int(leftTime), int(notice)
			self.interface.RegisterSpecialEmotions(emotionIdx, leftTime)

			if notice != 0:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.OPEN_THE_SPECIAL_ACTION_KIT % osfInfo.GET_SPECIAL_EMOTION_NAME(emotionIdx))

	if app.ENABLE_CAPTCHA_SYSTEM:
		def IsAFKMode(self, qid):
			osfInfo.AFKMode = int(qid)

		def ShowCaptcha(self, timeLeft):
			self.interface.ShowCaptcha(int(timeLeft))

	def OnShopError(self, type):
		try:
			self.PopupMessage(localeInfo.SHOP_ERROR_DICT[type])
		except KeyError:
			self.PopupMessage(localeInfo.SHOP_ERROR_UNKNOWN % (type))

	def OnSafeBoxError(self):
		self.PopupMessage(localeInfo.SAFEBOX_ERROR)

	def OnFishingSuccess(self, isFish, fishName):
		chat.AppendChatWithDelay(chat.CHAT_TYPE_INFO, localeInfo.FISHING_SUCCESS(isFish, fishName), 2000)

	# ADD_FISHING_MESSAGE
	def OnFishingNotifyUnknown(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.FISHING_UNKNOWN)

	def OnFishingWrongPlace(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.FISHING_WRONG_PLACE)
	# END_OF_ADD_FISHING_MESSAGE

	def OnFishingNotify(self, isFish, fishName):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.FISHING_NOTIFY(isFish, fishName))

	def OnFishingFailure(self):
		chat.AppendChatWithDelay(chat.CHAT_TYPE_INFO, localeInfo.FISHING_FAILURE, 2000)

	def OnCannotPickItem(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_CANNOT_PICK_ITEM)

	# MINING
	def OnCannotMining(self):
		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GAME_CANNOT_MINING)
	# END_OF_MINING

	def OnCannotUseSkill(self, vid, type):
		if localeInfo.USE_SKILL_ERROR_TAIL_DICT.has_key(type):
			textTail.RegisterInfoTail(vid, localeInfo.USE_SKILL_ERROR_TAIL_DICT[type])

		if localeInfo.USE_SKILL_ERROR_CHAT_DICT.has_key(type):
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.USE_SKILL_ERROR_CHAT_DICT[type])

	def OnCannotShotError(self, vid, type):
		textTail.RegisterInfoTail(vid, localeInfo.SHOT_ERROR_TAIL_DICT.get(type, localeInfo.SHOT_ERROR_UNKNOWN % (type)))

	## PointReset
	def StartPointReset(self):
		self.interface.OpenPointResetDialog()

	## Shop
	def StartShop(self, vid):
		self.interface.OpenShopDialog(vid)

	def EndShop(self):
		self.interface.CloseShopDialog()

	def RefreshShop(self):
		self.interface.RefreshShopDialog()

	def SetShopSellingPrice(self, Price):
		pass

	## Exchange
	def StartExchange(self):
		self.interface.StartExchange()

	def EndExchange(self):
		self.interface.EndExchange()

	def RefreshExchange(self):
		self.interface.RefreshExchange()

	if app.WJ_ENABLE_TRADABLE_ICON:
		def AddExchangeItemSlotIndex(self, idx):
			self.interface.AddExchangeItemSlotIndex(idx)

	## Party
	# ��Ƽ ���� ���� �ð������� ����.
	if app.WJ_NEW_USER_CARE:
		def RecvPartyInviteQuestion(self, leaderVID, leaderName):
			partyInviteQuestionDialog = uiCommon.QuestionDialogWithTimeLimit()
			partyInviteQuestionDialog.SetAcceptEvent(lambda arg=True: self.AnswerPartyInvite(arg))
			partyInviteQuestionDialog.SetCancelEvent(lambda arg=False: self.AnswerPartyInvite(arg))
			partyInviteQuestionDialog.Open(leaderName + localeInfo.PARTY_DO_YOU_JOIN, 10)
			partyInviteQuestionDialog.SetTimeOverMsg(localeInfo.PARTY_ANSWER_TIMEOVER)
			partyInviteQuestionDialog.SetCancelOnTimeOver()
			partyInviteQuestionDialog.partyLeaderVID = leaderVID
			self.partyInviteQuestionDialog = partyInviteQuestionDialog
	else:
		def RecvPartyInviteQuestion(self, leaderVID, leaderName):
			partyInviteQuestionDialog = uiCommon.QuestionDialog()
			partyInviteQuestionDialog.SetText(leaderName + localeInfo.PARTY_DO_YOU_JOIN)
			partyInviteQuestionDialog.SetAcceptEvent(lambda arg=True: self.AnswerPartyInvite(arg))
			partyInviteQuestionDialog.SetCancelEvent(lambda arg=False: self.AnswerPartyInvite(arg))
			partyInviteQuestionDialog.Open()
			partyInviteQuestionDialog.partyLeaderVID = leaderVID
			self.partyInviteQuestionDialog = partyInviteQuestionDialog

	def AnswerPartyInvite(self, answer):
		if not self.partyInviteQuestionDialog:
			return

		partyLeaderVID = self.partyInviteQuestionDialog.partyLeaderVID

		# ��Ƽ���Կ� ���� �Ÿ� ���� ����.
		if not app.WJ_NEW_USER_CARE:
			distance = player.GetCharacterDistance(partyLeaderVID)
			if distance < 0.0 or distance > 5000:
				answer = False

		net.SendPartyInviteAnswerPacket(partyLeaderVID, answer)

		self.partyInviteQuestionDialog.Close()
		self.partyInviteQuestionDialog = None

	if app.WJ_SHOW_PARTY_ON_MINIMAP:
		def AddPartyMember(self, pid, name, mapIdx, channel):
			self.interface.AddPartyMember(pid, name, mapIdx, channel)
	else:
		def AddPartyMember(self, pid, name):
			self.interface.AddPartyMember(pid, name)

	def UpdatePartyMemberInfo(self, pid):
		self.interface.UpdatePartyMemberInfo(pid)

	def RemovePartyMember(self, pid):
		self.interface.RemovePartyMember(pid)
		self.__RefreshTargetBoard()

	if app.WJ_SHOW_PARTY_ON_MINIMAP:
		def LinkPartyMember(self, pid, vid, mapIdx, channel):
			self.interface.LinkPartyMember(pid, vid, mapIdx, channel)
	else:
		def LinkPartyMember(self, pid, vid):
			self.interface.LinkPartyMember(pid, vid)

	def UnlinkPartyMember(self, pid):
		self.interface.UnlinkPartyMember(pid)

	def UnlinkAllPartyMember(self):
		self.interface.UnlinkAllPartyMember()

	def ExitParty(self):
		self.interface.ExitParty()
		self.RefreshTargetBoardByVID(self.targetBoard.GetTargetVID())

	def ChangePartyParameter(self, distributionMode):
		self.interface.ChangePartyParameter(distributionMode)

	## Messenger
	def OnMessengerAddFriendQuestion(self, name):
		messengerAddFriendQuestion = uiCommon.QuestionDialog2()
		messengerAddFriendQuestion.SetText1(localeInfo.MESSENGER_DO_YOU_ACCEPT_ADD_FRIEND_1 % (name))
		messengerAddFriendQuestion.SetText2(localeInfo.MESSENGER_DO_YOU_ACCEPT_ADD_FRIEND_2)
		messengerAddFriendQuestion.SetAcceptEvent(ui.__mem_func__(self.OnAcceptAddFriend))
		messengerAddFriendQuestion.SetCancelEvent(ui.__mem_func__(self.OnDenyAddFriend))
		messengerAddFriendQuestion.Open()
		messengerAddFriendQuestion.name = name
		self.messengerAddFriendQuestion = messengerAddFriendQuestion

	def OnAcceptAddFriend(self):
		name = self.messengerAddFriendQuestion.name
		net.SendChatPacket("/messenger_auth y " + name)
		self.OnCloseAddFriendQuestionDialog()
		return True

	def OnDenyAddFriend(self):
		name = self.messengerAddFriendQuestion.name
		net.SendChatPacket("/messenger_auth n " + name)
		self.OnCloseAddFriendQuestionDialog()
		return True

	def OnCloseAddFriendQuestionDialog(self):
		self.messengerAddFriendQuestion.Close()
		self.messengerAddFriendQuestion = None
		return True

	## SafeBox
	def OpenSafeboxWindow(self, size):
		self.interface.OpenSafeboxWindow(size)

	def RefreshSafebox(self):
		self.interface.RefreshSafebox()

	def RefreshSafeboxMoney(self):
		self.interface.RefreshSafeboxMoney()

	# ITEM_MALL
	def OpenMallWindow(self, size):
		self.interface.OpenMallWindow(size)

	def RefreshMall(self):
		self.interface.RefreshMall()
	# END_OF_ITEM_MALL

	## Guild
	def RecvGuildInviteQuestion(self, guildID, guildName):
		guildInviteQuestionDialog = uiCommon.QuestionDialog()
		guildInviteQuestionDialog.SetText(guildName + localeInfo.GUILD_DO_YOU_JOIN)
		guildInviteQuestionDialog.SetAcceptEvent(lambda arg=True: self.AnswerGuildInvite(arg))
		guildInviteQuestionDialog.SetCancelEvent(lambda arg=False: self.AnswerGuildInvite(arg))
		guildInviteQuestionDialog.Open()
		guildInviteQuestionDialog.guildID = guildID
		self.guildInviteQuestionDialog = guildInviteQuestionDialog

	def AnswerGuildInvite(self, answer):
		if not self.guildInviteQuestionDialog:
			return

		guildLeaderVID = self.guildInviteQuestionDialog.guildID
		net.SendGuildInviteAnswerPacket(guildLeaderVID, answer)

		self.guildInviteQuestionDialog.Close()
		self.guildInviteQuestionDialog = None

	def DeleteGuild(self):
		self.interface.DeleteGuild()

	## Clock
	def ShowClock(self, second):
		self.interface.ShowClock(second)

	def HideClock(self):
		self.interface.HideClock()

	## Emotion
	def BINARY_ActEmotion(self, emotionIndex):
		if self.interface.wndCharacter:
			self.interface.wndCharacter.ActEmotion(emotionIndex)

	###############################################################################################
	###############################################################################################
	## Keyboard Functions

	def CheckFocus(self):
		if False == self.IsFocus():
			if True == self.interface.IsOpenChat():
				self.interface.ToggleChat()

			self.SetFocus()

	def SaveScreen(self):
		print "save screen"

		# SCREENSHOT_CWDSAVE
		if SCREENSHOT_CWDSAVE:
			if not os.path.exists(os.getcwd() + os.sep + "screenshot"):
				os.mkdir(os.getcwd() + os.sep + "screenshot")

			(succeeded, name) = grp.SaveScreenShotToPath(os.getcwd()+os.sep+"screenshot"+os.sep)
		elif SCREENSHOT_DIR:
			(succeeded, name) = grp.SaveScreenShot(SCREENSHOT_DIR)
		else:
			(succeeded, name) = grp.SaveScreenShot()
		# END_OF_SCREENSHOT_CWDSAVE

		if succeeded:
			pass
			"""
			chat.AppendChat(chat.CHAT_TYPE_INFO, name + localeInfo.SCREENSHOT_SAVE1)
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SCREENSHOT_SAVE2)
			"""
		else:
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.SCREENSHOT_SAVE_FAILURE)

	def ShowConsole(self):
		if debugInfo.IsDebugMode() or True == self.consoleEnable or osfInfo.IsDebugMode():
			player.EndKeyWalkingImmediately()
			self.console.OpenWindow()

	def ShowName(self):
		self.ShowNameFlag = True
		self.playerGauge.EnableShowAlways()
		if not app.ENABLE_KEYCHANGE_SYSTEM:
			player.SetQuickPage(self.quickSlotPageIndex + 1)

	# ADD_ALWAYS_SHOW_NAME
	def __IsShowName(self):
		if systemSetting.IsAlwaysShowName():
			return True

		if self.ShowNameFlag:
			return True

		return False
	# END_OF_ADD_ALWAYS_SHOW_NAME

	def HideName(self):
		self.ShowNameFlag = False
		self.playerGauge.DisableShowAlways()
		if not app.ENABLE_KEYCHANGE_SYSTEM:
			player.SetQuickPage(self.quickSlotPageIndex)

	def ShowMouseImage(self):
		self.interface.ShowMouseImage()

	def HideMouseImage(self):
		self.interface.HideMouseImage()

	def StartAttack(self):
		player.SetAttackKeyState(True)

	def EndAttack(self):
		player.SetAttackKeyState(False)

	def MoveUp(self):
		player.SetSingleDIKKeyState(app.DIK_UP, True)

	def MoveDown(self):
		player.SetSingleDIKKeyState(app.DIK_DOWN, True)

	def MoveLeft(self):
		player.SetSingleDIKKeyState(app.DIK_LEFT, True)

	def MoveRight(self):
		player.SetSingleDIKKeyState(app.DIK_RIGHT, True)

	def StopUp(self):
		player.SetSingleDIKKeyState(app.DIK_UP, False)

	def StopDown(self):
		player.SetSingleDIKKeyState(app.DIK_DOWN, False)

	def StopLeft(self):
		player.SetSingleDIKKeyState(app.DIK_LEFT, False)

	def StopRight(self):
		player.SetSingleDIKKeyState(app.DIK_RIGHT, False)

	def PickUpItem(self):
		player.PickCloseItem()

	if app.ENABLE_QUICK_PICKUP:
		def PickUpAllItems(self):
			player.PickAllCloseItems()

	###############################################################################################
	###############################################################################################
	## Event Handler

	def OnKeyDown(self, key):
		if self.interface.wndWeb and self.interface.wndWeb.IsShow():
			return

		if key == app.DIK_ESC:
			self.RequestDropItem(False)
			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

		try:
			if app.ENABLE_KEYCHANGE_SYSTEM:
				if self.wndKeyChange.IsOpen() == 1:
					## Ű���� â�� ��������. �׸��� ���� �ٲ��� ����������
					if self.wndKeyChange.IsSelectKeySlot():
						if app.IsPressed(app.DIK_LCONTROL) or app.IsPressed(app.DIK_RCONTROL):
							if self.wndKeyChange.IsChangeKey(self.wndKeyChange.GetSelectSlotNumber()):
								self.wndKeyChange.ChangeKey(key + app.DIK_LCONTROL + self.ADDKEYBUFFERCONTROL)
							else:
								chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.KEYCHANGE_IMPOSSIBLE_CHANGE)
						elif app.IsPressed(app.DIK_LALT) or app.IsPressed(app.DIK_RALT):
							if self.wndKeyChange.IsChangeKey(self.wndKeyChange.GetSelectSlotNumber()):
								self.wndKeyChange.ChangeKey(key + app.DIK_LALT + self.ADDKEYBUFFERALT)
							else:
								chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.KEYCHANGE_IMPOSSIBLE_CHANGE)
						elif app.IsPressed(app.DIK_LSHIFT) or app.IsPressed(app.DIK_RSHIFT):
							if self.wndKeyChange.IsChangeKey(self.wndKeyChange.GetSelectSlotNumber()):
								self.wndKeyChange.ChangeKey(key + app.DIK_LSHIFT + self.ADDKEYBUFFERSHIFT)
							else:
								chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.KEYCHANGE_IMPOSSIBLE_CHANGE)
						else:
							self.wndKeyChange.ChangeKey(key)
				else:
					player.OnKeyDown(key)
			else:
				self.onPressKeyDict[key]()
		except KeyError:
			pass
		except:
			raise

		return True

	def OnKeyUp(self, key):
		if app.ENABLE_KEYCHANGE_SYSTEM:
			player.OnKeyUp(key)
		else:
			try:
				self.onClickKeyDict[key]()
			except KeyError:
				pass
			except:
				raise

		return True

	def OnMouseLeftButtonDown(self):
		if self.interface.BUILD_OnMouseLeftButtonDown():
			return

		if mouseModule.mouseController.isAttached():
			self.CheckFocus()
		else:
			hyperlink = ui.GetHyperlink()
			if hyperlink:
				return
			else:
				self.CheckFocus()
				player.SetMouseState(player.MBT_LEFT, player.MBS_PRESS);

		return True

	def OnMouseLeftButtonUp(self):
		if self.interface.BUILD_OnMouseLeftButtonUp():
			return

		if mouseModule.mouseController.isAttached():
			attachedType = mouseModule.mouseController.GetAttachedType()
			attachedItemIndex = mouseModule.mouseController.GetAttachedItemIndex()
			attachedItemSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemCount = mouseModule.mouseController.GetAttachedItemCount()

			## QuickSlot
			if player.SLOT_TYPE_QUICK_SLOT == attachedType:
				player.RequestDeleteGlobalQuickSlot(attachedItemSlotPos)

			## Inventory
			elif player.SLOT_TYPE_INVENTORY == attachedType:
				if player.ITEM_MONEY == attachedItemIndex:
					# Note : �κ� �� ���� ��, �ٸ� ���� Ŭ���ϸ� ������ �Լ�
					if app.ENABLE_CHEQUE_SYSTEM:
						cheque = mouseModule.mouseController.GetCheque()
						self.__PutMoney(attachedType, attachedItemCount, self.PickingCharacterIndex, cheque)
					else:
						self.__PutMoney(attachedType, attachedItemCount, self.PickingCharacterIndex)
				else:
					self.__PutItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, self.PickingCharacterIndex)

			## DragonSoul
			elif player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedType:
				self.__PutItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, self.PickingCharacterIndex)

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				if player.SLOT_TYPE_CHANGE_LOOK == attachedType:
					self.__PutItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, self.PickingCharacterIndex)

			mouseModule.mouseController.DeattachObject()
		else:
			hyperlink = ui.GetHyperlink()
			if hyperlink:
				if app.IsPressed(app.DIK_LALT):
					link = chat.GetLinkFromHyperlink(hyperlink)
					#ime.PasteString(link)
				else:
					self.interface.MakeHyperlinkTooltip(hyperlink)
				return
			else:
				player.SetMouseState(player.MBT_LEFT, player.MBS_CLICK)

		# player.EndMouseWalking()
		return True

	def __PutItem(self, attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount, dstChrID):
		if player.SLOT_TYPE_INVENTORY == attachedType or player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedType:

			attachedInvenType = player.SlotTypeToInvenType(attachedType)
			if True == chr.HasInstance(self.PickingCharacterIndex) and player.GetMainCharacterIndex() != dstChrID:
				if player.IsEquipmentSlot(attachedItemSlotPos) and player.SLOT_TYPE_DRAGON_SOUL_INVENTORY != attachedType:
					self.stream.popupWindow.Close()
					self.stream.popupWindow.Open(localeInfo.EXCHANGE_FAILURE_EQUIP_ITEM, 0, localeInfo.UI_OK)
				else:
					if chr.IsNPC(dstChrID):
						if app.ENABLE_AUTO_REFINE:
							osfInfo.AUTO_REFINE_TYPE = 2
							osfInfo.AUTO_REFINE_DATA["NPC"][0] = dstChrID
							osfInfo.AUTO_REFINE_DATA["NPC"][1] = attachedInvenType
							osfInfo.AUTO_REFINE_DATA["NPC"][2] = attachedItemSlotPos
							osfInfo.AUTO_REFINE_DATA["NPC"][3] = attachedItemCount

						net.SendGiveItemPacket(dstChrID, attachedInvenType, attachedItemSlotPos, attachedItemCount)
					else:
						if app.ENABLE_GUILD_DRAGONLAIR_SYSTEM:
							if chr.IsStone(dstChrID):
								net.SendGiveItemPacket(dstChrID, attachedInvenType, attachedItemSlotPos, attachedItemCount)
							else:
								net.SendExchangeStartPacket(dstChrID)
								net.SendExchangeItemAddPacket(attachedInvenType, attachedItemSlotPos, 0)
						else:
							net.SendExchangeStartPacket(dstChrID)
							net.SendExchangeItemAddPacket(attachedInvenType, attachedItemSlotPos, 0)
			else:
				self.__DropItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount)

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			if player.SLOT_TYPE_CHANGE_LOOK == attachedType:
				self.__DropItem(attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount)

	if app.ENABLE_CHEQUE_SYSTEM:
		def __PutMoney(self, attachedType, attachedMoney, dstChrID, cheque):
			if True == chr.HasInstance(dstChrID) and player.GetMainCharacterIndex() != dstChrID:
				net.SendExchangeStartPacket(dstChrID)
				net.SendExchangeElkAddPacket(attachedMoney)
				net.SendExchangeChequeAddPacket(cheque)
			else:
				self.stream.popupWindow.Close()
				self.stream.popupWindow.Open(localeInfo.CHEQUE_SYSTEM_DO_NOT_DROP_MONEY, 0, localeInfo.UI_OK)
	else:
		def __PutMoney(self, attachedType, attachedMoney, dstChrID):
			if True == chr.HasInstance(dstChrID) and player.GetMainCharacterIndex() != dstChrID:
				net.SendExchangeStartPacket(dstChrID)
				net.SendExchangeElkAddPacket(attachedMoney)
			else:
				self.__DropMoney(attachedType, attachedMoney)

	def __DropMoney(self, attachedType, attachedMoney):
		# PRIVATESHOP_DISABLE_ITEM_DROP - ���λ��� ���� �ִ� ���� ������ ���� ����
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return
		# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP

		if attachedMoney >= 1000:
			self.stream.popupWindow.Close()
			self.stream.popupWindow.Open(localeInfo.DROP_MONEY_FAILURE_1000_OVER, 0, localeInfo.UI_OK)
			return

		itemDropQuestionDialog = uiCommon.QuestionDialog()
		itemDropQuestionDialog.SetText(localeInfo.DO_YOU_DROP_MONEY % (attachedMoney))
		itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDropItem(arg))
		itemDropQuestionDialog.SetCancelEvent(lambda arg=False: self.RequestDropItem(arg))
		itemDropQuestionDialog.Open()
		itemDropQuestionDialog.dropType = attachedType
		itemDropQuestionDialog.dropCount = attachedMoney
		itemDropQuestionDialog.dropNumber = player.ITEM_MONEY
		self.itemDropQuestionDialog = itemDropQuestionDialog

	def __DropItem(self, attachedType, attachedItemIndex, attachedItemSlotPos, attachedItemCount):
		# PRIVATESHOP_DISABLE_ITEM_DROP - ���λ��� ���� �ִ� ���� ������ ���� ����
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return
		# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP

		if player.SLOT_TYPE_INVENTORY == attachedType and player.IsEquipmentSlot(attachedItemSlotPos):
			self.stream.popupWindow.Close()
			self.stream.popupWindow.Open(localeInfo.DROP_ITEM_FAILURE_EQUIP_ITEM, 0, localeInfo.UI_OK)

		else:
			if player.SLOT_TYPE_INVENTORY == attachedType:
				dropItemIndex = player.GetItemIndex(attachedItemSlotPos)

				item.SelectItem(dropItemIndex)
				dropItemName = item.GetItemName()

				## Question Text
				questionText = localeInfo.HOW_MANY_ITEM_DO_YOU_DROP(dropItemName, attachedItemCount)

				## Dialog
				if app.ENABLE_NEW_DROP_DIALOG:
					itemDropQuestionDialog = uiCommon.QuestionDropDialog()
					itemDropQuestionDialog.SetText(questionText)
					itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDropItem(arg))
					itemDropQuestionDialog.SetDestroyEvent(lambda arg=True: self.RequestDestroyItem(arg))
					itemDropQuestionDialog.SetCancelEvent(lambda arg=False: self.RequestDropItem(arg))
					itemDropQuestionDialog.SetItemSlot(attachedItemSlotPos)
					itemDropQuestionDialog.Open()
					itemDropQuestionDialog.dropType = attachedType
					itemDropQuestionDialog.dropNumber = attachedItemSlotPos
					itemDropQuestionDialog.dropCount = attachedItemCount
					self.itemDropQuestionDialog = itemDropQuestionDialog
				else:
					itemDropQuestionDialog = uiCommon.QuestionDialog()
					itemDropQuestionDialog.SetText(questionText)
					itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDropItem(arg))
					itemDropQuestionDialog.SetCancelEvent(lambda arg=False: self.RequestDropItem(arg))
					itemDropQuestionDialog.Open()
					itemDropQuestionDialog.dropType = attachedType
					itemDropQuestionDialog.dropNumber = attachedItemSlotPos
					itemDropQuestionDialog.dropCount = attachedItemCount
					self.itemDropQuestionDialog = itemDropQuestionDialog

				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

			elif player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == attachedType:
				dropItemIndex = player.GetItemIndex(player.DRAGON_SOUL_INVENTORY, attachedItemSlotPos)

				item.SelectItem(dropItemIndex)
				dropItemName = item.GetItemName()

				## Question Text
				questionText = localeInfo.HOW_MANY_ITEM_DO_YOU_DROP(dropItemName, attachedItemCount)

				## Dialog
				itemDropQuestionDialog = uiCommon.QuestionDialog()
				itemDropQuestionDialog.SetText(questionText)
				itemDropQuestionDialog.SetAcceptEvent(lambda arg=True: self.RequestDropItem(arg))
				itemDropQuestionDialog.SetCancelEvent(lambda arg=False: self.RequestDropItem(arg))
				itemDropQuestionDialog.Open()
				itemDropQuestionDialog.dropType = attachedType
				itemDropQuestionDialog.dropNumber = attachedItemSlotPos
				itemDropQuestionDialog.dropCount = attachedItemCount
				self.itemDropQuestionDialog = itemDropQuestionDialog

				constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(1)

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				if player.SLOT_TYPE_CHANGE_LOOK == attachedType:
					net.SendChangeLookCheckOut(attachedItemSlotPos)

	def RequestDropItem(self, answer):
		if not self.itemDropQuestionDialog:
			return

		if answer:
			dropType = self.itemDropQuestionDialog.dropType
			dropCount = self.itemDropQuestionDialog.dropCount
			dropNumber = self.itemDropQuestionDialog.dropNumber

			if player.SLOT_TYPE_INVENTORY == dropType:
				if dropNumber == player.ITEM_MONEY:
					net.SendGoldDropPacketNew(dropCount)
					snd.PlaySound("sound/ui/money.wav")
				elif dropNumber == player.ITEM_MONEY and app.ENABLE_CHEQUE_SYSTEM:
					net.SendChequeDropPacketNew(dropCount)
					snd.PlaySound("sound/ui/money.wav")
				else:
					# PRIVATESHOP_DISABLE_ITEM_DROP
					self.__SendDropItemPacket(dropNumber, dropCount)
					# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP

			elif player.SLOT_TYPE_DRAGON_SOUL_INVENTORY == dropType:
					# PRIVATESHOP_DISABLE_ITEM_DROP
					self.__SendDropItemPacket(dropNumber, dropCount, player.DRAGON_SOUL_INVENTORY)
					# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP

		self.itemDropQuestionDialog.Close()
		self.itemDropQuestionDialog = None

		constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

	if app.ENABLE_NEW_DROP_DIALOG:
		def RequestDestroyItem(self, answer):
			if not self.itemDropQuestionDialog:
				return

			if answer:
				dropType = self.itemDropQuestionDialog.dropType
				dropCount = self.itemDropQuestionDialog.dropCount
				dropNumber = self.itemDropQuestionDialog.dropNumber

				if player.SLOT_TYPE_INVENTORY == dropType:
					if dropNumber == player.ITEM_MONEY:
						return
					elif dropNumber == player.ITEM_MONEY and app.ENABLE_CHEQUE_SYSTEM:
						return
					else:
						self.__SendDestroyItemPacket(dropNumber)

			self.itemDropQuestionDialog.Close()
			self.itemDropQuestionDialog = None

			constInfo.SET_ITEM_QUESTION_DIALOG_STATUS(0)

		def __SendDestroyItemPacket(self, itemVNum, itemInvenType = player.INVENTORY):
			if uiPrivateShopBuilder.IsBuildingPrivateShop():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
				return

			net.SendItemDestroyPacket(itemVNum)

	# PRIVATESHOP_DISABLE_ITEM_DROP
	def __SendDropItemPacket(self, itemVNum, itemCount, itemInvenType = player.INVENTORY):
		if uiPrivateShopBuilder.IsBuildingPrivateShop():
			chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.DROP_ITEM_FAILURE_PRIVATE_SHOP)
			return

		net.SendItemDropPacketNew(itemInvenType, itemVNum, itemCount)
	# END_OF_PRIVATESHOP_DISABLE_ITEM_DROP

	def OnMouseRightButtonDown(self):
		self.CheckFocus()

		if True == mouseModule.mouseController.isAttached():
			mouseModule.mouseController.DeattachObject()
		else:
			player.SetMouseState(player.MBT_RIGHT, player.MBS_PRESS)

		return True

	def OnMouseRightButtonUp(self):
		if True == mouseModule.mouseController.isAttached():
			return True

		player.SetMouseState(player.MBT_RIGHT, player.MBS_CLICK)
		return True

	def OnMouseMiddleButtonDown(self):
		player.SetMouseMiddleButtonState(player.MBS_PRESS)

	def OnMouseMiddleButtonUp(self):
		player.SetMouseMiddleButtonState(player.MBS_CLICK)

	def OnUpdate(self):
		app.UpdateGame()

		if self.mapNameShower.IsShow():
			self.mapNameShower.Update()

		if self.isShowDebugInfo:
			self.UpdateDebugInfo()

		if self.enableXMasBoom:
			self.__XMasBoom_Update()

		self.interface.BUILD_OnUpdate()

	def UpdateDebugInfo(self):
		#
		# ĳ���� ��ǥ �� FPS ���
		(x, y, z) = player.GetMainCharacterPosition()
		nUpdateTime = app.GetUpdateTime()
		nUpdateFPS = app.GetUpdateFPS()
		nRenderFPS = app.GetRenderFPS()
		nFaceCount = app.GetFaceCount()
		fFaceSpeed = app.GetFaceSpeed()
		nST=background.GetRenderShadowTime()
		(fAveRT, nCurRT) = app.GetRenderTime()
		(iNum, fFogStart, fFogEnd, fFarCilp) = background.GetDistanceSetInfo()
		(iPatch, iSplat, fSplatRatio, sTextureNum) = background.GetRenderedSplatNum()
		if iPatch == 0:
			iPatch = 1

		#(dwRenderedThing, dwRenderedCRC) = background.GetRenderedGraphicThingInstanceNum()

		self.PrintCoord.SetText("Coordinate: %.2f %.2f %.2f ATM: %d" % (x, y, z, app.GetAvailableTextureMemory()/(1024*1024)))
		xMouse, yMouse = wndMgr.GetMousePosition()
		self.PrintMousePos.SetText("MousePosition: %d %d" % (xMouse, yMouse))

		self.FrameRate.SetText("UFPS: %3d UT: %3d FS %.2f" % (nUpdateFPS, nUpdateTime, fFaceSpeed))

		if fAveRT>1.0:
			self.Pitch.SetText("RFPS: %3d RT:%.2f(%3d) FC: %d(%.2f) " % (nRenderFPS, fAveRT, nCurRT, nFaceCount, nFaceCount/fAveRT))

		self.Splat.SetText("PATCH: %d SPLAT: %d BAD(%.2f)" % (iPatch, iSplat, fSplatRatio))
		# self.Pitch.SetText("Pitch: %.2f" % (app.GetCameraPitch())
		# self.TextureNum.SetText("TN : %s" % (sTextureNum))
		# self.ObjectNum.SetText("GTI : %d, CRC : %d" % (dwRenderedThing, dwRenderedCRC))
		self.ViewDistance.SetText("Num : %d, FS : %f, FE : %f, FC : %f" % (iNum, fFogStart, fFogEnd, fFarCilp))

	def OnRender(self):
		app.RenderGame()

		if self.console.Console.collision:
			background.RenderCollision()
			chr.RenderCollision()

		(x, y) = app.GetCursorPosition()

		########################
		# Picking
		########################
		textTail.UpdateAllTextTail()

		if True == wndMgr.IsPickedWindow(self.hWnd):
			self.PickingCharacterIndex = chr.Pick()

			if -1 != self.PickingCharacterIndex:
				textTail.ShowCharacterTextTail(self.PickingCharacterIndex)
			if 0 != self.targetBoard.GetTargetVID():
				textTail.ShowCharacterTextTail(self.targetBoard.GetTargetVID())

			# ADD_ALWAYS_SHOW_NAME
			if not self.__IsShowName():
				self.PickingItemIndex = item.Pick()
				if -1 != self.PickingItemIndex:
					textTail.ShowItemTextTail(self.PickingItemIndex)
			# END_OF_ADD_ALWAYS_SHOW_NAME

		## Show all name in the range

		# ADD_ALWAYS_SHOW_NAME
		if self.__IsShowName():
			textTail.ShowAllTextTail()
			self.PickingItemIndex = textTail.Pick(x, y)
		# END_OF_ADD_ALWAYS_SHOW_NAME

		if app.ENABLE_GRAPHIC_ON_OFF:
			if systemSetting.IsShowSalesText():
				uiPrivateShopBuilder.UpdateADBoard()

		textTail.UpdateShowingTextTail()
		textTail.ArrangeTextTail()
		if -1 != self.PickingItemIndex:
			textTail.SelectItemName(self.PickingItemIndex)

		grp.PopState()
		grp.SetInterfaceRenderState()

		textTail.Render()
		textTail.HideAllTextTail()

	def OnPressEscapeKey(self):
		if app.TARGET == app.GetCursor():
			app.SetCursor(app.NORMAL)

		elif True == mouseModule.mouseController.isAttached():
			mouseModule.mouseController.DeattachObject()
		else:
			self.interface.OpenSystemDialog()

		return True

	def OnIMEReturn(self):
		if app.IsPressed(app.DIK_LSHIFT):
			self.interface.OpenWhisperDialogWithoutTarget()
		else:
			self.interface.ToggleChat()
		return True

	def OnPressExitKey(self):
		self.interface.ToggleSystemDialog()
		return True

	## BINARY CALLBACK
	######################################################################################

	# WEDDING
	def BINARY_LoverInfo(self, name, lovePoint):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnAddLover(name, lovePoint)
		if self.affectShower:
			self.affectShower.SetLoverInfo(name, lovePoint)

	def BINARY_UpdateLovePoint(self, lovePoint):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnUpdateLovePoint(lovePoint)
		if self.affectShower:
			self.affectShower.OnUpdateLovePoint(lovePoint)
	# END_OF_WEDDING

	if app.ENABLE_SEND_TARGET_INFO:
		def BINARY_RefreshTargetMonsterDropInfo(self, raceNum):
			self.targetBoard.RefreshMonsterInfoBoard()

	# QUEST_CONFIRM
	def BINARY_OnQuestConfirm(self, msg, timeout, pid):
		confirmDialog = uiCommon.QuestionDialogWithTimeLimit()
		confirmDialog.Open(msg, timeout)
		confirmDialog.SetAcceptEvent(lambda answer=True, pid=pid: net.SendQuestConfirmPacket(answer, pid) or self.confirmDialog.Hide())
		confirmDialog.SetCancelEvent(lambda answer=False, pid=pid: net.SendQuestConfirmPacket(answer, pid) or self.confirmDialog.Hide())
		confirmDialog.SetCancelOnTimeOver() # hotfix
		self.confirmDialog = confirmDialog
	# END_OF_QUEST_CONFIRM

	# GIFT command
	def Gift_Show(self):
		self.interface.ShowGift()

	def Gift_Hide(self):
		self.interface.HideGift()

	# CUBE
	def BINARY_Cube_Open(self, npcVNUM, reload = 0):
		self.currentCubeNPC = npcVNUM

		if reload:
			self.cubeInformation.clear()
			self.interface.wndCube.ClearOldSlots()

		self.interface.OpenCubeWindow()

		if npcVNUM not in self.cubeInformation:
			net.SendChatPacket("/cube r_info")
		else:
			cubeInfoList = self.cubeInformation[npcVNUM]

			i = 0
			for cubeInfo in cubeInfoList:
				self.interface.wndCube.AddCubeResultItem(cubeInfo["vnum"], cubeInfo["count"])

				j = 0
				for materialList in cubeInfo["materialList"]:
					for materialInfo in materialList:
						itemVnum, itemCount = materialInfo
						self.interface.wndCube.AddMaterialInfo(i, j, itemVnum, itemCount)
					j = j + 1

				i = i + 1

			self.interface.wndCube.Refresh()

	def BINARY_Cube_Close(self):
		self.interface.CloseCubeWindow()

	# ���ۿ� �ʿ��� ���, ����Ǵ� �ϼ�ǰ�� VNUM�� ���� ���� update
	def BINARY_Cube_UpdateInfo(self, gold, itemVnum, count):
		self.interface.UpdateCubeInfo(gold, itemVnum, count)

	def BINARY_Cube_Succeed(self, itemVnum, count):
		print "ť�� ���� ����"
		self.interface.SucceedCubeWork(itemVnum, count)
		pass

	def BINARY_Cube_Failed(self):
		print "ť�� ���� ����"
		self.interface.FailedCubeWork()
		pass

	def BINARY_Cube_ResultList(self, npcVNUM, listText):
		# ResultList Text Format : 72723,1/72725,1/72730.1/50001,5 �̷������� "/" ���ڷ� ���е� ����Ʈ�� ��
		#print listText

		if npcVNUM == 0:
			npcVNUM = self.currentCubeNPC

		self.cubeInformation[npcVNUM] = []

		try:
			for eachInfoText in listText.split("/"):
				eachInfo = eachInfoText.split(",")
				itemVnum = int(eachInfo[0])
				itemCount = int(eachInfo[1])

				self.cubeInformation[npcVNUM].append({"vnum": itemVnum, "count": itemCount})
				self.interface.wndCube.AddCubeResultItem(itemVnum, itemCount)

			resultCount = len(self.cubeInformation[npcVNUM])
			requestCount = 5
			modCount = resultCount % requestCount
			splitCount = resultCount / requestCount
			for i in xrange(splitCount):
				#print("/cube r_info %d %d" % (i * requestCount, requestCount))
				net.SendChatPacket("/cube r_info %d %d" % (i * requestCount, requestCount))

			if 0 < modCount:
				#print("/cube r_info %d %d" % (splitCount * requestCount, modCount))
				net.SendChatPacket("/cube r_info %d %d" % (splitCount * requestCount, modCount))

		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0

		pass

	def BINARY_Cube_MaterialInfo(self, startIndex, listCount, listText):
		# Material Text Format : 125,1|126,2|127,2|123,5&555,5&555,4/120000
		try:
			#print listText

			if 3 > len(listText):
				dbg.TraceError("Wrong Cube Material Infomation")
				return 0

			eachResultList = listText.split("@")

			cubeInfo = self.cubeInformation[self.currentCubeNPC]

			itemIndex = 0
			for eachResultText in eachResultList:
				cubeInfo[startIndex + itemIndex]["materialList"] = [[], [], [], [], []]
				materialList = cubeInfo[startIndex + itemIndex]["materialList"]

				gold = 0
				splitResult = eachResultText.split("/")
				if 1 < len(splitResult):
					gold = int(splitResult[1])

				#print "splitResult : ", splitResult
				eachMaterialList = splitResult[0].split("&")

				i = 0
				for eachMaterialText in eachMaterialList:
					complicatedList = eachMaterialText.split("|")

					if 0 < len(complicatedList):
						for complicatedText in complicatedList:
							(itemVnum, itemCount) = complicatedText.split(",")
							itemVnum = int(itemVnum)
							itemCount = int(itemCount)
							self.interface.wndCube.AddMaterialInfo(itemIndex + startIndex, i, itemVnum, itemCount)

							materialList[i].append((itemVnum, itemCount))
					else:
						itemVnum, itemCount = eachMaterialText.split(",")
						itemVnum = int(itemVnum)
						itemCount = int(itemCount)
						self.interface.wndCube.AddMaterialInfo(itemIndex + startIndex, i, itemVnum, itemCount)

						materialList[i].append((itemVnum, itemCount))

					i = i + 1

				itemIndex = itemIndex + 1

			self.interface.wndCube.Refresh()

		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0

		pass

	# END_OF_CUBE

	# ��ȥ��
	def BINARY_Highlight_Item(self, inven_type, inven_pos):
		self.interface.Highligt_Item(inven_type, inven_pos)

	def BINARY_DragonSoulGiveQuilification(self):
		self.interface.DragonSoulGiveQuilification()

	if app.ENABLE_DS_CHANGE_ATTR:
		def BINARY_DragonSoulRefineWindow_Open(self, type):
			self.interface.OpenDragonSoulRefineWindow(type)
	else:
		def BINARY_DragonSoulRefineWindow_Open(self):
			self.interface.OpenDragonSoulRefineWindow()

	def BINARY_DragonSoulRefineWindow_RefineFail(self, reason, inven_type, inven_pos):
		self.interface.FailDragonSoulRefine(reason, inven_type, inven_pos)

	def BINARY_DragonSoulRefineWindow_RefineSucceed(self, inven_type, inven_pos):
		self.interface.SucceedDragonSoulRefine(inven_type, inven_pos)

	# END of DRAGON SOUL REFINE WINDOW

	def BINARY_SetBigMessage(self, message):
		self.interface.bigBoard.SetTip(message)

	def BINARY_SetTipMessage(self, message):
		self.interface.tipBoard.SetTip(message)

	if app.ENABLE_12ZI:
		def BINARY_SetMissionMessage(self, message):
			self.interface.missionBoard.SetMission(message)

		def BINARY_SetSubMissionMessage(self, message):
			self.interface.missionBoard.SetSubMission(message)

		def BINARY_CleanMissionMessage(self):
			self.interface.missionBoard.CleanMission()

	if app.ENABLE_DUNGEON_INFO_SYSTEM:
		def DungeonInfo(self, questindex):
			osfInfo.dungeonData["quest_index"] = questindex

		def CleanDungeonInfo(self):
			osfInfo.dungeonInfo = []

		def CleanDungeonRanking(self):
			osfInfo.dungeonRanking["ranking_list"] = []
			osfInfo.dungeonRanking["my_ranking"] = []

		def GetDungeonInfo(self, cmd):
			cmd = cmd.split("#")

			if cmd[0] == "INPUT":
				osfInfo.INPUT_IGNORE = int(cmd[1])

			elif cmd[0] == "CMD":
				net.SendQuestInputStringPacket(osfInfo.dungeonData["quest_cmd"])
				osfInfo.dungeonData["quest_cmd"] = ""

			else:
				pass

		def UpdateDungeonInfo(self, status, waitTime, type, organization, levelMinLimit, levelMaxLimit, partyMemberMinLimit, partyMemberMaxLimit, mapIndex, mapCoordX, mapCoordY, cooldown, duration, entranceMapIndex, strengthBonusName, resistanceBonusName, itemVnum, itemCount, finished, fastestTime, highestDamage):
			status = int(status)
			waitTime = int(waitTime)
			type = int(type)
			organization = int(organization)
			levelMinLimit = int(levelMinLimit)
			levelMaxLimit = int(levelMaxLimit)
			partyMemberMinLimit = int(partyMemberMinLimit)
			partyMemberMaxLimit = int(partyMemberMaxLimit)
			mapIndex = int(mapIndex)
			mapCoordX = int(mapCoordX)
			mapCoordY = int(mapCoordY)
			cooldown = int(cooldown)
			duration = int(duration)
			entranceMapIndex = int(entranceMapIndex)
			strengthBonusName = str(strengthBonusName).replace("_", " ")
			resistanceBonusName = str(resistanceBonusName).replace("_", " ")
			itemVnum = int(itemVnum)
			itemCount = int(itemCount)
			finished = int(finished)
			fastestTime = int(fastestTime)
			highestDamage = int(highestDamage)

			osfInfo.dungeonInfo.append(
				{
					"status" : status,
					"wait_time" : waitTime,
					"type" : type,
					"organization" : organization,
					"level_limit" : [levelMinLimit, levelMaxLimit],
					"party_member_limit" : [partyMemberMinLimit, partyMemberMaxLimit],
					"map_index" : mapIndex,
					"map_coords" : [mapCoordX, mapCoordY],
					"cooldown" : cooldown,
					"duration" : duration,
					"entrance_map_index" : entranceMapIndex,
					"strength_bonus" : strengthBonusName,
					"resistance_bonus" : resistanceBonusName,
					"required_item" : [itemVnum, itemCount],
					"finished" : finished,
					"fastest_time" : fastestTime,
					"highest_damage" : highestDamage,
				},
			)

		def UpdateDungeonRanking(self, name, level, rankType):
			name = str(name)
			level = int(level)
			rankType = int(rankType)

			osfInfo.dungeonRanking["ranking_list"].append([name, level, rankType],)

		def UpdateMyDungeonRanking(self, position, name, level, rankType):
			position = int(position)
			name = str(name)
			level = int(level)
			rankType = int(rankType)

			osfInfo.dungeonRanking["my_ranking"].append([position, name, level, rankType],)

		def OpenDungeonRanking(self):
			import uiDungeonInfo
			self.DungeonRank = uiDungeonInfo.DungeonRank()
			self.DungeonRank.Open()

	if app.ENABLE_HIDE_COSTUME_SYSTEM:
		def SetBodyCostumeHidden(self, hidden):
			osfInfo.HIDDEN_BODY_COSTUME = int(hidden)
			self.interface.RefreshVisibleCostume()

		def SetHairCostumeHidden(self, hidden):
			osfInfo.HIDDEN_HAIR_COSTUME = int(hidden)
			self.interface.RefreshVisibleCostume()

		def SetAcceCostumeHidden(self, hidden):
			if app.ENABLE_ACCE_COSTUME_SYSTEM:
				osfInfo.HIDDEN_ACCE_COSTUME = int(hidden)
				self.interface.RefreshVisibleCostume()
			else:
				pass

		def SetWeaponCostumeHidden(self, hidden):
			if app.ENABLE_WEAPON_COSTUME_SYSTEM:
				osfInfo.HIDDEN_WEAPON_COSTUME = int(hidden)
				self.interface.RefreshVisibleCostume()
			else:
				pass

	## EVENTS
	def __LateSummerEvent(self):
		EnvironmentData = {
			0:	"d:/ymir work/environment/late_summer.msenv",
			1:	"d:/ymir work/environment/late_summer.msenv",
			2:	"d:/ymir work/environment/late_summer.msenv",
			3:	"d:/ymir work/environment/late_summer.msenv",
			4:	"d:/ymir work/environment/late_summer.msenv",
			5:	"d:/ymir work/environment/late_summer.msenv",
			6:	"d:/ymir work/environment/late_summer.msenv",
			7:	"d:/ymir work/environment/late_summer.msenv",
			8:	"d:/ymir work/environment/late_summer.msenv",
			9:	"d:/ymir work/environment/late_summer.msenv",
			10:	"d:/ymir work/environment/late_summer.msenv",
			11:	"d:/ymir work/environment/late_summer.msenv",
			12:	"d:/ymir work/environment/late_summer.msenv",
			13:	"d:/ymir work/environment/late_summer.msenv",
			14:	"d:/ymir work/environment/late_summer.msenv",
			15:	"d:/ymir work/environment/late_summer.msenv",
			16:	"d:/ymir work/environment/late_summer.msenv",
			17:	"d:/ymir work/environment/late_summer.msenv",
			18:	"d:/ymir work/environment/late_summer.msenv",
			19:	"d:/ymir work/environment/late_summer.msenv",
			20:	"d:/ymir work/environment/late_summer.msenv",
			21:	"d:/ymir work/environment/late_summer.msenv",
			22:	"d:/ymir work/environment/late_summer.msenv",
			23:	"d:/ymir work/environment/late_summer.msenv",
		}

		hour = (app.GetGlobalTimeStamp() / 60) / 60 % 24
		for key, c_pszName in EnvironmentData.iteritems():
			if hour is key and app.IsExistFile(c_pszName):
				background.RegisterEnvironmentData(0, c_pszName)
				background.SetEnvironmentData(0)
	## END_OF_EVENTS

	def BINARY_RecieveWhisperDetails(self, name, country):
		if self.interface:
			self.interface.RecieveWhisperDetails(name, country)

	def BINARY_AppendNotifyMessage(self, type):
		if not type in localeInfo.NOTIFY_MESSAGE:
			return

		chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.NOTIFY_MESSAGE[type])

	def BINARY_Guild_EnterGuildArea(self, areaID):
		self.interface.BULID_EnterGuildArea(areaID)

	def BINARY_Guild_ExitGuildArea(self, areaID):
		self.interface.BULID_ExitGuildArea(areaID)

	def BINARY_GuildWar_OnSendDeclare(self, guildID):
		pass

	def BINARY_GuildWar_OnRecvDeclare(self, guildID, warType):
		mainCharacterName = player.GetMainCharacterName()
		masterName = guild.GetGuildMasterName()
		if mainCharacterName == masterName:
			self.__GuildWar_OpenAskDialog(guildID, warType)

	def BINARY_GuildWar_OnRecvPoint(self, gainGuildID, opponentGuildID, point):
		self.interface.OnRecvGuildWarPoint(gainGuildID, opponentGuildID, point)

	def BINARY_GuildWar_OnStart(self, guildSelf, guildOpp):
		self.interface.OnStartGuildWar(guildSelf, guildOpp)

	def BINARY_GuildWar_OnEnd(self, guildSelf, guildOpp):
		self.interface.OnEndGuildWar(guildSelf, guildOpp)

	def BINARY_BettingGuildWar_SetObserverMode(self, isEnable):
		self.interface.BINARY_SetObserverMode(isEnable)

	def BINARY_BettingGuildWar_UpdateObserverCount(self, observerCount):
		self.interface.wndMiniMap.UpdateObserverCount(observerCount)

	def __GuildWar_UpdateMemberCount(self, guildID1, memberCount1, guildID2, memberCount2, observerCount):
		guildID1 = int(guildID1)
		guildID2 = int(guildID2)
		memberCount1 = int(memberCount1)
		memberCount2 = int(memberCount2)
		observerCount = int(observerCount)

		self.interface.UpdateMemberCount(guildID1, memberCount1, guildID2, memberCount2)
		self.interface.wndMiniMap.UpdateObserverCount(observerCount)

	def __GuildWar_OpenAskDialog(self, guildID, warType):
		guildName = guild.GetGuildName(guildID)

		# REMOVED_GUILD_BUG_FIX
		if "Noname" == guildName:
			return
		# END_OF_REMOVED_GUILD_BUG_FIX

		import uiGuild
		questionDialog = uiGuild.AcceptGuildWarDialog()
		questionDialog.SAFE_SetAcceptEvent(self.__GuildWar_OnAccept)
		questionDialog.SAFE_SetCancelEvent(self.__GuildWar_OnDecline)
		questionDialog.Open(guildName, warType)

		self.guildWarQuestionDialog = questionDialog

	def __GuildWar_CloseAskDialog(self):
		self.guildWarQuestionDialog.Close()
		self.guildWarQuestionDialog = None

	def __GuildWar_OnAccept(self):
		guildName = self.guildWarQuestionDialog.GetGuildName()

		net.SendChatPacket("/war " + guildName)
		self.__GuildWar_CloseAskDialog()

		return 1

	def __GuildWar_OnDecline(self):
		guildName = self.guildWarQuestionDialog.GetGuildName()

		net.SendChatPacket("/nowar " + guildName)
		self.__GuildWar_CloseAskDialog()

		return 1

	## BINARY CALLBACK
	######################################################################################

	def __ServerCommand_Build(self):
		serverCommandList = {
			"ConsoleEnable" : self.__Console_Enable,
			"DayMode" : self.__DayMode_Update,
			"PRESERVE_DayMode" : self.__PRESERVE_DayMode_Update,
			"CloseRestartWindow" : self.__RestartDialog_Close,
			"OpenPrivateShop" : self.__PrivateShop_Open,
			"PartyHealReady" : self.PartyHealReady,
			"ShowMeSafeboxPassword" : self.AskSafeboxPassword,
			"CloseSafebox" : self.CommandCloseSafebox,

			# ITEM_MALL
			"CloseMall" : self.CommandCloseMall,
			"ShowMeMallPassword" : self.AskMallPassword,
			"item_mall" : self.__ItemMall_Open,
			# END_OF_ITEM_MALL

			"RefineSuceeded" : self.RefineSuceededMessage,
			"RefineFailed" : self.RefineFailedMessage,
			"xmas_snow" : self.__XMasSnow_Enable,
			"xmas_boom" : self.__XMasBoom_Enable,
			"xmas_song" : self.__XMasSong_Enable,
			"xmas_tree" : self.__XMasTree_Enable,
			"newyear_boom" : self.__XMasBoom_Enable,
			"PartyRequest" : self.__PartyRequestQuestion,
			"PartyRequestDenied" : self.__PartyRequestDenied,
			"horse_state" : self.__Horse_UpdateState,
			"hide_horse_state" : self.__Horse_HideState,
			"WarUC" : self.__GuildWar_UpdateMemberCount,
			"test_server" : self.__EnableTestServerFlag,
			"mall" : self.__InGameShop_Show,

			# WEDDING
			"lover_login" : self.__LoginLover,
			"lover_logout" : self.__LogoutLover,
			"lover_near" : self.__LoverNear,
			"lover_far" : self.__LoverFar,
			"lover_divorce" : self.__LoverDivorce,
			"PlayMusic" : self.__PlayMusic,
			# END_OF_WEDDING

			# PRIVATE_SHOP_PRICE_LIST
			"MyShopPriceList" : self.__PrivateShop_PriceList,
			# END_OF_PRIVATE_SHOP_PRICE_LIST
		}

		if app.ENABLE_GEM_SYSTEM:
			serverCommandList.update({
				"OpenGuiGem" : self.OpenGuiGem,
				"GemCheck" : self.GemCheck,
				"OpenGuiGemMarket" : self.OpenGuiGemMarket,
				"GemMarketSlotsDesblock" : self.GemMarketSlotsDesblock,
				"GemMarketItems" : self.GemMarketItems,
				"GemMarketClear" : self.GemMarketClear,
				"GemMarketTime" : self.GemTimeMarket,
			})

		if app.ENABLE_CAPTCHA_SYSTEM:
			serverCommandList.update({
				"IsAFKMode" : self.IsAFKMode,
				"ShowCaptcha" : self.ShowCaptcha,
			})

		if app.ENABLE_12ZI:
			serverCommandList.update({
				"SetMissionMessage" : self.BINARY_SetMissionMessage,
				"SetSubMissionMessage" : self.BINARY_SetSubMissionMessage,
				"CleanMissionMessage" : self.BINARY_CleanMissionMessage,
			})

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			serverCommandList.update({
				"DungeonInfo" : self.DungeonInfo,
				"CleanDungeonInfo" : self.CleanDungeonInfo,
				"CleanDungeonRanking" : self.CleanDungeonRanking,
				"GetDungeonInfo" : self.GetDungeonInfo,
				"UpdateDungeonInfo" : self.UpdateDungeonInfo,
				"UpdateDungeonRanking" : self.UpdateDungeonRanking,
				"UpdateMyDungeonRanking" : self.UpdateMyDungeonRanking,
				"OpenDungeonRanking" : self.OpenDungeonRanking,
			})

		if app.ENABLE_HIDE_COSTUME_SYSTEM:
			serverCommandList.update({
				"SetBodyCostumeHidden" : self.SetBodyCostumeHidden,
				"SetHairCostumeHidden" : self.SetHairCostumeHidden,
				"SetAcceCostumeHidden" : self.SetAcceCostumeHidden,
				"SetWeaponCostumeHidden" : self.SetWeaponCostumeHidden,
			})

		if app.ENABLE_EXPRESSING_EMOTION:
			serverCommandList.update({
				"RegisterSpecialEmotion" : self.RegisterSpecialEmotion,
				"ClearSpecialEmotions" : self.ClearSpecialEmotions,
			})

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			serverCommandList.update({
				"OpenSpecialInventoryWindow" : self.OpenSpecialInventoryWindow,
			})

		serverCommandList.update({
			"OpenCostumeWindow" : self.OpenCostumeWindow,
		})

		if app.ENABLE_MOVE_CHANNEL:
			serverCommandList.update({
				"ChannelInfo" : self.ChannelInfo
			})

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			serverCommandList["ShowChangeDialog"] = self.__ChangeWindowOpen

		if app.ENABLE_GUILD_DRAGONLAIR_SYSTEM:
			serverCommandList.update({
				"ClearDragonLairRanking" : self.ClearDragonLairRanking,
				"OpenDragonLairRanking" : self.OpenDragonLairRanking,
				"AddDragonLairRanking" : self.AddDragonLairRanking,
			})

		if app.ENABLE_SOUL_SYSTEM:
			serverCommandList["RefineSoulSuceeded"] = self.__RefineSoulSuceededMessage
			serverCommandList["RefineSoulFailed"] = self.__RefineSoulFailedMessage

		if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
			serverCommandList["OpenSkillbookCombinationDialog"] = self.OpenSkillbookCombinationDialog

		if app.ENABLE_MYSHOP_DECO:
			serverCommandList["OpenMyShopDecoWnd"] = self.__OpenMyShopDecoWnd

		self.serverCommander = stringCommander.Analyzer()
		for serverCommandItem in serverCommandList.items():
			self.serverCommander.SAFE_RegisterCallBack(
				serverCommandItem[0], serverCommandItem[1]
			)

	if app.ENABLE_CHANGE_LOOK_SYSTEM:
		def __ChangeWindowOpen(self, type):
			self.interface.ChangeWindowOpen(type)

	def BINARY_ServerCommand_Run(self, line):
		#dbg.TraceError(line)
		try:
			#print " BINARY_ServerCommand_Run", line
			return self.serverCommander.Run(line)
		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0

	def __ProcessPreservedServerCommand(self):
		try:
			command = net.GetPreservedServerCommand()
			while command:
				print " __ProcessPreservedServerCommand", command
				self.serverCommander.Run(command)
				command = net.GetPreservedServerCommand()
		except RuntimeError, msg:
			dbg.TraceError(msg)
			return 0

	def PartyHealReady(self):
		self.interface.PartyHealReady()

	def AskSafeboxPassword(self):
		self.interface.AskSafeboxPassword()

	if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
		def OpenSkillbookCombinationDialog(self):
			self.interface.OpenSkillbookCombinationDialog()

	# ITEM_MALL
	def AskMallPassword(self):
		self.interface.AskMallPassword()

	def __ItemMall_Open(self):
		self.interface.OpenItemMall();

	def CommandCloseMall(self):
		self.interface.CommandCloseMall()
	# END_OF_ITEM_MALL

	def RefineSuceededMessage(self):
		snd.PlaySound("sound/ui/make_soket.wav")
		self.PopupMessage(localeInfo.REFINE_SUCCESS)
		if app.ENABLE_AUTO_REFINE:
			self.interface.RefineFail(False)

	def RefineFailedMessage(self):
		snd.PlaySound("sound/ui/jaeryun_fail.wav")
		self.PopupMessage(localeInfo.REFINE_FAILURE)
		if app.ENABLE_AUTO_REFINE:
			self.interface.RefineFail(True)

	if app.ENABLE_SOUL_SYSTEM:
		def __RefineSoulSuceededMessage(self):
			snd.PlaySound("sound/ui/make_soket.wav")
			self.PopupMessage(localeInfo.SOUL_REFINE_SUCCESS)

		def __RefineSoulFailedMessage(self):
			snd.PlaySound("sound/ui/jaeryun_fail.wav")
			self.PopupMessage(localeInfo.SOUL_REFINE_FAILURE)

	if app.ENABLE_REFINE_FAIL_TYPE:
		def BINARY_RefineFailedTypeMessage(self, type):
			REFINE_FAILURE_DICT = {
				player.REFINE_FAIL_GRADE_DOWN : localeInfo.REFINE_FAILURE_GRADE_DOWN,
				player.REFINE_FAIL_DEL_ITEM : localeInfo.REFINE_FAILURE_DEL_ITEM,
				player.REFINE_FAIL_KEEP_GRADE : localeInfo.REFINE_FAILURE_KEEP_GRADE,
				player.REFINE_FAIL_MAX : localeInfo.REFINE_FAILURE
			}

			snd.PlaySound("sound/ui/jaeryun_fail.wav")
			self.PopupMessage(REFINE_FAILURE_DICT.get(type, player.REFINE_FAIL_MAX))
			if app.ENABLE_AUTO_REFINE:
				self.interface.RefineFail(True)

	def CommandCloseSafebox(self):
		self.interface.CommandCloseSafebox()

	# PRIVATE_SHOP_PRICE_LIST
	def __PrivateShop_PriceList(self, itemVNum, itemPrice):
		if app.ENABLE_CHEQUE_SYSTEM:
			return
		else:
			uiPrivateShopBuilder.SetPrivateShopItemPrice(itemVNum, itemPrice)
	# END_OF_PRIVATE_SHOP_PRICE_LIST

	def __Horse_HideState(self):
		self.affectShower.SetHorseState(0, 0, 0)

	def __Horse_UpdateState(self, level, health, battery):
		self.affectShower.SetHorseState(int(level), int(health), int(battery))

	if app.ENABLE_CONQUEROR_LEVEL:
		def BINARY_SungMaAttr(self, str, hp, move, immune):
			self.affectShower.SetSungMaAffectImage(str, hp, move, immune)

	def __IsXMasMap(self):
		mapDict = (
			"metin2_map_n_flame_01",
			"metin2_map_n_desert_01",
			"metin2_map_spiderdungeon_01",
			"metin2_map_spiderdungeon_02",
			"metin2_map_spiderdungeon_03",
			"metin2_map_deviltower1",
			"metin2_map_sungzi_flame_hill_01",
			"metin2_map_sungzi_flame_hill_02",
			"metin2_map_sungzi_flame_hill_03",
			"metin2_map_sungzi_desert_01",
			"metin2_map_sungzi_desert_hill_01",
			"metin2_map_sungzi_desert_hill_02",
			"metin2_map_sungzi_desert_hill_03",
			"metin2_map_empirewar03",
			"metin2_map_devilsCatacomb",
			"metin2_map_Mt_Thunder",
			"metin2_map_n_flame_dungeon_01",
			"metin2_map_dawnmist_dungeon_01",
			"metin2_map_Mt_Th_dungeon_01",
		)

		if background.GetCurrentMapName() in mapDict:
			return False

		return True

	def __XMasSnow_Enable(self, mode):
		self.__XMasSong_Enable(mode)

		if "1" == mode:
			if app.ENABLE_ENVIRONMENT_EFFECT_OPTION:
				background.SetXMasShowEvent(1)

			if not self.__IsXMasMap():
				return

			print "XMAS_SNOW ON"
			background.EnableSnow(1)
		else:
			if app.ENABLE_ENVIRONMENT_EFFECT_OPTION:
				background.SetXMasShowEvent(0)

			print "XMAS_SNOW OFF"
			background.EnableSnow(0)

	def __XMasBoom_Enable(self, mode):
		if "1" == mode:
			if app.ENABLE_ENVIRONMENT_EFFECT_OPTION:
				if not background.IsBoomMap():
					return
			else:
				if not self.__IsXMasMap():
					return

			print "XMAS_BOOM ON"
			self.__DayMode_Update("dark")
			self.enableXMasBoom = True
			self.startTimeXMasBoom = app.GetTime()
		else:
			print "XMAS_BOOM OFF"
			self.__DayMode_Update("light")
			self.enableXMasBoom = False

	def __XMasTree_Enable(self, grade):
		print "XMAS_TREE ", grade
		background.SetXMasTree(int(grade))

	def __XMasSong_Enable(self, mode):
		if "1" == mode:
			print "XMAS_SONG ON"

			XMAS_BGM = "xmas.mp3"

			if app.IsExistFile("BGM/" + XMAS_BGM) == 1:
				if musicInfo.fieldMusic != "":
					snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic)

				musicInfo.fieldMusic = XMAS_BGM
				snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)
		else:
			print "XMAS_SONG OFF"

			if musicInfo.fieldMusic != "":
				snd.FadeOutMusic("BGM/" + musicInfo.fieldMusic)

			musicInfo.fieldMusic = musicInfo.METIN2THEMA
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

	def __RestartDialog_Close(self):
		self.interface.CloseRestartDialog()

	def __Console_Enable(self):
		self.consoleEnable = True
		app.EnableSpecialCameraMode()
		ui.EnablePaste(True)
		self.ShowConsole()

	## PrivateShop
	if app.ENABLE_CHEQUE_SYSTEM:
		def __CashPrivateShop_Open(self):
			if app.ENABLE_MYSHOP_DECO:
				return

			self.interface.OpenPrivateShopInputNameDialog(True)

		def __PrivateShop_Open(self):
			if app.ENABLE_MYSHOP_DECO:
				return

			self.interface.OpenPrivateShopInputNameDialog(False)
	else:
		def __PrivateShop_Open(self):
			self.interface.OpenPrivateShopInputNameDialog()

	if app.ENABLE_MYSHOP_DECO:
		def MyPrivShopOpen(self, cashItem, tabCnt):
			self.interface.OpenPrivateShopInputNameDialog(cashItem, tabCnt)

		def __OpenMyShopDecoWnd(self):
			self.interface.OpenMyShopDecoWnd()

		def BINARY_PrivateShop_Appear(self, vid, text, type):
			self.interface.AppearPrivateShop(vid, text, type)
	else:
		def BINARY_PrivateShop_Appear(self, vid, text):
			self.interface.AppearPrivateShop(vid, text)

	def BINARY_PrivateShop_Disappear(self, vid):
		self.interface.DisappearPrivateShop(vid)

	if app.ENABLE_MINI_GAME_OKEY:
		## MiniGame Rumi
		def BINARY_Cards_UpdateInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points):
			self.interface.UpdateCardsInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points)

		def BINARY_Cards_FieldUpdateInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
			self.interface.UpdateCardsFieldInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)

		def BINARY_Cards_PutReward(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
			self.interface.CardsPutReward(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)

		def BINARY_Cards_ShowIcon(self, active):
			IsRumiGameEvent = int(active)
			if IsRumiGameEvent > 0:
				osfInfo.RUMI_GAME_EVENT = True
			else:
				osfInfo.RUMI_GAME_EVENT = False

			if osfInfo.EVENT_BANNER == True:
				self.interface.IntegrationEventBanner()
			else:
				self.interface.MiniGameOkey()

		def BINARY_Cards_Open(self, safemode):
			self.interface.OpenCardsWindow(safemode)

	if app.ENABLE_MINI_GAME_CATCH_KING:
		def MiniGameCatchKingEvent(self, isEnable):
			if self.interface:
				self.interface.SetCatchKingEventStatus(isEnable)
				if osfInfo.EVENT_BANNER == True:
					self.interface.IntegrationEventBanner()

		def MiniGameCatchKingEventStart(self, bigScore):
			self.interface.MiniGameCatchKingEventStart(bigScore)

		def MiniGameCatchKingSetHandCard(self, cardNumber):
			self.interface.MiniGameCatchKingSetHandCard(cardNumber)

		def MiniGameCatchKingResultField(self, score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
			self.interface.MiniGameCatchKingResultField(score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)

		def MiniGameCatchKingSetEndCard(self, cardPos, cardNumber):
			self.interface.MiniGameCatchKingSetEndCard(cardPos, cardNumber)

		def MiniGameCatchKingReward(self, rewardCode):
			self.interface.MiniGameCatchKingReward(rewardCode)

	## DayMode
	def __PRESERVE_DayMode_Update(self, mode):
		if "light" == mode:
			if app.ENABLE_ENVIRONMENT_EFFECT_OPTION:
				if not background.IsBoomMap():
					return
			else:
				if not self.__IsXMasMap():
					return

			background.SetEnvironmentData(background.DAY_MODE_LIGHT)
		elif "dark" == mode:
			if app.ENABLE_ENVIRONMENT_EFFECT_OPTION:
				if not background.IsBoomMap():
					return
			else:
				if not self.__IsXMasMap():
					return

			background.RegisterEnvironmentData(background.DAY_MODE_DARK, constInfo.ENVIRONMENT_NIGHT)
			background.SetEnvironmentData(background.DAY_MODE_DARK)

	def __DayMode_Update(self, mode):
		if "light" == mode:
			if app.ENABLE_12ZI:
				if app.ENABLE_ENVIRONMENT_EFFECT_OPTION:
					if not background.IsBoomMap():
						return
					else:
						if not self.__IsXMasMap():
							return

			self.curtain.SAFE_FadeOut(self.__DayMode_OnCompleteChangeToLight)
		elif "dark" == mode:
			if app.ENABLE_ENVIRONMENT_EFFECT_OPTION:
				if not background.IsBoomMap():
					return
			else:
				if not self.__IsXMasMap():
					return

			self.curtain.SAFE_FadeOut(self.__DayMode_OnCompleteChangeToDark)

	def __DayMode_OnCompleteChangeToLight(self):
		background.SetEnvironmentData(background.DAY_MODE_LIGHT)
		self.curtain.FadeIn()

	def __DayMode_OnCompleteChangeToDark(self):
		background.RegisterEnvironmentData(background.DAY_MODE_DARK, constInfo.ENVIRONMENT_NIGHT)
		background.SetEnvironmentData(background.DAY_MODE_DARK)
		self.curtain.FadeIn()

	## XMasBoom
	def __XMasBoom_Update(self):
		self.BOOM_DATA_LIST = ( (2, 5), (5, 2), (7, 3), (10, 3), (20, 5) )
		if self.indexXMasBoom >= len(self.BOOM_DATA_LIST):
			return

		boomTime = self.BOOM_DATA_LIST[self.indexXMasBoom][0]
		boomCount = self.BOOM_DATA_LIST[self.indexXMasBoom][1]

		if app.GetTime() - self.startTimeXMasBoom > boomTime:
			self.indexXMasBoom += 1

			for i in xrange(boomCount):
				self.__XMasBoom_Boom()

	def __XMasBoom_Boom(self):
		x, y, z = player.GetMainCharacterPosition()
		randX = app.GetRandom(-150, 150)
		randY = app.GetRandom(-150, 150)

		snd.PlaySound3D(x+randX, -y+randY, z, "sound/common/etc/salute.mp3")

	if app.WJ_NEW_USER_CARE:
		def __PartyRequestQuestion(self, vid, name):
			vid = int(vid)
			partyRequestQuestionDialog = uiCommon.QuestionDialogWithTimeLimit()
			partyRequestQuestionDialog.SetAcceptText(localeInfo.UI_ACCEPT)
			partyRequestQuestionDialog.SetCancelText(localeInfo.UI_DENY)
			partyRequestQuestionDialog.SetAcceptEvent(lambda arg=True: self.__AnswerPartyRequest(arg))
			partyRequestQuestionDialog.SetCancelEvent(lambda arg=False: self.__AnswerPartyRequest(arg))
			partyRequestQuestionDialog.Open(name + localeInfo.PARTY_DO_YOU_ACCEPT, 10)
			partyRequestQuestionDialog.SetTimeOverMsg(localeInfo.PARTY_ANSWER_TIMEOVER)
			partyRequestQuestionDialog.SetCancelOnTimeOver()
			partyRequestQuestionDialog.vid = vid
			self.partyRequestQuestionDialog = partyRequestQuestionDialog
	else:
		def __PartyRequestQuestion(self, vid):
			vid = int(vid)
			partyRequestQuestionDialog = uiCommon.QuestionDialog()
			partyRequestQuestionDialog.SetText(chr.GetNameByVID(vid) + localeInfo.PARTY_DO_YOU_ACCEPT)
			partyRequestQuestionDialog.SetAcceptText(localeInfo.UI_ACCEPT)
			partyRequestQuestionDialog.SetCancelText(localeInfo.UI_DENY)
			partyRequestQuestionDialog.SetAcceptEvent(lambda arg=True: self.__AnswerPartyRequest(arg))
			partyRequestQuestionDialog.SetCancelEvent(lambda arg=False: self.__AnswerPartyRequest(arg))
			partyRequestQuestionDialog.Open()
			partyRequestQuestionDialog.vid = vid
			self.partyRequestQuestionDialog = partyRequestQuestionDialog

	def __AnswerPartyRequest(self, answer):
		if not self.partyRequestQuestionDialog:
			return

		vid = self.partyRequestQuestionDialog.vid

		if answer:
			net.SendChatPacket("/party_request_accept " + str(vid))
		else:
			net.SendChatPacket("/party_request_deny " + str(vid))

		self.partyRequestQuestionDialog.Close()
		self.partyRequestQuestionDialog = None

	def __PartyRequestDenied(self):
		self.PopupMessage(localeInfo.PARTY_REQUEST_DENIED)

	def __EnableTestServerFlag(self):
		app.EnableTestServerFlag()

	def __InGameShop_Show(self, url):
		if constInfo.IN_GAME_SHOP_ENABLE:
			self.interface.OpenWebWindow(url)

	# WEDDING
	def __LoginLover(self):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnLoginLover()

	def __LogoutLover(self):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.OnLogoutLover()
		if self.affectShower:
			self.affectShower.HideLoverState()

	def __LoverNear(self):
		if self.affectShower:
			self.affectShower.ShowLoverState()

	def __LoverFar(self):
		if self.affectShower:
			self.affectShower.HideLoverState()

	def __LoverDivorce(self):
		if self.interface.wndMessenger:
			self.interface.wndMessenger.ClearLoverInfo()
		if self.affectShower:
			self.affectShower.ClearLoverState()

	def __PlayMusic(self, flag, filename):
		flag = int(flag)
		if flag:
			snd.FadeOutAllMusic()
			musicInfo.SaveLastPlayFieldMusic()
			snd.FadeInMusic("BGM/" + filename)
		else:
			snd.FadeOutAllMusic()
			musicInfo.LoadLastPlayFieldMusic()
			snd.FadeInMusic("BGM/" + musicInfo.fieldMusic)

	# END_OF_WEDDING

	if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
		def OpenPShopSearchDialogCash(self):
			self.interface.OpenPShopSearchDialogCash()
		def RefreshPShopSearchDialog(self):
			self.interface.RefreshPShopSearchDialog()

	if app.WJ_ENABLE_PICKUP_ITEM_EFFECT:
		## HilightSlot Change
		def DeactivateSlot(self, slotindex, type):
			self.interface.DeactivateSlot(slotindex, type)

		## HilightSlot Change
		def ActivateSlot(self, slotindex, type):
			self.interface.ActivateSlot(slotindex, type)

	if app.ENABLE_GUILD_DRAGONLAIR_SYSTEM:
		def ClearDragonLairRanking(self, type):
			if self.interface:
				self.interface.ClearDragonLairRanking(int(type))

		def OpenDragonLairRanking(self, type):
			if self.interface:
				self.interface.OpenDragonLairRanking(int(type))

		def AddDragonLairRanking(self, type, data):
			if self.interface:
				line = int(data.split("#")[1])
				guildname = str(data.split("#")[2])
				membercount = int(data.split("#")[3])
				sec = int(data.split("#")[4])
				min = int(sec / 60)
				sec %= 60

				self.interface.AddDragonLairRanking(int(type), guildname, membercount, min, sec)

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def ActAcce(self, iAct, bWindow):
			if self.interface:
				self.interface.ActAcce(iAct, bWindow)

		def AlertAcce(self, bWindow):
			snd.PlaySound("sound/ui/make_soket.wav")
			if bWindow:
				self.PopupMessage(localeInfo.ACCE_DEL_SERVEITEM)
			else:
				self.PopupMessage(localeInfo.ACCE_DEL_ABSORDITEM)

	if app.ENABLE_MAILBOX:
		def MailBoxProcess(self, type, data):
			if self.interface:
				self.interface.MailBoxProcess( type, data )
