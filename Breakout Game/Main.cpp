#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "GameObject.h"
#include "ResourceManager.h"
#include <iostream>
#include <Windows.h>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My Window");
	sf::Event event;
	
	sf::err().rdbuf(NULL);

	ResourceManager rm;
	std::vector<std::unique_ptr<Tile>> tiles;

	float posy = 0;
	for (auto i : { "1", "2", "1" })
	{
		generateTileRow(tiles, rm, posy, i);
		posy += 40.0f;
	}

	Ball ball(rm, sf::Vector2f(300.0f, 250.0f), "ball", sf::Vector2f(16.0f, 16.0f), sf::Vector2f(1.0f, 1.0f));
	/*for (auto tile : tiles)
	{
		std::cout << tile->sprite.getPosition().x << " " << tile->sprite.getPosition().y << std::endl;
		std::cout << tile->textureID << std::endl << std::endl;
	}*/

	Paddle paddle(rm, sf::Vector2f(700.0f, 550.0f), "paddle_2", sf::Vector2f(32.0f, 4.0f), sf::Vector2f(5.0f, 5.0f));

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
		// clear the window with black color
		// *** otherwise the contents of the previous frames will be present

		// draw everything here...

		window.clear(sf::Color::Black);

		// code

		update(tiles, ball, paddle);
		
		for (const auto& t : tiles)
		{
			sf::Sprite temp = t->sprite;
			window.draw(temp);
		}
		sf::Sprite temp = ball.sprite;
		window.draw(temp);
		temp = paddle.sprite;
		window.draw(temp);

		// end the current frame
		// *** copies local buffer to window
		window.display();
		//Sleep(25);
		//std::system("pause");
		//window.close();
	}
}