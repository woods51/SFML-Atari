#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "OptionsMenu.h"

////////////////////////////////////////////////////////////
/// \brief SaveMenu Class.
///
/// This class inherits members from the State Class. The SaveMenu class
/// is a Menu State which provides an interface for the user to save custom
/// levels created in the LevelEditor state.
/// 
////////////////////////////////////////////////////////////
class SaveMenu : public State
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the Save Menu
	///
	/// Constructs and sets background to frame texture, generates
	/// user interfaces and sets tile map.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	/// \param a_frameTexture	--> Pause frame texture
	/// \param a_tileMap	--> Tile map to save
	////////////////////////////////////////////////////////////
	SaveMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture, const std::vector<Tile*> a_tileMap);

	////////////////////////////////////////////////////////////
	/// \brief Handles SFML window events.
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
	/// This function updates the file name validity icon texture.
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
	/// \brief Destructor for SaveMenu. 
	///
	///	Deconstructs and frees any allocated memory.
	////////////////////////////////////////////////////////////
	~SaveMenu();

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
	/// \brief Generates state user interfaces
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
	/// \brief Handles text input from keyboard
	///
	/// This function handles any keyboard text inputs and
	/// adds unicode character to file name if valid.
	/// 
	/// \param a_unicode		--> Unicode of keycode
	////////////////////////////////////////////////////////////
	void handleTextInput(sf::Uint32 a_unicode);

	////////////////////////////////////////////////////////////
	/// \brief Writes and saves current tile map to file
	///
	/// This function parses tile data from m_tileMap into an output buffer,
	/// this buffer is written to the output file, which name is specified by m_fileName.
	/// Data outputted in a csv format and written to a
	/// file in the SavedLevels/ directory.
	/// 
	////////////////////////////////////////////////////////////
	void saveMap();

	////////////////////////////////////////////////////////////
	/// \brief Verifies that file name doesn't exist.
	///
	/// This function verifies that the given file name doesn't
	///	exist already in the SavedLevels/ directory. A file name is
	/// considered valid as long as a file with same name doesn't exist.
	/// 
	/// \param a_fileName	--> Ansi string file name
	/// 
	/// \returns True if a_fileName is valid, false otherwise
	////////////////////////////////////////////////////////////
	bool verifyFileName(std::string a_fileName);

	// Loaded tile map
	std::vector<Tile*> m_tileMap;

	// Buttons
	std::vector<Button*> m_buttons;
	Button* m_editButton;

	// Backgrounds
	sf::Texture m_frameTexture;
	sf::Sprite m_frameSprite;
	sf::Sprite m_overlay;

	// Sprites
	sf::Sprite m_typeLine;
	sf::Sprite m_fileValid;

	// Text objects
	sf::Text m_saveResult;
	sf::Text m_fileName;

	// Text field
	std::string m_textInput;
	
	// States
	bool m_isTyping = false;
	bool m_isSaving = false;
	bool m_isFileValid = false;

	// Data members
	std::string m_savePath = "SavedLevels/";
};
