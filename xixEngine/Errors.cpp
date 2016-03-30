#include "Errors.h"
namespace xixEngine {
	void fatalError(std::string errorString)
	{
		std::cout << errorString << std::endl << "Press any key to quit \n";
		_getch();
		SDL_Quit(); //exit all from SDL
		exit(66); //exit with error
	}
}