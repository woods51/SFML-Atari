#pragma once
#include "../stdafx.h"
#include "Tile.h"

////////////////////////////////////////////////////////////
/// \brief Paddle Class.
///
/// This class inherits from the members of the Tile Class,
/// it is used to create a instance of a rectangular paddle object.
/// Paddles and supports basic horizontal and vertical movement
/// and tracks direction for tiles.
/// 
////////////////////////////////////////////////////////////
class Paddle : public Tile
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the paddle class.
	///
	/// Constructs paddle object using provided parameters. Calls tile constructor.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_pos		--> Position
	/// \param a_size		--> Size
	/// \param a_scale		--> Scale
	/// \param a_textureKey	--> String key for texture
	////////////////////////////////////////////////////////////
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

	////////////////////////////////////////////////////////////
	/// \brief Handles basic paddle movement
	///
	/// This functions handles paddle movement using the public isMoving variables,
	/// this function also calls handlerBorder.
	/// 
	////////////////////////////////////////////////////////////
	void move(const sf::Time& dt);

	////////////////////////////////////////////////////////////
	/// \brief Returns paddle to inital state
	///
	/// This functions returns paddle to its initial position.
	/// 
	////////////////////////////////////////////////////////////
	void reset() { m_shape.setPosition(m_defaultPos); }

	////////////////////////////////////////////////////////////
	/// \brief Sets Speed
	///
	/// \param a_speed --> Speed
	////////////////////////////////////////////////////////////
	void setSpeed(float a_speed) { m_speed = a_speed; }

	////////////////////////////////////////////////////////////
	/// \brief Sets Border Offset
	///
	/// \param a_offset --> Offset
	////////////////////////////////////////////////////////////
	void setBorderOffset(int a_offset) { m_borderOffset = a_offset; }

	////////////////////////////////////////////////////////////
	/// \brief Sets Direction
	///
	/// \param a_direction --> Direction
	////////////////////////////////////////////////////////////
	void setDirection(Direction a_direction) { m_currentDir = a_direction; }

	// Returns current direction
	enum class Direction getDirection() const { return m_currentDir; }

	// Movement Variables
	bool m_IsMovingLeft, m_IsMovingRight, m_IsMovingUp, m_IsMovingDown;

protected:

	////////////////////////////////////////////////////////////
	/// \brief Handles border collision
	///
	/// This functions handles paddle collision with the window borders
	/// defined under WIDTH and HEIGHT. Prevents paddle from leaving viewable area.
	/// 
	////////////////////////////////////////////////////////////
	void handleBorder();
	
	// Data members
	sf::Vector2f m_defaultPos;
	enum class Direction m_currentDir;
	float m_speed = 16.0f;
	int m_borderOffset = 60;

	// States
	bool m_isColliding = false;
};