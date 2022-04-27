#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "OptionsState.h"
#include "../../GameObjects/Tile.h"
#include "../../GameObjects/Button.h"
#include "../../LevelLoader.h"

////////////////////////////////////////////////////////////
/// \brief LoadMenu Class.
///
/// This class inherits members from the State Class. The LoadMenu class
/// is a Menu State which provides an interface for the user to view custom
/// created levels. The user can them load them into the Breakout game state.
/// 
////////////////////////////////////////////////////////////
class LoadMenu : public State
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for LoadMenu class
	///
	/// Constructs and sets the chosen lives value, generates user interfaces,
	/// loads level data and loads first page.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	/// \param a_background	--> Background texture
	/// \param a_background2	--> Offset background texture
	/// \param a_lives		--> Breakout lives value
	////////////////////////////////////////////////////////////
	LoadMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Sprite* a_background, sf::Sprite* a_background2, unsigned int a_lives);

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
	/// \brief Updates user interfaces.
	///
	/// This function updates user interfaces, specifically
	/// updates page number text and background positions.
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
	/// \brief Destructor for LoadMenu. 
	///
	///	Deconstructs and frees any dynmically allocated memory.
	////////////////////////////////////////////////////////////
	~LoadMenu();

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
	/// \param a_window	-->	RenderWindow
	/// \param a_rm		-->	ResourceManager
	/// \param a_mousePosition	-->	Mouse position
	////////////////////////////////////////////////////////////
	void buttonSelectUpdate(sf::RenderWindow& a_window, ResourceManager& a_rm, const sf::Vector2f& a_mousePosition);

	////////////////////////////////////////////////////////////
	/// \brief Generates user interfaces
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
	/// \brief Loads text data into level buttons.
	///
	/// This function loads the text data from m_levels defined between the
	/// start and end indexes into text objects of the level buttons.
	/// 
	/// \param a_start	-->	Start index
	/// \param a_end	-->	End index
	////////////////////////////////////////////////////////////
	void loadPage(int start, int end);

	////////////////////////////////////////////////////////////
	/// \brief Loads irst page of levels.
	///
	/// This function loads the initial text data from m_levels
	/// to fill out the first page of levels.
	/// 
	////////////////////////////////////////////////////////////
	void loadFirstPage();

	////////////////////////////////////////////////////////////
	/// \brief Loads the next page of levels.
	///
	/// This function loads the next page of levels using
	/// incremented indices for the text data in m_levels.
	/// 
	////////////////////////////////////////////////////////////
	void nextPage();

	////////////////////////////////////////////////////////////
	/// \brief Loads the previous page of levels.
	///
	/// This function loads the next page of levels using
	/// decremented indices for the text data in m_levels.
	/// 
	////////////////////////////////////////////////////////////
	void previousPage();

	// Levels
	std::vector<Tile*> m_tileMap;
	std::vector<std::string> m_levels;
	LevelLoader m_loader;

	// Buttons
	std::vector<Button*> m_buttons;
	std::vector<Button*> m_levelButtons;
	
	// Backgrounds
	sf::Sprite m_overlay;
	sf::Sprite* m_background;
	sf::Sprite* m_background2;

	// Text objects
	sf::Text m_pageNumber;
	sf::Text m_levelText;
	sf::Text m_selectedLevel;
	sf::Text m_loadError;

	// Flags
	bool m_isValid = false;
	
	// Data members
	std::string m_errorMsg;
	std::string m_loadPath = "SavedLevels/";
	unsigned int m_currentPage = 1;
	unsigned int m_maxPages = 1;
	unsigned int m_lives;
};
