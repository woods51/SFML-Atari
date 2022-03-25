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
	void update(sf::Time dt, ResourceManager&) override;
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
	void generateLevel1(ResourceManager& rm)
	{
		float pos_x = 0;
		float pos_y = 80.0f;
		std::string tiles[5] = { "tile_01", "tile_02", "tile_03", "tile_04", "tile_05" };
		for (auto const&texture : tiles)
		{
			for (int i = 0; i < 16; i++)
			{
				tileMap.push_back(std::make_unique<Tile>(rm, sf::Vector2f(pos_x, pos_y), texture,
					sf::Vector2f(25.0f, 16.0f), sf::Vector2f(2.0f, 2.5f)));
				pos_x += 50.0f;
			}
			pos_x = 0;
			pos_y += 40.0f;
		}
	}
private:
	std::vector<std::unique_ptr<Tile>> tileMap;
	std::vector<Button*> buttons;

	Ball *ball;
	Paddle *paddle;

	sf::Sprite border;
	sf::Text scoreText;
	bool color_flag = false;
	int score = 0;
	void updateUI();
	void generateUI(ResourceManager& rm);
};