#include "Ball.h"

double Ball::distance(sf::Vector2i p1, sf::Vector2i p2)
{
	return (double)sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}
void Ball::move(sf::Time dt)
{
	shape.move(velocity.x * dt.asSeconds() * MULTIPLIER, velocity.y * dt.asSeconds() * MULTIPLIER);
	handleBorder();
}
void Ball::handlePaddle(enum class Surface surface, enum class Direction paddleDir)
{
	float scalar = 1.01f;

	// Ball collides with Side of Paddle ...Should Rarely Happen :/
	if (surface == Surface::Left || surface == Surface::Right)
		velocity.x = -velocity.x;

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
			velocity = sf::Vector2f(-velocity.x * scalar, -velocity.y);
	}
	else if (surface == Surface::Diagonal)
	{
		velocity = -velocity;
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
		velocity.x = -velocity.x * scalar;
	}
	// Ball collides with Top or Bottom of Tile
	else if (surface == Surface::Top || surface == Surface::Bottom)
	{
		velocity.y = -velocity.y * scalar;
	}
	else if (surface == Surface::Diagonal)
	{
		velocity = -velocity * scalar;
	}
}
void Ball::handleBorder()
{
	// Window Border collision detection and handling w/ Ball
	float radius = shape.getRadius();

	if (shape.getPosition().x - radius <= 0)
	{
		shape.setPosition(radius, shape.getPosition().y);
		velocity.x = -velocity.x;
	}
	
	if (shape.getPosition().x + radius >= WIDTH)
	{
		shape.setPosition(WIDTH - radius, shape.getPosition().y);
		velocity.x = -velocity.x;
	}
	if (shape.getPosition().y - radius <= 0)
	{
		shape.setPosition(shape.getPosition().x, radius);
		velocity.y = -velocity.y;
	}
	if (shape.getPosition().y + radius >= HEIGHT)
	{
		reset();
	}
}
void Ball::reset()
{
	shape.setPosition(defaultPos);
	velocity = defaultVel;
}
enum class Surface Ball::collision(sf::Vector2f obj_pos, sf::Vector2f obj_diagonal_pos)
{
	sf::Vector2f tile_pos = obj_pos;
	sf::Vector2f tile_diagonal_pos = obj_diagonal_pos;

	sf::Vector2f pos = shape.getPosition();
	float radius = shape.getRadius();

	// Tile			Ball
	// p1	p2		       b_top
	// p3	p4		b_left	     b_right
	//                    b_bottom

	sf::Vector2i p1(tile_pos.x, tile_pos.y);
	sf::Vector2i p2(tile_diagonal_pos.x, tile_pos.y);
	sf::Vector2i p3(tile_pos.x, tile_diagonal_pos.y);
	sf::Vector2i p4(tile_diagonal_pos.x, tile_diagonal_pos.y);

	// Isolate Vertex data Left, Right, Top and Bottom
	sf::Vector2i b_top(pos.x, pos.y - radius);
	sf::Vector2i b_bottom(pos.x, pos.y + radius);
	sf::Vector2i b_left(pos.x - radius, pos.y);
	sf::Vector2i b_right(pos.x + radius, pos.y);

	sf::Vector2i vertex = sf::Vector2i(pos.x, pos.y);
	for (auto p : { p1, p2, p3, p4 })
	{
		// if the distance between any point and the center of the ball is less than the radius of the ball
		double dist = distance(p, vertex);

		if (dist <= double(radius))
		{
			//std::cout << "dist = " << dist << std::endl;
			//std::cout << "radius = " << dist << std::endl;
			return Surface::Diagonal;
		}
	}

	// Determine if any side of ball collides with object
	int i = 0;
	for (auto b : { b_top, b_bottom, b_left, b_right })
	{
		if ((b.x >= tile_pos.x && b.x <= tile_diagonal_pos.x) &&
			(b.y >= tile_pos.y && b.y <= tile_diagonal_pos.y))
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