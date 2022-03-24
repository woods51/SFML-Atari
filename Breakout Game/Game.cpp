#include "Game.h"

void Game::run()
{
	ResourceManager rm;
	rm.loadTextures();
	rm.loadFonts();
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My Window");

	m_states.push_back(std::make_unique<PlayState>(rm, window));
	sf::err().rdbuf(NULL);
	window.setFramerateLimit(240);

	sf::Clock deltaClock;
	sf::Time deltaTime;

	while (window.isOpen())
	{
		//system("pause");
		m_states.at(0)->eventHandler(window);

		deltaTime = deltaClock.restart();
		m_states.at(0)->update(deltaTime, rm);
		deltaClock.restart();

		m_states.at(0)->render(window);
	}
}