#include "SaveMenu.h"

SaveMenu::SaveMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture, const std::vector<Tile*> a_tileMap)
{
	m_tileMap = a_tileMap;

	// Setup background
	m_frameTexture = a_frameTexture;
	m_frameSprite.setTexture(m_frameTexture);

	sf::Vector2u windowSize = a_window.getSize();
	m_frameSprite.setScale((float)WIDTH / windowSize.x, (float)HEIGHT / windowSize.y);

	generateUI(a_rm);
}
void SaveMenu::eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states)
{
	sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
	bool lock_click = false;

	sf::Event event;
	while (a_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::TextEntered:
			handleTextInput(event.text.unicode);
			break;

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

		case sf::Event::Closed:
			a_window.close();
			break;
		}
	}
}

void SaveMenu::update(ResourceManager& a_rm, const sf::Time& a_dt)
{
	// Updates validity icon
	if (m_isFileValid)
		m_fileValid.setTexture(*a_rm.getTexture("icon_check"));
	else
		m_fileValid.setTexture(*a_rm.getTexture("icon_cross"));
}

void SaveMenu::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	// Render backgrounds
	a_window.draw(m_frameSprite);
	a_window.draw(m_overlay);

	// Render buttons
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	// Render sprites
	a_window.draw(m_typeLine);
	a_window.draw(m_fileValid);

	// Render text objects
	a_window.draw(m_fileName);
	a_window.draw(m_saveResult);
	
	a_window.display();
}

SaveMenu::~SaveMenu()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}

void SaveMenu::handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
	const sf::Vector2f& a_mousePosition)
{
	for (auto& b : m_buttons)
	{
		sf::Vector2f b_pos = b->getPosition();
		sf::Vector2f b_diag_pos = b->getDiagonalPosition();

		if (a_mousePosition.x >= b_pos.x && a_mousePosition.x <= b_diag_pos.x &&
			a_mousePosition.y >= b_pos.y && a_mousePosition.y <= b_diag_pos.y)
		{
			switch (b->OnClick(a_rm))
			{
			case Press::BACK:
				a_states.pop_back();
				break;

			// Enables text editing
			case Press::EDIT:
				m_isTyping = true;
				b->isSelected(m_isTyping);
				break;

			case Press::SAVE:
				m_isTyping = false;
				m_editButton->isSelected(m_isTyping);

				if (!m_isSaving && m_isFileValid)
					saveMap();
				break;

			case Press::QUIT:
				a_window.close();
				break;

			default:
				break;
			}
		}
	}
}

void SaveMenu::generateUI(ResourceManager& a_rm)
{
	generateButtons(a_rm);

	// Generate Sprites
	m_typeLine.setTexture(*a_rm.getTexture("type_line"));
	m_typeLine.setScale(sf::Vector2f(4, 4));
	m_typeLine.setPosition(WIDTH / 2 - 32 * 4, 300);

	m_fileValid.setTexture(*a_rm.getTexture("icon_cross"));
	m_fileValid.setScale(3, 3);
	m_fileValid.setPosition(sf::Vector2f(m_typeLine.getPosition()) + sf::Vector2f(300, -10));

	// Generate text objects
	setDefaultText(a_rm, m_fileName, 20, m_typeLine.getPosition() + sf::Vector2f(16, -15));
	setDefaultText(a_rm, m_saveResult, 25, sf::Vector2f(WIDTH / 2, HEIGHT / 2 + 300));

	// Generate backgrounds
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));
}

void SaveMenu::generateButtons(ResourceManager& a_rm)
{
	Button* temp;

	// Edit
	temp = createMenuButton(a_rm, sf::Vector2f((WIDTH / 2) - (32 * 8) - 10, (HEIGHT / 2) + 64),
		Press::EDIT, "EDIT");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(64.0f, 8.0f));
	m_editButton = temp;
	m_buttons.push_back(temp);

	// Save
	temp = createMenuButton(a_rm, sf::Vector2f((WIDTH / 2) + 10, (HEIGHT / 2) + 64),
		Press::SAVE, "SAVE");
	temp->setDefaultText(a_rm, 40, temp->getShape().getPosition() + sf::Vector2f(52.0f, 8.0f));
	m_buttons.push_back(temp);

	// Back
	temp = new Button(a_rm, sf::Vector2f(75, 100),
		Press::BACK, sf::Vector2f(4.0f, 6.0f), sf::Vector2f(32.0f, 8.0f), "button_menu", "button_menu_selected");
	temp->setString("BACK");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(16.0f, 10.0f));
	m_buttons.push_back(temp);
}

void SaveMenu::handleTextInput(sf::Uint32 a_unicode)
{
	if (!m_isTyping)
		return;

	// Unicode isn't a valid number or character
	if (!(a_unicode >= 48 && a_unicode <= 58)
		&& !(a_unicode >= 97 && a_unicode <= 122)
		&& (a_unicode != 95) && (a_unicode != 8))
		return;

	// Handles backspace event
	if (a_unicode == 8)
	{
		if (m_textInput.size() > 0)
			m_textInput.pop_back();
	}
	else if (m_textInput.size() < 14)
		m_textInput += a_unicode;

	// Update file name
	m_fileName.setString(m_textInput);

	// Determine text validity
	if (m_textInput.size() > 4)
		m_isFileValid = true;
	else
		m_isFileValid = false;
}

void SaveMenu::saveMap()
{
	m_isSaving = true;
	std::string levelName = m_fileName.getString().toAnsiString();

	// Verify that filename doesn't overrite another
	if (!(verifyFileName(levelName)))
	{
		// Display save error
		m_saveResult.setString("Save Failed: File already exists.");
		m_saveResult.setPosition(WIDTH / 2 -314, HEIGHT / 2 -160);
		m_saveResult.setFillColor(sf::Color::Red);
		m_isSaving = false;
		return;
	}

	std::ofstream outputFile;
	std::string fileName = m_savePath + levelName + ".csv";
	outputFile.open(fileName);

	std::string fileBuffer;

	// Save each tile property to buffer
	for (const auto& tile : m_tileMap)
	{
		sf::Vector2f pos = tile->getPosition();
		TileType type = tile->getTileType();
		if (type == TileType::Blank)
			continue;

		std::string texture = tile->getTextureKey();

		// Handle locked texture cases
		if (texture.find("_lock2") != std::string::npos)
			texture = texture.substr(0, texture.size() - 6);

		else if (texture.find("_lock") != std::string::npos)
			texture = texture.substr(0, texture.size() - 5);

		fileBuffer += std::to_string((int)pos.x) + "," + std::to_string((int)pos.y) + "," + std::to_string((int)type) + "," + texture + "\n";
	}

	// Write to file
	outputFile << fileBuffer;
	outputFile.close();
	m_isSaving = false;

	// Display save success
	m_saveResult.setString("Save Successful!");
	m_saveResult.setPosition(WIDTH / 2 - 160, HEIGHT / 2 - 160);
	m_saveResult.setFillColor(sf::Color::Green);
}

bool SaveMenu::verifyFileName(std::string a_fileName)
{
	bool isValid = true;
	std::string fileName = a_fileName + ".csv";
	std::string name;
	std::string texturePath;

	// Search directory for matching file name
	for (const auto& file : std::filesystem::directory_iterator(m_savePath))
	{
		texturePath = file.path().string();
		name = file.path().filename().string();

		if (fileName == name)
			isValid = false;

	}
	return isValid;
}