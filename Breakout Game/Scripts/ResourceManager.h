#pragma once
#include "stdafx.h"

////////////////////////////////////////////////////////////
/// \brief ResourceManager Class.
///
/// The ResourceManager class is a class used to
/// access any game assets, change in-game volume settings,
/// or play sound effects.
/// 
////////////////////////////////////////////////////////////
class ResourceManager
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the ResourceManager.
	///
	/// Constructs and loads all texture assets, fonts, and sounds.
	/// 
	////////////////////////////////////////////////////////////
	ResourceManager();

	////////////////////////////////////////////////////////////
	/// \brief Destructor for ResourceManager. 
	///
	///	Deconstructs and frees all dynmically allocated memory.
	////////////////////////////////////////////////////////////
	~ResourceManager();
	
	////////////////////////////////////////////////////////////
	/// \brief Returns texture pointer from texture map.
	///
	/// This function searches the texture map for the
	/// textureKey and returns the texture pointer at that location.
	/// 
	/// \param a_textureKey		--> Texture key
	/// 
	/// \returns Returns texture pointer at location, default texture otherwise. 
	////////////////////////////////////////////////////////////
	sf::Texture* getTexture(std::string a_textureKey);

	////////////////////////////////////////////////////////////
	/// \brief Returns font pointer from font map.
	///
	/// This function searches the font map for the
	/// fontKey and returns the font pointer at that location.
	/// 
	/// \param a_fontKey		--> Font key
	/// 
	/// \returns Returns font pointer at location, default font otherwise. 
	////////////////////////////////////////////////////////////
	sf::Font* getFont(std::string a_fontKey);
	
	////////////////////////////////////////////////////////////
	/// \brief Returns the volume value of the sound.
	///
	/// This function returns the volume value corresponding
	/// to the sound type.
	/// 
	/// \param a_sound		--> Sound
	/// 
	/// \returns Returns volume value of sound. 
	////////////////////////////////////////////////////////////
	unsigned int getVolume(enum class Sound a_sound);
	
	////////////////////////////////////////////////////////////
	/// \brief Update volume value of the sound.
	///
	/// This function either increases or decreases a
	/// sounds volume value by 10 units.
	/// 
	/// \param a_sound		--> Sound
	/// \param a_increase	--> Is increasing volume
	/// 
	////////////////////////////////////////////////////////////
	void setVolume(enum class Sound a_sound, bool a_increase);

	////////////////////////////////////////////////////////////
	/// \brief Plays sound.
	///
	/// This function plays the selected sound from the sound buffer.
	/// 
	/// \param a_sound		--> Sound
	/// 
	////////////////////////////////////////////////////////////
	void playSound(enum class Sound a_sound);
	
private:

	////////////////////////////////////////////////////////////
	/// \brief Loads texture assets.
	///
	/// This function loads texture assets from the
	/// Assets/ directory and places texture into a map at
	/// the key value corresponding to its file name.
	/// 
	////////////////////////////////////////////////////////////
	void loadTextures();

	////////////////////////////////////////////////////////////
	/// \brief Loads font assets.
	///
	/// This function loads font assets from the
	/// Fonts/ directory and places font into a map at
	/// the key value corresponding to its file name.
	/// 
	////////////////////////////////////////////////////////////
	void loadFonts();

	////////////////////////////////////////////////////////////
	/// \brief Loads sound assets.
	///
	/// This function loads sound assets from the
	/// Sounds/ directory and places sound into a map at
	/// the key value corresponding to its file name.
	/// 
	////////////////////////////////////////////////////////////
	void loadSounds();

	////////////////////////////////////////////////////////////
	/// \brief Returns corresponding sound pointer.
	///
	/// This function searches the sound map for the
	/// soundKey and returns the sound pointer at that location.
	/// 
	/// \param a_soundKey	--> Sound key
	/// 
	/// \returns Returns sound pointer at location, button sound otherwise. 
	////////////////////////////////////////////////////////////
	sf::SoundBuffer* getSound(std::string a_soundKey);

	void generateSounds();
	void adjustVolume(sf::Sound& a_sound, float a_value);

	// Textures
	std::map<std::string, sf::Texture*> m_textures;
	std::string m_defaultTexture = "Assets/default.png";
	
	// Fonts
	std::map<std::string, sf::Font*> m_fonts;
	std::string m_defaultFont = "Fonts/default.TTF";

	// Sounds
	std::map<std::string, sf::SoundBuffer*> m_sounds;
	sf::Sound m_buttonPress;
	sf::Sound m_ballBounce;
	sf::Sound m_gameOver;
	sf::Sound m_levelComplete;
};