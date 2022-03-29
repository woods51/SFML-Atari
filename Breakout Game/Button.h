#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

#define B_DEFAULT 0
#define B_START 1
#define B_QUIT 2
#define B_BALLCOLOR 3
#define B_OTHER 4

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
	~Button();
	
	virtual int OnClick();
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
	bool m_pressedDown = false;
	sf::RectangleShape m_shape;
	
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
class PlayButton : public Button
{
public:
	PlayButton(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f((WIDTH/2) - 128, (HEIGHT/2) - 64))
		: Button(rm, pos, sf::Vector2f(8.0f, 8.0f), sf::Vector2f(32.0f, 8.0f), "Play", "menu_button", "default")
	{
		m_text.setFillColor(sf::Color::White);
		m_text.setCharacterSize(40);
		m_text.setPosition(m_shape.getPosition() + sf::Vector2f(64.0f, 8.0f));
	}
	int OnClick() override;
};
class QuitButton : public Button
{
public:
	QuitButton(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 128))
		: Button(rm, pos, sf::Vector2f(8.0f, 8.0f), sf::Vector2f(32.0f, 8.0f), "Quit", "menu_button", "default")
	{
		m_text.setFillColor(sf::Color::White);
		m_text.setCharacterSize(40);
		m_text.setPosition(m_shape.getPosition() + sf::Vector2f(64.0f, 8.0f));
	}
	int OnClick() override;
};