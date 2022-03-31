#pragma once
#include "PlayState.h"

class OptionsState : public State
{
public:
	OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time a_dt, ResourceManager& a_rm) override;
	void render(sf::RenderWindow& a_window) override;

	~OptionsState();

private:
	std::vector<Button*> m_buttons;
	sf::Sprite m_frameSprite;
	sf::Sprite m_overlay;

	bool m_resumeFlag = false;
	bool m_escapeLock = true;

	sf::Text m_buttonText;
	sf::Text m_buttonVolume;
	sf::Text m_volumeText;
	int m_buttonVolumeNum;

	void setDefaultText(ResourceManager& a_rm, sf::Text& a_text, unsigned int a_charSize,
		sf::Vector2f a_pos, std::string a_font = "default", sf::Color fill = sf::Color::White)
	{
		a_text.setFont(*a_rm.getFont(a_font));
		a_text.setFillColor(fill);
		a_text.setCharacterSize(a_charSize);
		a_text.setPosition(a_pos);
	}

	void fixVolume(int& a_volume)
	{
		if (a_volume < 0)
			a_volume = 0;
		else if (a_volume > 100)
			a_volume = 100;
		return;
	}

	void generateUI(ResourceManager&);
};