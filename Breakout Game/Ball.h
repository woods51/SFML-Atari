#pragma once

#include "Tile.h"

class Ball : public Tile
{
public:
	Ball(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), std::string textureID = "ball", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f), sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f))
		: Tile(rm, pos, textureID, size, scale)
	{
		currentDir = Direction::Idle;
		defaultPos = pos;
	};
	sf::Vector2f defaultVel = sf::Vector2f((float)speed, (float)speed);
	sf::Vector2f velocity = defaultVel;
	sf::Vector2f defaultPos;

	enum class Direction currentDir;
	
	void move(sf::Time);
	void handlePaddle(enum class Surface surface, enum class Direction paddleDir);
	void handleTile(enum class Surface surface);
private:
	void handleBorder();
};