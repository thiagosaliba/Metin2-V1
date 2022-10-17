import app

ROOT = "d:/ymir work/ui/minimap/"

window = {
	"name" : "MiniMap",

	"x" : SCREEN_WIDTH - 136,
	"y" : 0,

	"width" : 136,
	"height" : 137,

	"children" :
	[
		## OpenWindow
		{
			"name" : "OpenWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 136,
			"height" : 137,

			"children" :
			[
				{
					"name" : "OpenWindowBGI",
					"type" : "image",
					"x" : 0,
					"y" : 0,
					"image" : ROOT + "minimap.sub",
				},
				## MiniMapWindow
				{
					"name" : "MiniMapWindow",
					"type" : "window",

					"x" : 4,
					"y" : 5,

					"width" : 128,
					"height" : 128,
				},
				## ScaleUpButton
				{
					"name" : "ScaleUpButton",
					"type" : "button",

					"x" : 101,
					"y" : 116,

					"default_image" : ROOT + "minimap_scaleup_default.sub",
					"over_image" : ROOT + "minimap_scaleup_over.sub",
					"down_image" : ROOT + "minimap_scaleup_down.sub",
				},
				## ScaleDownButton
				{
					"name" : "ScaleDownButton",
					"type" : "button",

					"x" : 115,
					"y" : 103,

					"default_image" : ROOT + "minimap_scaledown_default.sub",
					"over_image" : ROOT + "minimap_scaledown_over.sub",
					"down_image" : ROOT + "minimap_scaledown_down.sub",
				},
				## MiniMapHideButton
				{
					"name" : "MiniMapHideButton",
					"type" : "button",

					"x" : 111,
					"y" : 6,

					"default_image" : ROOT + "minimap_close_default.sub",
					"over_image" : ROOT + "minimap_close_over.sub",
					"down_image" : ROOT + "minimap_close_down.sub",
				},
				## AtlasShowButton
				{
					"name" : "AtlasShowButton",
					"type" : "button",

					"x" : 12,
					"y" : 12,

					"default_image" : ROOT + "atlas_open_default.sub",
					"over_image" : ROOT + "atlas_open_over.sub",
					"down_image" : ROOT + "atlas_open_down.sub",
				},
				## ServerInfo
				{
					"name" : "ServerInfo",
					"type" : "text",

					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 140,

					"text" : "",
				},
				## PositionInfo
				{
					"name" : "PositionInfo",
					"type" : "text",

					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 160,

					"text" : "",
				},
				## ObserverCount
				{
					"name" : "ObserverCount",
					"type" : "text",

					"text_horizontal_align" : "center",

					"outline" : 1,

					"x" : 70,
					"y" : 180,

					"text" : "",
				},
			],
		},
		{
			"name" : "CloseWindow",
			"type" : "window",

			"x" : 0,
			"y" : 0,

			"width" : 132,
			"height" : 48,

			"children" :
			[
				## ShowButton
				{
					"name" : "MiniMapShowButton",
					"type" : "button",

					"x" : 100,
					"y" : 4,

					"default_image" : ROOT + "minimap_open_default.sub",
					"over_image" : ROOT + "minimap_open_default.sub",
					"down_image" : ROOT + "minimap_open_default.sub",
				},
			],
		},
	],
}

window["children"][0]["children"] = window["children"][0]["children"] + [
	## InGameEventButton
	{
		"name" : "DungeonInfoShowButton",
		"type" : "button",

		"x" : 80,
		"y" : 120,

		"default_image" : ROOT + "minimap_dungeon_info_default.tga",
		"over_image" : ROOT + "minimap_dungeon_info_over.tga",
		"down_image" : ROOT + "minimap_dungeon_info_down.tga",
	},
]

