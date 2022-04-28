#include "stdafx.h"
#include "Game.h"

#include <windows.h>
int main()
{
	// Hide console
	//FreeConsole();

	// Set random seed
	srand((unsigned int)time(NULL));

	// Create game object
	Game game;

	// Launch SFML Atari Game
	game.start();

	return 0;
}