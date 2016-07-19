#include "MainGame.h"
const int GAME_SCREEN_WIDTH = 755;
const int GAME_SCREEN_HEIGHT = 600;
const char* GAME_TITLE = "Blocks Game";
const float GAME_MAX_FRAME_TICKS = 300.0f;
const float GAME_FONT_SCALE = 0.5f;
//Launch the main game
int main(int argc, char *argv[]) 
{
	try {
		MainGame game(
			GAME_TITLE,
			GAME_SCREEN_WIDTH,
			GAME_SCREEN_HEIGHT,
			GAME_MAX_FRAME_TICKS,
			GAME_FONT_SCALE
		);
		game.Start();
	}
	catch (const char* message) 
	{
		std::cout << "Fatal Error: " << message;
		return 0;
	}
	
	return 1;
}

