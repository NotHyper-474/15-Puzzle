#include "core/errBytesGen.h"
#include "main.h"

using namespace std;

int main()
{
	const int result = std::atexit(SDL_Quit);

	// Uncomment only if you want to change the error texture
	//regenerate_errBytes();
	std::cout << "Hello World!" << std::endl;

	PuzzleGame puzzleGame("Resources/puzzleimage.png");
	puzzleGame.Create("Picture Game", 300, 300, 1, false, false);
	
	InputManager::Initialize(puzzleGame);
	Engine::getInstance()->Start(&puzzleGame);

	std::cout << "Closing...\nGoodbye World!" << std::endl;

	return result;
}
