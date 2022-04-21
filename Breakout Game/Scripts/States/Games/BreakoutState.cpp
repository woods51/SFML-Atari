#include "BreakoutState.h"

// Regular Game
BreakoutState::BreakoutState(ResourceManager& a_rm, sf::RenderWindow& a_window, unsigned int a_lives)
{
	m_balls.push_back(std::make_unique<Ball>(a_rm));
	m_paddle = new Paddle(a_rm);
	m_livesRemaining = a_lives;
	m_isCustom = false;

	generateUI(a_rm);
	generateLevel1(a_rm);
}
// Custom Level
BreakoutState::BreakoutState(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<Tile*>& a_tileMap, unsigned int a_lives)
{
	m_balls.push_back(std::make_unique<Ball>(a_rm));
	m_paddle = new Paddle(a_rm);
	m_livesRemaining = a_lives;
	m_isCustom = true;

	generateUI(a_rm);
	generateLevel(a_rm, a_tileMap);
}
void BreakoutState::update(sf::Time a_dt, ResourceManager& a_rm)
{
	// Physics
	if (m_gameStarted && !m_gameOver)
	{
		handleBallPhysics(a_dt, a_rm);
		for (auto& ball : m_balls)
			if (ball->isActive())
				ball->move(a_rm, a_dt);

		bool flag = false;
		for (auto& ball : m_balls)
		{
			if (ball->isActive())
				flag = true;
		}
		if (!flag)
		{
			// Remove Lives
			m_livesRemaining -= 1;
			if (m_livesRemaining == 0)
			{
				m_gameOver = true;
				return;
			}
			resetBalls(a_rm);
			m_balls[0]->isActive(true);
		}
		m_paddle->move(a_dt);
	}
	
	if (m_completeFlag)
	{
		levelComplete(a_rm);
	}
	updateUI();
}
void BreakoutState::handleBallPhysics(sf::Time a_dt, ResourceManager& a_rm)
{
	// Handling Ball Physics
	Surface contact = Surface::None;

	m_completeFlag = true;

	// Collision with paddle
	sf::Vector2f paddlePos = m_paddle->getPosition();
	sf::Vector2f paddleDiagPos = m_paddle->getDiagonalPosition();

	for (unsigned int i = 0; i < m_balls.size(); i++)
	{
		if (!m_balls[i]->isActive())
			continue;

		bool flag = false;

		// Collision with a tile
		for (const auto& tile : m_tileMap)
		{
			sf::Vector2f tileDiagPos = tile->getDiagonalPosition();
			if (tile->isActive() && tile->isDestructable())
				m_completeFlag = false;

			if (!tile->isActive() || tileDiagPos.y < m_balls[i]->getPosition().y)
				continue;
			contact = m_balls[i]->collision(tile->getPosition(), tileDiagPos);
			if (contact != Surface::None && !m_balls[i]->isColliding())
			{
				m_balls[i]->handleTile(contact);

				if (tile->isDestructable())
				{
					tile->handleBall();
					switch (tile->getTileType())
					{
					case TileType::Special:
						m_balls.push_back(std::make_unique<Ball>(a_rm, m_balls[i]->getPosition()));
						m_balls[m_balls.size() - 1]->isActive(true);
						break;

					case TileType::Special2:
						for (int j = 0; j < 2; j++)
						{
							m_balls.push_back(std::make_unique<Ball>(a_rm, m_balls[i]->getPosition()));
							m_balls[m_balls.size() - 1]->isActive(true);
						}
						break;

					case TileType::Special3:
						for (int j = 0; j < 3; j++)
						{
							m_balls.push_back(std::make_unique<Ball>(a_rm, m_balls[i]->getPosition()));
							m_balls[m_balls.size() - 1]->isActive(true);
						}
						break;

					default:
						break;
					}

					m_score++;
				}
				flag = true;
				m_balls[i]->isColliding(true);
				a_rm.playSound(Sound::Ball);
			}
		}
		contact = m_balls[i]->collision(paddlePos, paddleDiagPos);
		if (contact != Surface::None)
		{
			flag = true;
			if (!m_paddle->isColliding() && !m_balls[i]->isColliding())
			{
				m_balls[i]->handlePaddle(contact, m_paddle->getDirection());
				m_balls[i]->isColliding(true);
				a_rm.playSound(Sound::Ball);
			}

		}
		else
			m_paddle->isColliding(false);

		if (!flag)
			m_balls[i]->isColliding(false);
	}
}
void BreakoutState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{
	if (a_key == sf::Keyboard::A || a_key == sf::Keyboard::Left)
		m_paddle->m_IsMovingLeft = a_isPressed;

	else if (a_key == sf::Keyboard::D || a_key == sf::Keyboard::Right)
		m_paddle->m_IsMovingRight = a_isPressed;

	else if (a_key == sf::Keyboard::Space && !m_gameStarted)
	{
		m_balls[0]->isActive(true);
		m_gameStarted = true;
	}
		
}
void BreakoutState::eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states)
{
	sf::Event event;
	static bool lock_click = false;
	sf::Vector2u winSize = a_window.getSize();

	// Initiate GameOver State
	if (m_gameOver)
	{
		m_frameTexture.create(winSize.x, winSize.y);
		m_frameTexture.update(a_window);
		a_states.push_back(std::make_unique<GameOver>(a_rm, a_window, m_frameTexture, m_score));
		return;
	}

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
							for (auto& ball : m_balls)
								ball->toggleColor(a_rm);
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
			// FOR TESTING LEVELS //
			if (event.key.code == sf::Keyboard::B)
			{
				levelComplete(a_rm);
			}
				
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
void BreakoutState::render(sf::RenderWindow& a_window)
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
	for (const auto& ball : m_balls)
		if (ball->isActive())
			a_window.draw(ball->getShape());
	a_window.draw(m_paddle->getShape());

	// Render UI
	a_window.draw(m_scoreText);
	a_window.draw(m_level);

	for (auto b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}
	a_window.draw(m_livesText);
	if (!m_gameStarted)
	{
		a_window.draw(m_startText);
		switch (m_currentLevel)
		{
		case 0:
			a_window.draw(m_startText);
			break;
		case 1:
			a_window.draw(m_secondLevelText);
			break;
		case 2:
			a_window.draw(m_thirdLevelText);
			break;
		case 3:
			a_window.draw(m_fourthLevelText);
			break;
		default:
			break;
		}
	}

	a_window.display();
}
void BreakoutState::updateUI()
{
	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_level.setString("Level " + std::to_string(m_currentLevel + 1));
	m_livesText.setString("Lives: " + std::to_string(m_livesRemaining));
}
void BreakoutState::generateUI(ResourceManager& a_rm)
{
	m_defaultFont = *a_rm.getFont("default");

	// generate all text UI -> textUI
	setDefaultText(a_rm, m_scoreText, 35, sf::Vector2f(16, 670));

	setDefaultText(a_rm, m_livesText, 25, sf::Vector2f(WIDTH - 284, 675));

	setDefaultText(a_rm, m_startText, 30, sf::Vector2f(WIDTH / 2 - 250, 500));
	m_startText.setString("Press space to start.");

	setDefaultText(a_rm, m_secondLevelText, 30, sf::Vector2f(WIDTH / 2 - 344, 450));
	m_secondLevelText.setString("Wall tiles cannot be broken!");

	setDefaultText(a_rm, m_thirdLevelText, 30, sf::Vector2f(WIDTH / 2 - 370, 450));
	m_thirdLevelText.setString("Special tiles spawn extra balls!");

	setDefaultText(a_rm, m_fourthLevelText, 25, sf::Vector2f(WIDTH / 2 - 330, 450));
	m_fourthLevelText.setString("Locked tiles have more durability!");

	setDefaultText(a_rm, m_level, 35, sf::Vector2f(WIDTH / 2 - 90, HEIGHT - 50));
	m_level.setString("Level 1");

	// generate all sprite UI / text -> spriteUI
	m_border.setTexture(*a_rm.getTexture("border"));
	m_border.setScale(sf::Vector2f((WIDTH / 32), (HEIGHT / 24)));

	// Buttons
	Button* temp;
	temp = new Button(a_rm, sf::Vector2f(WIDTH - 110, HEIGHT - 55), Press::BALLCOLOR, sf::Vector2f(3, 3),
		sf::Vector2f(16, 16), "button_ball", "button_ball_selected");
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(WIDTH - 55, HEIGHT - 55), Press::PAUSE, sf::Vector2f(3, 3),
		sf::Vector2f(16, 16), "button_pause", "button_pause_selected");
	m_buttons.push_back(temp);
}
void BreakoutState::resetBalls(ResourceManager& a_rm)
{
	// deletes unique pointers
	m_balls.clear();

	m_balls.push_back(std::make_unique<Ball>(a_rm));
}
void BreakoutState::levelComplete(ResourceManager& a_rm)
{
	a_rm.playSound(Sound::LevelComplete);
	m_gameStarted = false;
	m_completeFlag = false;
	
	resetBalls(a_rm);

	m_balls[0]->isActive(false);
	m_paddle->reset();

	if (m_isCustom)
	{
		resetLevel();
		return;
	}
	m_currentLevel++;

	switch (m_currentLevel)
	{
	case 0:
		generateLevel1(a_rm);
		break;
	case 1:
		generateLevel2(a_rm);
		break;
	case 2:
		generateLevel3(a_rm);
		break;
	case 3:
		generateLevel4(a_rm);
		break;
	default:
		generateLevel1(a_rm);
		break;
	}
}
void BreakoutState::resetLevel()
{
	for (auto& tile : m_tileMap)
	{
		tile->reset();
	}
}
void BreakoutState::generateLevel(ResourceManager& a_rm, std::vector<Tile*>& a_tileMap)
{
	m_tileMap.clear();
	std::string key;
	for (auto& tile : a_tileMap)
	{
		key = tile->getTextureKey();

		if (key.find("_lock2"))
		{
			key = key.substr(0, key.size() - 6);
		}
		else if (key.find("_lock"))
		{
			key = key.substr(0, key.size() - 5);
		}
		m_tileMap.push_back(std::make_unique<Tile>(a_rm, tile->getPosition(), tile->getTileType(), tile->getTextureKey()));
	}
}
void BreakoutState::generateLevel1(ResourceManager& a_rm)
{
	m_tileMap.clear();
	float posX = 0;
	float posY = 64;
	std::string keys[5] = { "tile_01", "tile_02", "tile_03", "tile_04", "tile_05" };
	for (auto const& key : keys)
	{
		for (int i = 0; i < 10; i++)
		{
			m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Default, key));
			posX += 128;
		}
		posX = 0;
		posY += 64;
	}
}
void BreakoutState::generateLevel2(ResourceManager& a_rm)
{
	m_tileMap.clear();
	float posX = 0;
	float posY = 64;
	std::string keys[5] = { "tile_01", "tile_02", "tile_03", "tile_04", "tile_05" };
	for (auto const& key : keys)
	{
		for (int i = 0; i < 10; i++)
		{
			if (key == "tile_03" && (i < 3 || i > 6))
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Wall, "tile_wall"));
			}
			else
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Default, key));
			}
				
			posX += 128;
		}
		posX = 0;
		posY += 64;
	}
}

