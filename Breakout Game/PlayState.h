#pragma once

#include "State.h"
#include "PauseState.h"
#include "Ball.h"
#include "Paddle.h"

class PlayState : public State
{
public:
	PlayState(ResourceManager& a_rm, sf::RenderWindow& a_window);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time dt, ResourceManager& a_window) override;
	void render(sf::RenderWindow& a_window) override;

	void generateLevel1(ResourceManager& a_rm);
	void generateLevel2(ResourceManager& a_rm);

	~PlayState();
private:
	void updateUI();
	void generateUI(ResourceManager& a_rm);
	void levelComplete(ResourceManager& a_rm);

	std::vector<std::unique_ptr<Tile>> m_tileMap;
	std::vector<Button*> m_buttons;
	Ball *m_ball;
	Paddle *m_paddle;

	std::string m_wallTile = "tile_06";
	sf::Sprite m_border;
	sf::Text m_scoreText;
	sf::Text m_startText;
	sf::Texture m_frameTexture;
	int m_currentLevel = 0;
	bool m_completeFlag = false;
	bool m_colorFlag = false;
	int m_score = 0;
};