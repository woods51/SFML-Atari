#include "PongBall.h"

void PongBall::move(ResourceManager& a_rm, const sf::Time& a_dt)
{
	m_shape.move(m_velocity.x * a_dt.asSeconds() * MULTIPLIER, m_velocity.y * a_dt.asSeconds() * MULTIPLIER);
	handleBorder(a_rm);

	// Update Direction
	if (m_velocity.y < 0)
		m_direction = Direction::Up;
	else
		m_direction = Direction::Down;
}

void PongBall::handleBorder(ResourceManager& a_rm)
{
	bool hitFlag = false;
	float offset = m_shape.getRadius() * 2;

	// Ball falls out of playable area
	if (m_shape.getPosition().x <= 0)
	{
		m_scoreP2++;
		reset();
	}
	else if (getDiagonalPosition().x >= WIDTH)
	{
		m_scoreP1++;
		reset();
	}

	if (m_shape.getPosition().y <= 0)
	{
		m_shape.setPosition(m_shape.getPosition().x, 0);
		m_velocity.y = -m_velocity.y;
		hitFlag = true;
	}
	else if (getDiagonalPosition().y >= HEIGHT - 80)
	{
		m_shape.setPosition(m_shape.getPosition().x, HEIGHT - 80 - offset);
		m_velocity.y = -m_velocity.y;
		hitFlag = true;
	}

	if (hitFlag)
		a_rm.playSound(Sound::Ball);
}

void PongBall::handlePaddle(enum class Surface a_surface, enum class Direction a_paddleDir)
{
	if (a_surface == Surface::Top || a_surface == Surface::Bottom)
		m_velocity.y = -m_velocity.y;

	else if (a_surface == Surface::Left || a_surface == Surface::Right)
	{
		// Paddle same direction
		if (m_direction == a_paddleDir)
			m_velocity = sf::Vector2f(-m_velocity.x * m_scalar * m_pongScalar, m_velocity.y * m_scalar);

		// Paddle Idle
		else if (a_paddleDir == Direction::Idle)
			m_velocity.x = -m_velocity.x * m_pongScalar;

		// Paddle opposite direction
		else
			m_velocity = sf::Vector2f(-m_velocity.x * m_pongScalar, -m_velocity.y * m_scalar);
	}

	else if (a_surface == Surface::Corner)
		m_velocity.x = -m_velocity.x;
}