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
		this->defaultTexture = textureID;

		shape.setPosition(pos);
		shape.setSize(size);
		shape.setScale(scale);
		shape.setTexture(rm.getTexture(textureID));

		setActive();
	};

	sf::Vector2f getPosition() { return this->shape.getPosition(); }
	sf::Vector2f getDiagonalPos();

	sf::RectangleShape getShape() { return this->shape; }

	void setActive()
	{
		m_active = true;
	}
	void setDeactive()
	{
		m_active = false;
	}
	bool isActive() { return m_active; }
	const sf::Texture* getTexture()
	{
		return shape.getTexture();
	}

protected:
	sf::RectangleShape shape;
	bool m_active;
	std::string defaultTexture;
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