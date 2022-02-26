#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include <map>

class ResourceManager
{
private:
	std::string defaultTexture = "Assets/0.png";
public:
	std::map<std::string, sf::Texture> textures;

	ResourceManager()
	{
		std::string name;
		std::string texturePath;
		std::string path = "\Assets/";
		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			texturePath = file.path().string();
			//if (texturePath == defaultTexture)
			//	continue;

			sf::Texture temp;
			name = file.path().filename().string();
			name = name.substr(0, name.length() - 4);

			if (!temp.loadFromFile(texturePath))
			{
				printf("Error loading texture: %s\n", texturePath.c_str());

				temp.loadFromFile(defaultTexture);
			}
			
			// Texture is copied to map
			textures[name] = temp;
			printf("Added %s at textures[%s].\n", texturePath.c_str(), name.c_str());
		}

	}
	sf::Texture getTexture(std::string textureID)
	{
		if (textures.find(textureID) == textures.end())
			return textures["0"];

		return textures[textureID];
	}
	//~ResourceManager();
};


class Tile
{
public:
	Tile(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0,0), std::string textureID = "0", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f), sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f))
	{
		this->textureID = textureID;
		texture = rm.getTexture(textureID);
		sprite.setTexture(texture);
		sprite.setPosition(pos);
		sprite.setScale(scale);
		this->size = size;
	}
	sf::Vector2f size;
	sf::Sprite sprite;
	sf::Texture texture;
	std::string textureID;
	float speed = 1.0f;

	sf::Vector2f getDiagonalPos()
	{
		return sprite.getPosition() + (size * sprite.getScale().x);
	}

	bool Collision(sf::Vector2f obj_pos, sf::Vector2f obj_pos_diagonal)
	{
		sf::Vector2f pos = this->sprite.getPosition();
		sf::Vector2f diagonal_pos = this->getDiagonalPos();

		sf::Vector2f p1(obj_pos.x, obj_pos.y);
		sf::Vector2f p2(obj_pos_diagonal.x, obj_pos.y);
		sf::Vector2f p3(obj_pos.x, obj_pos_diagonal.y);
		sf::Vector2f p4(obj_pos_diagonal.x, obj_pos_diagonal.y);

		for (auto p : { p1, p2, p3, p4 })
		{
			if ((p.x >= pos.x && p.x <= diagonal_pos.x) &&
				(p.y >= pos.y && p.y <= diagonal_pos.y))
				return true;

			continue;
		}
		return false;
	}
	
};
class Ball : public Tile
{
public:
	Ball(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), std::string textureID = "0", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f), sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f))
		: Tile(rm, pos, textureID, size, scale)
	{

	}

	sf::Vector2f velocity = sf::Vector2f(0.25f, 0.25f);
	sf::Vector2f lastPos = sprite.getPosition();

	void setLastPos()
	{
		lastPos = sprite.getPosition();
	}
	void updatePos()
	{
		setLastPos();
		sprite.setPosition(sprite.getPosition().x + velocity.x, sprite.getPosition().y + velocity.y);
	}
};

void generateTileRow(std::vector<std::unique_ptr<Tile>>& tileMap, ResourceManager& rm, float pos_y,
	std::string textureID, int iter = 20)
{
	float pos_x = 0;
	for (int i = 0; i < iter; i++)
	{
		tileMap.push_back(std::make_unique<Tile>(rm, sf::Vector2f(pos_x, pos_y), textureID));
		pos_x += 40.0f;
	}
}

void update(std::vector<std::unique_ptr<Tile>>& tileMap, Ball& ball, Tile& paddle)
{
	ball.setLastPos();

	// Player Input & Paddle Movment //

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		paddle.sprite.move(sf::Vector2f(-paddle.speed, 0));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		paddle.sprite.move(sf::Vector2f(paddle.speed, 0));
	}

	if (paddle.sprite.getPosition().x < 0)
	{
		paddle.sprite.setPosition(sf::Vector2f(0, paddle.sprite.getPosition().y));
	}
	if (paddle.getDiagonalPos().x > 800)
	{
		paddle.sprite.setPosition(sf::Vector2f(800 - (paddle.size.x * paddle.sprite.getScale().x), paddle.sprite.getPosition().y));
	}
	if (paddle.sprite.getPosition().y < 0)
	{
		paddle.sprite.setPosition(sf::Vector2f(paddle.sprite.getPosition().x, 0));
	}
	if (paddle.getDiagonalPos().y > 600)
	{
		paddle.sprite.setPosition(sf::Vector2f(paddle.sprite.getPosition().x, 600 - (paddle.size.y * paddle.sprite.getScale().y)));
	}

	// Ball Physics :) //
	ball.updatePos();
	for (auto const &tile : tileMap)
	{
		if (tile->Collision(ball.sprite.getPosition(), ball.getDiagonalPos()))
		{

		}
	}

	if (ball.sprite.getPosition().x <= 0)
	{
		ball.sprite.setPosition(0, ball.sprite.getPosition().y);
		ball.velocity = sf::Vector2f(+ball.velocity.x, ball.velocity.y);
	}
	if (ball.getDiagonalPos().x >= 800)
	{
		ball.sprite.setPosition(800, ball.sprite.getPosition().y);
		ball.velocity = sf::Vector2f(-ball.velocity.x, ball.velocity.y);
	}
	if (ball.sprite.getPosition().y <= 0)
	{
		ball.sprite.setPosition(ball.sprite.getPosition().x, 0);
		ball.velocity = sf::Vector2f(ball.velocity.x, +ball.velocity.y);
	}
	if (ball.getDiagonalPos().y >= 600)
	{
		ball.sprite.setPosition(ball.sprite.getPosition().x, 600);
		ball.velocity = sf::Vector2f(ball.velocity.x, -ball.velocity.y);
	}

}