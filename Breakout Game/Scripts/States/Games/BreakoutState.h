#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "../Menus/PauseMenu.h"
#include "../Menus/LoadMenu.h"
#include "../Menus/GameOver.h"
#include "../../GameObjects/Ball.h"
#include "../../GameObjects/Paddle.h"
#include "../../LevelLoader.h"

////////////////////////////////////////////////////////////
/// \brief BreakoutState Class.
///
/// This class inherits members from the State Class. The BreakoutState class
/// is a Game State which runs the Breakout game program.
/// 
////////////////////////////////////////////////////////////
class BreakoutState : public State
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the Default Breakout State.
	///
	/// Constructs the default breakout state by initializing game objects,
	/// generating user interfaces, loading default levels, and starting
	/// the game.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	/// \param a_lives		--> In-Game lives value
	////////////////////////////////////////////////////////////
	BreakoutState(ResourceManager& a_rm, sf::RenderWindow& a_window, unsigned int a_lives);

	////////////////////////////////////////////////////////////
	/// \brief Constructor for Custom Breakout States.
	///
	/// Constructs the custom breakout state by initializing game objects,
	/// generating user interfaces, loading the custom level map provided,
	/// and starting the game.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	/// \param a_tileMap	--> Custom tile map
	/// \param a_lives		--> In-Game lives value
	////////////////////////////////////////////////////////////
	BreakoutState(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<Tile*>& a_tileMap, unsigned int a_lives);

	////////////////////////////////////////////////////////////
	/// \brief Handles SFML window events.
	///
	/// This function handles any window events that occur
	/// in the render window. This function calls the inputHandler
	/// and handles any mouse events including button interactions.
	/// This function is responsible for pushing new game states
	/// and is called once every frame.
	/// 
	/// \param a_rm          --> ResourceManager
	/// \param a_window --> RenderWindow
	/// \param a_states     --> Vector of game states
	////////////////////////////////////////////////////////////
	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;

	////////////////////////////////////////////////////////////
	/// \brief Provides game updates.
	///
	/// This function computes in-game physics, collisions,
	/// movement, and updates user interfaces. This function may
	/// also set the m_gameOver flag or trigger a level complete.
	/// 
	/// \param a_rm	--> ResourceManager
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	void update(ResourceManager& a_rm, const sf::Time& a_dt) override;

	////////////////////////////////////////////////////////////
	/// \brief Draws objects to RenderWindow.
	///
	/// This function draws objects to the RenderWindow.
	/// This function is called once every frame.
	/// 
	/// \param a_window --> RenderWindow
	////////////////////////////////////////////////////////////
	void render(sf::RenderWindow& a_window) override;

	////////////////////////////////////////////////////////////
	/// \brief Destructor for BreakoutState. 
	///
	///	Deconstructs and frees any allocated memory.
	////////////////////////////////////////////////////////////
	~BreakoutState();
private:

	////////////////////////////////////////////////////////////
	/// \brief Handles keyboard inputs.
	///
	/// This function handles any keyboard inputs,
	/// it will handle both key pressed down and released events.
	/// This function is called every frame.
	/// 
	/// \param a_key		--> Position of Tile
	/// \param a_isPressed	--> True if key pressed, False if released
	////////////////////////////////////////////////////////////
	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed);

	////////////////////////////////////////////////////////////
	/// \brief Updates user interfaces.
	///
	/// This function updates user interfaces.
	/// 
	/// \param a_rm	--> ResourceManager
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	void updateUI(ResourceManager& a_rm, const sf::Time& a_dt);

	////////////////////////////////////////////////////////////
	/// \brief Generates state user interfaces.
	///
	/// This function generates and initializes all
	/// UI elements for its state.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateUI(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Generates text objects.
	///
	/// This function generates and initializes all
	/// text objects.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateText(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Handles ball collisions and physics.
	///
	/// This function computes and handles collisions for the balls
	/// and paddle objects. This function handles collisions between
	/// the balls and paddle as well as between the balls and tiles.
	/// This function may also set the completeFlag if no tiles are active.
	/// 
	/// \param a_rm	--> ResourceManager
	/// \param a_dt	--> deltaTime
	////////////////////////////////////////////////////////////
	void handleBallPhysics(ResourceManager& a_rm, const sf::Time& a_dt);

	////////////////////////////////////////////////////////////
	/// \brief Handles ball collisions with special tiles.
	///
	/// This function handles the abilities of special tiles,
	/// it will spawn the correct number of balls into the level.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_type		--> TileType
	/// \param a_pos	--> Spawn position
	////////////////////////////////////////////////////////////
	void handleSpecialTiles(ResourceManager& a_rm, TileType a_type, sf::Vector2f a_pos);

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
	/// \brief Resets balls vector to initial state.
	///
	/// This function clears the balls vector and creates
	/// a new single ball instance.
	/// 
	/// \param a_rm		--> ResourceManager
	////////////////////////////////////////////////////////////
	void resetBalls(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Makes a copy of the given tile map.
	///
	/// This function copies the tile map passed
	/// as the argument to the member tile map vector.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_tileMap	--> Tile map to copy
	////////////////////////////////////////////////////////////
	void copyLevelMap(ResourceManager& a_rm, std::vector<Tile*>& a_tileMap);

	////////////////////////////////////////////////////////////
	/// \brief Handles level completion and incrementation.
	///
	/// This function handles level completion and incrementation
	/// by resetting game objects, loading next level map, or
	/// reloading current level in the case of a custom game.
	/// 
	/// \param a_rm		--> ResourceManager
	////////////////////////////////////////////////////////////
	void levelComplete(ResourceManager& a_rm);

	////////////////////////////////////////////////////////////
	/// \brief Resets current tile map.
	///
	/// This function resets each tile in the current tile map.
	/// 
	////////////////////////////////////////////////////////////
	void resetLevel();

	////////////////////////////////////////////////////////////
	/// \brief Loads level into tile map.
	///
	/// This function loads a level into the tile map from the file
	/// directory defined under the member m_loadPath. This function
	/// will replace with the m_tileMap with the contents of the file.
	/// This function may set the m_errorFlag if an error occurs while loading.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_levelName	--> File name of level
	////////////////////////////////////////////////////////////
	void loadLevel(ResourceManager& a_rm, std::string a_levelName);

	// Levels
	std::vector<Tile*> m_tileMap;
	std::vector<std::string> m_levels;
	LevelLoader m_loader;
	int m_currentLevel = 0;

	// Buttons
	std::vector<Button*> m_buttons;

	// Game objects
	Paddle* m_paddle;
	std::vector<std::unique_ptr<Ball>> m_balls;

	// Text objects
	sf::Text m_freePlayText;
	sf::Text m_startText;
	sf::Text m_wallLevelText;
	sf::Text m_specialLevelText;
	sf::Text m_lockedLevelText;

	sf::Text m_scoreText;
	sf::Text m_level;
	sf::Text m_livesText;
	sf::Text m_loadError;

	// Backgrounds
	sf::Sprite m_border;
	sf::Texture m_frameTexture;
	sf::Texture m_backgroundTexture;
	sf::Sprite m_background;
	sf::Sprite m_background2;

	// Flags
	bool m_gameStarted = false;
	bool m_gameOver = false;
	bool m_completeFlag = false;
	bool m_isCustom = false;
	bool m_freePlay = false;
	bool m_errorFlag = false;
	bool m_backgroundFlag = true;
	
	// Data members
	std::string m_errorMsg;
	std::string m_loadPath = "BreakoutLevels/";
	int m_score = 0;
	int m_livesRemaining;
	
};