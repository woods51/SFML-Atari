#include "Ball.h"

void Ball::handlePaddle(enum class Surface surface, enum class Direction paddleDir, sf::Time dt)
{
	// Ball collides with Side of Paddle
	if (surface == Surface::Left || surface == Surface::Right)
	{
		if (paddleDir == currentDir)
			velocity = sf::Vector2f(-velocity.x * scalar, -velocity.y * scalar);
		else
			velocity.x = -velocity.x;
	}
	else if (surface == Surface::Top) // Ball Hits Top of Paddle
	{
		// Ball and Paddle moving in same direction
		if (currentDir == paddleDir)
			velocity = sf::Vector2f(velocity.x * scalar, -velocity.y * scalar);

		// Paddle Idle
		else if (paddleDir == Direction::Idle)
			velocity.y = -velocity.y;

		// Ball and Paddle moving in opposite directions
		else
			velocity = sf::Vector2f(-velocity.x, -velocity.y);
	}
	else // ball it bottom of paddle (aka something broke)
	{
		std::cout << "ball hit below paddle" << std::endl;
	}

	return;
}
void Ball::handleTile(enum class Surface surface, sf::Time dt)
{
	// Ball collides with Side of Tile
	if (surface == Surface::Left || surface == Surface::Right)
	{
		velocity.x = -velocity.x;
	}
	// Ball collides with Top or Bottom of Tile
	else if (surface == Surface::Top || surface == Surface::Bottom)
	{
		velocity.y = -velocity.y;
	}
}