CC=g++
CFLAGS=-g -I. -std=c++17 -I/usr/include/SDL2 -D_REENTRANT
DEPS = Vector2D.h Texture.h Ball.h Game.h Socket.h Server.h 
OBJ  = Vector2D.o Texture.o Ball.o Game.o Socket.o Server.o 
LIBS=-lpthread -lm -lX11 -std=c++11 -lSDL2 -lSDL2_image -lSDL2_gfx
SDL = `sdl-config --cflags --libs sdl2`

%.o: %.cc $(DEPS)
	g++ -g -c -o $@ $< $(CFLAGS)

all: mainc mains

mainc:	$(OBJ) main.o 
	g++ -o $@ $^ $(CFLAGS) $(LIBS)
	
mains:	$(OBJ) ServerMain.o 
	g++ -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean


clean:
	rm -f *.o mainc mains
