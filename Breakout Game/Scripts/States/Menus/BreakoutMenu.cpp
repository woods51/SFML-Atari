#include "BreakoutMenu.h"

BreakoutMenu::BreakoutMenu(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	generateUI(a_rm);
}

void BreakoutMenu::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
{
	sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
	static bool lock_click = false;

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

void BreakoutMenu::update(ResourceManager& a_rm, const sf::Time& a_dt)
{
	// Re-align livesValue position based on value
	m_livesValue.setString(std::to_string(m_lives[m_livesIndex]));
	switch (m_livesIndex)
	{
	case 0:
		setDefaultText(a_rm, m_livesValue, 40, sf::Vector2f(WIDTH / 2 - 14, HEIGHT / 2 + 120));
		break;
	case 1:
	case 2:
		setDefaultText(a_rm, m_livesValue, 40, sf::Vector2f(WIDTH / 2 - 16, HEIGHT / 2 + 120));
		break;
	case 3:
	case 4:
		setDefaultText(a_rm, m_livesValue, 40, sf::Vector2f(WIDTH / 2 - 30, HEIGHT / 2 + 120));
		break;
	case 5:
		setDefaultText(a_rm, m_livesValue, 40, sf::Vector2f(WIDTH / 2 - 36, HEIGHT / 2 + 120));
		break;
	default:
		break;
	}

	// Update backgrounds
	m_background.setPosition(m_background.getPosition() + sf::Vector2f(0.25f, 0));
	if (m_background.getPosition().x == 3200)
	{
		m_background.setPosition(sf::Vector2f(-3200, 0));
	}

	m_background2.setPosition(m_background2.getPosition() + sf::Vector2f(0.25f, 0));
	if (m_background2.getPosition().x == 3200)
	{
		m_background2.setPosition(sf::Vector2f(-3200, 0));
	}
}

void BreakoutMenu::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	// Render backgrounds
	a_window.draw(m_background);
	a_window.draw(m_background2);

	// Render buttons
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	// Render title sprite
	a_window.draw(m_breakoutTitle);

	// Render text objects
	a_window.draw(m_livesText);
	a_window.draw(m_livesValue);

	a_window.display();
}

BreakoutMenu::~BreakoutMenu()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}

void BreakoutMenu::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
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
			case Press::BREAKOUT:
				a_states.push_back(std::make_unique<BreakoutState>(a_rm, a_window, m_lives[m_livesIndex]));
				break;

			case Press::EDITOR:
				a_states.push_back(std::make_unique<LevelEditor>(a_rm, a_window));
				break;

			case Press::LOAD:
				a_states.push_back(std::make_unique<LoadMenu>(a_rm, a_window, &m_background, &m_background2, m_lives[m_livesIndex]));
				break;

			case Press::BACK:
				a_states.pop_back();
				break;

			case Press::BUTTON_UP:
				if (++m_livesIndex > 5)
					m_livesIndex = 0;
				break;

			case Press::BUTTON_DOWN:
				if (--m_livesIndex < 0)
					m_livesIndex = 5;
				break;

			default:
				break;
			}
		}
	}
}

void BreakoutMenu::buttonSelectUpdate(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition)
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

void BreakoutMenu::generateUI(ResourceManager& a_rm)
{
	// Generate menu buttons
	generateButtons(a_rm);

	// Generate text
	setDefaultText(a_rm, m_livesValue, 40, sf::Vector2f(WIDTH/2 -16, HEIGHT/2 +120));
	m_livesValue.setString(m_lives[m_livesIndex]);

	setDefaultText(a_rm, m_livesText, 22, sf::Vector2f(WIDTH/2 - 46, HEIGHT/2 +100));
	m_livesText.setString("LIVES");

	// Generate title
	m_breakoutTitle.setPosition((WIDTH / 2) - 324, 130);
	m_breakoutTitle.setTexture(*a_rm.getTexture("breakout_title_1"));
	m_breakoutTitle.setScale(sf::Vector2f(6, 6));

	// Generate Backgrounds
	m_frameTexture = *a_rm.getTexture("background_breakout");

	m_background.setTexture(m_frameTexture);
	m_background.setScale(40, 40);
	m_background2 = m_background;
	m_background2.setPosition(m_background.getPosition().x - 3200, 0);
}

void BreakoutMenu::generateButtons(ResourceManager& a_rm)
{
	Button* temp;

	// Play
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) - 68), Press::BREAKOUT, "Play Game");
	temp->setDefaultText(a_rm, 30, temp->getShape().getPosition() + sf::Vector2f(13, 13));
	m_buttons.push_back(temp);

	// Load
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 4), Press::LOAD, "Load Level");
	temp->setDefaultText(a_rm, 28, temp->getShape().getPosition() + sf::Vector2f(14, 14));
	m_buttons.push_back(temp);

	// Editor
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) + 136, (HEIGHT / 2) - 32),
		Press::EDITOR, sf::Vector2f(4, 4), sf::Vector2f(16, 16), "button_editor", "button_editor_selected");
	m_buttons.push_back(temp);

	// Back
	temp = new Button(a_rm, sf::Vector2f(75, 100), Press::BACK, sf::Vector2f(4, 6),
		sf::Vector2f(32, 8), "button_menu", "button_menu_selected");
	temp->setString("BACK");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 10));
	m_buttons.push_back(temp);

	// Generate tick buttons
	temp = new TickButton(a_rm, sf::Vector2f(WIDTH / 2 + 6, (HEIGHT / 2) + 170), Press::BUTTON_UP, ">");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);

	temp = new TickButton(a_rm, sf::Vector2f(WIDTH / 2 - 54, (HEIGHT / 2) + 170), Press::BUTTON_DOWN, "<");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);
}