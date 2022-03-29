#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}
void ResourceManager::loadTextures()
{
	std::string name;
	std::string texturePath;
	std::string path = "Assets/";
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		texturePath = file.path().string();

		sf::Texture* temp = new sf::Texture;
		name = file.path().filename().string();
		name = name.substr(0, name.length() - 4);

		if (!temp->loadFromFile(texturePath))
		{
			printf("Error loading texture: %s\n", texturePath.c_str());

			temp->loadFromFile(m_defaultTexture);
		}

		// Texture is copied to map
		m_textures[name] = temp;
		printf("Added %s at textures[%s].\n", texturePath.c_str(), name.c_str());
	}
}
void ResourceManager::loadFonts()
{
	std::string name;
	std::string fontPath;
	std::string path = "Fonts/";
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		fontPath = file.path().string();

		sf::Font* temp = new sf::Font;
		name = file.path().filename().string();
		name = name.substr(0, name.length() - 4);

		if (!temp->loadFromFile(fontPath))
		{
			printf("Error loading font: %s\n", fontPath.c_str());

			temp->loadFromFile(m_defaultFont);
		}

		// Font is copied to map
		m_fonts[name] = temp;
		printf("Added %s at fonts[%s].\n", fontPath.c_str(), name.c_str());
	}
}
sf::Texture* ResourceManager::getTexture(std::string textureID)
{
	if (m_textures.find(textureID) == m_textures.end())
		return m_textures["0"];

	return m_textures[textureID];
}
sf::Font* ResourceManager::getFont(std::string fontID)
{
	if (m_fonts.find(fontID) == m_fonts.end())
		return m_fonts["default"];

	return m_fonts[fontID];
}
ResourceManager::~ResourceManager()
{
	for (const auto& t : m_textures)
		delete t.second;

	for (const auto& f : m_fonts)
		delete f.second;
}