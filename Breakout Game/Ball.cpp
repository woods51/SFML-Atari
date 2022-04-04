#include "Ball.h"

void Ball::move(ResourceManager& a_rm, sf::Time a_dt)
{
	m_shape.move(m_velocity.x * a_dt.asSeconds() * MULTIPLIER, m_velocity.y * a_dt.asSeconds() * MULTIPLIER);
	handleBorder(a_rm);

	if (m_velocity.x < 0)
		m_currentDir = Direction::Left;
	else
		m_currentDir = Direction::Right;
}
void Ball::handleBorder(ResourceManager& a_rm)
{
	// Window Border collision detection and handling w/ Ball
	bool hit = false;
	float offset = m_shape.getRadius() * 2;

	if (m_shape.getPosition().x <= 0)
	{
		m_shape.setPosition(0, m_shape.getPosition().y);
		m_velocity.x = -m_velocity.x;
		hit = true;
	}
	else if (getDiagonalPosition().x >= WIDTH)
	{
		m_shape.setPosition(WIDTH - offset, m_shape.getPosition().y);
		m_velocity.x = -m_velocity.x;
		hit = true;
	}

	if (m_shape.getPosition().y <= 0)
	{
		m_shape.setPosition(m_shape.getPosition().x, 0);
		m_velocity.y = -m_velocity.y;
		hit = true;
	}
	else if (getDiagonalPosition().y >= HEIGHT/* - 60*/)
		reset();

	if (hit)
		a_rm.playSound(SoundType::Ball);

}
void Ball::handlePaddle(enum class Surface a_surface, enum class Direction a_paddleDir)
{
	// Collision with side of Paddle
	if (a_surface == Surface::Left || a_surface == Surface::Right)
		m_velocity.x = -m_velocity.x;

	// Collision top of Paddle
	else if (a_surface == Surface::Top)
	{
		// Paddle same direction
		if (m_currentDir == a_paddleDir)
			m_velocity = sf::Vector2f(m_velocity.x * m_scalar, -m_velocity.y * m_scalar);

		// Paddle Idle
		else if (a_paddleDir == Direction::Idle)
			m_velocity.y = -m_velocity.y;

		// Paddle opposite direction
		else
			m_velocity = sf::Vector2f(-m_velocity.x * m_scalar, -m_velocity.y);
	}

	// Collision with corner of Paddle
	else if (a_surface == Surface::Corner)
		m_velocity.y = -m_velocity.y;

	// Collision with bottom of Paddle (Something Broke)
	else
		std::cout << "ball hit below paddle" << std::endl;
}
void Ball::handleTile(enum class Surface a_surface)
{
	// Default scalar for Tile
	float scalar = 1.05f;

	// Collision with side of Tile
	if (a_surface == Surface::Left || a_surface == Surface::Right)
		m_velocity.x = -m_velocity.x * scalar;

	// Collision with top or bottom of Tile
	else if (a_surface == Surface::Top || a_surface == Surface::Bottom)
		m_velocity.y = -m_velocity.y * scalar;

	// Collision with corner of Tile
	else if (a_surface == Surface::Corner)
		m_velocity = -m_velocity * scalar;
}
void Ball::reset()
{
	m_shape.setPosition(m_startPos);
	m_velocity = sf::Vector2f(velocityRNG(), -m_speed);;
}

enum class Surface Ball::collision(sf::Vector2f a_tilePos, sf::Vector2f a_tileDiagPos) const
{
	sf::Vector2f tile_pos = a_tilePos;
	sf::Vector2f tile_diagonal_pos = a_tileDiagPos;

	sf::Vector2f pos = m_shape.getPosition();
	sf::Vector2f diagonal_pos = getDiagonalPosition();

	// Tile			Ball
	// p1	p2		       b_top
	// p3	p4		b_left	     b_right
	//                    b_bottom

