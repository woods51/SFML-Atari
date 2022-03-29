#include "Ball.h"

void Ball::update(sf::Time dt)
{
	move(dt);

	if (m_velocity.x < 0)
		m_currentDir = Direction::Left;
	else
		m_currentDir = Direction::Right;
}
void Ball::move(sf::Time dt)
{
	m_shape.move(m_velocity.x * dt.asSeconds() * MULTIPLIER, m_velocity.y * dt.asSeconds() * MULTIPLIER);
	handleBorder();
}
void Ball::handleBorder()
{
	// Window Border collision detection and handling w/ Ball
	float offset = m_shape.getRadius() * 2;

	if (m_shape.getPosition().x <= 0)
	{
		m_shape.setPosition(0, m_shape.getPosition().y);
		m_velocity.x = -m_velocity.x;
	}

	if (getDiagonalPosition().x >= WIDTH)
	{
		float pos_x = getPosition().y;
		float pos = getDiagonalPosition().y;
		m_shape.setPosition(WIDTH - offset, m_shape.getPosition().y);
		m_velocity.x = -m_velocity.x;
	}
	if (m_shape.getPosition().y <= 0)
	{
		m_shape.setPosition(m_shape.getPosition().x, 0);
		m_velocity.y = -m_velocity.y;
	}
	if (getDiagonalPosition().y >= HEIGHT)
	{
		reset();
	}
}
void Ball::handlePaddle(enum class Surface surface, enum class Direction paddleDir)
{
	float scalar = 1.01f;

	// Ball collides with Side of Paddle ...Should Rarely Happen :/
	if (surface == Surface::Left || surface == Surface::Right)
		m_velocity.x = -m_velocity.x;

	else if (surface == Surface::Top) // Ball Hits Top of Paddle
	{
		// Ball and Paddle moving in same direction
		if (m_currentDir == paddleDir)
			m_velocity = sf::Vector2f(m_velocity.x * scalar, -m_velocity.y * scalar);

		// Paddle Idle
		else if (paddleDir == Direction::Idle)
			m_velocity.y = -m_velocity.y;

		// Ball and Paddle moving in opposite directions
		else
			m_velocity = sf::Vector2f(-m_velocity.x * scalar, -m_velocity.y);
	}
	else if (surface == Surface::Diagonal)
	{
		m_velocity.y = -m_velocity.y;
	}
	else // ball it bottom of paddle (aka something broke)
	{
		std::cout << "ball hit below paddle" << std::endl;
	}

	return;
}
void Ball::handleTile(enum class Surface surface)
{
	float scalar = 1.05f;
	// Ball collides with Side of Tile
	if (surface == Surface::Left || surface == Surface::Right)
	{
		m_velocity.x = -m_velocity.x * scalar;
	}
	// Ball collides with Top or Bottom of Tile
	else if (surface == Surface::Top || surface == Surface::Bottom)
	{
		m_velocity.y = -m_velocity.y * scalar;
	}
	else if (surface == Surface::Diagonal)
	{
		m_velocity = -m_velocity * scalar;
	}
}
void Ball::reset()
{
	m_shape.setPosition(m_startPos);
	m_velocity = m_startVel;
}

enum class Surface Ball::collision(sf::Vector2f obj_pos, sf::Vector2f obj_diagonal_pos) const
{
	sf::Vector2f tile_pos = obj_pos;
	sf::Vector2f tile_diagonal_pos = obj_diagonal_pos;

	sf::Vector2f pos = m_shape.getPosition();
	sf::Vector2f diagonal_pos = getDiagonalPosition();
	float radius = m_shape.getRadius();

	// Tile			Ball
	// p1	p2		       b_top
	// p3	p4		b_left	     b_right
	//                    b_bottom

	sf::Vector2i p1((int)tile_pos.x, (int)tile_pos.y);
	sf::Vector2i p2((int)tile_diagonal_pos.x, (int)tile_pos.y);
	sf::Vector2i p3((int)tile_pos.x, (int)tile_diagonal_pos.y);
	sf::Vector2i p4((int)tile_diagonal_pos.x, (int)tile_diagonal_pos.y);

	// Isolate Vertex data Left, Right, Top and Bottom
	sf::Vector2i b_top((int)pos.x + ((int)diagonal_pos.x - (int)pos.x) / 2, (int)pos.y);
	sf::Vector2i b_bottom((int)pos.x + ((int)diagonal_pos.x - (int)pos.x) / 2, (int)diagonal_pos.y);
	sf::Vector2i b_left((int)pos.x, (int)pos.y + ((int)diagonal_pos.y - (int)pos.y) / 2);
	sf::Vector2i b_right((int)diagonal_pos.x, (int)pos.y + ((int)diagonal_pos.y - (int)pos.y) / 2);

	sf::Vector2i vertex = b_top + ((b_bottom - b_top) / 2);
	for (auto p : { p1, p2, p3, p4 })
	{
		// if the distance between any point and the center of the ball is less than the radius of the ball
		double dist = distance(p, vertex);

		if (dist <= double(radius))
		{
			return Surface::Diagonal;
		}
	}

	// Determine if any side of ball collides with object
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
double Ball::distance(sf::Vector2i p1, sf::Vector2i p2) const
{
	return (double)sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}
void Ball::toggleColor(ResourceManager& rm)
{
	m_colorIndex++;
	if (m_colorIndex > 5)
		m_colorIndex = 0;
	m_shape.setTexture(rm.getTexture(m_colors[m_colorIndex]));
}

sf::Vector2f Ball::getDiagonalPosition() const
{
	return sf::Vector2f(m_shape.getPosition().x + m_shape.getGlobalBounds().width,
		m_shape.getPosition().y + m_shape.getGlobalBounds().height);
}