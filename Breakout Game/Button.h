#pragma once
#include "stdafx.h"
#include "ResourceManager.h"

class Button
{
public:
	Button()
	{

	};
	Button(ResourceManager& a_rm, sf::Vector2f a_pos, Press a_type = Press::DEFAULT,
		sf::Vector2f a_scale = sf::Vector2f(8, 8), sf::Vector2f a_size = sf::Vector2f(32, 8),
		std::string a_textureKey = "", std::string a_selectedTexture = "", Sound a_sound = Sound::Button)
	{
		m_shape.setPosition(a_pos);
		m_shape.setScale(a_scale);
		m_shape.setSize(a_size);

		m_textureKey = a_textureKey;
		m_texture = a_rm.getTexture(a_textureKey);
		m_textureSelect = a_rm.getTexture(a_selectedTexture);
		m_shape.setTexture(m_texture);

		m_text.setPosition(m_shape.getPosition());
		m_text.setFont(*a_rm.getFont("default"));

		m_buttonType = a_type;
		m_sound = a_sound;
	}

	~Button();

	Press OnClick(ResourceManager& a_rm);

	void setSelected(bool isSelected);
	bool isSelected() { return m_selected; }

	const enum class Press getButtonType() const { return m_buttonType; }
	sf::Vector2f getDiagonalPosition() const;
	inline sf::Vector2f getPosition() const { return m_shape.getPosition(); }
	inline sf::FloatRect getGlobalBounds() const { return m_shape.getGlobalBounds(); }
	const sf::Text& getText() const { return m_text; }
	const sf::RectangleShape& getShape() const { return m_shape; }
	sf::Texture* getTexture() { return m_texture; }

	void setString(std::string s) { m_text.setString(s); }
	void setFillColor(const sf::Color& color) { m_text.setFillColor(color); }
	void setCharacterSize(unsigned int size) { m_text.setCharacterSize(size); }
	void setPosition(sf::Vector2f pos) { m_text.setPosition(pos); }
	
	std::string getTextureKey() { return m_textureKey; }

	void setDefaultText(ResourceManager& a_rm, unsigned int a_charSize,
		sf::Vector2f a_pos);

protected:
	sf::Text m_text;
	std::string m_textureKey;
	sf::Texture* m_texture;
	sf::Texture* m_textureSelect;
	bool m_pressedDown = false;
	bool m_selected = false;
	sf::RectangleShape m_shape;
	Press m_buttonType = Press::DEFAULT;
	Sound m_sound = Sound::None;
	
};
class MenuButton : public Button
{
public:
	MenuButton(ResourceManager& a_rm, sf::Vector2f a_pos, Press a_type = Press::DEFAULT, std::string a_text = "enter text")
		: Button(a_rm, a_pos, a_type, sf::Vector2f(8, 8), sf::Vector2f(32, 8), "button_menu", "button_menu_selected")
	{
		m_text.setString(a_text);
	}
};
class TickButton : public Button
{
public:
	TickButton(ResourceManager& a_rm, sf::Vector2f a_pos, Press a_type = Press::DEFAULT,
		std::string a_text = "enter text", Sound a_sound = Sound::Button)
		: Button(a_rm, a_pos, a_type, sf::Vector2f(3, 3), sf::Vector2f(16, 12), "button_options", "button_options_selected")
	{
		m_text.setString(a_text);
	}
};
class TileButton : public Button
{
public:
	TileButton(ResourceManager& a_rm, sf::Vector2f a_pos, TileType a_type, std::string a_textureKey = "0")
		: Button(a_rm, a_pos, Press::TILE, sf::Vector2f(4, 4), sf::Vector2f(32, 16), a_textureKey, a_textureKey, Sound::None)
	{
		m_tileType = a_type;
	}
	void setTileType(TileType type) { m_tileType = type; }
	TileType getTileType() { return m_tileType; }
protected:
	TileType m_tileType = TileType::None;
};