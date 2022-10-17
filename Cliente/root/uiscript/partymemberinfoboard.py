import uiScriptLocale
import app

ROOT = "d:/ymir work/ui/public/"

window = {
	"name" : "PartyMemeberInfoBoard",

	"x" : 0,
	"y" : 0,

	"width" : 106,
	"height" : 36,

	"children" :
	(

		{
			"name" : "StateButton",
			"type" : "button",

			"x" : 0,
			"y" : 0,

			"default_image" : "d:/ymir work/ui/game/windows/party_state_normal_01.sub",
			"over_image" : "d:/ymir work/ui/game/windows/party_state_normal_02.sub",
			"down_image" : "d:/ymir work/ui/game/windows/party_state_normal_03.sub",
		},

		{
			"name" : "NameSlot",
			"type" : "bar",
			"style" : ("not_pick",),

			"x" : 22,
			"y" : 0,
			"width" : 84,
			"height" : 17,
			"color" : 0x99000000,

			"children" :
			(
				{
					"name" : "NamePrint",
					"type" : "text",

					"x" : 3,
					"y" : 2,

					"text" : uiScriptLocale.PARTY_MEMBER_INFO_NAME,
				},
			),
		},

		{
			"name" : "Gauge",
			"type" : "gauge",
			"style" : ("not_pick",),

			"x" : 22,
			"y" : 17,
			"width" : 84,
			"color" : "red",
		},
		## ����ġ ���ʽ�
		{
			"name" : "ExperienceImage",
			"type" : "image",

			"x" : 22,
			"y" : 24,

			"image" : "d:/ymir work/ui/game/windows/party_affect_experience.sub",
		},
		## ����Ŀ ���ʽ�
		{
			"name" : "AttackerImage",
			"type" : "image",

			"x" : 34,
			"y" : 24,

			"image" : "d:/ymir work/ui/game/windows/party_affect_attackgrade.sub",
		},
		## ����Ŀ ���ʽ�
		{
			"name" : "TimeBonusImage",
			"type" : "image",

			"x" : 46,
			"y" : 24,

			"image" : "d:/ymir work/ui/game/windows/party_affect_timebonus.sub",
		},
		## ��Ŀ ���ʽ�
		{
			"name" : "DefenderImage",
			"type" : "image",

			"x" : 58,
			"y" : 24,

			"image" : "d:/ymir work/ui/game/windows/party_affect_defencegrade.sub",
		},
		## ���� ���ʽ�
		{
			"name" : "BufferImage",
			"type" : "image",

			"x" : 70,
			"y" : 24,

			"image" : "d:/ymir work/ui/game/windows/party_affect_bufferbonus.sub",
		},
		## ��ų������ ���ʽ�
		{
			"name" : "SkillMasterImage",
			"type" : "image",

			"x" : 82,
			"y" : 24,

			"image" : "d:/ymir work/ui/game/windows/party_affect_skillmasterbonus.sub",
		},
		## ���Ҵ� ���ʽ�
		{
			"name" : "RegenBonus",
			"type" : "image",

			"x" : 94,
			"y" : 24,

			"image" : "d:/ymir work/ui/game/windows/party_affect_regenbonus.sub",
		},
		
		{
			"name" : "IncreaseArea150",
			"type" : "image",

			"x" : 82,
			"y" : 24,

			"image" : "d:/ymir work/ui/game/windows/party_affect_increasearea_150.sub",
		},
		{
			"name" : "IncreaseArea200",
			"type" : "image",

			"x" : 94,
			"y" : 24,

			"image" : "d:/ymir work/ui/game/windows/party_affect_increasearea_200.sub",
		},

	),
}
