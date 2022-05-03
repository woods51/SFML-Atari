#include "LevelEditor.h"

LevelEditor::LevelEditor(ResourceManager& a_rm, sf::RenderWindow& a_window)
{
	generateTileMap(a_rm);
	generateUI(a_rm);
}

void LevelEditor::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
{
	static bool lockClick = false;
	sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));

	sf::Event event;
	while (a_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left && !lockClick)
			{
				lockClick = true;

				handleButtonEvents(a_rm, a_window, a_states, mousePosition);
				handleTileButtonEvents(a_rm, mousePosition);
			}
			break;

		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left && lockClick)
			{
				lockClick = false;
			}
			break;

		case sf::Event::Closed:
			a_window.close();
			break;
		}

		handleTileSelection(a_rm, mousePosition, lockClick);
	}
}

void LevelEditor::update(ResourceManager& a_rm, const sf::Time& a_dt) {}

void LevelEditor::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	// Draw background
	a_window.draw(m_border);

	// Draw tile map
	for (const auto& tile : m_tileMap)
	{
		if (!tile->isActive())
			continue;

		sf::RectangleShape temp = tile->getShape();
		a_window.draw(temp);
	}

	// Draw buttons
	for (auto b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}
	for (auto b : m_tileButtons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	// Draw text
	a_window.draw(m_regularText);
	a_window.draw(m_lockedText);
	a_window.draw(m_specialText);
	a_window.draw(m_wallText);
	a_window.draw(m_toolsText);

	// Draw tile overlays
	a_window.draw(m_selectorTile);
	a_window.draw(m_previewTile);

	a_window.display();
}

LevelEditor::~LevelEditor()
{
	for (auto& t : m_tileMap)
	{
		delete t;
	}
	for (auto& b : m_buttons)
	{
		delete b;
	}
	for (auto& b : m_tileButtons)
	{
		delete b;
	}
	delete m_selectedTile;
}

void LevelEditor::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
	const sf::Vector2f& a_mousePosition)
{
	sf::Vector2u windowSize = a_window.getSize();

	for (auto& b : m_buttons)
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

			case Press::PAUSE:
				m_frameTexture.create(windowSize.x, windowSize.y);
				m_frameTexture.update(a_window);
				a_states.push_back(std::make_unique<PauseMenu>(a_rm, a_window, m_frameTexture));
				break;

			case Press::SAVE:
				m_frameTexture.create(windowSize.x, windowSize.y);
				m_frameTexture.update(a_window);
				a_states.push_back(std::make_unique<SaveMenu>(a_rm, a_window, m_frameTexture, m_tileMap));
				break;

			case Press::DESELECT:
				// Reset tile selections
				delete m_selectedTile;
				m_selectedTile = nullptr;
				m_selectorTile.setPosition(WIDTH, HEIGHT);
				m_previewTile.setTexture(*a_rm.getTexture("tile_selected"));
				break;

			case Press::PEN:
				if (b->isSelected())
				{
					b->isSelected(false);
					break;
				}
				b->isSelected(true);
				m_erase->isSelected(false);
				break;

			case Press::ERASE:
				if (b->isSelected())
				{
					b->isSelected(false);
					break;
				}
				b->isSelected(true);
				m_pen->isSelected(false);
				break;

			default:
				break;
			}
		}
	}
}

void LevelEditor::handleTileButtonEvents(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition)
{
	for (auto b : m_tileButtons)
	{
		sf::Vector2f b_pos = b->getPosition();
		sf::Vector2f b_diag_pos = b->getDiagonalPosition();

		if (a_mousePosition.x >= b_pos.x && a_mousePosition.x <= b_diag_pos.x &&
			a_mousePosition.y >= b_pos.y && a_mousePosition.y <= b_diag_pos.y)
		{
			switch (b->OnClick(a_rm))
			{
			case Press::TILE:
				delete m_selectedTile;
				m_selectedTile = new Tile(a_rm);
				m_selectedTile->updateTile(a_rm, b->getTileType(), b->getTextureKey());

				m_selectorTile.setPosition(b->getPosition());
				m_previewTile.setTexture(*a_rm.getTexture(b->getTextureKey()));
				break;

			default:
				break;
			}
		}
	}
}

void LevelEditor::handleTileSelection(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition, const bool& a_lockClick)
{
	if (a_lockClick && (m_erase->isSelected() || (m_pen->isSelected() && m_selectedTile != nullptr)))
	{
		// Draw on tile map
		for (auto& tile : m_tileMap)
		{
			sf::Vector2f tile_pos = tile->getPosition();
			sf::Vector2f tile_diag_pos = tile->getDiagonalPosition();

			if (a_mousePosition.x >= tile_pos.x && a_mousePosition.x <= tile_diag_pos.x &&
				a_mousePosition.y >= tile_pos.y && a_mousePosition.y <= tile_diag_pos.y)
			{
				// Pen is selected
				if (m_pen->isSelected())
					tile->updateTile(a_rm, m_selectedTile->getTileType(), m_selectedTile->getTextureKey());

				// Eraser is selected
				else
					tile->updateTile(a_rm, TileType::Blank, "tile_08");
			}
		}
	}
}

