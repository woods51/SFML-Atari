#include "Paddle.h"

void Paddle::move(sf::Time dt)
{
	sf::Vector2f movement(0, 0);
	if (m_IsMovingLeft)
	{
		movement.x = -speed * dt.asMilliseconds();
		updateDir(Direction::Left);
	}
	else if (m_IsMovingRight)
	{
		movement.x = speed * dt.asMilliseconds();
		updateDir(Direction::Right);
	}
	else
		updateDir(Direction::Idle);

	sprite.move(movement);

	handleBorder();
}
void Paddle::updateDir(enum class Direction d) { currentDir = d; }

void Paddle::handleBorder()
{
	if (sprite.getPosition().x < 0)
	{
		sprite.setPosition(sf::Vector2f(0, sprite.getPosition().y));
	}
	if (getDiagonalPos().x > WIDTH)
	{
		sprite.setPosition(sf::Vector2f(WIDTH - (size.x * sprite.getScale().x), sprite.getPosition().y));
	}
	if (sprite.getPosition().y < 0)
	{
		sprite.setPosition(sf::Vector2f(sprite.getPosition().x, 0));
	}
	if (getDiagonalPos().y > HEIGHT)
	{
		sprite.setPosition(sf::Vector2f(sprite.getPosition().x, HEIGHT - (size.y * sprite.getScale().y)));
	}
}