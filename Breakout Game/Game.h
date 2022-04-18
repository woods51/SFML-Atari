#pragma once
#include "stdafx.h"

#include "ResourceManager.h"
#include "State.h"
#include "MenuState.h"
#include "BreakoutMenu.h"
#include "BreakoutState.h"
#include "PongState.h"
#include "LevelEditor.h"
#include "OptionsState.h"
#include "PauseState.h"
#include "SaveMenu.h"
#include "LoadMenu.h"

#include "Tile.h"
#include "Ball.h"
#include "Paddle.h"
#include "Button.h"

class Game
{
public:
	Game() {
		
	};
	void run();
	void exitGame();

private:
	// Vector of smart pointers to game states
	std::vector<std::unique_ptr<State>> m_states;
};