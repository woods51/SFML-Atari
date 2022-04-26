#include "GameOver.h"

GameOver::GameOver(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture, unsigned int a_score)
{
	m_score = a_score;
	m_frameTexture = a_frameTexture;
	m_frameSprite.setTexture(m_frameTexture);

	a_rm.playSound(Sound::GameOver);
	generateUI(a_rm);
}
GameOver::~GameOver()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}
void GameOver::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
{
	// Button Selector Update
	sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
	static bool lock_click = false;

	for (auto b : m_buttons)
	{
		sf::Vector2f b_pos = b->getPosition();
		sf::Vector2f b_diag_pos = b->getDiagonalPosition();

		if (mousePosition.x >= b_pos.x && mousePosition.x <= b_diag_pos.x &&
			mousePosition.y >= b_pos.y && mousePosition.y <= b_diag_pos.y)
		{
			b->isSelected(true);
		}
		else
			b->isSelected(false);
	}

	// Handle Events
	sf::Event event;
	while (a_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			// Left Mouse Click
			if (event.mouseButton.button == sf::Mouse::Left && !lock_click)
			{
				lock_click = true;

				for (auto b : m_buttons)
				{
					sf::Vector2f b_pos = b->getPosition();
					sf::Vector2f b_diag_pos = b->getDiagonalPosition();

					if (mousePosition.x >= b_pos.x && mousePosition.x <= b_diag_pos.x &&
						mousePosition.y >= b_pos.y && mousePosition.y <= b_diag_pos.y)
					{
						switch (b->OnClick(a_rm))
						{
						case Press::MAINMENU:
							for (unsigned int i = 0; i < a_states.size(); i++)
								a_states.pop_back();
							break;
						case Press::QUIT:
							a_window.close();
							break;
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

		case sf::Event::Closed:
			a_window.close();
			break;
		}
	}
}
void GameOver::update(ResourceManager& a_rm, sf::Time a_dt)
{

}
void GameOver::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	a_window.draw(m_frameSprite);
	a_window.draw(m_overlay);

	a_window.draw(m_scoreText);
	a_window.draw(m_scoreValue);
	a_window.draw(m_gameText);

	// Render UI
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.display();
}
void GameOver::generateUI(ResourceManager& a_rm)
{
	// generate all buttons
	Button* temp;

	// Main Menu
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 4),
		Press::MAINMENU, "Menu");
	temp->setDefaultText(a_rm, 35, temp->getShape().getPosition() + sf::Vector2f(64.0f, 12.0f));
	m_buttons.push_back(temp);

	// Quit
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 76),
		Press::QUIT, "Quit");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(64.0f, 8.0f));
	m_buttons.push_back(temp);

	//UI
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));

	//Text
	setDefaultText(a_rm, m_scoreText, 30, sf::Vector2f(WIDTH / 2 - 180, HEIGHT / 2 -60));
	m_scoreText.setString("SCORE:");

	setDefaultText(a_rm, m_scoreValue, 60, sf::Vector2f(WIDTH / 2 -16, HEIGHT / 2 - 80));
	m_scoreValue.setString(std::to_string(m_score));

	setDefaultText(a_rm, m_gameText, 100, sf::Vector2f(WIDTH / 2 - 418, HEIGHT / 2 - 200), "default", sf::Color::Red);
	m_gameText.setString("GAME OVER");

}