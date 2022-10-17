import uiScriptLocale
import item

EQUIPMENT_START_INDEX = 90
BOARD_WIDTH = 148
BOARD_HIGHT = 139

window = {
	"name" : "BeltInventoryWindow",
	
	"x" : SCREEN_WIDTH - 176 - 148,
	"y" : SCREEN_HEIGHT - 37 - 565 + 209 + 32,

	"style" : ("float",),

	##"style" : ("attach",),
	
	"width" : BOARD_WIDTH,
	"height" : BOARD_HIGHT,
	
	"children" :
	(
		## Expand Buttons
		{
			"name" : "ExpandBtn",
			"type" : "button",

			"x" : 2,
			"y" : 15,

			"default_image" : "d:/ymir work/ui/game/belt_inventory/btn_expand_normal.tga",
			"over_image" : "d:/ymir work/ui/game/belt_inventory/btn_expand_over.tga",
			"down_image" : "d:/ymir work/ui/game/belt_inventory/btn_expand_down.tga",
			"disable_image" : "d:/ymir work/ui/game/belt_inventory/btn_expand_disabled.tga",
		},

		## Belt Inventory Layer (include minimize button)
		{
			"name" : "BeltInventoryLayer",
			#"type" : "board",
			#"style" : ("attach", "float"),

			"x" : 0,
			"y" : 0,

			"width" : 148,
			"height" : 139,

			"children" :
			(
				## Minimize Button
				{
					"name" : "MinimizeBtn",
					"type" : "button",

					"x" : 2,
					"y" : 15,

					"width" : 10,

					"default_image" : "d:/ymir work/ui/game/belt_inventory/btn_minimize_normal.tga",
					"over_image" : "d:/ymir work/ui/game/belt_inventory/btn_minimize_over.tga",
					"down_image" : "d:/ymir work/ui/game/belt_inventory/btn_minimize_down.tga",
					"disable_image" : "d:/ymir work/ui/game/belt_inventory/btn_minimize_disabled.tga",
				},
				## Iamge
				{
					"name" : "beltbackimage",
					"type" : "image",

					"x" : 12,
					"y" : 0,
			
					"width" : 148,
					"height" : 139,

					"image" : "d:/ymir work/ui/game/belt_inventory/bg.tga",

					"children" :
					(
						## Belt Inventory Slots
						{
							"name" : "BeltInventorySlot",
							"type" : "grid_table",

							"x" : 7,
							"y" : 5,

							"start_index" : item.BELT_INVENTORY_SLOT_START,
							"x_count" : 4,
							"y_count" : 4,
							"x_step" : 32,
							"y_step" : 32,

							"image" : "d:/ymir work/ui/public/Slot_Base.sub"
						},
					),
				},
			),
		},
	),
}
