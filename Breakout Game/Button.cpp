#include "Button.h"

Button::~Button()
{

}

sf::Vector2f Button::getDiagonalPosition() const
{
	return sf::Vector2f(m_shape.getPosition().x + m_shape.getGlobalBounds().width,
		m_shape.getPosition().y + m_shape.getGlobalBounds().height);
}
Press Button::OnClick(ResourceManager& a_rm)
{
	a_rm.playSound(m_sound);

	return m_buttonType;
}
void Button::setSelected(bool isSelected)
{
	if (isSelected)
	{
		if (!m_selected)
		{
			m_shape.setTexture(m_textureSelect);
			m_selected = true;
			return;
		}
		return;
	}
	else
	{
		if (m_selected)
		{
			m_shape.setTexture(m_texture);
			m_selected = false;
			return;
		}
		return;
	}
}
void Button::setDefaultText(ResourceManager& a_rm, unsigned int a_charSize,
	sf::Vector2f a_pos)
{
	m_text.setFillColor(sf::Color::White);
	m_text.setCharacterSize(a_charSize);
	m_text.setPosition(a_pos);
}