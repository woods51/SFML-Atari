#pragma once
#include "State.h"
#include "OptionsState.h"
#include <fstream>
#include <sstream>
#include <iostream>

class LoadMenu : public State
{
public:
	LoadMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Sprite*, sf::Sprite*);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time a_dt, ResourceManager& a_rm) override;
	void render(sf::RenderWindow& a_window) override;

	~LoadMenu();

private:
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
	int m_currentPage = 1;
	int m_maxPages = 1;
	sf::Text m_pageNumber;
	sf::Sprite m_levelSelector;
	sf::Text m_selectedLevel;
	sf::Text m_loadError;

	void generateUI(ResourceManager& a_rm);
	void loadPage(int start, int end);
	void loadFirstPage();
	void nextPage();
	void previousPage();
	void loadFiles();
	bool loadMap(ResourceManager& a_rm, std::string a_path);
	bool parseTileData(ResourceManager& a_rm, std::string a_line);
	TileType getTileType(int a_type);
};
