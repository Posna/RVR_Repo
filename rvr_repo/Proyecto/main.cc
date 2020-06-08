#include <stdio.h>
#include <stdbool.h>
#include "Game.h"

int main ( int argc, char *argv[] )
{
  Game* game = new Game(argv[1], argv[2]);
  std::thread info(&Game::recieve_information, game);
	game->run();
  info.join();
	delete game; //delete
	return 0;

}
