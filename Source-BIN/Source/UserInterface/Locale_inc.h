#if !defined(__INC_LOCALE_INC_H__)
#define __INC_LOCALE_INC_H__

#define VERSION 5035
#define PATCH 35

#define LOCALE_SERVICE_EUROPE
#define LOCALE_SERVICE_OWSAP

////////////////////////////////////////////////////////////////////////////////
// Currency Related
#define ENABLE_CHEQUE_SYSTEM // Cheque (Won) System
#define ENABLE_GEM_SYSTEM // Gem (Gaya) System

////////////////////////////////////////////////////////////////////////////////
// Minigame Related
#define /* @author: VegaS */ ENABLE_MINI_GAME_OKEY // Minigame Rumi (Okey)
#define /* @author: ZeNu */ ENABLE_MINI_GAME_CATCH_KING // Minigame Catch King

////////////////////////////////////////////////////////////////////////////////
// Dragon Soul Related
#define ENABLE_DRAGON_SOUL_SYSTEM // Dragon Soul System
#define ENABLE_DS_GRADE_MYTH // Dragon Soul Mythical Grade
#define ENABLE_DS_SET // Dragon Soul Table Handling
#define /* @author: Owsap */ ENABLE_DS_CHANGE_ATTR // Dragon Soul Change Attribute
#define ENABLE_DSS_EXTENDED_RECHARGE // [ITEM] Dragon Soul Extended Recharge

////////////////////////////////////////////////////////////////////////////////
// Costume Related
#define ENABLE_COSTUME_SYSTEM // Costume System
#define ENABLE_MOUNT_COSTUME_SYSTEM // Mount Costume System
#define ENABLE_ACCE_COSTUME_SYSTEM // Acce (Sash) Costume System
#define ENABLE_WEAPON_COSTUME_SYSTEM // Weapon Cstume System
#define ENABLE_COSTUME_ATTR_SYSTEM // [ITEM] Costume Attributes System
#define /* @author: Owsap */ ENABLE_COSTUME_EXTENDED_RECHARGE // [ITEM] Costume Extended Recharge
#define /* @author: Owsap */ ENABLE_HIDE_COSTUME_SYSTEM // Hide Costume System
#define ENABLE_AURA_COSTUME_SYSTEM // Aura Costume System (Proto Support Only)

////////////////////////////////////////////////////////////////////////////////
// Inventory & Equipment Related
#define ENABLE_NEW_EQUIPMENT_SYSTEM // New Equipement System (Belt)
#define ENABLE_SWAP_SLOT_SYSTEM // Swap Item Slot
#if defined(ENABLE_SWAP_SLOT_SYSTEM)
#	define ENABLE_SWAP_SLOT_DIFFUSE_COLOR // Diffuse Color Swap Slot
#endif
#define ENABLE_EXTEND_INVEN_SYSTEM // Extended Inventory Pages (4)
#define ENABLE_SPECIAL_INVENTORY_SYSTEM // Special Inventory System
//#define ENABLE_SORT_INVENTORY // Sort Inventory Items
#define ENABLE_QUIVER_SYSTEM // Quiver Equipement
#define ENABLE_PENDANT // Talisman Elements
#define ENABLE_GLOVE_SYSTEM // Glove Equipement

////////////////////////////////////////////////////////////////////////////////
// Messenger Related
#define ENABLE_MESSENGER_BLOCK // Messenger Block System
#define ENABLE_MESSENGER_LOCK_FIX // Messenger List Lock Fix
#define ENABLE_MESSENGER_GM // Messenger GM List

////////////////////////////////////////////////////////////////////////////////
// Shop Related
#define /* @author: blackdragonx61 */ ENABLE_SHOPEX_RENEWAL // ShopEX Renewal
#if defined(ENABLE_SHOPEX_RENEWAL)
#	define ENABLE_SHOPEX_TAB4 // ShopEx 4 Tabs
#endif
#define /* @author: Owsap */ ENABLE_MYSHOP_DECO // Private Shop Decoration
#define /* @author: blackdragonx61 */ ENABLE_PRIVATESHOP_SEARCH_SYSTEM // Private Shop Search System

