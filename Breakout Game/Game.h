#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "BaseState.h"

class Game
{
public:
	Game();

	void run();
	void exitGame();

private:
	void eventHandler();

	// Vector of smart pointers to game states
	std::vector<std::unique_ptr<BaseState>> m_states;
};