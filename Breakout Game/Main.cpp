#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "GameObject.h"
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "My Window");
	sf::Event event;

	/*
		std::string buffer;
		std::string path = "\Assets/";
		for (const auto& file : std::filesystem::directory_iterator(path))
		{
			std::cout << file.path().filename().string() << std::endl;
			buffer = file.path().string();
			std::cout << buffer << std::endl;

			std::string name = file.path().filename().string();
			name = name.substr(0, name.length() - 4);
			printf("name: %s\n", name.c_str());

		}*/
	
	ResourceManager rm;
	std::vector<std::unique_ptr<Tile>> tiles;
	float y1 = 0;
	float y2 = 40.0f;
	float y3 = 80.0f;
	float posx = 0;
	for (int i = 0; i < 20; i++)
	{
		tiles.push_back(std::make_unique<Tile>(rm, sf::Vector2f(posx, y1), "1"));
		tiles.push_back(std::make_unique<Tile>(rm, sf::Vector2f(posx, y2), "2"));
		tiles.push_back(std::make_unique<Tile>(rm, sf::Vector2f(posx, y3), "1"));
		posx += 40.0f;
	}
	Tile ball(rm, sf::Vector2f(300.0f, 250.0f), "0");
	/*for (auto tile : tiles)
	{
		std::cout << tile->sprite.getPosition().x << " " << tile->sprite.getPosition().y << std::endl;
		std::cout << tile->textureID << std::endl << std::endl;
	}*/

	Tile paddle(rm, sf::Vector2f(700.0f, 550.0f), "paddle", sf::Vector2f(16.0f, 4.0f), sf::Vector2f(5.0f, 5.0f));

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
		/*
		for (const auto& t : tiles)
		{
			std::cout << t->sprite.getPosition().x << " " << t->sprite.getPosition().y <<
				(t->Collision(ball.sprite.getPosition(), ball.getDiagonalPos()) ? "true" : "false") << std::endl;
		}
		printf("%f,%f\n%f,%f\n", ball.sprite.getPosition().x, ball.sprite.getPosition().y, ball.getDiagonalPos().x, ball.getDiagonalPos().y);*/

		// end the current frame
		// *** copies local buffer to window
		window.display();
		//std::system("pause");
		//window.close();
	}
}