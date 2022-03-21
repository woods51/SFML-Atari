#include "PlayState.h"

PlayState::PlayState(ResourceManager& rm, sf::RenderWindow& window) //: State(rm, window)
{
	this->ball = new Ball(rm, sf::Vector2f(300.0f, 250.0f), "ball", sf::Vector2f(16.0f, 16.0f), sf::Vector2f(1.0f, 1.0f));
	this->paddle = new Paddle(rm, sf::Vector2f(700.0f, 530.0f), "paddle_2", sf::Vector2f(32.0f, 4.0f), sf::Vector2f(5.0f, 5.0f));

	float posy = 0;
	for (auto i : { "1", "2", "1" })
	{
		generateTileRow(rm, posy, i);
		posy += 40.0f;
	}
	generateUI(rm);
}

PlayState::~PlayState()
{
	delete this->ball;
	delete this->paddle;

	for (auto b : buttons)
		delete b;
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
			continue;

		contact = tile->collision(ball->sprite.getPosition(), ball->getDiagonalPos());
		if (contact != Surface::None)
		{
			ball->handleTile(contact);
			this->score++;
			tile->isActive = false;
		}
	}

	// Ball collides with paddle
	contact = paddle->collision(ball->sprite.getPosition(), ball->getDiagonalPos());
	if (contact != Surface::None)
	{
		ball->handlePaddle(contact, paddle->currentDir);
	}
	
	ball->move(dt);

	//system("CLS");
	//std::cout << "v:" << ball->velocity.x << std::endl;
	//printf("v: (%.3f, %.3f)\n", ball->velocity.x, ball->velocity.y);
	//printf("dt: %.6f\n", (float)dt.asMilliseconds() * 1000);

	if (ball->velocity.x < 0)
		ball->currentDir = Direction::Left;
	else
		ball->currentDir = Direction::Right;

	updateUI();
}
void PlayState::inputHandler(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::A || key == sf::Keyboard::Left)
		paddle->m_IsMovingLeft = isPressed;
	else if (key == sf::Keyboard::D || key == sf::Keyboard::Right)
		paddle->m_IsMovingRight = isPressed;
}
void PlayState::eventHandler(sf::RenderWindow& window)
{
	sf::Event event;
	static bool lock_click = false;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left && !lock_click)
			{
				sf::Vector2i pos = sf::Mouse::getPosition(window);
				printf("mpos: %d, %d\n", pos.x, pos.y);
				lock_click = true;
				
				for (auto b : buttons)
				{
					//std::cout << "bpos: " << b->sprite.getPosition().x << " " << b->sprite.getPosition().y
					//	<< " ... " << b->getDiagonalPos().x << " " << b->getDiagonalPos().y << std::endl;
					if (pos.y < b->sprite.getPosition().y)
						continue;
					
					sf::Vector2f b_pos = b->sprite.getPosition();
					sf::Vector2f b_diag_pos = b->getDiagonalPos();

					if ((pos.x >= b_pos.x && pos.y >= b_pos.y) &&
						(pos.x <= b_diag_pos.x && pos.y <= b_diag_pos.y))
					{
						b->OnClick();
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
	window.draw(border);
	// Render Tiles
	for (const auto& tile : tileMap)
	{
		if (!tile->isActive)
			continue;

		sf::Sprite temp = tile->sprite;
		window.draw(temp);
	}
	// Render Ball & Paddle
	sf::Sprite temp = ball->sprite;
	window.draw(temp);
	temp = paddle->sprite;
	window.draw(temp);

	// Render UI
	window.draw(scoreText);

	for (auto b : buttons)
	{
		temp = b->sprite;
		window.draw(temp);
		window.draw(b->text);
	}

	window.display();
}
void PlayState::updateUI()
{
	scoreText.setString("Score: " + std::to_string(score));

}
void PlayState::generateUI(ResourceManager& rm)
{
	// generate all text UI -> textUI
	scoreText.setFont(*rm.getFont("default"));
	scoreText.setFillColor(sf::Color::White);
	scoreText.setCharacterSize(25);
	scoreText.setPosition(0, 562);
	// generate all sprite UI / text -> spriteUI
	border.setTexture(*rm.getTexture("border"));
	border.setScale(sf::Vector2f((WIDTH / 32), (HEIGHT / 24)));

	// Buttons
	Button* temp = new Button(rm);
	temp->text.setString("test");
	temp->text.setFillColor(sf::Color::Blue);
	temp->sprite.setPosition(sf::Vector2f(WIDTH / 2, HEIGHT / 2));
	temp->text.setPosition(temp->sprite.getPosition());

	buttons.push_back(temp);
}