#include "stdafx.h"
#include "Game.h"

int main()
{
	// Set random seed
	srand((unsigned int)time(NULL));

	// Create game object
	Game game;

	// Launch SFML Atari Game
	game.start();

	return 0;
}