//////////////////////////////////////////////////////////////////////////
// Player Related
#define ENABLE_VIEW_TARGET_DECIMAL_HP // View Target HP
#if defined(ENABLE_VIEW_TARGET_DECIMAL_HP)
#	define ENABLE_VIEW_TARGET_PLAYER_HP // View Player Target HP
#endif
#define ENABLE_PLAYER_PER_ACCOUNT5 // Players Per Account (5)
#define ENABLE_WOLFMAN_CHARACTER // Wolfman Character
#if defined(ENABLE_WOLFMAN_CHARACTER)
//#	define DISABLE_WOLFMAN_CREATION // Disable Wolfman Creation
#endif
#define /* @author: Owsap */ ENABLE_PLAYER_PIN_SYSTEM // Player PIN Code
#define ENABLE_GUILD_LEADER_GRADE_NAME // Guild Leader Grade Name (TextTail)
#define /* @auhtor: Vegas */ ENABLE_GENDER_ALIGNMENT // Gender Alignment (M, F)
#define /* @author: blackdragonx61 */ ENABLE_IMPROVED_LOGOUT_POINTS // Improved Logout Points
#define /* @author: blackdragonx61*/ ENABLE_SKILL_COOLTIME_UPDATE // Refresh Skill Cooldown After Death
#define /* @author: Owsap */ ENABLE_EXPRESSING_EMOTION // Special Actions
#define ENABLE_EMOTION_HIDE_WEAPON // Hide Weapon While Acting
#define ENABLE_678TH_SKILL // 6th & 7th Skills
#define ENABLE_CONQUEROR_LEVEL // Conqueror Level

//////////////////////////////////////////////////////////////////////////
// Game Related
#define ENABLE_DICE_SYSTEM // New Dice System (Party)
#define ENABLE_MOVE_CHANNEL // Move Game Channel
#define /* @author: Owsap */ ENABLE_CAPTCHA_SYSTEM // Captcha System
//#define ENABLE_SKILL_COLOR_SYSTEM // Skill Color System
#if defined(ENABLE_SKILL_COLOR_SYSTEM)
#	define ENABLE_5LAYER_SKILL_COLOR // Skill Color w/ 5 Layers
#endif
#define /* @author: Owsap */ ENABLE_SKILLBOOK_COMB_SYSTEM // Skill Book Combination
#define ENABLE_SEND_TARGET_INFO // Monster Information & Drops
#define ENABLE_ELEMENT_ADD // Monster Element
#define /* @auhtor: blackdragonx61 */ ENABLE_CHANGE_LOOK_SYSTEM // Change Look System (Item Look)
#define /* @author: VegaS */ ENABLE_REFINE_FAIL_TYPE // Extended Refine Fail Message
#define /* @author: Owsap */ ENABLE_CHANNEL_STATUS_UPDATE // Channel Player Count
#define /* @author: blackdragonx61 */ WJ_SHOW_PARTY_ON_MINIMAP // Party Member Atlas (Map)
#define /* @author: Owsap */ ENABLE_EVENT_BANNER_FLAG // Event Banner Flags
#define /* @author: blackdragonx61 */ ENABLE_MAILBOX // Mail Box System

////////////////////////////////////////////////////////////////////////////////
// Dungeon Related
#define ENABLE_12ZI // 12ZI Mission Notice (w/o Dungeon)
#define /* @author: Owsap */ ENABLE_DUNGEON_INFO_SYSTEM // Dungeon Information System
#define ENABLE_GUILD_DRAGONLAIR_PARTY_SYSTEM // Guild Dragon Lair Party
#define ENABLE_GUILD_DRAGONLAIR_SYSTEM // Guild Dragon Lair Dungeon
#if defined(ENABLE_GUILD_DRAGONLAIR_SYSTEM)
#define ENABLE_GUILD_DRAGONLAIR_PARTY_SYSTEM
#	define GUILD_DRAGONLAIR_STONE 6118 // Guild Dragon Lair Statue VNum
#endif
#define /* @author: Owsap */ ENABLE_SHIP_DEFENSE // Ship Defense (Hydra Dungeon)

////////////////////////////////////////////////////////////////////////////////
// Item Related
#define ENABLE_ENERGY_SYSTEM // Energy System
#define ENABLE_ITEM_DROP_RENEWAL // Item Drop Renewal w/ Color Effect
#define ENABLE_QUICK_PICKUP // Quick Item Drop Picking
#define WJ_ENABLE_PICKUP_ITEM_EFFECT // Picking Item Effect
#define WJ_ENABLE_TRADABLE_ICON // Block Untradable Items
#define ENABLE_NEW_DROP_DIALOG // New Drop Dialog w/ Delete Item Option
#define ENABLE_67TH_BONUS // 6th & 7th Rare Bonus (Classic)
#define ENABLE_SOUL_BIND_SYSTEM // Soul Bind System (Protect Items)
#define ENABLE_GACHA // Boss (Gacha) Boxes (Open x Times)
#define ENABLE_MAGIC_REDUCTION // Magic Reduction Item
#define ENABLE_STONE_OF_BLESS // Stone of Bless (Refinement Item)
#define /* @author: Owsap */ ENABLE_SOUL_SYSTEM // Soul System
#define /* @author: Owsap */ ENABLE_NEW_BLEND_AFFECT // Blend Affects w/ Icon
#define /* @author: Owsap */ ENABLE_EXTENDED_BLEND_AFFECT // Extended Blend Item Affect
#define ENABLE_ITEM_SOCKET5 // Extended Item Sockets (5)
#define ENABLE_ITEM_APPLY4 // Extended Apply Bonus (4)
#define /* @author: Owsap */ ENABLE_APPLY_RANDOM // Apply Random Bonus (Base Bonus)

