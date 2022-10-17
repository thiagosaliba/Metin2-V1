#if !defined(__INC_COMMON_SERVICE_H__)
#define __INC_COMMON_SERVICE_H__

#define __LOCALE_SERVICE_EUROPE__
#define __LOCALE_SERVICE_OWSAP__

////////////////////////////////////////////////////////////////////////////////
// Currency Related
#define __CHEQUE_SYSTEM__ // Cheque (Won) System
#define __GEM_SYSTEM__ // Gem (Gaya) System
#if defined(__GEM_SYSTEM__)
#	define __GEM_MARKET_SYSTEM__ // Gem (Gaya) Market System
#endif

////////////////////////////////////////////////////////////////////////////////
// Minigame Related
#define /* @author: VegaS */ __MINI_GAME_OKEY__ // Minigame Rumi (Okey)
#define /* @author: ZeNu */ __MINI_GAME_CATCH_KING__ // Minigame Catch King

////////////////////////////////////////////////////////////////////////////////
// Dragon Soul Related
#define __DS_GRADE_MYTH__ // Dragon Soul Mythical Grade
#define __DS_SET__ // Dragon Soul Table Handling
#define /* @author: Owsap */ __DS_CHANGE_ATTR__ // Dragon Soul Change Attribute
#define __EXTENDED_DSS_RECHARGE__ // [ITEM] Dragon Soul Extended Recharge

////////////////////////////////////////////////////////////////////////////////
// Costume Related
#define __COSTUME_SYSTEM__ // Costume System
#define __MOUNT_COSTUME_SYSTEM__ // Mount Costume System
#define __ACCE_COSTUME_SYSTEM__ // Acce (Sash) Costume System
#define __WEAPON_COSTUME_SYSTEM__ // Weapon Costume System
#define __COSTUME_ATTR_SYSTEM__ // [ITEM] Costume Attributes System
#define /* @author: Owsap */ __EXTENDED_COSTUME_RECHARGE__ // [ITEM] Costume Extended Recharge
#define /* @author: Owsap */ __HIDE_COSTUME_SYSTEM__ // Hide Costume System
#define __AURA_COSTUME_SYSTEM__ // Aura Costume System (Proto Support Only)

////////////////////////////////////////////////////////////////////////////////
// Inventory & Equipment Related
#define __SWAP_ITEM_SYSTEM__ // Swap Item Slot
#define __INVENTORY_4PAGES__ // Extended Inventory Pages (4)
#define __SPECIAL_INVENTORY_SYSTEM__ // Special Inventory System
//#define __SORT_INVENTORY_ITEMS__ // Sort Inventory Items
#define __QUIVER_SYSTEM__ // Quiver Equipement
#define __PENDANT_SYSTEM__ // Talisman Elements
#define __GLOVE_SYSTEM__ // Glove Equipement

////////////////////////////////////////////////////////////////////////////////
// Messenger Related
#define __MESSENGER_BLOCK_SYSTEM__ // Messenger Block System
#define __MESSENGER_GM__ // Messenger GM List

////////////////////////////////////////////////////////////////////////////////
// Shop Related
#define /* @author: blackdragonx61 */ __SHOPEX_RENEWAL__ // ShopEX Renewal
#if defined(__SHOPEX_RENEWAL__)
#	define __SHOPEX_TAB4__ // ShopEx 4 Tabs
#endif
#define /* @author: Owsap */ __MYSHOP_DECO__ // Private Shop Decoration
#define /* @author: blackdragonx61 */ __PRIVATESHOP_SEARCH_SYSTEM__ // Private Shop Search System

////////////////////////////////////////////////////////////////////////////////
// Player Related
#define __VIEW_TARGET_DECIMAL_HP__ // View Target HP
#if defined(__VIEW_TARGET_DECIMAL_HP__)
#	define __VIEW_TARGET_PLAYER_HP__ // View Player Target HP
#endif
#define __PLAYER_PER_ACCOUNT5__ // Players Per Account (5)
#define __WOLFMAN_CHARACTER__ // Wolfman Character
#if defined(__WOLFMAN_CHARACTER__)
//#	define __DISABLE_WOLFMAN_CREATION__ // Disable Wolfman Creation
#endif
#define /* @author: Owsap */ __PLAYER_PIN_SYSTEM__ // Player PIN Code
#if defined(__PLAYER_PIN_SYSTEM__)
//#	define __DISABLE_PIN_SYSTEM__ // Disable Player PIN Code Input
#endif
#define __GUILD_LEADER_GRADE_NAME__ // Guild Leader Grade Name (TextTail)
#define /* @auhtor: VegaS */ __GENDER_ALIGNMENT__ // Gender Alignment (M, F)
#define /* @author: blackdragonx61 */ __IMPROVED_LOGOUT_POINTS__ // Improved Logout Points
#define /* @author: blackdragonx61 */ __SKILL_COOLTIME_UPDATE__ // Refresh Skill Cooldown After Death
#define /* @author: Owsap */ __EXPRESSING_EMOTIONS__ // Special Actions
#define __RANDOM_STATUS_PER_LEVEL__ // Random Status Per Level
#define __PVP_COUNTDOWN__ // PvP Duel Countdown
#define __7AND8TH_SKILLS__ // 7th, 8th Passive Skills
#define __PARTY_PROFICY__ // Party Proficy Passive Skill
#define __9TH_SKILL__ // 9th Player Skill
#define __CONQUEROR_LEVEL__ // Conqueror Level

