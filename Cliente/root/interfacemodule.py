##
## Interface
##
import constInfo
import osfInfo
import systemSetting
import wndMgr
import chat
import app
import player
import uiTaskBar
import uiCharacter
import uiInventory
import uiDragonSoul
import uiChat
import uiMessenger
import guild

import ui
import uiHelp
import uiWhisper
import uiPointReset
import uiShop
import uiExchange
import uiSystem
import uiRestart
import uiToolTip
import uiMiniMap
import uiParty
import uiSafebox
import uiGuild
import uiQuest
import uiPrivateShopBuilder
import uiCommon
import uiRefine
import uiEquipmentDialog
import uiGameButton
import uiTip
import uiCube
import miniMap
# ACCESSORY_REFINE_ADD_METIN_STONE
import uiselectitem
# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
if app.ENABLE_GEM_SYSTEM:
	import uiselectitemEx
import uiScriptLocale
import event
import localeInfo
import item
if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
	import uiSkillBookCombination
if app.ENABLE_ACCE_COSTUME_SYSTEM:
	import uiAcce

if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
	import uiSpecialInventory

import uiMiniGame

if app.ENABLE_CHANGE_LOOK_SYSTEM:
	import uiChangeLook
	import shop

if app.ENABLE_MINI_GAME_OKEY:
	import uiMiniGameRumi

if app.ENABLE_GEM_SYSTEM:
	import uiGemShop

if app.ENABLE_DUNGEON_INFO_SYSTEM:
	import uiDungeonInfo

if app.ENABLE_CAPTCHA_SYSTEM:
	import uiCaptcha

if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
	import uiPrivateShopSearch

if app.ENABLE_GUILD_DRAGONLAIR_SYSTEM:
	import uiGuildDragonLairRanking

if app.ENABLE_MYSHOP_DECO:
	import uiMyShopDecoration
import uiPrivateShop

if app.ENABLE_MAILBOX:
	import uiMailBox

IsQBHide = 0
IsWisperHide = 0

