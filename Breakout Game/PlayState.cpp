#include "PlayState.h"

PlayState::PlayState(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	m_ball = new Ball(a_rm);
	m_paddle = new Paddle(a_rm);

	generateLevel1(a_rm);
	generateUI(a_rm);
}

void PlayState::update(sf::Time a_dt, ResourceManager& a_rm)
{
	// Check color flag
	if (m_colorFlag)
	{
		m_colorFlag = false;
		m_ball->toggleColor(a_rm);
	}
	// Paddle Movment
	m_paddle->move(a_dt);

	// Ball Physics
	Surface contact = Surface::None;
	sf::Vector2f ballPos = m_ball->getPosition();
	sf::Vector2f ballDiagPos = m_ball->getDiagonalPosition();

	m_completeFlag = true;
	// Collision with a tile
	for (auto const& tile : m_tileMap)
	{
		sf::Vector2f tileDiagPos = tile->getDiagonalPosition();
		if (tile->isActive() && tile->isDestructable())
			m_completeFlag = false;

		if (!tile->isActive() || tileDiagPos.y < ballPos.y)
			continue;

		contact = m_ball->collision(tile->getPosition(), tileDiagPos);
		if (contact != Surface::None)
		{
			m_ball->handleTile(contact);
			if (tile->isDestructable())
			{
				tile->setDeactive();
				m_score++;
			}	
			a_rm.playSound(SoundType::Ball);
		}
	}

	// Collision with paddle
	sf::Vector2f paddlePos = m_paddle->getPosition();
	sf::Vector2f paddleDiagPos = m_paddle->getDiagonalPosition();

	// Prevents compounded collisions with paddle
	if (m_paddle->hasCollided() && paddlePos.y > ballDiagPos.y)
	{
		m_paddle->collided(false);
	}
	// Ball is near Paddle
	if (!m_paddle->hasCollided()
		&& paddlePos.x <= ballDiagPos.x
		&& paddlePos.y <= ballDiagPos.y)
	{
		contact = m_ball->collision(paddlePos, paddleDiagPos);
		if (contact != Surface::None)
		{
			m_ball->handlePaddle(contact, m_paddle->getDirection());
			m_paddle->collided(true);
			a_rm.playSound(SoundType::Ball);
		}
	}
	
	m_ball->move(a_rm, a_dt);

	if (m_completeFlag)
	{
		levelComplete(a_rm);
	}
	updateUI();
}
void PlayState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{
	if (a_key == sf::Keyboard::A || a_key == sf::Keyboard::Left)
		m_paddle->m_IsMovingLeft = a_isPressed;

	else if (a_key == sf::Keyboard::D || a_key == sf::Keyboard::Right)
		m_paddle->m_IsMovingRight = a_isPressed;

	else if (a_key == sf::Keyboard::Space)
		m_ball->setActive();
}
void PlayState::eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states)
{
	sf::Event event;
	static bool lock_click = false;
	sf::Vector2u winSize = a_window.getSize();
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
					sf::Vector2f b_pos = b->getPosition();
					sf::Vector2f b_diag_pos = b->getDiagonalPosition();

					if (mousePosition.x >= b_pos.x && mousePosition.x <= b_diag_pos.x &&
						mousePosition.y >= b_pos.y && mousePosition.y <= b_diag_pos.y)
					{
						switch (b->OnClick(a_rm))
						{
						case Press::DEFAULT:
							break;
						case Press::BALLCOLOR:
							m_colorFlag = true;
							break;
						case Press::PAUSE:
							m_frameTexture.create(winSize.x, winSize.y);
							m_frameTexture.update(a_window);
							a_states.push_back(std::make_unique<PauseState>(a_rm, a_window, m_frameTexture));
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
void PlayState::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);
	a_window.draw(m_border);
	// Render Tiles
	for (const auto& tile : m_tileMap)
	{
		if (!tile->isActive())
			continue;

		sf::RectangleShape temp = tile->getShape();
		a_window.draw(temp);
	}
	// Render Ball & Paddle
	a_window.draw(m_ball->getShape());
	a_window.draw(m_paddle->getShape());

	// Render UI
	a_window.draw(m_scoreText);

	for (auto b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	if (!m_ball->getActive())
		a_window.draw(m_startText);

	a_window.display();
}
void PlayState::updateUI()
{
	m_scoreText.setString("Score: " + std::to_string(m_score));

}
void PlayState::generateUI(ResourceManager& a_rm)
{
	// generate all text UI -> textUI
	m_scoreText.setFont(*a_rm.getFont("default"));
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setCharacterSize(35);
	m_scoreText.setPosition(5, 670);

	m_startText.setFont(*a_rm.getFont("default"));
	m_startText.setFillColor(sf::Color::White);
	m_startText.setCharacterSize(30);
	m_startText.setString("Press space to start.");
	m_startText.setPosition(WIDTH / 2 - 250, 500);

	// generate all sprite UI / text -> spriteUI
	m_border.setTexture(*a_rm.getTexture("border"));
	m_border.setScale(sf::Vector2f((WIDTH / 32), (HEIGHT / 24)));

	// Buttons
	Button* temp = new Button(a_rm, sf::Vector2f(1100, 660), Press::BALLCOLOR, sf::Vector2f(5.0f, 5.0f),
		sf::Vector2f(20.0f, 4.0f), "Ball Color", "empty_button", "empty_button");
	temp->setDefaultText(a_rm, 12, temp->getShape().getPosition() + sf::Vector2f(0, 2.0f));
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(WIDTH - 55, HEIGHT - 55),
		Press::PAUSE, sf::Vector2f(6.0f, 6.0f), sf::Vector2f(8.0f, 8.0f), "", "pause_button", "pause_button_selected");
	m_buttons.push_back(temp);
}
void PlayState::levelComplete(ResourceManager& a_rm)
{
	m_completeFlag = false;
	m_currentLevel++;

	m_ball->reset();
	m_paddle->reset();

	switch (m_currentLevel)
	{
	case 0:
		generateLevel1(a_rm);
		break;
	case 1:
		generateLevel2(a_rm);
		break;
	default:
		generateLevel1(a_rm);
		break;
	}
}
void PlayState::generateLevel1(ResourceManager& a_rm)
{
	m_tileMap.clear();
	float posX = 0;
	float posY = 100.0f;
	std::string keys[5] = { "tile_01", "tile_02", "tile_03", "tile_04", "tile_05" };
	for (auto const& textureKey : keys)
	{
		for (int i = 0; i < 10; i++)
		{
			m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), textureKey,
				sf::Vector2f(32.0f, 24.0f), sf::Vector2f(4.0f, 2.5f)));
			posX += 128.0f;
		}
		posX = 0;
		posY += 60.0f;
	}
}
void PlayState::generateLevel2(ResourceManager& a_rm)
{
	m_tileMap.clear();
	float posX = 0;
	float posY = 100.0f;
	std::string keys[5] = { "tile_01", "tile_02", "tile_03", "tile_04", "tile_05" };
	for (auto const& textureKey : keys)
	{
		for (int i = 0; i < 10; i++)
		{
			if (textureKey == "tile_03" && (i < 3 || i > 6))
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), m_wallTile,
					sf::Vector2f(32.0f, 24.0f), sf::Vector2f(4.0f, 2.5f), false));
			}
			else
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), textureKey,
					sf::Vector2f(32.0f, 24.0f), sf::Vector2f(4.0f, 2.5f)));
			posX += 128.0f;
		}
		posX = 0;
		posY += 60.0f;
	}
}
PlayState::~PlayState()
{
	delete m_ball;
	delete m_paddle;

	for (auto b : m_buttons)
		delete b;
}