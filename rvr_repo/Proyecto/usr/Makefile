DEPS = Game.h Vector2D.h Texture.h
OBJ = Game.o Vector2D.o Texture.o
SDL = `sdl-config --cflags --libs sdl2`

%.o: %.cc $(DEPS)
	gcc -g -c -o $@ $< $(SDL)

all: main

main:	$(OBJ) main.o 
	gcc -o $@ $^ $(SDL)

.PHONY: clean


clean:
	rm -f *.o main
