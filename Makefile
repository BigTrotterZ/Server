# CC = g++
# CFLAGS = -std=c++11 -Wall
 
# all: ServerMain.cpp Server.o
# 	$(CC) $(CFLAGS) ServerMain.cpp  Server.o -o chatroom_server
# Server.o: Server.cpp Server.h Common.h
# 	$(CC) $(CFLAGS) -c Server.cpp
# clean:
# 	rm -f *.o world_server

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

