#pragma once
#include "../../stdafx.h"
#include "../Games/BreakoutState.h"
#include "../../GameObjects/Button.h"

class OptionsState : public State
{
public:
	OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Texture& a_frameTexture);
	OptionsState(ResourceManager& a_rm, sf::RenderWindow& a_window, sf::Sprite* a_background, sf::Sprite* a_background2);

	void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) override;
	void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) override;
	void update(ResourceManager& a_rm, sf::Time a_dt) override;
	void render(sf::RenderWindow& a_window) override;

	////////////////////////////////////////////////////////////
	/// \brief Destructor for BreakoutMenu. 
	///
	///	Deconstructs and frees any dynmically allocated memory.
	////////////////////////////////////////////////////////////
	~OptionsState();

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

	////////////////////////////////////////////////////////////
	/// \brief Adjust position of text object.
	///
	/// This is a helper function used to update the text positions for volume text.
	/// This function adjust the position of the given text object
	/// based on interger value and location in screen coordinates.
	/// 
	/// \param a_volume	--> Volume value
	/// \param a_text	--> Volume text object
	/// \param a_isOnLeft	--> Text is located left of the center
	////////////////////////////////////////////////////////////
	void adjustVolumeText(int a_volume, sf::Text& a_text, bool a_isOnLeft);

	// Buttons
	std::vector<Button*> m_buttons;

	// Backgrounds
	sf::Sprite m_overlay;
	sf::Sprite* m_background = nullptr;
	sf::Sprite* m_background2 = nullptr;

	// Text objects
	sf::Text m_buttonText;
	sf::Text m_buttonVolume;
	sf::Text m_ballText;
	sf::Text m_ballVolume;
	sf::Text m_gameText;
	sf::Text m_gameVolume;
	sf::Text m_levelText;
	sf::Text m_levelVolume;
	sf::Text m_volumeText;
};