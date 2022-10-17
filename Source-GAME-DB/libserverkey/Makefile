CXX = clang++90

BIN = ./libserverkey.a

IFLAGS =
CFLAGS = $(IFLAGS) -m32 -Wall -O2 -pipe -D_THREAD_SAFE -fno-exceptions
CXXFLAGS += -std=c++17

LIBS =

OBJFILES = base64_ssl.o RSACrypto.o

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
	$(CXX) $(CFLAGS) $(CXXFLAGS) -MM *.cpp > Depend

$(OBJFILES):
	$(CXX) $(CFLAGS) $(CXXFLAGS) -c $<

sinclude Depend
