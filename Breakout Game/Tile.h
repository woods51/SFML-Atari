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
		std::string a_textureKey = "0", sf::Vector2f a_size = sf::Vector2f(16.0f, 16.0f),
		sf::Vector2f a_scale = sf::Vector2f(2.5f, 2.5f), bool a_isDestructable = true)
	{
		m_defaultTexture = a_textureKey;
		m_firstTexture = a_rm.getTexture(a_textureKey);

		m_shape.setPosition(a_pos);
		m_shape.setSize(a_size);
		m_shape.setScale(a_scale);
		m_shape.setTexture(m_firstTexture);

		m_destructable = a_isDestructable;
		setActive();
	};
	Tile(ResourceManager& a_rm, sf::Vector2f a_pos = sf::Vector2f(0, 0),
		TileType a_type = TileType::Default, std::string a_textureKey = "", bool a_toChild = false)
	{
		m_shape.setPosition(a_pos);
		m_shape.setSize(sf::Vector2f(32, 16));
		m_shape.setScale(sf::Vector2f(4, 4));

		setTile(a_rm, a_type, a_textureKey, a_toChild);
		setActive();
	};

	sf::Vector2f getDiagonalPosition() const;
	sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	sf::Texture* getTexture() { return m_firstTexture; }
	const sf::RectangleShape& getShape() const { return m_shape; }
	
	void handleBall();
	bool isActive() const { return m_active; }
	bool isDestructable() const { return m_destructable; }
	void setActive() { m_active = true; }
	void setDeactive() { m_active = false; }
	void setPosition(sf::Vector2f pos) { m_shape.setPosition(pos); }

	// Returns isColliding
	bool isColliding() const { return m_isColliding; }
	// Sets isColliding
	void isColliding(bool a_isColliding) { m_isColliding = a_isColliding; }
	
	std::string getTextureKey() { return m_defaultTexture; }
	TileType getTileType() { return m_type; }
	void setTile(ResourceManager& a_rm, TileType a_type = TileType::Default, std::string a_textureKey = "", bool a_toChild = false)
	{
		if (a_toChild)
		{
			m_type = a_type;
			m_defaultTexture = a_textureKey;
			m_firstTexture = a_rm.getTexture(m_defaultTexture);
			m_shape.setTexture(m_firstTexture);
			return;
		}
		m_type = a_type;
		m_durability = 1;
		m_destructable = true;

		m_firstTexture = a_rm.getTexture(a_textureKey);
		sf::Texture* texture = m_firstTexture;
		switch (a_type)
		{
		case TileType::Default:
			break;
		case TileType::Wall:
			m_defaultTexture = a_textureKey;
			m_firstTexture = a_rm.getTexture(m_defaultTexture);
			texture = m_firstTexture;
			m_destructable = false;
			break;
		case TileType::LOCK:
			m_durability = 2;
			m_secondTexture = a_rm.getTexture(a_textureKey + "_lock");
			texture = m_secondTexture;
			break;
		case TileType::LOCK2:
			m_durability = 3;
			m_secondTexture = a_rm.getTexture(a_textureKey + "_lock");
			m_thirdTexture = a_rm.getTexture(a_textureKey + "_lock2");
			texture = m_thirdTexture;
			break;
		case TileType::Special:
			m_defaultTexture = "tile_special";
			m_firstTexture = a_rm.getTexture(m_defaultTexture);
			texture = m_firstTexture;
			break;
		case TileType::Special2:
			m_defaultTexture = "tile_special2";
			m_firstTexture = a_rm.getTexture(m_defaultTexture);
			texture = m_firstTexture;
			break;
		case TileType::Special3:
			m_defaultTexture = "tile_special3";
			m_firstTexture = a_rm.getTexture(m_defaultTexture);
			texture = m_firstTexture;
			break;
		case TileType::Blank:
			m_defaultTexture = "tile_08";
			m_firstTexture = a_rm.getTexture(m_defaultTexture);
			texture = m_firstTexture;
			break;
		default:
			break;
		}
		m_shape.setTexture(texture);
	}

protected:
	TileType m_type;
	sf::RectangleShape m_shape;
	int m_durability = 1;
	bool m_destructable;
	bool m_active;
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