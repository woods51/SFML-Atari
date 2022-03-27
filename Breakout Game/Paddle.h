#pragma once

#include "Tile.h"

class Paddle : public Tile
{
public:
	Paddle(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), sf::Vector2f size = sf::Vector2f(32.0f, 4.0f),
		sf::Vector2f scale = sf::Vector2f(5.0f, 5.0f), std::string textureID = "paddle_2")
		: Tile(rm, pos, textureID, size, scale)
	{
		currentDir = Direction::Idle;
		m_IsMovingLeft = false;
		m_IsMovingRight = false;
	};

	
	bool m_IsMovingLeft, m_IsMovingRight;
	void move(sf::Time dt);

	bool hasCollided = false;
	enum class Direction getDirection() { return this->currentDir; }
private:
	void setDirection(enum class Direction);
	enum class Direction currentDir;
	void handleBorder();
	float speed = 8.0f;
};