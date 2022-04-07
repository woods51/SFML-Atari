#pragma once

#include <SFML\Graphics.hpp>
#include "ResourceManager.h"
#include <cmath>
#include <iostream>

class Tile
{
public:
	Tile();
	Tile(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0),
		std::string textureKey = "0", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f),
		sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f), bool a_isDestructable = true)
	{
		m_defaultTexture = textureKey;

		m_shape.setPosition(pos);
		m_shape.setSize(size);
		m_shape.setScale(scale);
		m_shape.setTexture(rm.getTexture(textureKey));

		m_destructable = a_isDestructable;
		setActive();
	};
	Tile(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0),
		TileType type = TileType::Default, std::string textureKey = "0")
	{
		m_shape.setPosition(pos);
		m_shape.setSize(sf::Vector2f(16, 16));
		m_shape.setScale(sf::Vector2f(2.5f, 2.5f));

		setTile(rm, type, textureKey);
		setActive();
	};

	sf::Vector2f getDiagonalPosition() const;
	inline sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	const sf::Texture* getTexture() { return m_shape.getTexture(); }
	const sf::RectangleShape& getShape() const { return m_shape; }
	
	void handleBall();
	bool isActive() const { return m_active; }
	bool isDestructable() const { return m_destructable; }
	void setActive() { m_active = true; }
	void setDeactive() { m_active = false; }

	void setTile(ResourceManager& a_rm, TileType a_type = TileType::Default, std::string a_textureKey = "0")
	{
		m_type = a_type;
		m_durability = 1;
		m_destructable = true;

		switch (a_type)
		{
		case TileType::Default:
			m_defaultTexture = a_textureKey;
			break;
		case TileType::Wall:
			m_defaultTexture = "tile_06";
			m_destructable = false;
			break;
		case TileType::Strong:
			m_durability = 2;
			m_defaultTexture = "tile_07";
			m_secondTexture = a_rm.getTexture("tile_07_1");
			break;
		case TileType::Box:
			m_defaultTexture = "crate";
			break;
		default:
			break;
		}
		m_shape.setTexture(a_rm.getTexture(m_defaultTexture));
	}

protected:
	TileType m_type;
	sf::RectangleShape m_shape;
	int m_durability = 1;
	bool m_destructable;
	bool m_active;
	std::string m_defaultTexture;
	sf::Texture* m_secondTexture;
};

class StrongTile : public Tile
{
public:
	StrongTile(ResourceManager& a_rm, sf::Vector2f a_pos = sf::Vector2f(0, 0),
		std::string a_textureKey = "0", sf::Vector2f a_size = sf::Vector2f(16.0f, 16.0f),
		sf::Vector2f a_scale = sf::Vector2f(2.5f, 2.5f), std::string a_secondTextureKey = "0", int a_durability = 2)
		: Tile(a_rm, a_pos, a_textureKey, a_size, a_scale)
	{
		m_durability = a_durability;
		m_secondTexture = a_rm.getTexture(a_secondTextureKey);
	};
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
enum class TileType
{
	Default,
	Strong,
	Box,
	Wall
};