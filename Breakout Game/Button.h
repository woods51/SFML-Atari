#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class Button
{
public:
	Button(ResourceManager& rm, sf::Vector2f pos, Press type = Press::DEFAULT,
		sf::Vector2f scale = sf::Vector2f(8.0f, 8.0f), sf::Vector2f size = sf::Vector2f(32.0f, 8.0f),
		std::string text = "enter text", std::string textureKey = "button_default",
		std::string fontID = "default")
	{
		m_shape.setPosition(pos);
		m_shape.setScale(scale);
		m_shape.setSize(size);
		m_texture = rm.getTexture(textureKey);
		if (textureKey == "menu_button")
			m_textureSelect = rm.getTexture("menu_button_selected");
		else
			m_textureSelect = m_texture;

		m_shape.setTexture(m_texture);
		m_text.setPosition(m_shape.getPosition());
		m_text.setString(text);
		m_text.setFont(*rm.getFont(fontID));

		m_buttonType = type;
	}
	~Button();
	
	Press OnClick() const { return m_buttonType; }

	void setSelected(bool isSelected)
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

	sf::Vector2f getDiagonalPosition() const;
	inline sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	inline sf::FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }
	const sf::Text& getText() const { return m_text; }
	const sf::RectangleShape& getShape() const { return m_shape; }

	void setString(std::string s) { m_text.setString(s); }
	void setFillColor(const sf::Color& color) { m_text.setFillColor(color); }
	void setCharacterSize(unsigned int size) { m_text.setCharacterSize(size); }

	sf::Text m_text;

protected:
	sf::Texture* m_texture;
	sf::Texture* m_textureSelect;
	bool m_pressedDown = false;
	bool m_selected = false;
	sf::RectangleShape m_shape;
	Press m_buttonType = Press::DEFAULT;
};