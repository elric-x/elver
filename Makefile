# -*- Makefile -*-
CC=gcc
CFLAGS= -Wall -Wextra -Werror -g 
DEBUG_CFLAGS=$(CFLAGS) 

SRC_DIR=./src
OBJ_DIR=./obj
INCLUDE_DIR=./include
TEST_DIR=./debug
BIN_DIR=./bin

$(BIN_DIR)/httpServer: $(OBJ_DIR)/string.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/node.o $(OBJ_DIR)/hashtable.o $(OBJ_DIR)/server.o $(OBJ_DIR)/queue.o $(OBJ_DIR)/httpRequest.o $(OBJ_DIR)/httpResponse.o $(OBJ_DIR)/httpRouter.o $(OBJ_DIR)/httpHeader.o  $(OBJ_DIR)/error.o $(OBJ_DIR)/httpServer.o $(OBJ_DIR)/main.o
	$(CC)  $^ -o $@


$(OBJ_DIR)/main.o:$(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/node.o:../dataStructure/src/node.c ../dataStructure/include/node.h
	$(CC) -c ../dataStructure/src/node.c -o $@

$(OBJ_DIR)/hashtable.o:../dataStructure/src/hashtable.c ../dataStructure/include/hashtable.h
	$(CC)  -c ../dataStructure/src/hashtable.c -o $@

$(OBJ_DIR)/queue.o:../dataStructure/src/queue.c ../dataStructure/include/queue.h
	$(CC)  -c ../dataStructure/src/queue.c -o $@

$(OBJ_DIR)/server.o:src/server.c $(INCLUDE_DIR)/server.h
	$(CC) $(CFLAGS) -c src/server.c -o $@

$(OBJ_DIR)/httpServer.o:src/httpServer.c $(INCLUDE_DIR)/httpServer.h
	$(CC) $(CFLAGS) -c src/httpServer.c -o $@

$(OBJ_DIR)/httpResponse.o:src/httpResponse.c $(INCLUDE_DIR)/httpResponse.h
	$(CC) $(CFLAGS) -c src/httpResponse.c -o $@

$(OBJ_DIR)/httpRequest.o:src/httpRequest.c $(INCLUDE_DIR)/httpRequest.h
	$(CC) $(CFLAGS) -c src/httpRequest.c -o $@

$(OBJ_DIR)/httpHeader.o:src/httpHeader.c $(INCLUDE_DIR)/httpHeader.h
	$(CC) $(CFLAGS) -c src/httpHeader.c -o $@

$(OBJ_DIR)/httpRouter.o:src/httpRouter.c $(INCLUDE_DIR)/httpRouter.h
	$(CC) $(CFLAGS) -c src/httpRouter.c -o $@

$(OBJ_DIR)/error.o:src/error.c $(INCLUDE_DIR)/error.h
	$(CC) $(CFLAGS) -c src/error.c -o $@

$(OBJ_DIR)/string.o:src/string.c $(INCLUDE_DIR)/string.h
	$(CC) $(CFLAGS) -c src/string.c -o $@

$(OBJ_DIR)/utils.o:src/utils.c $(INCLUDE_DIR)/utils.h
	$(CC) $(CFLAGS) -c src/utils.c -o $@
#$(OBJ_DIR)/httpRouter.o:$(SRC_DIR)/httpRouter.c $(INCLUDE_DIR)/httpRouter.h
#	$(CC) $(CFLAGS) -c $(SRC_DIR)/httpRouter.c -o $@


run: $(BIN_DIR)/httpServer
	./bin/httpServer
clean:
	rm obj/*.o

