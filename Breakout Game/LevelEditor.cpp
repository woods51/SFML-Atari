#include "LevelEditor.h"

LevelEditor::LevelEditor(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
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
	sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
	while (a_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			// Left Mouse Click
			if (event.mouseButton.button == sf::Mouse::Left && !lock_click)
			{
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
							delete m_selectedTile;
							m_selectedTile = new Tile(a_rm, sf::Vector2f(0, 0), b->getTileType(), b->getTextureKey(), true);

							m_selectorTile.setPosition(b->getPosition());
							m_previewTile.setTexture(*a_rm.getTexture(b->getTextureKey()));
							break;

						case Press::DESELECT:
							delete m_selectedTile;
							m_selectedTile = nullptr;
							m_selectorTile.setPosition(WIDTH, HEIGHT);
							m_previewTile.setTexture(*a_rm.getTexture("tile_selected"));
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
		if (lock_click && (m_pen->isSelected() || m_erase->isSelected()) && m_selectedTile != nullptr)
		{
			for (auto& tile : m_tileMap)
			{
				sf::Vector2f tile_pos = tile->getPosition();
				sf::Vector2f tile_diag_pos = tile->getDiagonalPosition();

				if (mousePosition.x >= tile_pos.x && mousePosition.x <= tile_diag_pos.x &&
					mousePosition.y >= tile_pos.y && mousePosition.y <= tile_diag_pos.y)
				{
					if (m_pen->isSelected())
						tile->setTile(a_rm, m_selectedTile->getTileType(), m_selectedTile->getTextureKey(), true);

					// Eraser is selected
					else
						tile->setTile(a_rm, TileType::Blank, "tile_08", true);
				}
			}
		}
	}
}
void LevelEditor::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);
	a_window.draw(m_border);

	// Draw Tile Map
	for (const auto& tile : m_tileMap)
	{
		if (!tile->isActive())
			continue;

		sf::RectangleShape temp = tile->getShape();
		a_window.draw(temp);
	}

	// Draw Buttons
	for (auto b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.draw(m_regularTile);
	a_window.draw(m_lockedTile);
	a_window.draw(m_specialTile);
	a_window.draw(m_wallTile);
	a_window.draw(m_toolsText);

	a_window.draw(m_selectorTile);
	a_window.draw(m_previewTile);

	a_window.display();
}
void LevelEditor::updateUI(ResourceManager& a_rm)
{
	
}
void LevelEditor::generateUI(ResourceManager& a_rm)
{
	// Generate Text UI
	setDefaultText(a_rm, m_toolsText, 22, sf::Vector2f(74, HEIGHT - 228));
	m_toolsText.setString("Tools");
	setDefaultText(a_rm, m_regularTile, 22, sf::Vector2f(334, HEIGHT - 228));
	m_regularTile.setString("Regular Tiles");
	setDefaultText(a_rm, m_lockedTile, 22, sf::Vector2f(660, HEIGHT - 228));
	m_lockedTile.setString("Locked Tiles");
	setDefaultText(a_rm, m_specialTile, 22, sf::Vector2f(975, HEIGHT - 108));
	m_specialTile.setString("Special Tiles");
	setDefaultText(a_rm, m_wallTile, 22, sf::Vector2f(1000, HEIGHT - 228));
	m_wallTile.setString("Wall Tiles");

	// Set Editor Border
	m_border.setTexture(*a_rm.getTexture("border_editor"));
	m_border.setScale(sf::Vector2f(40, 40));

	// Set Tile Selector
	m_selectorTile.setTexture(*a_rm.getTexture("tile_selected"));
	m_selectorTile.setScale(sf::Vector2f(4, 4));
	m_selectorTile.setPosition(WIDTH, HEIGHT);
	
	// Set Preview Tile
	m_previewTile.setTexture(*a_rm.getTexture("tile_selected"));
	m_previewTile.setScale(sf::Vector2f(6, 6));
	m_previewTile.setPosition(sf::Vector2f(24, HEIGHT - 120));

	// Generate Buttons (Pen, Erase, Deselect, Pause)
	generateButtons(a_rm);

	// Generate Tile Buttons
	generateTileButtons(a_rm);
}
void LevelEditor::generateButtons(ResourceManager& a_rm)
{
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
		Press::DESELECT, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 16.0f), "", "deselect_button", "deselect_button");
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(WIDTH / 2 - 20, 10),
		Press::PAUSE, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16, 16), "", "pause_button", "pause_button_selected");
	m_buttons.push_back(temp);
}
void LevelEditor::generateTileButtons(ResourceManager& a_rm)
{
	int i = 1;
	float posX_offset = 256;
	float posY_offset = 64;
	Button* temp;

	// Generate Default Tiles
	for (const auto& key : { "tile_03", "tile_02", "tile_01",
							"tile_04", "tile_05", "tile_06",
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

	// Generate Locked Tiles
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

	// Generate Special Tiles
	i = 1;
	posY_offset = 64;
	for (const auto& key : { "tile_special", "tile_special2", "tile_special3" })
	{
		TileType type = TileType::Special;
		if (i == 2)
			type = TileType::Special2;
		else if (i == 3)
			type = TileType::Special3;

		temp = new Button(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), type, key);
		m_buttons.push_back(temp);

		posX_offset += 128;
		i++;
	}

	// Generate Wall Tiles
	posX_offset = WIDTH - 384;
	posY_offset = 192;
	for (const auto& key : { "tile_wall", "tile_wall2", "tile_wall3" })
	{
		temp = new Button(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), TileType::Wall, key);
		m_buttons.push_back(temp);
		posX_offset += 128;
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
	delete m_selectedTile;
}