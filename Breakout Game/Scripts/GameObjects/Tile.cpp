#include "Tile.h"

Tile::Tile()
{
	isActive(true);
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
		isActive(false);
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
void Tile::updateTile(ResourceManager& a_rm, TileType a_type, std::string a_textureKey)
{
	m_type = a_type;
	m_defaultTexture = a_textureKey;
	m_firstTexture = a_rm.getTexture(m_defaultTexture);
	m_shape.setTexture(m_firstTexture);

	isActive(true);
	return;
}
void Tile::setTile(ResourceManager& a_rm, TileType a_type, std::string a_textureKey)
{
	m_type = a_type;
	m_durability = 1;
	m_isDestructable = true;

	m_defaultTexture = a_textureKey;
	m_firstTexture = a_rm.getTexture(a_textureKey);
	sf::Texture* texture = m_firstTexture;
	switch (a_type)
	{
	case TileType::Default:
		break;
	case TileType::Lock:
		m_durability = 2;
		m_secondTexture = a_rm.getTexture(a_textureKey + "_lock");
		texture = m_secondTexture;
		break;
	case TileType::Lock2:
		m_durability = 3;
		m_secondTexture = a_rm.getTexture(a_textureKey + "_lock");
		m_thirdTexture = a_rm.getTexture(a_textureKey + "_lock2");
		texture = m_thirdTexture;
		break;
	case TileType::Special:
		m_defaultTexture = "tile_special";
		m_firstTexture = a_rm.getTexture(m_defaultTexture);
		texture = m_firstTexture;
		break;
	case TileType::Special2:
		m_defaultTexture = "tile_special2";
		m_firstTexture = a_rm.getTexture(m_defaultTexture);
		texture = m_firstTexture;
		break;
	case TileType::Special3:
		m_defaultTexture = "tile_special3";
		m_firstTexture = a_rm.getTexture(m_defaultTexture);
		texture = m_firstTexture;
		break;
	case TileType::Wall:
		m_defaultTexture = a_textureKey;
		m_firstTexture = a_rm.getTexture(m_defaultTexture);
		texture = m_firstTexture;
		m_isDestructable = false;
		break;
	case TileType::Blank:
		m_defaultTexture = "tile_08";
		m_firstTexture = a_rm.getTexture(m_defaultTexture);
		texture = m_firstTexture;
		break;
	default:
		break;
	}
	m_shape.setTexture(texture);
}
void Tile::reset()
{
	isActive(true);
	m_durability = 1;
	m_isDestructable = true;

	sf::Texture* texture = m_firstTexture;
	switch (m_type)
	{
	case TileType::Wall:
		m_isDestructable = false;
		break;
	case TileType::Lock:
		m_durability = 2;
		texture = m_secondTexture;
		break;
	case TileType::Lock2:
		m_durability = 3;
		texture = m_thirdTexture;
		break;
	default:
		break;
	}
	m_shape.setTexture(texture);
}