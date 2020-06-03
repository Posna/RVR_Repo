#include <unistd.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Texture.h"
#include "Vector2D.h"


enum TexturesName{Ball}; //Numero de cada textura
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int NUM_TEXTURES = 1; //numero de texturas a cargar
const std::string RUTA = "..\\images\\";
const uint32_t FRAME_RATE = 5;



class Game {
private:
	SDL_Window * window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool exit = false;
	struct TexturesAtributes {
		std::string nombre;
		int row;
		int col;
	};
	Texture* texturas[NUM_TEXTURES]; //Array de las texturas
	TexturesAtributes atributos[NUM_TEXTURES] = { "ball.png", 1, 1};

public:
	Game();
	~Game();
	void run();
	void render() const;
	void update();
	void handleEvents();
};
