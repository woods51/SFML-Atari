#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Button.h"

class State
{
public:
	State() {

	}
	virtual void inputHandler(sf::Keyboard::Key, bool) = 0;
	virtual void eventHandler(sf::RenderWindow&) = 0;
	virtual void update(sf::Time, ResourceManager&) = 0;
	virtual void render(sf::RenderWindow&) = 0;

};