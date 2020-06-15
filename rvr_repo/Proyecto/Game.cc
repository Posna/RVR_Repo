#include <string.h>
#include "Ball.h"


using namespace std;
typedef unsigned int uint;

	std::mutex mutex_bolitas;

Game::Game(const char* s, const char* p): socket(s,p) {
	Vector2D origen(0, 0);

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Edgar.io", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	//SDL_RenderSetLogicalSize(WIN_WIDTH, WIN_HEIGHT);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (window == nullptr || renderer == nullptr)
    printf("Error loading the SDL window or renderer"); //Esto deberia dar un error

	//Inicializacion de las texturas
	//for (int i = 0; i < NUM_TEXTURES; i++) {
	//	texturas[i] = new Texture(renderer, atributos[i].nombre, atributos[i].row, atributos[i].col);
	//}

	//Bolitas de prueba
	//bolitas = {new Ball(Vector2D(500, 300)), new Ball(Vector2D(1300, 10)), new Ball(Vector2D(1700, 10))};

	player = new Ball(Vector2D(500, 300), true, 10);

  /* INICIALIZACION DE TODO DEL JUEGO */
}

void Game::render() const{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);


  /* Todos los renders aqui*/
	player->render(renderer);
	player->desfase(player->getPos() - Vector2D(WIN_WIDTH/2, WIN_HEIGHT/2));
	mutex_bolitas.lock();
	for(int i = 0; i < bolitas.size(); i++){
		bolitas[i]->desfase(player->getPos()- Vector2D(WIN_WIDTH/2, WIN_HEIGHT/2));
		bolitas[i]->render(renderer);
	}
	mutex_bolitas.unlock();


	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event) && !exit) {
		if (event.type == SDL_QUIT)
      exit = true;
		//if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP)
		//	player->addRadius(1);

	}
}

void Game::run() {
	uint32_t startTime, frameTime;
	startTime = SDL_GetTicks();
	login();
	//login a server
	while (!exit) {
			handleEvents();
			frameTime = SDL_GetTicks() - startTime; // Tiempo desde última actualización
			if (frameTime >= FRAME_RATE) {
				update(frameTime); // Actualiza el estado de todos los objetos del juego
				startTime = SDL_GetTicks();
				sendPos(); //Envío de posicion
			}
			render();
	}
	logout();
}

void Game::update(uint32_t frameTime) {
	player->update(frameTime);
}

void Game::sendPos()
{
	player->setType(Ball::POSITION);
	socket.send(*player, socket);
}

void Game::login()
{
	player->setType(Ball::LOGIN);
	socket.send(*player, socket);
}

void Game::logout()
{
	player->setType(Ball::LOGOUT);
	socket.send(*player, socket);
}

void Game::recieve_information()
{
    while(!exit)
    {
			Ball b;
			socket.recv(b);

			switch (b.getType()) {
				case Ball::POSITION:
				mutex_bolitas.lock();
				for (Ball* ball : bolitas)
				{
					if (b.getId() == ball->getId())
					{
						ball->setPos(b.getPos());
					}
				}
				mutex_bolitas.unlock();
				break;

				case Ball::EAT:
				player->setRadius(b.getRadius());
				break;

				case Ball::DEAD:
				printf("Me he muerto\n");
				exit = true;
				break;

				case Ball::ID:
				player->setId(b.getId());
				break;

			}

      //Recibir Mensajes de red
      //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
      /*ChatMessage cm;
      Socket* s;
      socket.recv(cm);
      std::cout << &cm.nick[0] << ": " << &cm.message[0] << "\n";*/
    }
}
Game::~Game() {
	delete player;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
