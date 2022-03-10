#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "State.h"
#include "PlayState.h"
#include "Tile.h"
#include "Ball.h"
#include "Paddle.h"
#include "ResourceManager.h"

#define WIDTH 800
#define HEIGHT 600

class Game
{
public:
	Game() {

	};
	void run();
	void exitGame();

	//ResourceManager rm;
	//sf::RenderWindow* window ;

private:
	// Vector of smart pointers to game states
	std::vector<std::unique_ptr<PlayState>> m_states;
};