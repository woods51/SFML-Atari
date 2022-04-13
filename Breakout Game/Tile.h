#pragma once

#include <SFML\Graphics.hpp>
#include "ResourceManager.h"
#include <cmath>
#include <iostream>

enum class TileType
{
	Default,
	LOCK,
	LOCK2,
	Special,
	Special2,
	Special3,
	Wall,
	Blank,
	None
};

class Tile
{
public:
	Tile();
	Tile(ResourceManager& a_rm, sf::Vector2f a_pos = sf::Vector2f(0, 0),
		TileType a_type = TileType::Default, std::string a_textureKey = "")
	{
		m_shape.setPosition(a_pos);
		m_shape.setSize(sf::Vector2f(32, 16));
		m_shape.setScale(sf::Vector2f(4, 4));

		setTile(a_rm, a_type, a_textureKey);
		isActive(true);
	};

	sf::Vector2f getDiagonalPosition() const;
	sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	sf::Texture* getTexture() { return m_firstTexture; }
	const sf::RectangleShape& getShape() const { return m_shape; }

	std::string getTextureKey() const { return m_defaultTexture; }
	TileType getTileType() const { return m_type; }
	
	void setPosition(sf::Vector2f pos) { m_shape.setPosition(pos); }

	// Returns isActive
	bool isActive() const { return m_isActive; }

	// Sets isActive
	void isActive(bool a_isActive) { m_isActive = a_isActive; }

	// Returns isDestructable
	bool isDestructable() const { return m_isDestructable; }
	
	// Returns isColliding
	bool isColliding() const { return m_isColliding; }

	// Sets isColliding
	void isColliding(bool a_isColliding) { m_isColliding = a_isColliding; }
	
	void handleBall();

	void updateTile(ResourceManager& a_rm, TileType a_type, std::string a_textureKey);

	void setTile(ResourceManager& a_rm, TileType a_type, std::string a_textureKey);

protected:
	TileType m_type;
	sf::RectangleShape m_shape;
	int m_durability = 1;
	bool m_isDestructable;
	bool m_isActive;
	bool m_isColliding = false;
	std::string m_defaultTexture;
	sf::Texture* m_firstTexture;
	sf::Texture* m_secondTexture;
	sf::Texture* m_thirdTexture;
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