	sf::Vector2i p1((int)tile_pos.x, (int)tile_pos.y);
	sf::Vector2i p2((int)tile_diagonal_pos.x, (int)tile_pos.y);
	sf::Vector2i p3((int)tile_pos.x, (int)tile_diagonal_pos.y);
	sf::Vector2i p4((int)tile_diagonal_pos.x, (int)tile_diagonal_pos.y);

	sf::Vector2i b_top((int)pos.x + ((int)diagonal_pos.x - (int)pos.x) / 2, (int)pos.y);
	sf::Vector2i b_bottom((int)pos.x + ((int)diagonal_pos.x - (int)pos.x) / 2, (int)diagonal_pos.y);
	sf::Vector2i b_left((int)pos.x, (int)pos.y + ((int)diagonal_pos.y - (int)pos.y) / 2);
	sf::Vector2i b_right((int)diagonal_pos.x, (int)pos.y + ((int)diagonal_pos.y - (int)pos.y) / 2);

	sf::Vector2i vertex = b_top + ((b_bottom - b_top) / 2);
	float radius = m_shape.getRadius();

	// Determine if any corner of tile collides with ball
	for (auto p : { p1, p2, p3, p4 })
	{
		double dist = vectorDistance(p, vertex);

		if (dist <= double(radius))
		{
			return Surface::Corner;
		}
	}

	// Determine if any side of tile collides with ball
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
////////////////////////////////////////////////////////////
/// \brief Computes distance between two 2D vectors
///
/// \param a_p1	--> Vector 1
/// \param a_p2	--> Vector 2
///
/// \return Distance between 2D vectors as double.
///
////////////////////////////////////////////////////////////
double vectorDistance(sf::Vector2i a_p1, sf::Vector2i a_p2)
{
	return (double)sqrt(pow((a_p2.x - a_p1.x), 2) + pow((a_p2.y - a_p1.y), 2));
}

void PongBall::move(ResourceManager& a_rm, sf::Time a_dt)
{
	m_shape.move(m_velocity.x * a_dt.asSeconds() * MULTIPLIER, m_velocity.y * a_dt.asSeconds() * MULTIPLIER);
	handleBorder(a_rm);

	if (m_velocity.y < 0)
		m_currentDir = Direction::Up;
	else
		m_currentDir = Direction::Down;
}

void PongBall::handleBorder(ResourceManager& a_rm)
{
	// Window Border collision detection and handling w/ Ball
	bool hit = false;
	float offset = m_shape.getRadius() * 2;

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
		hit = true;
	}
	else if (getDiagonalPosition().y >= HEIGHT - 60)
	{
		m_shape.setPosition(m_shape.getPosition().x, HEIGHT - 60 - offset);
		m_velocity.y = -m_velocity.y;
		hit = true;
	}

	if (hit)
		a_rm.playSound(SoundType::Ball);
}
void PongBall::handlePaddle(enum class Surface a_surface, enum class Direction a_paddleDir)
{
	// Collision with side of Paddle
	if (a_surface == Surface::Top || a_surface == Surface::Bottom)
		m_velocity.y = -m_velocity.y;

	// Collision top of Paddle
	else if (a_surface == Surface::Left || a_surface == Surface::Right)
	{
		// Paddle same direction
		if (m_currentDir == a_paddleDir)
			m_velocity = sf::Vector2f(-m_velocity.x * m_scalar, m_velocity.y * m_scalar);

		// Paddle Idle
		else if (a_paddleDir == Direction::Idle)
			m_velocity.x = -m_velocity.x;

		// Paddle opposite direction
		else
			m_velocity = sf::Vector2f(-m_velocity.x, -m_velocity.y * m_scalar);
	}

	// Collision with corner of Paddle
	else if (a_surface == Surface::Corner)
		m_velocity.x = -m_velocity.x;
}

float velocityRNG()
{
	int num = rand() % 8 + 6;
	int sign = rand() % 2 + 1;
	if (sign == 2)
		num = -num;
	return (float)num;
}