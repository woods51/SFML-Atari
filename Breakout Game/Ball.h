#pragma once

#include "Tile.h"

class Ball : public Tile
{
public:
	Ball(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), sf::Vector2f size = sf::Vector2f(16.0f, 16.0f),
		sf::Vector2f scale = sf::Vector2f(1.0f, 1.0f), std::string textureID = "ball_01")
	{
		this->textureID = textureID;
		texture = rm.getTexture(textureID);
		sprite.setTexture(*texture);
		sprite.setPosition(pos);
		sprite.setScale(scale);
		this->scale = scale;
		this->size = size;
		colorIndex = 0;
		currentDir = Direction::Idle;
		defaultPos = pos;

		isActive = true;
	};
	sf::Vector2f defaultVel = sf::Vector2f((float)speed, (float)speed);
	sf::Vector2f velocity = defaultVel;
	sf::Vector2f defaultPos;
	std::string colors[6] = { "ball_01", "ball_02", "ball_03", "ball_04", "ball_05", "ball_06" };

	enum class Direction currentDir;
	
	void move(sf::Time);
	void handlePaddle(enum class Surface surface, enum class Direction paddleDir);
	void handleTile(enum class Surface surface);
	void changeColor(ResourceManager &rm)
	{
		colorIndex++;
		if (colorIndex > 5)
			colorIndex = 0;
		texture = rm.getTexture(colors[colorIndex]);
		sprite.setTexture(*texture);
	}
private:
	void handleBorder();
	int colorIndex;
};