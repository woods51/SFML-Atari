#pragma once

#include "Tile.h"

class Paddle : public Tile
{
public:
	Paddle(ResourceManager* rm, sf::Vector2f pos = sf::Vector2f(0, 0), std::string textureID = "0", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f), sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f))
		: Tile(rm, pos, textureID, size, scale)
	{
		currentDir = Direction::Idle;
		m_IsMovingLeft = false;
		m_IsMovingRight = false;
	};

	enum class Direction currentDir;
	bool m_IsMovingLeft, m_IsMovingRight;

	void updateDir(enum class Direction d);

	void move(sf::Time dt);
private:
	void handleBorder();
};