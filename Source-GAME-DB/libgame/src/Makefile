CXX = clang++90

BIN_DIR = ../lib
BIN = $(BIN_DIR)/libgame.a

CFLAGS = -m32 -Wall -O2 -pipe -fno-exceptions -I../include
CXXFLAGS += -std=c++17

LIBS =

OBJFILES = grid.o attribute.o targa.o

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
	touch Depend
	$(CXX) $(CFLAGS) $(CXXFLAGS) -MM *.cc > Depend

$(OBJFILES):
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $<

include Depend
