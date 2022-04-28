#include "./ResourceManager.h"

ResourceManager::ResourceManager()
{
	// Load assets
	loadTextures();
	loadFonts();
	loadSounds();
}

ResourceManager::~ResourceManager()
{
	for (auto& t : m_textures)
		delete t.second;

	for (auto& f : m_fonts)
		delete f.second;

	for (auto& s : m_sounds)
		delete s.second;
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
			delete temp;
			continue;
		}

		// Texture is copied to map
		m_textures[name] = temp;
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
			delete temp;
			continue;
		}

		// Font is copied to map
		m_fonts[name] = temp;
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
			delete temp;
			continue;
		}

		// Sound is copied to map
		m_sounds[name] = temp;
	}

	generateSounds();
}

sf::Texture* ResourceManager::getTexture(std::string a_textureKey)
{
	if (m_textures.find(a_textureKey) == m_textures.end())
		return m_textures["default"];

	return m_textures[a_textureKey];
}

sf::Font* ResourceManager::getFont(std::string a_fontKey)
{
	if (m_fonts.find(a_fontKey) == m_fonts.end())
		return m_fonts["default"];

	return m_fonts[a_fontKey];
}

sf::SoundBuffer* ResourceManager::getSound(std::string a_soundKey)
{
	if (m_sounds.find(a_soundKey) == m_sounds.end())
		return m_sounds["button_click"];

	return m_sounds[a_soundKey];
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
	// Prevents unintended volume incrementation
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