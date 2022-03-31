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
void ResourceManager::loadSounds()
{
	std::string name;
	std::string soundPath;
	std::string path = "Sounds/";
	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		soundPath = file.path().string();

		sf::SoundBuffer* temp = new sf::SoundBuffer;
		name = file.path().filename().string();
		name = name.substr(0, name.length() - 4);

		if (!temp->loadFromFile(soundPath))
		{
			printf("Error loading font: %s\n", soundPath.c_str());
			return;
		}

		// Sound is copied to map
		m_sounds[name] = temp;
		printf("Added %s at sounds[%s].\n", soundPath.c_str(), name.c_str());
	}
	m_buttonSound.setBuffer(*getSound("button_click"));
	m_buttonSound.setVolume(100);

	m_ballSound.setBuffer(*getSound("ball_tap"));
	m_buttonSound.setVolume(75);
}
sf::Texture* ResourceManager::getTexture(std::string textureKey)
{
	if (m_textures.find(textureKey) == m_textures.end())
		return m_textures["0"];

	return m_textures[textureKey];
}
sf::Font* ResourceManager::getFont(std::string fontKey)
{
	if (m_fonts.find(fontKey) == m_fonts.end())
		return m_fonts["default"];

	return m_fonts[fontKey];
}
sf::SoundBuffer* ResourceManager::getSound(std::string soundKey)
{
	if (m_sounds.find(soundKey) == m_sounds.end())
		return m_sounds["button_click"];

	return m_sounds[soundKey];
}
unsigned int ResourceManager::getVolume(enum class SoundType a_sound)
{
	switch (a_sound)
	{
	case SoundType::Button:
		return m_buttonSound.getVolume();
		break;
	case SoundType::Ball:
		return m_ballSound.getVolume();
		break;
	default:
		break;
	}
	return 0;
}
void ResourceManager::playSound(enum class SoundType a_sound)
{
	switch (a_sound)
	{
	case SoundType::Button:
		m_buttonSound.play();
		break;
	case SoundType::Ball:
		m_ballSound.play();
		break;
	default:
		break;
	}
	return;
}
void ResourceManager::setVolume(enum class SoundType a_sound, int a_volume)
{
	if (a_volume < 0)
	{
		m_buttonSound.setVolume(0);
		return;
	}
	else if (a_volume > 100)
	{
		m_buttonSound.setVolume(100);
		return;
	}
		
	switch (a_sound)
	{
	case SoundType::Button:
		m_buttonSound.setVolume(a_volume);
		break;
	case SoundType::Ball:
		m_ballSound.setVolume(a_volume);
		break;
	default:
		break;
	}
	return;
}
ResourceManager::~ResourceManager()
{
	for (const auto& t : m_textures)
		delete t.second;

	for (const auto& f : m_fonts)
		delete f.second;
}