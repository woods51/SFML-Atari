#pragma once
#include "stdafx.h"
#include "ResourceManager.h"
#include "GameObjects/Tile.h"

class LevelLoader
{
public:
	LevelLoader()
	{

	};

	bool parseTileData(ResourceManager& a_rm, std::string a_line, std::vector<Tile*>& a_tileMap);

private:
	TileType getTileType(int a_type);
};