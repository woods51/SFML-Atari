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
void Tile::handleBall()
{
	m_durability--;
	if (m_durability <= 0)
	{
		setDeactive();
	}
	else
		m_shape.setTexture(m_secondTexture);

	return;
}