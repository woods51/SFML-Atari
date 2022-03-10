#include "Tile.h"

sf::Vector2f Tile::getDiagonalPos()
{
	return sprite.getPosition() + (size * sprite.getScale().x);
}

float Tile::distance(sf::Vector2f p1, sf::Vector2f p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) * 1.0);
}

enum class Surface Tile::collision(sf::Vector2f obj_pos, sf::Vector2f obj_pos_diagonal)
{
	sf::Vector2f pos = this->sprite.getPosition();
	sf::Vector2f diagonal_pos = this->getDiagonalPos();

	sf::Vector2f b1(obj_pos.x, obj_pos.y);
	sf::Vector2f b2(obj_pos_diagonal.x, obj_pos.y);
	sf::Vector2f b3(obj_pos.x, obj_pos_diagonal.y);
	sf::Vector2f b4(obj_pos_diagonal.x, obj_pos_diagonal.y);

	sf::Vector2f p1(pos.x, pos.y);
	sf::Vector2f p2(diagonal_pos.x, pos.y);
	sf::Vector2f p3(pos.x, diagonal_pos.y);
	sf::Vector2f p4(diagonal_pos.x, diagonal_pos.y);

	bool coll = false;
	bool point[4] = { false, false, false, false };
	int num = 0;

	int i = 0;
	for (auto p : { b1, b2, b3, b4 })
	{
		if ((p.x >= pos.x && p.x <= diagonal_pos.x) &&
			(p.y >= pos.y && p.y <= diagonal_pos.y))
		{
			coll = true;
			point[i] = true;
			num++;
		}
		i++;
		continue;
	}
	// p1	p2
	// p3	p4
	float p1_dist, p2_dist, p3_dist, p4_dist;
	if (!coll)
		return Surface::None;

	if (num == 1)
	{
		for (int i = 0; i < 4; i++) {
			if (point[i])
			{
				switch (i + 1) {
				case 1:
					p2_dist = abs(distance(b1, p2));
					p4_dist = abs(distance(b1, p4));

					if (p2_dist > p4_dist)
						return Surface::Right;
					else
						return Surface::Bottom;
					break;

				case 2:
					p1_dist = abs(distance(b2, p1));
					p3_dist = abs(distance(b2, p3));

					if (p1_dist > p3_dist)
						return Surface::Left;
					else
						return Surface::Bottom;
					break;

				case 3:
					p2_dist = abs(distance(b3, p2));
					p4_dist = abs(distance(b3, p4));

					if (p2_dist > p4_dist)
						return Surface::Top;
					else
						return Surface::Right;
					break;

				default:
					p1_dist = abs(distance(b4, p1));
					p3_dist = abs(distance(b4, p3));

					if (p1_dist > p3_dist)
						return Surface::Top;
					else
						return Surface::Left;
					break;
				}

			}
		}
	}
	else
	{
		if (point[0] && point[1])
			return Surface::Top;
		if (point[0] && point[2])
			return Surface::Right;
		if (point[1] && point[3])
			return Surface::Left;
		if (point[2] && point[3])
			return Surface::Bottom;
	}
	return Surface::None;
}