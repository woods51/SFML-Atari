#pragma once

#include "Tile.h"

class Ball
{
public:
	Ball(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), float radius = 8.0f,
		sf::Vector2f scale = sf::Vector2f(1.0f, 1.0f), std::string textureID = "ball_01")
	{
		defaultPos = pos;
		defaultVel = sf::Vector2f(speed, speed);
		velocity = defaultVel;
		this->shape.setPosition(pos);
		this->shape.setRadius(radius);
		this->shape.setScale(scale);
		this->shape.setTexture(rm.getTexture(textureID));
	};
	

	sf::Vector2f defaultVel;
	sf::Vector2f velocity;
	sf::Vector2f defaultPos;
	std::string colors[6] = { "ball_01", "ball_02", "ball_03", "ball_04", "ball_05", "ball_06" };
	
	void move(sf::Time);
	void handlePaddle(enum class Surface surface, enum class Direction paddleDir);
	void handleTile(enum class Surface surface);
	void reset();
	void update(sf::Time dt)
	{
		move(dt);

		if (this->velocity.x < 0)
			this->currentDir = Direction::Left;
		else
			this->currentDir = Direction::Right;
	}
	void toggleColor(ResourceManager &rm)
	{
		colorIndex++;
		if (colorIndex > 5)
			colorIndex = 0;
		shape.setTexture(rm.getTexture(colors[colorIndex]));
	}
	enum class Direction getDirection() { return currentDir;  }
	float getRadius() { return shape.getRadius(); }
	sf::Vector2f getPosition() { return shape.getPosition(); }

	enum class Surface collision(sf::Vector2f, sf::Vector2f);

	double distance(sf::Vector2i, sf::Vector2i);
	sf::CircleShape getShape() { return this->shape; }

	const sf::Texture* getTexture()
	{
		return shape.getTexture();
	}
private:
	sf::CircleShape shape;

	void handleBorder();
	int colorIndex = 0;
	enum class Direction currentDir = Direction::Idle;
	float speed = 8.0f;
};