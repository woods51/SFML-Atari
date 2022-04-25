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
	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_rm, sf::Time a_dt) override;
	void render(sf::RenderWindow& a_window) override;

	~BreakoutState();
private:
	void updateUI();
	void generateUI(ResourceManager& a_rm);
	void generateLevel(ResourceManager& a_rm, std::vector<Tile*>& a_tileMap);
	void levelComplete(ResourceManager& a_rm);
	void resetLevel();
	void handleBallPhysics(sf::Time a_dt, ResourceManager& a_rm);
	void resetBalls(ResourceManager& a_rm);
	void loadFiles();
	bool loadLevel(ResourceManager& a_rm, std::string a_path);

	std::vector<std::string> m_levels;
	std::vector<Tile*> m_tileMap;
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