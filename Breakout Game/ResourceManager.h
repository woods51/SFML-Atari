#pragma once

#include <SFML\Graphics.hpp>
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

	sf::Texture* getTexture(std::string textureID);
	sf::Font* getFont(std::string fontID);
	
	
private:
	std::map<std::string, sf::Texture*> m_textures;
	std::map<std::string, sf::Font*> m_fonts;

	std::string m_defaultTexture = "Assets/0.png";
	std::string m_defaultFont = "Fonts/default.TTF";
};