#pragma once
#include "State.h"
/*
program that doesn't emulate the tank game from the atari.
its actually now a program that allows you create a custom breakout level and save it as a playable level.
-there is a grid setup for tiles, user can place any tile they want into a specific location within the grid,
	when placed the tile will snap to the nearest location on the grid.
-there is a asset manager menu that allows the user to drag
code 
code
more code
more code
some other code
complex algorithm
more code
you get the point
*/

class LevelEditor : public State
{
public:
	LevelEditor(ResourceManager& a_rm, sf::RenderWindow& a_window)
	{

	};

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(sf::Time dt, ResourceManager& a_window) override;
	void render(sf::RenderWindow& a_window) override;

	~LevelEditor();
private:
	void updateUI();
	void generateUI(ResourceManager& a_rm);
};