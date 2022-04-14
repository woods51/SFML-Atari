#include "OptionsState.h"

OptionsState::OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture)
{
	m_frameSprite.setTexture(a_frameTexture);

	generateUI(a_rm);
}
OptionsState::~OptionsState()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}
void OptionsState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{
	
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
						case Press::BALL_UP:
							a_rm.setVolume(SoundType::Ball, m_ballVolumeNum++);
							break;
						case Press::BALL_UP_X:
							a_rm.setVolume(SoundType::Ball, m_ballVolumeNum += 10);
							break;
						case Press::BALL_DOWN:
							a_rm.setVolume(SoundType::Ball, m_ballVolumeNum--);
							break;
						case Press::BALL_DOWN_X:
							a_rm.setVolume(SoundType::Ball, m_ballVolumeNum -= 10);
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
void OptionsState::update(sf::Time a_dt, ResourceManager& a_rm)
{
	fixVolume(m_buttonVolumeNum);
	m_buttonVolume.setString(std::to_string(m_buttonVolumeNum));
	adjustVolumeText(m_buttonVolumeNum, m_buttonVolume);

	fixVolume(m_ballVolumeNum);
	m_ballVolume.setString(std::to_string(m_ballVolumeNum));
	adjustVolumeText(m_ballVolumeNum, m_ballVolume);
	
}
void OptionsState::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	a_window.draw(m_frameSprite);
	a_window.draw(m_overlay);

	a_window.draw(m_buttonText);
	a_window.draw(m_buttonVolume);

	a_window.draw(m_ballText);
	a_window.draw(m_ballVolume);

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
	Button* temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6, (HEIGHT / 2) - 68), Press::BUTTON_UP, ">");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// >>
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 66, (HEIGHT / 2) - 68), Press::BUTTON_UP_X, ">>");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(12.0f, 4.0f));
	m_buttons.push_back(temp);
	// <
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54, (HEIGHT / 2) - 68), Press::BUTTON_DOWN, "<");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// <<
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 114, (HEIGHT / 2) - 68), Press::BUTTON_DOWN_X, "<<");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(9.0f, 4.0f));
	m_buttons.push_back(temp);

	//BACK
	temp = new Button(a_rm, sf::Vector2f(75, 100),
		Press::BACK, sf::Vector2f(4.0f, 6.0f), sf::Vector2f(32.0f, 8.0f), "button_menu", "button_menu_selected");
	temp->setString("BACK");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(16.0f, 10.0f));
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
	setDefaultText(a_rm, m_buttonText, 22, sf::Vector2f((WIDTH/2) - 55, 200));
	m_buttonText.setString("Button");
	
	// >
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6, (HEIGHT / 2) + 92), Press::BALL_UP, ">", SoundType::Ball);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// >>
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 66, (HEIGHT / 2) + 92), Press::BALL_UP_X, ">>", SoundType::Ball);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(12.0f, 4.0f));
	m_buttons.push_back(temp);
	// <
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54, (HEIGHT / 2) + 92), Press::BALL_DOWN, "<", SoundType::Ball);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// <<
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 114, (HEIGHT / 2) + 92), Press::BALL_DOWN_X, "<<", SoundType::Ball);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(9.0f, 4.0f));
	m_buttons.push_back(temp);

	//Ball Volume Number
	m_ballVolumeNum = a_rm.getVolume(SoundType::Ball);
	setDefaultText(a_rm, m_ballVolume, 40, sf::Vector2f((WIDTH / 2) - 50, (HEIGHT / 2) + 30));
	m_ballVolume.setString(std::to_string(m_ballVolumeNum));

	//Ball Text
	setDefaultText(a_rm, m_ballText, 22, sf::Vector2f((WIDTH / 2) - 35, (HEIGHT / 2)));
	m_ballText.setString("Ball");

	//Volume Text
	setDefaultText(a_rm, m_volumeText, 40, sf::Vector2f((WIDTH / 2) - 100, 100));
	m_volumeText.setString("Volume");
}
void OptionsState::fixVolume(int& a_volume)
{
	if (a_volume < 0)
		a_volume = 0;
	else if (a_volume > 100)
		a_volume = 100;
	return;
}
void OptionsState::adjustVolumeText(int& a_volume, sf::Text& a_text)
{
	float posY = a_text.getPosition().y;
	if (a_volume == 100)
		a_text.setPosition((WIDTH / 2) - 50, posY);
	else if (a_volume < 10)
		a_text.setPosition((WIDTH / 2) - 15, posY);
	else
		a_text.setPosition((WIDTH / 2) - 35, posY);
	return;
}