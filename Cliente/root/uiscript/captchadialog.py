#
# File: CaptchaDialog.py
# Date: 2019.11.17
# Author: Owsap
#

import uiScriptLocale

window = {
	"name" : "CaptchaDialog",

	"x" : 0,
	"y" : 0,

	#"style" : ("movable", "float",),
	"style" : ("attach",),

	"width" : SCREEN_WIDTH,
	"height" : SCREEN_HEIGHT,

	"children" :
	(
		{
			"name" : "thin_board",
			"type" : "thinboard",

			"x" : 0,
			"y" : 0,

			"width" : SCREEN_WIDTH,
			"height" : SCREEN_HEIGHT,
		},
		{
			"name" : "board",
			"type" : "board",

			"x" : 0,
			"y" : 0,

			"width" : 150,
			"height" : 125,

			"title" : uiScriptLocale.CAPTCHA,

			"horizontal_align" : "center",
			"vertical_align" : "center",

			"children" :
			(
				{
					"name" : "thin_board",
					"type" : "thinboard",
					"style" : ("attach",),

					"x" : 10,
					"y" : 31,

					"width" : 150 - 20,
					"height" : 125 - 38,
				},

				{
					"name" : "title_bar",
					"type" : "titlebar",
					"style" : ("attach",),

					"x" : 8,
					"y" : 8,
 
					"width" : 134,
					"color" : "yellow",

					"children" :
					(
						{
							"name" : "title_name",
							"type" : "text",

							"x" : 0,
							"y" : 3,

							"horizontal_align" : "center",
							"text" : uiScriptLocale.CAPTCHA,
							"text_horizontal_align":"center"
						},
					),
				},

				## Random Captcha Slot
				{
					"name" : "random_captcha_slot",
					"type" : "image",

					"x" : 20,
					"y" : 34,

					"image" : "d:/ymir work/ui/game/user_alram/bg.sub",

					"children" :
					(
						{
							"name" : "random_captcha_info",
							"type" : "button",

							"x" : 2,
							"y" : 3,

							"default_image" : "d:/ymir work/ui/pattern/q_mark_01.tga",
							"over_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
							"down_image" : "d:/ymir work/ui/pattern/q_mark_02.tga",
						},
						{
							"name" : "random_captcha_name",
							"type" : "text",

							"x" : 25,
							"y" : 3,

							"width" : 60,
							"height" : 18,

							"text" : "",
							#"horizontal_align" : "center",
							#"text_horizontal_align":"center"
						},
						{
							"name" : "captcha_time_image",
							"type" : "expanded_image",

							"x" : 65,
							"y" : 3,

							"image" : "d:/ymir work/ui/game/premium_private_shop/sandglass_icon.sub",
						},
						{
							"name" : "captcha_time_name",
							"type" : "text",

							"x" : 85,
							"y" : 3,

							"text" : "00:00",
						},
					),
				},

				## Captcha Slot
				{
					"name" : "captcha_slot",
					"type" : "image",

					"x" : 70,
					"y" : 34 + 25,

					"image" : "d:/ymir work/ui/public/Parameter_Slot_02.sub",

					"children" :
					(
						{
							"name" : "captcha_name",
							"type" : "text",

							"x" : -50,
							"y" : 3,
							"text" : uiScriptLocale.CAPTCHA + ":",
						},
						{
							"name" : "captcha_value",
							"type" : "editline",

							"x" : 3,
							"y" : 3,

							"width" : 150,
							"height" : 18,

							"input_limit" : 4,
							"text" : "",
						},
					),
				},

				## Buttons
				{
					"name" : "accept_button",
					"type" : "button",

					"x" : -30,
					"y" : 88,
					"tooltip_text" : uiScriptLocale.CAPTCHA_CONFIRM,
					"tooltip_x" : 0,
					"tooltip_y" : -13,
					"horizontal_align" : "center",

					"default_image" : "d:/ymir work/ui/public/acceptbutton00.sub",
					"over_image" : "d:/ymir work/ui/public/acceptbutton01.sub",
					"down_image" : "d:/ymir work/ui/public/acceptbutton02.sub",
				},
				{
					"name" : "cancel_button",
					"type" : "button",

					"x" : 30,
					"y" : 88,
					"tooltip_text" : uiScriptLocale.CAPTCHA_CANCEL,
					"tooltip_x" : 0,
					"tooltip_y" : -13,
					"horizontal_align" : "center",

					"default_image" : "d:/ymir work/ui/public/canclebutton00.sub",
					"over_image" : "d:/ymir work/ui/public/canclebutton01.sub",
					"down_image" : "d:/ymir work/ui/public/canclebutton02.sub",
				},
			),
		},
	),
}