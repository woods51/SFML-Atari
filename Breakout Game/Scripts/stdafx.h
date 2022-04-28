#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <cmath>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>

// Width of window
#define WIDTH 1280

// Height of window
#define HEIGHT 720

// Multiplier for speed
#define MULTIPLIER 25

////////////////////////////////////////////////////////////
/// \brief TileType Enum Class.
///
/// The TileType enum class holds every possible tile type used
/// in the LevelEditor and BreakoutGame.
/// 
////////////////////////////////////////////////////////////
enum class TileType
{
	Default,
	Lock,
	Lock2,
	Special,
	Special2,
	Special3,
	Wall,
	Blank,
	None
};

////////////////////////////////////////////////////////////
/// \brief Direction Enum Class.
///
/// The Direction enum class stores all direction types.
/// 
////////////////////////////////////////////////////////////
enum class Direction
{
	Left,
	Right,
	Up,
	Down,
	Idle
};

////////////////////////////////////////////////////////////
/// \brief Surface Enum Class.
///
/// The Surface enum class stores each potential surface of contact.
/// This class mainly used for computing and handling collisions.
/// 
////////////////////////////////////////////////////////////
enum class Surface
{
	Top,
	Bottom,
	Left,
	Right,
	Corner,
	None
};

////////////////////////////////////////////////////////////
/// \brief Press Enum Class.
///
/// The Press enum class stores every possible button press type.
/// Press types are typically returned by a buttons OnClick function
/// and may certain events.
/// 
////////////////////////////////////////////////////////////
enum class Press
{
	DEFAULT,
	OTHER,
	START,
	MAINMENU,
	QUIT,
	BACK,
	PAUSE,
	RESUME,
	OPTIONS,
	SAVE,
	LOAD,
	PONG,
	BREAKOUT,
	EDITOR,
	EDIT,
	GAMESELECT,
	BALLCOLOR,
	NEXT,
	PREVIOUS,
	LEVEL,
	RELOAD,
	BACKGROUND,

	BUTTON_UP,
	BUTTON_DOWN,
	BALL_UP,
	BALL_DOWN,
	BUTTON_UP_X,
	BUTTON_DOWN_X,
	BALL_UP_X,
	BALL_DOWN_X,

	GAME_UP,
	GAME_DOWN,
	LEVEL_UP,
	LEVEL_DOWN,

	TILE,
	PEN,
	ERASE,
	DESELECT
};

////////////////////////////////////////////////////////////
/// \brief Sound Enum Class.
///
/// The Sound enum class stores the types of in-game sounds
/// that are supported by the ResourceManager.
/// 
////////////////////////////////////////////////////////////
enum class Sound
{
	Button,
	Ball,
	GameOver,
	LevelComplete,
	None
};