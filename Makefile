# CC = g++
# CFLAGS = -std=c++11 -Wall
 
# all: ServerMain.cpp Server.o
# 	$(CC) $(CFLAGS) ServerMain.cpp  Server.o -o chatroom_server
# Server.o: Server.cpp Server.h Common.h
# 	$(CC) $(CFLAGS) -c Server.cpp
# clean:
# 	rm -f *.o world_server

OBJ+=Server.o GameLogicManager.o ProtocolsManager.o
SRC+=./src/Server.cpp ./src/GameLogicManager.cpp ./src/ProtocolsManager.cpp
BIN+=./bin/world_server
INCLUDE+=-I./include

CC = g++
CFLAGS = -std=c++11 -Wall

$(BIN):$(OBJ) ./src/ServerMain.cpp
	$(CC) $(CFLAGS) -o $(BIN) ./src/ServerMain.cpp $(OBJ) $(INCLUDE)
$(OBJ):$(SRC)
	$(CC) $(CFLAGS) -c $(<) -o $(@) $(INCLUDE)
clean:
	-rm -rf $(BIN) $(OBJ)
