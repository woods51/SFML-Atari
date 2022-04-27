#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "../Menus/PauseState.h"
#include "../../GameObjects/PongBall.h"
#include "../../GameObjects/Paddle.h"

////////////////////////////////////////////////////////////
/// \brief PongState Class.
///
/// This class inherits members from the State Class. The PongState class
/// is a Game State which runs the Pong game program.
/// 
////////////////////////////////////////////////////////////
class PongState : public State
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the Pong State.
	///
	/// Constructs ball and paddle game objects and generates user interfaces.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	////////////////////////////////////////////////////////////
	PongState(ResourceManager& a_rm, sf::RenderWindow& a_window);

	////////////////////////////////////////////////////////////
	/// \brief Handles sfml window events.
	///
	/// This function handles any window events that occur
	/// in the render window. This function calls the inputHandler
	/// and also handles any mouse events including button interactions.
	/// This function is responsible for pushing new game states
	/// and is called once every frame.
	/// 
	/// \param a_rm          --> ResourceManager
	/// \param a_window --> RenderWindow
	/// \param a_states     --> Vector of game states
	////////////////////////////////////////////////////////////
	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;

	////////////////////////////////////////////////////////////
	/// \brief Provides game updates.
	///
	/// This function computes in-game physics, collisions,
	/// movement, and updates user interfaces. This function may
	/// also set the m_gameOver flag or trigger a level complete.
	/// 
	/// \param a_rm	--> ResourceManager
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	void update(ResourceManager& a_window, const sf::Time& a_dt) override;

	////////////////////////////////////////////////////////////
	/// \brief Draws objects to RenderWindow.
	///
	/// This function draws objects to the RenderWindow.
	/// This function is called once every frame.
	/// 
	/// \param a_window --> RenderWindow
	////////////////////////////////////////////////////////////
	void render(sf::RenderWindow& a_window) override;

	////////////////////////////////////////////////////////////
	/// \brief Destructor for PongState. 
	///
	///	Deconstructs and frees any dynmically allocated memory.
	////////////////////////////////////////////////////////////
	~PongState();
private:

	////////////////////////////////////////////////////////////
	/// \brief Handles keyboard inputs.
	///
	/// This function handles any keyboard inputs,
	/// it will handle both key pressed down and released events.
	/// This function is called every frame.
	/// 
	/// \param a_key		--> Position of Tile
	/// \param a_isPressed	--> True if key pressed, False if released
	////////////////////////////////////////////////////////////
	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed);

	////////////////////////////////////////////////////////////
	/// \brief Handles and checks for any button events.
	///
	/// This function works in part with the eventHandler to
	/// compute whether the user has clicked a button when
	/// a left mouse button is triggered.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	/// \param a_states		--> Vector of game states
	/// \param a_mousePosition	--> Current mouse position
	////////////////////////////////////////////////////////////
	void handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
		const sf::Vector2f& a_mousePosition);

	////////////////////////////////////////////////////////////
	/// \brief Updates user interfaces.
	///
	/// This function updates user interfaces.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void updateUI();

	////////////////////////////////////////////////////////////
	/// \brief Generates state user interfaces.
	///
	/// This function generates and initializes all
	/// UI elements for its state.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateUI(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Generates text objects.
	///
	/// This function generates and initializes all
	/// text objects.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateText(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Handles ball collisions and physics.
	///
	/// This function computes and handles collisions for the balls
	/// and paddle objects. This function handles collisions between
	/// the balls and paddles.
	/// 
	/// \param a_rm	--> ResourceManager
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	void handleBallPhysics(ResourceManager& a_rm, const sf::Time& a_dt);

	// Buttons
	std::vector<Button*> m_buttons;

	// Game objects
	std::vector<Paddle*> m_paddles;
	Paddle* m_paddleP1;
	Paddle* m_paddleP2;
	PongBall* m_ball;

	// Background
	sf::Sprite m_border;
	sf::Texture m_frameTexture;

	// Text objects
	sf::Text m_startText;
	sf::Text m_player1;
	sf::Text m_player2;
	sf::Text m_scoreP1Text;
	sf::Text m_scoreP2Text;

	// Scores
	sf::Vector2i m_pScores = sf::Vector2i(0, 0);
	
};