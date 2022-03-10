#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <map>
#include "ResourceManager.h"

#define WIDTH 800
#define HEIGHT 600

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
		this->scale = scale;
		this->size = size;
	}
	sf::Vector2f scale;
	sf::Vector2f size;
	sf::Sprite sprite;
	sf::Texture texture;
	std::string textureID;
	float speed = 5.0f;

	sf::Vector2f getDiagonalPos()
	{
		return sprite.getPosition() + (size * sprite.getScale().x);
	}

	sf::Vector2f Collision(sf::Vector2f obj_pos, sf::Vector2f obj_pos_diagonal)
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
				return p;

			continue;
		}
		return sf::Vector2f(0, 0);
	}
	
};
enum class Direction
{
	Left,
	Right,
	Idle
};
class Ball : public Tile
{
public:
	Ball(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), std::string textureID = "ball", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f), sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f))
		: Tile(rm, pos, textureID, size, scale)
	{
		currentDir = Direction::Idle;
		defaultPos = pos;
	}
	sf::Vector2f defaultVel = sf::Vector2f(5.0f, 5.0f);
	sf::Vector2f velocity = defaultVel;
	sf::Vector2f defaultPos;
	Direction currentDir;

	void updatePos(float dt)
	{
		sprite.setPosition(sprite.getPosition().x + velocity.x * dt, sprite.getPosition().y + velocity.y * dt);
		system("CLS");
		std::cout << "v:" << velocity.x << " dt:" << dt << std::endl;
		
		if (velocity.x < 0)
			currentDir = Direction::Left;
		else
			currentDir = Direction::Right;
	}
	void reset(float dt)
	{
		sprite.setPosition(defaultPos);
		velocity = defaultVel;
		//updatePos(dt);
	}
};
class Paddle : public Tile
{
public:
	Paddle(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), std::string textureID = "0", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f), sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f))
		: Tile(rm, pos, textureID, size, scale)
	{
		currentDir = Direction::Idle;
	}

	Direction currentDir;

	void updateDir(Direction d) { currentDir = d; }
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

void update(std::vector<std::unique_ptr<Tile>>& tileMap, Ball& ball, Paddle& paddle, float dt)
{
	// Player Input & Paddle Movment //

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		paddle.sprite.move(sf::Vector2f(-paddle.speed * dt, 0));
		paddle.updateDir(Direction::Left);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		paddle.sprite.move(sf::Vector2f(paddle.speed * dt, 0));
		paddle.updateDir(Direction::Right);
	}
	else
	{
		paddle.updateDir(Direction::Idle);
	}

	if (paddle.sprite.getPosition().x < 0)
	{
		paddle.sprite.setPosition(sf::Vector2f(0, paddle.sprite.getPosition().y));
	}
	if (paddle.getDiagonalPos().x > WIDTH)
	{
		paddle.sprite.setPosition(sf::Vector2f(WIDTH - (paddle.size.x * paddle.sprite.getScale().x), paddle.sprite.getPosition().y));
	}
	if (paddle.sprite.getPosition().y < 0)
	{
		paddle.sprite.setPosition(sf::Vector2f(paddle.sprite.getPosition().x, 0));
	}
	if (paddle.getDiagonalPos().y > HEIGHT)
	{
		paddle.sprite.setPosition(sf::Vector2f(paddle.sprite.getPosition().x, HEIGHT - (paddle.size.y * paddle.sprite.getScale().y)));
	}

	// Ball Physics :) //
	sf::Vector2f temp;
	for (auto const& tile : tileMap) // Collision with a tile
	{
		if (( temp = tile->Collision(ball.sprite.getPosition(), ball.getDiagonalPos()) ) != sf::Vector2f(0,0));
		{

		}
	}
	float scalar = 1.1f;
	// Ball collides with paddle
	if ((temp = paddle.Collision(ball.sprite.getPosition(), ball.getDiagonalPos())) != sf::Vector2f(0, 0))
	{
		// Ball collides with side of Paddle
		// check if either temp vector compnenet is in between the y's side points of the paddle
		// temp = sf::Vector2f( pos.x , pos.y )
		/*if (temp.y >= paddle.sprite.getPosition().y && temp.y <= paddle.getDiagonalPos().y)
		{
			ball.velocity = sf::Vector2f(-ball.velocity.x * scalar, ball.velocity.y);
		}*/


		//
		// Assuming Paddle Hits Top of Paddle
		//
		// Ball and Paddle moving in same direction
		if (ball.currentDir == paddle.currentDir)
			ball.velocity = sf::Vector2f(ball.velocity.x * scalar, -ball.velocity.y * scalar);

		// Ball and Paddle moving in opposite directions
		else if (paddle.currentDir != Direction::Idle)
			ball.velocity = sf::Vector2f(-ball.velocity.x, -ball.velocity.y);

		// Paddle Idle
		else
			ball.velocity = sf::Vector2f(ball.velocity.x, -ball.velocity.y);

	}

	// Window Border collision detection and handling w/ Ball
	float offset = ball.scale.x * ball.size.x;
	//sf::Vector2f ball_movement(0, 0);
	if (ball.sprite.getPosition().x <= 0)
	{
		ball.sprite.setPosition(0, ball.sprite.getPosition().y);
		ball.velocity = sf::Vector2f(-ball.velocity.x, ball.velocity.y);
	}
	if (ball.getDiagonalPos().x >= WIDTH)
	{
		ball.sprite.setPosition(WIDTH - offset, ball.sprite.getPosition().y);
		ball.velocity = sf::Vector2f(-ball.velocity.x, ball.velocity.y);
	}
	if (ball.sprite.getPosition().y <= 0)
	{
		ball.sprite.setPosition(ball.sprite.getPosition().x, 0);
		ball.velocity = sf::Vector2f(ball.velocity.x, -ball.velocity.y);
	}
	if (ball.getDiagonalPos().y >= HEIGHT)
	{
		ball.reset(dt);
	}
	ball.updatePos(dt);
}