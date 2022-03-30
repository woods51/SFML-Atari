#pragma once

#include "Tile.h"

class Paddle : public Tile
{
public:
	Paddle(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(WIDTH/2 - 160.0f, 640.0f), sf::Vector2f size = sf::Vector2f(64.0f, 4.0f),
		sf::Vector2f scale = sf::Vector2f(5.0f, 5.0f), std::string textureKey = "paddle")
		: Tile(rm, pos, textureKey, size, scale)
	{
		m_currentDir = Direction::Idle;
		m_IsMovingLeft = false;
		m_IsMovingRight = false;
	};

	void move(sf::Time dt);

	void handleBorder();

	// Sets collided
	void collided(bool b) { m_collided = b; }

	// Sets current direction
	void setDirection(enum class Direction direction) { m_currentDir = direction; }

	// Returns collided
	bool hasCollided() const { return m_collided; }

	// Returns current direction
	enum class Direction getDirection() const { return m_currentDir; }

	bool m_IsMovingLeft, m_IsMovingRight;

private:
	
	enum class Direction m_currentDir;
	float m_speed = 12.0f;
	bool m_collided = false;
};