////////////////////////////////////////////////////////////////////////////////
// UI Related
#define ENABLE_SELECT_RENEWAL // New Select UI
#define ENABLE_CREATE_RENEWAL // New Create UI
#define ENABLE_SERVER_SELECT_RENEWAL // Server List Renewal
#define ENABLE_CHANNEL_LIST // Channel List Renewal
#define LOGIN_COUNT_DOWN_UI_MODIFY // Login Count Down
#define WJ_MULTI_TEXTLINE // Multi TextLine
#define ENABLE_TEXT_IMAGE_LINE // Image TextLine (Emoji)
#define ENABLE_HIGH_LIGHT_IMAGE // High Light Image
#define ENABLE_IMAGE_CLIP_RECT // Image Clipping (Borders)
#define ENABLE_OSF_MINIMAP_INFO // MiniMap Information (DateTime, Patch Version)
#define RENDER_TARGET // Render Target Model
#define WJ_SHOW_MOB_INFO // Monsters Level & Aggressive Flag
#define WJ_NEW_USER_CARE // User Care (Control)
#define ENABLE_AUTO_REFINE // Auto Refinement Option
#define ENABLE_ATLAS_MARK_INFO // Atlas Mark Info Load
#define /* @author: Owsap */ ENABLE_QUEST_RENEWAL // Quest Page Renewal
#define /* @author: Owsap */ ENABLE_KEYCHANGE_SYSTEM // Keyboard Customization System
//#	define ENABLE_HEART_BEAT_MALL // Mall Icon Heart Beat Effect
#define ENABLE_DETAILS_UI // Character Details (Bonus Page)
#define /* @author: Owsap */ ENABLE_CHATTING_WINDOW_RENEWAL // Chatting Window Renewal (Mini Version)
#define /* @author: blackdragonx61 */ ENABLE_SLOT_COVER_IMAGE_SYSTEM // Slot Cover Image

////////////////////////////////////////////////////////////////////////////////
// Environment Related
#define REMOVE_SAFEZONE_COLLISION // Remove Safe Zone NPC Collision 
#define REMOVE_SHOP_COLLISION // Remove Shop Collision
#define REMOVE_HORSE_COLLISION // Remove Horse Collision
#define REMOVE_PET_COLLISION // Remove Pet Collision
#define REMOVE_OX_PLAYER_COLLISION // Remove OX Map Player Collision
#define ENABLE_EFFECT_CAMERA_VIEW_FIX // Camera Effect View Fix
#define ENABLE_FOG_FIX // Fog Fix
#define ENABLE_FOV_OPTION // Field of View (FoV) Game Option
#define ENABLE_ENVIRONMENT_EFFECT_OPTION // Environment Effect Options
#define ENABLE_DISABLE_SOFTWARE_TILING // Disable Software Tiling
#define ENABLE_GRAPHIC_ON_OFF // Graphic System (On/Off Options)
#define ENABLE_SHADOW_RENDER_QUALITY_OPTION // Shadow Render Quality

////////////////////////////////////////////////////////////////////////////////
// Application Related
#define ENABLE_TAB_NEXT_TARGET // Next Target Tab Key
#define DISABLE_INDEX_FILE // Built In Pack Index
#define ENABLE_LOADING_TIP // Loading Tips
#define ENABLE_MAP_LOCATION_APP_NAME // Application Title w/ Map Name
#define ENABLE_ETER_PACK_OBSCURING // EterPack Key Obscuring
#define ENABLE_CONFIG_MODULE // New Configuration Module (File Settings)
#define ENABLE_DISCORD_RPC // DiscordRPC
#define CEF_BROWSER // CEF Browser

#if !defined(_DEBUG)
//#	define ENABLE_MD5_FILE_CHECK // Check File MD5
//#	define ENABLE_PATCH_CHECK // Check Patch Argument
//#	define ENABLE_CYTHON // Cython (RootLib)
#endif

//////////////////////////////////////////////////////////////////////////
// Network Related
#define /* @author: Owsap */ ENABLE_RTT_STATISTICS // Round Trip Time (RTT) Ping Statistics
#if defined(ENABLE_RTT_STATISTICS)
//#	define _DEBUG_RTT // Enable RTT logs in debug mode.
#endif

#endif // __INC_LOCALE_INC_H__
