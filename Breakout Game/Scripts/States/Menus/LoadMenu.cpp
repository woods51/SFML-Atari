#include "LoadMenu.h"

LoadMenu::LoadMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Sprite* a_background, sf::Sprite* a_background2, unsigned int a_lives)
{
	m_lives = a_lives;

	m_background = a_background;
	m_background2 = a_background2;
	m_background->setPosition(m_background->getPosition() + sf::Vector2f(0.25f, 0));
	m_background2->setPosition(m_background2->getPosition() + sf::Vector2f(0.25f, 0));

	generateUI(a_rm);

	m_loader.loadFiles(m_loadPath, m_levels);

	// Compute max pages
	m_maxPages = m_levels.size() / 5;
	if (m_levels.size() % 5 != 0)
		m_maxPages++;

	loadFirstPage();
}

void LoadMenu::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
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

void LoadMenu::update(ResourceManager& a_rm, const sf::Time& a_dt)
{
	// Update page number
	m_pageNumber.setString(std::to_string(m_currentPage));

	// Update backgrounds
	m_background->setPosition(m_background->getPosition() + sf::Vector2f(62.5f * a_dt.asSeconds(), 0));
	if (m_background->getPosition().x >= 3195)
	{
		m_background->setPosition(sf::Vector2f(-3195, 0));
	}

	m_background2->setPosition(m_background2->getPosition() + sf::Vector2f(62.5f * a_dt.asSeconds(), 0));
	if (m_background2->getPosition().x >= 3195)
	{
		m_background2->setPosition(sf::Vector2f(-3195, 0));
	}
}

void LoadMenu::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);
	
	// Render backgrounds
	a_window.draw(*m_background);
	a_window.draw(*m_background2);
	a_window.draw(m_overlay);

	// Render text objects
	a_window.draw(m_pageNumber);
	a_window.draw(m_levelText);
	a_window.draw(m_selectedLevel);
	a_window.draw(m_loadError);

	// Render buttons
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}
	for (const auto& b : m_levelButtons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.display();
}

LoadMenu::~LoadMenu()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}

void LoadMenu::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
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
			case Press::LEVEL:
				handleLevel(b);
				break;

			case Press::BACK:
				a_states.pop_back();
				break;

			case Press::NEXT:
				nextPage();
				break;

			case Press::PREVIOUS:
				previousPage();
				break;

			case Press::RELOAD:
				reload();
				break;

			case Press::LOAD:
				if (!m_isValid)
					break;

				loadLevel(a_rm, a_window, a_states);
				break;

			default:
				break;
			}
		}
	}
}

void LoadMenu::buttonSelectUpdate(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition)
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

void LoadMenu::generateUI(ResourceManager& a_rm)
{
	generateButtons(a_rm);

	// Generate text objects
	setDefaultText(a_rm, m_pageNumber, 40, sf::Vector2f(WIDTH / 2 - 14, HEIGHT / 2 - 210));
	m_pageNumber.setString(std::to_string(m_currentPage));

	setDefaultText(a_rm, m_selectedLevel, 25, sf::Vector2f(896, HEIGHT / 2 - 136));
	m_selectedLevel.setString("");

	setDefaultText(a_rm, m_levelText, 20, sf::Vector2f((WIDTH / 2) - 36, HEIGHT / 2 - 240));
	m_levelText.setString("PAGE");

	setDefaultText(a_rm, m_loadError, 20, sf::Vector2f(24, HEIGHT / 2 - 136), "default", sf::Color::Red);

	// Generate background
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));
}

void LoadMenu::generateButtons(ResourceManager & a_rm)
{
	Button* temp;

	// Generate Tick Buttons
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6, (HEIGHT / 2) - 150), Press::NEXT, ">", Sound::Button);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54, (HEIGHT / 2) - 150), Press::PREVIOUS, "<", Sound::Button);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// Back
	temp = new Button(a_rm, sf::Vector2f(75, 100),
		Press::BACK, sf::Vector2f(4, 6), sf::Vector2f(32, 8), "button_menu", "button_menu_selected");
	temp->setString("BACK");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 10));
	m_buttons.push_back(temp);

	// Load
	temp = new Button(a_rm, sf::Vector2f(896, HEIGHT / 2 - 100),
		Press::LOAD, sf::Vector2f(8, 12), sf::Vector2f(32, 8), "button_menu", "button_menu_selected");
	temp->setString("LOAD");
	temp->setDefaultText(a_rm, 50, temp->getShape().getPosition() + sf::Vector2f(40, 20));
	m_buttons.push_back(temp);

	// Reload
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) + 100, (HEIGHT / 2) - 210),
		Press::RELOAD, sf::Vector2f(3, 3), sf::Vector2f(16, 16), "button_reload", "button_reload");
	m_buttons.push_back(temp);

	// Generate level buttons
	float posY = HEIGHT / 2 - 100;
	for (int i = 0; i < 5; i++)
	{
		temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - (32 * 4), posY), Press::LEVEL, sf::Vector2f(8, 8), sf::Vector2f(32, 6),
			"loaded_level", "loaded_level_selected");
		temp->setDefaultText(a_rm, 20, temp->getPosition() + sf::Vector2f(8, 10));

		m_buttons.push_back(temp);
		m_levelButtons.push_back(temp);
		posY += 60;
	}
}

void LoadMenu::loadPage(int start, int end)
{
	for (int i = 0; i < 5; start++, i++)
	{
		if (!(start < end))
			m_levelButtons[i]->setString("");

		else
			m_levelButtons[i]->setString(m_levels[start]);
	}
}

void LoadMenu::loadFirstPage()
{
	m_currentPage = 1;

	// Reset level selection
	m_selectedLevel.setString("");
	m_isValid = false;

	if (m_levels.size() > 5)
		loadPage(0, 5);
	else
		loadPage(0, m_levels.size());
}

void LoadMenu::nextPage()
{
	if (m_currentPage == m_maxPages)
		return;

	m_currentPage++;

	int start = (m_currentPage - 1) * 5;
	int end;

	// Compute remaining levels
	if ((m_currentPage * 5) > m_levels.size())
		end = (m_levels.size() - (m_currentPage - 1) * 5) + start;
	else
		end = m_currentPage * 5;

	loadPage(start, end);
}

void LoadMenu::previousPage()
{
	if (m_currentPage == 1)
		return;

	m_currentPage--;

	int start;
	int end;

	start = (m_currentPage - 1) * 5;

	// Compute remaining levels
	if ((m_currentPage * 5) > m_levels.size())
		end = m_levels.size() - (m_currentPage - 1) * 5;
	else
		end = m_currentPage * 5;

	loadPage(start, end);
}

void LoadMenu::reload()
{
	// Reloads level files
	m_loader.loadFiles(m_loadPath, m_levels);

	// Compute max pages
	m_maxPages = m_levels.size() / 5;
	if (m_levels.size() % 5 != 0)
		m_maxPages++;

	loadFirstPage();
}

void LoadMenu::handleLevel(Button*& a_button)
{
	// Checks for valid level name
	std::string level = a_button->getText().getString().toAnsiString();
	if (level.size() < 3)
		return;

	m_selectedLevel.setString(level);
	m_isValid = true;
	a_button->isSelected(true);
}

void LoadMenu::loadLevel(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
{
	if (!m_loader.loadMap(a_rm, m_selectedLevel.getString().toAnsiString(), m_loadPath, m_tileMap, m_errorMsg))
	{
		m_loadError.setString(m_errorMsg);
		return;
	}

	a_states.push_back(std::make_unique<BreakoutState>(a_rm, a_window, m_tileMap, m_lives));
}