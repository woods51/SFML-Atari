#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ResourceManager.h"

class Button
{
public:
	Button(ResourceManager& a_rm, sf::Vector2f a_pos, Press a_type = Press::DEFAULT,
		sf::Vector2f a_scale = sf::Vector2f(8.0f, 8.0f), sf::Vector2f a_size = sf::Vector2f(32.0f, 8.0f),
		std::string a_text = "enter text", std::string a_textureKey = "button_default", SoundType a_sound = SoundType::Button,
		std::string a_fontID = "default")
	{
		m_shape.setPosition(a_pos);
		m_shape.setScale(a_scale);
		m_shape.setSize(a_size);

		m_texture = a_rm.getTexture(a_textureKey);
		if (a_textureKey == "menu_button")
			m_textureSelect = a_rm.getTexture("menu_button_selected");
		else
			m_textureSelect = m_texture;

		m_shape.setTexture(m_texture);
		m_sound = a_sound;
		m_text.setPosition(m_shape.getPosition());
		m_text.setString(a_text);
		m_text.setFont(*a_rm.getFont(a_fontID));

		m_buttonType = a_type;
	}
	~Button();
	
	Press OnClick(ResourceManager& a_rm)
	{
		a_rm.playSound(SoundType::Button);
		
		return m_buttonType;
	}

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

	void setDefaultText(ResourceManager& a_rm, unsigned int a_charSize,
		sf::Vector2f a_pos, sf::Color fill = sf::Color::White)
	{
		m_text.setFillColor(fill);
		m_text.setCharacterSize(a_charSize);
		m_text.setPosition(a_pos);
	}

protected:
	sf::Text m_text;
	sf::Texture* m_texture;
	sf::Texture* m_textureSelect;
	bool m_pressedDown = false;
	bool m_selected = false;
	sf::RectangleShape m_shape;
	Press m_buttonType = Press::DEFAULT;
	SoundType m_sound = SoundType::None;
};