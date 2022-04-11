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
	
	switch (m_durability)
	{
	case 0:
		setDeactive();
		break;
	case 1:
		m_shape.setTexture(m_firstTexture);
		break;
	case 2:
		m_shape.setTexture(m_secondTexture);
		break;
	case 3:
		m_shape.setTexture(m_thirdTexture);
		break;
	default:
		break;
	}

	return;
}