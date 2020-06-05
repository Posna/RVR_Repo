CC=g++
CFLAGS=-g -I. -std=c++11 -I/usr/include/SDL2 -D_REENTRANT
DEPS = Vector2D.h Texture.h Ball.h Game.h
OBJ = Vector2D.o Texture.o Ball.o Game.o
LIBS=-lpthread -lm -lX11 -std=c++11 -lSDL2 -lSDL2_image -lSDL2_gfx
SDL = `sdl-config --cflags --libs sdl2`

%.o: %.cc $(DEPS)
	g++ -g -c -o $@ $< $(CFLAGS)

all: main

main:	$(OBJ) main.o 
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean


clean:
	rm -f *.o main
