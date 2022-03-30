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
		std::string textureKey = "0", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f), sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f))
	{
		m_defaultTexture = textureKey;

		m_shape.setPosition(pos);
		m_shape.setSize(size);
		m_shape.setScale(scale);
		m_shape.setTexture(rm.getTexture(textureKey));

		setActive();
	};

	sf::Vector2f getDiagonalPosition() const;
	inline sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	const sf::Texture* getTexture() { return m_shape.getTexture(); }
	const sf::RectangleShape& getShape() const { return m_shape; }
	
	bool isActive() const { return m_active; }
	void setActive() { m_active = true; }
	void setDeactive() { m_active = false; }

protected:
	sf::RectangleShape m_shape;
	bool m_active;
	std::string m_defaultTexture;
};

enum class Direction
{
	Left,
	Right,
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