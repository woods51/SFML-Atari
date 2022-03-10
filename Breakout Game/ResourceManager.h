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
public:
	std::map<std::string, sf::Texture*> textures;
	void load();

	ResourceManager();
	sf::Texture* getTexture(std::string textureID);
	//~ResourceManager();
};