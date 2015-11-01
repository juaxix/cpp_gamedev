#include <iostream>
#include "Game.h"
/**
@brief Create an instance of Game and run it in a while until the user wants to exit or there is an error.
*/
int main(char **argv, int argc)
{
	Game *game = new Game();
	while(!game->playerWantToExit){
		game->Loop();
	}
}