class Interface(object):
	CHARACTER_STATUS_TAB = 1
	CHARACTER_SKILL_TAB = 2

	def __init__(self):
		systemSetting.SetInterfaceHandler(self)
		self.windowOpenPosition = 0
		self.dlgWhisperWithoutTarget = None
		self.inputDialog = None
		self.tipBoard = None
		self.bigBoard = None

		if app.ENABLE_12ZI:
			self.missionBoard = None

		# ITEM_MALL
		self.mallPageDlg = None
		# END_OF_ITEM_MALL

		self.wndWeb = None
		self.wndTaskBar = None
		self.wndCharacter = None
		self.wndInventory = None
		self.wndExpandedTaskBar = None
		self.wndDragonSoul = None
		self.wndDragonSoulRefine = None
		self.wndChat = None
		self.wndMessenger = None
		self.wndMiniMap = None
		self.wndGuild = None
		self.wndGuildBuilding = None
		if app.WJ_ENABLE_TRADABLE_ICON or app.ENABLE_MOVE_COSTUME_ATTR:
			self.OnTopWindow = None
			self.dlgShop = None
			self.dlgExchange = None
			self.privateShopBuilder = None
			self.wndSafebox = None
		self.listGMName = {}
		self.wndQuestWindow = {}
		self.wndQuestWindowNewKey = 0
		self.privateShopAdvertisementBoardDict = {}
		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}

		if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
			self.wndSkillBookCombination = None

		if app.ENABLE_MYSHOP_DECO:
			self.wndMyShopDeco = None

		self.wndMiniGame = None

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			self.wndChangeLook = None

		if app.ENABLE_GEM_SYSTEM:
			self.wndExpandedMoneyTaskBar = None

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndSpecialInventory = None

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.wndDungeonInfo = None

		if app.ENABLE_CAPTCHA_SYSTEM:
			self.wndCaptcha = None

		if app.ENABLE_MAILBOX:
			self.mail_box = None

		event.SetInterfaceWindow(self)
		self.uiAffectshower = None
		self.uitargetBoard = None
		if app.ENABLE_SHIP_DEFENSE:
			self.uiAllianceTargetBoard = None
		self.IsHideUiMode = False

	def __del__(self):
		systemSetting.DestroyInterfaceHandler()
		event.SetInterfaceWindow(None)

	################################
	## Make Windows & Dialogs
	def __MakeUICurtain(self):
		wndUICurtain = ui.Bar("TOP_MOST")
		wndUICurtain.SetSize(wndMgr.GetScreenWidth(), wndMgr.GetScreenHeight())
		wndUICurtain.SetColor(0x77000000)
		wndUICurtain.Hide()
		self.wndUICurtain = wndUICurtain

	def __MakeMessengerWindow(self):
		self.wndMessenger = uiMessenger.MessengerWindow()
		if app.ENABLE_CHATTING_WINDOW_RENEWAL:
			self.wndMessenger.BindInterface(self)

		from _weakref import proxy
		self.wndMessenger.SetWhisperButtonEvent(lambda n,i=proxy(self):i.OpenWhisperDialog(n))
		self.wndMessenger.SetGuildButtonEvent(ui.__mem_func__(self.ToggleGuildWindow))

	def __MakeGuildWindow(self):
		self.wndGuild = uiGuild.GuildWindow()

	def __MakeChatWindow(self):
		wndChat = uiChat.ChatWindow()

		wndChat.SetSize(wndChat.CHAT_WINDOW_WIDTH, 0)
		wndChat.SetPosition(wndMgr.GetScreenWidth()/2 - wndChat.CHAT_WINDOW_WIDTH/2, wndMgr.GetScreenHeight() - wndChat.EDIT_LINE_HEIGHT - 37)
		wndChat.SetHeight(200)
		wndChat.Refresh()
		wndChat.Show()

		self.wndChat = wndChat
		self.wndChat.BindInterface(self)
		self.wndChat.SetSendWhisperEvent(ui.__mem_func__(self.OpenWhisperDialogWithoutTarget))
		self.wndChat.SetOpenChatLogEvent(ui.__mem_func__(self.ToggleChatLogWindow))

	def __MakeTaskBar(self):
		wndTaskBar = uiTaskBar.TaskBar()
		wndTaskBar.LoadWindow()
		self.wndTaskBar = wndTaskBar
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CHARACTER, ui.__mem_func__(self.ToggleCharacterWindowStatusPage))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_INVENTORY, ui.__mem_func__(self.ToggleInventoryWindow))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_MESSENGER, ui.__mem_func__(self.ToggleMessenger))
		self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_SYSTEM, ui.__mem_func__(self.ToggleSystemDialog))
		if uiTaskBar.TaskBar.IS_EXPANDED:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_EXPAND, ui.__mem_func__(self.ToggleExpandedButton))
			self.wndExpandedTaskBar = uiTaskBar.ExpandedTaskBar()
			self.wndExpandedTaskBar.LoadWindow()
			self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, ui.__mem_func__(self.ToggleDragonSoulWindow))
			if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
				self.wndExpandedTaskBar.SetToggleButtonEvent(uiTaskBar.ExpandedTaskBar.BUTTON_SPECIAL_INVENTORY, ui.__mem_func__(self.ToggleSpecialInventoryWindow))
		else:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_CHAT, ui.__mem_func__(self.ToggleChat))

		self.wndEnergyBar = None
		if app.ENABLE_ENERGY_SYSTEM:
			wndEnergyBar = uiTaskBar.EnergyBar()
			wndEnergyBar.LoadWindow()
			self.wndEnergyBar = wndEnergyBar

		if app.ENABLE_GEM_SYSTEM:
			self.wndTaskBar.SetToggleButtonEvent(uiTaskBar.TaskBar.BUTTON_EXPAND_MONEY, ui.__mem_func__(self.ToggleExpandedMoneyButton))
			self.wndExpandedMoneyTaskBar = uiTaskBar.ExpandedMoneyTaskBar()
			self.wndExpandedMoneyTaskBar.LoadWindow()
			if self.wndInventory:
				self.wndInventory.SetExpandedMoneyBar(self.wndExpandedMoneyTaskBar)

	def __MakeParty(self):
		wndParty = uiParty.PartyWindow()
		wndParty.Hide()
		self.wndParty = wndParty

	def __MakeGameButtonWindow(self):
		wndGameButton = uiGameButton.GameButtonWindow()
		wndGameButton.SetTop()
		wndGameButton.Show()
		wndGameButton.SetButtonEvent("STATUS", ui.__mem_func__(self.__OnClickStatusPlusButton))
		wndGameButton.SetButtonEvent("SKILL", ui.__mem_func__(self.__OnClickSkillPlusButton))
		wndGameButton.SetButtonEvent("QUEST", ui.__mem_func__(self.__OnClickQuestButton))
		wndGameButton.SetButtonEvent("HELP", ui.__mem_func__(self.__OnClickHelpButton))
		wndGameButton.SetButtonEvent("BUILD", ui.__mem_func__(self.__OnClickBuildButton))

		self.wndGameButton = wndGameButton

	def __IsChatOpen(self):
		return True

	def __MakeWindows(self):
		self.wndCharacter = uiCharacter.CharacterWindow()

		self.wndInventory = uiInventory.InventoryWindow()
		self.wndInventory.BindInterfaceClass(self)

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul = uiDragonSoul.DragonSoulWindow()
			self.wndDragonSoul.BindInterfaceClass(self)
			self.wndDragonSoulRefine = uiDragonSoul.DragonSoulRefineWindow()
		else:
			wndDragonSoul = None
			wndDragonSoulRefine = None

		self.wndMiniMap = uiMiniMap.MiniMap()
		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.wndMiniMap.BindInterfaceClass(self)
		self.wndSafebox = uiSafebox.SafeboxWindow()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.wndSafebox.BindInterface(self)

		if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
			self.wndSkillBookCombination = uiSkillBookCombination.SkillBookCombinationWindow()

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			self.wndChangeLook = uiChangeLook.ChangeLookWindow()

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			self.wndPrivateShopSearch = uiPrivateShopSearch.PrivateShopSeachWindow()

		if app.ENABLE_MYSHOP_DECO:
			self.wndMyShopDeco = uiMyShopDecoration.MyShopDecoration()

		self.wndMall = uiSafebox.MallWindow()
		wndChatLog = uiChat.ChatLogWindow()
		wndChatLog.BindInterface(self)
		self.wndChatLog = wndChatLog

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)
			self.wndDragonSoulRefine.SetInventoryWindows(self.wndInventory, self.wndDragonSoul)
			self.wndInventory.SetDragonSoulRefineWindow(self.wndDragonSoulRefine)

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndSpecialInventory = uiSpecialInventory.SpecialInventoryWindow()
			self.wndSpecialInventory.BindInterfaceClass(self)

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			self.wndDungeonInfo = uiDungeonInfo.DungeonInfoWindow()

		if app.ENABLE_CAPTCHA_SYSTEM:
			self.wndCaptcha = uiCaptcha.CaptchaDialog()
			self.wndCaptcha.BindInterface(self)

		if app.ENABLE_MAILBOX:
			self.mail_box = uiMailBox.MailBox()
			self.mail_box.BindInterface(self)
			self.mail_box.SetInven(self.wndInventory)
			self.mail_box.SetDSWindow(self.wndDragonSoul)

	def __MakeDialogs(self):
		self.dlgExchange = uiExchange.ExchangeDialog()
		self.dlgExchange.LoadDialog()
		self.dlgExchange.SetCenterPosition()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.dlgExchange.BindInterface(self)
		self.dlgExchange.Hide()

		self.dlgPointReset = uiPointReset.PointResetDialog()
		self.dlgPointReset.LoadDialog()
		self.dlgPointReset.Hide()

		self.dlgShop = uiShop.ShopDialog()
		self.dlgShop.LoadDialog()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.dlgShop.BindInterface(self)
		self.dlgShop.Hide()

		self.dlgPrivateShop = uiPrivateShop.PrivateShopDialog()
		self.dlgPrivateShop.Hide()

		self.dlgRestart = uiRestart.RestartDialog()
		self.dlgRestart.LoadDialog()
		self.dlgRestart.Hide()

		self.dlgSystem = uiSystem.SystemDialog()
		self.dlgSystem.LoadDialog()
		self.dlgSystem.SetOpenHelpWindowEvent(ui.__mem_func__(self.OpenHelpWindow))

		self.dlgSystem.Hide()

		self.dlgPassword = uiSafebox.PasswordDialog()
		self.dlgPassword.Hide()

		self.hyperlinkItemTooltip = uiToolTip.HyperlinkItemToolTip()
		self.hyperlinkItemTooltip.Hide()

		self.tooltipItem = uiToolTip.ItemToolTip()
		if app.ENABLE_DS_SET:
			self.tooltipItem.BindInterface(self)
		self.tooltipItem.Hide()

		self.tooltipSkill = uiToolTip.SkillToolTip()
		self.tooltipSkill.Hide()

		self.privateShopBuilder = uiPrivateShopBuilder.PrivateShopBuilder()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.privateShopBuilder.BindInterface(self)
		self.privateShopBuilder.Hide()

		self.dlgRefineNew = uiRefine.RefineDialogNew()
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.dlgRefineNew.SetInven(self.wndInventory)
		self.dlgRefineNew.Hide()

	def __MakeHelpWindow(self):
		self.wndHelp = uiHelp.HelpWindow()
		self.wndHelp.LoadDialog()
		self.wndHelp.SetCloseEvent(ui.__mem_func__(self.CloseHelpWindow))
		self.wndHelp.Hide()

	def __MakeTipBoard(self):
		self.tipBoard = uiTip.TipBoard()
		self.tipBoard.Hide()

		self.bigBoard = uiTip.BigBoard()
		self.bigBoard.Hide()

		if app.ENABLE_12ZI:
			self.missionBoard = uiTip.MissionBoard()
			self.missionBoard.Hide()

	def __MakeWebWindow(self):
		if constInfo.IN_GAME_SHOP_ENABLE:
			import uiWeb
			self.wndWeb = uiWeb.WebWindow()
			self.wndWeb.LoadWindow()
			self.wndWeb.Hide()

	if app.ENABLE_GUILD_DRAGONLAIR_SYSTEM:
		def __MakeGuildDragonLairRanking(self):
			self.wndGuildDragonLairRanking = uiGuildDragonLairRanking.GuildDragonLairRankingDialog()
			self.wndGuildDragonLairRanking.Hide()

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def __MakeAcceWindow(self):
			self.wndAcceCombine = uiAcce.CombineWindow()
			self.wndAcceCombine.LoadWindow()
			if app.WJ_ENABLE_TRADABLE_ICON:
				self.wndAcceCombine.BindInterface(self)
				if self.wndInventory:
					self.wndAcceCombine.SetInven(self.wndInventory)
				if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
					if self.wndSpecialInventory:
						self.wndAcceCombine.SetSpecialInven(self.wndSpecialInventory)
			self.wndAcceCombine.Hide()

			self.wndAcceAbsorption = uiAcce.AbsorbWindow()
			self.wndAcceAbsorption.LoadWindow()
			if app.WJ_ENABLE_TRADABLE_ICON:
				self.wndAcceAbsorption.BindInterface(self)
				if self.wndInventory:
					self.wndAcceAbsorption.SetInven(self.wndInventory)
				if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
					if self.wndSpecialInventory:
						self.wndAcceAbsorption.SetSpecialInven(self.wndSpecialInventory)

			self.wndAcceAbsorption.Hide()

			if self.wndInventory:
				self.wndInventory.SetAcceWindow(self.wndAcceCombine, self.wndAcceAbsorption)

	def __MakeCubeWindow(self):
		self.wndCube = uiCube.CubeWindow()
		self.wndCube.LoadWindow()
		self.wndCube.Hide()

	def __MakeCubeResultWindow(self):
		self.wndCubeResult = uiCube.CubeResultWindow()
		self.wndCubeResult.LoadWindow()
		self.wndCubeResult.Hide()

	if app.ENABLE_MINI_GAME_OKEY:
		def __MakeCardsInfoWindow(self):
			self.wndCardsInfo = uiMiniGameRumi.CardsInfoWindow()
			self.wndCardsInfo.LoadWindow()
			self.wndCardsInfo.Hide()

		def __MakeCardsWindow(self):
			self.wndCards = uiMiniGameRumi.CardsWindow()
			self.wndCards.LoadWindow()
			self.wndCards.Hide()

		def __MakeCardsIconWindow(self):
			self.wndCardsIcon = uiMiniGameRumi.IngameWindow()
			self.wndCardsIcon.LoadWindow()
			self.wndCardsIcon.Hide()

	# ACCESSORY_REFINE_ADD_METIN_STONE
	def __MakeItemSelectWindow(self):
		self.wndItemSelect = uiselectitem.SelectItemWindow()
		self.wndItemSelect.Hide()

	if app.ENABLE_GEM_SYSTEM:
		def __MakeItemSelectWindowEx(self):
			self.wndItemSelectEx = uiselectitemEx.SelectItemWindowEx()
			self.wndItemSelectEx.Hide()

		def __MakeGemGui(self):
			self.wndGemG = uiGemShop.SelectGems()
			self.wndGemG.LoadWindow()
			self.wndGemG.Hide()

			self.wndGemM = uiGemShop.SelectGemsShop()
			self.wndGemM.LoadWindow()
			self.wndGemM.Hide()

	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

	def MakeInterface(self):
		self.__MakeTipBoard() # ENABLE_12ZI 미션 표시가 다른ui 밑에 오도록 위치 이동.
		self.__MakeMessengerWindow()
		self.__MakeGuildWindow()
		self.__MakeChatWindow()
		self.__MakeParty()
		self.__MakeWindows()
		self.__MakeDialogs()
		if app.ENABLE_GEM_SYSTEM:
			self.__MakeGemGui()

		self.__MakeUICurtain()
		self.__MakeTaskBar()
		self.__MakeGameButtonWindow()
		self.__MakeHelpWindow()
		self.__MakeWebWindow()
		if app.ENABLE_ACCE_COSTUME_SYSTEM:
			self.__MakeAcceWindow()
		if app.ENABLE_GUILD_DRAGONLAIR_SYSTEM:
			self.__MakeGuildDragonLairRanking()
		self.__MakeCubeWindow()
		self.__MakeCubeResultWindow()

		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.__MakeItemSelectWindow()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

		if app.ENABLE_MINI_GAME_OKEY:
			self.__MakeCardsInfoWindow()
			self.__MakeCardsWindow()
			self.__MakeCardsIconWindow()

		if app.ENABLE_GEM_SYSTEM:
			self.__MakeItemSelectWindowEx()

		self.questButtonList = []
		self.whisperButtonList = []
		self.whisperDialogDict = {}
		self.privateShopAdvertisementBoardDict = {}

		self.wndInventory.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.SetItemToolTip(self.tooltipItem)
			self.wndDragonSoulRefine.SetItemToolTip(self.tooltipItem)
		self.wndSafebox.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_ACCE_COSTUME_SYSTEM:
			self.wndAcceCombine.SetItemToolTip(self.tooltipItem)
			self.wndAcceAbsorption.SetItemToolTip(self.tooltipItem)
		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			self.wndPrivateShopSearch.SetItemToolTip(self.tooltipItem)
		self.wndCube.SetItemToolTip(self.tooltipItem)
		self.wndCubeResult.SetItemToolTip(self.tooltipItem)

		# MT-818 [GF] 아이템 - 제조 성공된 아이템이 펫먹이가 되는 이슈
		self.wndCube.SetInven(self.wndInventory)
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndCube.SetSpecialInven(self.wndSpecialInventory)

		# ITEM_MALL
		self.wndMall.SetItemToolTip(self.tooltipItem)
		# END_OF_ITEM_MALL

		if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
			self.wndSkillBookCombination.SetInven(self.wndInventory)
			if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
				self.wndSkillBookCombination.SetSpecialInven(self.wndSpecialInventory)

		self.wndCharacter.SetSkillToolTip(self.tooltipSkill)
		self.wndTaskBar.SetItemToolTip(self.tooltipItem)
		self.wndTaskBar.SetSkillToolTip(self.tooltipSkill)
		self.wndGuild.SetSkillToolTip(self.tooltipSkill)

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			self.wndChangeLook.SetItemToolTip(self.tooltipItem)

		# ACCESSORY_REFINE_ADD_METIN_STONE
		self.wndItemSelect.SetItemToolTip(self.tooltipItem)
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE
		if app.ENABLE_GEM_SYSTEM:
			self.wndItemSelectEx.SetItemToolTip(self.tooltipItem)

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndSpecialInventory.SetItemToolTip(self.tooltipItem)

		self.dlgShop.SetItemToolTip(self.tooltipItem)
		self.dlgPrivateShop.SetItemToolTip(self.tooltipItem)

		self.dlgExchange.SetItemToolTip(self.tooltipItem)
		self.privateShopBuilder.SetItemToolTip(self.tooltipItem)
		if app.WJ_ENABLE_TRADABLE_ICON:
			self.privateShopBuilder.SetInven(self.wndInventory)
			self.dlgExchange.SetInven(self.wndInventory)
			if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
				self.privateShopBuilder.SetSpecialInven(self.wndSpecialInventory)
				self.dlgExchange.SetSpecialInven(self.wndSpecialInventory)

		self.__InitWhisper()
		self.DRAGON_SOUL_IS_QUALIFIED = False

		#self.IntegrationEventBanner()

		if app.ENABLE_GEM_SYSTEM:
			self.wndGemM.SetItemToolTip(self.tooltipItem)

		if app.ENABLE_MAILBOX:
			if self.mail_box:
				self.mail_box.SetItemToolTip(self.tooltipItem)

	def MakeHyperlinkTooltip(self, hyperlink):
		tokens = hyperlink.split(":")
		if tokens and len(tokens):
			type = tokens[0]
			if "item" == type:
				self.hyperlinkItemTooltip.SetHyperlinkItem(tokens)

	## Make Windows & Dialogs
	################################

	def Close(self):
		if self.dlgWhisperWithoutTarget:
			self.dlgWhisperWithoutTarget.Destroy()
			del self.dlgWhisperWithoutTarget

		if uiQuest.QuestDialog.__dict__.has_key("QuestCurtain"):
			uiQuest.QuestDialog.QuestCurtain.Close()

		if self.wndQuestWindow:
			for key, eachQuestWindow in self.wndQuestWindow.items():
				eachQuestWindow.nextCurtainMode = -1
				eachQuestWindow.Hide()
				eachQuestWindow.CloseSelf()
				eachQuestWindow = None
		self.wndQuestWindow = {}

		if self.wndChat:
			self.wndChat.Destroy()

		if self.wndTaskBar:
			self.wndTaskBar.Destroy()

		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Destroy()

		if app.ENABLE_GEM_SYSTEM:
			if self.wndExpandedMoneyTaskBar:
				self.wndExpandedMoneyTaskBar.Destroy()

		if self.wndEnergyBar:
			self.wndEnergyBar.Destroy()

		if self.wndCharacter:
			if app.ENABLE_DETAILS_UI:
				self.wndCharacter.Hide()
			self.wndCharacter.Destroy()

		if self.wndInventory:
			self.wndInventory.Hide()
			self.wndInventory.Destroy()

		if self.wndDragonSoul:
			self.wndDragonSoul.Destroy()

		if self.wndDragonSoulRefine:
			self.wndDragonSoulRefine.Destroy()

		if self.dlgExchange:
			self.dlgExchange.Destroy()

		if self.dlgPointReset:
			self.dlgPointReset.Destroy()

		if self.dlgShop:
			self.dlgShop.Destroy()

		if self.dlgPrivateShop:
			self.dlgPrivateShop.Destroy()

		if self.dlgRestart:
			self.dlgRestart.Destroy()

		if self.dlgSystem:
			self.dlgSystem.Destroy()

		if self.dlgPassword:
			self.dlgPassword.Destroy()

		if self.wndMiniMap:
			self.wndMiniMap.Destroy()

		if self.wndSafebox:
			self.wndSafebox.Destroy()

		if self.wndWeb:
			self.wndWeb.Destroy()
			self.wndWeb = None

		if self.wndMall:
			self.wndMall.Destroy()

		if self.wndParty:
			self.wndParty.Destroy()

		if self.wndHelp:
			self.wndHelp.Destroy()

		if app.ENABLE_ACCE_COSTUME_SYSTEM:
			if self.wndAcceCombine:
				self.wndAcceCombine.Destroy()

			if self.wndAcceAbsorption:
				self.wndAcceAbsorption.Destroy()

		if app.ENABLE_GUILD_DRAGONLAIR_SYSTEM:
			if self.wndGuildDragonLairRanking:
				self.wndGuildDragonLairRanking.Destroy()

		if self.wndCube:
			self.wndCube.Destroy()

		if self.wndCubeResult:
			self.wndCubeResult.Destroy()

		if self.wndMessenger:
			self.wndMessenger.Destroy()

		if self.wndGuild:
			self.wndGuild.Destroy()

		if self.privateShopBuilder:
			self.privateShopBuilder.Destroy()

		if self.dlgRefineNew:
			self.dlgRefineNew.Destroy()

		if self.wndGuildBuilding:
			self.wndGuildBuilding.Destroy()

		if self.wndGameButton:
			self.wndGameButton.Destroy()

		# ITEM_MALL
		if self.mallPageDlg:
			self.mallPageDlg.Destroy()
		# END_OF_ITEM_MALL

		# ACCESSORY_REFINE_ADD_METIN_STONE
		if self.wndItemSelect:
			self.wndItemSelect.Destroy()
		# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

		if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
			self.wndSkillBookCombination.Destroy()
			del self.wndSkillBookCombination

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			if self.wndChangeLook:
				del self.wndChangeLook

		if app.ENABLE_MYSHOP_DECO:
			if self.wndMyShopDeco:
				self.wndMyShopDeco.Destroy()
				del self.wndMyShopDeco

		if app.ENABLE_MINI_GAME_OKEY:
			if self.wndCardsInfo:
				self.wndCardsInfo.Destroy()

			if self.wndCards:
				self.wndCards.Destroy()

			if self.wndCardsIcon:
				self.wndCardsIcon.Destroy()

		if app.ENABLE_GEM_SYSTEM:
			if self.wndItemSelectEx:
				self.wndItemSelectEx.Destroy()

		if app.ENABLE_GEM_SYSTEM:
			if self.wndGemG:
				self.wndGemG.Destroy()
				del self.wndGemG

			if self.wndGemM:
				self.wndGemM.Destroy()
				del self.wndGemM

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if self.wndSpecialInventory:
				self.wndSpecialInventory.Destroy()

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if self.wndDungeonInfo:
				self.wndDungeonInfo.Destroy()

		if app.ENABLE_CAPTCHA_SYSTEM:
			if self.wndCaptcha:
				self.wndCaptcha.Destroy()

		if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
			if self.wndPrivateShopSearch:
				self.wndPrivateShopSearch.Destroy()
				del self.wndPrivateShopSearch

		self.wndChatLog.Destroy()
		for btn in self.questButtonList:
			btn.SetEvent(0)
		for btn in self.whisperButtonList:
			btn.SetEvent(0)
		for dlg in self.whisperDialogDict.itervalues():
			dlg.Destroy()
		for brd in self.guildScoreBoardDict.itervalues():
			brd.Destroy()
		for dlg in self.equipmentDialogDict.itervalues():
			dlg.Destroy()

		# ITEM_MALL
		del self.mallPageDlg
		# END_OF_ITEM_MALL

		del self.wndGuild
		del self.wndMessenger
		del self.wndUICurtain
		del self.wndChat
		del self.wndTaskBar
		if self.wndExpandedTaskBar:
			del self.wndExpandedTaskBar

		if app.ENABLE_GEM_SYSTEM:
			if self.wndExpandedMoneyTaskBar:
				del self.wndExpandedMoneyTaskBar

		del self.wndEnergyBar
		del self.wndCharacter
		del self.wndInventory
		if self.wndDragonSoul:
			del self.wndDragonSoul
		if self.wndDragonSoulRefine:
			del self.wndDragonSoulRefine

		if app.ENABLE_MAILBOX:
			if self.mail_box:
				self.mail_box.Destroy()
				del self.mail_box

		del self.dlgExchange
		del self.dlgPointReset
		del self.dlgShop
		del self.dlgRestart
		del self.dlgSystem
		del self.dlgPassword
		del self.hyperlinkItemTooltip
		del self.tooltipItem
		del self.tooltipSkill
		del self.wndMiniMap
		del self.wndSafebox
		del self.wndMall
		del self.wndParty
		del self.wndHelp
		if app.ENABLE_ACCE_COSTUME_SYSTEM:
			del self.wndAcceCombine
			del self.wndAcceAbsorption
		if app.ENABLE_GUILD_DRAGONLAIR_SYSTEM:
			del self.wndGuildDragonLairRanking
		del self.wndCube
		del self.wndCubeResult
		del self.privateShopBuilder
		del self.inputDialog
		del self.wndChatLog
		del self.dlgRefineNew
		del self.wndGuildBuilding
		del self.wndGameButton
		del self.tipBoard
		del self.bigBoard
		del self.wndItemSelect

		if self.wndMiniGame:
			self.wndMiniGame.Destroy()
			del self.wndMiniGame

		del self.dlgPrivateShop

		if app.ENABLE_MINI_GAME_OKEY:
			del self.wndCardsInfo
			del self.wndCards
			del self.wndCardsIcon

		if app.ENABLE_GEM_SYSTEM:
			del self.wndItemSelectEx

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if self.wndSpecialInventory:
				del self.wndSpecialInventory

		if app.ENABLE_12ZI:
			del self.missionBoard

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if self.wndDungeonInfo:
				del self.wndDungeonInfo

		if app.ENABLE_CAPTCHA_SYSTEM:
			if self.wndCaptcha:
				del self.wndCaptcha

		self.questButtonList = []
		self.whisperButtonList = []
		self.whisperDialogDict = {}
		self.privateShopAdvertisementBoardDict = {}
		self.guildScoreBoardDict = {}
		self.equipmentDialogDict = {}

		uiChat.DestroyChatInputSetWindow()

	## Skill
	def OnUseSkill(self, slotIndex, coolTime):
		self.wndCharacter.OnUseSkill(slotIndex, coolTime)
		self.wndTaskBar.OnUseSkill(slotIndex, coolTime)
		self.wndGuild.OnUseSkill(slotIndex, coolTime)

	def OnActivateSkill(self, slotIndex):
		self.wndCharacter.OnActivateSkill(slotIndex)
		self.wndTaskBar.OnActivateSkill(slotIndex)

	def OnDeactivateSkill(self, slotIndex):
		self.wndCharacter.OnDeactivateSkill(slotIndex)
		self.wndTaskBar.OnDeactivateSkill(slotIndex)

	def OnChangeCurrentSkill(self, skillSlotNumber):
		self.wndTaskBar.OnChangeCurrentSkill(skillSlotNumber)

	if app.ENABLE_SKILL_COOLTIME_UPDATE:
		def SkillClearCoolTime(self, slotIndex):
			self.wndCharacter.SkillClearCoolTime(slotIndex)
			self.wndTaskBar.SkillClearCoolTime(slotIndex)

	def SelectMouseButtonEvent(self, dir, event):
		self.wndTaskBar.SelectMouseButtonEvent(dir, event)

	## Refresh
	def RefreshAlignment(self):
		self.wndCharacter.RefreshAlignment()

	def RefreshStatus(self):
		self.wndTaskBar.RefreshStatus()
		self.wndCharacter.RefreshStatus()
		self.wndInventory.RefreshStatus()
		if self.wndEnergyBar:
			self.wndEnergyBar.RefreshStatus()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.RefreshStatus()

	def RefreshStamina(self):
		self.wndTaskBar.RefreshStamina()

	def RefreshSkill(self):
		self.wndCharacter.RefreshSkill()
		self.wndTaskBar.RefreshSkill()

	def RefreshInventory(self):
		self.wndTaskBar.RefreshQuickSlot()
		self.wndInventory.RefreshItemSlot()
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.RefreshItemSlot()
		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			self.wndSpecialInventory.RefreshItemSlot()

		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			if player.GetChangeLookWindowOpen() == 1:
				self.wndChangeLook.RefreshChangeLookWindow()

	def RefreshCharacter(self): ## Character 페이지의 얼굴, Inventory 페이지의 전신 그림 등의 Refresh
		self.wndCharacter.RefreshCharacter()
		self.wndTaskBar.RefreshQuickSlot()

	def RefreshQuest(self):
		self.wndCharacter.RefreshQuest()

	def RefreshSafebox(self):
		self.wndSafebox.RefreshSafebox()

	# ITEM_MALL
	def RefreshMall(self):
		self.wndMall.RefreshMall()

	def OpenItemMall(self):
		if not self.mallPageDlg:
			self.mallPageDlg = uiShop.MallPageDialog()

		self.mallPageDlg.Open()
	# END_OF_ITEM_MALL

	if app.ENABLE_CHATTING_WINDOW_RENEWAL:
		def RefreshWhisperMessengerButtons(self):
			if self.whisperDialogDict:
				for dlg in self.whisperDialogDict.itervalues():
					dlg.RefreshMessengerButtons()

	def RefreshMessenger(self):
		self.wndMessenger.RefreshMessenger()

	def RefreshGuildInfoPage(self):
		self.wndGuild.RefreshGuildInfoPage()

	def RefreshGuildBoardPage(self):
		self.wndGuild.RefreshGuildBoardPage()

	def RefreshGuildMemberPage(self):
		self.wndGuild.RefreshGuildMemberPage()

	def RefreshGuildMemberPageGradeComboBox(self):
		self.wndGuild.RefreshGuildMemberPageGradeComboBox()

	def RefreshGuildSkillPage(self):
		self.wndGuild.RefreshGuildSkillPage()

	def RefreshGuildGradePage(self):
		self.wndGuild.RefreshGuildGradePage()

	def DeleteGuild(self):
		self.wndMessenger.ClearGuildMember()
		self.wndGuild.DeleteGuild()

	def OnBlockMode(self, mode):
		self.dlgSystem.OnBlockMode(mode)

	## Calling Functions
	# PointReset
	def OpenPointResetDialog(self):
		self.dlgPointReset.Show()
		self.dlgPointReset.SetTop()

	def ClosePointResetDialog(self):
		self.dlgPointReset.Close()

	# Shop
	def OpenShopDialog(self, vid):
		self.wndInventory.Show()
		self.wndInventory.SetTop()

		import chr
		if chr.IsNPC(vid):
			self.dlgShop.Open(vid)
			self.dlgShop.SetTop()
		else:
			self.dlgPrivateShop.Open(vid)
			self.dlgPrivateShop.SetTop()

		# self.dlgShop.Open(vid)
		# self.dlgShop.SetTop()
		# event.SetInterfaceWindow(self)

	def CloseShopDialog(self):
		# self.dlgShop.Close()
		if self.dlgPrivateShop.IsShow():
			self.dlgPrivateShop.Close()
		elif self.dlgShop.IsShow():
			self.dlgShop.Close()
		else:
			return

	def RefreshShopDialog(self):
		# self.dlgShop.Refresh()
		if self.dlgPrivateShop.IsShow():
			self.dlgPrivateShop.Refresh()
		elif self.dlgShop.IsShow():
			self.dlgShop.Refresh()
		else:
			return

	## Quest
	def OpenCharacterWindowQuestPage(self):
		self.wndCharacter.Show()
		self.wndCharacter.SetState("QUEST")

	def OpenQuestWindow(self, skin, idx):
		wnds = ()

		q = uiQuest.QuestDialog(skin, idx)
		q.SetWindowName("QuestWindow" + str(idx))
		q.Show()
		if skin:
			q.Lock()
			wnds = self.__HideWindows()

			# UNKNOWN_UPDATE
			q.AddOnDoneEvent(lambda tmp_self, args=wnds: self.__ShowWindows(args))
			# END_OF_UNKNOWN_UPDATE

		if skin:
			q.AddOnCloseEvent(q.Unlock)
		q.AddOnCloseEvent(lambda key = self.wndQuestWindowNewKey:ui.__mem_func__(self.RemoveQuestDialog)(key))
		self.wndQuestWindow[self.wndQuestWindowNewKey] = q

		self.wndQuestWindowNewKey = self.wndQuestWindowNewKey + 1

		# END_OF_UNKNOWN_UPDATE

	def RemoveQuestDialog(self, key):
		del self.wndQuestWindow[key]

	## Exchange
	def StartExchange(self):
		self.dlgExchange.OpenDialog()
		self.dlgExchange.Refresh()

	def EndExchange(self):
		self.dlgExchange.CloseDialog()

	def RefreshExchange(self):
		self.dlgExchange.Refresh()

	if app.WJ_ENABLE_TRADABLE_ICON:
		def AddExchangeItemSlotIndex(self, idx):
			self.dlgExchange.AddExchangeItemSlotIndex(idx)

	## Party
	if app.WJ_SHOW_PARTY_ON_MINIMAP:
		def AddPartyMember(self, pid, name, mapIdx, channel):
			self.wndParty.AddPartyMember(pid, name, mapIdx, channel)
			self.__ArrangeQuestButton()
	else:
		def AddPartyMember(self, pid, name):
			self.wndParty.AddPartyMember(pid, name)
			self.__ArrangeQuestButton()

	def UpdatePartyMemberInfo(self, pid):
		self.wndParty.UpdatePartyMemberInfo(pid)

	def RemovePartyMember(self, pid):
		self.wndParty.RemovePartyMember(pid)

		##!! 20061026.levites.퀘스트_위치_보정
		self.__ArrangeQuestButton()

	if app.WJ_SHOW_PARTY_ON_MINIMAP:
		def LinkPartyMember(self, pid, vid, mapIdx, channel):
			self.wndParty.LinkPartyMember(pid, vid, mapIdx, channel)
	else:
		def LinkPartyMember(self, pid, vid):
			self.wndParty.LinkPartyMember(pid, vid)

	def UnlinkPartyMember(self, pid):
		self.wndParty.UnlinkPartyMember(pid)

	def UnlinkAllPartyMember(self):
		self.wndParty.UnlinkAllPartyMember()

	def ExitParty(self):
		self.wndParty.ExitParty()

		##!! 20061026.levites.퀘스트_위치_보정
		self.__ArrangeQuestButton()

	def PartyHealReady(self):
		self.wndParty.PartyHealReady()

	def ChangePartyParameter(self, distributionMode):
		self.wndParty.ChangePartyParameter(distributionMode)

	## Safebox
	def AskSafeboxPassword(self):
		if self.wndSafebox.IsShow():
			return

		# SAFEBOX_PASSWORD
		self.dlgPassword.SetTitle(localeInfo.PASSWORD_TITLE)
		self.dlgPassword.SetSendMessage("/safebox_password ")
		# END_OF_SAFEBOX_PASSWORD

		self.dlgPassword.ShowDialog()

	def OpenSafeboxWindow(self, size):
		self.dlgPassword.CloseDialog()
		self.wndSafebox.ShowWindow(size)

	def RefreshSafeboxMoney(self):
		self.wndSafebox.RefreshSafeboxMoney()

	def CommandCloseSafebox(self):
		self.wndSafebox.CommandCloseSafebox()

	if app.ENABLE_PRIVATESHOP_SEARCH_SYSTEM:
		def OpenPShopSearchDialogCash(self):
			self.wndPrivateShopSearch.Open(1)
		def RefreshPShopSearchDialog(self):
			self.wndPrivateShopSearch.RefreshList()

	## HilightSlot Change
	def DeactivateSlot(self, slotindex, type):
		self.wndInventory.DeactivateSlot(slotindex, type)

	## HilightSlot Change
	def ActivateSlot(self, slotindex, type):
		self.wndInventory.ActivateSlot(slotindex, type)

	if app.ENABLE_CHANGE_LOOK_SYSTEM:
		def ChangeWindowOpen(self, type):
			if self.wndChangeLook:
				if not shop.GetNameDialogOpen():
					self.wndChangeLook.Open(type)

				if self.dlgRefineNew:
					if self.dlgRefineNew.IsShow:
						self.wndChangeLook.Close()

	if app.ENABLE_GEM_SYSTEM:
		def OpenGuiGem(self):
			self.wndGemG.Open()

		def GemCheck(self):
			self.wndGemG.SucceedGem()

		def OpenGuiGemMarket(self):
			self.wndGemM.Open()

		def GemMarketItems(self, vnums, gem, count):
			self.wndGemM.Information(vnums, gem, count)
			self.wndGemM.LoadInformation()

		def GemMarketSlotsDesblock(self, slot0, slot1, slot2, slot3, slot4, slot5):
			self.wndGemM.SlotsDesblock(slot0, slot1, slot2, slot3, slot4, slot5)

		def GemMarketClear(self):
			self.wndGemM.Clear()

		def GemTime(self,time):
			self.wndGemM.Time(time)

	if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
		def OpenSkillbookCombinationDialog(self):
			if self.wndSkillBookCombination.IsShow():
				return

			if self.privateShopBuilder.IsShow():
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_NOTICE_NOT_OPEN)
				return

			self.wndSkillBookCombination.Open()
			self.wndSkillBookCombination.Show()

			if not self.wndInventory.IsShow():
				self.wndInventory.Show()

	# ITEM_MALL
	def AskMallPassword(self):
		if self.wndMall.IsShow():
			return

		self.dlgPassword.SetTitle(localeInfo.MALL_PASSWORD_TITLE)
		self.dlgPassword.SetSendMessage("/mall_password ")
		self.dlgPassword.ShowDialog()

	def OpenMallWindow(self, size):
		self.dlgPassword.CloseDialog()
		self.wndMall.ShowWindow(size)

	def CommandCloseMall(self):
		self.wndMall.CommandCloseMall()
	# END_OF_ITEM_MALL

	## Guild
	def OnStartGuildWar(self, guildSelf, guildOpp):
		self.wndGuild.OnStartGuildWar(guildSelf, guildOpp)

		guildWarScoreBoard = uiGuild.GuildWarScoreBoard()
		guildWarScoreBoard.Open(guildSelf, guildOpp)
		guildWarScoreBoard.Show()
		self.guildScoreBoardDict[uiGuild.GetGVGKey(guildSelf, guildOpp)] = guildWarScoreBoard

	def OnEndGuildWar(self, guildSelf, guildOpp):
		self.wndGuild.OnEndGuildWar(guildSelf, guildOpp)

		key = uiGuild.GetGVGKey(guildSelf, guildOpp)

		if not self.guildScoreBoardDict.has_key(key):
			return

		self.guildScoreBoardDict[key].Destroy()
		del self.guildScoreBoardDict[key]

	# GUILDWAR_MEMBER_COUNT
	def UpdateMemberCount(self, gulidID1, memberCount1, guildID2, memberCount2):
		key = uiGuild.GetGVGKey(gulidID1, guildID2)

		if not self.guildScoreBoardDict.has_key(key):
			return

		self.guildScoreBoardDict[key].UpdateMemberCount(gulidID1, memberCount1, guildID2, memberCount2)
	# END_OF_GUILDWAR_MEMBER_COUNT

	def OnRecvGuildWarPoint(self, gainGuildID, opponentGuildID, point):
		key = uiGuild.GetGVGKey(gainGuildID, opponentGuildID)
		if not self.guildScoreBoardDict.has_key(key):
			return

		guildBoard = self.guildScoreBoardDict[key]
		guildBoard.SetScore(gainGuildID, opponentGuildID, point)

	## PK Mode
	def OnChangePKMode(self):
		self.wndCharacter.RefreshAlignment()
		self.dlgSystem.OnChangePKMode()

	## Refine
	def OpenRefineDialog(self, targetItemPos, nextGradeItemVnum, cost, prob, type, apply_random_list, src_vnum):
		self.dlgRefineNew.Open(targetItemPos, nextGradeItemVnum, cost, prob, type, apply_random_list, src_vnum)

	if app.ENABLE_AUTO_REFINE:
		def RefineFail(self, isFail):
			self.dlgRefineNew.CheckRefine(isFail)

	def AppendMaterialToRefineDialog(self, vnum, count):
		self.dlgRefineNew.AppendMaterial(vnum, count)

	def SetAffectShower(self, shower):
		#from _weakref import proxy
		#self.uiAffectshower = proxy(shower)
		self.uiAffectshower = shower

	def SettargetBoard(self, targetBoard):
		#from _weakref import proxy
		#self.uitargetBoard = proxy(targetBoard)
		self.uitargetBoard = targetBoard

	if app.ENABLE_SHIP_DEFENSE:
		def SetAllianceTargetBoard(self, targetBoard):
			#from _weakref import proxy
			#self.uitargetBoard = proxy(targetBoard)
			self.uiAllianceTargetBoard = targetBoard

	## Show & Hide
	def ShowDefaultWindows(self):
		self.wndTaskBar.Show()
		self.wndMiniMap.Show()
		self.wndMiniMap.ShowMiniMap()
		if self.wndEnergyBar:
			self.wndEnergyBar.Show()
		if self.wndGameButton:
			self.wndGameButton.Show()

		if self.uiAffectshower:
			self.uiAffectshower.Show()

		if self.wndParty:
			self.wndParty.Show()

		if app.ENABLE_CAPTCHA_SYSTEM:
			if self.wndChat:
				self.wndChat.Show()

		self.IsHideUiMode = False

	def IsHideUiMode(self):
		return self.IsHideUiMode

	def ShowAllWindows(self):
		self.wndTaskBar.Show()
		self.wndCharacter.Show()
		self.wndInventory.Show()

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Show()
			self.wndDragonSoulRefine.Show()

		self.wndChat.Show()
		self.wndMiniMap.Show()

		if self.wndEnergyBar:
			self.wndEnergyBar.Show()

		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Show()
			self.wndExpandedTaskBar.SetTop()

		if app.ENABLE_GEM_SYSTEM:
			if self.wndExpandedMoneyTaskBar:
				self.wndExpandedMoneyTaskBar.Show()
				self.wndExpandedMoneyTaskBar.SetTop()

	def HideAllWindows(self):
		if self.wndParty:
			self.wndParty.Hide()

		if self.uiAffectshower:
			self.uiAffectshower.Hide()

		if self.uitargetBoard:
			self.uitargetBoard.Hide()

		if app.ENABLE_SHIP_DEFENSE:
			if self.uiAllianceTargetBoard:
				self.uiAllianceTargetBoard.Hide()

		if self.wndMiniGame:
			self.wndMiniGame.hide_mini_game_dialog()

		if self.wndTaskBar:
			self.wndTaskBar.Hide()

		if self.wndGameButton:
			self.wndGameButton.Hide()

		if self.wndEnergyBar:
			self.wndEnergyBar.Hide()

		if app.ENABLE_DETAILS_UI or app.ENABLE_SKILL_COLOR_SYSTEM:
			if self.wndCharacter:
				self.wndCharacter.Close()
		else:
			if self.wndCharacter:
				self.wndCharacter.Hide()

		if self.wndInventory:
			self.wndInventory.Hide()

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.Hide()
			self.wndDragonSoulRefine.Hide()

		if self.wndChat:
			self.wndChat.hide_btnChatSizing()
			self.wndChat.Hide()

		if self.wndMiniMap:
			self.wndMiniMap.Hide()

		if self.wndMessenger:
			self.wndMessenger.Hide()

		if self.wndGuild:
			self.wndGuild.Hide()

		if self.wndExpandedTaskBar:
			self.wndExpandedTaskBar.Hide()

		if app.ENABLE_GEM_SYSTEM:
			if self.wndExpandedMoneyTaskBar:
				self.wndExpandedMoneyTaskBar.Hide()

		if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
			if self.wndSkillBookCombination:
				self.wndSkillBookCombination.Hide()

		if app.ENABLE_MYSHOP_DECO:
			if self.wndMyShopDeco:
				self.wndMyShopDeco.Hide()

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if self.wndSpecialInventory:
				self.wndSpecialInventory.Hide()

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if self.wndDungeonInfo:
				self.wndDungeonInfo.Hide()

		if app.ENABLE_MAILBOX:
			if self.mail_box:
				self.mail_box.Hide()

		self.IsHideUiMode = True

	def ShowMouseImage(self):
		self.wndTaskBar.ShowMouseImage()

	def HideMouseImage(self):
		self.wndTaskBar.HideMouseImage()

	def ToggleChat(self):
		if True == self.wndChat.IsEditMode():
			self.wndChat.CloseChat()
		else:
			# 웹페이지가 열렸을때는 채팅 입력이 안됨
			if self.wndWeb and self.wndWeb.IsShow():
				pass
			else:
				self.wndChat.OpenChat()

	def IsOpenChat(self):
		return self.wndChat.IsEditMode()

	def SetChatFocus(self):
		self.wndChat.SetChatFocus()

	def OpenRestartDialog(self):
		self.dlgRestart.OpenDialog()
		self.dlgRestart.SetTop()

	def CloseRestartDialog(self):
		self.dlgRestart.Close()

	def ToggleSystemDialog(self):
		if False == self.dlgSystem.IsShow():
			self.dlgSystem.OpenDialog()
			self.dlgSystem.SetTop()
		else:
			self.dlgSystem.Close()

	def OpenSystemDialog(self):
		self.dlgSystem.OpenDialog()
		self.dlgSystem.SetTop()

	def ToggleMessenger(self):
		if self.wndMessenger.IsShow():
			self.wndMessenger.Hide()
		else:
			self.wndMessenger.SetTop()
			self.wndMessenger.Show()

	def ToggleMiniMap(self):
		if app.IsPressed(app.DIK_LSHIFT) or app.IsPressed(app.DIK_RSHIFT):
			if False == self.wndMiniMap.isShowMiniMap():
				self.wndMiniMap.ShowMiniMap()
				self.wndMiniMap.SetTop()
			else:
				self.wndMiniMap.HideMiniMap()
		else:
			self.wndMiniMap.ToggleAtlasWindow()

	def PressMKey(self):
		if app.IsPressed(app.DIK_LALT) or app.IsPressed(app.DIK_RALT):
			self.ToggleMessenger()
		else:
			self.ToggleMiniMap()

	def SetMapName(self, mapName):
		self.wndMiniMap.SetMapName(mapName)

	def MiniMapScaleUp(self):
		self.wndMiniMap.ScaleUp()

	def MiniMapScaleDown(self):
		self.wndMiniMap.ScaleDown()

	def ToggleCharacterWindow(self, state):
		if False == player.IsObserverMode():
			if False == self.wndCharacter.IsShow():
				self.OpenCharacterWindowWithState(state)
			else:
				if state == self.wndCharacter.GetState():
					self.wndCharacter.OverOutItem()
					if app.ENABLE_DETAILS_UI or app.ENABLE_SKILL_COLOR_SYSTEM:
						self.wndCharacter.Close()
					else:
						self.wndCharacter.Hide()
				else:
					self.wndCharacter.SetState(state)

	def OpenCharacterWindowWithState(self, state):
		if False == player.IsObserverMode():
			self.wndCharacter.SetState(state)
			self.wndCharacter.Show()
			self.wndCharacter.SetTop()

	def ToggleCharacterWindowStatusPage(self):
		self.ToggleCharacterWindow("STATUS")

	def ToggleInventoryWindow(self):
		if False == player.IsObserverMode():
			if False == self.wndInventory.IsShow():
				self.wndInventory.Show()
				self.wndInventory.SetTop()
			else:
				self.wndInventory.OverOutItem()
				self.wndInventory.Close()

	def ToggleExpandedButton(self):
		if False == player.IsObserverMode():
			if False == self.wndExpandedTaskBar.IsShow():
				self.wndExpandedTaskBar.Show()
				self.wndExpandedTaskBar.SetTop()
			else:
				self.wndExpandedTaskBar.Close()

		if app.ENABLE_GEM_SYSTEM:
			##self.wndExpandedMoneyTaskBar.LoadWindow()
			pass

	if app.ENABLE_GEM_SYSTEM:
		def ToggleExpandedMoneyButton(self):
			if False == self.wndExpandedMoneyTaskBar.IsShow():
				self.wndExpandedMoneyTaskBar.Show()
				self.wndExpandedMoneyTaskBar.SetTop()
			else:
				self.wndExpandedMoneyTaskBar.Close()

	if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
		def ToggleSpecialInventoryWindow(self):
			if False == player.IsObserverMode():
				if False == self.wndSpecialInventory.IsShow():
					self.wndSpecialInventory.Show()
					self.wndSpecialInventory.SetTop()
				else:
					self.wndSpecialInventory.OverOutItem()
					self.wndSpecialInventory.Close()

		def OpenSpecialInventoryWindow(self, category = 0):
			if False == player.IsObserverMode():
				if False == self.wndSpecialInventory.IsShow():
					self.wndSpecialInventory.Show()
					self.wndSpecialInventory.SetTop()
					self.wndSpecialInventory.SetInventoryType(category)

	def OpenCostumeWindow(self):
		self.wndInventory.ShowCostumeInventory()

	# wj.2014.12.2. 인벤토리와 DS인벤 간의 상태를 확인 및 설정하기 위한 함수.
	def IsShowDlgQuestionWindow(self):
		if self.wndInventory.IsDlgQuestionShow():
			return True
		elif self.wndDragonSoul.IsDlgQuestionShow():
			return True
		else:
			return False

	def CloseDlgQuestionWindow(self):
		if self.wndInventory.IsDlgQuestionShow():
			self.wndInventory.CancelDlgQuestion()
		if self.wndDragonSoul.IsDlgQuestionShow():
			self.wndDragonSoul.CancelDlgQuestion()

	def SetUseItemMode(self, bUse):
		self.wndInventory.SetUseItemMode(bUse)
		self.wndDragonSoul.SetUseItemMode(bUse)

	# 용혼석
	def DragonSoulActivate(self, deck):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.ActivateDragonSoulByExtern(deck)

	def DragonSoulDeactivate(self):
		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			self.wndDragonSoul.DeactivateDragonSoul()

	if app.ENABLE_DS_SET:
		def DragonSoulSetGrade(self, grade):
			self.wndDragonSoul.SetDSSetGrade(grade)

	def Highligt_Item(self, inven_type, inven_pos):
		if player.DRAGON_SOUL_INVENTORY == inven_type:
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				self.wndDragonSoul.HighlightSlot(inven_pos)
		elif app.WJ_ENABLE_PICKUP_ITEM_EFFECT and player.INVENTORY == inven_type:
			self.wndInventory.HighlightSlot(inven_pos)
			if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
				self.wndSpecialInventory.HighlightSlot(inven_pos)

	def DragonSoulGiveQuilification(self):
		self.DRAGON_SOUL_IS_QUALIFIED = True
		self.wndExpandedTaskBar.SetToolTipText(uiTaskBar.ExpandedTaskBar.BUTTON_DRAGON_SOUL, uiScriptLocale.TASKBAR_DRAGON_SOUL)

	def ToggleDragonSoulWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoul.IsShow():
					if self.DRAGON_SOUL_IS_QUALIFIED:
						self.wndDragonSoul.Show()
					else:
						try:
							self.wndPopupDialog.SetText(localeInfo.DRAGON_SOUL_UNQUALIFIED)
							self.wndPopupDialog.Open()
						except:
							self.wndPopupDialog = uiCommon.PopupDialog()
							self.wndPopupDialog.SetText(localeInfo.DRAGON_SOUL_UNQUALIFIED)
							self.wndPopupDialog.Open()
				else:
					self.wndDragonSoul.Close()

	def ToggleDragonSoulWindowWithNoInfo(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if False == self.wndDragonSoul.IsShow():
					if self.DRAGON_SOUL_IS_QUALIFIED:
						self.wndDragonSoul.Show()
				else:
					self.wndDragonSoul.Close()

	if app.ENABLE_DUNGEON_INFO_SYSTEM:
		def ToggleDungeonInfoWindow(self):
			if False == player.IsObserverMode():
				if False == self.wndDungeonInfo.IsShow():
					self.wndDungeonInfo.Open()
				else:
					self.wndDungeonInfo.Close()

		def DungeonInfoOpen(self):
			if self.wndDungeonInfo:
				self.wndDungeonInfo.OnOpen()

		def DungeonInfoReload(self, onReset):
			if self.wndDungeonInfo:
				self.wndDungeonInfo.OnReload(onReset)

		def DungeonCooldownInfo(self, key, cooldown):
			if self.wndDungeonInfo:
				self.wndDungeonInfo.SetCooldown(key, cooldown)

	def FailDragonSoulRefine(self, reason, inven_type, inven_pos):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.RefineFail(reason, inven_type, inven_pos)

	def SucceedDragonSoulRefine(self, inven_type, inven_pos):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.RefineSucceed(inven_type, inven_pos)

	if app.ENABLE_DS_CHANGE_ATTR:
		def OpenDragonSoulRefineWindow(self, type):
			if False == player.IsObserverMode():
				if app.ENABLE_DRAGON_SOUL_SYSTEM:
					if False == self.wndDragonSoulRefine.IsShow():
						self.wndDragonSoulRefine.SetWindowType(type)
						self.wndDragonSoulRefine.Show()
						if None != self.wndDragonSoul:
							if False == self.wndDragonSoul.IsShow():
								self.wndDragonSoul.Show()
	else:
		def OpenDragonSoulRefineWindow(self):
			if False == player.IsObserverMode():
				if app.ENABLE_DRAGON_SOUL_SYSTEM:
					if False == self.wndDragonSoulRefine.IsShow():
						self.wndDragonSoulRefine.Show()
						if None != self.wndDragonSoul:
							if False == self.wndDragonSoul.IsShow():
								self.wndDragonSoul.Show()

	def CloseDragonSoulRefineWindow(self):
		if False == player.IsObserverMode():
			if app.ENABLE_DRAGON_SOUL_SYSTEM:
				if True == self.wndDragonSoulRefine.IsShow():
					self.wndDragonSoulRefine.Close()

	# 용혼석 끝

	def ToggleGuildWindow(self):
		if not self.wndGuild.IsShow():
			if self.wndGuild.CanOpen():
				self.wndGuild.Open()
			else:
				chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.GUILD_YOU_DO_NOT_JOIN)
		else:
			self.wndGuild.OverOutItem()
			self.wndGuild.Hide()

	def ToggleChatLogWindow(self):
		if self.wndChatLog.IsShow():
			self.wndChatLog.Hide()
		else:
			self.wndChatLog.Show()

	def CheckGameButton(self):
		if self.wndGameButton:
			self.wndGameButton.CheckGameButton()

	def __OnClickStatusPlusButton(self):
		self.ToggleCharacterWindow("STATUS")

	def __OnClickSkillPlusButton(self):
		self.ToggleCharacterWindow("SKILL")

	def __OnClickQuestButton(self):
		self.ToggleCharacterWindow("QUEST")

	def __OnClickHelpButton(self):
		player.SetPlayTime(1)
		self.CheckGameButton()
		self.OpenHelpWindow()

	def __OnClickBuildButton(self):
		self.BUILD_OpenWindow()

	if app.ENABLE_KEYCHANGE_SYSTEM:
		def ToggleHelpWindow(self):
			if self.wndHelp.IsShow():
				self.CloseHelpWindow()
			else:
				self.OpenHelpWindow()

	def OpenHelpWindow(self):
		self.wndHelp.Open()
		self.wndUICurtain.Show()

	def CloseHelpWindow(self):
		self.wndHelp.Close()
		self.wndUICurtain.Hide()

	def OpenWebWindow(self, url):
		self.wndWeb.Open(url)
		# 웹페이지를 열면 채팅을 닫는다
		self.wndChat.CloseChat()

	if app.ENABLE_GUILD_DRAGONLAIR_SYSTEM:
		def ClearDragonLairRanking(self, type):
			self.wndGuildDragonLairRanking.ClearDragonLairRanking(type)

		def OpenDragonLairRanking(self, type):
			self.wndGuildDragonLairRanking.Open(type)
			self.wndGuildDragonLairRanking.RefreshGuildDragonLairRanking(type)

		def AddDragonLairRanking(self, type, guildname, membercount, min, sec):
			self.wndGuildDragonLairRanking.AddDragonLairRanking(type, guildname, membercount, min, sec)

	if app.ENABLE_MINI_GAME_OKEY:
		def OpenCardsInfoWindow(self):
			self.wndCardsInfo.Open()

		def OpenCardsWindow(self, safemode):
			self.wndCards.Open(safemode)

		def UpdateCardsInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points):
			self.wndCards.UpdateCardsInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, hand_4, hand_4_v, hand_5, hand_5_v, cards_left, points)

		def UpdateCardsFieldInfo(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
			self.wndCards.UpdateCardsFieldInfo(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)

		def CardsPutReward(self, hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points):
			self.wndCards.CardsPutReward(hand_1, hand_1_v, hand_2, hand_2_v, hand_3, hand_3_v, points)

		def CardsShowIcon(self):
			self.wndCardsIcon.Show()

	# Show GIFT
	def ShowGift(self):
		self.wndTaskBar.ShowGift()

	def HideGift(self):
		self.wndTaskBar.HideGift()

	def CloseWbWindow(self):
		self.wndWeb.Close()

	if app.ENABLE_ACCE_COSTUME_SYSTEM:
		def ActAcce(self, iAct, bWindow):
			if iAct == 1:
				if bWindow == True:
					if not self.wndAcceCombine.IsOpened():
						self.wndAcceCombine.Open()

					if not self.wndInventory.IsShow():
						self.wndInventory.Show()
				else:
					if not self.wndAcceAbsorption.IsOpened():
						self.wndAcceAbsorption.Open()

					if not self.wndInventory.IsShow():
						self.wndInventory.Show()

				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 2:
				if bWindow == True:
					if self.wndAcceCombine.IsOpened():
						self.wndAcceCombine.Close()
				else:
					if self.wndAcceAbsorption.IsOpened():
						self.wndAcceAbsorption.Close()

				self.wndInventory.RefreshBagSlotWindow()
			elif iAct == 3 or iAct == 4:
				if bWindow == True:
					if self.wndAcceCombine.IsOpened():
						self.wndAcceCombine.Refresh(iAct)
				else:
					if self.wndAcceAbsorption.IsOpened():
						self.wndAcceAbsorption.Refresh(iAct)

				self.wndInventory.RefreshBagSlotWindow()

	if app.ENABLE_CAPTCHA_SYSTEM:
		def ShowCaptcha(self, timeLeft):
			self.wndCaptcha.Open(timeLeft)

	def OpenCubeWindow(self):
		self.wndCube.Open()

		if False == self.wndInventory.IsShow():
			self.wndInventory.Show()

	def UpdateCubeInfo(self, gold, itemVnum, count):
		self.wndCube.UpdateInfo(gold, itemVnum, count)

	def CloseCubeWindow(self):
		self.wndCube.Close()

	def FailedCubeWork(self):
		self.wndCube.Refresh()

	def SucceedCubeWork(self, itemVnum, count):
		self.wndCube.Clear()

		if 0: # 결과 메시지 출력은 생략 한다
			self.wndCubeResult.SetPosition(*self.wndCube.GetGlobalPosition())
			self.wndCubeResult.SetCubeResultItem(itemVnum, count)
			self.wndCubeResult.Open()
			self.wndCubeResult.SetTop()

	def __HideWindows(self):
		hideWindows = self.wndTaskBar,\
						self.wndCharacter,\
						self.wndInventory,\
						self.wndMiniMap,\
						self.wndGuild,\
						self.wndMessenger,\
						self.wndChat,\
						self.wndParty,\
						self.wndGameButton,

		if self.wndMiniGame:
			hideWindows += self.wndMiniGame,

		if self.dlgSystem:
			hideWindows += self.dlgSystem,

		if self.wndEnergyBar:
			hideWindows += self.wndEnergyBar,

		if self.wndExpandedTaskBar:
			hideWindows += self.wndExpandedTaskBar,

		if app.ENABLE_GEM_SYSTEM:
			if self.wndExpandedMoneyTaskBar:
				hideWindows += self.wndExpandedMoneyTaskBar,

		if app.ENABLE_DRAGON_SOUL_SYSTEM:
			hideWindows += self.wndDragonSoul,\
						self.wndDragonSoulRefine,

		if app.ENABLE_MYSHOP_DECO:
			if self.wndMyShopDeco:
				hideWindows += self.wndMyShopDeco,

		if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
			if self.wndSpecialInventory:
				hideWindows += self.wndSpecialInventory,

		if app.ENABLE_DUNGEON_INFO_SYSTEM:
			if self.wndDungeonInfo:
				hideWindows += self.wndDungeonInfo,

		if app.ENABLE_MAILBOX:
			if self.mail_box:
				hideWindows += self.mail_box,

		hideWindows = filter(lambda x:x.IsShow(), hideWindows)
		map(lambda x:x.Hide(), hideWindows)
		import sys

		self.HideAllQuestButton()
		self.HideAllWhisperButton()

		if self.wndChat.IsEditMode():
			self.wndChat.CloseChat()

		if self.dlgSystem:
			self.dlgSystem.HideAllSystemOptioin()

		self.wndMiniMap.AtlasWindow.Hide()

		if self.wndWeb:
			self.wndWeb.CloseWhenOpenQuest()

		return hideWindows

	def __ShowWindows(self, wnds):
		import sys
		map(lambda x:x.Show(), wnds)
		global IsQBHide
		if not IsQBHide:
			self.ShowAllQuestButton()
		else:
			self.HideAllQuestButton()

		self.ShowAllWhisperButton()

		if self.dlgSystem:
			self.dlgSystem.ShowAllSystemOptioin()

		if self.wndMiniMap.AtlasWindow and self.wndMiniMap.AtlasWindow.IsShowWindow():
			self.wndMiniMap.AtlasWindow.Show()

		if self.wndWeb:
			self.wndWeb.OpenWhenOpenQuest()

	def BINARY_OpenAtlasWindow(self):
		if self.wndMiniMap:
			self.wndMiniMap.ShowAtlas()

	def BINARY_SetObserverMode(self, flag):
		self.wndGameButton.SetObserverMode(flag)

	# ACCESSORY_REFINE_ADD_METIN_STONE
	def BINARY_OpenSelectItemWindow(self):
		self.wndItemSelect.Open()
	# END_OF_ACCESSORY_REFINE_ADD_METIN_STONE

	if app.ENABLE_GEM_SYSTEM:
		def BINARY_OpenSelectItemWindowEx(self):
			self.wndItemSelectEx.Open()

		def BINARY_RefreshSelectItemWindowEx(self):
			self.wndItemSelectEx.RefreshSlot()

	#####################################################################################
	### Private Shop ###

	if app.ENABLE_MYSHOP_DECO:
		def OpenMyShopDecoWnd(self):
			if self.inputDialog:
				return

			if self.privateShopBuilder.IsShow():
				return

			if self.wndMyShopDeco:
				self.wndMyShopDeco.Open()
			else:
				return

	if app.ENABLE_CHEQUE_SYSTEM:
		def OpenPrivateShopInputNameDialog(self, bCashItem, tabCnt):
			if self.wndMyShopDeco.IsShow():
				return

			if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
				if self.wndSkillBookCombination.IsShow():
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_NOTICE)
					return

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				shop.SetNameDialogOpen(True)
			inputDialog = uiCommon.InputDialog()
			inputDialog.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_NAME_DIALOG_TITLE)
			inputDialog.SetMaxLength(32)
			inputDialog.SetUseCodePage(False)
			inputDialog.SetAcceptEvent(lambda arg = bCashItem, arg1 = tabCnt : ui.__mem_func__(self.OpenPrivateShopBuilder)(arg, arg1))
			inputDialog.SetCancelEvent(ui.__mem_func__(self.ClosePrivateShopInputNameDialog))
			inputDialog.Open()
			self.inputDialog = inputDialog
	else:
		def OpenPrivateShopInputNameDialog(self):
			#if player.IsInSafeArea():
			#	chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.CANNOT_OPEN_PRIVATE_SHOP_IN_SAFE_AREA)
			#	return

			if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
				if self.wndSkillBookCombination.IsShow():
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_NOTICE)
					return

			inputDialog = uiCommon.InputDialog()
			inputDialog.SetTitle(localeInfo.PRIVATE_SHOP_INPUT_NAME_DIALOG_TITLE)
			inputDialog.SetMaxLength(32)
			inputDialog.SetAcceptEvent(ui.__mem_func__(self.OpenPrivateShopBuilder))
			inputDialog.SetCancelEvent(ui.__mem_func__(self.ClosePrivateShopInputNameDialog))
			inputDialog.Open()
			self.inputDialog = inputDialog

	def ClosePrivateShopInputNameDialog(self):
		self.inputDialog = None
		if app.ENABLE_CHANGE_LOOK_SYSTEM:
			shop.SetNameDialogOpen(False)
		return True

	if app.ENABLE_CHEQUE_SYSTEM:
		def OpenPrivateShopBuilder(self, bCashItem, tabCnt):
			if not self.inputDialog:
				return True

			if not len(self.inputDialog.GetText()):
				return True

			if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
				if self.wndSkillBookCombination.IsShow():
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_NOTICE)
					return

			self.privateShopBuilder.Open(self.inputDialog.GetText())
			self.privateShopBuilder.SetIsCashItem(bCashItem)
			self.privateShopBuilder.SetTabCount(tabCnt)
			self.ClosePrivateShopInputNameDialog()

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				shop.SetNameDialogOpen(True)

			return True
	else:
		def OpenPrivateShopBuilder(self):
			if not self.inputDialog:
				return True

			if not len(self.inputDialog.GetText()):
				return True

			if app.ENABLE_SKILLBOOK_COMB_SYSTEM:
				if self.wndSkillBookCombination.IsShow():
					chat.AppendChat(chat.CHAT_TYPE_INFO, localeInfo.COMB_NOTICE)
					return

			self.privateShopBuilder.Open(self.inputDialog.GetText())
			self.ClosePrivateShopInputNameDialog()

			if app.ENABLE_CHANGE_LOOK_SYSTEM:
				shop.SetNameDialogOpen(True)

			return True

	def AppearPrivateShop(self, vid, text, type):
		board = None

		if type == 0:
			board = uiPrivateShopBuilder.PrivateShopAdvertisementBoard()
			board.Open(vid, text)
		else:
			board = uiPrivateShopBuilder.PrivateShopTitleBar(type)
			board.Open(vid, text)

		self.privateShopAdvertisementBoardDict[vid] = board

	def DisappearPrivateShop(self, vid):
		if not self.privateShopAdvertisementBoardDict.has_key(vid):
			return

		del self.privateShopAdvertisementBoardDict[vid]
		uiPrivateShopBuilder.DeleteADBoard(vid)

	#####################################################################################
	### Equipment ###

	def OpenEquipmentDialog(self, vid):
		dlg = uiEquipmentDialog.EquipmentDialog()
		dlg.SetItemToolTip(self.tooltipItem)
		dlg.SetCloseEvent(ui.__mem_func__(self.CloseEquipmentDialog))
		dlg.Open(vid)

		self.equipmentDialogDict[vid] = dlg

	if app.ENABLE_CHANGE_LOOK_SYSTEM:
		def SetEquipmentDialogItem(self, vid, slotIndex, vnum, count, dwChangeLookVnum):
			if not vid in self.equipmentDialogDict:
				return
			self.equipmentDialogDict[vid].SetEquipmentDialogItem(slotIndex, vnum, count, dwChangeLookVnum)
	else:
		def SetEquipmentDialogItem(self, vid, slotIndex, vnum, count):
			if not vid in self.equipmentDialogDict:
				return
			self.equipmentDialogDict[vid].SetEquipmentDialogItem(slotIndex, vnum, count)

	def SetEquipmentDialogSocket(self, vid, slotIndex, socketIndex, value):
		if not vid in self.equipmentDialogDict:
			return
		self.equipmentDialogDict[vid].SetEquipmentDialogSocket(slotIndex, socketIndex, value)

	def SetEquipmentDialogAttr(self, vid, slotIndex, attrIndex, type, value):
		if not vid in self.equipmentDialogDict:
			return
		self.equipmentDialogDict[vid].SetEquipmentDialogAttr(slotIndex, attrIndex, type, value)

	def CloseEquipmentDialog(self, vid):
		if not vid in self.equipmentDialogDict:
			return
		del self.equipmentDialogDict[vid]

	#####################################################################################

	#####################################################################################
	### Quest ###

	def BINARY_ClearQuest(self, index):
		btn = self.__FindQuestButton(index)
		if 0 != btn:
			self.__DestroyQuestButton(btn)

	def RecvQuest(self, index, name):
		# QUEST_LETTER_IMAGE
		self.BINARY_RecvQuest(index, name, "file", localeInfo.GetLetterImageName())
		# END_OF_QUEST_LETTER_IMAGE

	def BINARY_RecvQuest(self, index, name, iconType, iconName):
		btn = self.__FindQuestButton(index)
		if 0 != btn:
			self.__DestroyQuestButton(btn)

		btn = uiWhisper.WhisperButton()

		# QUEST_LETTER_IMAGE
		##!! 20061026.levites.퀘스트_이미지_교체
		import item

		if "item" == iconType:
			item.SelectItem(int(iconName))
			buttonImageFileName = item.GetIconImageFileName()
		elif "blue_quest" == iconType or name[0] == "*":
			buttonImageFileName = localeInfo.GetBlueLetterImageName()
		else:
			buttonImageFileName = iconName

		if localeInfo.IsEUROPE():
			if name[0] == "*":
				btn.SetUpVisual(localeInfo.GetBlueLetterImageName())
				btn.SetOverVisual(localeInfo.GetBlueLetterOpenImageName())
				btn.SetDownVisual(localeInfo.GetBlueLetterCloseImageName())
				name = name[1:]
			else:
				if "highlight" == iconType:
					btn.SetUpVisual("locale/ymir_ui/highlighted_quest.tga")
					btn.SetOverVisual("locale/ymir_ui/highlighted_quest_r.tga")
					btn.SetDownVisual("locale/ymir_ui/highlighted_quest_r.tga")
				else:
					btn.SetUpVisual(localeInfo.GetLetterCloseImageName())
					btn.SetOverVisual(localeInfo.GetLetterOpenImageName())
					btn.SetDownVisual(localeInfo.GetLetterOpenImageName())
		else:
			btn.SetUpVisual(buttonImageFileName)
			btn.SetOverVisual(buttonImageFileName)
			btn.SetDownVisual(buttonImageFileName)
			btn.Flash()
		# END_OF_QUEST_LETTER_IMAGE

		if localeInfo.IsARABIC():
			btn.SetToolTipText(name, -20, 35)
			btn.ToolTipText.SetHorizontalAlignRight()
		else:
			btn.SetToolTipText(name, -20, 35)
			btn.ToolTipText.SetHorizontalAlignLeft()

		btn.SetEvent(ui.__mem_func__(self.__StartQuest), btn)
		btn.Show()

		btn.index = index
		btn.name = name

		self.questButtonList.insert(0, btn)
		self.__ArrangeQuestButton()

		#chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.QUEST_APPEND)

	def __ArrangeQuestButton(self):
		screenWidth = wndMgr.GetScreenWidth()
		screenHeight = wndMgr.GetScreenHeight()

		##!! 20061026.levites.퀘스트_위치_보정
		if self.wndParty.IsShow():
			xPos = 100 + 30
		else:
			xPos = 20

		if localeInfo.IsARABIC():
			xPos = xPos + 15

		yPos = 170 * screenHeight / 600
		yCount = (screenHeight - 330) / 63

		count = 0
		for btn in self.questButtonList:
			btn.SetPosition(xPos + (int(count / yCount) * 100), yPos + (count % yCount * 63))
			count += 1

			global IsQBHide
			if IsQBHide:
				btn.Hide()
			else:
				btn.Show()

	def __StartQuest(self, btn):
		event.QuestButtonClick(btn.index)
		self.__DestroyQuestButton(btn)

	def __FindQuestButton(self, index):
		for btn in self.questButtonList:
			if btn.index == index:
				return btn

		return 0

	def __DestroyQuestButton(self, btn):
		btn.SetEvent(0)
		self.questButtonList.remove(btn)
		self.__ArrangeQuestButton()

	def HideAllQuestButton(self):
		for btn in self.questButtonList:
			btn.Hide()

	def ShowAllQuestButton(self):
		for btn in self.questButtonList:
			btn.Show()

	#####################################################################################

	#####################################################################################
	### Whisper ###

	def __InitWhisper(self):
		#chat.InitWhisper(self)
		from _weakref import proxy
		chat.InitWhisper(proxy(self))

	## 채팅창의 "메시지 보내기"를 눌렀을때 이름 없는 대화창을 여는 함수
	## 이름이 없기 때문에 기존의 WhisperDialogDict 와 별도로 관리된다.
	def OpenWhisperDialogWithoutTarget(self):
		if not self.dlgWhisperWithoutTarget:
			dlgWhisper = uiWhisper.WhisperDialog(self.MinimizeWhisperDialog, self.CloseWhisperDialog)
			dlgWhisper.BindInterface(self)
			dlgWhisper.LoadDialog()
			dlgWhisper.OpenWithoutTarget(self.RegisterTemporaryWhisperDialog)
			dlgWhisper.SetPosition(self.windowOpenPosition * 30,self.windowOpenPosition * 30)
			dlgWhisper.Show()
			self.dlgWhisperWithoutTarget = dlgWhisper

			self.windowOpenPosition = (self.windowOpenPosition + 1) % 5

		else:
			self.dlgWhisperWithoutTarget.SetTop()
			self.dlgWhisperWithoutTarget.OpenWithoutTarget(self.RegisterTemporaryWhisperDialog)

	## 이름 없는 대화창에서 이름을 결정했을때 WhisperDialogDict에 창을 넣어주는 함수
	def RegisterTemporaryWhisperDialog(self, name):
		if not self.dlgWhisperWithoutTarget:
			return

		btn = self.__FindWhisperButton(name)
		if 0 != btn:
			self.__DestroyWhisperButton(btn)

		elif self.whisperDialogDict.has_key(name):
			oldDialog = self.whisperDialogDict[name]
			oldDialog.Destroy()
			del self.whisperDialogDict[name]

		self.whisperDialogDict[name] = self.dlgWhisperWithoutTarget
		self.dlgWhisperWithoutTarget.OpenWithTarget(name)
		self.dlgWhisperWithoutTarget = None
		self.__CheckGameMaster(name)

	def RecieveWhisperDetails(self, name, country):
		if self.whisperDialogDict.has_key(name):
			self.whisperDialogDict[name].SetCountryFlag(country)
		else:
			btn = self.__FindWhisperButton(name)
			if btn != 0:
				btn.countryID = country

	## 캐릭터 메뉴의 1:1 대화 하기를 눌렀을때 이름을 가지고 바로 창을 여는 함수
	def OpenWhisperDialog(self, name):
		if not self.whisperDialogDict.has_key(name):
			dlg = self.__MakeWhisperDialog(name)
			dlg.OpenWithTarget(name)
			dlg.chatLine.SetFocus()
			dlg.Show()

			self.__CheckGameMaster(name)
			btn = self.__FindWhisperButton(name)
			if 0 != btn:
				self.__DestroyWhisperButton(btn)

	## 다른 캐릭터로부터 메세지를 받았을때 일단 버튼만 띄워 두는 함수
	def RecvWhisper(self, name):
		if not self.whisperDialogDict.has_key(name):
			btn = self.__FindWhisperButton(name)
			if 0 == btn:
				btn = self.__MakeWhisperButton(name)
				btn.Flash()
				app.FlashApplication() #osp. flash application
				chat.AppendChat(chat.CHAT_TYPE_NOTICE, localeInfo.RECEIVE_MESSAGE % (name))
			else:
				btn.Flash()
		elif self.IsGameMasterName(name):
			dlg = self.whisperDialogDict[name]
			dlg.SetGameMasterLook()

	def MakeWhisperButton(self, name):
		self.__MakeWhisperButton(name)

	## 버튼을 눌렀을때 창을 여는 함수
	def ShowWhisperDialog(self, btn):
		try:
			self.__MakeWhisperDialog(btn.name)
			dlgWhisper = self.whisperDialogDict[btn.name]
			dlgWhisper.OpenWithTarget(btn.name)
			dlgWhisper.Show()
			self.__CheckGameMaster(btn.name)
		except:
			import dbg
			dbg.TraceError("interface.ShowWhisperDialog - Failed to find key")

		## 버튼 초기화
		self.__DestroyWhisperButton(btn)

	## WhisperDialog 창에서 최소화 명령을 수행했을때 호출되는 함수
	## 창을 최소화 합니다.
	def MinimizeWhisperDialog(self, name):
		if 0 != name:
			self.__MakeWhisperButton(name)

		self.CloseWhisperDialog(name)

	## WhisperDialog 창에서 닫기 명령을 수행했을때 호출되는 함수
	## 창을 지웁니다.
	def CloseWhisperDialog(self, name):
		if 0 == name:
			if self.dlgWhisperWithoutTarget:
				self.dlgWhisperWithoutTarget.Destroy()
				self.dlgWhisperWithoutTarget = None

			return

		try:
			dlgWhisper = self.whisperDialogDict[name]
			dlgWhisper.Destroy()
			del self.whisperDialogDict[name]
		except:
			import dbg
			dbg.TraceError("interface.CloseWhisperDialog - Failed to find key")

	## 버튼의 개수가 바뀌었을때 버튼을 재정렬 하는 함수
	def __ArrangeWhisperButton(self):
		screenWidth = wndMgr.GetScreenWidth()
		screenHeight = wndMgr.GetScreenHeight()

		xPos = screenWidth - 70
		yPos = 170 * screenHeight / 600
		yCount = (screenHeight - 330) / 63
		#yCount = (screenHeight - 285) / 63
		if app.ENABLE_RTT_STATISTICS:
			yPos += 30

		count = 0
		for button in self.whisperButtonList:

			button.SetPosition(xPos + (int(count/yCount) * -50), yPos + (count%yCount * 63))
			count += 1

	## 이름으로 Whisper 버튼을 찾아 리턴해 주는 함수
	## 버튼은 딕셔너리로 하지 않는 것은 정렬 되어 버려 순서가 유지 되지 않으며
	## 이로 인해 ToolTip들이 다른 버튼들에 의해 가려지기 때문이다.
	def __FindWhisperButton(self, name):
		for button in self.whisperButtonList:
			if button.name == name:
				return button

		return 0

	## 창을 만듭니다.
	def __MakeWhisperDialog(self, name):
		dlgWhisper = uiWhisper.WhisperDialog(self.MinimizeWhisperDialog, self.CloseWhisperDialog)
		dlgWhisper.BindInterface(self)
		dlgWhisper.LoadDialog()
		dlgWhisper.SetPosition(self.windowOpenPosition*30,self.windowOpenPosition*30)
		self.whisperDialogDict[name] = dlgWhisper

		self.windowOpenPosition = (self.windowOpenPosition+1) % 5

		return dlgWhisper

	## 버튼을 만듭니다.
	def __MakeWhisperButton(self, name):
		whisperButton = uiWhisper.WhisperButton()
		if osfInfo.USE_NEW_WHISPER_LETTER_CATEGORY == True:
			if self.IsGameMasterName(name):
				whisperButton.SetUpVisual("d:/ymir work/ui/game/windows/btn_gm_mail_01.tga")
				whisperButton.SetOverVisual("d:/ymir work/ui/game/windows/btn_gm_mail_02.tga")
				whisperButton.SetDownVisual("d:/ymir work/ui/game/windows/btn_gm_mail_02.tga")
			else:
				whisperButton.SetUpVisual("d:/ymir work/ui/game/windows/btn_normal_mail_01.tga")
				whisperButton.SetOverVisual("d:/ymir work/ui/game/windows/btn_normal_mail_02.tga")
				whisperButton.SetDownVisual("d:/ymir work/ui/game/windows/btn_normal_mail_02.tga")
		else:
			whisperButton.SetUpVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
			whisperButton.SetOverVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
			whisperButton.SetDownVisual("d:/ymir work/ui/game/windows/btn_mail_up.sub")
		if self.IsGameMasterName(name):
			whisperButton.SetToolTipTextWithColor(name, 0xffffa200)
		else:
			whisperButton.SetToolTipText(name)
		whisperButton.ToolTipText.SetHorizontalAlignCenter()
		whisperButton.SetEvent(ui.__mem_func__(self.ShowWhisperDialog), whisperButton)
		whisperButton.Show()
		whisperButton.name = name

		self.whisperButtonList.insert(0, whisperButton)
		self.__ArrangeWhisperButton()

		return whisperButton

	def __DestroyWhisperButton(self, button):
		button.SetEvent(0)
		self.whisperButtonList.remove(button)
		self.__ArrangeWhisperButton()

	def HideAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Hide()

	def ShowAllWhisperButton(self):
		for btn in self.whisperButtonList:
			btn.Show()

	def __CheckGameMaster(self, name):
		if not self.listGMName.has_key(name):
			return
		if self.whisperDialogDict.has_key(name):
			dlg = self.whisperDialogDict[name]
			dlg.SetGameMasterLook()

	def RegisterGameMasterName(self, name):
		if self.listGMName.has_key(name):
			return
		self.listGMName[name] = "GM"

	def IsGameMasterName(self, name):
		if self.listGMName.has_key(name):
			return True
		else:
			return False

	#####################################################################################

	#####################################################################################
	### Guild Building ###

	def BUILD_OpenWindow(self):
		self.wndGuildBuilding = uiGuild.BuildGuildBuildingWindow()
		self.wndGuildBuilding.Open()
		self.wndGuildBuilding.wnds = self.__HideWindows()
		self.wndGuildBuilding.SetCloseEvent(ui.__mem_func__(self.BUILD_CloseWindow))

	def BUILD_CloseWindow(self):
		self.__ShowWindows(self.wndGuildBuilding.wnds)
		self.wndGuildBuilding = None

	def BUILD_OnUpdate(self):
		if not self.wndGuildBuilding:
			return

		if self.wndGuildBuilding.IsPositioningMode():
			import background
			x, y, z = background.GetPickingPoint()
			self.wndGuildBuilding.SetBuildingPosition(x, y, z)

	def BUILD_OnMouseLeftButtonDown(self):
		if not self.wndGuildBuilding:
			return

		# GUILD_BUILDING
		if self.wndGuildBuilding.IsPositioningMode():
			self.wndGuildBuilding.SettleCurrentPosition()
			return True
		elif self.wndGuildBuilding.IsPreviewMode():
			pass
		else:
			return True
		# END_OF_GUILD_BUILDING
		return False

	def BUILD_OnMouseLeftButtonUp(self):
		if not self.wndGuildBuilding:
			return

		if not self.wndGuildBuilding.IsPreviewMode():
			return True

		return False

	def BULID_EnterGuildArea(self, areaID):
		# GUILD_BUILDING
		mainCharacterName = player.GetMainCharacterName()
		masterName = guild.GetGuildMasterName()

		if mainCharacterName != masterName:
			return

		if areaID != player.GetGuildID():
			return
		# END_OF_GUILD_BUILDING

		self.wndGameButton.ShowBuildButton()

	def BULID_ExitGuildArea(self, areaID):
		self.wndGameButton.HideBuildButton()

	#####################################################################################

	def IsEditLineFocus(self):
		if self.ChatWindow.chatLine.IsFocus():
			return 1

		if self.ChatWindow.chatToLine.IsFocus():
			return 1

		return 0

	def EmptyFunction(self):
		pass

	if app.ENABLE_MINI_GAME_OKEY:
		def MiniGameOkey(self):
			isOpen = osfInfo.RUMI_GAME_EVENT

			if isOpen == True:
				if not self.wndMiniGame:
					self.wndMiniGame = uiMiniGame.MiniGameWindow()

				self.wndMiniGame.MiniGameOkeyEvent(True)
			else:
				if self.wndMiniGame:
					self.wndMiniGame.MiniGameOkeyEvent(False)

	if app.ENABLE_MINI_GAME_CATCH_KING:
		def SetCatchKingEventStatus(self, isEnable):
			if isEnable:
				osfInfo.CATCH_KING_GAME_EVENT = True
			else:
				osfInfo.CATCH_KING_GAME_EVENT = False

		def MiniGameCatchKingEventStart(self, bigScore):
			if self.wndMiniGame:
				self.wndMiniGame.MiniGameCatchKingEventStart(bigScore)

		def MiniGameCatchKingSetHandCard(self, cardNumber):
			if self.wndMiniGame:
				self.wndMiniGame.MiniGameCatchKingSetHandCard(cardNumber)

		def MiniGameCatchKingResultField(self, score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear):
			if self.wndMiniGame:
				self.wndMiniGame.MiniGameCatchKingResultField(score, rowType, cardPos, cardValue, keepFieldCard, destroyHandCard, getReward, isFiveNear)

		def MiniGameCatchKingSetEndCard(self, cardPos, cardNumber):
			if self.wndMiniGame:
				self.wndMiniGame.MiniGameCatchKingSetEndCard(cardPos, cardNumber)

		def MiniGameCatchKingReward(self, rewardCode):
			if self.wndMiniGame:
				self.wndMiniGame.MiniGameCatchKingReward(rewardCode)

	def IntegrationEventBanner(self):
		isOpen = []

		if app.ENABLE_MINI_GAME_OKEY:
			isOpen.append(osfInfo.RUMI_GAME_EVENT)

		if app.ENABLE_MINI_GAME_CATCH_KING:
			isOpen.append(osfInfo.CATCH_KING_GAME_EVENT)

		if True in isOpen:
			if not self.wndMiniGame:
				self.wndMiniGame = uiMiniGame.MiniGameWindow()

				if self.tooltipItem:
					self.wndMiniGame.SetItemToolTip(self.tooltipItem)

			self.wndMiniGame.IntegrationMiniGame(True)
		else:
			if self.wndMiniGame:
				self.wndMiniGame.IntegrationMiniGame(False)

	if app.WJ_ENABLE_TRADABLE_ICON:
		def AttachInvenItemToOtherWindowSlot(self, slotIndex):
			# Used for (ENABLE_MOVE_COSTUME_ATTR, ENABLE_GROWTH_PET_SYSTEM)
			return False

		def MarkUnusableInvenSlotOnTopWnd(self, onTopWnd, InvenSlot):
			if app.WJ_ENABLE_TRADABLE_ICON:
				if onTopWnd == player.ON_TOP_WND_SHOP and self.dlgShop and self.dlgShop.CantSellInvenItem(InvenSlot):
					return True
				elif onTopWnd == player.ON_TOP_WND_SAFEBOX and self.wndSafebox and self.wndSafebox.CantCheckInItem(InvenSlot):
					return True
				elif onTopWnd == player.ON_TOP_WND_PRIVATE_SHOP and self.privateShopBuilder and self.privateShopBuilder.CantTradableItem(InvenSlot):
					return True
				elif onTopWnd == player.ON_TOP_WND_EXCHANGE and self.dlgExchange and self.dlgExchange.CantTradableItem(InvenSlot):
					return True

			if app.ENABLE_MAILBOX:
				if onTopWnd == player.ON_TOP_WND_MAILBOX and self.mail_box and self.mail_box.CantPostItemSlot(InvenSlot):
					return True

			return False

		if app.ENABLE_MAILBOX:
			def MarkUnusableDSInvenSlotOnTopWnd(self, onTopWnd, index):
				if onTopWnd == player.ON_TOP_WND_MAILBOX and self.mail_box and self.mail_box.CantPostItemSlot(index, player.DRAGON_SOUL_INVENTORY):
					return True

				return False

		def SetOnTopWindow(self, onTopWnd):
			self.OnTopWindow = onTopWnd

		def GetOnTopWindow(self):
			return self.OnTopWindow

		def RefreshMarkInventoryBag(self):
			if self.wndInventory and self.wndInventory.IsShow():
				self.wndInventory.RefreshBagSlotWindow()

			if app.ENABLE_MAILBOX:
				if self.wndDragonSoul and self.wndDragonSoul.IsShow():
					self.wndDragonSoul.RefreshBagSlotWindow()

			if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
				if self.wndSpecialInventory and self.wndSpecialInventory.IsShow():
					self.wndSpecialInventory.RefreshBagSlotWindow()

	if app.ENABLE_HIDE_COSTUME_SYSTEM:
		def RefreshVisibleCostume(self):
			self.wndInventory.RefreshVisibleCostume()

	if app.ENABLE_MOVE_CHANNEL:
		def RefreshServerInfo(self):
			if self.wndMiniMap:
				self.wndMiniMap.RefreshServerInfo()

	if app.ENABLE_EXPRESSING_EMOTION:
		def ClearSpecialEmotions(self):
			if self.wndCharacter:
				self.wndCharacter.ClearSpecialEmotions()

		def RegisterSpecialEmotions(self, emotionIdx, leftTime):
			if self.wndCharacter:
				self.wndCharacter.RegisterSpecialEmotions(emotionIdx, leftTime)

	if app.ENABLE_MAILBOX:
		def MailBoxProcess(self, type, data):
			if not self.mail_box:
				return

			self.mail_box.MailBoxProcess(type, data)

		def MiniMapMailProcess(self, type, data):
			if not self.wndMiniMap:
				return

			self.wndMiniMap.MiniMapMailProcess(type, data)

