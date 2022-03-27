#include "PlayState.h"

PlayState::PlayState(ResourceManager& rm, sf::RenderWindow& window) //: State(rm, window)
{
	this->ball = new Ball(rm, sf::Vector2f(300.0f, 350.0f));
	this->paddle = new Paddle(rm, sf::Vector2f(700.0f, 530.0f), sf::Vector2f(32.0f, 4.0f), sf::Vector2f(5.0f, 5.0f), "paddle_2");

	generateLevel1(rm);
	generateUI(rm);
}

PlayState::~PlayState()
{
	delete this->ball;
	delete this->paddle;

	for (auto b : buttons)
		delete b;
}

void PlayState::update(sf::Time dt, ResourceManager& rm)
{
	if (color_flag)
	{
		color_flag = false;
		ball->toggleColor(rm);
	}
	// Paddle Movment //
	paddle->move(dt);

	// Ball Physics :) //
	Surface contact = Surface::None;
	for (auto const& tile : tileMap)	// Collision with a tile
	{
		if (!tile->isActive() || tile->getDiagonalPos().y < ball->getPosition().y)
			continue;

		contact = ball->collision(tile->getPosition(), tile->getDiagonalPos());
		if (contact != Surface::None)
		{
			ball->handleTile(contact);
			this->score++;
			tile->setDeactive();
		}
	}

	// Ball collides with paddle
	// Paddle velocity > Ball escape velocity // Prevents multiple collisions in a frame
	if (paddle->hasCollided && paddle->getPosition().y > ball->getPosition().y + ball->getRadius())
	{
		paddle->hasCollided = false;
	}
	// Ball is near Paddle
	if (!paddle->hasCollided &&
		paddle->getPosition().y <= ball->getPosition().y + ball->getRadius() && paddle->getPosition().x <= ball->getPosition().x + ball->getRadius())
	{
		contact = ball->collision(paddle->getPosition(), paddle->getDiagonalPos());
		if (contact != Surface::None)
		{
			ball->handlePaddle(contact, paddle->getDirection());
			paddle->hasCollided = true;
		}
	}
	
	ball->update(dt);

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
			// Left Mouse Click
			if (event.mouseButton.button == sf::Mouse::Left && !lock_click)
			{
				sf::Vector2i pos = sf::Mouse::getPosition(window);
				printf("mpos: %d, %d\n", pos.x, pos.y);
				lock_click = true;
				
				for (auto b : buttons)
				{
					if (pos.y < b->sprite.getPosition().y)
						continue;
					
					sf::Vector2f b_pos = b->sprite.getPosition();
					sf::Vector2f b_diag_pos = b->getDiagonalPos();

					if ((pos.x >= b_pos.x && pos.y >= b_pos.y) &&
						(pos.x <= b_diag_pos.x && pos.y <= b_diag_pos.y))
					{
						switch (b->OnClick())
						{
						case B_DEFAULT:
							break;
						case B_BALLCOLOR:
							this->color_flag = true;
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
		if (!tile->isActive())
			continue;

		sf::RectangleShape temp = tile->getShape();
		window.draw(temp);
	}
	// Render Ball & Paddle
	window.draw(ball->getShape());
	window.draw(paddle->getShape());

	// Render UI
	window.draw(scoreText);

	for (auto b : buttons)
	{
		sf::Sprite temp;
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
	scoreText.setPosition(5, 562);

	// generate all sprite UI / text -> spriteUI
	border.setTexture(*rm.getTexture("border"));
	border.setScale(sf::Vector2f((WIDTH / 32), (HEIGHT / 24)));

	// Buttons
	Button* temp = new Button(rm, sf::Vector2f(WIDTH / 2, HEIGHT / 2), sf::Vector2f(5.0f, 5.0f), sf::Vector2f(16.0f, 8.0f), "test");
	temp->text.setFillColor(sf::Color::Blue);
	temp->text.setCharacterSize(25);
	buttons.push_back(temp);

	temp = new BallColor(rm, sf::Vector2f(700, 560));
	buttons.push_back(temp);
}