#include "BreakoutState.h"

BreakoutState::BreakoutState(ResourceManager& a_rm, sf::RenderWindow& a_window, unsigned int a_lives)
{
	m_balls.push_back(std::make_unique<Ball>(a_rm));
	m_paddle = new Paddle(a_rm);
	m_livesRemaining = a_lives;
	m_isCustom = false;

	generateUI(a_rm);
	m_loader.loadFiles(m_loadPath, m_levels);
	loadLevel(a_rm, m_levels[m_currentLevel]);
}

BreakoutState::BreakoutState(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<Tile*>& a_tileMap, unsigned int a_lives)
{
	m_balls.push_back(std::make_unique<Ball>(a_rm));
	m_paddle = new Paddle(a_rm);
	m_livesRemaining = a_lives;
	m_isCustom = true;

	generateUI(a_rm);
	copyLevelMap(a_rm, a_tileMap);
}

void BreakoutState::update(ResourceManager& a_rm, const sf::Time& a_dt)
{
	if (m_gameStarted && !m_gameOver && !m_errorFlag)
	{
		handleBallPhysics(a_rm, a_dt);

		// Move balls
		for (auto& ball : m_balls)
			if (ball->isActive())
				ball->move(a_rm, a_dt);

		bool flag = false;
		for (auto& ball : m_balls)
		{
			if (ball->isActive())
				flag = true;
		}

		// Lost a life
		if (!flag)
		{
			m_livesRemaining -= 1;
			if (m_livesRemaining == 0)
			{
				m_gameOver = true;
				return;
			}
			resetBalls(a_rm);
			m_balls[0]->isActive(true);
		}

		// Move paddle
		m_paddle->move(a_dt);
	}
	
	if (m_completeFlag)
	{
		levelComplete(a_rm);
	}
	updateUI();
}

void BreakoutState::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
{
	if (m_errorFlag)
		return;

	sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
	sf::Vector2u windowSize = a_window.getSize();
	bool lock_click = false;

	// Push GameOver state
	if (m_gameOver)
	{
		m_frameTexture.create(windowSize.x, windowSize.y);
		m_frameTexture.update(a_window);
		a_states.push_back(std::make_unique<GameOver>(a_rm, a_window, m_frameTexture, m_score));
		return;
	}

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

		case sf::Event::KeyPressed:

			// Keybind for stepping through levels,
			// primarily used for testing purposes.
			if (event.key.code == sf::Keyboard::B)
			{
				levelComplete(a_rm);
			}
				
			inputHandler(event.key.code, true);
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

void BreakoutState::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	// Render background
	if (m_backgroundFlag)
	{
		a_window.draw(m_background);
		a_window.draw(m_background2);
	}
	a_window.draw(m_border);

	// Render tile map
	for (const auto& tile : m_tileMap)
	{
		if (!tile->isActive())
			continue;

		sf::RectangleShape temp = tile->getShape();
		a_window.draw(temp);
	}

	// Render game objects
	for (const auto& ball : m_balls)
		if (ball->isActive())
			a_window.draw(ball->getShape());
	a_window.draw(m_paddle->getShape());

	// Render text objects
	a_window.draw(m_scoreText);
	a_window.draw(m_livesText);
	a_window.draw(m_level);

	// Render buttons
	for (auto b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}
	
	// Render level text
	if (m_freePlay && !m_gameStarted)
	{
		a_window.draw(m_startText);
		a_window.draw(m_freePlayText);
	}
	else if (!m_gameStarted)
	{
		a_window.draw(m_startText);
		switch (m_currentLevel)
		{
		case 1:
			a_window.draw(m_wallLevelText);
			break;

		case 2:
			a_window.draw(m_specialLevelText);
			break;

		case 5:
			a_window.draw(m_lockedLevelText);
			break;

		default:
			break;
		}
	}

	a_window.display();
}

BreakoutState::~BreakoutState()
{
	m_balls.clear();

	delete m_paddle;

	for (auto& b : m_buttons)
		delete b;

	for (auto& tile : m_tileMap)
		delete tile;
}

void BreakoutState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{
	// Set paddle movement
	if (a_key == sf::Keyboard::A || a_key == sf::Keyboard::Left)
		m_paddle->m_IsMovingLeft = a_isPressed;

	else if (a_key == sf::Keyboard::D || a_key == sf::Keyboard::Right)
		m_paddle->m_IsMovingRight = a_isPressed;

	// Start level
	else if (a_key == sf::Keyboard::Space && !m_gameStarted)
	{
		m_balls[0]->isActive(true);
		m_gameStarted = true;
	}
}