if __name__ == "__main__":
	import app
	import wndMgr
	import systemSetting
	import mouseModule
	import grp
	import ui
	import localeInfo

	app.SetMouseHandler(mouseModule.mouseController)
	app.SetHairColorEnable(True)
	wndMgr.SetMouseHandler(mouseModule.mouseController)
	wndMgr.SetScreenSize(systemSetting.GetWidth(), systemSetting.GetHeight())
	app.Create(localeInfo.APP_TITLE, systemSetting.GetWidth(), systemSetting.GetHeight(), 1)
	mouseModule.mouseController.Create()

	class TestGame(ui.Window):
		def __init__(self):
			ui.Window.__init__(self)

			localeInfo.LoadLocaleData()
			player.SetItemData(0, 27001, 10)
			player.SetItemData(1, 27004, 10)

			self.interface = Interface()
			self.interface.MakeInterface()
			self.interface.ShowDefaultWindows()
			self.interface.RefreshInventory()
			#self.interface.OpenCubeWindow()

		def __del__(self):
			ui.Window.__del__(self)

		def OnUpdate(self):
			app.UpdateGame()

		def OnRender(self):
			app.RenderGame()
			grp.PopState()
			grp.SetInterfaceRenderState()

	game = TestGame()
	game.SetSize(systemSetting.GetWidth(), systemSetting.GetHeight())
	game.Show()

	app.Loop()