#pragma once
#include "../stdafx.h"
#include "../ResourceManager.h"

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
	Ball(ResourceManager& a_rm, sf::Vector2f a_pos = sf::Vector2f(WIDTH/2 - 12, 610), float a_radius = 12,
		sf::Vector2f a_scale = sf::Vector2f(1, 1), std::string a_textureKey = "ball_01")
	{
		m_startPos = a_pos;
		m_startVel = sf::Vector2f(velocityRNG(), -m_speed);
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
	/// \param a_rm	--> ResourceManager
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	virtual void move(ResourceManager& a_rm, sf::Time a_dt);

	////////////////////////////////////////////////////////////
	/// \brief Handles collision with screen bounds.
	///
	/// This function adjusts the ball if positioned outside the
	/// bounds of the playable screen defined by WIDTH and HEIGHT.
	/// Adjusts position and velocity.
	/// 
	/// \param a_rm --> ResourceManager
	////////////////////////////////////////////////////////////
	virtual void handleBorder(ResourceManager& a_rm);

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
	virtual void handlePaddle(enum class Surface a_surface, enum class Direction a_paddleDir);

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
	/// Increments m_colorIndex and switches ball color.
	///
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void toggleColor(ResourceManager& a_rm);

	// Returns texture pointer
	const sf::Texture* getTexture() const { return m_shape.getTexture(); }

	// Returns const reference to shape object
	const sf::CircleShape& getShape() const { return m_shape; }

	// Returns position
	inline sf::Vector2f getPosition() const { return m_shape.getPosition(); }

	// Computes and returns diagonal position
	sf::Vector2f getDiagonalPosition() const;

	// Returns current direction
	enum class Direction getDirection() const { return m_direction; }

	// Returns radius
	float getRadius() const { return m_shape.getRadius(); }

	// Returns velocity
	sf::Vector2f getVelocity() const { return m_velocity; }

	// Returns isActive state
	inline bool isActive() const { return m_isActive; }

	// Returns isColliding state
	inline bool isColliding() const { return m_isColliding; }

	////////////////////////////////////////////////////////////
	/// \brief Sets isColliding state
	///
	/// \param a_isColliding	--> State
	////////////////////////////////////////////////////////////
	inline void isColliding(bool a_isColliding) { m_isColliding = a_isColliding; }

	////////////////////////////////////////////////////////////
	/// \brief Sets isActive state
	///
	/// This function sets isActive state and makes adjustments
	/// to velocity and position according to state.
	/// 
	/// \param a_isActive	--> State
	////////////////////////////////////////////////////////////
	void isActive(bool a_isActive);

	////////////////////////////////////////////////////////////
	/// \brief Sets velocity scalar
	///
	/// \param a_scalar	--> Scalar
	////////////////////////////////////////////////////////////
	void setScalar(float a_scalar) { m_scalar = a_scalar; }

	////////////////////////////////////////////////////////////
	/// \brief Sets position
	///
	/// \param a_pos	--> Position
	////////////////////////////////////////////////////////////
	void setPosition(sf::Vector2f a_position) { m_shape.setPosition(a_position); }

	////////////////////////////////////////////////////////////
	/// \brief Sets velocity
	///
	/// \param a_vel	--> Velocity
	////////////////////////////////////////////////////////////
	void setVelocity(sf::Vector2f a_velocity) { m_velocity = a_velocity; }

protected:

	////////////////////////////////////////////////////////////
	/// \brief Computes distance between two 2D vectors
	///
	/// \param a_p1	--> Vector 1
	/// \param a_p2	--> Vector 2
	///
	/// \return Distance between 2D vectors as double.
	///
	////////////////////////////////////////////////////////////
	double vectorDistance(const sf::Vector2i& a_p1, const sf::Vector2i& a_p2) const;

	////////////////////////////////////////////////////////////
	/// \brief Resets ball.
	///
	/// This function sets the position to its default value
	/// and generates random velocity.
	////////////////////////////////////////////////////////////
	void reset();

	////////////////////////////////////////////////////////////
	/// \brief Random velocity number generator
	/// 
	/// This function computes a random floating
	/// point number between prespecified values
	///
	/// \return Velocity as floating point number
	////////////////////////////////////////////////////////////
	float velocityRNG();

	// Properties
	sf::CircleShape m_shape;
	enum class Direction m_direction = Direction::Idle;

	// Position and Velocity
	sf::Vector2f m_startPos;
	sf::Vector2f m_startVel;
	sf::Vector2f m_velocity;
	float m_speed = 8;
	float m_scalar = 1.01f;

	// States
	bool m_isActive = false;
	bool m_isColliding = false;
	
	// Color Change Variables
	std::string m_colors[6] = { "ball_01", "ball_02", "ball_03", "ball_04", "ball_05", "ball_06" };
	int m_colorIndex = 0;
	
};