#pragma once
#include "../stdafx.h"
#include "../ResourceManager.h"

////////////////////////////////////////////////////////////
/// \brief State Class.
///
/// The State class is an abstract class that serves as the
/// base class for all game and menu states. The state class defines
/// three pure virtual functions that must be defined by every derived state,
/// these being the eventHandler, update, and render.
/// 
////////////////////////////////////////////////////////////
class State
{
public:
    // Default Constructor for State class
	State() {

	}

    ////////////////////////////////////////////////////////////
    /// \brief Handles sfml window events
    ///
    /// This function handles any window events that occur
    /// in the render window. This function is called once every frame.
    /// 
    /// \param a_rm          --> ResourceManager
    /// \param a_window --> RenderWindow
    /// \param a_states     --> Vector of game states
    ////////////////////////////////////////////////////////////
	virtual void eventHandler(ResourceManager& a_rm, sf::RenderWindow& a_window, std::vector<std::unique_ptr<State>>& a_states) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Updates game elements
    ///
    /// This function updates all gameplay elements in its state.
    /// This function is called every frame.
    /// 
    /// \param a_rm --> ResourceManager
    /// \param a_dt  --> deltaTime
    ////////////////////////////////////////////////////////////
	virtual void update(ResourceManager& a_rm, const sf::Time& a_dt) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Draws objects to RenderWindow.
    ///
    /// This function draws objects to the RenderWindow.
    /// This function is once called every frame.
    /// 
    /// \param a_window --> RenderWindow
    ////////////////////////////////////////////////////////////
	virtual void render(sf::RenderWindow& a_window) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Setup function for text objects
    ///
    /// This function sets the character size, position, font,
    /// and fill color for the reference text object.
    /// 
    /// \param a_rm             --> ResourceManager
    /// \param a_text           --> Text object
    /// \param a_charSize   --> Character size
    /// \param a_pos           --> Text position
    /// \param a_font          --> Font
    /// \param a_fill             --> Fill color
    ////////////////////////////////////////////////////////////
    void setDefaultText(ResourceManager& a_rm, sf::Text& a_text, unsigned int a_charSize,
        sf::Vector2f a_pos, std::string a_font = "default", sf::Color fill = sf::Color::White)
    {
        a_text.setFont(*a_rm.getFont(a_font));
        a_text.setFillColor(fill);
        a_text.setCharacterSize(a_charSize);
        a_text.setPosition(a_pos);
    }
};