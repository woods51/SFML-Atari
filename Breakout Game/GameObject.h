#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include <map>

class ResourceManager
{
private:
	std::string defaultTexture = "Assets/default.png";
public:
	std::map<std::string, sf::Texture*> textures;

	ResourceManager()
	{
		std::string name;
		std::string texturePath;
		std::string path = "\Assets/";
		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			texturePath = path + file.path().string();
			if (texturePath == defaultTexture)
				continue;

			sf::Texture* temp = new sf::Texture();
			name = file.path().filename().string();
			name = name.substr(0, name.length() - 4);

			if (!temp->loadFromFile(texturePath))
			{
				printf("Error loading texture: %s", texturePath.c_str());

				temp->loadFromFile(defaultTexture);
			}

			textures[name] = temp;
		}

	}
	sf::Texture getTexture(std::string textureID)
	{
		sf::Texture* temp = this->textures[textureID];
		return *temp;
	}
	~ResourceManager()
	{
		for (auto iter = textures.begin(); iter != textures.end(); iter++)
		{
			delete(iter->second);
		}
	}
};


class Tile
{
public:
	Tile(ResourceManager rm, sf::Vector2f pos = sf::Vector2f(0,0), sf::Vector2f scale = sf::Vector2f(0, 0), std::string textureID = "default")
	{
		sprite.setPosition(pos);
		sprite.setScale(scale);

		sprite.setTexture(rm.getTexture(textureID));
	}

	sf::Sprite sprite;

	sf::Vector2f getDiagonolPos()
	{
		return sprite.getPosition() + (sprite.getPosition() * sprite.getScale().x);
	}
};