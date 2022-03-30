#pragma once
#include "Button.h"

Button::~Button()
{

}

sf::Vector2f Button::getDiagonalPosition() const
{
	return sf::Vector2f(m_shape.getPosition().x + m_shape.getGlobalBounds().width,
		m_shape.getPosition().y + m_shape.getGlobalBounds().height);
}