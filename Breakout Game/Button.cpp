#pragma once
#include "Button.h"

void Button::OnClick()
{
	if (i)
	{
		text.setFillColor(sf::Color::Red);
		i = false;
	}
	else
	{
		text.setFillColor(sf::Color::Blue);
		i = true;
	}
}
sf::Vector2f Button::getDiagonalPos()
{
	return sf::Vector2f(sprite.getPosition().x + (size.x * sprite.getScale().x),
		sprite.getPosition().y + (size.y * sprite.getScale().y));
}