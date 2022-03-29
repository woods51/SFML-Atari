#include "MenuState.h"

MenuState::MenuState(ResourceManager& rm, sf::RenderWindow& window)
{
	generateUI(rm);
}
MenuState::~MenuState()
{
	for (const auto& b : m_buttons)
	{
		delete b;
	}
}
void MenuState::inputHandler(sf::Keyboard::Key key, bool isPressed)
{

}
void MenuState::eventHandler(sf::RenderWindow& window, ResourceManager& rm, std::vector<std::unique_ptr<State>>& states)
{
	sf::Event event;
	static bool lock_click = false;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			// Left Mouse Click
			if (event.mouseButton.button == sf::Mouse::Left && !lock_click)
			{
				sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				lock_click = true;

				for (auto b : m_buttons)
				{
					if (mousePosition.y < b->getPosition().y || mousePosition.x < b->getPosition().x)
						continue;

					sf::Vector2f b_pos = b->getPosition();
					sf::Vector2f b_diag_pos = b->getDiagonalPosition();

					if (mousePosition.x >= b_pos.x && mousePosition.x <= b_diag_pos.x &&
						mousePosition.y >= b_pos.y && mousePosition.y <= b_diag_pos.y)
					{
						switch (b->OnClick())
						{
						case B_START:
							states.push_back(std::make_unique<PlayState>(rm, window));
							break;
						case B_QUIT:
							window.close();
						default:
							break;
						}
					}
				}
			}
			break;

		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left && lock_click)
				lock_click = false;
			break;

		case sf::Event::KeyPressed:
			inputHandler(event.key.code, true);
			break;

		case sf::Event::Resized:
			break;

		case sf::Event::KeyReleased:
			inputHandler(event.key.code, false);
			break;

		case sf::Event::Closed:
			window.close();
			break;
		}
	}
}
void MenuState::update(sf::Time dt, ResourceManager& rm)
{

}
void MenuState::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::Black);

	// Render UI
	for (const auto& b : m_buttons)
	{
		window.draw(b->getShape());
		window.draw(b->getText());
	}

	for (auto b : m_buttons)
	{
		window.draw(b->getShape());
		window.draw(b->getText());
	}

	window.draw(m_breakoutText);

	window.display();
}
void MenuState::generateUI(ResourceManager& rm)
{
	// generate all buttons
	Button* temp = new PlayButton(rm);
	m_buttons.push_back(temp);

	temp = new QuitButton(rm);
	m_buttons.push_back(temp);

	// generate text UI
	m_breakoutText.setPosition((WIDTH / 2) - 108*3, 150);
	m_breakoutText.setTexture(*rm.getTexture("breakout_title"));
	m_breakoutText.setScale(sf::Vector2f(6.0f, 6.0f));
}