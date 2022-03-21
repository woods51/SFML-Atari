#pragma once

#include <SFML\Graphics.hpp>
#include <filesystem>
#include <string>

#define WIDTH 800
#define HEIGHT 600
#define MULTIPLIER 25

class ResourceManager
{
private:
	std::string defaultTexture = "Assets/0.png";
	std::string defaultFont = "default.TTF";
public:
	std::map<std::string, sf::Texture*> textures;
	std::map<std::string, sf::Font*> fonts;
	void loadTextures();
	void loadFonts();

	ResourceManager();
	sf::Texture* getTexture(std::string textureID);
	sf::Font* getFont(std::string fontID);
	~ResourceManager();
};