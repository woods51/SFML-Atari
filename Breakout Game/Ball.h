#pragma once

#include "Tile.h"

class Ball
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the ball class.
	///
	/// Constructs ball object using provided parameters.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_pos		--> Position
	/// \param a_radius		--> Radius
	/// \param a_scale		--> Scale
	/// \param a_textureKey --> Key for texture
	////////////////////////////////////////////////////////////
	Ball(ResourceManager& a_rm, sf::Vector2f a_pos = sf::Vector2f(WIDTH/2, 610.0f), float a_radius = 12.0f,
		sf::Vector2f a_scale = sf::Vector2f(1.0f, 1.0f), std::string a_textureKey = "ball_01")
	{
		m_startPos = a_pos;
		m_startVel = sf::Vector2f(m_speed, -m_speed);
		m_shape.setPosition(a_pos);
		m_shape.setRadius(a_radius);
		m_shape.setScale(a_scale);
		m_shape.setTexture(a_rm.getTexture(a_textureKey));
	};
	
	////////////////////////////////////////////////////////////
	/// \brief Updates ball position and direction.
	///
	/// This function moves the position of the ball according to deltaTime
	/// and updates current direction according to velocity.
	/// 
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	void move(ResourceManager& a_rm, sf::Time a_dt);

	////////////////////////////////////////////////////////////
	/// \brief Handles collision with paddle.
	///
	/// This function adjusts the velocity of the ball
	/// according to the direction of the paddle and
	/// surface of contact.
	/// 
	/// \param a_surface	--> Surface of contact
	/// \param a_paddleDir	--> Current direction of paddle
	////////////////////////////////////////////////////////////
	void handlePaddle(enum class Surface a_surface, enum class Direction a_paddleDir);

	////////////////////////////////////////////////////////////
	/// \brief Handles collision with tile.
	///
	/// This function adjusts the velocity of the ball
	/// according to surface of contact.
	/// 
	/// \param a_surface	--> Surface of contact
	////////////////////////////////////////////////////////////
	void handleTile(enum class Surface a_surface);

	////////////////////////////////////////////////////////////
	/// \brief Handles collision with screen bounds.
	///
	/// This function adjusts the ball if positioned outside the
	/// bounds of the playable screen. Adjusts position and velocity.
	////////////////////////////////////////////////////////////
	void handleBorder(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Resets ball.
	///
	/// This function sets the position and velocity of the ball
	/// to their start values.
	////////////////////////////////////////////////////////////
	void reset();
	
	////////////////////////////////////////////////////////////
	/// \brief Checks for collision between ball and tile.
	///
	/// This function determines if the shapes of the ball and
	/// given tile have intersected. It will first determine whether
	/// the ball has collided with any of the corners of the tile. If
	/// not, it will determine if the ball is intersecting with
	/// any surface of the tile.
	/// 
	/// \param a_tilePos		--> Position of Tile
	/// \param a_tileDiagPos	--> Diagonal Position of Tile
	///
	/// \return Surface of contact, otherwise Surface::None.
	////////////////////////////////////////////////////////////
	enum class Surface collision(sf::Vector2f a_tilePos, sf::Vector2f a_tileDiagPos) const;
	
	////////////////////////////////////////////////////////////
	/// \brief Changes color of ball
	///
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void toggleColor(ResourceManager& a_rm);

	/// Returns texture pointer
	const sf::Texture* getTexture() const { return m_shape.getTexture(); }

	/// Returns const reference to shape object
	const sf::CircleShape& getShape() const { return m_shape; }

	////////////////////////////////////////////////////////////
	/// \brief Computes diagonal position
	///
	/// \return Diagonal position
	////////////////////////////////////////////////////////////
	sf::Vector2f getDiagonalPosition() const;

	// Returns position
	inline sf::Vector2f getPosition() const { return m_shape.getPosition(); }

	// Returns current direction
	enum class Direction getDirection() const { return m_currentDir; }

	// Returns radius
	float getRadius() const { return m_shape.getRadius(); }

	// Returns velocity
	sf::Vector2f getVelocity() const { return m_velocity; }

	// Returns start position
	sf::Vector2f getStartPosition() const { return m_startPos; }

	// Returns start velocity
	sf::Vector2f getStartVelocity() const { return m_startVel; }

	// Returns true if ball is active
	inline bool getActive() const { return m_active; }

	// Sets ball position
	void setPosition(sf::Vector2f a_pos) { m_shape.setPosition(a_pos); }

	// Sets ball velocity
	void setVelocity(sf::Vector2f a_vel) { m_velocity = a_vel; }

	// Sets start position
	void setStartPosition(sf::Vector2f pos) { m_startPos = pos; }

	// Sets start velocity
	void setStartVelocity(sf::Vector2f vel) { m_startVel = vel; }

	// Sets active
	void setActive() { if (!m_active) m_velocity = m_startVel; m_active = true; }

private:
	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;
	float m_speed = 8.0f;
	enum class Direction m_currentDir = Direction::Idle;

	sf::Vector2f m_startPos;
	sf::Vector2f m_startVel;
	bool m_active = false;
	
	std::string m_colors[6] = { "ball_01", "ball_02", "ball_03", "ball_04", "ball_05", "ball_06" };
	int m_colorIndex = 0;
	
};
double vectorDistance(sf::Vector2i a_p1, sf::Vector2i a_p2);