MAKE = gmake
CC = clang++90

INCDIR =
LIBDIR =
BINDIR = ..
OBJDIR = .obj

PLATFORM = $(shell file /bin/ls | cut -d' ' -f3 | cut -d'-' -f1)
GCC_VERSION = $(shell $(CC) --version 2>&1 | grep "(GCC)" | cut -d' ' -f3 | cut -d'.' -f1)
BSD_VERSION = $(shell uname -v 2>&1 | cut -d' ' -f2 | cut -d'.' -f1)
VERSION = $(shell cat ../REVISION)
REVISION = $(shell echo $$(($(VERSION)+1)))

$(shell if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi)

# Standard Setting
LIBS = -lm -lmd
CFLAGS = -m32 -w -g -Wall -O2 -pipe -fexceptions -fno-strict-aliasing -pthread -D_THREAD_SAFE -DNDEBUG
CFLAGS += -Wno-invalid-source-encoding

# Stack protector
CFLAGS += -fstack-protector-all

CXXFLAGS += -std=c++17

INCDIR += -I/usr/local/include
LIBDIR += -I/usr/local/lib

# External
INCDIR += -I../../../External/include
LIBDIR += -L../../../External/library
ifeq ($(BSD_VERSION), 13)
LIBDIR += -L../../../External/library/13.0/
endif
ifeq ($(BSD_VERSION), 12)
LIBDIR += -L../../../External/library/12.0/
endif
ifeq ($(BSD_VERSION), 11)
LIBDIR += -L../../../External/library/11.0/
endif

# Boost
INCDIR += -I../../../External/include/boost

# DevIL
INCDIR += -I../../../External/include/devil/IL
LIBS += -lIL -lpng -ltiff -lmng -llcms -ljpeg
# LIBS += -ljasper -ljbig
#LIBS += -llzma
LIBS += /usr/lib/liblzma.a

# CryptoPP
LIBS += ../../../External/library/libcryptopp.a

# MySQL
ifeq ($(PLATFORM), 64)
LIBS += -lmysqlclient -lz -lzstd
else
INCDIR += -I/usr/local/include/mysql
LIBS += /usr/local/lib/mysql/libmysqlclient.a /usr/lib/libz.a /usr/local/lib/libzstd.a
endif

# OpenSSL
INCDIR += -I/usr/include
LIBS += -lssl -lcrypto
# LIBS += /usr/lib/libssl.a

# Project Library
INCDIR += -I../../liblua/include
INCDIR += -I../../libserverkey
LIBDIR += -L../../libthecore/lib -L../../libpoly -L../../libsql -L../../libgame/lib -L../../liblua/lib -L../../libserverkey

LIBS += -lthecore -lpoly -llua -llualib -lsql -lgame -lserverkey

TARGET = $(BINDIR)/game_r$(REVISION)_$(PLATFORM)

CFILE = minilzo.c

