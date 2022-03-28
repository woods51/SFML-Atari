#include "Game.h"

void Game::run()
{
	// Initializing Game Elements
	ResourceManager rm;
	rm.loadTextures();
	rm.loadFonts();

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My Window");
	window.setFramerateLimit(240);

	m_states.push_back(std::make_unique<PlayState>(rm, window));

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
		
		m_states.at(m_states.size() - 1)->eventHandler(window, view);

		deltaTime = deltaClock.restart();
		m_states.at(m_states.size() - 1)->update(deltaTime, rm);
		deltaClock.restart();

		m_states.at(m_states.size() - 1)->render(window);
	}
}