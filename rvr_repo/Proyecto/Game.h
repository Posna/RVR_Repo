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


const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const uint32_t FRAME_RATE = 16;

//Dimension del nivel
const int LEVEL_WIDTH = 700;
const int LEVEL_HEIGHT = 700;

class Ball;

class Game {
private:
	SDL_Window * window = nullptr; //Ventana de la aplicacion
	SDL_Renderer* renderer = nullptr; //Render de la aplicacion
	bool exit = false; //Bool para finalizar la ejecucion del juego
	bool muerto = false; //Boll para saber cuando se muere el jugador
	SDL_Rect cam = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
	SDL_Surface* p = NULL;

	Ball* player = nullptr;
	Socket socket; //Socket del servidor
	std::vector<Ball*> bolitas;//

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
