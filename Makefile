# CC = g++
# CFLAGS = -std=c++11 -Wall
 
# all: ServerMain.cpp Server.o
# 	$(CC) $(CFLAGS) ServerMain.cpp  Server.o -o chatroom_server
# Server.o: Server.cpp Server.h Common.h
# 	$(CC) $(CFLAGS) -c Server.cpp
# clean:
# 	rm -f *.o world_server

OBJ+=./src/Server.o ./src/GameLogicManager.o ./src/ProtocolsManager.o
SRC+=./src/Server.cpp ./src/GameLogicManager.cpp ./src/ProtocolsManager.cpp
BIN+=./bin/world_server
INCLUDE+=-Iinclude

CC = g++
CFLAGS = -std=c++11 -Wall

$(BIN):$(OBJ) ./src/ServerMain.cpp
	$(CC) $(<) -o $(BIN) $(INCLUDE) $(CFLAGS)
$(OBJ): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(<) -o $(@) $(INCLUDE)
clean:
	-rm -rf $(BIN) $(OBJ)