void BreakoutState::updateUI()
{
	// Update general UI
	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_level.setString("Level " + std::to_string(m_currentLevel + 1));
	m_livesText.setString("Lives: " + std::to_string(m_livesRemaining));

	// Update backgrounds
	m_background.setPosition(m_background.getPosition() + sf::Vector2f(0.1f, 0));
	if (m_background.getPosition().x == 3960)
	{
		m_background.setPosition(sf::Vector2f(-3960, 0));
	}

	m_background2.setPosition(m_background2.getPosition() + sf::Vector2f(0.1f, 0));
	if (m_background2.getPosition().x == 3960)
	{
		m_background2.setPosition(sf::Vector2f(-3960, 0));
	}
}

void BreakoutState::generateUI(ResourceManager& a_rm)
{
	// Generate buttons
	Button* temp;

	temp = new Button(a_rm, sf::Vector2f(WIDTH - 110, HEIGHT - 55), Press::BALLCOLOR, sf::Vector2f(3, 3),
		sf::Vector2f(16, 16), "button_ball", "button_ball_selected");
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(WIDTH - 55, HEIGHT - 55), Press::PAUSE, sf::Vector2f(3, 3),
		sf::Vector2f(16, 16), "button_pause", "button_pause_selected");
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(WIDTH - 165, HEIGHT - 55), Press::BACKGROUND, sf::Vector2f(3, 3),
		sf::Vector2f(16, 16), "button_background", "button_background");
	m_buttons.push_back(temp);

	// Generate text objects
	generateText(a_rm);

	// Generate background
	m_border.setTexture(*a_rm.getTexture("border"));
	m_border.setScale(sf::Vector2f(40, 30));

	m_backgroundTexture = *a_rm.getTexture("background_game");

	m_background.setTexture(m_backgroundTexture);
	m_background.setScale(40, 40);
	m_background2 = m_background;
	m_background2.setPosition(m_background.getPosition().x - 3960, 0);
}

void BreakoutState::generateText(ResourceManager& a_rm)
{
	setDefaultText(a_rm, m_scoreText, 35, sf::Vector2f(16, 670));
	setDefaultText(a_rm, m_livesText, 25, sf::Vector2f(WIDTH - 340, 675));

	setDefaultText(a_rm, m_startText, 30, sf::Vector2f(WIDTH / 2 - 250, 500));
	m_startText.setString("Press space to start.");

	setDefaultText(a_rm, m_freePlayText, 30, sf::Vector2f(WIDTH / 2 - 262, 450));
	m_freePlayText.setString("Now in free play mode!");

	setDefaultText(a_rm, m_wallLevelText, 30, sf::Vector2f(WIDTH / 2 - 344, 450));
	m_wallLevelText.setString("Wall tiles cannot be broken!");

	setDefaultText(a_rm, m_specialLevelText, 30, sf::Vector2f(WIDTH / 2 - 370, 450));
	m_specialLevelText.setString("Special tiles spawn extra balls!");

	setDefaultText(a_rm, m_lockedLevelText, 25, sf::Vector2f(WIDTH / 2 - 324, 450));
	m_lockedLevelText.setString("Locked tiles have more durability!");

	setDefaultText(a_rm, m_level, 35, sf::Vector2f(WIDTH / 2 - 90, HEIGHT - 50));
	m_level.setString("Level 1");

	setDefaultText(a_rm, m_loadError, 20, sf::Vector2f(24, HEIGHT / 2 - 136), "default", sf::Color::Red);
	m_loadError.setString("");
}

