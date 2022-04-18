#include "Game.h"

void Game::run()
{
	// Initializing Game Elements
	ResourceManager rm;
	rm.loadTextures();
	rm.loadFonts();
	rm.loadSounds();

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML Atari");
	window.setFramerateLimit(240);
	sf::Texture temp;
	//m_states.push_back(std::make_unique<BreakoutState>(rm, window));
	m_states.push_back(std::make_unique<MenuState>(rm, window));
	//m_states.push_back(std::make_unique<LoadMenu>(rm, window, temp));
	//m_states.push_back(std::make_unique<LevelEditor>(rm, window));
	//m_states.push_back(std::make_unique<EditorMenu>(rm, window));
	//m_states.push_back(std::make_unique<PauseState>(rm, window));
	//m_states.push_back(std::make_unique<PongState>(rm, window));
	
	//m_states.push_back(std::make_unique<OptionsState>(rm, window, temp));

	// Turning off console errors
	sf::err().rdbuf(NULL);

	sf::View view(sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(WIDTH, HEIGHT));
	window.setView(view);
	
	// Delta Time
	sf::Clock deltaClock;
	sf::Time deltaTime;

	while (window.isOpen())
	{
		//system("pause");
		
		m_states.at(m_states.size() - 1)->eventHandler(window, rm, m_states);

		deltaTime = deltaClock.restart();
		m_states.at(m_states.size() - 1)->update(deltaTime, rm);
		deltaClock.restart();

		m_states.at(m_states.size() - 1)->render(window);
	}
}