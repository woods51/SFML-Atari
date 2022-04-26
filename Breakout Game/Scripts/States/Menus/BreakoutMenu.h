#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "../Games/BreakoutState.h"
#include "../Games/LevelEditor.h"
#include "../Menus/OptionsState.h"
#include "../Menus/LoadMenu.h"
#include "../../GameObjects/Button.h"

////////////////////////////////////////////////////////////
/// \brief BreakoutMenu Class.
///
/// This class inherits members from the State Class. The BreakoutMenu class
/// is a Menu State which provides an interface for the user to choose
/// between loading a custom level or playing the default breakout game.
/// 
////////////////////////////////////////////////////////////
class BreakoutMenu : public State
{
public:
	////////////////////////////////////////////////////////////
	/// \brief Constructor for BreakoutMenu class
	///
	/// Constructs the BreakoutMenu object and generates user interfaces.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	////////////////////////////////////////////////////////////
	BreakoutMenu(ResourceManager& a_rm, sf::RenderWindow& a_window);

	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;

	////////////////////////////////////////////////////////////
	/// \brief Updates user interfaces.
	///
	/// This function updates user interfaces, specifically
	/// updates text positions and updates background positions.
	/// 
	/// \param a_rm	--> ResourceManager
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	void update(ResourceManager& a_rm, sf::Time a_dt) override;
	void render(sf::RenderWindow& a_window) override;

	////////////////////////////////////////////////////////////
	/// \brief Destructor for BreakoutMenu. 
	///
	///	Deconstructs and frees any dynmically allocated memory.
	////////////////////////////////////////////////////////////
	~BreakoutMenu();

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

	// Title
	sf::Sprite m_breakoutTitle;

	// Text
	sf::Text m_livesText;
	sf::Text m_livesValue;

	// Backgrounds
	sf::Texture m_frameTexture;
	sf::Sprite m_background;
	sf::Sprite m_background2;

	// In-Game lives
	unsigned int m_lives[6]{ 1, 3, 5, 10, 15, 99 };
	int m_livesIndex = 2;
};