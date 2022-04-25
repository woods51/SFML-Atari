#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "../Menus/PauseState.h"
#include "../Menus/SaveMenu.h"
#include "../../GameObjects/Tile.h"
#include "../../GameObjects/Button.h"

////////////////////////////////////////////////////////////
/// \brief LevelEditor Class.
///
/// This class inherits members from the State Class. The LevelEditor class
/// is a Game State which allows the user to create custom tile maps and save them.
/// User created levels can be accessed and played in the BreakoutState Load Menu.
/// 
////////////////////////////////////////////////////////////
class LevelEditor : public State
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the level editor class.
	///
	/// Constructs intial tile map and generates user interfaces.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	////////////////////////////////////////////////////////////
	LevelEditor(ResourceManager& a_rm, sf::RenderWindow& a_window);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_rm, sf::Time dt) override;
	void render(sf::RenderWindow& a_window) override;

	////////////////////////////////////////////////////////////
	/// \brief Destructor for LevelEditor. 
	///
	///	Deconstructs and frees any dynmically allocated memory.
	////////////////////////////////////////////////////////////
	~LevelEditor();
private:

	////////////////////////////////////////////////////////////
	/// \brief Handles and checks for any button events.
	///
	/// This function works in part with the eventHandler to
	/// compute whether the user has clicked a button when
	/// a left mouse button is triggered.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	/// \param a_states		--> Vector of game states
	/// \param a_mousePosition	--> Current mouse position
	////////////////////////////////////////////////////////////
	void handleButtonEvents(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states,
		const sf::Vector2f& a_mousePosition);

	////////////////////////////////////////////////////////////
	/// \brief Handles and checks for any tile button events.
	///
	/// This function works in part with the eventHandler to
	/// compute whether the user has clicked a tile button when
	/// a left mouse button is triggered.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	/// \param a_mousePosition	--> Current mouse position
	////////////////////////////////////////////////////////////
	void handleTileButtonEvents(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition);

	////////////////////////////////////////////////////////////
	/// \brief Handles selection overlay for tiles
	///
	/// This function works in part with the eventHandler to
	/// change each tile on the tile map texture based on the current tool selected.
	/// Allows user to draw onto tile map with provided pen tools.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_mousePosition	--> Current mouse position
	/// \param a_lockClick	--> Mouse click lock
	////////////////////////////////////////////////////////////
	void handleTileSelection(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition, const bool& a_lockClick);
	
	////////////////////////////////////////////////////////////
	/// \brief Generates initial tile map.
	///
	/// This function generates the default blank tile map for the level editor.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateTileMap(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Generates user interfaces.
	///
	/// This function generates and initializes all
	/// UI elements for its state.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateUI(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Generates button objects.
	///
	/// This function generates and initializes all
	/// button objects.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateButtons(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Generates tile button objects.
	///
	/// This function generates and initializes all
	/// tile button objects.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateTileButtons(ResourceManager& a_rm);

	// Tile map
	std::vector<Tile*> m_tileMap;
	
	// Buttons & UI
	std::vector<Button*> m_buttons;
	std::vector<TileButton*> m_tileButtons;
	Button* m_pen;
	Button* m_erase;
	Tile* m_selectedTile;

	// Textures
	sf::Texture m_frameTexture;
	sf::Sprite m_border;
	sf::Sprite m_previewTile;
	sf::Sprite m_selectorTile;
	
	// Text objects
	sf::Text m_toolsText;
	sf::Text m_regularText;
	sf::Text m_lockedText;
	sf::Text m_specialText;
	sf::Text m_wallText;
};