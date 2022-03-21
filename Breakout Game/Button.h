#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class Button
{
public:
	Button(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0,0), sf::Vector2f scale = sf::Vector2f(5.0f, 5.0f), sf::Vector2f size = sf::Vector2f(16.0f, 8.0f), std::string textureID = "button", std::string text = "enter text",
		std::string fontID = "default", int fontSize = 25)
	{
		this->sprite.setTexture(*rm.getTexture(textureID));
		this->sprite.setScale(scale);
		this->text.setString(text);
		this->text.setCharacterSize(fontSize);
		this->text.setFont(*rm.getFont(fontID));
		this->size = size;
	}
	//~Button();

	sf::Sprite sprite;
	sf::Text text;
	sf::Vector2f size;
	void OnClick();
	sf::Vector2f getDiagonalPos();
	int i = true;
};