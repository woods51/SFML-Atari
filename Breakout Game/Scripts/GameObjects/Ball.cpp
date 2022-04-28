#include "./Ball.h"

void Ball::move(ResourceManager& a_rm, const sf::Time& a_dt)
{
	m_shape.move(m_velocity.x * a_dt.asSeconds() * MULTIPLIER, m_velocity.y * a_dt.asSeconds() * MULTIPLIER);
	handleBorder(a_rm);

	// Update Direction
	if (m_velocity.x < 0)
		m_direction = Direction::Left;
	else
		m_direction = Direction::Right;
}

void Ball::handleBorder(ResourceManager& a_rm)
{
	bool hitFlag = false;
	float offset = m_shape.getRadius() * 2;

	if (m_shape.getPosition().x <= 0)
	{
		m_shape.setPosition(0, m_shape.getPosition().y);
		m_velocity.x = -m_velocity.x;
		hitFlag = true;
	}
	else if (getDiagonalPosition().x >= WIDTH)
	{
		m_shape.setPosition(WIDTH - offset, m_shape.getPosition().y);
		m_velocity.x = -m_velocity.x;
		hitFlag = true;
	}

	if (m_shape.getPosition().y <= 0)
	{
		m_shape.setPosition(m_shape.getPosition().x, 0);
		m_velocity.y = -m_velocity.y;
		hitFlag = true;
	}

	// Ball falls out of playable area
	else if (getDiagonalPosition().y >= HEIGHT)
	{
		m_velocity = sf::Vector2f(0, 0);
		isActive(false);
	}
		
	if (hitFlag)
		a_rm.playSound(Sound::Ball);
}

void Ball::handlePaddle(enum class Surface a_surface, enum class Direction a_paddleDir)
{
	if (a_surface == Surface::Left || a_surface == Surface::Right)
		m_velocity.x = -m_velocity.x;

	else if (a_surface == Surface::Top)
	{
		// Moving in same direction
		if (m_direction == a_paddleDir)
			m_velocity = sf::Vector2f(m_velocity.x * m_scalar, -m_velocity.y * m_scalar);

		// Not moving
		else if (a_paddleDir == Direction::Idle)
			m_velocity.y = -m_velocity.y;

		// Moving in opposite directions
		else
			m_velocity = sf::Vector2f(-m_velocity.x * m_scalar, -m_velocity.y);
	}

	else if (a_surface == Surface::Corner)
		m_velocity.y = -m_velocity.y;
}

void Ball::handleTile(enum class Surface a_surface)
{
	float scalar = 1.05f;

	if (a_surface == Surface::Left || a_surface == Surface::Right)
		m_velocity.x = -m_velocity.x * scalar;

	else if (a_surface == Surface::Top || a_surface == Surface::Bottom)
		m_velocity.y = -m_velocity.y * scalar;

	else if (a_surface == Surface::Corner)
		m_velocity = -m_velocity * scalar;
}

enum class Surface Ball::collision(sf::Vector2f a_tilePos, sf::Vector2f a_tileDiagPos) const
{
	// Get positions
	sf::Vector2f tile_pos = a_tilePos;
	sf::Vector2f tile_diagonal_pos = a_tileDiagPos;

	sf::Vector2f pos = m_shape.getPosition();
	sf::Vector2f diagonal_pos = getDiagonalPosition();
	/*
	Data point visualization
	
	Tile		Ball
	p1	p2		       b_top
			    b_left	     b_right
	p3	p4            b_bottom
	*/
	// Generate tile data points
	sf::Vector2i p1((int)tile_pos.x, (int)tile_pos.y);
	sf::Vector2i p2((int)tile_diagonal_pos.x, (int)tile_pos.y);
	sf::Vector2i p3((int)tile_pos.x, (int)tile_diagonal_pos.y);
	sf::Vector2i p4((int)tile_diagonal_pos.x, (int)tile_diagonal_pos.y);

	// Generate ball data points
	sf::Vector2i b_top((int)pos.x + ((int)diagonal_pos.x - (int)pos.x) / 2, (int)pos.y);
	sf::Vector2i b_bottom((int)pos.x + ((int)diagonal_pos.x - (int)pos.x) / 2, (int)diagonal_pos.y);
	sf::Vector2i b_left((int)pos.x, (int)pos.y + ((int)diagonal_pos.y - (int)pos.y) / 2);
	sf::Vector2i b_right((int)diagonal_pos.x, (int)pos.y + ((int)diagonal_pos.y - (int)pos.y) / 2);

	sf::Vector2i vertex = b_top + ((b_bottom - b_top) / 2);
	float radius = m_shape.getRadius();

	// Check for side collision
	int i = 0;
	for (auto b : { b_top, b_bottom, b_left, b_right })
	{
		if (b.x >= tile_pos.x && b.x <= tile_diagonal_pos.x &&
			b.y >= tile_pos.y && b.y <= tile_diagonal_pos.y)
		{
			switch (i)
			{
			case 0:
				return Surface::Bottom;
			case 1:
				return Surface::Top;
			case 2:
				return Surface::Left;
			case 3:
				return Surface::Right;
			}
		}
		i++;
	}

	// Check for corner collision
	for (auto p : { p1, p2, p3, p4 })
	{
		double dist = vectorDistance(p, vertex);

		if (dist <= double(radius))
		{
			return Surface::Corner;
		}
	}
	return Surface::None;
}

void Ball::toggleColor(ResourceManager& a_rm)
{
	if (++m_colorIndex > 5)
		m_colorIndex = 0;
	m_shape.setTexture(a_rm.getTexture(m_colors[m_colorIndex]));
}

sf::Vector2f Ball::getDiagonalPosition() const
{
	return sf::Vector2f(m_shape.getPosition().x + m_shape.getGlobalBounds().width,
		m_shape.getPosition().y + m_shape.getGlobalBounds().height);
}
void Ball::isActive(bool a_isActive)
{
	if (a_isActive || !a_isActive)
		m_velocity = m_startVel;

	else if (!a_isActive)
	{
		m_shape.setPosition(m_startPos);
		m_velocity = sf::Vector2f(0, 0);
	}

	m_isActive = a_isActive;
}
double Ball::vectorDistance(const sf::Vector2i& a_p1, const sf::Vector2i& a_p2) const
{
	return (double)sqrt(pow((a_p2.x - a_p1.x), 2) + pow((a_p2.y - a_p1.y), 2));
}

void Ball::reset()
{
	m_shape.setPosition(m_startPos);
	m_velocity = sf::Vector2f(velocityRNG(), -m_speed);
}

float Ball::velocityRNG()
{
	int num = rand() % 8 + 6;
	int sign = rand() % 2 + 1;
	if (sign == 2)
		num = -num;
	return (float)num;
}