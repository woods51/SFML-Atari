#include "Game.h"

void Game::run()
{
	m_states.push_back(std::make_unique<PlayState>(&rm, window));
	sf::err().rdbuf(NULL);
	window->setFramerateLimit(240);

	sf::Clock deltaClock;
	sf::Time deltaTime;

	while (window->isOpen())
	{
		system("pause");
		m_states.at(0)->eventHandler();

		deltaTime = deltaClock.restart();
		m_states.at(0)->update(deltaTime);
		deltaClock.restart();

		m_states.at(0)->render();
	}
}