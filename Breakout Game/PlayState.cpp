#include "PlayState.h"

PlayState::PlayState(ResourceManager* rm, sf::RenderWindow* window) : State(rm, window)
{
	this->ball = new Ball(m_rm, sf::Vector2f(300.0f, 250.0f), "ball", sf::Vector2f(16.0f, 16.0f), sf::Vector2f(1.0f, 1.0f));
	this->paddle = new Paddle(m_rm, sf::Vector2f(700.0f, 550.0f), "paddle_2", sf::Vector2f(32.0f, 4.0f), sf::Vector2f(5.0f, 5.0f));

	float posy = 0;
	for (auto i : { "1", "2", "1" })
	{
		generateTileRow(posy, i);
		posy += 40.0f;
	}
}

PlayState::~PlayState()
{
	delete this->ball;
	delete this->paddle;
}

void PlayState::update(sf::Time dt)
{
	// Paddle Movment //
	paddle->move(dt);

	// Ball Physics :) //
	Surface contact = Surface::None;
	for (auto const& tile : tileMap)	// Collision with a tile
	{
		if (tile->isActive == false || tile->getDiagonalPos().y < ball->sprite.getPosition().y)
			;			continue;

		contact = tile->collision(ball->sprite.getPosition(), ball->getDiagonalPos());
		if (contact != Surface::None)
		{
			ball->handleTile(contact, dt);
			tile->isActive = false;
		}
	}
	// Ball collides with paddle
	contact = paddle->collision(ball->sprite.getPosition(), ball->getDiagonalPos());
	if (contact != Surface::None)
	{
		ball->handlePaddle(contact, paddle->currentDir, dt);
	}

	// Window Border collision detection and handling w/ Ball
	float offset = ball->scale.x * ball->size.x;
	if (ball->sprite.getPosition().x <= 0)
	{
		ball->sprite.setPosition(0, ball->sprite.getPosition().y);
		ball->velocity.x = -ball->velocity.x;
	}
	if (ball->getDiagonalPos().x >= WIDTH)
	{
		ball->sprite.setPosition(WIDTH - offset, ball->sprite.getPosition().y);
		ball->velocity.x = -ball->velocity.x;
	}
	if (ball->sprite.getPosition().y <= 0)
	{
		ball->sprite.setPosition(ball->sprite.getPosition().x, 0);
		ball->velocity.y = -ball->velocity.y;
	}
	if (ball->getDiagonalPos().y >= HEIGHT)
	{
		ball->sprite.setPosition(ball->defaultPos);
		ball->velocity = ball->defaultVel;
	}
	ball->sprite.move(ball->velocity.x * dt.asMilliseconds(), ball->velocity.y * dt.asMilliseconds());
	system("CLS");
	std::cout << "v:" << ball->velocity.x << " dt:" << dt.asMilliseconds() << std::endl;

	if (ball->velocity.x < 0)
		ball->currentDir = Direction::Left;
	else
		ball->currentDir = Direction::Right;
}
void PlayState::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
		paddle->m_IsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
		paddle->m_IsMovingRight = isPressed;
}
void PlayState::eventHandler()
{
	sf::Event event;
	while (m_window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			inputHandler(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			inputHandler(event.key.code, false);
			break;
		case sf::Event::Closed:
			m_window->close();
			break;
		}
	}
}
void PlayState::render()
{
	m_window->clear(sf::Color::Black);

	for (const auto& tile : tileMap)
	{
		if (!tile->isActive)
			continue;

		sf::Sprite temp = tile->sprite;
		m_window->draw(temp);
	}
	sf::Sprite temp = ball->sprite;
	m_window->draw(temp);
	temp = paddle->sprite;

	m_window->draw(temp);

	m_window->display();
}