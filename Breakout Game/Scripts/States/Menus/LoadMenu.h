#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "OptionsState.h"
#include "../../GameObjects/Tile.h"
#include "../../GameObjects/Button.h"
#include "../../LevelLoader.h"

class LoadMenu : public State
{
public:
	LoadMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Sprite*, sf::Sprite*, unsigned int a_lives);

	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_rm, const sf::Time& a_dt) override;
	void render(sf::RenderWindow& a_window) override;

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

	void loadPage(int start, int end);
	void loadFirstPage();
	void nextPage();
	void previousPage();

	LevelLoader m_loader;
	std::vector<Tile*> m_tileMap;
	std::vector<Button*> m_buttons;
	std::vector<Button*> m_levelButtons;
	std::vector<std::string> m_levels;
	sf::Sprite m_overlay;
	sf::Sprite* m_background;
	sf::Sprite* m_background2;

	sf::Font m_defaultFont;
	sf::Text m_levelText;
	bool m_isValid = false;
	unsigned int m_currentPage = 1;
	unsigned int m_maxPages = 1;
	sf::Text m_pageNumber;
	sf::Sprite m_levelSelector;
	sf::Text m_selectedLevel;
	sf::Text m_loadError;
	std::string m_errorMsg;
	std::string m_loadPath = "SavedLevels/";

	unsigned int m_lives;
};
