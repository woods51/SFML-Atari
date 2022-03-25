#pragma once
//#define WIDTH 800
//#define HEIGHT 600

#include <SFML\Graphics.hpp>
#include "ResourceManager.h"
#include <cmath>
#include <iostream>

class Tile
{
public:
	Tile();
	Tile(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0, 0), 
		std::string textureID = "0", sf::Vector2f size = sf::Vector2f(16.0f, 16.0f), sf::Vector2f scale = sf::Vector2f(2.5f, 2.5f))
	{
		this->textureID = textureID;
		texture = rm.getTexture(textureID);
		sprite.setTexture(*texture);
		sprite.setPosition(pos);
		sprite.setScale(scale);
		this->scale = scale;
		this->size = size;

		isActive = true;
	};

	sf::Vector2f scale;
	sf::Vector2f size;
	sf::Sprite sprite;
	sf::Texture* texture;
	std::string textureID;
	float speed = 8.0f;
	bool isActive;

	sf::Vector2f getDiagonalPos();

	enum class Surface collision(sf::Vector2f, sf::Vector2f);

	double distance(sf::Vector2i, sf::Vector2i);
};

enum class Direction
{
	Left,
	Right,
	Idle
};
enum class Surface
{
	Top,
	Bottom,
	Left,
	Right,
	Diagonal,
	None
};