if app.ENABLE_OSF_MINIMAP_INFO:
	window["children"][0]["children"] = window["children"][0]["children"] + [
		{
			"name" : "DateTimeTextLine",
			"type" : "text",

			"x" : 0,
			"y" : 160,

			"text" : app.GetCurrentTime(),
			"horizontal_align" : "center",
			"text_horizontal_align" : "center",
		},
		{
			"name" : "TimeZoneTextLine",
			"type" : "text",

			"x" : 0,
			"y" : 160 + 15,

			"text" : app.GetTimeZone(),
			"horizontal_align" : "center",
			"text_horizontal_align" : "center",
		},
		{
			"name" : "PatchTextLine",
			"type" : "text",

			"x" : 0,
			"y" : 160 + 35,

			"text" : app.GetPatch(),
			"horizontal_align" : "center",
			"text_horizontal_align" : "center",
		},
	]

if app.ENABLE_RTT_STATISTICS:
	window["children"][0]["children"] = window["children"][0]["children"] + [
		{
			"name" : "RTTTextLine",
			"type" : "text",

			"x" : 0,
			"y" : 160 + 55,

			"horizontal_align" : "center",
			"text_horizontal_align" : "center",

			"text" : "",
			"outline" : 1,
		},
		{
			"name" : "PacketLossTextLine",
			"type" : "text",

			"x" : 0,
			"y" : 160 + 55 + 15,

			"horizontal_align" : "center",
			"text_horizontal_align" : "center",

			"text" : "",
			"outline" : 1,
		},
	]

#window["children"][0]["children"] = window["children"][0]["children"] + [
#	## Party Match
#	{
#		"name" : "PartyMatchButton",
#		"type" : "button",
#
#		"x" : 4,
#		"y" : 85,
#
#		"default_image" : ROOT + "party_match/0.sub",
#		"over_image" : ROOT + "party_match/0.sub",
#		"down_image" : ROOT + "party_match/0.sub",
#	},
#	{
#		"name" : "PartyMatchEffect",
#		"type" : "ani_image",
#
#		"x" : 4,
#		"y" : 85,
#
#		"delay" : 6,
#
#		"images" :
#		(
#			ROOT + "party_match/0.sub",
#			ROOT + "party_match/1.sub",
#			ROOT + "party_match/2.sub",
#			ROOT + "party_match/3.sub",
#			ROOT + "party_match/4.sub",
#			ROOT + "party_match/5.sub",
#			ROOT + "party_match/6.sub",
#			ROOT + "party_match/7.sub",
#			ROOT + "party_match/7.sub",
#			ROOT + "party_match/6.sub",
#			ROOT + "party_match/5.sub",
#			ROOT + "party_match/4.sub",
#			ROOT + "party_match/3.sub",
#			ROOT + "party_match/2.sub",
#			ROOT + "party_match/1.sub",
#			ROOT + "party_match/0.sub",
#		),
#	},
#]

if app.ENABLE_MAILBOX:
	window["children"][0]["children"] = window["children"][0]["children"] + [
		## MailBox
		{
			"name" : "MailBoxButton",
			"type" : "button",

			"x" : 110,
			"y" : 30,

			"default_image" : "d:/ymir work/ui/game/mailbox/post_minimap.sub",
			"over_image" : "d:/ymir work/ui/game/mailbox/post_minimap.sub",
			"down_image" : "d:/ymir work/ui/game/mailbox/post_minimap.sub",
		},
		{
			"name" : "MailBoxEffect",
			"type" : "ani_image",

			"x" : 110,
			"y" : 30,

			"delay" : 6,

			"images" :
			(
				"d:/ymir work/ui/game/mailbox/minimap_flash/2.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/3.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/4.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/5.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/4.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/3.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/2.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
				"d:/ymir work/ui/game/mailbox/minimap_flash/1.sub",
			),
		},
	]

#window["children"][0]["children"] = window["children"][0]["children"] + [
#	## InGameEventButton
#	{
#		"name" : "InGameEventButton",
#		"type" : "button",
#
#		"x" : 2,
#		"y" : 28,
#
#		"default_image" : "d:/ymir work/ui/minimap/E_open_default.tga",
#		"over_image" : "d:/ymir work/ui/minimap/E_open_over.tga",
#		"down_image" : "d:/ymir work/ui/minimap/E_open_down.tga",
#	},
#]
