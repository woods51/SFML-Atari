#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "State.h"
#include "PlayState.h"
#include "MenuState.h"

#include "Tile.h"
#include "Ball.h"
#include "Paddle.h"
#include "ResourceManager.h"
#include "Button.h"

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
	std::vector<std::unique_ptr<State>> m_states;
};