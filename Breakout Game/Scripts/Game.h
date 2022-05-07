#pragma once

#include "stdafx.h"
#include "ResourceManager.h"

#include "./States/State.h"
#include "./States/Games/BreakoutState.h"
#include "./States/Games/PongState.h"
#include "./States/Games/LevelEditor.h"
#include "./States/Menus/MainMenu.h"
#include "./States/Menus/BreakoutMenu.h"
#include "./States/Menus/OptionsMenu.h"
#include "./States/Menus/PauseMenu.h"
#include "./States/Menus/SaveMenu.h"
#include "./States/Menus/LoadMenu.h"

#include "./GameObjects/Tile.h"
#include "./GameObjects/Ball.h"
#include "./GameObjects/PongBall.h"
#include "./GameObjects/Paddle.h"
#include "./GameObjects/Button.h"

////////////////////////////////////////////////////////////
/// \brief Game Class.
///
/// The Game class represents the origin or base class for the
/// SFML Atari Game.
/// 
////////////////////////////////////////////////////////////
class Game
{
public:
	Game() {};

	////////////////////////////////////////////////////////////
	/// \brief Start SFML Atari Game
	///
	/// This function hosts the main game loop along with creating
	/// and setting up instances of the ResourceManager, RenderWindow,
	/// and Delta time. This function also initializes the game states vector
	/// with the initial MainMenu state.
	/// 
	////////////////////////////////////////////////////////////
	void start();

private:

	// Game States
	std::vector<std::unique_ptr<State>> m_states;
};