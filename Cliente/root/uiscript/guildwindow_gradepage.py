import uiScriptLocale
import app

ROOT_DIR = "d:/ymir work/ui/game/guild/guildgradepage/"
GUILD_PATH = uiScriptLocale.GUILD_PATH

window = {
	"name" : "GuildWindow_BoardPage",

	"x" : 8,
	"y" : 30,

	"width" : 360,
	"height" : 298,

	"children" :
	(
				
		## GuildGradeTItle
		{
			"name" : "GuildGradeTItle", "type" : "image", "x" : 3, "y" : 1, "image" : GUILD_PATH+"title.sub",
		},
		## GradeNumber
		{
			"name" : "gradenumber", "type" : "image", "x" : 21, "y" : 5, "image" : ROOT_DIR+"gradenumber.sub",
		},
		## GradeName
		{
			"name" : "gradename", "type" : "image", "x" : 76, "y" : 5, "image" : ROOT_DIR+"gradename.sub",
		},
		## InviteAuthority
		{
			"name" : "inviteauthority", "type" : "image", "x" : 126, "y" : 5, "image" : ROOT_DIR+"inviteauthority.sub",
		},
		## DriveOutAuthority
		{
			"name" : "driveoutauthority", "type" : "image", "x" : 181-28, "y" : 5, "image" : ROOT_DIR+"driveoutauthority.sub",
		},
		## NoticeAuthority
		{
			"name" : "noticeauthority", "type" : "image", "x" : 238-44, "y" : 5, "image" : ROOT_DIR+"noticeauthority.sub",
		},
		## SkillAuthority
		{
			"name" : "skillauthority", "type" : "image", "x" : 295-50, "y" : 5, "image" : ROOT_DIR+"skillauthority.sub",
		},
		## GuildWar
		{
			"name" : "GuildWar", "type" : "image", "x" : 295-57+60, "y" : 5, "image" : ROOT_DIR+"guildwar.sub",
		},
	),
}
