#pragma once
#include "stdafx.h"
#include "Tile.h"

class Paddle : public Tile
{
public:
	Paddle(ResourceManager& a_rm, sf::Vector2f a_pos = sf::Vector2f(WIDTH/2 - 160.0f, 640.0f),
		sf::Vector2f a_size = sf::Vector2f(64.0f, 4.0f), sf::Vector2f a_scale = sf::Vector2f(5.0f, 5.0f),
		std::string a_textureKey = "paddle")
		: Tile(a_rm, a_pos, TileType::Default, a_textureKey)
	{
		m_shape.setSize(a_size);
		m_shape.setScale(a_scale);

		m_defaultPos = a_pos;
		m_currentDir = Direction::Idle;
		m_IsMovingLeft = false;
		m_IsMovingRight = false;
		m_IsMovingUp = false;
		m_IsMovingDown = false;
	};

	void move(sf::Time dt);
	void reset();

	void handleBorder();

	// Sets current direction
	void setDirection(enum class Direction direction) { m_currentDir = direction; }


	// Returns current direction
	enum class Direction getDirection() const { return m_currentDir; }

	bool m_IsMovingLeft, m_IsMovingRight, m_IsMovingUp, m_IsMovingDown;

private:
	
	enum class Direction m_currentDir;
	float m_speed = 12.0f;
	bool m_isColliding = false;

	sf::Vector2f m_defaultPos;
};