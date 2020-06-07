#include <string.h>
#include "Ball.h"


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

	//Bolitas de prueba
	bolitas = {new Ball(Vector2D(500, 300)), new Ball(Vector2D(1300, 10)), new Ball(Vector2D(1700, 10))};

	player = new Ball(Vector2D(500, 300), true, 10);

  /* INICIALIZACION DE TODO DEL JUEGO */
}

void Game::render() const{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);


  /* Todos los renders aqui*/
	player->render(renderer);
	player->desfase(player->getPos() - Vector2D(WIN_WIDTH/2, WIN_HEIGHT/2));
	for(int i = 0; i < bolitas.size(); i++){
		bolitas[i]->desfase(player->getPos()- Vector2D(WIN_WIDTH/2, WIN_HEIGHT/2));
		bolitas[i]->render(renderer);
	}


	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) && !exit) {
		if (event.type == SDL_QUIT)
      exit = true;
		if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP)
			player->addRadius(1);

	}
}

void Game::run() {
	uint32_t startTime, frameTime;
	startTime = SDL_GetTicks();
	while (!exit) {
			handleEvents();
			frameTime = SDL_GetTicks() - startTime; // Tiempo desde última actualización
			if (frameTime >= FRAME_RATE) {
				update(frameTime); // Actualiza el estado de todos los objetos del juego
				startTime = SDL_GetTicks();
			}
			render();
	}

}

void Game::update(uint32_t frameTime) {
	player->update(frameTime);
}



Game::~Game() {
	delete player;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
