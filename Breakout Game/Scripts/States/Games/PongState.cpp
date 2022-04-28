#include "PongState.h"

PongState::PongState(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	// Generate Pong Paddles
	m_paddleP1 = new Paddle(a_rm, sf::Vector2f(100, HEIGHT/2 - 100), sf::Vector2f(4, 32), sf::Vector2f(5,5), "paddle_pong_02");
	m_paddleP2 = new Paddle(a_rm, sf::Vector2f(WIDTH - 120, HEIGHT / 2 - 100), sf::Vector2f(4, 32), sf::Vector2f(5, 5), "paddle_pong_03");
	m_paddleP1->setSpeed(20);
	m_paddleP1->setBorderOffset(80);
	m_paddleP2->setSpeed(20);
	m_paddleP2->setBorderOffset(80);
	m_paddles.push_back(m_paddleP1);
	m_paddles.push_back(m_paddleP2);

	m_ball = new PongBall(a_rm);
	m_ball->setScalar(1.25f);

	generateUI(a_rm);
}

void PongState::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
{
	bool lock_click = false;
	sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));

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

void PongState::update(ResourceManager& a_rm, const sf::Time& a_dt)
{
	// Update game objects
	handleBallPhysics(a_rm, a_dt);
	m_paddleP1->move(a_dt);
	m_paddleP2->move(a_dt);
	m_ball->move(a_rm, a_dt);

	updateUI();
}

void PongState::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	// Render background
	if (m_backgroundFlag)
	{
		a_window.draw(m_background);
		a_window.draw(m_background2);
	}
	a_window.draw(m_border);
	
	// Render game objects
	a_window.draw(m_ball->getShape());
	a_window.draw(m_paddleP1->getShape());
	a_window.draw(m_paddleP2->getShape());

	// Render text objects
	a_window.draw(m_scoreP1Text);
	a_window.draw(m_scoreP2Text);
	a_window.draw(m_player1);
	a_window.draw(m_player2);

	if (!m_ball->isActive())
		a_window.draw(m_startText);

	// Render buttons
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.display();
}

PongState::~PongState()
{
	delete m_ball;
	delete m_paddleP1;
	delete m_paddleP2;

	for (auto& b : m_buttons)
		delete b;
}

void PongState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{
	// Register paddle movement
	if (a_key == sf::Keyboard::W || a_key == sf::Keyboard::LShift)
		m_paddleP1->m_IsMovingUp = a_isPressed;

	else if (a_key == sf::Keyboard::S || a_key == sf::Keyboard::LControl)
		m_paddleP1->m_IsMovingDown = a_isPressed;

	if (a_key == sf::Keyboard::I || a_key == sf::Keyboard::RShift)
		m_paddleP2->m_IsMovingUp = a_isPressed;

	else if (a_key == sf::Keyboard::K || a_key == sf::Keyboard::RControl)
		m_paddleP2->m_IsMovingDown = a_isPressed;

	// Start game
	if (a_key == sf::Keyboard::Space)
		m_ball->isActive(true);
}

void PongState::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
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
				m_ball->toggleColor(a_rm);
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

void PongState::updateUI()
{
	// Update scores
	m_pScores = m_ball->getScores();
	m_scoreP1Text.setString("Score: " + std::to_string(m_pScores.x));
	m_scoreP2Text.setString("Score: " + std::to_string(m_pScores.y));

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

void PongState::generateUI(ResourceManager& a_rm)
{
	// Generate buttons
	Button* temp;

	temp = new Button(a_rm, sf::Vector2f(WIDTH/2 - 52, HEIGHT - 64),
		Press::PAUSE, sf::Vector2f(6.0f, 6.0f), sf::Vector2f(8.0f, 8.0f), "button_pause", "button_pause_selected");
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(WIDTH/2 + 7, HEIGHT - 64), Press::BACKGROUND, sf::Vector2f(3, 3),
		sf::Vector2f(16, 16), "button_background", "button_background");
	m_buttons.push_back(temp);

	// Generate text objects
	generateText(a_rm);

	// Generate backgrounds

	m_border.setTexture(*a_rm.getTexture("border_pong"));
	m_border.setScale(sf::Vector2f(40, 40));

	m_backgroundTexture = *a_rm.getTexture("background_game");
	m_background.setTexture(m_backgroundTexture);
	m_background.setScale(40, 40);
	m_background2 = m_background;
	m_background2.setPosition(m_background.getPosition().x - 3960, 0);
}

void PongState::generateText(ResourceManager& a_rm)
{
	setDefaultText(a_rm, m_scoreP1Text, 25, sf::Vector2f(5, 675));

	setDefaultText(a_rm, m_scoreP2Text, 25, sf::Vector2f(WIDTH - 160, 675));

	setDefaultText(a_rm, m_startText, 30, sf::Vector2f(WIDTH / 2 - 250, 500));
	m_startText.setString("Press space to start.");

	setDefaultText(a_rm, m_player1, 12, sf::Vector2f(10, 656), "default", sf::Color::Yellow);
	m_player1.setString("Player 1");

	setDefaultText(a_rm, m_player2, 12, sf::Vector2f(1190, 656), "default", sf::Color::Yellow);
	m_player2.setString("Player 2");
}

void PongState::handleBallPhysics(ResourceManager& a_rm, const sf::Time& a_dt)
{
	sf::Vector2f ballPos = m_ball->getPosition();
	sf::Vector2f ballDiagPos = m_ball->getDiagonalPosition();

	// Check for collision with paddles
	Surface contact = Surface::None;
	for (const auto& paddle : m_paddles)
	{
		sf::Vector2f paddlePos = paddle->getPosition();
		sf::Vector2f paddleDiagPos = paddle->getDiagonalPosition();

		contact = m_ball->collision(paddlePos, paddleDiagPos);
		if (contact != Surface::None)
		{
			if (!paddle->isColliding())
			{
				m_ball->handlePaddle(contact, paddle->getDirection());
				paddle->isColliding(true);
				a_rm.playSound(Sound::Ball);
			}
		}
		else
			paddle->isColliding(false);
	}
}