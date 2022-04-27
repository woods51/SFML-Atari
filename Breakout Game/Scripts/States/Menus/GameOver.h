#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "OptionsState.h"
#include "../../GameObjects/Button.h"

////////////////////////////////////////////////////////////
/// \brief GameOver Class.
///
/// This class inherits members from the State Class. The GameOver class
/// is a Menu State which provides an interface for the user to view
/// their final breakout score. The user can either return to a prior menu state
/// or exit the game.
/// 
////////////////////////////////////////////////////////////
class GameOver : public State
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for GameOver class
	///
	/// Constructs and sets the score value and generates user interfaces.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	/// \param a_frameTexture	--> Background texture
	/// \param a_score		--> BreakoutGame score
	////////////////////////////////////////////////////////////
	GameOver(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture, unsigned int a_score);

	////////////////////////////////////////////////////////////
	/// \brief Handles sfml window events
	///
	/// This function handles any window events that occur
	/// in the render window. This function handles mouse events
	/// including button selections and interactions.
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
	/// This function is unused in the GameOver state.
	/// 
	/// \param a_rm	--> ResourceManager
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	void update(ResourceManager& a_rm, const sf::Time& a_dt) override;

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
	/// \brief Destructor for GameOver. 
	///
	///	Deconstructs and frees any dynmically allocated memory.
	////////////////////////////////////////////////////////////
	~GameOver();

private:

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
	/// \brief Updates selected state of buttons
	///
	/// This function updates the isSelected state of any button
	/// that is within the bounds of the current mouse position.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_mousePosition	--> Current mouse position
	////////////////////////////////////////////////////////////
	void buttonSelectUpdate(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition);

	////////////////////////////////////////////////////////////
	/// \brief Generates state user interfaces
	///
	/// This function generates and initializes all
	/// UI elements for its state.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateUI(ResourceManager& a_rm);

	// Buttons
	std::vector<Button*> m_buttons;

	// Text objects
	sf::Text m_gameText;
	sf::Text m_scoreText;
	sf::Text m_scoreValue;

	// Background
	sf::Texture m_frameTexture;
	sf::Sprite m_frameSprite;
	sf::Sprite m_overlay;

	// Data members
	int m_score;
};