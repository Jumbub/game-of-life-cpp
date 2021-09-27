OBJS =
OBJS_TEST = $(OBJS)

CC = g++

COMPILER_FLAGS = -Wall -std=c++2a -ggdb
COMPILER_FLAGS_TEST = $(COMPILER_FLAGS)

LINKER_FLAGS = -lSDL2
LINKER_FLAGS_TEST = $(LINKER_FLAGS)

OBJ_NAME = build/graphics
OBJ_NAME_TEST = build/test

test:
	$(CC) src/test.cpp $(COMPILER_FLAGS_TEST) $(LINKER_FLAGS_TEST) -o $(OBJ_NAME_TEST)
	./$(OBJ_NAME_TEST)

graphics:
	$(CC) src/graphics.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

all :
	make test
	./$(OBJ_NAME_TEST)
	make graphics
