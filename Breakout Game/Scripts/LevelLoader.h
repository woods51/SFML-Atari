#pragma once
#include "stdafx.h"
#include "ResourceManager.h"
#include "GameObjects/Tile.h"

////////////////////////////////////////////////////////////
/// \brief LevelLoader Class.
///
/// The LevelLoader class is used for loading tile maps
/// or level name strings from a particular directory.
/// 
////////////////////////////////////////////////////////////
class LevelLoader
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Default constructor for the LevelLoader.
	///
	/// Constructs level loader.
	/// 
	////////////////////////////////////////////////////////////
	LevelLoader() {};

	////////////////////////////////////////////////////////////
	/// \brief Loads level names from directory.
	///
	/// This function loads the file names
	/// 
	/// \param a_path		--> Directory of levels
	/// \param a_levelNames	--> Vector of level names
	/// 
	////////////////////////////////////////////////////////////
	void loadFiles(const std::string& a_path, std::vector<std::string>& a_levelNames);

	////////////////////////////////////////////////////////////
	/// \brief Loads level data into tile map from file.
	///
	/// This function handles 
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_levelName	--> Level file name
	/// \param a_path		--> Directory of levels
	/// \param a_tileMap	--> Tile map to fill
	/// \param a_errorMsg	--> Error message output
	/// 
	/// \returns True if tile map loaded successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool loadMap(ResourceManager& a_rm, const std::string& a_levelName, const std::string& a_path,
		std::vector<Tile*>& a_tileMap, std::string& a_errorMsg);

private:

	////////////////////////////////////////////////////////////
	/// \brief Parses tile data from a line of text.
	///
	/// This function parses tile data from a given line
	/// and creates a new tile from that data.
	/// That new tile is then pushed into the tile map.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_line	--> Line of file
	/// \param a_tileMap	--> Tile map to fill
	/// 
	/// \returns True if line data was parsed successful, false otherwise.
	////////////////////////////////////////////////////////////
	bool parseTileData(ResourceManager& a_rm, std::string a_line, std::vector<Tile*>& a_tileMap);

	////////////////////////////////////////////////////////////
	/// \brief Determines corresponding TileType based on integer respresentation.
	///
	/// \param a_type	--> Number of associated with TileType
	/// 
	/// \returns True corresponding TileType.
	////////////////////////////////////////////////////////////
	TileType getTileType(int a_type);
};