#include "PauseState.h"

PauseState::PauseState(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	sf::Vector2u winSize = a_window.getSize();
	m_frameTexture.create(winSize.x, winSize.y);
	m_frameTexture.update(a_window);
	m_frameSprite.setTexture(m_frameTexture);

	generateUI(a_rm);
}
PauseState::~PauseState()
{
	for (const auto& b : m_buttons)
	{
		delete b;
	}
}
void PauseState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{
	if (!a_isPressed)
	{
		m_escapeLock = false;
		return;
	}
		
	if (a_key == sf::Keyboard::Escape && !m_escapeLock)
		m_resumeFlag = true;
}
void PauseState::eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states)
{
	sf::Event event;
	static bool lock_click = false;
	while (a_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			// Left Mouse Click
			if (event.mouseButton.button == sf::Mouse::Left && !lock_click)
			{
				sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
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
						case Press::RESUME:
							a_states.pop_back();
							m_resumeFlag = false;
							break;
						case Press::OPTIONS:
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
	if (m_resumeFlag)
	{
		a_states.pop_back();
	}
}
void PauseState::update(sf::Time a_dt, ResourceManager& a_rm)
{

}
void PauseState::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);
	
	a_window.draw(m_frameSprite);
	a_window.draw(m_overlay);

	// Render UI
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.display();
}
void PauseState::generateUI(ResourceManager& a_rm)
{
	// generate all buttons
	Button* temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) - 64),
		Press::RESUME, sf::Vector2f(8.0f, 8.0f), sf::Vector2f(32.0f, 8.0f), "Resume", "menu_button");
	temp->m_text.setFillColor(sf::Color::White);
	temp->m_text.setCharacterSize(40);
	temp->m_text.setPosition(temp->getShape().getPosition() + sf::Vector2f(29.0f, 8.0f));
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - 128, (HEIGHT / 2) + 128),
		Press::OPTIONS, sf::Vector2f(8.0f, 8.0f), sf::Vector2f(32.0f, 8.0f), "Options", "menu_button");
	temp->m_text.setFillColor(sf::Color::White);
	temp->m_text.setCharacterSize(40);
	temp->m_text.setPosition(temp->getShape().getPosition() + sf::Vector2f(20.0f, 8.0f));
	m_buttons.push_back(temp);

	//UI
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));
}