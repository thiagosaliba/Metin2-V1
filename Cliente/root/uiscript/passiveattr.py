import uiScriptLocale
import localeInfo

ROOT = "d:/ymir work/ui/game/passive_attr/"
PUBLIC_PATH = "d:/ymir work/ui/public/"

WINDOW_WIDTH	= 200
WINDOW_HEIGHT	= 274

BG_START_X		= 13
BG_START_Y		= 35

BG_WIDTH		= 172
BG_HEIGHT		= 172

SLOT_INDEX_JOB		= 0		#m2netm2g.PASSIVE_ATTR_SLOT_INDEX_JOB
SLOT_INDEX_WEAPON	= 1		#m2netm2g.PASSIVE_ATTR_SLOT_INDEX_WEAPON
SLOT_INDEX_ELEMENT	= 2		#m2netm2g.PASSIVE_ATTR_SLOT_INDEX_ELEMENT
SLOT_INDEX_ARMOR	= 3		#m2netm2g.PASSIVE_ATTR_SLOT_INDEX_ARMOR
SLOT_INDEX_ACCE		= 4		#m2netm2g.PASSIVE_ATTR_SLOT_INDEX_ACCE

window = {
	"name" : "PassiveAttrUI",
	"style" : ("movable", "float",),
	
	"x" : SCREEN_WIDTH / 2 - WINDOW_WIDTH / 2,
	"y" : SCREEN_HEIGHT / 2 - WINDOW_HEIGHT / 2,
	
	"width" : WINDOW_WIDTH,
	"height" : WINDOW_HEIGHT,

	"children" :
	(
		{
			"name" : "board",
			"type" : "board_with_titlebar",
			
			"x" : 0,
			"y" : 0,
			
			"width" : WINDOW_WIDTH,
			"height" : WINDOW_HEIGHT,
			
			"title" : uiScriptLocale.PASSIVE_ATTR_UI_TITLE,

			"children" :
			(
				## bg
				{
					"name" : "bg",
					"type" : "image",
					
					"x" : BG_START_X,
					"y" : BG_START_Y,
					"image" : ROOT + "bg.sub",
					
					"children" :
					(
						## sub slot
						{
							"name" : "passive_attr_sub_slot",
							"type" : "slot",
					
							"x" : 0,
							"y" : 0,
					
							"width" : BG_WIDTH,
							"height" : BG_HEIGHT,
					
							"slot" : (
								{"index":SLOT_INDEX_WEAPON,		"x":83-BG_START_X,	"y":48-BG_START_Y,	"width":32, "height":32},	# ��( ���� )
								{"index":SLOT_INDEX_ELEMENT,	"x":83-BG_START_X,	"y":162-BG_START_Y,	"width":32, "height":32},	# ��( ������Ż )
								{"index":SLOT_INDEX_ARMOR,		"x":26-BG_START_X,	"y":105-BG_START_Y,	"width":32, "height":32},	# ��( �� )
								{"index":SLOT_INDEX_ACCE,		"x":140-BG_START_X,	"y":105-BG_START_Y,	"width":32, "height":32},	# ��( �׼����� )
							),
						},
						## passive job up slot
						{
							"name" : "passive_job_up_slot",
							"type" : "slot",
					
							"x" : 83-BG_START_X,
							"y" : 105-BG_START_Y,
					
							"width" : 32,
							"height" : 32,
					
							"slot" : (
								{"index":SLOT_INDEX_JOB, "x":0,	"y":0,	"width":32, "height":32},	# �߾�( ���� ���� )
							),
						},
						## passive job down slot
						{
							"name" : "passive_job_down_slot",
							"type" : "slot",
					
							"x" : 83-BG_START_X,
							"y" : 105-BG_START_Y,
					
							"width" : 32,
							"height" : 32,
					
							"slot" : (
								{"index":SLOT_INDEX_JOB, "x":0,	"y":0,	"width":32, "height":32},	# �߾�( ���� ���� )
							),
						},
						
						## �� ��ư
						{
							"name" : "deck_button1",
							"type" : "radio_button",

							"x" : 19 - BG_START_X,
							"y" : 169 - BG_START_Y,

							"default_image" : ROOT + "up_button_default.sub",
							"over_image" : ROOT + "up_button_over.sub",
							"down_image" : ROOT + "up_button_down.sub",
						},
						
						## �� ��ư
						{
							"name" : "deck_button2",
							"type" : "radio_button",

							"x" : 147 - BG_START_X,
							"y" : 169 - BG_START_Y,

							"default_image" : ROOT + "down_button_default.sub",
							"over_image" : ROOT + "down_button_over.sub",
							"down_image" : ROOT + "down_button_down.sub",
						},
					),
				},
				
				## ���� ��ư
				{
					"name" : "ChargeButton",
					"type" : "button",

					"x" : 12,
					"y" : 213,

					"default_image" : PUBLIC_PATH + "large_button_01.sub",
					"over_image" : PUBLIC_PATH + "large_button_02.sub",
					"down_image" : PUBLIC_PATH + "large_button_03.sub",

					"text" : uiScriptLocale.PASSIVE_ATTR_UI_CHARGE_BUTTON,
				},

				## �ο� ��ư
				{
					"name" : "AddButton",
					"type" : "button",

					"x" : 100,
					"y" : 213,

					"default_image" : PUBLIC_PATH + "large_button_01.sub",
					"over_image" : PUBLIC_PATH + "large_button_02.sub",
					"down_image" : PUBLIC_PATH + "large_button_03.sub",

					"text" : uiScriptLocale.PASSIVE_ATTR_UI_ADD_BUTTON,
				},
				
				## Ȱ��ȭ ��ư
				{
					"name" : "ActivateButton",
					"type" : "toggle_button",

					"x" : 100,
					"y" : 239,

					"default_image" : PUBLIC_PATH + "large_button_01.sub",
					"over_image" : PUBLIC_PATH + "large_button_02.sub",
					"down_image" : PUBLIC_PATH + "large_button_03.sub",

					"text" : uiScriptLocale.PASSIVE_ATTR_ACTIVATE_BUTTON,
				},
			),
		},
	),
}