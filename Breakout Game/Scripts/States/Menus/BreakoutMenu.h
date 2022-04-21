#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "../Games/BreakoutState.h"
#include "../Games/LevelEditor.h"
#include "../Menus/OptionsState.h"
#include "../Menus/LoadMenu.h"
#include "../../GameObjects/Button.h"

class BreakoutMenu : public State
{
public:
	////////////////////////////////////////////////////////////
	/// \brief Constructor for BreakoutMenu class
	///
	/// Constructs the BreakoutMenu object, calls generateUI.
	/// 
	/// \param a_window	-->	RenderWindow
	/// \param a_rm		-->	ResourceManager
	////////////////////////////////////////////////////////////
	BreakoutMenu(ResourceManager& a_rm, sf::RenderWindow& a_window);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time a_dt, ResourceManager& a_rm) override;
	void render(sf::RenderWindow& a_window) override;

	~BreakoutMenu();

private:

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

	// Buttons
	std::vector<Button*> m_buttons;

	// Text
	sf::Sprite m_breakoutText;
	sf::Sprite m_editorText;
	sf::Sprite m_loadText;

	// Backgrounds
	sf::Sprite m_background;
	sf::Sprite m_background2;
	sf::Texture m_frameTexture;

	// Title Flags
	bool m_breakoutFlag = false;
	bool m_pongFlag = false;
	bool m_editorFlag = false;

	unsigned int m_lives[6]{ 1, 3, 5, 10, 15, 99 };
	int m_livesIndex = 2;
	sf::Text m_livesText;
	sf::Text m_livesValue;
};