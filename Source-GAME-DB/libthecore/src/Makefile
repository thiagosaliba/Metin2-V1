CC = clang++90
BIN_DIR = ../lib
BIN = $(BIN_DIR)/libthecore.a
INCLUDE = ../include

CFLAGS = -m32 -Wall -O2 -pipe -g -I$(INCLUDE)
CFLAGS += -Wno-deprecated
CFLAGS += -Wno-invalid-source-encoding
CXXFLAGS += -std=c++17

LIBS =

OBJFILES = socket.o fdwatch.o buffer.o signal.o log.o utils.o \
	kstbl.o hangul.o heart.o main.o tea.o des.o gost.o memcpy.o

default:
	$(MAKE) $(BIN)

$(BIN): $(OBJFILES)
	if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi
	ar cru $(BIN) $(OBJFILES) $(LIBS)
	ranlib $(BIN)
	chmod 700 $(BIN)

clean:
	rm -f *.o
	rm -f $(BIN)

dep:
	$(CC) $(CFLAGS) $(CXXFLAGS) -MM *.c > Depend

$(OBJFILES):
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

memcpy: memcpy.o utils.o log.o
	$(CC) $(CFLAGS) $(CXXFLAGS) -c -D__MAIN__ memcpy.c
	$(CC) $(CFLAGS) $(CXXFLAGS) -o memcpy memcpy.o utils.o log.o

include Depend
