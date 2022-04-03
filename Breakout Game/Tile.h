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

	sf::Vector2f getDiagonalPosition() const;
	inline sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	const sf::Texture* getTexture() { return m_shape.getTexture(); }
	const sf::RectangleShape& getShape() const { return m_shape; }
	
	void handleBall();
	bool isActive() const { return m_active; }
	bool isDestructable() const { return m_destructable; }
	void setActive() { m_active = true; }
	void setDeactive() { m_active = false; }

protected:
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