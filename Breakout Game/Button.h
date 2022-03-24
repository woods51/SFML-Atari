#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

#define B_DEFAULT 0
#define B_BALLCOLOR 1
#define B_OTHER 2

class Button
{
public:
	Button(ResourceManager& rm, sf::Vector2f pos, sf::Vector2f scale, sf::Vector2f size,
		std::string text = "enter text", std::string textureID = "button_default",
		std::string fontID = "default")
	{
		this->sprite.setPosition(pos);
		this->text.setPosition(sprite.getPosition());
		this->sprite.setScale(scale);
		this->size = size;
		this->sprite.setTexture(*rm.getTexture(textureID));
		
		this->text.setString(text);
		this->text.setFont(*rm.getFont(fontID));
		
	}
	//~Button();

	sf::Sprite sprite;
	sf::Text text;
	sf::Vector2f size;

	virtual int OnClick();
	sf::Vector2f getDiagonalPos();

private:
	int i = true;
};

class BallColor : public Button
{
public:
	BallColor(ResourceManager& rm, sf::Vector2f pos)
		: Button(rm, pos, sf::Vector2f(5.0f, 5.0f), sf::Vector2f(32.0f, 4.0f), "Ball Color", "button_02", "default")
	{
		this->sprite.setTexture(*(sf::Texture*)nullptr);
		this->text.setFillColor(sf::Color::White);
		this->text.setCharacterSize(12);
		this->text.setPosition(this->sprite.getPosition() + sf::Vector2f(0, 2.0f));
	}
	int OnClick() override;
};