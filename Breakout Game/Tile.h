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
		std::string textureID = "0", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f), sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f))
	{
		m_defaultTexture = textureID;

		m_shape.setPosition(pos);
		m_shape.setSize(size);
		m_shape.setScale(scale);
		m_shape.setTexture(rm.getTexture(textureID));

		setActive();
	};

	sf::Vector2f getPosition() const;
	sf::Vector2f getDiagonalPosition() const;
	const sf::Texture* getTexture();
	const sf::RectangleShape& getShape() const;
	
	bool isActive() const;
	void setActive();
	void setDeactive();

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
	Diagonal,
	None
};