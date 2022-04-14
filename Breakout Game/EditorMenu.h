#pragma once
#include "State.h"
#include "OptionsState.h"

class EditorMenu : public State
{
public:
	EditorMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time a_dt, ResourceManager& a_rm) override;
	void render(sf::RenderWindow& a_window) override;

	~EditorMenu();

private:
	std::vector<Button*> m_buttons;
	sf::Texture m_frameTexture;
	sf::Sprite m_frameSprite;
	sf::Sprite m_overlay;

	bool m_resumeFlag = false;
	bool m_escapeLock = true;

	void generateUI(ResourceManager&);
};
