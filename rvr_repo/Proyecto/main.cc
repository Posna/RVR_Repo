#include <stdio.h>
#include <stdbool.h>
#include "Game.h"

int main ( int argc, char *argv[] )
{
  Game* game = new Game();
	game->run();
	delete game; //delete
	return 0;

  return 0;
}
