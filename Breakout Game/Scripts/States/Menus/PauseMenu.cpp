#include "PauseMenu.h"

PauseMenu::PauseMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture)
{
	// Setup background
	m_frameTexture = a_frameTexture;
	m_frameSprite.setTexture(m_frameTexture);

	sf::Vector2u windowSize = a_window.getSize();
	m_frameSprite.setScale((float)WIDTH/windowSize.x, (float)HEIGHT/windowSize.y);

	generateUI(a_rm);
}

void PauseMenu::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
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

void PauseMenu::update(ResourceManager& a_rm, const sf::Time& a_dt) {}

void PauseMenu::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);
	
	// Render backgrounds
	a_window.draw(m_frameSprite);
	a_window.draw(m_overlay);

	// Render buttons
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.display();
}

PauseMenu::~PauseMenu()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}

void PauseMenu::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
	const sf::Vector2f& a_mousePosition)
{
	for (auto b : m_buttons)
	{
		sf::Vector2f b_pos = b->getPosition();
		sf::Vector2f b_diag_pos = b->getDiagonalPosition();

		if (a_mousePosition.x >= b_pos.x && a_mousePosition.x <= b_diag_pos.x &&
			a_mousePosition.y >= b_pos.y && a_mousePosition.y <= b_diag_pos.y)
		{
			switch (b->OnClick(a_rm))
			{
			case Press::RESUME:
				a_states.pop_back();
				break;

			case Press::OPTIONS:
				a_states.push_back(std::make_unique<OptionsMenu>(a_rm, a_window, m_frameTexture));
				break;

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

void PauseMenu::buttonSelectUpdate(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition)
{
	for (auto b : m_buttons)
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

void PauseMenu::generateUI(ResourceManager& a_rm)
{
	generateButtons(a_rm);

	// Generate background
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));
}

void PauseMenu::generateButtons(ResourceManager& a_rm)
{
	Button* temp;

	// Resume
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) - 140),
		Press::RESUME, "Resume");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(29.0f, 8.0f));
	m_buttons.push_back(temp);

	// Options
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) - 68),
		Press::OPTIONS, "Options");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(20.0f, 8.0f));
	m_buttons.push_back(temp);

	// MainMenu
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 4),
		Press::MAINMENU, "MainMenu");
	temp->setDefaultText(a_rm, 35, temp->getShape().getPosition() + sf::Vector2f(15.0f, 12.0f));
	m_buttons.push_back(temp);

	// Quit
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 76),
		Press::QUIT, "Quit");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(64.0f, 8.0f));
	m_buttons.push_back(temp);
}