////////////////////////////////////////////////////////////////////////////////
// Game Related
#define __PET_SYSTEM__ // Pet System
#define __DICE_SYSTEM__ // New Dice System (Party)
#define __MOVE_CHANNEL__ // Move Game Channel
//#define __SKILL_COLOR_SYSTEM__ // Skill Color System
#define /* @author: Owsap */ __SKILLBOOK_COMB_SYSTEM__ // Skill Book Combination
#define __SEND_TARGET_INFO__ // Monster Information & Drops
#define /* @author: Owsap */ __ELEMENT_SYSTEM__ // Element System
#define /* @author: blackdragonx61 */ __CHANGE_LOOK_SYSTEM__ // Change Look System (Item Look)
#define /* @author: VegaS */ __EXTENDED_ITEM_AWARD__ // Extended Item Award
#define /* @author: VegaS */ __REFINE_FAIL_TYPE__ // Extended Refine Fail Message
#define /* @author: blackdragonx61 */ __CHANNEL_STATUS_UPDATE__ // Channel Player Count
#define /* @author: blackdragonx61 */ __WJ_SHOW_PARTY_ON_MINIMAP__ // Party Member Atlas (Map)
#define /* @author: Owsap */ __EVENT_BANNER_FLAG__ // Event Banner Flags
#define /* @author: blackdragonx61 */ __MAILBOX__ // Mail Box System

////////////////////////////////////////////////////////////////////////////////
// Item Related
#define __ANTI_EXP_RING__ // Anti Experience Ring
#define __ITEM_DROP_RENEWAL__ // Item Drop Renewal w/ Color Effect
#define __WJ_PICKUP_ITEM_EFFECT__ // Picking Item Effect
#define __NEW_DROP_DIALOG__ // New Drop Dialog w/ Delete Item Option
#define __SOUL_BIND_SYSTEM__ // Soul Bind System (Protect Items)
#define __GACHA_SYSTEM__ // Boss (Gacha) Boxes (Open x Times)
#define __MAGIC_REDUCTION__ // Magic Reduction Item
#define __STONE_OF_BLESS__ // Stone of Bless (Refinement Item)
#define /* @author: Owsap */ __SOUL_SYSTEM__ // Soul System
#define /* @author: Owsap */ __BLEND_AFFECT__ // Blend Affects w/ Icon
#define /* @author: Owsap */ __EXTENDED_BLEND_AFFECT__ // Extended Blend Item Affect
#define __ITEM_SOCKET5__ // Extended Item Sockets (5)
#define __ITEM_APPLY4__ // Extended Apply Bonus (4)
#define /* @author: Owsap */ __ITEM_APPLY_RANDOM__ // Apply Random Bonus (Base Bonus)

////////////////////////////////////////////////////////////////////////////////
// Dungeon Related
#define __12ZI_NOTICE__ // 12ZI Mission Notice (w/o Dungeon)
#define __DUNGEON_INFO_SYSTEM__ // Dungeon Information System
#define __GUILD_DRAGONLAIR_PARTY_SYSTEM__ // Guild Dragon Lair Party
#if defined(__GUILD_DRAGONLAIR_PARTY_SYSTEM__)
#	define __GUILD_DRAGONLAIR__ // Guild Dragon Lair Dungeon
#	if defined(__GUILD_DRAGONLAIR__)
#		define GUILD_DRAGONLAIR_DESTROY_STATUE_GM
#		define GUILD_DRAGONLAIR_LAZER_EFFECT_75HP
#		define GUILD_DRAGONLAIR_LAZER_EFFECT_50HP
#	endif
#endif
#define __TEMPLE_OCHAO__ // Temple of the Ochao
#define __EREBUS_DUNGEON__ // Erebus Dungeon
#define /* @author: Owsap */ __SHIP_DEFENSE__ // Ship Defense (Hydra Dungeon)

////////////////////////////////////////////////////////////////////////////////
// UI Related
#define __WJ_SHOW_MOB_INFO__ // Monsters Level & Aggressive Flag
#define __WJ_NEW_USER_CARE__ // User Care (Control)
#define __BINARY_ATLAS_MARK_INFO__ // Atlas Mark Info Load
#define /* @author: Owsap */ __QUEST_RENEWAL__ // Quest Page Renewal
#define /* @author: Owsap */ __CHATTING_WINDOW_RENEWAL__ // Chatting Window Renewal (Mini Version)

////////////////////////////////////////////////////////////////////////////////
// Environment Related
#define /* @author: blackdragonx61 */ __ENVIRONMENT__ // Environment System

////////////////////////////////////////////////////////////////////////////////
// Miscellaneous
#define __EXTENDED_RELOAD__ // Extended GM Reload Commands (For drops)

//////////////////////////////////////////////////////////////////////////
// Network Related
#define __IMPROVED_PACKET_ENCRYPTION__ // 패킷 암호화 개선
//#define __SEND_SEQUENCE__ // Sequence Matching
#define __UDP_BLOCK__ // UDP Block
//#define __ALLOW_EXTERNAL_PEER__ // Allow External Peer API

#endif // __INC_COMMON_SERVICE_H__