void BreakoutState::generateLevel3(ResourceManager& a_rm)
{
	m_tileMap.clear();
	float posX = 0;
	float posY = 64;
	std::string keys[5] = { "tile_01", "tile_02", "tile_03", "tile_04", "tile_05" };
	for (auto const& key : keys)
	{
		for (int i = 0; i < 10; i++)
		{
			if ((key == "tile_02" && (i == 1 || i == 8)) || (key == "tile_04" && (i == 3 || i == 5)))
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Special));
			}
			else if (key == "tile_04" && (i == 0 || i == 9))
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Wall, "tile_wall"));
			}
			else
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Default, key));
			}

			posX += 128;
		}
		posX = 0;
		posY += 64;
	}
}
void BreakoutState::generateLevel4(ResourceManager& a_rm)
{
	m_tileMap.clear();
	float posX = 0;
	float posY = 64;
	int counter = 9;
	std::string keys[5] = { "tile_01", "tile_02", "tile_03", "tile_09", "tile_10" };
	for (auto const& key : keys)
	{
		for (int j = 0; j < 10; j++)
		{
			if ((key == "tile_02" && (j == 0 || j == 9)) || (key == "tile_09" && (j == 4 || j == 5)))
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Special));
			}
			else if (key == "tile_03" && (j == 0 || j == 9 || (j > 2 && j < 7)))
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Wall, "tile_wall"));
			}
			else if (key == "tile_09" && (j == 3 || j == 6))
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Lock2, key));
			}
			else if (key == "tile_10" && (j == 3 || j == 6))
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Lock, key));
			}
			else if (key == "tile_10" && (j == 4 || j == 5))
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Lock2, key));
			}
			else
			{
				m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Default, key));
			}
			posX += 128;
		}
		posX = 0;
		posY += 64;
		counter--;
	}
}
BreakoutState::~BreakoutState()
{
	m_balls.clear();

	delete m_paddle;

	for (auto& b : m_buttons)
		delete b;
}