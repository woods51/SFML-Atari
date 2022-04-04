#pragma once
#include "State.h"
#include "OptionsState.h"
#include "PongState.h"

class MenuState : public State
{
public:
	MenuState(ResourceManager&, sf::RenderWindow&);

	void inputHandler(sf::Keyboard::Key, bool) override;
	void eventHandler(sf::RenderWindow&, ResourceManager&, std::vector<std::unique_ptr<State>>&) override;
	void update(sf::Time dt, ResourceManager&) override;
	void render(sf::RenderWindow&) override;

	~MenuState();

private:
	std::vector<Button*> m_buttons;
	sf::Sprite m_atariText;
	sf::Sprite m_breakoutText;
	sf::Sprite m_pongText;
	sf::Texture m_frameTexture;

	bool m_breakoutFlag = false;
	bool m_pongFlag = false;
	void generateUI(ResourceManager&);
};