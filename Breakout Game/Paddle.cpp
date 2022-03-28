#include "Paddle.h"

void Paddle::move(sf::Time dt)
{
	sf::Vector2f movement(0, 0);
	if (m_IsMovingLeft)
	{
		movement.x = -m_speed * dt.asSeconds() * MULTIPLIER;
		setDirection(Direction::Left);
	}
	else if (m_IsMovingRight)
	{
		movement.x = m_speed * dt.asSeconds() * MULTIPLIER;
		setDirection(Direction::Right);
	}
	else
		setDirection(Direction::Idle);

	m_shape.move(movement);

	handleBorder();
}
void Paddle::handleBorder()
{
	if (getPosition().x < 0)
	{
		m_shape.setPosition(sf::Vector2f(0, getPosition().y));
	}
	if (getDiagonalPosition().x > WIDTH)
	{
		m_shape.setPosition(sf::Vector2f(WIDTH - (m_shape.getSize().x * m_shape.getScale().x), getPosition().y));
	}
	if (getPosition().y < 0)
	{
		m_shape.setPosition(sf::Vector2f(getPosition().x, 0));
	}
	if (getDiagonalPosition().y > HEIGHT)
	{
		m_shape.setPosition(sf::Vector2f(getPosition().x, HEIGHT - (m_shape.getSize().y * m_shape.getScale().y)));
	}
}


void Paddle::collided(bool b) { m_collided = b; }
bool Paddle::hasCollided() const { return m_collided; }
enum class Direction Paddle::getDirection() const { return m_currentDir; }

void Paddle::setDirection(enum class Direction d) { m_currentDir = d; }
