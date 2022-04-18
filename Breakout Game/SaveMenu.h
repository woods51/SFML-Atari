#pragma once
#include "stdafx.h"
#include "State.h"
#include "OptionsState.h"

class SaveMenu : public State
{
public:
	SaveMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture, const std::vector<Tile*> a_tileMap);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time a_dt, ResourceManager& a_rm) override;
	void render(sf::RenderWindow& a_window) override;

	~SaveMenu();

private:
	std::vector<Tile*> m_tileMap;
	std::vector<Button*> m_buttons;
	sf::Texture m_frameTexture;
	sf::Sprite m_frameSprite;
	sf::Sprite m_overlay;

	sf::Sprite m_fileValid;
	sf::Text m_saveState;
	sf::Text m_fileName;
	std::string m_playerInput;
	
	sf::Sprite m_playerText;
	Button* m_editButton;
	bool m_isTyping = false;
	bool m_isSaving = false;
	bool m_isFileValid = false;

	void generateUI(ResourceManager& a_rm);
	void handleTextInput(sf::Uint32 a_unicode);
	void saveMap();
	bool verifyFileName(std::string a_fileName);
};
