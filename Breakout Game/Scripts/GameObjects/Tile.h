#pragma once
#include "../stdafx.h"
#include "../ResourceManager.h"

////////////////////////////////////////////////////////////
/// \brief Tile Class.
///
/// This class is used to create rectangular shaped tile objects.
/// Tile objects are primarily used by the BreakoutState and LevelEditor.
/// 
////////////////////////////////////////////////////////////
class Tile
{
public:
	Tile();

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the tile class.
	///
	/// Constructs tile object using provided parameters and sets active.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_pos		--> Position
	/// \param a_type		--> TileType
	/// \param a_textureKey	--> String key for texture
	////////////////////////////////////////////////////////////
	Tile(ResourceManager& a_rm, sf::Vector2f a_pos = sf::Vector2f(0, 0),
		TileType a_type = TileType::Default, std::string a_textureKey = "")
	{
		m_shape.setPosition(a_pos);
		m_shape.setSize(sf::Vector2f(32, 16));
		m_shape.setScale(sf::Vector2f(4, 4));

		setTile(a_rm, a_type, a_textureKey);
		isActive(true);
	};

	////////////////////////////////////////////////////////////
	/// \brief Handles collision with ball and tile.
	///
	/// This functions decrements durability and alters current textures.
	/// This function may also set the tile to an inactive state.
	/// 
	////////////////////////////////////////////////////////////
	void handleBall();

	////////////////////////////////////////////////////////////
	/// \brief Updates data members for tile objects.
	///
	/// Helper function used to alter the texture,
	/// TileType and isActive state. Primarily used by the LevelEditor
	/// to make changes to the current tile selection.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_type		--> TileType
	/// \param a_textureKey	--> String key for texture
	////////////////////////////////////////////////////////////
	void updateTile(ResourceManager& a_rm, TileType a_type, std::string a_textureKey);

	////////////////////////////////////////////////////////////
	/// \brief Setup for tile objects.
	///
	/// Helper function used to alter or construct an tile object.
	/// Makes changes to first, second, and third texture
	/// values as well as durability based on TileType.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_type		--> TileType
	/// \param a_textureKey	--> String key for texture
	////////////////////////////////////////////////////////////
	void setTile(ResourceManager& a_rm, TileType a_type, std::string a_textureKey);

	////////////////////////////////////////////////////////////
	/// \brief Resets tile object back to its original state
	///
	/// This function resets the tiles isActive state, durability,
	/// default textures, and destructable state.
	/// 
	////////////////////////////////////////////////////////////
	void reset();

	// Computes and returns diagonal position
	sf::Vector2f getDiagonalPosition() const;

	// Returns position
	sf::Vector2f getPosition() const { return m_shape.getPosition(); }

	// Returns const reference to shape object
	const sf::RectangleShape& getShape() const { return m_shape; }

	// Returns default texture key
	std::string getTextureKey() const { return m_defaultTexture; }

	// Returns TileType
	TileType getTileType() const { return m_type; }
	
	////////////////////////////////////////////////////////////
	/// \brief Sets Position
	///
	/// \param a_pos --> Position
	////////////////////////////////////////////////////////////
	void setPosition(sf::Vector2f a_pos) { m_shape.setPosition(a_pos); }

	// Returns isActive state
	bool isActive() const { return m_isActive; }

	////////////////////////////////////////////////////////////
	/// \brief Sets isActive state
	///
	/// \param a_isActive --> State
	////////////////////////////////////////////////////////////
	void isActive(bool a_isActive) { m_isActive = a_isActive; }

	// Returns isDestructible
	bool isDestructible() const { return m_isDestructible; }
	
	// Returns isColliding state
	bool isColliding() const { return m_isColliding; }

	////////////////////////////////////////////////////////////
	/// \brief Sets isColliding state
	///
	/// \param a_isColliding --> State
	////////////////////////////////////////////////////////////
	void isColliding(bool a_isColliding) { m_isColliding = a_isColliding; }

protected:

	// Data members
	TileType m_type;
	sf::RectangleShape m_shape;
	int m_durability = 1;

	// Textures
	std::string m_defaultTexture;
	sf::Texture* m_firstTexture;
	sf::Texture* m_secondTexture;
	sf::Texture* m_thirdTexture;

	// States
	bool m_isDestructible = true;
	bool m_isActive = true;
	bool m_isColliding = false;
};