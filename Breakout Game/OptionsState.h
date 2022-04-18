#pragma once
#include "stdafx.h"
#include "BreakoutState.h"

class OptionsState : public State
{
public:
	OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture);
	OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Sprite* a_background, sf::Sprite* a_background2);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time a_dt, ResourceManager& a_rm) override;
	void render(sf::RenderWindow& a_window) override;

	~OptionsState();

private:
	std::vector<Button*> m_buttons;
	sf::Sprite m_overlay;
	sf::Sprite* m_background = nullptr;
	sf::Sprite* m_background2 = nullptr;

	sf::Text m_buttonText;
	sf::Text m_buttonVolume;
	int m_buttonVolumeNum;

	sf::Text m_ballText;
	sf::Text m_ballVolume;
	int m_ballVolumeNum;

	sf::Text m_volumeText;

	void setDefaultText(ResourceManager& a_rm, sf::Text& a_text, unsigned int a_charSize,
		sf::Vector2f a_pos, std::string a_font = "default", sf::Color fill = sf::Color::White)
	{
		a_text.setFont(*a_rm.getFont(a_font));
		a_text.setFillColor(fill);
		a_text.setCharacterSize(a_charSize);
		a_text.setPosition(a_pos);
	}

	void fixVolume(int& a_volume);
	void adjustVolumeText(int& a_volume, sf::Text& a_text);
	void generateUI(ResourceManager&);
};