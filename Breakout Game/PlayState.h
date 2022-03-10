#pragma once

#include "State.h"
#include "Ball.h"
#include "Paddle.h"

class PlayState : public State
{
public:
	PlayState(ResourceManager& rm, sf::RenderWindow& window);

	void inputHandler(sf::Keyboard::Key key, bool isPressed) override;
	void eventHandler(sf::RenderWindow&) override;
	void update(sf::Time dt) override;
	void render(sf::RenderWindow&) override;

	~PlayState();

	void generateTileRow(ResourceManager& rm, float pos_y, std::string textureID, int iter = 20)
	{
		float pos_x = 0;
		for (int i = 0; i < iter; i++)
		{
			tileMap.push_back(std::make_unique<Tile>(rm, sf::Vector2f(pos_x, pos_y), textureID));
			pos_x += 40.0f;
		}
	}
private:
	std::vector<std::unique_ptr<Tile>> tileMap;

	Ball *ball;
	Paddle *paddle;
};