void BreakoutState::handleBallPhysics(ResourceManager& a_rm, const sf::Time& a_dt)
{
	m_completeFlag = true;

	sf::Vector2f paddlePos = m_paddle->getPosition();
	sf::Vector2f paddleDiagPos = m_paddle->getDiagonalPosition();

	Surface contact = Surface::None;
	for (unsigned int i = 0; i < m_balls.size(); i++)
	{
		if (!m_balls[i]->isActive())
			continue;

		bool hasCollided = false;

		for (const auto& tile : m_tileMap)
		{
			sf::Vector2f tileDiagPos = tile->getDiagonalPosition();
			if (tile->isActive() && tile->isDestructable())
				m_completeFlag = false;

			if (!tile->isActive() || tileDiagPos.y < m_balls[i]->getPosition().y)
				continue;

			// Check for collision with tile
			contact = m_balls[i]->collision(tile->getPosition(), tileDiagPos);
			if (contact != Surface::None && !m_balls[i]->isColliding())
			{
				m_balls[i]->handleTile(contact);

				if (tile->isDestructable())
				{
					tile->handleBall();
					handleSpecialTiles(a_rm, tile->getTileType(), m_balls[i]->getPosition());

					m_score++;
				}
				hasCollided = true;
				m_balls[i]->isColliding(true);
				a_rm.playSound(Sound::Ball);
			}
		}
		// Check for collision with paddle
		contact = m_balls[i]->collision(paddlePos, paddleDiagPos);
		if (contact != Surface::None)
		{
			hasCollided = true;
			// Prevents multiple collisions
			if (!m_paddle->isColliding() && !m_balls[i]->isColliding())
			{
				m_balls[i]->handlePaddle(contact, m_paddle->getDirection());
				m_balls[i]->isColliding(true);
				a_rm.playSound(Sound::Ball);
			}
		}
		else
			m_paddle->isColliding(false);

		if (!hasCollided)
			m_balls[i]->isColliding(false);
	}
}

void BreakoutState::handleSpecialTiles(ResourceManager& a_rm, TileType a_type, sf::Vector2f a_pos)
{
	int balls = 0;
	switch (a_type)
	{
	case TileType::Special:
		balls = 1;
		break;

	case TileType::Special2:
		balls = 2;
		break;

	case TileType::Special3:
		balls = 3;
		break;

	default:
		break;
	}
	for (int i = 0; i < balls; i++)
	{
		m_balls.push_back(std::make_unique<Ball>(a_rm, a_pos));
		m_balls[m_balls.size() - 1]->isActive(true);
	}
}

void BreakoutState::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
	const sf::Vector2f& a_mousePosition)
{
	sf::Vector2u windowSize = a_window.getSize();

	for (auto b : m_buttons)
	{
		sf::Vector2f b_pos = b->getPosition();
		sf::Vector2f b_diag_pos = b->getDiagonalPosition();

		if (a_mousePosition.x >= b_pos.x && a_mousePosition.x <= b_diag_pos.x &&
			a_mousePosition.y >= b_pos.y && a_mousePosition.y <= b_diag_pos.y)
		{
			switch (b->OnClick(a_rm))
			{
			case Press::DEFAULT:
				break;

			case Press::BALLCOLOR:
				for (auto& ball : m_balls)
					ball->toggleColor(a_rm);
				break;

			case Press::PAUSE:
				m_frameTexture.create(windowSize.x, windowSize.y);
				m_frameTexture.update(a_window);
				a_states.push_back(std::make_unique<PauseMenu>(a_rm, a_window, m_frameTexture));
				break;

			case Press::BACKGROUND:
				m_backgroundFlag = !m_backgroundFlag;
				break;

			default:
				break;
			}
		}
	}
}

void BreakoutState::resetBalls(ResourceManager& a_rm)
{
	m_balls.clear();
	m_balls.push_back(std::make_unique<Ball>(a_rm));
}

void BreakoutState::levelComplete(ResourceManager& a_rm)
{
	a_rm.playSound(Sound::LevelComplete);
	m_gameStarted = false;
	m_completeFlag = false;
	
	// Reset game objects
	resetBalls(a_rm);
	m_balls[0]->isActive(false);
	m_paddle->reset();

	// Reload custom level
	if (m_isCustom)
	{
		resetLevel();
		return;
	}

	// Load next level
	m_currentLevel++;
	if (m_currentLevel == m_levels.size())
	{
		m_currentLevel = 0;
		m_freePlay = true;
	}

	loadLevel(a_rm, m_levels[m_currentLevel]);
}

void BreakoutState::resetLevel()
{
	for (auto& tile : m_tileMap)
	{
		tile->reset();
	}
}

void BreakoutState::copyLevelMap(ResourceManager& a_rm, std::vector<Tile*>& a_tileMap)
{
	m_tileMap.clear();
	for (auto& tile : a_tileMap)
	{
		m_tileMap.push_back(tile);
	}
}

void BreakoutState::loadLevel(ResourceManager& a_rm, std::string a_levelName)
{
	if (!m_loader.loadMap(a_rm, a_levelName, m_loadPath, m_tileMap, m_errorMsg))
	{
		m_errorFlag = true;
	}
	m_loadError.setString(m_errorMsg);
}