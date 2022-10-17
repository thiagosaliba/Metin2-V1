#
# Date : YMD.2020.03.23
# File : uiSkillColor.py
# Version : 1.0.1.6
# Author : Owsap
#

import app
import ui
import uiCommon
import grp
import wndMgr
import uiScriptLocale
import localeInfo
import player
import chat
import uiToolTip
import skill
import cfg
import uiCommon
import sys

MAX_COLOR_LAYERS = 5
COLOR_PRESET_SELECT_UI_SHOW_MAX = 10
HEX_CODE_LENGTH = 7
COLOR_PRESET_SAVE_MAX = (COLOR_PRESET_SELECT_UI_SHOW_MAX * (HEX_CODE_LENGTH + 1)) - 1
if app.ENABLE_5LAYER_SKILL_COLOR:
	COLOR_GRADIENT = [255, 255, 255, 255, 255]
else:
	COLOR_GRADIENT = [255, 204, 153, 102, 51]
COLOR_PRESET_TOOLTIP_LIST = [
	localeInfo.SKILL_COLOR_PRESET_TOOLTIP_1,
	localeInfo.SKILL_COLOR_PRESET_TOOLTIP_2,
	localeInfo.SKILL_COLOR_PRESET_TOOLTIP_3
]

CUSTOM_COLOR_HEX_CODE_TOOLTIP_LIST = [
	localeInfo.SKILL_COLOR_CUSTOM_HEX_CODE_TOOLTIP_1,
	localeInfo.SKILL_COLOR_CUSTOM_HEX_CODE_TOOLTIP_2
]

