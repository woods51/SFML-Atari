#pragma once
#include "Button.h"

int Button::OnClick()
{
	if (!m_pressedDown)
	{
		m_text.setFillColor(sf::Color::Red);
		m_pressedDown = true;
	}
	else
	{
		m_text.setFillColor(sf::Color::Blue);
		m_pressedDown = false;
	}
	return B_DEFAULT;
}

sf::Vector2f Button::getPosition() const { return m_shape.getPosition(); }
sf::Vector2f Button::getDiagonalPosition() const
{
	return sf::Vector2f(m_shape.getPosition().x + m_shape.getGlobalBounds().width,
		m_shape.getPosition().y + m_shape.getGlobalBounds().height);
}
sf::FloatRect Button::getGlobalBounds() const { return m_shape.getGlobalBounds(); }
const sf::Text& Button::getText() const { return m_text; }
const sf::RectangleShape& Button::getShape() const { return m_shape; }

int BallColor::OnClick()
{
	return B_BALLCOLOR;
}