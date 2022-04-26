#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "OptionsState.h"

class SaveMenu : public State
{
public:
	SaveMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture, const std::vector<Tile*> a_tileMap);

	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_rm, sf::Time a_dt) override;
	void render(sf::RenderWindow& a_window) override;

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
	/// \brief Handles text input from keyboard
	///
	/// This function handles any keyboard text inputs and
	/// adds unicode character to file name if valid.
	/// 
	/// \param a_unicode		--> Unicode of keycode
	////////////////////////////////////////////////////////////
	void handleTextInput(sf::Uint32 a_unicode);

	void saveMap();
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
	sf::Text m_saveState;
	sf::Text m_fileName;

	// Text field
	std::string m_textInput;
	
	// States
	bool m_isTyping = false;
	bool m_isSaving = false;
	bool m_isFileValid = false;
};
