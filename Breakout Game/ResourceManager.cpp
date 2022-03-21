#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}
void ResourceManager::loadTextures()
{
	std::string name;
	std::string texturePath;
	std::string path = "\Assets/";
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		texturePath = file.path().string();

		sf::Texture* temp = new sf::Texture;
		name = file.path().filename().string();
		name = name.substr(0, name.length() - 4);

		if (!temp->loadFromFile(texturePath))
		{
			printf("Error loading texture: %s\n", texturePath.c_str());

			temp->loadFromFile(defaultTexture);
		}

		// Texture is copied to map
		textures[name] = temp;
		printf("Added %s at textures[%s].\n", texturePath.c_str(), name.c_str());
	}
}
void ResourceManager::loadFonts()
{
	std::string name;
	std::string fontPath;
	std::string path = "\Fonts/";
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		fontPath = file.path().string();

		sf::Font* temp = new sf::Font;
		name = file.path().filename().string();
		name = name.substr(0, name.length() - 4);

		if (!temp->loadFromFile(fontPath))
		{
			printf("Error loading font: %s\n", fontPath.c_str());

			temp->loadFromFile(defaultFont);
		}

		// Font is copied to map
		fonts[name] = temp;
		printf("Added %s at fonts[%s].\n", fontPath.c_str(), name.c_str());
	}
}
sf::Texture* ResourceManager::getTexture(std::string textureID)
{
	if (textures.find(textureID) == textures.end())
		return textures["0"];

	return textures[textureID];
}
sf::Font* ResourceManager::getFont(std::string fontID)
{
	if (fonts.find(fontID) == fonts.end())
		return fonts["default"];

	return fonts[fontID];
}
ResourceManager::~ResourceManager()
{
	for (auto t : textures)
		delete t.second;

	for (auto f : fonts)
		delete f.second;
}