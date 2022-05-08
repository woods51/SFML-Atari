#pragma once
#include "Ball.h"

////////////////////////////////////////////////////////////
/// \brief PongBall Class.
///
/// This class inherits members from the Pong Class.
/// This class create Pong Balls which handles movement,
/// collisions and the screen border in unique ways.
/// 
////////////////////////////////////////////////////////////
class PongBall : public Ball
{
public:
	////////////////////////////////////////////////////////////
	/// \brief Constructor for the PongBall class.
	///
	/// Constructs PongBall object using provided parameters.
	/// Calls Ball constructor.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_pos		--> Position
	/// \param a_radius		--> Radius
	/// \param a_scale		--> Scale
	/// \param a_textureKey	--> Key for texture
	////////////////////////////////////////////////////////////
	PongBall(ResourceManager& a_rm, sf::Vector2f a_pos = sf::Vector2f(WIDTH / 2 - 12, HEIGHT / 2 - 32), float a_radius = 12.0f,
		sf::Vector2f a_scale = sf::Vector2f(1.0f, 1.0f), std::string a_textureKey = "ball_01")
		: Ball(a_rm, a_pos, a_radius, a_scale, a_textureKey)
	{
		m_velocity = sf::Vector2f(velocityRNG(sf::Vector2i(6, 9)), velocityRNG(sf::Vector2i(2, 5)));
	};

	////////////////////////////////////////////////////////////
	/// \brief Updates ball position and direction.
	///
	/// This function moves the position of the ball according to deltaTime
	/// and updates current direction according to velocity.
	/// Changes to values m_direction is set to.
	/// 
	/// \param a_rm	--> ResourceManager
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	void move(ResourceManager& a_rm, const sf::Time& a_dt) override;

	////////////////////////////////////////////////////////////
	/// \brief Handles collision with screen bounds.
	///
	/// This function adjusts the ball if positioned outside the
	/// bounds of the playable screen defined by WIDTH and HEIGHT.
	/// Adjusts position and velocity.
	/// Replicates Pong border effects
	/// 
	/// \param a_rm --> ResourceManager
	////////////////////////////////////////////////////////////
	void handleBorder(ResourceManager& a_rm) override;

	////////////////////////////////////////////////////////////
	/// \brief Handles collision with paddle.
	///
	/// This function adjusts the velocity of the ball
	/// according to the direction of the paddle and
	/// surface of contact. Velocity changes specific to Pong.
	/// 
	/// \param a_surface	--> Surface of contact
	/// \param a_paddleDir	--> Current direction of paddle
	////////////////////////////////////////////////////////////
	void handlePaddle(enum class Surface a_surface, enum class Direction a_paddleDir) override;

	////////////////////////////////////////////////////////////
	/// \brief Resets ball.
	///
	/// This function sets the position to its default value
	/// and generates random velocity. This function is set
	/// specifically for Pong gameplay.
	////////////////////////////////////////////////////////////
	void reset() override;

	// Returns player scores as sf::Vector2i (p1_score, p2_score)
	sf::Vector2i getScores() const { return sf::Vector2i(m_scoreP1, m_scoreP2); };

protected:	

	// Data members
	float m_pongScalar = 1.02f;

	// Player Scores
	int m_scoreP1 = 0;
	int m_scoreP2 = 0;
};
