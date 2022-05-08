#include "GameOver.h"

GameOver::GameOver(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture, unsigned int a_score)
{
	m_score = a_score;

	// Setup background
	m_frameTexture = a_frameTexture;
	m_frameSprite.setTexture(m_frameTexture);

	sf::Vector2u windowSize = a_window.getSize();
	m_frameSprite.setScale((float)WIDTH / windowSize.x, (float)HEIGHT / windowSize.y);

	a_rm.playSound(Sound::GameOver);
	generateUI(a_rm);
}

void GameOver::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
{
	sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
	bool lock_click = false;

	buttonSelectUpdate(a_rm, mousePosition);

	sf::Event event;
	while (a_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left && !lock_click)
			{
				lock_click = true;
				handleButtonEvents(a_rm, a_window, a_states, mousePosition);
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

void GameOver::update(ResourceManager& a_rm, const sf::Time& a_dt) {}

void GameOver::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	// Render background
	a_window.draw(m_frameSprite);
	a_window.draw(m_overlay);

	// Render text objects
	a_window.draw(m_scoreText);
	a_window.draw(m_scoreValue);
	a_window.draw(m_gameText);

	// Render buttons
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.display();
}

GameOver::~GameOver()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}

void GameOver::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
	const sf::Vector2f& a_mousePosition)
{
	for (auto& b : m_buttons)
	{
		sf::Vector2f b_pos = b->getPosition();
		sf::Vector2f b_diag_pos = b->getDiagonalPosition();

		if (a_mousePosition.x >= b_pos.x && a_mousePosition.x <= b_diag_pos.x &&
			a_mousePosition.y >= b_pos.y && a_mousePosition.y <= b_diag_pos.y)
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
void GameOver::buttonSelectUpdate(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition)
{
	for (auto& b : m_buttons)
	{
		sf::Vector2f b_pos = b->getPosition();
		sf::Vector2f b_diag_pos = b->getDiagonalPosition();

		if (a_mousePosition.x >= b_pos.x && a_mousePosition.x <= b_diag_pos.x &&
			a_mousePosition.y >= b_pos.y && a_mousePosition.y <= b_diag_pos.y)
		{
			b->isSelected(true);
		}
		else
			b->isSelected(false);
	}
}

void GameOver::generateUI(ResourceManager& a_rm)
{
	// Generate buttons
	Button* temp;

	// MainMenu
	temp = createMenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 4),
		Press::MAINMENU, "Menu");
	temp->setDefaultText(a_rm, 35, temp->getShape().getPosition() + sf::Vector2f(64.0f, 12.0f));
	m_buttons.push_back(temp);

	// Quit
	temp = createMenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 76),
		Press::QUIT, "Quit");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(64.0f, 8.0f));
	m_buttons.push_back(temp);

	// Generate text objects
	setDefaultText(a_rm, m_scoreText, 30, sf::Vector2f(WIDTH / 2 - 180, HEIGHT / 2 -60));
	m_scoreText.setString("SCORE:");

	setDefaultText(a_rm, m_scoreValue, 60, sf::Vector2f(WIDTH / 2 -16, HEIGHT / 2 - 80));
	m_scoreValue.setString(std::to_string(m_score));

	setDefaultText(a_rm, m_gameText, 100, sf::Vector2f(WIDTH / 2 - 418, HEIGHT / 2 - 200), "default", sf::Color::Red);
	m_gameText.setString("GAME OVER");

	// Generate background
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));
}