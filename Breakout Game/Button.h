#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

#define B_DEFAULT 0
#define B_BALLCOLOR 1
#define B_OTHER 2

class Button
{
public:
	Button(ResourceManager& rm, sf::Vector2f pos, sf::Vector2f scale, sf::Vector2f size,
		std::string text = "enter text", std::string textureID = "button_default",
		std::string fontID = "default")
	{
		m_shape.setPosition(pos);
		m_shape.setScale(scale);
		m_shape.setSize(size);
		m_shape.setTexture(rm.getTexture(textureID));

		m_text.setPosition(m_shape.getPosition());
		m_text.setString(text);
		m_text.setFont(*rm.getFont(fontID));
	}
	//~Button();
	
	virtual int OnClick();
	sf::Vector2f getDiagonalPosition() const;
	sf::Vector2f getPosition() const;
	sf::FloatRect getGlobalBounds() const;
	const sf::Text& getText() const;
	const sf::RectangleShape& getShape() const;

	void setString(std::string s) { m_text.setString(s); }
	void setFillColor(const sf::Color& color) { m_text.setFillColor(color); }
	void setCharacterSize(unsigned int size) { m_text.setCharacterSize(size); }

protected:
	bool m_pressedDown = false;
	sf::RectangleShape m_shape;
	sf::Text m_text;
};

class BallColor : public Button
{
public:
	BallColor(ResourceManager& rm, sf::Vector2f pos)
		: Button(rm, pos, sf::Vector2f(5.0f, 5.0f), sf::Vector2f(20.0f, 4.0f), "Ball Color", "empty_button", "default")
	{
		m_text.setFillColor(sf::Color::White);
		m_text.setCharacterSize(12);
		m_text.setPosition(m_shape.getPosition() + sf::Vector2f(0, 2.0f));
	}
	int OnClick() override;
};