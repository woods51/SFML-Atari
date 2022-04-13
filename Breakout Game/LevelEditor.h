#pragma once
#include "State.h"
#include "Tile.h"
#include "PauseState.h"

class LevelEditor : public State
{
public:
	LevelEditor(ResourceManager& a_rm, sf::RenderWindow& a_window);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time dt, ResourceManager& a_window) override;
	void render(sf::RenderWindow& a_window) override;

	~LevelEditor();
private:
	std::vector<std::unique_ptr<Tile>> m_tileMap;
	std::vector<Button*> m_buttons;
	std::vector<TileButton*> m_tileButtons;
	Button* m_pen;
	Button* m_erase;

	sf::Texture m_frameTexture;
	sf::Sprite m_border;

	sf::Text m_toolsText;
	sf::Text m_regularTile;
	sf::Text m_lockedTile;
	sf::Text m_specialTile;
	sf::Text m_wallTile;

	Tile* m_selectedTile;
	sf::Sprite m_previewTile;
	sf::Sprite m_selectorTile;

	void generateTileMap(ResourceManager& a_rm);
	void updateUI(ResourceManager& a_rm);
	void generateUI(ResourceManager& a_rm);
	void generateButtons(ResourceManager& a_rm);
	void generateTileButtons(ResourceManager& a_rm);

	void setDefaultText(ResourceManager& a_rm, sf::Text& a_text, unsigned int a_charSize,
		sf::Vector2f a_pos, std::string a_font = "default", sf::Color fill = sf::Color::White)
	{
		a_text.setFont(*a_rm.getFont(a_font));
		a_text.setFillColor(fill);
		a_text.setCharacterSize(a_charSize);
		a_text.setPosition(a_pos);
	}
};