class SkillColorWindow(ui.ScriptWindow):
	USE_HEX_COLOR_INFO_TABLE = 0

	def __init__(self, skillSlot, skillIndex):
		print("skillSlot : {} skillIndex: {}".format(skillSlot, skillIndex))
		ui.Window.__init__(self)
		self.isLoaded = False

		self.skillSlot = skillSlot
		self.skillIndex = skillIndex

		self.pageIndex = 1

		self.colorHexCodes = {}

		self.selectedColor = False
		self.rgbColor = None
		self.grpColor = None
		self.grpColorPreview = None

		self.colorPresetDict = {}
		self.colorPresetListOpen = False
		self.colorPresetWindowHeight = 0
		self.colorPresetPreview = False
		self.selectedColorPreset = 0

		if app.ENABLE_5LAYER_SKILL_COLOR:
			self.rgbLayerColor = {}
			self.selectedLayer = 0
			self.selectedColorLayer = {}
			self.arrowAnimation = {}
			self.nextColorLayer = 0

		self.skillColors = {}

	def __del__(self):
		ui.Window.__del__(self)

	def __LoadWindow(self):
		if self.isLoaded == 1:
			return

		self.isLoaded = 1

		try:
			pyScrLoader = ui.PythonScriptLoader()
			pyScrLoader.LoadScriptFile(self, "UIScript/SkillColorWindow.py")
		except:
			import exception
			exception.Abort("SkillColorWindow.__LoadWindow.LoadScriptFile")

		try:
			self.__BindObject()
		except:
			import exception
			exception.Abort("SkillColorWindow.__LoadWindow.__BindObject")

		try:
			self.__BindEvent()
		except:
			import exception
			exception.Abort("SkillColorWindow.__LoadWindow.__BindEvent")

	def __BindObject(self):
		self.GetChild("Board").SetCloseEvent(ui.__mem_func__(self.Close))

		""" --------------------------------------------------------------- """
		""" PAGE 1															"""
		""" --------------------------------------------------------------- """

		""" Background Objects """
		## Background Image & Color Bar
		self.bgImg = self.GetChild("BGImage")
		self.bgColorBar = self.GetChild("BGColorBar")

		## Color Picker Background Image & Button
		self.bgColorPickerImg = self.GetChild("BGColorPickerImage")
		self.bgColorPickerButton = self.GetChild("BGColorPickerButton")
		## Color Picker Dot Image
		self.bgColorPickerDotImg = self.GetChild("BGColorPickerDotImage")
		self.bgColorPickerDotImg.Hide()

		## Color Layers Button
		self.bgColorLayerButton = []
		self.bgColorLayerButton.append(self.GetChild("ColorLayer1Button"))
		self.bgColorLayerButton.append(self.GetChild("ColorLayer2Button"))
		self.bgColorLayerButton.append(self.GetChild("ColorLayer3Button"))
		self.bgColorLayerButton.append(self.GetChild("ColorLayer4Button"))
		self.bgColorLayerButton.append(self.GetChild("ColorLayer5Button"))
		if not app.ENABLE_5LAYER_SKILL_COLOR:
			for button in self.bgColorLayerButton:
				button.Hide()

		## Confirm & Cancel Buttons
		self.confirmButton = self.GetChild("ConfirmButton")
		self.cancelButton = self.GetChild("CancelButton")

		## Page Navigation Buttons
		self.prevPageButton = self.GetChild("PrevPageButton")
		self.nextPageButton = self.GetChild("NextPageButton")

		""" --------------------------------------------------------------- """
		""" PAGE 2															"""
		""" --------------------------------------------------------------- """

		""" Background Objects """
		## Background Image
		self.bg2Img = self.GetChild("BG2Image")

		""" Color Preset Objects """
		## Color Preset Button (Open / Close List)
		self.bg2ColorPresetButton = self.GetChild("BG2ColorPresetButton")
		## Color Preset Window (List)
		self.bg2ColorPresetWindow = self.GetChild("BG2ColorPresetWindow")
		## Color Preset EditLine
		self.bg2ColorPresetEditLine = self.GetChild("BG2ColorPresetEditLine")
		## Color Preset ToolTip
		self.bg2ColorPresetToolTip = self.GetChild("BG2ColorPresetToolTip")
		## Color Preset Buttons
		self.bg2ColorPresetSaveButton = self.GetChild("BG2ColorPresetSaveButton")
		self.bg2ColorPresetClearButton = self.GetChild("BG2ColorPresetClearButton")

		## Color Preset Mouse Over Image (List)
		self.bg2MouseOverImage = self.GetChild("BG2MouseOverImage")
		self.bg2MouseOverImage.Hide()

		""" Custom Color Objects """
		## Custom Color Input Slot Image
		self.bg2CustonColorInputSlotImg = self.GetChild("BG2CustomColorInputSlotImage")
		## Custom Color EditLine
		self.bg2CustomColorEditLine = self.GetChild("BG2CustomColorEditLine")
		## Custom Color ToolTip
		self.bg2CustomColorToolTip = self.GetChild("BG2CustomColorToolTip")

		## Default Color Button
		self.defaultButton = self.GetChild("DefaultButton")

		self.perfCheckBox = ui.CheckBox()
		self.perfCheckBox.SetParent(self.bg2Img)
		self.perfCheckBox.SetPosition(0, 23)
		self.perfCheckBox.SetWindowHorizontalAlignCenter()
		self.perfCheckBox.SetWindowVerticalAlignBottom()
		self.perfCheckBox.SetEvent(ui.__mem_func__(self.__OnClickPerfCheckBox), "ON_CHECK", 1)
		self.perfCheckBox.SetEvent(ui.__mem_func__(self.__OnClickPerfCheckBox), "ON_UNCKECK", 0)
		self.perfCheckBox.SetCheckStatus(self.USE_HEX_COLOR_INFO_TABLE)
		self.perfCheckBox.SetTextInfo(localeInfo.SKILL_COLOR_USE_EXTERNAL_TABLE)
		self.perfCheckBox.Show()

	def __OnClickPerfCheckBox(self, checkType, flag):
		self.USE_HEX_COLOR_INFO_TABLE = flag

		if self.USE_HEX_COLOR_INFO_TABLE:
			import hexcolorinfo
			self.colorHexCodes = hexcolorinfo.HEX_CODES

	def __BindEvent(self):
		""" --------------------------------------------------------------- """
		""" PAGE 1															"""
		""" --------------------------------------------------------------- """

		## Color Picker Dot Selected Image
		self.colorPickerDotSelectImg = ui.ImageBox()
		self.colorPickerDotSelectImg.LoadImage("d:/ymir work/ui/skillcolor/color_picker_dot.tga")
		self.colorPickerDotSelectImg.SetParent(self.bgColorPickerImg)
		self.colorPickerDotSelectImg.SetSize(12, 12)
		self.colorPickerDotSelectImg.Hide()

		self.bgColorPickerDotImg.SetEvent(ui.__mem_func__(self.OnClickColorPicker), "mouse_click")

		## Confirm & Cancel Buttons
		self.confirmButton.SetEvent(ui.__mem_func__(self.OnClickConfirmButton))
		self.cancelButton.SetEvent(ui.__mem_func__(self.Close))

		## Page Navigation Objects (Buttons)
		self.prevPageButton.SetEvent(ui.__mem_func__(self.OnClickPrevButton))
		self.nextPageButton.SetEvent(ui.__mem_func__(self.OnClickNextButton))

		""" --------------------------------------------------------------- """
		""" PAGE 2															"""
		""" --------------------------------------------------------------- """

		""" Color Preset Objects """
		## Color Preset List
		self.bg2ColorPresetEditLine.SetEvent(ui.__mem_func__(self.OnClickColorPresetButton))
		self.bg2ColorPresetButton.SetEvent(ui.__mem_func__(self.OnClickColorPresetButton))
		self.bg2ColorPresetEditLine.SetTextAlignLeft(self.bg2ColorPresetEditLine.GetText(), 2)

		## Color Preset Button
		self.bg2ColorPresetSaveButton.SetEvent(ui.__mem_func__(self.OnClickSaveColorButton))
		self.bg2ColorPresetSaveButton.Disable()
		self.bg2ColorPresetSaveButton.Down()
		## Clear Color Preset Button
		self.bg2ColorPresetClearButton.SetEvent(ui.__mem_func__(self.OnClickClearColorButton))
		self.bg2ColorPresetClearButton.Disable()
		self.bg2ColorPresetClearButton.Down()

		## Color Preset ToolTip
		self.colorPresetToolTip = self.__CreateGameTypeToolTip(localeInfo.SKILL_COLOR_PRESET_TOOLTIP_TITLE, COLOR_PRESET_TOOLTIP_LIST)
		self.bg2ColorPresetToolTip.SetOverEvent(ui.__mem_func__(self.OnOverColorPresetToolTip))
		self.bg2ColorPresetToolTip.SetOverOutEvent(ui.__mem_func__(self.OnOverOutColorPresetToolTip))

		self.customColorToolTip = self.__CreateGameTypeToolTip(localeInfo.SKILL_COLOR_CUSTOM_HEX_CODE_TOOLTIP_TITLE, CUSTOM_COLOR_HEX_CODE_TOOLTIP_LIST)
		self.bg2CustomColorToolTip.SetOverEvent(ui.__mem_func__(self.OnOverCustomColorToolTip))
		self.bg2CustomColorToolTip.SetOverOutEvent(ui.__mem_func__(self.OnOverOutCustomColorToolTip))

		## Custom Color EditLine
		self.bg2CustomColorEditLine.OnIMEUpdate = ui.__mem_func__(self.OnUpdateHex)

		## Default Color Button
		self.defaultButton.SetEvent(ui.__mem_func__(self.OnClickDefaultButton))

		## ToolTip
		self.toolTip = uiToolTip.ToolTip()
		self.toolTip.ClearToolTip()

		## PopUp
		self.popup = uiCommon.PopupDialog()

		self.GetCurrentColor(self.skillSlot)
		self.ReloadSavedColorPreset(self.skillSlot)
		self.ReloadPage()
		if app.ENABLE_5LAYER_SKILL_COLOR:
			i = 0
			for button in self.bgColorLayerButton:
				button.SetEvent(ui.__mem_func__(self.SelectColorLayer), i)

				## Arrow Animation
				self.arrowAnimation[i] = ui.AniImageBox()
				self.arrowAnimation[i].SetParent(self.bgColorLayerButton[i])
				self.arrowAnimation[i].SetPosition(7.5, -35)
				self.arrowAnimation[i].SetDelay(8)
				for x in xrange(6):
					self.arrowAnimation[i].AppendImage("d:/ymir work/ui/minigame/yutnori/move_arrow/%d.sub" % x)
				self.arrowAnimation[i].Hide()

				i += 1

			self.SelectColorLayer(0)
			self.confirmButton.Disable()
			self.confirmButton.Down()

	if app.ENABLE_5LAYER_SKILL_COLOR:
		def SelectColorLayer(self, layerIndex):
			if self.nextColorLayer != layerIndex:
				if layerIndex >= self.nextColorLayer:
					return

			if layerIndex > 0:
				if not self.CanChangeColor():
					self.popup.SetText(localeInfo.SKILL_COLOR_SELECT_FIRST)
					self.popup.Open()
					return

			for button in self.bgColorLayerButton:
				button.Enable()

			if self.arrowAnimation:
				self.arrowAnimation[layerIndex].Hide()

			#self.bgColorLayerButton[layerIndex].DisableFlash()
			self.bgColorLayerButton[layerIndex].Disable()
			self.selectedLayer = layerIndex

			self.selectedColorLayer[layerIndex] = True

			if layerIndex >= MAX_COLOR_LAYERS - 1:
				self.confirmButton.Enable()

	def OnUpdateHex(self):
		ui.EditLine.OnIMEUpdate(self.bg2CustomColorEditLine) 
		text = self.bg2CustomColorEditLine.GetText()

		if len(text):
			self.bg2CustomColorEditLine.SetText(str(text))
			self.bgColorPickerDotImg.Hide()

		strLen = len(str(text))
		if strLen >= HEX_CODE_LENGTH - 1:
			if text.find("#") == -1:
				self.bg2CustomColorEditLine.SetText("")
				self.bg2CustomColorEditLine.SetText("#" + str(text) + "")

			color = str(self.bg2CustomColorEditLine.GetText()).split("#")
			rgbColor = self.HexToRGB(str(color[1]))
			r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)
			self.rgbColor = rgbColor
			self.grpColor = (r, g, b)
			self.grpColorPreview = None
			self.selectedColor = True

			if app.ENABLE_5LAYER_SKILL_COLOR:
				self.rgbLayerColor = {}

				self.confirmButton.Enable()

			if self.colorPickerDotSelectImg:
				self.colorPickerDotSelectImg.Hide()

			if self.bgColorBar:
				self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

			if self.bg2ColorPresetSaveButton:
				self.bg2ColorPresetSaveButton.Enable()
				self.bg2ColorPresetSaveButton.SetUp()

	def OnClickDefaultButton(self):
		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.SKILL_COLOR_DO_YOU_RESET)
		questionDialog.SetAcceptEvent(lambda arg = 0 : ui.__mem_func__(self.OnAcceptQuestionDialog)(arg))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def OnOverColorPresetToolTip(self):
		if self.colorPresetToolTip:
			self.colorPresetToolTip.Show()
	def OnOverOutColorPresetToolTip(self):
		if self.colorPresetToolTip:
			self.colorPresetToolTip.Hide()

	def OnOverCustomColorToolTip(self):
		if self.customColorToolTip:
			self.customColorToolTip.Show()
	def OnOverOutCustomColorToolTip(self):
		if self.customColorToolTip:
			self.customColorToolTip.Hide()

	def __CreateGameTypeToolTip(self, title, descList):
		tooltip = uiToolTip.ToolTip()
		tooltip.SetTitle(title)
		tooltip.AppendSpace(7)

		for desc in descList:
			tooltip.AutoAppendTextLine(desc)

		tooltip.AlignHorizonalCenter()
		tooltip.SetTop()
		return tooltip

	def CanChangeColor(self):
		if not self.selectedColor:
			return False

		if not self.grpColor and not self.grpColorPreview:
			return False

		if not self.rgbColor:
			return False

		return True

	def OnClickConfirmButton(self):
		if app.ENABLE_5LAYER_SKILL_COLOR:
			if self.rgbLayerColor:
				for i in self.rgbLayerColor:
					rgbColor = self.rgbLayerColor[i]
					print("HEX {} : #{:02x}{:02x}{:02x}").format(i, int(rgbColor[0]), int(rgbColor[1]), int(rgbColor[2]))

		if not self.CanChangeColor():
			self.popup.SetText(localeInfo.SKILL_COLOR_SELECT_FIRST)
			self.popup.Open()
			return

		if app.ENABLE_5LAYER_SKILL_COLOR:
			if len(self.selectedColorLayer) != MAX_COLOR_LAYERS:
				self.popup.SetText("Please select all 5 layers.")
				self.popup.Open()
				return

		questionDialog = uiCommon.QuestionDialog()
		questionDialog.SetText(localeInfo.SKILL_COLOR_DO_YOU_CHANGE)
		questionDialog.SetAcceptEvent(lambda arg = 1 : ui.__mem_func__(self.OnAcceptQuestionDialog)(arg))
		questionDialog.SetCancelEvent(ui.__mem_func__(self.OnCloseQuestionDialog))
		questionDialog.Open()
		self.questionDialog = questionDialog

	def OnAcceptQuestionDialog(self, arg):
		if arg == 0:
			player.SetSkillColor(self.skillSlot, 0, 0, 0, 0, 0)
		else:
			if app.ENABLE_5LAYER_SKILL_COLOR:
				if self.rgbLayerColor and len(self.selectedColorLayer) >= MAX_COLOR_LAYERS:
					rgbLayerColor = self.rgbLayerColor
					tmpR1, tmpG1, tmpB1 = (float(rgbLayerColor[0][0]), float(rgbLayerColor[0][1]), float(rgbLayerColor[0][2]))
					tmpR2, tmpG2, tmpB2 = (float(rgbLayerColor[1][0]), float(rgbLayerColor[1][1]), float(rgbLayerColor[1][2]))
					tmpR3, tmpG3, tmpB3 = (float(rgbLayerColor[2][0]), float(rgbLayerColor[2][1]), float(rgbLayerColor[2][2]))
					tmpR4, tmpG4, tmpB4 = (float(rgbLayerColor[3][0]), float(rgbLayerColor[3][1]), float(rgbLayerColor[3][2]))
					tmpR5, tmpG5, tmpB5 = (float(rgbLayerColor[4][0]), float(rgbLayerColor[4][1]), float(rgbLayerColor[4][2]))

					self.skillColors[0] = grp.GenerateColor(tmpR1 / COLOR_GRADIENT[0], tmpG1 / COLOR_GRADIENT[0], tmpB1 / COLOR_GRADIENT[0], 0.0) ## 0 ~ 1
					self.skillColors[1] = grp.GenerateColor(tmpR2 / COLOR_GRADIENT[1], tmpG2 / COLOR_GRADIENT[1], tmpB2 / COLOR_GRADIENT[1], 0.0) ## 1 ~ 2
					self.skillColors[2] = grp.GenerateColor(tmpR3 / COLOR_GRADIENT[2], tmpG3 / COLOR_GRADIENT[2], tmpB3 / COLOR_GRADIENT[2], 0.0) ## 2 ~ 3
					self.skillColors[3] = grp.GenerateColor(tmpR4 / COLOR_GRADIENT[3], tmpG4 / COLOR_GRADIENT[3], tmpB4 / COLOR_GRADIENT[3], 0.0) ## 3 ~ 4
					self.skillColors[4] = grp.GenerateColor(tmpR5 / COLOR_GRADIENT[4], tmpG5 / COLOR_GRADIENT[4], tmpB5 / COLOR_GRADIENT[4], 0.0) ## 4 ~ 5
					player.SetSkillColor(self.skillSlot,\
						self.skillColors[0],\
						self.skillColors[1],\
						self.skillColors[2],\
						self.skillColors[3],\
						self.skillColors[4]\
					)
				else:
					rgbColor = self.rgbColor
					tmpR, tmpG, tmpB = (float(rgbColor[0]), float(rgbColor[1]), float(rgbColor[2]))
					self.skillColors[0] = grp.GenerateColor(tmpR / COLOR_GRADIENT[0], tmpG / COLOR_GRADIENT[0], tmpB / COLOR_GRADIENT[0], 0.0) ## 0 ~ 1
					self.skillColors[1] = grp.GenerateColor(tmpR / COLOR_GRADIENT[1], tmpG / COLOR_GRADIENT[1], tmpB / COLOR_GRADIENT[1], 0.0) ## 1 ~ 2
					self.skillColors[2] = grp.GenerateColor(tmpR / COLOR_GRADIENT[2], tmpG / COLOR_GRADIENT[2], tmpB / COLOR_GRADIENT[2], 0.0) ## 2 ~ 3
					self.skillColors[3] = grp.GenerateColor(tmpR / COLOR_GRADIENT[3], tmpG / COLOR_GRADIENT[3], tmpB / COLOR_GRADIENT[3], 0.0) ## 3 ~ 4
					self.skillColors[4] = grp.GenerateColor(tmpR / COLOR_GRADIENT[4], tmpG / COLOR_GRADIENT[4], tmpB / COLOR_GRADIENT[4], 0.0) ## 4 ~ 5
					player.SetSkillColor(self.skillSlot,\
						self.skillColors[0],\
						self.skillColors[1],\
						self.skillColors[2],\
						self.skillColors[3],\
						self.skillColors[4]\
					)
			else:
				rgbColor = self.rgbColor
				tmpR, tmpG, tmpB = (float(rgbColor[0]), float(rgbColor[1]), float(rgbColor[2]))
				self.skillColors[0] = grp.GenerateColor(tmpR / COLOR_GRADIENT[0], tmpG / COLOR_GRADIENT[0], tmpB / COLOR_GRADIENT[0], 0.0) ## 0 ~ 1
				self.skillColors[1] = grp.GenerateColor(tmpR / COLOR_GRADIENT[1], tmpG / COLOR_GRADIENT[1], tmpB / COLOR_GRADIENT[1], 0.0) ## 1 ~ 2
				self.skillColors[2] = grp.GenerateColor(tmpR / COLOR_GRADIENT[2], tmpG / COLOR_GRADIENT[2], tmpB / COLOR_GRADIENT[2], 0.0) ## 2 ~ 3
				self.skillColors[3] = grp.GenerateColor(tmpR / COLOR_GRADIENT[3], tmpG / COLOR_GRADIENT[3], tmpB / COLOR_GRADIENT[3], 0.0) ## 3 ~ 4
				self.skillColors[4] = grp.GenerateColor(tmpR / COLOR_GRADIENT[4], tmpG / COLOR_GRADIENT[4], tmpB / COLOR_GRADIENT[4], 0.0) ## 4 ~ 5
				player.SetSkillColor(self.skillSlot,\
					self.skillColors[0],\
					self.skillColors[1],\
					self.skillColors[2],\
					self.skillColors[3],\
					self.skillColors[4]\
				)

		self.Close()

		self.OnCloseQuestionDialog()
		return True

	def OnCloseQuestionDialog(self):
		self.questionDialog.Close()
		self.questionDialog = None
		return True

	def GetCurrentColor(self, skillSlot):
		currentSkillColor = player.GetSkillColor(skillSlot)
		decColor = 0

		if currentSkillColor[0] > 0:
			decColor = currentSkillColor[0]

		elif currentSkillColor[1] > 0:
			decColor = currentSkillColor[1]

		elif currentSkillColor[2] > 0:
			decColor = currentSkillColor[2]

		elif currentSkillColor[3] > 0:
			decColor = currentSkillColor[3]

		elif currentSkillColor[4] > 0:
			decColor = currentSkillColor[4]

		else:
			return # no color data from current skill slot index

		skillColorCode = hex(decColor).split('x')[-1]
		rgbColor = self.HexToRGB(skillColorCode)
		r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)
		#self.grpColor = (r, g, b)
		#self.grpColorPreview = (r, g, b)
		#self.selectedColor = True

		if self.bgColorBar:
			self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

	def OnUpdate(self):
		if self.bgColorPickerButton.IsIn() or self.bgColorPickerDotImg.IsIn():
			self.ColorPickerButtonOverIn()
		else:
			self.ColorPickerButtonOverOut()

	def GetRGBColor(self):
		xMouse, yMouse = wndMgr.GetRealMousePosition()
		return wndMgr.GetColorAtPosition(xMouse, yMouse)

	def ColorPickerButtonOverIn(self):
		xButtonGPos, yButtonGPos = self.bgColorPickerImg.GetGlobalPosition()
		xRealMousePos, yRealMousePos = wndMgr.GetRealMousePosition()
		xMousePos, yMousePos = wndMgr.GetMousePosition()

		xMouse = xMousePos - xButtonGPos
		yMouse = yMousePos - yButtonGPos

		if xMouse > 255:
			xMouse = 255

		if yMouse > 255:
			yMouse = 255

		if self.bgColorPickerDotImg:
			self.bgColorPickerDotImg.SetPosition(xMousePos - xButtonGPos - 6, yMousePos - yButtonGPos - 6)
			self.bgColorPickerDotImg.Show()

			if self.USE_HEX_COLOR_INFO_TABLE:
				hexCode = self.colorHexCodes[str("%sx%s" % (xMouse, yMouse))].lstrip('#')
				rgbColor = tuple(int(hexCode[i:i+2], 16) for i in (0, 2, 4))
			else:
				rgbColor = self.GetRGBColor()

			r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)

			if self.bgColorBar:
				self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

	def ColorPickerButtonOverOut(self):
		if not self.bgColorPickerButton.IsIn() and not self.bgColorPickerDotImg.IsIn():
			self.bgColorPickerDotImg.Hide()
			if self.selectedColor:
				if self.grpColorPreview:
					r, g, b = self.grpColorPreview
					self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))
				elif self.grpColor:
					r, g, b = self.grpColor
					self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))
				else:
					self.bgColorBar.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 1.0))
			else:
				if self.grpColorPreview:
					r, g, b = self.grpColorPreview
					self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))
				else:
					self.bgColorBar.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 1.0))

	def MarkColorDotPos(self):
		xButtonGPos, yButtonGPos = self.bgColorPickerImg.GetGlobalPosition()
		xMousePos, yMousePos = wndMgr.GetMousePosition()

		if self.colorPickerDotSelectImg:
			self.colorPickerDotSelectImg.SetPosition(xMousePos - xButtonGPos - 6, yMousePos - yButtonGPos - 6)
			self.colorPickerDotSelectImg.Show()

	def OnClickClearColorButton(self):
		if not self.colorPresetDict:
			return

		skillColorPreset = self.GetSavedColorPresets(self.skillSlot)
		selectedColorCode = skillColorPreset[self.selectedColorPreset][:HEX_CODE_LENGTH]

		skillColorCfg = cfg.Get(cfg.SAVE_SKILL_COLOR, str(self.skillSlot))
		skillColorCfgStr = skillColorCfg.replace(selectedColorCode, '')

		cfg.Set(cfg.SAVE_SKILL_COLOR, str(self.skillSlot), skillColorCfgStr)

		self.ClearColors()

		self.ReloadSavedColorPreset(self.skillSlot)

	def ClearColors(self):
		self.selectedColorPreset = 0
		self.grpColorPreview = None
		self.colorPresetPreview = False
		self.selectedColor = False

		if self.colorPickerDotSelectImg:
			self.colorPickerDotSelectImg.Hide()

		self.bg2CustomColorEditLine.SetText("")
		self.bgColorBar.SetColor(grp.GenerateColor(0.0, 0.0, 0.0, 1.0))

		self.bg2ColorPresetClearButton.Disable()
		self.bg2ColorPresetClearButton.Down()

	def OnClickColorPicker(self):
		xRealMousePos, yRealMousePos = wndMgr.GetRealMousePosition()
		rgbColor = wndMgr.GetColorAtPosition(xRealMousePos, yRealMousePos)

		r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)
		self.rgbColor = rgbColor
		self.grpColor = (r, g, b)
		if app.ENABLE_5LAYER_SKILL_COLOR:
			self.rgbLayerColor[self.selectedLayer] = rgbColor
		self.selectedColor = True

		self.MarkColorDotPos()

		if self.bgColorBar:
			self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

		if self.bg2ColorPresetSaveButton:
			self.bg2ColorPresetSaveButton.Enable()
			self.bg2ColorPresetSaveButton.SetUp()

		if app.ENABLE_5LAYER_SKILL_COLOR:
			print("HEX {}: #{:02x}{:02x}{:02x}").format(self.selectedLayer, int(self.rgbLayerColor[self.selectedLayer][0]), int(self.rgbLayerColor[self.selectedLayer][1]), int(self.rgbLayerColor[self.selectedLayer][2]))
		else:
			print("HEX : #{:02x}{:02x}{:02x}").format(int(rgbColor[0]), int(rgbColor[1]), int(rgbColor[2]))

		if app.ENABLE_5LAYER_SKILL_COLOR:
			if self.selectedLayer < MAX_COLOR_LAYERS - 1:
				# self.bgColorLayerButton[self.selectedLayer + 1].Flash()
				if self.arrowAnimation:
					self.arrowAnimation[self.selectedLayer].Hide()
					self.arrowAnimation[self.selectedLayer + 1].Show()
					self.nextColorLayer = self.selectedLayer + 1

	def GetSavedColorPresets(self, skillSlot):
		skillSlotColorCfg = cfg.Get(cfg.SAVE_SKILL_COLOR, str(skillSlot))
		if not skillSlotColorCfg:
			return

		hexCodeList = skillSlotColorCfg.split()

		tmpColorPresetSaves = []

		index = 0
		while index < len(hexCodeList):
			tmpColorPresetSaves.append(hexCodeList[index])
			index += 1

		return tmpColorPresetSaves

	def OnClickSaveColorButton(self):
		if not self.rgbColor or not self.grpColor or not self.selectedColor:
			self.popup.SetText(localeInfo.SKILL_COLOR_SELECT_FIRST)
			self.popup.Open()
			return

		rgbColor = self.rgbColor
		hexCode = "#{:02x}{:02x}{:02x}".format(int(rgbColor[0]), int(rgbColor[1]), int(rgbColor[2]))

		skillColorCfg = cfg.Get(cfg.SAVE_SKILL_COLOR, str(self.skillSlot))
		if len(skillColorCfg) >= COLOR_PRESET_SAVE_MAX:
			self.popup.SetText(localeInfo.SKILL_COLOR_CANNOT_SAVE)
			self.popup.Open()
			return

		if len(skillColorCfg) < HEX_CODE_LENGTH:
			cfg.Set(cfg.SAVE_SKILL_COLOR, str(self.skillSlot), hexCode)
		else:
			cfg.Set(cfg.SAVE_SKILL_COLOR, str(self.skillSlot), skillColorCfg + " " + hexCode)

		self.ReloadSavedColorPreset(self.skillSlot)

	def ReloadSavedColorPreset(self, skillColor):
		if self.colorPresetListOpen:
			self.__ColorPresetWindow(False)

		if self.bg2ColorPresetEditLine:
			if not self.GetSavedColorPresets(self.skillSlot):
				self.bg2ColorPresetEditLine.SetText(localeInfo.SKILL_COLOR_SELECT_PRESET_NONE)
			else:
				self.bg2ColorPresetEditLine.SetText(localeInfo.SKILL_COLOR_SELECT_PRESET)

		self.colorPresetDict = {}
		self.colorPresetListOpen = False
		self.colorPresetWindowHeight = 0
		self.colorPresetPreview = False
		self.selectedColorPreset = 0

		self.bg2MouseOverImage.SetParent(self)

	def __CreateColorPresetButton(self):
		if not self.bg2ColorPresetWindow:
			return

		if self.colorPresetDict:
			return

		colorPresetList = self.GetSavedColorPresets(self.skillSlot)
		if not colorPresetList:
			return

		buttonHeight = 16
		dictLen = len(colorPresetList)
		self.colorPresetWindowHeight = dictLen * buttonHeight

		for i in xrange(min(COLOR_PRESET_SELECT_UI_SHOW_MAX, dictLen)):
			key = i
			button = ui.Button()
			button.SetParent(self.bg2ColorPresetWindow)
			button.SetPosition(0, buttonHeight * i - 1)

			if 1 == dictLen:
				button.SetUpVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_only.sub")
				button.SetDownVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_only.sub")
				button.SetOverVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_only.sub")
			elif i == 0:
				button.SetUpVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_top.sub")
				button.SetDownVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_top.sub")
				button.SetOverVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_top.sub")
			elif i >= COLOR_PRESET_SELECT_UI_SHOW_MAX - 1:
				button.SetUpVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_bottom.sub")
				button.SetDownVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_bottom.sub")
				button.SetOverVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_bottom.sub")
			else:
				button.SetUpVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_middle.sub")
				button.SetDownVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_middle.sub")
				button.SetOverVisual("d:/ymir work/ui/game/mailbox/friend_list_pattern_middle.sub")

			button.SetEvent(ui.__mem_func__(self.OnClickColorPreset), key)
			button.SetOverEvent(ui.__mem_func__(self.OnColorPresetOver), key)
			button.SetOverOutEvent(ui.__mem_func__(self.OnColorPresetOut), key)
			button.SetListText(colorPresetList[key][:HEX_CODE_LENGTH])
			button.Hide()

			self.colorPresetDict[key] = button

		self.bg2MouseOverImage.SetParent(self.bg2ColorPresetWindow)

	def HexToRGB(self, strValue):
		strValue = strValue.lstrip("#")
		lv = len(strValue)
		rgbCode = (0, 0 ,0)
		try:
			rgbCode = tuple(int(strValue[i:i+int(lv/3)], 16) for i in range(0, lv, int(lv/3)))
		except:
			pass # unvalid hex string

		return rgbCode

	def OnClickColorPreset(self, index):
		for button in self.colorPresetDict.values():
			button.Hide()

		self.__ColorPresetWindow(False)

		skillColorPreset = self.GetSavedColorPresets(self.skillSlot)
		skillColorPresetCode = skillColorPreset[index][:HEX_CODE_LENGTH]

		self.selectedColorPreset = index

		if self.bg2ColorPresetEditLine:
			self.bg2ColorPresetEditLine.SetText(skillColorPresetCode)

		if self.bg2ColorPresetClearButton:
			self.bg2ColorPresetClearButton.Enable()
			self.bg2ColorPresetClearButton.SetUp()

		color = str(skillColorPresetCode).split("#")
		rgbColor = self.HexToRGB(str(color[1]))

		self.rgbColor = rgbColor
		r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)
		self.grpColor = (r, g, b)
		self.grpColorPreview = (r, g, b)
		self.selectedColor = True

		if app.ENABLE_5LAYER_SKILL_COLOR:
			self.rgbLayerColor = {}

			self.confirmButton.Enable()

		if self.colorPickerDotSelectImg:
			self.colorPickerDotSelectImg.Hide()

		if self.bgColorBar:
			self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

	def OnColorPresetOver(self, index):
		if not self.bg2MouseOverImage:
			return

		button = self.colorPresetDict[index]
		if 0 == button:
			return

		skillColorPreset = self.GetSavedColorPresets(self.skillSlot)
		skillColorPresetCode = skillColorPreset[index][:HEX_CODE_LENGTH]
		self.__PreviewColorPreset(skillColorPresetCode)

		(buttonX, buttonY) = button.GetLocalPosition()
		self.bg2MouseOverImage.SetParent(self.bg2ColorPresetWindow)
		self.bg2MouseOverImage.SetPosition(buttonX, buttonY)
		self.bg2MouseOverImage.Show()

	def OnColorPresetOut(self, index):
		if not self.bg2MouseOverImage:
			return

		r, g, b = (0, 0, 0)
		self.grpColorPreview = None
		self.colorPresetPreview = False

		if self.bgColorBar:
			self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

		self.bg2MouseOverImage.Hide()

	def __PreviewColorPreset(self, hexCode):
		if hexCode == "":
			return

		color = str(hexCode).split("#")
		rgbColor = self.HexToRGB(str(color[1]))

		r, g, b = (float(rgbColor[0]) / 255, float(rgbColor[1]) / 255, float(rgbColor[2]) / 255)
		self.grpColorPreview = (r, g, b)
		self.colorPresetPreview = True

		if self.bgColorBar:
			self.bgColorBar.SetColor(grp.GenerateColor(r, g, b, 1.0))

	def OnClickColorPresetButton(self):
		self.__CreateColorPresetButton()

		if self.colorPresetListOpen:
			self.__ColorPresetWindow(False)
		else:
			self.__ColorPresetWindow(True)

	def __ColorPresetWindow(self, isOpen):
		if isOpen:
			self.colorPresetListOpen = True
			if self.bg2ColorPresetWindow:
				self.bg2ColorPresetWindow.SetSize(131, self.colorPresetWindowHeight)

			for button in self.colorPresetDict.values():
				button.Show()
		else:
			self.colorPresetListOpen = False
			if self.bg2ColorPresetWindow:
				self.bg2ColorPresetWindow.SetSize(131, 0)

			for button in self.colorPresetDict.values():
				button.Hide()

	def OnClickPrevButton(self):
		if self.pageIndex <= 1:
			return

		self.pageIndex -= 1
		self.ReloadPage()

	def OnClickNextButton(self):
		if self.pageIndex >= 2:
			return

		self.pageIndex += 1
		self.ReloadPage()

	def ReloadPage(self):
		if self.colorPresetListOpen:
			self.__ColorPresetWindow(False)

		pageIndex = self.pageIndex

		if pageIndex >= 2:
			self.prevPageButton.Show()
			self.nextPageButton.Hide()

			self.bgColorPickerImg.Hide()
			self.bg2Img.Show()

			if app.ENABLE_5LAYER_SKILL_COLOR:
				for button in self.bgColorLayerButton:
					button.Hide()
		else:
			self.prevPageButton.Hide()
			self.nextPageButton.Show()

			self.bgColorPickerImg.Show()
			self.bg2Img.Hide()

			if app.ENABLE_5LAYER_SKILL_COLOR:
				for button in self.bgColorLayerButton:
					button.Show()

	def OnPressEscapeKey(self):
		self.Close()
		return True

	def Close(self):
		self.Hide()

	def Show(self, pageIndex = 1):
		ui.ScriptWindow.Show(self)

		if not self.isLoaded:
			self.pageIndex = pageIndex
			self.__LoadWindow()

		self.SetCenterPosition()
		self.SetTop()
