#include "Paddle.h"

void Paddle::move(sf::Time a_dt)
{
	if (m_IsMovingLeft)
	{
		m_shape.move(-m_speed * a_dt.asSeconds() * MULTIPLIER, 0);
		setDirection(Direction::Left);
	}
	else if (m_IsMovingRight)
	{
		m_shape.move(m_speed * a_dt.asSeconds() * MULTIPLIER, 0);
		setDirection(Direction::Right);
	}
	else
		setDirection(Direction::Idle);

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
