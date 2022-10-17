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

CFLAGS = -m32 -g -Wall -O2 -pipe -fno-rtti -fno-exceptions -Wno-long-long -pthread -D_THREAD_SAFE
CFLAGS += -Wno-invalid-source-encoding
CFLAGS += -Wno-tautological-pointer-compare
CXXFLAGS = -std=c++17

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

# MySQL
INCDIR += -I/usr/local/include/mysql
ifeq ($(PLATFORM), 64)
LIBS = -lmysqlclient -lz -lzstd
else
LIBS += /usr/local/lib/mysql/libmysqlclient.a /usr/lib/libz.a /usr/local/lib/libzstd.a
endif

# OpenSSL
LIBS += -lssl -lcrypto

# Project Library
LIBDIR += -L../../libthecore/lib -L../../libosp/lib -L../../libsql -L../../libpoly -L../../libgame/lib
LIBS += -lthecore -losp -lsql -lpoly -lgame -lm

TARGET = $(BINDIR)/db_r$(REVISION)_$(PLATFORM)

SRCS = Config.cpp NetBase.cpp Peer.cpp PeerBase.cpp Main.cpp Lock.cpp DBManager.cpp \
	Cache.cpp LoginData.cpp ClientManager.cpp ClientManagerPlayer.cpp ClientManagerLogin.cpp \
	ClientManagerBoot.cpp ClientManagerParty.cpp ClientManagerGuild.cpp GuildManager.cpp HB.cpp \
	PrivManager.cpp MoneyLog.cpp ItemAwardManager.cpp ClientManagerEventFlag.cpp Marriage.cpp \
	Monarch.cpp BlockCountry.cpp ItemIDRangeManager.cpp ClientManagerHorseName.cpp version.cpp \
	ProtoReader.cpp CsvReader.cpp

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

default: $(TARGET)

$(TARGET): $(OBJS)
	@echo linking $(TARGET)....
	@$(CC) $(CFLAGS) $(CXXFLAGS) $(LIBDIR) $(OBJS) $(LIBS) -o $(TARGET)
	@expr $(REVISION) > ../REVISION

$(OBJDIR)/%.o: %.cpp
	@echo compiling $<
	@$(CC) $(CFLAGS) $(CXXFLAGS) $(INCDIR) -c $< -o $@

$(OBJDIR)/version.o: version.cpp
	@$(CC) $(CFLAGS) $(CXXFLAGS) -D__REVISION__=\"$(REVISION)\" -c $< -o $@
	@echo compiling $<

$(OBJDIR):
	@mkdir $(OBJDIR)

clean:
	@rm -f $(OBJS) $(BINDIR)/db_r*

dep:
	@touch Depend
	makedepend -f Depend $(INCDIR) -I/usr/include/c++/3.3 -I/usr/include/c++/4.2 -p$(OBJDIR)/ $(SRCS) 2> /dev/null

sinclude Depend
