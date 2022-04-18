#pragma once
#include "stdafx.h"
#include "State.h"
#include "BreakoutState.h"
#include "OptionsState.h"
#include "LevelEditor.h"
#include "LoadMenu.h"

class BreakoutMenu : public State
{
public:
	BreakoutMenu(ResourceManager&, sf::RenderWindow&);

	void inputHandler(sf::Keyboard::Key, bool) override;
	void eventHandler(sf::RenderWindow&, ResourceManager&, std::vector<std::unique_ptr<State>>&) override;
	void update(sf::Time dt, ResourceManager&) override;
	void render(sf::RenderWindow&) override;

	~BreakoutMenu();

private:
	std::vector<Button*> m_buttons;
	sf::Sprite m_breakoutText;
	sf::Sprite m_editorText;
	sf::Sprite m_loadText;

	sf::Sprite m_background;
	sf::Sprite m_background2;
	sf::Texture m_frameTexture;
	bool m_breakoutFlag = false;
	bool m_pongFlag = false;
	bool m_editorFlag = false;
	void generateUI(ResourceManager&);
};