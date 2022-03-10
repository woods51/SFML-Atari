#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	std::string name;
	std::string texturePath;
	std::string path = "\Assets/";
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		texturePath = file.path().string();

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
sf::Texture ResourceManager::getTexture(std::string textureID)
{
	if (textures.find(textureID) == textures.end())
		return textures["0"];

	return textures[textureID];
}