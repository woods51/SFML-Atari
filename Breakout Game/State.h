#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class State
{
public:
	State() {

	}
	/*
	State(ResourceManager& rm, sf::RenderWindow& window)
	{
		m_rm = rm;
		m_window = window;
	}
	*/
	virtual void inputHandler(sf::Keyboard::Key, bool) = 0;
	virtual void eventHandler(sf::RenderWindow&) = 0;
	virtual void update(sf::Time) = 0;
	virtual void render(sf::RenderWindow&) = 0;

	//ResourceManager* m_rm;
	//sf::RenderWindow* m_window;
};