#include "Ball.h"

void Ball::move(sf::Time dt)
{
	sprite.move(velocity.x * dt.asSeconds() * MULTIPLIER, velocity.y * dt.asSeconds() * MULTIPLIER);
	handleBorder();
}
void Ball::handlePaddle(enum class Surface surface, enum class Direction paddleDir)
{
	float scalar = 0.1f;
	float incr_scalar = 0.25f;
	// Ball collides with Side of Paddle
	if (surface == Surface::Left || surface == Surface::Right)
	{
		if (paddleDir == currentDir)
			velocity = sf::Vector2f(-velocity.x + scalar, -velocity.y + scalar);
		else
			velocity.x = -velocity.x;
	}
	else if (surface == Surface::Top) // Ball Hits Top of Paddle
	{
		// Ball and Paddle moving in same direction
		if (currentDir == paddleDir)
			velocity = sf::Vector2f(velocity.x + (speed * scalar), -velocity.y + (speed * incr_scalar));
			//velocity = sf::Vector2f(velocity.x * scalar + 0.1f, -velocity.y * scalar);

		// Paddle Idle
		else if (paddleDir == Direction::Idle)
			velocity = sf::Vector2f(velocity.x + scalar, -velocity.y + scalar);
			//velocity.y = -velocity.y;

		// Ball and Paddle moving in opposite directions
		else
			velocity = sf::Vector2f( -1 *(velocity.x + (speed * incr_scalar)), -1 * (velocity.y + (speed * scalar)));
			//velocity = sf::Vector2f(-velocity.x, -velocity.y);
	}
	else // ball it bottom of paddle (aka something broke)
	{
		std::cout << "ball hit below paddle" << std::endl;
	}

	return;
}
void Ball::handleTile(enum class Surface surface)
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
void Ball::handleBorder()
{
	// Window Border collision detection and handling w/ Ball
	float offset = scale.x * size.x;
	if (sprite.getPosition().x <= 0)
	{
		sprite.setPosition(0, sprite.getPosition().y);
		velocity.x = -velocity.x;
	}
	if (getDiagonalPos().x >= WIDTH)
	{
		sprite.setPosition(WIDTH - offset, sprite.getPosition().y);
		velocity.x = -velocity.x;
	}
	if (sprite.getPosition().y <= 0)
	{
		sprite.setPosition(sprite.getPosition().x, 0);
		velocity.y = -velocity.y;
	}
	if (getDiagonalPos().y >= HEIGHT)
	{
		sprite.setPosition(defaultPos);
		velocity = defaultVel;
	}
}