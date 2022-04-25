#pragma once
#include "../../stdafx.h"
#include "../Games/BreakoutState.h"
#include "../../GameObjects/Button.h"

class OptionsState : public State
{
public:
	OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture);
	OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Sprite* a_background, sf::Sprite* a_background2);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_rm, sf::Time a_dt) override;
	void render(sf::RenderWindow& a_window) override;

	~OptionsState();

private:
	std::vector<Button*> m_buttons;
	sf::Sprite m_overlay;
	sf::Sprite* m_background = nullptr;
	sf::Sprite* m_background2 = nullptr;

	sf::Text m_buttonText;
	sf::Text m_buttonVolume;

	sf::Text m_ballText;
	sf::Text m_ballVolume;

	sf::Text m_gameText;
	sf::Text m_gameVolume;

	sf::Text m_levelText;
	sf::Text m_levelVolume;

	sf::Text m_volumeText;

	void adjustVolumeText(int a_volume, sf::Text& a_text, bool a_left);
	void generateUI(ResourceManager& a_rm);
};