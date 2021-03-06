#include "MainMenu.h"

MainMenu::MainMenu(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	generateUI(a_rm);
}

void MainMenu::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
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

void MainMenu::update(ResourceManager& a_rm, const sf::Time& a_dt)
{
	// Update background positions
	m_background.setPosition(m_background.getPosition() + sf::Vector2f(62.5f * a_dt.asSeconds(), 0));
	if (m_background.getPosition().x >= 3195)
	{
		m_background.setPosition(sf::Vector2f(-3195, 0));
	}

	m_background2.setPosition(m_background2.getPosition() + sf::Vector2f(62.5f * a_dt.asSeconds(), 0));
	if (m_background2.getPosition().x >= 3195)
	{
		m_background2.setPosition(sf::Vector2f(-3195, 0));
	}
}

void MainMenu::render(sf::RenderWindow& a_window)
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

	// Render title sprites
	if (m_breakoutFlag)
		a_window.draw(m_breakoutText);
	else if (m_editorFlag)
		a_window.draw(m_editorText);
	else if (m_pongFlag)
		a_window.draw(m_pongText);
	else
		a_window.draw(m_atariText);

	a_window.display();
}

MainMenu::~MainMenu()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}

void MainMenu::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
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
			case Press::BREAKOUT:
				a_states.push_back(std::make_unique<BreakoutMenu>(a_rm, a_window));
				break;

			case Press::EDITOR:
				a_states.push_back(std::make_unique<LevelEditor>(a_rm, a_window));
				break;

			case Press::PONG:
				a_states.push_back(std::make_unique<PongState>(a_rm, a_window));
				break;

			case Press::OPTIONS:
				a_states.push_back(std::make_unique<OptionsMenu>(a_rm, a_window, &m_background, &m_background2));
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

void MainMenu::buttonSelectUpdate(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition)
{
	for (auto& b : m_buttons)
	{
		sf::Vector2f b_pos = b->getPosition();
		sf::Vector2f b_diag_pos = b->getDiagonalPosition();

		if (a_mousePosition.x >= b_pos.x && a_mousePosition.x <= b_diag_pos.x &&
			a_mousePosition.y >= b_pos.y && a_mousePosition.y <= b_diag_pos.y)
		{
			b->isSelected(true);

			// Updates title sprite flags
			switch (b->getButtonType())
			{
			case Press::BREAKOUT:
				m_breakoutFlag = true;
				m_editorFlag = false;
				m_pongFlag = false;
				break;

			case Press::EDITOR:
				m_breakoutFlag = false;
				m_editorFlag = true;
				m_pongFlag = false;
				break;

			case Press::PONG:
				m_breakoutFlag = false;
				m_editorFlag = false;
				m_pongFlag = true;
				break;

			default:
				m_breakoutFlag = m_editorFlag = m_pongFlag = false;
				break;
			}
		}
		else
			b->isSelected(false);
	}
}

void MainMenu::generateUI(ResourceManager& a_rm)
{
	generateButtons(a_rm);

	// Generate title sprites
	generateSprites(a_rm);

	// Generate backgrounds
	m_frameTexture = *a_rm.getTexture("background_breakout_1");

	m_background.setTexture(m_frameTexture);
	m_background.setScale(40, 40);
	m_background2 = m_background;
	m_background2.setPosition(m_background.getPosition().x - 3195, 0);
}

void MainMenu::generateButtons(ResourceManager& a_rm)
{
	Button* temp;

	// Breakout
	temp = createMenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) - 100), Press::BREAKOUT, "Breakout");
	temp->setDefaultText(a_rm, 35, temp->getShape().getPosition() + sf::Vector2f(12, 10));
	m_buttons.push_back(temp);

	// Editor
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) + 136, (HEIGHT / 2) - 100),
		Press::EDITOR, sf::Vector2f(4, 4), sf::Vector2f(16, 16), "button_editor", "button_editor_selected");
	m_buttons.push_back(temp);

	// Pong
	temp = createMenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) - 28), Press::PONG, "Pong");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(64, 8));
	m_buttons.push_back(temp);

	// Options
	temp = createMenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 44), Press::OPTIONS, "Options");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(20, 8));
	m_buttons.push_back(temp);

	// Quit
	temp = createMenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 116), Press::QUIT, "Quit");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(64, 8));
	m_buttons.push_back(temp);
}

void MainMenu::generateSprites(ResourceManager& a_rm)
{
	m_breakoutText.setPosition((WIDTH / 2) - 324, 130);
	m_breakoutText.setTexture(*a_rm.getTexture("breakout_title"));
	m_breakoutText.setScale(sf::Vector2f(6, 6));

	m_editorText.setPosition((WIDTH / 2) - 350, 140);
	m_editorText.setTexture(*a_rm.getTexture("level_editor_title"));
	m_editorText.setScale(sf::Vector2f(5, 5));

	m_pongText.setPosition((WIDTH / 2) - 144, 130);
	m_pongText.setTexture(*a_rm.getTexture("pong_title"));
	m_pongText.setScale(sf::Vector2f(6, 6));

	m_atariText.setPosition((WIDTH / 2) - 295, 140);
	m_atariText.setTexture(*a_rm.getTexture("sfml_atari_title"));
	m_atariText.setScale(sf::Vector2f(5, 5));
}