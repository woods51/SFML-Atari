#include "PongState.h"

PongState::PongState(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	m_ball = new PongBall(a_rm);
	m_ball->setScalar(1.25f);
	m_paddleP1 = new Paddle(a_rm, sf::Vector2f(100, HEIGHT/2 - 80), sf::Vector2f(4, 32), sf::Vector2f(5,5), "paddle_pong_02");
	m_paddleP2 = new Paddle(a_rm, sf::Vector2f(WIDTH - 120, HEIGHT/2 - 80), sf::Vector2f(4, 32), sf::Vector2f(5, 5), "paddle_pong_03");

	m_paddles.push_back(m_paddleP1);
	m_paddles.push_back(m_paddleP2);

	generateUI(a_rm);
}

void PongState::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{
	if (a_key == sf::Keyboard::W || a_key == sf::Keyboard::LShift)
		m_paddleP1->m_IsMovingUp = a_isPressed;

	else if (a_key == sf::Keyboard::S || a_key == sf::Keyboard::LControl)
		m_paddleP1->m_IsMovingDown = a_isPressed;

	if (a_key == sf::Keyboard::I || a_key == sf::Keyboard::RShift)
		m_paddleP2->m_IsMovingUp = a_isPressed;

	else if (a_key == sf::Keyboard::K || a_key == sf::Keyboard::RControl)
		m_paddleP2->m_IsMovingDown = a_isPressed;

	if (a_key == sf::Keyboard::Space)
		m_ball->isActive(true);
}

void PongState::eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states)
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
void PongState::update(sf::Time a_dt, ResourceManager& a_rm)
{
	// Check color flag
	if (m_colorFlag)
	{
		m_colorFlag = false;
		m_ball->toggleColor(a_rm);
	}

	
	handleBallPhysics(a_dt, a_rm);
	m_paddleP1->move(a_dt);
	m_paddleP2->move(a_dt);
	m_ball->move(a_rm, a_dt);

	m_pScores = m_ball->getScores();
	updateUI();
}

void PongState::handleBallPhysics(sf::Time a_dt, ResourceManager& a_rm)
{
	// Ball Physics
	Surface contact = Surface::None;
	sf::Vector2f ballPos = m_ball->getPosition();
	sf::Vector2f ballDiagPos = m_ball->getDiagonalPosition();

	// Collision with paddles
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

void PongState::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);
	a_window.draw(m_border);

	// Render Ball & Paddle
	a_window.draw(m_ball->getShape());
	a_window.draw(m_paddleP1->getShape());
	a_window.draw(m_paddleP2->getShape());

	// Render UI
	a_window.draw(m_scoreP1Text);
	a_window.draw(m_scoreP2Text);
	a_window.draw(m_player1);
	a_window.draw(m_player2);

	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	if (!m_ball->isActive())
	{
		a_window.draw(m_startText);
	}

	a_window.display();
}

void PongState::updateUI()
{
	m_scoreP1Text.setString("Score: " + std::to_string(m_pScores.x));
	m_scoreP2Text.setString("Score: " + std::to_string(m_pScores.y));
}
void PongState::generateUI(ResourceManager& a_rm)
{
	// generate all text UI -> textUI
	m_scoreP1Text.setFont(*a_rm.getFont("default"));
	m_scoreP1Text.setFillColor(sf::Color::White);
	m_scoreP1Text.setCharacterSize(25);
	m_scoreP1Text.setPosition(5, 675);

	m_scoreP2Text.setFont(*a_rm.getFont("default"));
	m_scoreP2Text.setFillColor(sf::Color::White);
	m_scoreP2Text.setCharacterSize(25);
	m_scoreP2Text.setPosition(WIDTH-160, 675);

	m_startText.setFont(*a_rm.getFont("default"));
	m_startText.setFillColor(sf::Color::White);
	m_startText.setCharacterSize(30);
	m_startText.setString("Press space to start.");
	m_startText.setPosition(WIDTH / 2 - 250, 500);

	m_player1.setFont(*a_rm.getFont("default"));
	m_player1.setFillColor(sf::Color::Yellow);
	m_player1.setCharacterSize(12);
	m_player1.setString("Player 1");
	m_player1.setPosition(10, 662);
	m_player2 = m_player1;
	m_player2.setString("Player 2");
	m_player2.setPosition(1190, 662);

	// generate all sprite UI / text -> spriteUI
	m_border.setTexture(*a_rm.getTexture("border"));
	m_border.setScale(sf::Vector2f((WIDTH / 32), (HEIGHT / 24)));

	// Buttons
	Button* temp;

	temp = new Button(a_rm, sf::Vector2f(WIDTH/2 - 24, HEIGHT - 55),
		Press::PAUSE, sf::Vector2f(6.0f, 6.0f), sf::Vector2f(8.0f, 8.0f), "button_pause", "button_pause_selected");
	m_buttons.push_back(temp);
}

PongState::~PongState()
{
	delete m_ball;
	delete m_paddleP1;
	delete m_paddleP2;

	for (auto& b : m_buttons)
		delete b;
}