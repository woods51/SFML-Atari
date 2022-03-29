#include "Tile.h"

Tile::Tile()
{
	setActive();
}

sf::Vector2f Tile::getDiagonalPosition() const
{
	return sf::Vector2f(m_shape.getPosition().x + m_shape.getGlobalBounds().width,
		m_shape.getPosition().y + m_shape.getGlobalBounds().height);
}