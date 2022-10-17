CXX = clang++90

BIN = ./libpoly.a

CFLAGS = -m32 -Wall -O2 -pipe -fno-exceptions -fno-rtti
CXXFLAGS += -std=c++17

LIBS =

OBJFILES = Base.o Poly.o SymTable.o Symbol.o

default:
	$(MAKE) $(BIN)

$(BIN): $(OBJFILES)
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
