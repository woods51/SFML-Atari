#include "LevelLoader.h"

void LevelLoader::loadFiles(const std::string& a_path, std::vector<std::string>& a_levelNames)
{
	a_levelNames.clear();
	std::string filePath;
	std::string fileName;
	
	// Load level names
	for (const auto& file : std::filesystem::directory_iterator(a_path))
	{
		filePath = file.path().filename().string();
		fileName = filePath.substr(0, filePath.length() - 4);

		a_levelNames.push_back(fileName);
	}
}

bool LevelLoader::loadMap(ResourceManager& a_rm, const std::string& a_levelName, const std::string& a_path,
	std::vector<Tile*>& a_tileMap, std::string& a_errorMsg)
{
	// Clear current tile map
	for (auto& tile : a_tileMap)
	{
		delete tile;
	}
	a_tileMap.clear();

	std::string line;
	std::string path = a_path + a_levelName + ".csv";
	std::ifstream inputFile(path);

	int i = 1;
	if (inputFile.is_open())
	{
		// Steps through each line of file
		while (std::getline(inputFile, line))
		{
			if (!parseTileData(a_rm, line, a_tileMap))
			{
				// Error parsing line
				std::string temp = "Error reading file.\nLine (" + std::to_string(i) + ")\nLoad Canceled.";
				a_errorMsg = temp;
				return false;
			}
			i++;
		}
		// Successful Load
		a_errorMsg = "";
		inputFile.close();
	}
	return true;
}

bool LevelLoader::parseTileData(ResourceManager& a_rm, std::string a_line, std::vector<Tile*>& a_tileMap)
{
	TileType tileType = TileType::None;
	float posX, posY;
	int type;
	std::string texture, error;

	// Initalizing local variables
	posX = posY = 0;
	type = 0;
	texture = error = "";

	// Start line parsing
	replace(a_line.begin(), a_line.end(), ',', ' ');
	std::istringstream parser(a_line);
	parser >> posX >> posY >> type >> texture >> error;

	if (!error.empty())
		return false;

	tileType = getTileType(type);

	Tile* temp = new Tile(a_rm, sf::Vector2f(posX, posY), tileType, texture);
	a_tileMap.push_back(temp);
	return true;
}

TileType LevelLoader::getTileType(int a_type)
{
	switch (a_type)
	{
	case 0:
		return TileType::Default;
		break;
	case 1:
		return TileType::Lock;
		break;
	case 2:
		return TileType::Lock2;
		break;
	case 3:
		return TileType::Special;
		break;
	case 4:
		return TileType::Special2;
		break;
	case 5:
		return TileType::Special3;
		break;
	case 6:
		return TileType::Wall;
		break;
	default:
		return TileType::Blank;
		break;
	}
}