#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "OptionsState.h"
#include "../../GameObjects/Button.h"

class GameOver : public State
{
public:
	GameOver(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture, unsigned int a_score);

	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_rm, sf::Time a_dt) override;
	void render(sf::RenderWindow& a_window) override;

	~GameOver();

private:

	////////////////////////////////////////////////////////////
	/// \brief Generates state user interfaces
	///
	/// This function generates and initializes all
	/// UI elements for its state.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateUI(ResourceManager& a_rm);

	std::vector<Button*> m_buttons;

	sf::Text m_gameText;
	int m_score;
	sf::Text m_scoreText;
	sf::Text m_scoreValue;
	sf::Texture m_frameTexture;
	sf::Sprite m_frameSprite;
	sf::Sprite m_overlay;
};