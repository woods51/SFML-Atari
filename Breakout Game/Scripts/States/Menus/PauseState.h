#pragma once
#include "../../stdafx.h"
#include "../State.h"
#include "OptionsState.h"
#include "../../GameObjects/Button.h"

////////////////////////////////////////////////////////////
/// \brief PauseState Class.
///
/// This class inherits members from the State Class. The PauseState class
/// is a Menu State which provides a menu interface allowing the user
/// to switch between game states while in a paused state.
/// 
////////////////////////////////////////////////////////////
class PauseState : public State
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for the Pause State
	///
	/// Constructs the menu state UI elements and
	/// generates background textures.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_window	--> RenderWindow
	////////////////////////////////////////////////////////////
	PauseState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture);

	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_rm, sf::Time a_dt) override;
	void render(sf::RenderWindow& a_window) override;

	////////////////////////////////////////////////////////////
	/// \brief Destructor for PauseState. 
	///
	///	Deconstructs and frees any dynmically allocated memory.
	////////////////////////////////////////////////////////////
	~PauseState();

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
	/// \brief Updates selected state of buttons
	///
	/// This function updates the isSelected state of any button
	/// that is within the bounds of the current mouse position.
	/// This function also sets the title flags depending on
	/// which button is selected.
	/// 
	/// \param a_rm		--> ResourceManager
	/// \param a_mousePosition	--> Current mouse position
	////////////////////////////////////////////////////////////
	void buttonSelectUpdate(ResourceManager& a_rm, const sf::Vector2f& a_mousePosition);

	////////////////////////////////////////////////////////////
	/// \brief Generates state user interfaces
	///
	/// This function generates and initializes all
	/// UI elements for its state.
	/// 
	/// \param a_rm	--> ResourceManager
	////////////////////////////////////////////////////////////
	void generateUI(ResourceManager& a_rm);

	// Buttons
	std::vector<Button*> m_buttons;

	// Backgrounds
	sf::Texture m_frameTexture;
	sf::Sprite m_frameSprite;
	sf::Sprite m_overlay;

};