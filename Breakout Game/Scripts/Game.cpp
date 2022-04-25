#include "Game.h"

void Game::run()
{
	// Setup RenderWindow, View and FPS
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML Atari");
	sf::View view(sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(WIDTH, HEIGHT));
	window.setView(view);
	window.setFramerateLimit(240);

	// Initialize Game States
	ResourceManager rm;
	m_states.push_back(std::make_unique<MenuState>(rm, window));

	// Turning off console errors
	sf::err().rdbuf(NULL);
	
	// Delta Time
	sf::Clock deltaClock;
	sf::Time deltaTime;

	// Main Game Loop
	while (window.isOpen())
	{
		if (m_states.size() == 0)
			break;

		m_states.at(m_states.size() - 1)->eventHandler(rm, window, m_states);

		deltaTime = deltaClock.restart();
		m_states.at(m_states.size() - 1)->update(rm, deltaTime);
		deltaClock.restart();

		m_states.at(m_states.size() - 1)->render(window);
	}
	exit();
}
void Game::exit()
{
	m_states.clear();
}