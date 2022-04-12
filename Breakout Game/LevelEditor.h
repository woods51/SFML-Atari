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
	sf::Texture m_frameTexture;
	sf::Sprite m_border;
	sf::RectangleShape m_childTile;
	Tile* m_child = nullptr;
	sf::Texture* m_childTexture;
	TileType m_childTileType;
	sf::Text m_toolsText;
	sf::Text m_regularTile;

	sf::Sprite m_currentTile;
	sf::Sprite m_selectedTile;
	sf::Sprite m_seperator;
	Button* m_pen;
	Button* m_erase;
	/*
	Paint Button
	Erase Button
	Deselect Button
	Clear Map Button
	Info Button

	8 Default Tile Buttons
	6 Chained Tiles
	2 Wall Tiles
	2 Box Tiles
	*/

	void generateTileMap(ResourceManager& a_rm);
	void updateUI(ResourceManager& a_rm);
	void generateUI(ResourceManager& a_rm);
	
	/*void setChild()
	{
		m_childTile.setTexture(m_childTexture);
	}*/
	void updateChild(sf::RenderWindow& a_window)
	{
		sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
		sf::Vector2f pos = m_child->getPosition();
		sf::Vector2f diagPos = m_child->getDiagonalPosition();
		m_child->setPosition(sf::Vector2f(mousePosition.x - (diagPos.x - pos.x)/2, mousePosition.y - (diagPos.y - pos.y)/2));
	}
	void removeChild(ResourceManager& a_rm)
	{
		m_childTile.setTexture(a_rm.getTexture("empty_button"));
	}
	void setDefaultText(ResourceManager& a_rm, sf::Text& a_text, unsigned int a_charSize,
		sf::Vector2f a_pos, std::string a_font = "default", sf::Color fill = sf::Color::White)
	{
		a_text.setFont(*a_rm.getFont(a_font));
		a_text.setFillColor(fill);
		a_text.setCharacterSize(a_charSize);
		a_text.setPosition(a_pos);
	}
};