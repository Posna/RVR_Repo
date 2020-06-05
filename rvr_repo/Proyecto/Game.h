#include <unistd.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Ball.h"

enum TexturesName{Logo}; //Numero de cada textura
const int WIN_WIDTH = 1200;
const int WIN_HEIGHT = 900;
const int NUM_TEXTURES = 1; //numero de texturas a cargar
const std::string RUTA = "..\\images\\";
const uint32_t FRAME_RATE = 5;

//Dimension del nivel
const int LEVEL_WIDTH = 2000;
const int LEVEL_HEIGHT = 2000;


//class Ball;

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
	TexturesAtributes atributos[NUM_TEXTURES] = { "sdl_logo.bmp", 1, 1};
	SDL_Rect cam = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
	SDL_Surface* p = NULL;
	Ball* player = nullptr;
public:
	Game();
	~Game();
	void run();
	void render() const;
	void update();
	void handleEvents();
	//void apply_surface(Vector2D v, SDL_Surface* source
};