CPPFILE = BattleArena.cpp FSM.cpp MarkConvert.cpp MarkImage.cpp MarkManager.cpp OXEvent.cpp TrafficProfiler.cpp ani.cpp\
	arena.cpp banword.cpp battle.cpp blend_item.cpp block_country.cpp buffer_manager.cpp building.cpp castle.cpp\
	char.cpp char_affect.cpp char_battle.cpp char_change_empire.cpp char_horse.cpp char_item.cpp char_cards.cpp char_manager.cpp\
	char_quickslot.cpp char_resist.cpp char_skill.cpp char_state.cpp char_gem.cpp PetSystem.cpp cmd.cpp cmd_emotion.cpp cmd_general.cpp\
	cmd_gm.cpp cmd_oxevent.cpp config.cpp constants.cpp crc32.cpp cube.cpp db.cpp desc.cpp\
	desc_client.cpp desc_manager.cpp desc_p2p.cpp dev_log.cpp dungeon.cpp empire_text_convert.cpp entity.cpp\
	entity_view.cpp event.cpp event_queue.cpp exchange.cpp file_loader.cpp fishing.cpp gm.cpp guild.cpp\
	guild_manager.cpp guild_war.cpp horse_rider.cpp horsename_manager.cpp input.cpp input_auth.cpp input_db.cpp\
	input_login.cpp input_main.cpp input_p2p.cpp ip_ban.cpp\
	item.cpp item_addon.cpp item_attribute.cpp item_manager.cpp item_manager_idrange.cpp locale.cpp\
	locale_service.cpp log.cpp login_data.cpp lzo_manager.cpp marriage.cpp\
	messenger_manager.cpp mining.cpp mob_manager.cpp monarch.cpp motion.cpp over9refine.cpp p2p.cpp packet_info.cpp\
	party.cpp pcbang.cpp polymorph.cpp priv_manager.cpp pvp.cpp\
	questevent.cpp questlua.cpp questlua_affect.cpp questlua_arena.cpp questlua_ba.cpp questlua_building.cpp\
	questlua_danceevent.cpp questlua_dungeon.cpp questlua_forked.cpp questlua_game.cpp questlua_global.cpp\
	questlua_guild.cpp questlua_horse.cpp questlua_pet.cpp questlua_item.cpp questlua_marriage.cpp questlua_mgmt.cpp\
	questlua_monarch.cpp questlua_npc.cpp questlua_oxevent.cpp questlua_party.cpp questlua_pc.cpp\
	questlua_quest.cpp questlua_target.cpp questmanager.cpp questnpc.cpp questpc.cpp\
	refine.cpp regen.cpp safebox.cpp sectree.cpp sectree_manager.cpp sequence.cpp shop.cpp\
	skill.cpp start_position.cpp target.cpp text_file_loader.cpp trigger.cpp utils.cpp vector.cpp war_map.cpp\
	wedding.cpp xmas_event.cpp version.cpp panama.cpp threeway_war.cpp map_location.cpp auth_brazil.cpp\
	BlueDragon.cpp BlueDragon_Binder.cpp DragonLair.cpp questlua_dragonlair.cpp\
	skill_power.cpp affect.cpp changelook.cpp\
	FileMonitor_FreeBSD.cpp ClientPackageCryptInfo.cpp cipher.cpp\
	buff_on_attributes.cpp dragon_soul_table.cpp DragonSoul.cpp\
	group_text_parse_tree.cpp char_dragonsoul.cpp questlua_dragonsoul.cpp\
	shop_manager.cpp shopEx.cpp item_manager_read_tables.cpp MeleyLair.cpp questlua_meleylair.cpp\
	TempleOchao.cpp questlua_templeochao.cpp minigame.cpp minigame_catch_king.cpp CsvReader.cpp\
	MailBox.cpp item_apply_random_table.cpp ShipDefense.cpp questlua_shipdefense_mgr.cpp

COBJS = $(CFILE:%.c=$(OBJDIR)/%.o)
CPPOBJS = $(CPPFILE:%.cpp=$(OBJDIR)/%.o)

MAINOBJ = $(OBJDIR)/main.o
MAINCPP = main.cpp

default: $(TARGET)

$(OBJDIR)/minilzo.o: minilzo.c
	@$(CC) $(CFLAGS) $(CXXFLAGS) $(INCDIR) -c $< -o $@
	@echo compiling $<

$(OBJDIR)/version.o: version.cpp
	@$(CC) $(CFLAGS) $(CXXFLAGS) -D__REVISION__="$(REVISION)" -c $< -o $@
	@echo compiling $<

$(OBJDIR)/%.o: %.cpp
	@echo compiling $<
	@$(CC) $(CFLAGS) $(CXXFLAGS) $(INCDIR) -c $< -o $@

limit_time:
	@echo update limit time
	@python update_limit_time.py

$(TARGET): $(CPPOBJS) $(COBJS) $(MAINOBJ)
	@echo linking $(TARGET)....
	@$(CC) $(CFLAGS) $(CXXFLAGS) $(LIBDIR) $(COBJS) $(CPPOBJS) $(MAINOBJ) $(LIBS) -o $(TARGET)
	@expr $(REVISION) > ../REVISION

clean:
	@rm -f $(COBJS) $(CPPOBJS)
	@rm -f $(BINDIR)/game_r* $(BINDIR)/conv

tag:
	ctags *.cpp *.h *.c

dep:
	makedepend -f Depend $(INCDIR) -I/usr/include/c++/3.3 -I/usr/include/c++/4.2 -p$(OBJDIR)/ $(CPPFILE) $(CFILE) $(MAINCPP) 2> /dev/null > Depend

sinclude Depend
