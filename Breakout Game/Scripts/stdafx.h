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

#define WIDTH 1280
#define HEIGHT 720
#define MULTIPLIER 25

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
enum class Direction
{
	Left,
	Right,
	Up,
	Down,
	Idle
};
enum class Surface
{
	Top,
	Bottom,
	Left,
	Right,
	Corner,
	None
};
enum class Press {
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
enum class Sound {
	Button,
	Ball,
	GameOver,
	LevelComplete,
	None
};