import uiScriptLocale
import app

window = {
	"x" : 0,
	"y" : 0,

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" :
	[
		## Board
		{
			"name" : "BackGround",
			"type" : "expanded_image",

			"x" : 0,
			"y" : 0,

			"image" : "d:/ymir work/ui/intro/pattern/Line_Pattern.tga",

			"x_scale" : float(SCREEN_WIDTH) / 800.0,
			"y_scale" : float(SCREEN_HEIGHT) / 600.0,
		},
		{
			"name" : "ErrorMessage",
			"type" : "text", "x" : 10, "y" : 10,
			"text" : uiScriptLocale.LOAD_ERROR,
		},
		{
			"name" : "GageBoard",
			"type" : "window",
			"style" : ("ltr",),

			"x" : float(SCREEN_WIDTH) * 400 / 800.0 - 200,
			"y" : float(SCREEN_HEIGHT) * 500 / 600.0,

			"width" : 400,
			"height" : 80,

			"children" :
			(
				{
					"name" : "BackGage",
					"type" : "expanded_image",

					"x" : 40,
					"y" : 25,

					"image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "loading/gauge_empty.dds",
				},
				{
					"name" : "FullGage",
					"type" : "expanded_image",

					"x" : 40,
					"y" : 25,

					"image" : uiScriptLocale.LOCALE_UISCRIPT_PATH + "loading/gauge_full.dds",
				},
			),
		},
	],
}

if app.ENABLE_LOADING_TIP:
	window["children"] = window["children"] + [
		{
			"name" : "TipBackground",
			"type" : "middleboard",

			"x" : 0,
			"y" : float(SCREEN_HEIGHT) * 500 / 600.0 - 100,

			"width" : SCREEN_WIDTH,
			"height" : 100,

			"children" :
			(
				{
					"name" : "LoadingTip",
					"type" : "text",

					"x" : float(SCREEN_WIDTH) / 2,
					"y" : 40,

					"text" : " ",
					"text_horizontal_align" : "center",
					"fontsize" : "LARGE",
				},
			),
		},
	]
