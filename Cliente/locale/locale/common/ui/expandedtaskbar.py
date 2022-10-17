import uiScriptLocale
import app

ROOT = "d:/ymir work/ui/game/"

Y_ADD_POSITION = 0
window = {
	"name" : "ExpandTaskBar",
	"style" : ("ltr", ),

	"x" : SCREEN_WIDTH/2 - 5,
	"y" : SCREEN_HEIGHT - 74,

	"width" : 37,
	"height" : 37,

	"children" :
	[
		{
			"name" : "ExpanedTaskBar_Board",
			"type" : "window",
			"style" : ("ltr", ),

			"x" : 0,
			"y" : 0,

			"width" : 37,
			"height" : 37,

			"children" :
			[
				{
					"name" : "DragonSoulButton",
					"type" : "button",
					"style" : ("ltr", ),

					"x" : 0,
					"y" : 0,

					"width" : 37,
					"height" : 37,

					"tooltip_text" : uiScriptLocale.TASKBAR_DRAGON_SOUL,

					"default_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_01.tga",
					"over_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_02.tga",
					"down_image" : "d:/ymir work/ui/dragonsoul/DragonSoul_Button_03.tga",
				},
			],
		},
	],
}

if app.ENABLE_SPECIAL_INVENTORY_SYSTEM:
	window["width"] = 37 * 2
	window["children"][0]["width"] = window["children"][0]["width"] + 37
	window["children"][0]["children"] = window["children"][0]["children"] + [
		{
			"name" : "SpecialInventoryButton",
			"type" : "button",
			"style" : ("ltr", ),

			"x" : 38,
			"y" : 0,

			"width" : 37,
			"height" : 37,

			"tooltip_text" : uiScriptLocale.TASKBAR_SPECIAL_INVENTORY,

			"default_image" : "d:/ymir work/ui/game/taskbar/inventory_button_01.sub",
			"over_image" : "d:/ymir work/ui/game/taskbar/inventory_button_02.sub",
			"down_image" : "d:/ymir work/ui/game/taskbar/inventory_button_03.sub",
		},
	]
