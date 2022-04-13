#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include "Game.h"

int main()
{
	srand(time(NULL));

	Game game;
	game.run();

	return 0;
}