void LevelEditor::generateUI(ResourceManager& a_rm)
{
	// Generate border
	m_border.setTexture(*a_rm.getTexture("border_editor"));
	m_border.setScale(sf::Vector2f(40, 40));

	// Generate text objects
	setDefaultText(a_rm, m_toolsText, 22, sf::Vector2f(74, HEIGHT - 228));
	m_toolsText.setString("Tools");
	setDefaultText(a_rm, m_regularText, 22, sf::Vector2f(334, HEIGHT - 228));
	m_regularText.setString("Regular Tiles");
	setDefaultText(a_rm, m_lockedText, 22, sf::Vector2f(660, HEIGHT - 228));
	m_lockedText.setString("Locked Tiles");
	setDefaultText(a_rm, m_specialText, 22, sf::Vector2f(975, HEIGHT - 108));
	m_specialText.setString("Special Tiles");
	setDefaultText(a_rm, m_wallText, 22, sf::Vector2f(1000, HEIGHT - 228));
	m_wallText.setString("Wall Tiles");

	// Set tile selector
	m_selectorTile.setTexture(*a_rm.getTexture("tile_selected"));
	m_selectorTile.setScale(sf::Vector2f(4, 4));
	m_selectorTile.setPosition(WIDTH, HEIGHT);
	
	// Set preview tile
	m_previewTile.setTexture(*a_rm.getTexture("tile_selected"));
	m_previewTile.setScale(sf::Vector2f(6, 6));
	m_previewTile.setPosition(sf::Vector2f(24, HEIGHT - 120));

	// Generate buttons (Pen, Erase, Deselect, Pause)
	generateButtons(a_rm);

	// Generate tile buttons
	generateTileButtons(a_rm);
}

void LevelEditor::generateButtons(ResourceManager& a_rm)
{
	Button* temp;
	temp = new Button(a_rm, sf::Vector2f(24, HEIGHT - 192),
		Press::PEN, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 16.0f), "button_pen", "button_pen_selected");
	m_pen = temp;
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(96, HEIGHT - 192),
		Press::ERASE, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 16.0f), "button_eraser", "button_eraser_selected");
	m_erase = temp;
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(168, HEIGHT - 192),
		Press::DESELECT, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16.0f, 16.0f), "button_deselect", "button_deselect");
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(WIDTH / 2 - 56, 10),
		Press::PAUSE, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16, 16), "button_pause", "button_pause_selected");
	m_buttons.push_back(temp);

	temp = new Button(a_rm, sf::Vector2f(WIDTH / 2 + 8, 10),
		Press::SAVE, sf::Vector2f(3.0f, 3.0f), sf::Vector2f(16, 16), "button_save", "button_save_selected");
	m_buttons.push_back(temp);
}

void LevelEditor::generateTileButtons(ResourceManager& a_rm)
{
	int i = 1;
	float posX_offset = 256;
	float posY_offset = 64;
	TileButton* temp;

	// Generate Default Tiles
	for (const auto& key : {"tile_03", "tile_02", "tile_01",
							"tile_04", "tile_05", "tile_06",
							"tile_09", "tile_10", "tile_11"})
	{
		temp = new TileButton(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), TileType::Default, key);
		m_tileButtons.push_back(temp);
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
	for (const auto& key : {"tile_09_lock", "tile_10_lock", "tile_11_lock",
							"tile_09_lock2", "tile_10_lock2", "tile_11_lock2"})
	{
		if (i <= 3)
			temp = new TileButton(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), TileType::Lock, key);
		else
			temp = new TileButton(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), TileType::Lock2, key);

		m_tileButtons.push_back(temp);
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
	for (const auto& key : {"tile_special", "tile_special2", "tile_special3"})
	{
		TileType type = TileType::Special;
		if (i == 2)
			type = TileType::Special2;
		else if (i == 3)
			type = TileType::Special3;

		temp = new TileButton(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), type, key);
		m_tileButtons.push_back(temp);

		posX_offset += 128;
		i++;
	}

	// Generate Wall Tiles
	posX_offset = WIDTH - 384;
	posY_offset = 192;
	for (const auto& key : {"tile_wall", "tile_wall2", "tile_wall3"})
	{
		temp = new TileButton(a_rm, sf::Vector2f(posX_offset, HEIGHT - posY_offset), TileType::Wall, key);
		m_tileButtons.push_back(temp);
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
			m_tileMap.push_back(new Tile(a_rm, sf::Vector2f(posX, posY), TileType::Blank));
			posX += 128.0f;
		}
		posX = 0;
		posY += 64.0f;
	}
}