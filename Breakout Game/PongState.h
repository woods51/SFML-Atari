#pragma once

#include "State.h"
#include "PauseState.h"
#include "Ball.h"
#include "Paddle.h"

class PongState : public State
{
public:
	PongState(ResourceManager& a_rm, sf::RenderWindow& a_window);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time dt, ResourceManager& a_window) override;
	void render(sf::RenderWindow& a_window) override;

	~PongState();
private:
	void updateUI();
	void generateUI(ResourceManager& a_rm);

	//std::vector<std::unique_ptr<Tile>> m_tileMap;
	std::vector<Button*> m_buttons;
	PongBall* m_ball;
	Paddle* m_paddleP1;
	Paddle* m_paddleP2;
	std::vector<Paddle*> m_paddles;

	sf::Sprite m_border;
	sf::Text m_scoreP1Text;
	sf::Text m_scoreP2Text;
	sf::Vector2i m_pScores = sf::Vector2i(0,0);

	sf::Text m_player1;
	sf::Text m_player2;

	sf::Text m_startText;
	sf::Texture m_frameTexture;
	bool m_colorFlag = false;
};