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
	std::vector<Tile*> tiles;
	float posx = 0;
	float posy = 0;
	for (int i = 0; i < 20; i++)
	{
		Tile* temp = new Tile(rm, sf::Vector2f(posx, posy), sf::Vector2f(2.5f, 2.5f), (std::string)"1");
		tiles.push_back(temp);
		posx += 40.0f;
	}
	for (auto tile : tiles)
	{
		std::cout << tile->sprite.getPosition().x << " " << tile->sprite.getPosition().y << std::endl;
		std::cout << tile->textureID << std::endl << std::endl;
	}

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				for (auto t : tiles)
				{
					Tile* temp = t;
					delete t;
				}

				window.close();
			}
		}
		// clear the window with black color
		// *** otherwise the contents of the previous frames will be present

		// draw everything here...

		window.clear(sf::Color::Black);

		// inside the main loop, between window.clear() and window.display()

		for (auto t : tiles)
		{
			sf::Sprite temp = t->sprite;
			window.draw(temp);
		}


		// end the current frame
		// *** copies local buffer to window 
		window.display();
		std::system("pause");
	}
}