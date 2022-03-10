#pragma once

#include <SFML\Graphics.hpp>
#include <filesystem>
#include <string>

class ResourceManager
{
private:
	std::string defaultTexture = "Assets/0.png";
public:
	std::map<std::string, sf::Texture> textures;

	ResourceManager();
	sf::Texture getTexture(std::string textureID);
	//~ResourceManager();
};