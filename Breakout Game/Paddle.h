#pragma once

#include "Tile.h"

class Paddle : public Tile
{
public:
	Paddle(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), sf::Vector2f size = sf::Vector2f(32.0f, 4.0f),
		sf::Vector2f scale = sf::Vector2f(5.0f, 5.0f), std::string textureID = "paddle")
		: Tile(rm, pos, textureID, size, scale)
	{
		m_currentDir = Direction::Idle;
		m_IsMovingLeft = false;
		m_IsMovingRight = false;
	};

	void move(sf::Time dt);

	void collided(bool);
	bool hasCollided() const;
	enum class Direction getDirection() const;

	bool m_IsMovingLeft, m_IsMovingRight;

private:
	void setDirection(enum class Direction);
	void handleBorder();

	enum class Direction m_currentDir;
	float m_speed = 12.0f;
	bool m_collided = false;
};