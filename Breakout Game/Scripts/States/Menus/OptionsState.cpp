#include "OptionsState.h"

OptionsState::OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture)
{
	m_background = new sf::Sprite(a_frameTexture);

	generateUI(a_rm);
}
OptionsState::OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Sprite* a_background, sf::Sprite* a_background2)
{
	m_background = a_background;
	m_background2 = a_background2;
	m_background->setPosition(m_background->getPosition() + sf::Vector2f(0.25f, 0));
	m_background2->setPosition(m_background2->getPosition() + sf::Vector2f(0.25f, 0));

	generateUI(a_rm);
}

void OptionsState::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
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
void OptionsState::update(ResourceManager& a_rm , sf::Time a_dt)
{
	// Update volume values and text strings
	int volume = a_rm.getVolume(Sound::Button);
	m_buttonVolume.setString(std::to_string(volume));
	adjustVolumeText(volume, m_buttonVolume, true);

	volume = a_rm.getVolume(Sound::Ball);
	m_ballVolume.setString(std::to_string(volume));
	adjustVolumeText(volume, m_ballVolume, true);

	volume = a_rm.getVolume(Sound::GameOver);
	m_gameVolume.setString(std::to_string(volume));
	adjustVolumeText(volume, m_gameVolume, false);

	volume = a_rm.getVolume(Sound::LevelComplete);
	m_levelVolume.setString(std::to_string(volume));
	adjustVolumeText(volume, m_levelVolume, false);

	// Update backgrounds
	if (m_background2 != nullptr)
	{
		m_background->setPosition(m_background->getPosition() + sf::Vector2f(0.25f, 0));
		if (m_background->getPosition().x == 3200)
		{
			m_background->setPosition(sf::Vector2f(-3200, 0));
		}
		m_background2->setPosition(m_background2->getPosition() + sf::Vector2f(0.25f, 0));
		if (m_background2->getPosition().x == 3200)
		{
			m_background2->setPosition(sf::Vector2f(-3200, 0));
		}
	}
	
}
void OptionsState::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	// Render backgrounds
	a_window.draw(*m_background);
	if (m_background2 != nullptr)
		a_window.draw(*m_background2);
	else
		a_window.draw(m_overlay);

	// Render text
	a_window.draw(m_buttonText);
	a_window.draw(m_buttonVolume);

	a_window.draw(m_ballText);
	a_window.draw(m_ballVolume);

	a_window.draw(m_gameText);
	a_window.draw(m_gameVolume);

	a_window.draw(m_levelText);
	a_window.draw(m_levelVolume);

	a_window.draw(m_volumeText);

	// Render UI
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.display();
}
OptionsState::~OptionsState()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}

	if (m_background2 != nullptr)
		delete m_background;
	delete m_background2;
}
void OptionsState::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
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
			case Press::BACK:
				a_states.pop_back();
				break;
			case Press::BUTTON_UP:
				a_rm.setVolume(Sound::Button, true);
				break;
			case Press::BUTTON_DOWN:
				a_rm.setVolume(Sound::Button, false);
				break;
			case Press::BALL_UP:
				a_rm.setVolume(Sound::Ball, true);
				break;
			case Press::BALL_DOWN:
				a_rm.setVolume(Sound::Ball, false);
				break;
			case Press::GAME_UP:
				a_rm.setVolume(Sound::GameOver, true);
				break;
			case Press::GAME_DOWN:
				a_rm.setVolume(Sound::GameOver, false);
				break;
			case Press::LEVEL_UP:
				a_rm.setVolume(Sound::LevelComplete, true);
				break;
			case Press::LEVEL_DOWN:
				a_rm.setVolume(Sound::LevelComplete, false);
				break;
			default:
				break;
			}
		}
	}
}
void OptionsState::buttonSelectUpdate(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition)
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
void OptionsState::generateUI(ResourceManager& a_rm)
{
	// Generate buttons
	Button* temp;

	// BACK
	temp = new Button(a_rm, sf::Vector2f(75, 100),
		Press::BACK, sf::Vector2f(4, 6), sf::Vector2f(32, 8), "button_menu", "button_menu_selected");
	temp->setString("BACK");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 10));
	m_buttons.push_back(temp);

	// Generate tick buttons
	// BUTTON
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6 - 100, (HEIGHT / 2) - 68), Press::BUTTON_UP, ">");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);

	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54 - 100, (HEIGHT / 2) - 68), Press::BUTTON_DOWN, "<");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);

	// BALL
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6 - 100, (HEIGHT / 2) + 92), Press::BALL_UP, ">", Sound::Ball);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54 - 100, (HEIGHT / 2) + 92), Press::BALL_DOWN, "<", Sound::Ball);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// GAME
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6 + 100, (HEIGHT / 2) - 68), Press::GAME_UP, ">", Sound::GameOver);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);

	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54 + 100, (HEIGHT / 2) - 68), Press::GAME_DOWN, "<", Sound::GameOver);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);

	// LEVEL
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6 + 100, (HEIGHT / 2) + 92), Press::LEVEL_UP, ">", Sound::LevelComplete);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54 + 100, (HEIGHT / 2) + 92), Press::LEVEL_DOWN, "<", Sound::LevelComplete);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// Generate text
	setDefaultText(a_rm, m_buttonVolume, 40, sf::Vector2f((WIDTH / 2) - 35 - 100, (HEIGHT / 2) - 130));
	setDefaultText(a_rm, m_buttonText, 22, sf::Vector2f((WIDTH/2) - 55 - 100, 200));
	m_buttonText.setString("Button");
	
	setDefaultText(a_rm, m_ballVolume, 40, sf::Vector2f((WIDTH / 2) - 50 - 100, (HEIGHT / 2) + 30));
	setDefaultText(a_rm, m_ballText, 22, sf::Vector2f((WIDTH / 2) - 35 - 100, (HEIGHT / 2)));
	m_ballText.setString("Ball");

	setDefaultText(a_rm, m_gameVolume, 40, sf::Vector2f((WIDTH / 2) - 35 + 100, (HEIGHT / 2) - 130));
	setDefaultText(a_rm, m_gameText, 22, sf::Vector2f((WIDTH / 2) - 37 + 100, 200));
	m_gameText.setString("Game");

	setDefaultText(a_rm, m_levelVolume, 40, sf::Vector2f((WIDTH / 2) - 50 + 100, (HEIGHT / 2) + 30));
	setDefaultText(a_rm, m_levelText, 22, sf::Vector2f((WIDTH / 2) - 44 + 100, (HEIGHT / 2)));
	m_levelText.setString("Level");

	setDefaultText(a_rm, m_volumeText, 40, sf::Vector2f((WIDTH / 2) - 100, 100));
	m_volumeText.setString("Volume");

	// Generate Background
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));
}
void OptionsState::adjustVolumeText(int a_volume, sf::Text& a_text, bool a_isOnLeft)
{
	// Reposition text offset
	int offset = (WIDTH / 2) - 100;
	if (!a_isOnLeft)
		offset = (WIDTH / 2) + 100;

	float posY = a_text.getPosition().y;

	// Reposition by value
	if (a_volume == 100)
		a_text.setPosition(offset - 50, posY);
	else if (a_volume < 10)
		a_text.setPosition(offset - 15, posY);
	else
		a_text.setPosition(offset - 35, posY);
	return;
}