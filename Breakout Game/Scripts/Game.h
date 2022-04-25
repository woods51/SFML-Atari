#pragma once

#include "stdafx.h"
#include "ResourceManager.h"

#include "./States/State.h"
#include "./States/Games/BreakoutState.h"
#include "./States/Games/PongState.h"
#include "./States/Games/LevelEditor.h"
#include "./States/Menus/MenuState.h"
#include "./States/Menus/BreakoutMenu.h"
#include "./States/Menus/OptionsState.h"
#include "./States/Menus/PauseState.h"
#include "./States/Menus/SaveMenu.h"
#include "./States/Menus/LoadMenu.h"

#include "./GameObjects/Tile.h"
#include "./GameObjects/Ball.h"
#include "./GameObjects/PongBall.h"
#include "./GameObjects/Paddle.h"
#include "./GameObjects/Button.h"

class Game
{
public:
	Game() {
		
	};
	void run();

private:
	void exit();

	// Vector of smart pointers to game states
	std::vector<std::unique_ptr<State>> m_states;
};