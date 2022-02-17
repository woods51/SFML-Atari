#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include <map>

class ResourceManager
{
private:
	std::string defaultTexture = "Assets/0.png";
public:
	std::map<std::string, sf::Texture> textures;

	ResourceManager()
	{
		std::string name;
		std::string texturePath;
		std::string path = "\Assets/";
		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			texturePath = file.path().string();
			//if (texturePath == defaultTexture)
			//	continue;

			sf::Texture temp;
			name = file.path().filename().string();
			name = name.substr(0, name.length() - 4);

			if (!temp.loadFromFile(texturePath))
			{
				printf("Error loading texture: %s\n", texturePath.c_str());

				temp.loadFromFile(defaultTexture);
			}
			
			// Texture is copied to map
			textures[name] = temp;
			printf("Added %s at textures[%s].\n", texturePath.c_str(), name.c_str());
		}

	}
	sf::Texture getTexture(std::string textureID)
	{
		return textures[textureID];
	}
	//~ResourceManager();
};


class Tile
{
public:
	Tile(ResourceManager& rm, sf::Vector2f pos = sf::Vector2f(0,0), sf::Vector2f scale = sf::Vector2f(1.0f, 1.0f), std::string textureID = "0") : textureID(textureID)
	{
		texture = rm.getTexture(textureID);
		sprite.setTexture(texture);
		sprite.setPosition(pos);
		sprite.setScale(scale);
	}

	sf::Sprite sprite;
	sf::Texture texture;
	std::string textureID;

	sf::Vector2f getDiagonalPos()
	{
		return sprite.getPosition() + (16.0f * sprite.getScale());
	}

	bool ballCollision(sf::Vector2f obj_pos, sf::Vector2f obj_pos_diagonal)
	{
		sf::Vector2f pos = this->sprite.getPosition();
		sf::Vector2f diagonal_pos = this->getDiagonalPos();

		sf::Vector2f p1(obj_pos.x, obj_pos.y);
		sf::Vector2f p2(obj_pos_diagonal.x, obj_pos.y);
		sf::Vector2f p3(obj_pos.x, obj_pos_diagonal.y);
		sf::Vector2f p4(obj_pos_diagonal.x, obj_pos_diagonal.y);

		//std::vector<sf::Vector2f> points {p1, p2, p3, p4};

		for (auto p : { p1, p2, p3, p4 })
		{
			if ((p.x >= pos.x && p.x <= diagonal_pos.x) &&
				(p.y >= pos.y && p.y <= diagonal_pos.y))
				return true;

			continue;
		}
		return false;
	}
};