#include "Tile.h"

Tile::Tile() {

}
sf::Vector2f Tile::getPosition() const
{
	return m_shape.getPosition();
}
sf::Vector2f Tile::getDiagonalPosition() const
{
	return sf::Vector2f(m_shape.getPosition().x + m_shape.getGlobalBounds().width,
		m_shape.getPosition().y + m_shape.getGlobalBounds().height);
}
const sf::Texture* Tile::getTexture()
{
	return m_shape.getTexture();
}
const sf::RectangleShape& Tile::getShape() const
{
	return m_shape;
}

bool Tile::isActive() const
{
	return m_active;
}
void Tile::setActive()
{
	m_active = true;
}
void Tile::setDeactive()
{
	m_active = false;
}