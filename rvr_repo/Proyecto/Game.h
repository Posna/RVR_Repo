#include <unistd.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <mutex>
#include <thread>
#include <shared_mutex>
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
const int LEVEL_WIDTH = 700;
const int LEVEL_HEIGHT = 700;

const int MAX_BOLITAS = 100;


class Ball;

class Game {
private:
	SDL_Window * window = nullptr; //Ventana de la aplicacion
	SDL_Renderer* renderer = nullptr; //Render de la aplicacion
	bool exit = false; //Bool para finalizar la ejecucion del juego
	bool muerto = false; //Boll para saber cuando se muere el jugador
	Texture* texturas[NUM_TEXTURES]; //Array de las texturas
	//TexturesAtributes atributos[NUM_TEXTURES] = { "sdl_logo.bmp", 1, 1};
	SDL_Rect cam = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
	SDL_Surface* p = NULL;
	Ball* player = nullptr;
	Socket socket; //Socket del servidor
	std::vector<Ball*> bolitas;// = {new Ball(Vector2D(1,10)), new Ball(Vector2D(1,30)), new Ball(Vector2D(1,50))};

	//Metodos para interactuar con el servidor
	void sendPos();
	void login();
	void logout();


public:
	Game(const char* s, const char* p);
	~Game();

	//Recibe la informacion que le manda el Server
	//Se ejecuta con un thread
	void recieve_information();

	//Funcion sobre la que corre todo el juego
	void run();

	//Funciones de ejecucion
	void render() const;
	void update(uint32_t frameTime);
	void handleEvents();
};
