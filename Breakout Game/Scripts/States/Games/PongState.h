#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "../Menus/PauseState.h"
#include "../../GameObjects/PongBall.h"
#include "../../GameObjects/Paddle.h"

class PongState : public State
{
public:
	PongState(ResourceManager& a_rm, sf::RenderWindow& a_window);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_window, sf::Time a_dt) override;
	void render(sf::RenderWindow& a_window) override;

	~PongState();
private:
	void updateUI();
	void handleBallPhysics(sf::Time a_dt, ResourceManager& a_rm);
	void generateUI(ResourceManager& a_rm);

	std::vector<Button*> m_buttons;
	PongBall* m_ball;
	Paddle* m_paddleP1;
	Paddle* m_paddleP2;
	std::vector<Paddle*> m_paddles;

	sf::Sprite m_border;
	sf::Text m_startText;
	sf::Texture m_frameTexture;
	bool m_colorFlag = false;

	sf::Text m_player1;
	sf::Text m_player2;

	sf::Text m_scoreP1Text;
	sf::Text m_scoreP2Text;
	sf::Vector2i m_pScores = sf::Vector2i(0, 0);
	
};