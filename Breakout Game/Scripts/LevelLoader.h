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

	bool loadMap(ResourceManager& a_rm, const std::string& a_levelName, const std::string& a_path,
		std::vector<Tile*>& a_tileMap, std::string& a_errorMsg);

	void loadFiles(const std::string& a_path, std::vector<std::string>& a_levelNames);
private:
	bool parseTileData(ResourceManager& a_rm, std::string a_line, std::vector<Tile*>& a_tileMap);
	TileType getTileType(int a_type);
};