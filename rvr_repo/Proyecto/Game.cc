#include "Game.h"
#include <string.h>
//#include "Ball.h"


using namespace std;
typedef unsigned int uint;

Game::Game() {
	Vector2D origen(0, 0);

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Edgar.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	//SDL_RenderSetLogicalSize(WIN_WIDTH, WIN_HEIGHT);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (window == nullptr || renderer == nullptr)
    printf("Error loading the SDL window or renderer"); //Esto deberia dar un error

	//Inicializacion de las texturas
	for (int i = 0; i < NUM_TEXTURES; i++) {
		texturas[i] = new Texture(renderer, atributos[i].nombre, atributos[i].row, atributos[i].col);
	}

	player = new Ball(Vector2D(50, 300), 10);

  /* INICIALIZACION DE TODO DEL JUEGO */
}

void Game::render() const{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);


  /* Todos los renders aqui*/
	player->render(renderer);


	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) && !exit) {
		if (event.type == SDL_QUIT)
      exit = true;

	}
}

void Game::run() {
	uint32_t startTime, frameTime;
	startTime = SDL_GetTicks();
	while (!exit) {
			handleEvents();
			frameTime = SDL_GetTicks() - startTime; // Tiempo desde última actualización
			if (frameTime >= FRAME_RATE) {
				update(); // Actualiza el estado de todos los objetos del juego
				startTime = SDL_GetTicks();
			}
			render();
	}

}

void Game::update() {
	player->update();
}



Game::~Game() {
	delete player;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
