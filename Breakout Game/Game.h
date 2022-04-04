#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

#include "State.h"
#include "BreakoutState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "PongState.h"

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