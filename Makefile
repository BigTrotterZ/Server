# makefile

includedir=include
srcdir=src
tmpodir=src
libdir=lib

OBJ=$(tmpodir)/Server.o $(tmpodir)/GameLogicManager.o $(tmpodir)/ProtocolsManager.o
SRC=$(srcdir)/Server.cpp $(srcdir)/GameLogicManager.cpp $(srcdir)/ProtocolsManager.cpp
BIN=./bin/world_server
INCLUDE=-I$(includedir)

CC = g++
CFLAGS = -std=c++11 -Wall -ggdb3

$(BIN):$(OBJ) ./src/ServerMain.cpp
	$(CC) $(CFLAGS) $(^) -o $(@) $(INCLUDE)
$(OBJ): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(<) -o $(@) $(INCLUDE)
clean:
	-rm -rf $(BIN) $(OBJ)
