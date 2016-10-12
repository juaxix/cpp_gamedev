#include "Game.hpp"

int main(int argc, const char * argv[])
{
	//Mark to Dump memory leaks always
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Game game;
	
	game.run();
	//dump memory leaks before return
	_CrtDumpMemoryLeaks();

    return 0;
}
