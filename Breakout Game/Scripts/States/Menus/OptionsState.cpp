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
OptionsState::~OptionsState()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}

	delete m_background;
	delete m_background2;
}
void OptionsState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{
	
}
void OptionsState::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
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
			b->isSelected(true);
		}
		else
			b->isSelected(false);
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
void OptionsState::update(ResourceManager& a_rm , sf::Time a_dt)
{
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

	a_window.draw(*m_background);
	if (m_background2 != nullptr)
		a_window.draw(*m_background2);
	a_window.draw(m_overlay);

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
void OptionsState::generateUI(ResourceManager& a_rm)
{
	// generate all buttons
	Button* temp;

	//BACK
	temp = new Button(a_rm, sf::Vector2f(75, 100),
		Press::BACK, sf::Vector2f(4, 6), sf::Vector2f(32, 8), "button_menu", "button_menu_selected");
	temp->setString("BACK");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 10));
	m_buttons.push_back(temp);

	// BUTTON
	// >
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6 - 100, (HEIGHT / 2) - 68), Press::BUTTON_UP, ">");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);

	// <
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54 - 100, (HEIGHT / 2) - 68), Press::BUTTON_DOWN, "<");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);

	// BALL
	// >
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6 - 100, (HEIGHT / 2) + 92), Press::BALL_UP, ">", Sound::Ball);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// <
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54 - 100, (HEIGHT / 2) + 92), Press::BALL_DOWN, "<", Sound::Ball);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// Game
	// >
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6 + 100, (HEIGHT / 2) - 68), Press::GAME_UP, ">", Sound::GameOver);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);

	// <
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54 + 100, (HEIGHT / 2) - 68), Press::GAME_DOWN, "<", Sound::GameOver);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 4));
	m_buttons.push_back(temp);

	// Level
	// >
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6 + 100, (HEIGHT / 2) + 92), Press::LEVEL_UP, ">", Sound::LevelComplete);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// <
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54 + 100, (HEIGHT / 2) + 92), Press::LEVEL_DOWN, "<", Sound::LevelComplete);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	//UI
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));

	//Text
	//Button Volume Number
	setDefaultText(a_rm, m_buttonVolume, 40, sf::Vector2f((WIDTH / 2) - 35 - 100, (HEIGHT / 2) - 130));

	//Button Text
	setDefaultText(a_rm, m_buttonText, 22, sf::Vector2f((WIDTH/2) - 55 - 100, 200));
	m_buttonText.setString("Button");
	
	//Ball Volume Number
	setDefaultText(a_rm, m_ballVolume, 40, sf::Vector2f((WIDTH / 2) - 50 - 100, (HEIGHT / 2) + 30));

	//Ball Text
	setDefaultText(a_rm, m_ballText, 22, sf::Vector2f((WIDTH / 2) - 35 - 100, (HEIGHT / 2)));
	m_ballText.setString("Ball");

	//Game Volume Number
	setDefaultText(a_rm, m_gameVolume, 40, sf::Vector2f((WIDTH / 2) - 35 + 100, (HEIGHT / 2) - 130));

	//Game Text
	setDefaultText(a_rm, m_gameText, 22, sf::Vector2f((WIDTH / 2) - 37 + 100, 200));
	m_gameText.setString("Game");

	//Level Volume Number
	setDefaultText(a_rm, m_levelVolume, 40, sf::Vector2f((WIDTH / 2) - 50 + 100, (HEIGHT / 2) + 30));

	//Level Text
	setDefaultText(a_rm, m_levelText, 22, sf::Vector2f((WIDTH / 2) - 44 + 100, (HEIGHT / 2)));
	m_levelText.setString("Level");

	//Volume Text
	setDefaultText(a_rm, m_volumeText, 40, sf::Vector2f((WIDTH / 2) - 100, 100));
	m_volumeText.setString("Volume");
}
void OptionsState::adjustVolumeText(int a_volume, sf::Text& a_text, bool a_left)
{
	int offset = (WIDTH / 2) - 100;
	if (!a_left)
		offset = (WIDTH / 2) + 100;

	float posY = a_text.getPosition().y;
	if (a_volume == 100)
		a_text.setPosition(offset - 50, posY);
	else if (a_volume < 10)
		a_text.setPosition(offset - 15, posY);
	else
		a_text.setPosition(offset - 35, posY);
	return;
}