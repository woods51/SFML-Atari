#pragma once
#include "../stdafx.h"
#include "../ResourceManager.h"

////////////////////////////////////////////////////////////
/// \brief Button Class.
///
/// This class is used to create rectangular shaped button objects
/// which handle OnClick events.
/// 
////////////////////////////////////////////////////////////
class Button
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the button class.
	///
	/// Constructs button object using provided parameters.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_pos		--> Position
	/// \param a_type		--> OnClick Press type
	/// \param a_scale		--> Scale
	/// \param a_size		--> Size
	/// \param a_textureKey	--> String key for texture
	/// \param a_selectTextureKey --> String Key for selected texture
	////////////////////////////////////////////////////////////
	Button(ResourceManager& a_rm, sf::Vector2f a_pos, Press a_type = Press::DEFAULT,
		sf::Vector2f a_scale = sf::Vector2f(8, 8), sf::Vector2f a_size = sf::Vector2f(32, 8),
		std::string a_textureKey = "", std::string a_selectTextureKey = "", Sound a_sound = Sound::Button)
	{
		m_shape.setPosition(a_pos);
		m_shape.setScale(a_scale);
		m_shape.setSize(a_size);

		m_textureKey = a_textureKey;
		m_texture = a_rm.getTexture(a_textureKey);
		m_textureSelect = a_rm.getTexture(a_selectTextureKey);
		m_shape.setTexture(m_texture);

		m_text.setPosition(m_shape.getPosition());
		m_text.setFont(*a_rm.getFont("default"));

		m_buttonType = a_type;
		m_sound = a_sound;
	}

	////////////////////////////////////////////////////////////
	/// \brief Handles OnClick events
	///
	/// This function is called when the button is pressed and plays button audio.
	/// 
	/// \param a_rm	--> ResourceManager
	/// \return Press type
	////////////////////////////////////////////////////////////
	Press OnClick(ResourceManager& a_rm);

	// Returns Press type
	const enum class Press getButtonType() const { return m_buttonType; }

	// Computes and returns diagonal position
	sf::Vector2f getDiagonalPosition() const;

	// Returns position
	inline sf::Vector2f getPosition() const { return m_shape.getPosition(); }

	// Returns const reference to text field
	const sf::Text& getText() const { return m_text; }

	// Returns const reference to shape object
	const sf::RectangleShape& getShape() const { return m_shape; }

	// Returns texture key
	std::string getTextureKey() { return m_textureKey; }

	// Returns isSelected State
	inline bool isSelected() { return m_isSelected; }

	////////////////////////////////////////////////////////////
	/// \brief Sets isSelected state.
	/// 
	/// This function updates the isSelected state,
	/// if selected the texture of the button will be changed to its selectedTexture.
	/// Otherwise it will be returned to its original texture.
	///
	/// \param a_isSelected	--> State
	////////////////////////////////////////////////////////////
	void isSelected(bool a_isSelected);

	////////////////////////////////////////////////////////////
	/// \brief Sets string text
	///
	/// \param a_string	--> String
	////////////////////////////////////////////////////////////
	void setString(std::string a_string) { m_text.setString(a_string); }

	////////////////////////////////////////////////////////////
	/// \brief Setup function for text objects
	///
	/// \param a_rm		--> ResourceManager
	/// \param a_charSize	--> Character size
	/// \param a_pos		--> Text position
	////////////////////////////////////////////////////////////
	void setDefaultText(ResourceManager& a_rm, unsigned int a_charSize,
		sf::Vector2f a_pos);

protected:

	// Properties
	sf::RectangleShape m_shape;
	sf::Text m_text;
	Press m_buttonType = Press::DEFAULT;
	Sound m_sound = Sound::None;

	// Textures
	std::string m_textureKey;
	sf::Texture* m_texture;
	sf::Texture* m_textureSelect;
	
	// States
	bool m_isSelected = false;
	
};

////////////////////////////////////////////////////////////
/// \brief MenuButton Class.
///
/// This class inherits the members of the Button class,
/// it is primarily used to easily construct menu buttons for menu states.
/// 
////////////////////////////////////////////////////////////
class MenuButton : public Button
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the MenuButton class.
	///
	/// Constructs a menu button object using provided parameters.
	/// Calls Button constructor with predetermined scale, size and texture values.
	/// Sets string text for button.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_pos		--> Position
	/// \param a_type		--> OnClick Press type
	/// \param a_text		--> String text
	////////////////////////////////////////////////////////////
	MenuButton(ResourceManager& a_rm, sf::Vector2f a_pos, Press a_type = Press::DEFAULT, std::string a_text = "enter text")
		: Button(a_rm, a_pos, a_type, sf::Vector2f(8, 8), sf::Vector2f(32, 8), "button_menu", "button_menu_selected")
	{
		m_text.setString(a_text);
	}
};

////////////////////////////////////////////////////////////
/// \brief TickButton Class.
///
/// This class inherits the members of the Button class,
/// it is primarily used as increment and decrement buttons in options and menu states.
/// 
////////////////////////////////////////////////////////////
class TickButton : public Button
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the TickButton class.
	///
	/// Constructs a tick button object using provided parameters.
	/// Calls Button constructor with predetermined scale, size and texture values.
	/// Sets string text for button.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_pos		--> Position
	/// \param a_type		--> OnClick Press type
	/// \param a_text		--> String text
	/// \param a_sound		--> Button Sound
	////////////////////////////////////////////////////////////
	TickButton(ResourceManager& a_rm, sf::Vector2f a_pos, Press a_type = Press::DEFAULT,
		std::string a_text = "enter text", Sound a_sound = Sound::Button)
		: Button(a_rm, a_pos, a_type, sf::Vector2f(3, 3), sf::Vector2f(16, 12), "button_options", "button_options_selected", a_sound)
	{
		m_text.setString(a_text);
	}
};

////////////////////////////////////////////////////////////
/// \brief TileButton Class.
///
/// This class inherits the members of the Button class,
/// it is primarily used in the LevelEditor to create buttons
/// that act as interactable instances of tile objects.
/// 
////////////////////////////////////////////////////////////
class TileButton : public Button
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the TileButton class.
	///
	/// Constructs a tick button object using provided parameters.
	/// Calls Button constructor with predetermined scale, size and texture values.
	/// Sets string text for button.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_pos		--> Position
	/// \param a_type		--> OnClick Press type
	/// \param a_textureKey	--> String key for texture
	////////////////////////////////////////////////////////////
	TileButton(ResourceManager& a_rm, sf::Vector2f a_pos, TileType a_type, std::string a_textureKey = "0")
		: Button(a_rm, a_pos, Press::TILE, sf::Vector2f(4, 4), sf::Vector2f(32, 16), a_textureKey, a_textureKey, Sound::None)
	{
		m_tileType = a_type;
	}

	// Returns TileType
	TileType getTileType() { return m_tileType; }

	////////////////////////////////////////////////////////////
	/// \brief Sets TileType
	///
	/// \param a_type	--> TileType
	////////////////////////////////////////////////////////////
	void setTileType(TileType a_type) { m_tileType = a_type; }
	
protected:

	TileType m_tileType = TileType::None;
};