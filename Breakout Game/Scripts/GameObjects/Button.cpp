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

Button* createMenuButton(ResourceManager& a_rm, sf::Vector2f a_pos, Press a_type, std::string a_text)
{
	Button* temp = new Button(a_rm, a_pos, a_type, sf::Vector2f(8, 8), sf::Vector2f(32, 8), "button_menu", "button_menu_selected");
	temp->setString(a_text);
	return temp;
}

Button* createTickButton(ResourceManager& a_rm, sf::Vector2f a_pos, Press a_type,
	std::string a_text, Sound a_sound)
{
	Button* temp = new Button(a_rm, a_pos, a_type, sf::Vector2f(3, 3), sf::Vector2f(16, 12), "button_options", "button_options_selected", a_sound);
	temp->setString(a_text);
	return temp;
}