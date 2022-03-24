#include "Tile.h"

Tile::Tile() {

}
sf::Vector2f Tile::getDiagonalPos()
{
	return sprite.getPosition() + (size * sprite.getScale().x);
}

float Tile::distance(sf::Vector2f p1, sf::Vector2f p2)
{
	return abs(p1.y-p2.y) * 1.0;
}

enum class Surface Tile::collision(sf::Vector2f ball_pos, sf::Vector2f ball_diagonal_pos)
{
	sf::Vector2f pos = this->sprite.getPosition();
	sf::Vector2f diagonal_pos = this->getDiagonalPos();
	// Tile			Ball
	// p1	p2		b1	 b2
	// p3	p4		b3	 b4
	sf::Vector2f p1(pos.x, pos.y);
	sf::Vector2f p2(diagonal_pos.x, pos.y);
	sf::Vector2f p3(pos.x, diagonal_pos.y);
	sf::Vector2f p4(diagonal_pos.x, diagonal_pos.y);

	sf::Vector2f b1(ball_pos.x, ball_pos.y);
	sf::Vector2f b2(ball_diagonal_pos.x, ball_pos.y);
	sf::Vector2f b3(ball_pos.x, ball_diagonal_pos.y);
	sf::Vector2f b4(ball_diagonal_pos.x, ball_diagonal_pos.y);

	bool coll = false;
	bool point[4] = { false, false, false, false };
	int num = 0;

	int i = 0;
	for (auto b : { b1, b2, b3, b4 })
	{
		if ((b.x >= pos.x && b.x <= diagonal_pos.x) &&
			(b.y >= pos.y && b.y <= diagonal_pos.y))
		{
			coll = true;
			point[i] = true;
			num++;
		}
		i++;
		continue;
	}

	float p1_dist, p2_dist, p3_dist, p4_dist;
	if (!coll)
		return Surface::None;
	for (int i = 0; i < 4; i++) {
		bool x = point[i];
		//std::cout << "p" << i + 1 << ": " << (point[i] ? "True" : "False") << "  ";
	}
	//std::cout<<std::endl;
	if (num == 1)
	{
		for (int i = 0; i < 4; i++) {
			if (point[i])
			{
				switch (i + 1) {
				case 1:
					p2_dist = distance(b1, p2);
					p4_dist = distance(b1, p4);

					if (p2_dist < p4_dist)
						return Surface::Right;
					else
						return Surface::Bottom;
					break;

				case 2:
					p1_dist = distance(b2, p1);
					p3_dist = distance(b2, p3);

					if (p1_dist < p3_dist)
						return Surface::Left;
					else
						return Surface::Bottom;
					break;

				case 3:
					p2_dist = distance(b3, p2);
					p4_dist = distance(b3, p4);

					if (p2_dist < p4_dist)
						return Surface::Top;
					else
						return Surface::Right;
					break;

				default:
					p1_dist = distance(b4, p1);
					p3_dist = distance(b4, p3);

					if (p1_dist < p3_dist) {
						//std::cout << "Top" << std::endl;
						return Surface::Top;
					}
					else {
						//std::cout << "Left" << std::endl;
						return Surface::Left;
					}
					break;
				}

			}
		}
	}
	else
	{
		if (point[0] && point[1])
			return Surface::Bottom;
		if (point[0] && point[2])
			return Surface::Right;
		if (point[1] && point[3])
			return Surface::Left;
		if (point[2] && point[3])
			return Surface::Top;
	}
	return Surface::None;
}