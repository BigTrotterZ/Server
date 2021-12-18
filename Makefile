# makefile

includedir=include
srcdir=src
tmpodir=src
# libdir=lib /usr/lib

OBJ=$(tmpodir)/Server.o \
$(tmpodir)/GameLogicManager.o \
$(tmpodir)/ProtocolsManager.o \
$(tmpodir)/NetManager.o \
$(tmpodir)/ThreadPoolManager.o \
$(tmpodir)/workthreadpool.o \
$(tmpodir)/lckfree.o \
$(tmpodir)/LoginProtocol.o
SRC=$(srcdir)/Server.cpp \
$(srcdir)/GameLogicManager.cpp \
$(srcdir)/ProtocolsManager.cpp \
$(srcdir)/NetManager.cpp \
$(srcdir)/ThreadPoolManager.cpp \
$(srcdir)/workthreadpool.cpp \
$(srcdir)/lckfree.cpp \
$(srcdir)/LoginProtocol.cpp

# bin
BIN=./bin/world_server
INCLUDE=-I$(includedir)
# LIB=-L$(libdir)

CC = g++
CFLAGS = -std=c++11 -Wall -ggdb3 -lpthread $(mysql_config --cflags) $(mysql_config --libs)

$(BIN):$(OBJ) ./src/ServerMain.cpp
	$(CC) $(CFLAGS) $(^) -o $(@) $(INCLUDE)
$(OBJ): %.o: %.cpp
	$(CC) -c $(CFLAGS) $(<) -o $(@) $(INCLUDE)
clean:
	-rm -rf $(BIN) $(OBJ)
