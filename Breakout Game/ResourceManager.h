#pragma once

#include <SFML\Graphics.hpp>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <string>

#define WIDTH 1280
#define HEIGHT 720
#define MULTIPLIER 25

class ResourceManager
{

public:
	ResourceManager();
	~ResourceManager();

	void loadTextures();
	void loadFonts();
	void loadSounds();

	sf::Texture* getTexture(std::string textureKey);
	sf::Font* getFont(std::string fontKey);
	sf::SoundBuffer* getSound(std::string soundKey);
	unsigned int getVolume(enum class SoundType a_sound);
	void playSound(enum class SoundType a_sound);
	void setVolume(enum class SoundType a_sound, int a_volume);
	
private:
	std::map<std::string, sf::Texture*> m_textures;
	std::map<std::string, sf::Font*> m_fonts;
	std::map<std::string, sf::SoundBuffer*> m_sounds;

	std::string m_defaultTexture = "Assets/0.png";
	std::string m_defaultFont = "Fonts/default.TTF";

	sf::Sound m_buttonSound;
	sf::Sound m_ballSound;

};

enum class Press {
	DEFAULT,
	OTHER,
	START,
	MAINMENU,
	QUIT,
	BACK,
	PAUSE,
	RESUME,
	OPTIONS,
	PONG,
	BREAKOUT,
	GAMESELECT,
	BALLCOLOR,

	BUTTON_UP,
	BUTTON_DOWN,
	BALL_UP,
	BALL_DOWN,
	BUTTON_UP_X,
	BUTTON_DOWN_X,
	BALL_UP_X,
	BALL_DOWN_X,

	TILE,
	PEN,
	ERASE,
	DESELECT
};

enum class SoundType {
	Button,
	Ball,
	Win,
	Fail,
	None
};