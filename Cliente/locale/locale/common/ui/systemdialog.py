import uiScriptLocale
import localeInfo
import app

ROOT = "d:/ymir work/ui/public/"
VERSION_ADD_Y = 10

BOARD_SIZE_X = 200
BOARD_SIZE_Y = 358

window = {
	"name" : "SystemDialog",
	"style" : ("float",),

	"x" : (SCREEN_WIDTH  - BOARD_SIZE_X) /2,
	"y" : (SCREEN_HEIGHT - (BOARD_SIZE_Y - 30)) /2,

	"width" : BOARD_SIZE_X,
	"height" : BOARD_SIZE_Y + VERSION_ADD_Y,

	"children" :
	(
		{
			"name" : "board",
			"type" : "thinboard",

			"x" : 0,
			"y" : 0,

			"width" : BOARD_SIZE_X,
			"height" : BOARD_SIZE_Y - 30 + VERSION_ADD_Y,

			"children" :
			(
				{
					"name" : "help_button",
					"type" : "button",

					"x" : 10,
					"y" : 17,

					"text" : uiScriptLocale.SYSTEM_HELP,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "version",
					"type" : "text",

					"x" : 0,
					"y" : 47,

					"text" : localeInfo.FormatVersion(app.VERSION),
					"horizontal_align" : "center",
					"text_horizontal_align" : "center",
				},
				{
					"name" : "mall_button",
					"type" : "button",

					"x" : 10,
					"y" : 57 + VERSION_ADD_Y,

					"text" : uiScriptLocale.SYSTEM_MALL,
					"text_color" : 0xffF8BF24,

					"default_image" : ROOT + "XLarge_Button_02.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_02.sub",
				},

				{
					"name" : "system_option_button",
					"type" : "button",

					"x" : 10,
					"y" : 87 + VERSION_ADD_Y,

					"text" : uiScriptLocale.SYSTEMOPTION_TITLE,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "game_option_button",
					"type" : "button",

					"x" : 10,
					"y" : 117 + VERSION_ADD_Y,

					"text" : uiScriptLocale.GAMEOPTION_TITLE,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "change_button",
					"type" : "button",

					"x" : 10,
					"y" : 147 + VERSION_ADD_Y,

					"text" : uiScriptLocale.SYSTEM_CHANGE,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "logout_button",
					"type" : "button",

					"x" : 10,
					"y" : 207 - 30 + VERSION_ADD_Y,

					"text" : uiScriptLocale.SYSTEM_LOGOUT,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "movechannel_button",
					"type" : "button",

					"x" : 10,
					"y" : 177 + VERSION_ADD_Y,

					"text" : uiScriptLocale.SYSTEM_MOVE_CHANNEL,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "logout_button",
					"type" : "button",

					"x" : 10,
					"y" : 237 - 30 + VERSION_ADD_Y,

					"text" : uiScriptLocale.SYSTEM_LOGOUT,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				#{
				#	"name" : "textline",
				#	"type" : "text",
				#
				#	"x" : 0,
				#	"y" : 267 - 30 + VERSION_ADD_Y,
				#
				#	"text" : "",
				#	"horizontal_align" : "center",
				#	"text_horizontal_align" : "center",
				#},
				{
					"name" : "exit_button",
					"type" : "button",

					"x" : 10,
					"y" : 287 - 30 + VERSION_ADD_Y,

					"text" : uiScriptLocale.SYSTEM_EXIT,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
				{
					"name" : "cancel_button",
					"type" : "button",

					"x" : 10,
					"y" : 317 - 30 + VERSION_ADD_Y,

					"text" : uiScriptLocale.CANCEL,

					"default_image" : ROOT + "XLarge_Button_01.sub",
					"over_image" : ROOT + "XLarge_Button_02.sub",
					"down_image" : ROOT + "XLarge_Button_03.sub",
				},
			),
		},
	),
}
