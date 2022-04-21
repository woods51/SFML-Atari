#include "LoadMenu.h"

LoadMenu::LoadMenu(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Sprite* a_background, sf::Sprite* a_background2, unsigned int a_lives)
{
	m_lives = a_lives;

	m_background = a_background;
	m_background2 = a_background2;
	m_background->setPosition(m_background->getPosition() + sf::Vector2f(0.25f, 0));
	m_background2->setPosition(m_background2->getPosition() + sf::Vector2f(0.25f, 0));

	generateUI(a_rm);

	loadFiles();
	loadFirstPage();
}
LoadMenu::~LoadMenu()
{
	for (auto& b : m_buttons)
	{
		delete b;
	}
}
void LoadMenu::inputHandler(sf::Keyboard::Key a_key, bool a_isPressed)
{

}
void LoadMenu::eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states)
{
	sf::Event event;
	sf::Vector2f mousePosition = a_window.mapPixelToCoords(sf::Mouse::getPosition(a_window));
	static bool lock_click = false;

	buttonSelectUpdate(a_window, a_rm, mousePosition);

	// Handle Events
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

					// Button Pressed
					if (mousePosition.x >= b_pos.x && mousePosition.x <= b_diag_pos.x &&
						mousePosition.y >= b_pos.y && mousePosition.y <= b_diag_pos.y)
					{
						std::string level;
						switch (b->OnClick(a_rm))
						{
						case Press::LEVEL:
							level = b->getText().getString().toAnsiString();
							if (level.size() < 3)
								break;
							m_selectedLevel.setString(level);
							m_isValid = true;
							b->isSelected(true);
							break;
						case Press::BACK:
							a_states.pop_back();
							break;
						case Press::NEXT:
							nextPage();
							break;
						case Press::PREVIOUS:
							previousPage();
							break;
						case Press::RELOAD:
							loadFiles();
							loadFirstPage();
							break;
						case Press::LOAD:
							if (!m_isValid)
							{
								break;
							}
							if (!loadMap(a_rm, m_selectedLevel.getString().toAnsiString()))
							{
								break;
							}
							a_states.push_back(std::make_unique<BreakoutState>(a_rm, a_window, m_tileMap, m_lives));
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

		case sf::Event::Closed:
			a_window.close();
			break;
		}
	}
}
void LoadMenu::update(sf::Time a_dt, ResourceManager& a_rm)
{
	m_pageNumber.setString(std::to_string(m_currentPage));

	m_background->setPosition(m_background->getPosition() + sf::Vector2f(0.25f, 0));
	if (m_background->getPosition().x == 3200)
	{
		m_background->setPosition(sf::Vector2f(-3200, 0));
	}
	m_background2->setPosition(m_background2->getPosition() + sf::Vector2f(0.25f, 0));
	if (m_background2->getPosition().x == 3200)
	{
		m_background2->setPosition(sf::Vector2f(-3200, 0));
	}
}
void LoadMenu::render(sf::RenderWindow& a_window)
{
	a_window.clear(sf::Color::Black);

	a_window.draw(*m_background);
	a_window.draw(*m_background2);
	a_window.draw(m_overlay);

	a_window.draw(m_pageNumber);
	a_window.draw(m_levelText);
	a_window.draw(m_selectedLevel);
	a_window.draw(m_loadError);

	// Render UI
	for (const auto& b : m_buttons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}
	for (const auto& b : m_levelButtons)
	{
		a_window.draw(b->getShape());
		a_window.draw(b->getText());
	}

	a_window.display();
}
void LoadMenu::generateUI(ResourceManager& a_rm)
{
	// generate all buttons
	Button* temp;

	// > Next
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) + 6, (HEIGHT / 2) -150), Press::NEXT, ">", Sound::Button);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// < Previous
	temp = new TickButton(a_rm, sf::Vector2f((WIDTH / 2) - 54, (HEIGHT / 2) -150), Press::PREVIOUS, "<", Sound::Button);
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18.0f, 4.0f));
	m_buttons.push_back(temp);

	// BACK
	temp = new Button(a_rm, sf::Vector2f(75, 100),
		Press::BACK, sf::Vector2f(4, 6), sf::Vector2f(32, 8), "button_menu", "button_menu_selected");
	temp->setString("BACK");
	temp->setDefaultText(a_rm, 25, temp->getShape().getPosition() + sf::Vector2f(18, 10));
	m_buttons.push_back(temp);

	// Load
	temp = new Button(a_rm, sf::Vector2f(896, HEIGHT/2 -100),
		Press::LOAD, sf::Vector2f(8, 12), sf::Vector2f(32, 8), "button_menu", "button_menu_selected");
	temp->setString("LOAD");
	temp->setDefaultText(a_rm, 50, temp->getShape().getPosition() + sf::Vector2f(40, 20));
	m_buttons.push_back(temp);

	// RELOAD
	temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) + 100, (HEIGHT / 2) - 210),
		Press::RELOAD, sf::Vector2f(3, 3), sf::Vector2f(16, 16), "button_reload", "button_reload");
	m_buttons.push_back(temp);

	//UI
	m_overlay.setTexture(*a_rm.getTexture("pause_menu"));
	m_overlay.setScale(sf::Vector2f(80.0f, 80.0f));

	// Text
	m_defaultFont = *a_rm.getFont("default");
	// Page Number
	setDefaultText(a_rm, m_pageNumber, 40, sf::Vector2f(WIDTH / 2 - 14, HEIGHT / 2 - 210));
	m_pageNumber.setString(std::to_string(m_currentPage));

	// Selected Level
	setDefaultText(a_rm, m_selectedLevel, 25, sf::Vector2f(896, HEIGHT / 2 - 136));
	m_selectedLevel.setString("");

	// Level
	setDefaultText(a_rm, m_levelText, 20, sf::Vector2f((WIDTH / 2) - 36, HEIGHT / 2 - 240));
	m_levelText.setString("PAGE");

	// Load Error
	setDefaultText(a_rm, m_loadError, 20, sf::Vector2f(24, HEIGHT / 2 - 136), "default", sf::Color::Red);

	int posY = HEIGHT / 2 - 100;
	for (int i = 0; i < 5; i++)
	{
		temp = new Button(a_rm, sf::Vector2f((WIDTH / 2) - (32 * 4), posY), Press::LEVEL, sf::Vector2f(8, 8), sf::Vector2f(32, 6),
			"loaded_level", "loaded_level_selected");
		temp->setDefaultText(a_rm, 20, temp->getPosition() + sf::Vector2f(8, 10));
		m_buttons.push_back(temp);
		m_levelButtons.push_back(temp);
		posY += 60;
	}
	
}
void LoadMenu::loadFirstPage()
{
	m_currentPage = 1;
	m_selectedLevel.setString("");
	m_isValid = false;
	if (m_levels.size() > 5)
		loadPage(0, 5);
	else
		loadPage(0, m_levels.size());
}
void LoadMenu::nextPage()
{
	if (m_currentPage == m_maxPages)
		return;

	m_currentPage++;

	int start = (m_currentPage - 1) * 5;
	int end;

	if ((m_currentPage * 5) > m_levels.size())
		end = (m_levels.size() - (m_currentPage - 1) * 5) + start;
	else
		end = m_currentPage * 5;

	loadPage(start, end);
}
void LoadMenu::previousPage()
{
	if (m_currentPage == 1)
		return;

	m_currentPage--;

	int start;
	int end;

	start = (m_currentPage - 1) * 5;

	if ((m_currentPage * 5) > m_levels.size())
		end = m_levels.size() - (m_currentPage - 1) * 5;
	else
		end = m_currentPage * 5;

	loadPage(start, end);
}
void LoadMenu::loadPage(int start, int end)
{
	for (int i = 0; i < 5; start++, i++)
	{
		if (!(start < end))
		{
			m_levelButtons[i]->setString("");
		}
		else
			m_levelButtons[i]->setString(m_levels[start]);
	}
}
void LoadMenu::loadFiles()
{
	m_levels.clear();
	std::string name;
	std::string texturePath;
	std::string path = "SavedLevels/";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		texturePath = file.path().string();

		name = file.path().filename().string();
		name = name.substr(0, name.length() - 4);

		m_levels.push_back(name);
	}
	
	m_maxPages = m_levels.size() / 5;
	if (m_levels.size() % 5 != 0)
		m_maxPages++;
}
bool LoadMenu::loadMap(ResourceManager& a_rm, std::string a_path)
{
	std::string line;
	std::string path = "SavedLevels/" + a_path + ".csv";
	std::ifstream inputFile(path);

	int i = 1;
	if (inputFile.is_open())
	{
		while (std::getline(inputFile, line))
		{
			if (!parseTileData(a_rm, line))
			{
				// Error Loading File
				std::string temp = "Error reading file.\nLine (" + std::to_string(i) + ")\nLoad Canceled.";
				m_loadError.setString(temp);
				return false;
			}
			i++;
		}
		// Successful Load
		m_loadError.setString("");
		inputFile.close();
	}
	return true;
}
bool LoadMenu::parseTileData(ResourceManager& a_rm, std::string a_line)
{
	Tile* temp;
	TileType tileType = TileType::None;
	int posX, posY, type;
	std::string texture, error;
	posX = posY = type = 0;
	texture = error = "";

	replace(a_line.begin(), a_line.end(), ',', ' ');
	std::istringstream parser(a_line);
	parser >> posX >> posY >> type >> texture >> error;

	if (!error.empty())
		return false;
	
	tileType = getTileType(type);

	temp = new Tile(a_rm, sf::Vector2f(posX, posY), tileType, texture);

	m_tileMap.push_back(temp);
	return true;
}
TileType LoadMenu::getTileType(int a_type)
{
	switch (a_type)
	{
	case 0:
		return TileType::Default;
		break;
	case 1:
		return TileType::Lock;
		break;
	case 2:
		return TileType::Lock2;
		break;
	case 3:
		return TileType::Special;
		break;
	case 4:
		return TileType::Special2;
		break;
	case 5:
		return TileType::Special3;
		break;
	case 6:
		return TileType::Wall;
		break;
	default:
		return TileType::Blank;
		break;
	}
}
void LoadMenu::buttonSelectUpdate(sf::RenderWindow& a_window, ResourceManager& a_rm, const sf::Vector2f& a_mousePosition)
{
	for (auto& b : m_buttons)
	{
		sf::Vector2f b_pos = b->getPosition();
		sf::Vector2f b_diag_pos = b->getDiagonalPosition();

		if (a_mousePosition.x >= b_pos.x && a_mousePosition.x <= b_diag_pos.x &&
			a_mousePosition.y >= b_pos.y && a_mousePosition.y <= b_diag_pos.y)
		{
			b->isSelected(true);
		}
		else
			b->isSelected(false);
	}
}