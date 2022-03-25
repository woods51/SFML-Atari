#include "Tile.h"

Tile::Tile() {

}
sf::Vector2f Tile::getDiagonalPos()
{
	return sprite.getPosition() + (size * sprite.getScale().x);
}

double Tile::distance(sf::Vector2i p1, sf::Vector2i p2)
{
	return (double)sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

enum class Surface Tile::collision(sf::Vector2f ball_pos, sf::Vector2f ball_diagonal_pos)
{
	sf::Vector2f pos = this->sprite.getPosition();
	sf::Vector2f diagonal_pos = this->getDiagonalPos();

	// Tile			Ball
	// p1	p2		       b_top
	// p3	p4		b_left	     b_right
	//                    b_bottom

	sf::Vector2i p1(pos.x, pos.y);
	sf::Vector2i p2(diagonal_pos.x, pos.y);
	sf::Vector2i p3(pos.x, diagonal_pos.y);
	sf::Vector2i p4(diagonal_pos.x, diagonal_pos.y);

	// Isolate Vertex data Left, Right, Top and Bottom
	sf::Vector2i b_top(ball_pos.x + (ball_diagonal_pos.x - ball_pos.x), ball_pos.y);
	sf::Vector2i b_bottom(ball_pos.x + (ball_diagonal_pos.x - ball_pos.x), ball_diagonal_pos.y);
	sf::Vector2i b_left(ball_pos.x, ball_pos.y + (ball_diagonal_pos.y - ball_pos.y));
	sf::Vector2i b_right(ball_diagonal_pos.x, ball_pos.y + (ball_diagonal_pos.y - ball_pos.y));

	sf::Vector2i vertex = b_top + ((b_bottom - b_top) / 2);
	double radius = distance(b_top, b_bottom) / 2;

	for (auto p : { p1, p2, p3, p4 })
	{
		// if the distance between any point and the center of the ball is less than the radius of the ball
		double dist = distance(p, vertex);

		if (dist <= radius)
		{
			std::cout << "dist = " << dist << std::endl;
			std::cout << "radius = " << dist << std::endl;
			return Surface::Diagonal;
		}
	}

	// Determine if any side of ball collides with object
	int i = 0;
	for (auto b : { b_top, b_bottom, b_left, b_right })
	{
		if ((b.x >= pos.x && b.x <= diagonal_pos.x) &&
			(b.y >= pos.y && b.y <= diagonal_pos.y))
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