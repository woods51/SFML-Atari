#include "BreakoutMenu.h"
#include "LevelEditor.h"

BreakoutMenu::BreakoutMenu(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	generateUI(a_rm);
}
BreakoutMenu::~BreakoutMenu()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}
void BreakoutMenu::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{

}
void BreakoutMenu::eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states)
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
			b->setSelected(true);
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
		{
			b->setSelected(false);
		}

	}

	// Handle Events
	sf::Event event;
	sf::Vector2u winSize = a_window.getSize();
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
						case Press::BREAKOUT:
							a_states.push_back(std::make_unique<BreakoutState>(a_rm, a_window));
							break;
						case Press::EDITOR:
							a_states.push_back(std::make_unique<LevelEditor>(a_rm, a_window));
							break;
						case Press::LOAD:
							a_states.push_back(std::make_unique<LoadMenu>(a_rm, a_window, &m_background,&m_background2));
							break;
						case Press::BACK:
							a_states.pop_back();
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

		case sf::Event::KeyPressed:
			inputHandler(event.key.code, true);
			break;

		case sf::Event::Resized:
			break;

		case sf::Event::KeyReleased:
			inputHandler(event.key.code, false);
			break;

		case sf::Event::Closed:
			a_window.close();
			break;
		}
	}
}
void BreakoutMenu::update(sf::Time a_dt, ResourceManager& a_rm)
{
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
	a_window.draw(m_background);
	a_window.draw(m_background2);
	// Render UI
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.draw(m_breakoutText);

	a_window.display();
}
void BreakoutMenu::generateUI(ResourceManager& a_rm)
{
	// generate all buttons
	// Breakout
	Button* temp;
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) - 28), Press::BREAKOUT, "Play Game");
	temp->setDefaultText(a_rm, 30, temp->getShape().getPosition() + sf::Vector2f(13, 13));
	m_buttons.push_back(temp);

	// Editor
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) + 136, (HEIGHT / 2) - 28),
		Press::EDITOR, sf::Vector2f(4, 4), sf::Vector2f(16, 16), "button_editor", "button_editor_selected");
	m_buttons.push_back(temp);

	// Load
	temp = new MenuButton(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 44), Press::LOAD, "Load Level");
	temp->setDefaultText(a_rm, 28, temp->getShape().getPosition() + sf::Vector2f(14, 14));
	m_buttons.push_back(temp);

	// Back
	temp = new Button(a_rm, sf::Vector2f(75, 100),
		Press::BACK, sf::Vector2f(4, 6), sf::Vector2f(32, 8), "button_menu", "button_menu_selected");
	temp->setString("BACK");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 10));
	m_buttons.push_back(temp);

	// generate text UI
	m_breakoutText.setPosition((WIDTH / 2) - 324, 130);
	m_breakoutText.setTexture(*a_rm.getTexture("breakout_title"));
	m_breakoutText.setScale(sf::Vector2f(6, 6));

	// background
	m_frameTexture = *a_rm.getTexture("background_breakout");

	m_background.setTexture(m_frameTexture);
	m_background.setScale(40, 40);
	m_background2 = m_background;
	m_background2.setPosition(m_background.getPosition().x - 3200, 0);
}