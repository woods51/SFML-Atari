#include "LevelEditor.h"

LevelEditor::LevelEditor(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	m_childTile.setSize(sf::Vector2f(32, 24));
	m_childTile.setScale(sf::Vector2f(4, 2.5f));

	removeChild(a_rm);
	generateTileMap(a_rm);
	generateUI(a_rm);
}

void LevelEditor::update(sf::Time a_dt, ResourceManager& a_rm)
{

	updateUI(a_rm);
}
void LevelEditor::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{

}
void LevelEditor::eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states)
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
						case Press::PAUSE:
							m_frameTexture.create(winSize.x, winSize.y);
							m_frameTexture.update(a_window);
							a_states.push_back(std::make_unique<PauseState>(a_rm, a_window, m_frameTexture));
							break;

						case Press::TILE:
							m_child = new Tile(a_rm, sf::Vector2f(0, 0), b->getTileType(), b->getTextureKey(), true);
							break;

						case Press::PEN:
							if (b->isSelected())
							{
								b->setSelected(false);
								// Handle actions
								break;
							}
							b->setSelected(true);
							m_erase->setSelected(false);
							// Handle actions
							break;

						case Press::ERASE:
							if (b->isSelected())
							{
								b->setSelected(false);
								// Handle actions
								break;
							}
							b->setSelected(true);
							m_pen->setSelected(false);
							// Handle actions
							break;

						case Press::DESELECT:
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
			{
				lock_click = false;

				if (m_child == nullptr)
					break;

				sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
				for (auto& tile : m_tileMap)
				{
					sf::Vector2f tile_pos = tile->getPosition();
					sf::Vector2f tile_diag_pos = tile->getDiagonalPosition();

					if (mousePosition.x >= tile_pos.x && mousePosition.x <= tile_diag_pos.x &&
						mousePosition.y >= tile_pos.y && mousePosition.y <= tile_diag_pos.y)
					{
						tile->setTile(a_rm, m_child->getTileType(), m_child->getTextureKey(), true);
					}
				}
				if (m_child != NULL)
				{
					delete m_child;
					m_child = nullptr;
				}

					
			}
			
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
	//
	if (m_child != nullptr)
	{
		updateChild(a_window);
	}
	
}
void LevelEditor::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);
	a_window.draw(m_border);
	for (const auto& tile : m_tileMap)
	{
		if (!tile->isActive())
			continue;

		sf::RectangleShape temp = tile->getShape();
		a_window.draw(temp);
	}

	for (auto b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}
	if (m_child != nullptr)
		a_window.draw(m_child->getShape());
	a_window.draw(m_regularTile);
	a_window.draw(m_toolsText);

	//a_window.draw(m_selectedTile);
	a_window.draw(m_currentTile);
	//a_window.draw(m_seperator);

	a_window.display();
}
void LevelEditor::updateUI(ResourceManager& a_rm)
{
	
}
void LevelEditor::generateUI(ResourceManager& a_rm)
{
	// generate all text UI -> textUI
	setDefaultText(a_rm, m_toolsText, 22, sf::Vector2f(74, HEIGHT - 228));
	m_toolsText.setString("Tools");
	setDefaultText(a_rm, m_regularTile, 22, sf::Vector2f(240 + 20, HEIGHT - 228));
	m_regularTile.setString("Regular Tiles");

	// generate all sprite UI / text -> spriteUI
	m_border.setTexture(*a_rm.getTexture("test_04"));
	m_border.setScale(sf::Vector2f(40, 40));

	m_seperator.setTexture(*a_rm.getTexture("tile_seperator"));
	m_seperator.setScale(sf::Vector2f(20, 40));
	m_seperator.setPosition(sf::Vector2f(240, HEIGHT - 240));

	//m_selectedTile.setTexture(*a_rm.getTexture("tile_selected_1"));
	//m_selectedTile.setSize(sf::Vector2f(32, 24));
	//m_selectedTile.setScale(sf::Vector2f(8, 5));
	
	m_currentTile.setTexture(*a_rm.getTexture("tile_empty"));
	m_currentTile.setScale(sf::Vector2f(6, 6));
	m_currentTile.setPosition(sf::Vector2f(24, HEIGHT - 120));

	Button* temp;
	temp = new Button(a_rm, sf::Vector2f(24, HEIGHT - 192),
		Press::PEN, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 16.0f), "", "button_pen", "button_pen_selected");
	m_pen = temp;
	m_buttons.push_back(temp);
	temp = new Button(a_rm, sf::Vector2f(96, HEIGHT - 192),
		Press::ERASE, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 16.0f), "", "button_eraser", "button_eraser_selected");
	m_erase = temp;
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(168, HEIGHT - 192),
		Press::DESELECT, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 16.0f), "", "button_03", "button_03");
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(WIDTH / 2 - 24, HEIGHT - 240),
		Press::PAUSE, sf::Vector2f(6.0f, 6.0f), sf::Vector2f(8.0f, 8.0f), "", "pause_button", "pause_button_selected");
	m_buttons.push_back(temp);


	// Buttons
	
	float posX_offset = 256;
	float posY_offset = 64;
	int i = 1;
	for (const auto& key : { "tile_03", "tile_02", "tile_01",
		"tile_07", "tile_12", "tile_06",
		"tile_04", "tile_05", "tile_08",
		"tile_09", "tile_10", "tile_11" })
	{
		temp = new Button(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), TileType::Default, key);
		m_buttons.push_back(temp);
		posY_offset += 64;
		if (i % 3 == 0)
		{
			posX_offset += 128;
			posY_offset = 64;
		}
		i++;
	}
	i = 1;
	for (const auto& key : { "tile_09_lock", "tile_10_lock", "tile_11_lock",
							"tile_09_lock2", "tile_10_lock2", "tile_11_lock2" })
	{

		if (i > 3)
			temp = new Button(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), TileType::LOCK, key);
		else
			temp = new Button(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), TileType::LOCK2, key);

		m_buttons.push_back(temp);
		posY_offset += 64;
		if (i % 3 == 0)
		{
			posX_offset += 128;
			posY_offset = 64;
		}
		i++;
	}

}
void LevelEditor::generateTileMap(ResourceManager& a_rm)
{
	float posX = 0;
	float posY = 64.0f;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			m_tileMap.push_back(std::make_unique<Tile>(a_rm, sf::Vector2f(posX, posY), TileType::Blank));
			posX += 128.0f;
		}
		posX = 0;
		posY += 64.0f;
	}
}

LevelEditor::~LevelEditor()
{
	for (const auto& b : m_buttons)
	{
		delete b;
	}
}