#include "Tile.h"

Tile::Tile() {

}
sf::Vector2f Tile::getDiagonalPos()
{
	return sf::Vector2f(shape.getPosition().x + (shape.getSize().x * shape.getScale().x),
		shape.getPosition().y + (shape.getSize().y * shape.getScale().y));
}