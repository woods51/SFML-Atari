#pragma once
#include "Button.h"

Button::~Button()
{

}
int Button::OnClick()
{
	if (!m_pressedDown)
	{
		m_pressedDown = true;
	}
	else
	{
		m_pressedDown = false;
	}
	return B_DEFAULT;
}

sf::Vector2f Button::getDiagonalPosition() const
{
	return sf::Vector2f(m_shape.getPosition().x + m_shape.getGlobalBounds().width,
		m_shape.getPosition().y + m_shape.getGlobalBounds().height);
}

int BallColor::OnClick()
{
	return B_BALLCOLOR;
}
int PlayButton::OnClick()
{
	return B_START;
}
int QuitButton::OnClick()
{
	return B_QUIT;
}