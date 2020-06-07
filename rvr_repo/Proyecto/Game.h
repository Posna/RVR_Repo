#include <unistd.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "Vector2D.h"
#include "Texture.h"
#include "Socket.h"


enum TexturesName{Logo}; //Numero de cada textura
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int NUM_TEXTURES = 1; //numero de texturas a cargar
const std::string RUTA = "..\\images\\";
const uint32_t FRAME_RATE = 16;

//Dimension del nivel
const int LEVEL_WIDTH = 2000;
const int LEVEL_HEIGHT = 2000;


class Ball;

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
	Socket socket;
	std::vector<Ball*> bolitas;// = {new Ball(Vector2D(1,10)), new Ball(Vector2D(1,30)), new Ball(Vector2D(1,50))};
	void sendPos();
	void login();
	void logout();
	void recieve_information();
public:
	Game();
	~Game();
	void run();
	void render() const;
	void update(uint32_t frameTime);
	void handleEvents();
	//void apply_surface(Vector2D v, SDL_Surface* source
};
