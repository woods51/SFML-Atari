#include "PlayState.h"
PlayState::PlayState(ResourceManager& rm, sf::RenderWindow& window)
{
	m_ball = new Ball(rm, sf::Vector2f(300.0f, 550.0f));
	m_paddle = new Paddle(rm, sf::Vector2f(300.0f, 640.0f));

	generateLevel1(rm);
	generateUI(rm);
}

void PlayState::update(sf::Time dt, ResourceManager& rm)
{
	if (m_color_flag)
	{
		m_color_flag = false;
		m_ball->toggleColor(rm);
	}
	// Paddle Movment
	m_paddle->move(dt);

	// Ball Physics
	Surface contact = Surface::None;
	//
	// Collision with a tile
	//
	for (auto const& tile : m_tileMap)
	{
		if (!tile->isActive() || tile->getDiagonalPosition().y < m_ball->getPosition().y)
			continue;

		contact = m_ball->collision(tile->getPosition(), tile->getDiagonalPosition());
		if (contact != Surface::None)
		{
			m_ball->handleTile(contact);
			m_score++;
			tile->setDeactive();
		}
	}
	//
	// Collision with paddle
	// 
	// Prevents compounded collisions with paddle
	if (m_paddle->hasCollided() && m_paddle->getPosition().y > m_ball->getDiagonalPosition().y)
	{
		m_paddle->collided(false);
	}
	// Ball is near Paddle
	if (!m_paddle->hasCollided() &&
		m_paddle->getPosition().x <= m_ball->getDiagonalPosition().x && m_paddle->getPosition().y <= m_ball->getDiagonalPosition().y)
	{
		contact = m_ball->collision(m_paddle->getPosition(), m_paddle->getDiagonalPosition());
		if (contact != Surface::None)
		{
			m_ball->handlePaddle(contact, m_paddle->getDirection());
			m_paddle->collided(true);
		}
	}
	
	m_ball->update(dt);

	updateUI();
}
void PlayState::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
		m_paddle->m_IsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
		m_paddle->m_IsMovingRight = isPressed;
}
void PlayState::eventHandler(sf::RenderWindow& window, sf::View& view)
{
	sf::Event event;
	static bool lock_click = false;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			// Left Mouse Click
			if (event.mouseButton.button == sf::Mouse::Left && !lock_click)
			{
				sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				lock_click = true;
				
				for (auto b : m_buttons)
				{
					if (mousePosition.y < b->getPosition().y || mousePosition.x < b->getPosition().x)
						continue;
					
					sf::Vector2f b_pos = b->getPosition();
					sf::Vector2f b_diag_pos = b->getDiagonalPosition();

					if (mousePosition.x >= b_pos.x - b->getGlobalBounds().width
						&& mousePosition.y >= b_pos.y - b->getGlobalBounds().height
						&& mousePosition.x <= b_pos.x + b->getGlobalBounds().width
						&& mousePosition.y <= b_pos.y + b->getGlobalBounds().height)
					{
						switch (b->OnClick())
						{
						case B_DEFAULT:
							break;
						case B_BALLCOLOR:
							m_color_flag = true;
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
			window.close();
			break;
		}
	}
}
void PlayState::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::Black);
	window.draw(m_border);
	// Render Tiles
	for (const auto& tile : m_tileMap)
	{
		if (!tile->isActive())
			continue;

		sf::RectangleShape temp = tile->getShape();
		window.draw(temp);
	}
	// Render Ball & Paddle
	window.draw(m_ball->getShape());
	window.draw(m_paddle->getShape());

	// Render UI
	window.draw(m_scoreText);

	for (auto b : m_buttons)
	{
		window.draw(b->getShape());
		window.draw(b->getText());
	}

	window.display();
}
void PlayState::updateUI()
{
	m_scoreText.setString("Score: " + std::to_string(m_score));

}
void PlayState::generateUI(ResourceManager& rm)
{
	// generate all text UI -> textUI
	m_scoreText.setFont(*rm.getFont("default"));
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setCharacterSize(35);
	m_scoreText.setPosition(5, 670);

	// generate all sprite UI / text -> spriteUI
	m_border.setTexture(*rm.getTexture("border"));
	m_border.setScale(sf::Vector2f((WIDTH / 32), (HEIGHT / 24)));

	// Buttons
	Button* temp = new Button(rm, sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(5.0f, 5.0f), sf::Vector2f(16.0f, 8.0f), "test");
	temp->setFillColor(sf::Color::Blue);
	temp->setCharacterSize(25);
	m_buttons.push_back(temp);

	temp = new BallColor(rm, sf::Vector2f(1100, 660));
	m_buttons.push_back(temp);
}

void PlayState::generateTileRow(ResourceManager& rm, float pos_y, std::string textureID, int iter = 20)
{
	float pos_x = 0;
	for (int i = 0; i < iter; i++)
	{
		m_tileMap.push_back(std::make_unique<Tile>(rm, sf::Vector2f(pos_x, pos_y), textureID));
		pos_x += 40.0f;
	}
}
void PlayState::generateLevel1(ResourceManager& rm)
{
	float pos_x = 0;
	float pos_y = 100.0f;
	std::string tiles[5] = { "tile_01", "tile_02", "tile_03", "tile_04", "tile_05" };
	for (auto const& texture : tiles)
	{
		for (int i = 0; i < 10; i++)
		{
			m_tileMap.push_back(std::make_unique<Tile>(rm, sf::Vector2f(pos_x, pos_y), texture,
				sf::Vector2f(32.0f, 24.0f), sf::Vector2f(4.0f, 2.5f)));
			pos_x += 128.0f;
		}
		pos_x = 0;
		pos_y += 60.0f;
	}
}

PlayState::~PlayState()
{
	delete m_ball;
	delete m_paddle;

	for (auto b : m_buttons)
		delete b;
}