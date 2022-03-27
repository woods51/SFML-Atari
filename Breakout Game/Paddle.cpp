#include "Paddle.h"

void Paddle::move(sf::Time dt)
{
	sf::Vector2f movement(0, 0);
	if (m_IsMovingLeft)
	{
		movement.x = -speed * dt.asSeconds() * MULTIPLIER;
		setDirection(Direction::Left);
	}
	else if (m_IsMovingRight)
	{
		movement.x = speed * dt.asSeconds() * MULTIPLIER;
		setDirection(Direction::Right);
	}
	else
		setDirection(Direction::Idle);

	this->shape.move(movement);

	handleBorder();
}
void Paddle::setDirection(enum class Direction d) { currentDir = d; }

void Paddle::handleBorder()
{
	if (getPosition().x < 0)
	{
		shape.setPosition(sf::Vector2f(0, getPosition().y));
	}
	if (getDiagonalPos().x > WIDTH)
	{
		shape.setPosition(sf::Vector2f(WIDTH - (shape.getSize().x * shape.getScale().x), getPosition().y));
	}
	if (getPosition().y < 0)
	{
		shape.setPosition(sf::Vector2f(getPosition().x, 0));
	}
	if (getDiagonalPos().y > HEIGHT)
	{
		shape.setPosition(sf::Vector2f(getPosition().x, HEIGHT - (shape.getSize().y * shape.getScale().y)));
	}
}