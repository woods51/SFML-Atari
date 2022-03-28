#pragma once

#include "State.h"
#include "Ball.h"
#include "Paddle.h"

class PlayState : public State
{
public:
	PlayState(ResourceManager&, sf::RenderWindow&);

	void inputHandler(sf::Keyboard::Key, bool) override;
	void eventHandler(sf::RenderWindow&, sf::View&) override;
	void update(sf::Time dt, ResourceManager&) override;
	void render(sf::RenderWindow&) override;

	void generateTileRow(ResourceManager&, float, std::string, int);
	void generateLevel1(ResourceManager&);

	~PlayState();
private:
	void updateUI();
	void generateUI(ResourceManager& rm);

	std::vector<std::unique_ptr<Tile>> m_tileMap;
	std::vector<Button*> m_buttons;
	Ball *m_ball;
	Paddle *m_paddle;

	sf::Sprite m_border;
	sf::Text m_scoreText;
	bool m_color_flag = false;
	int m_score = 0;
};