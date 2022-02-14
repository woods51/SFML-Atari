#include <SFML/Graphics.hpp>
#include <stdio.h>
#include "GameObject.h"

int main()
{
	//sf::RenderWindow window(sf::VideoMode(800, 600), "My Window");
	//sf::Event event;

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

	}

	return 0;
	/*
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
	}*/
}