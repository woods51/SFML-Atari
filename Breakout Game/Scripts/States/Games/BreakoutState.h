#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "../Menus/PauseState.h"
#include "../Menus/LoadMenu.h"
#include "../Menus/GameOver.h"
#include "../../GameObjects/Ball.h"
#include "../../GameObjects/Paddle.h"

class BreakoutState : public State
{
public:
	BreakoutState(ResourceManager& a_rm, sf::RenderWindow& a_window, unsigned int a_lives);
	BreakoutState(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<Tile*>& a_tileMap, unsigned int a_lives);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time a_dt, ResourceManager& a_rm) override;
	void render(sf::RenderWindow& a_window) override;

	void generateLevel(ResourceManager& a_rm, std::vector<Tile*>& a_tileMap);
	void generateLevel1(ResourceManager& a_rm);
	void generateLevel2(ResourceManager& a_rm);
	void generateLevel3(ResourceManager& a_rm);
	void generateLevel4(ResourceManager& a_rm);

	~BreakoutState();
private:
	void updateUI();
	void generateUI(ResourceManager& a_rm);
	void levelComplete(ResourceManager& a_rm);
	void resetLevel();
	void handleBallPhysics(sf::Time a_dt, ResourceManager& a_rm);
	void resetBalls(ResourceManager& a_rm);

	std::vector<std::unique_ptr<Tile>> m_tileMap;
	std::vector<Button*> m_buttons;
	std::vector<std::unique_ptr<Ball>> m_balls;

	Paddle* m_paddle;
	int m_livesRemaining;
	sf::Text m_livesText;

	sf::Sprite m_border;
	sf::Text m_scoreText;
	sf::Text m_level;
	sf::Text m_startText;
	sf::Text m_secondLevelText;
	sf::Text m_thirdLevelText;
	sf::Text m_fourthLevelText;
	sf::Texture m_frameTexture;
	sf::Font m_defaultFont;
	int m_currentLevel = 0;
	bool m_gameStarted = false;
	bool m_gameOver = false;
	bool m_completeFlag = false;
	bool m_isCustom = false;
	int m_score = 0;
};