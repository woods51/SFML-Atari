#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "BreakoutMenu.h"
#include "OptionsState.h"
#include "../Games/PongState.h"
#include "../Games/LevelEditor.h"

class MenuState : public State
{
public:
	MenuState(ResourceManager& a_rm, sf::RenderWindow& a_window);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_rm, sf::Time a_dt) override;
	void render(sf::RenderWindow& a_window) override;

	~MenuState();

private:
	std::vector<Button*> m_buttons;
	sf::Sprite m_atariText;
	sf::Sprite m_breakoutText;
	sf::Sprite m_editorText;
	sf::Sprite m_pongText;
	sf::Texture m_frameTexture;
	sf::Sprite m_background;
	sf::Sprite m_background2;

	bool m_breakoutFlag = false;
	bool m_pongFlag = false;
	bool m_editorFlag = false;
	void generateUI(ResourceManager& a_rm);
};