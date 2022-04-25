#pragma once
#include "stdafx.h"

class ResourceManager
{

public:
	ResourceManager();
	~ResourceManager();

	sf::Texture* getTexture(std::string textureKey);
	sf::Font* getFont(std::string fontKey);
	sf::SoundBuffer* getSound(std::string soundKey);
	unsigned int getVolume(enum class Sound a_sound);
	void playSound(enum class Sound a_sound);
	void setVolume(enum class Sound a_sound, bool a_increase);
	
private:
	void loadTextures();
	void loadFonts();
	void loadSounds();

	void generateSounds();
	void adjustVolume(sf::Sound& a_sound, float a_value);

	std::map<std::string, sf::Texture*> m_textures;
	std::map<std::string, sf::Font*> m_fonts;
	std::map<std::string, sf::SoundBuffer*> m_sounds;

	std::string m_defaultTexture = "Assets/0.png";
	std::string m_defaultFont = "Fonts/default.TTF";

	sf::Sound m_buttonPress;
	sf::Sound m_ballBounce;
	sf::Sound m_gameOver;
	//sf::Sound m_gameStart;
	sf::Sound m_levelComplete;

};