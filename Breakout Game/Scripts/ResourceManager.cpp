#include "./ResourceManager.h"

ResourceManager::ResourceManager()
{
	loadTextures();
	loadFonts();
	loadSounds();
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

	generateSounds();
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
unsigned int ResourceManager::getVolume(enum class Sound a_sound)
{
	unsigned int volume = 0;
	switch (a_sound)
	{
	case Sound::Button:
		volume = (unsigned int)m_buttonPress.getVolume();
		break;
	case Sound::Ball:
		volume = (unsigned int)m_ballBounce.getVolume();
		break;
	case Sound::GameOver:
		volume = (unsigned int)m_gameOver.getVolume();
		break;
	case Sound::LevelComplete:
		volume = (unsigned int)m_levelComplete.getVolume();
		break;
	default:
		break;
	}
	return volume;
}
void ResourceManager::playSound(enum class Sound a_sound)
{
	switch (a_sound)
	{
	case Sound::Button:
		m_buttonPress.play();
		break;
	case Sound::Ball:
		m_ballBounce.play();
		break;
	case Sound::GameOver:
		m_gameOver.play();
		break;
	case Sound::LevelComplete:
		m_levelComplete.play();
		break;
	default:
		break;
	}
	return;
}
void ResourceManager::generateSounds()
{
	m_buttonPress.setBuffer(*getSound("button_click"));
	m_buttonPress.setVolume(100);

	m_ballBounce.setBuffer(*getSound("ball_tap"));
	m_ballBounce.setVolume(70);

	m_gameOver.setBuffer(*getSound("game_over"));
	m_gameOver.setVolume(50);

	m_levelComplete.setBuffer(*getSound("level_complete"));
	m_levelComplete.setVolume(50);
}
void ResourceManager::setVolume(enum class Sound a_sound, bool a_increase)
{
	float value = 10.001f;
	if (!a_increase)
		value = -9.901f;

	switch (a_sound)
	{
	case Sound::Button:
		adjustVolume(m_buttonPress, value);
		break;
	case Sound::Ball:
		adjustVolume(m_ballBounce, value);
		break;
	case Sound::GameOver:
		adjustVolume(m_gameOver, value);
		break;
	case Sound::LevelComplete:
		adjustVolume(m_levelComplete, value);
		break;
	default:
		break;
	}
	return;
}
void ResourceManager::adjustVolume(sf::Sound& a_sound, float a_value)
{
	float volume = a_sound.getVolume();

	if (volume + a_value < 0)
		volume = 0.0f;

	else if (volume + a_value > 100)
		volume = 100.0f;

	else
		volume += a_value;

	a_sound.setVolume(volume);
}
ResourceManager::~ResourceManager()
{
	for (const auto& t : m_textures)
		delete t.second;

	for (const auto& f : m_fonts)
		delete f.second;
}