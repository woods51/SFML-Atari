#include "MenuState.h"

MenuState::MenuState(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	generateUI(a_rm);
}
MenuState::~MenuState()
{
	for (const auto& b : m_buttons)
	{
		delete b;
	}
}
void MenuState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{

}
void MenuState::eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states)
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
				m_pongFlag = false;
				break;
			case Press::PONG:
				m_breakoutFlag = false;
				m_pongFlag = true;
				break;
			default:
				m_breakoutFlag = m_pongFlag = false;
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
						case Press::PONG:
							a_states.push_back(std::make_unique<PongState>(a_rm, a_window));
							break;
						case Press::OPTIONS:
							a_states.push_back(std::make_unique<OptionsState>(a_rm, a_window, m_frameTexture));
							break;
						case Press::QUIT:
							a_window.close();
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
void MenuState::update(sf::Time a_dt, ResourceManager& a_rm)
{

}
void MenuState::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	// Render UI
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}
	if (m_breakoutFlag)
		a_window.draw(m_breakoutText);
	else if (m_pongFlag)
		a_window.draw(m_pongText);
	else
		a_window.draw(m_atariText);

	a_window.display();
}
void MenuState::generateUI(ResourceManager& a_rm)
{
	// generate all buttons
	Button* temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) - 100),
		Press::BREAKOUT, sf::Vector2f(8.0f, 8.0f), sf::Vector2f(32.0f, 8.0f), "Breakout");
	temp->setDefaultText(a_rm, 35, temp->getShape().getPosition() + sf::Vector2f(12.0f, 10.0f));
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) - 28),
		Press::PONG, sf::Vector2f(8.0f, 8.0f), sf::Vector2f(32.0f, 8.0f), "Pong", "button_menu");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(64.0f, 8.0f));
	m_buttons.push_back(temp);

	// Options
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 44),
		Press::OPTIONS, sf::Vector2f(8.0f, 8.0f), sf::Vector2f(32.0f, 8.0f), "Options");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(20.0f, 8.0f));
	m_buttons.push_back(temp);

	// Quit
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 116),
		Press::QUIT, sf::Vector2f(8.0f, 8.0f), sf::Vector2f(32.0f, 8.0f), "Quit");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(64.0f, 8.0f));
	m_buttons.push_back(temp);

	// generate text UI
	m_breakoutText.setPosition((WIDTH / 2) - (108 * 3), 130);
	m_breakoutText.setTexture(*a_rm.getTexture("breakout_title"));
	m_breakoutText.setScale(sf::Vector2f(6, 6));

	m_pongText.setPosition((WIDTH / 2) - (48*3), 130);
	m_pongText.setTexture(*a_rm.getTexture("pong_title"));
	m_pongText.setScale(sf::Vector2f(6, 6));

	m_atariText.setPosition((WIDTH / 2) - 295, 130);
	m_atariText.setTexture(*a_rm.getTexture("sfml_atari_title"));
	m_atariText.setScale(sf::Vector2f(5, 5));
}