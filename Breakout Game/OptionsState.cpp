#include "OptionsState.h"

OptionsState::OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture)
{
	m_frameSprite.setTexture(a_frameTexture);

	generateUI(a_rm);
}
OptionsState::~OptionsState()
{
	for (const auto& b : m_buttons)
	{
		delete b;
	}
}
void OptionsState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{
	if (!a_isPressed)
	{
		m_escapeLock = false;
		return;
	}

	if (a_key == sf::Keyboard::Escape && !m_escapeLock)
		m_resumeFlag = true;
}
void OptionsState::eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states)
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
		}
		else
			b->setSelected(false);
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
						case Press::BACK:
							a_states.pop_back();
							m_resumeFlag = false;
							break;
						case Press::BUTTON_UP:
							a_rm.setVolume(SoundType::Button, m_buttonVolumeNum++);
							break;
						case Press::BUTTON_UP_X:
							a_rm.setVolume(SoundType::Button, m_buttonVolumeNum+=10);
							break;
						case Press::BUTTON_DOWN:
							a_rm.setVolume(SoundType::Button, m_buttonVolumeNum--);
							break;
						case Press::BUTTON_DOWN_X:
							a_rm.setVolume(SoundType::Button, m_buttonVolumeNum-=10);
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
void OptionsState::update(sf::Time a_dt, ResourceManager& a_rm)
{
	fixVolume(m_buttonVolumeNum);
	m_buttonVolume.setString(std::to_string(m_buttonVolumeNum));
}
void OptionsState::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	a_window.draw(m_frameSprite);
	a_window.draw(m_overlay);

	a_window.draw(m_buttonText);
	a_window.draw(m_buttonVolume);
	a_window.draw(m_volumeText);
	// Render UI
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.display();
}
void OptionsState::generateUI(ResourceManager& a_rm)
{
	// generate all buttons
	// >
	Button* temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) + 6, (HEIGHT / 2) - 68),
		Press::BUTTON_UP, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 12.0f), ">", "tick_button");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// >>
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) + 66, (HEIGHT / 2) - 68),
		Press::BUTTON_UP_X, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 12.0f), ">>", "tick_button");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(12.0f, 4.0f));
	m_buttons.push_back(temp);
	// <
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - 54, (HEIGHT / 2) - 68),
		Press::BUTTON_DOWN, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 12.0f), "<", "tick_button");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// <<
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - 114, (HEIGHT / 2) - 68),
		Press::BUTTON_DOWN_X, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 12.0f), "<<", "tick_button");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(9.0f, 4.0f));
	m_buttons.push_back(temp);

	//BACK
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - 114, (HEIGHT / 2) - 68),
		Press::BACK, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 12.0f), "<<", "tick_button");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(9.0f, 4.0f));
	m_buttons.push_back(temp);

	//UI
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));

	//Text
	//Button Volume Number
	m_buttonVolumeNum = a_rm.getVolume(SoundType::Button);
	setDefaultText(a_rm, m_buttonVolume, 40, sf::Vector2f((WIDTH / 2) - 35, (HEIGHT / 2) - 130));
	m_buttonVolume.setString(std::to_string(m_buttonVolumeNum));

	//Button Text
	setDefaultText(a_rm, m_buttonText, 22, sf::Vector2f((WIDTH/2) - 150, (HEIGHT/2) - 120));
	m_buttonText.setString("Button");

	//Volume Text
	setDefaultText(a_rm, m_volumeText, 22, sf::Vector2f((WIDTH / 2) - 250, (HEIGHT / 2) - 250));
	m_volumeText.setString("Volume");
}
