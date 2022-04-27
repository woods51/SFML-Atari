#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "BreakoutMenu.h"
#include "OptionsMenu.h"
#include "../Games/PongState.h"
#include "../Games/LevelEditor.h"

////////////////////////////////////////////////////////////
/// \brief MainMenu Class.
///
/// This class inherits members from the State Class. The MainMenu class
/// is a Menu State which provides a start menu interface allowing the user
/// to interact with and access all the various games and features of SFML atari.
/// 
////////////////////////////////////////////////////////////
class MainMenu : public State
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the Main Menu.
	///
	/// Constructs the main menu UI elements and
	/// generates animated background textures.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	////////////////////////////////////////////////////////////
	MainMenu(ResourceManager& a_rm, sf::RenderWindow& a_window);

	////////////////////////////////////////////////////////////
	/// \brief Handles sfml window events.
	///
	/// This function handles any window events that occur
	/// in the render window. This function handles mouse events
	/// including button selections and interactions.
	/// 
	/// \param a_rm          --> ResourceManager
	/// \param a_window --> RenderWindow
	/// \param a_states     --> Vector of game states
	////////////////////////////////////////////////////////////
	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;

	////////////////////////////////////////////////////////////
	/// \brief Updates user interfaces.
	///
	/// This function updates user interfaces, specifically
	/// updates background positions.
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
	/// \brief Destructor for MainMenu. 
	///
	///	Deconstructs and frees any dynmically allocated memory.
	////////////////////////////////////////////////////////////
	~MainMenu();

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
	/// \brief Updates selected state of buttons.
	///
	/// This function updates the isSelected state of any button
	/// that is within the bounds of the current mouse position.
	/// This function also sets the title flags depending on
	/// which button is selected.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_mousePosition	--> Current mouse position
	////////////////////////////////////////////////////////////
	void buttonSelectUpdate(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition);

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
	/// \brief Generates button objects.
	///
	/// This function generates and initializes all
	/// button objects.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateButtons(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Generates sprite objects.
	///
	/// This function generates and initializes all
	/// sprite objects.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateSprites(ResourceManager& a_rm);

	// Buttons
	std::vector<Button*> m_buttons;

	// Title sprites
	sf::Sprite m_atariText;
	sf::Sprite m_breakoutText;
	sf::Sprite m_editorText;
	sf::Sprite m_pongText;

	// Backgrounds
	sf::Texture m_frameTexture;
	sf::Sprite m_background;
	sf::Sprite m_background2;

	// Title flags
	bool m_breakoutFlag = false;
	bool m_pongFlag = false;
	bool m_editorFlag = false;
};