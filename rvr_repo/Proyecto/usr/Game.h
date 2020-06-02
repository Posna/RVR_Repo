#include <unistd.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>


const enum TexturesName{Ball}; //Numero de cada textura
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const int NUM_TEXTURES = 1; //numero de texturas a cargar
const uint anchoW = 20;
const std::string RUTA = "..\\images\\";
const uint32_t FRAME_RATE = 5;
const uint ballAA = 10;
const uint largoP = 70;



class Game {
private:
	SDL_Window * window = nullptr;
	SDL_Renderer* renderer = nullptr;
	bool exit = false;
	struct TexturesAtributes {
		string nombre;
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
