#include "Button.h"

Press Button::OnClick(ResourceManager& a_rm)
{
	a_rm.playSound(m_sound);

	return m_buttonType;
}
sf::Vector2f Button::getDiagonalPosition() const
{
	return sf::Vector2f(m_shape.getPosition().x + m_shape.getGlobalBounds().width,
		m_shape.getPosition().y + m_shape.getGlobalBounds().height);
}

void Button::isSelected(bool a_isSelected)
{
	// Select
	if (a_isSelected && !m_isSelected)
	{
		m_shape.setTexture(m_textureSelect);
		m_isSelected = true;
		return;
	}

	// Un-Select
	else if (!a_isSelected && m_isSelected)
	{
		m_shape.setTexture(m_texture);
		m_isSelected = false;
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