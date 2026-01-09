CC = g++
CFLAGS = -g -Wall -std=c++20 -I. -I./core -I./core/managers -I./game -I./game/gameObjects -I./game/scenes
LIBS = -lSDL2 -lm

SRC = main.cpp $(shell find core game -name "*.cpp")
OBJ = beatemup_game

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ) $(LIBS)

clean:
	rm -f $(OBJ)