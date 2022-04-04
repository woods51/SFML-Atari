#include <SFML/Graphics.hpp>
//#include "GameObject.h"
//#include "ResourceManager.h"
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