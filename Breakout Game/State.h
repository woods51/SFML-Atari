#pragma once
#include "stdafx.h"
#include "ResourceManager.h"

class State
{
public:
    // Default Constructor for State class
	State() {

	}

    ////////////////////////////////////////////////////////////
    /// \brief Handles keyboard inputs
    ///
    /// This function handles any keyboard inputs,
    /// it will handle both key pressed down and released events.
    /// This function is called every frame.
    /// 
    /// \param a_key		--> Position of Tile
    /// \param a_isPressed	--> True if key pressed, False if released
    ////////////////////////////////////////////////////////////
	virtual void inputHandler(sf::Keyboard::Key a_key, bool a_isPressed) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Handles sfml window events
    ///
    /// This function handles any window events that occur
    /// in the render window. This function calls the inputHandler
    /// and also handles any mouse events.
    /// This function is called every frame.
    /// 
    /// \param a_window --> RenderWindow
    /// \param a_rm          --> ResourceManager
    /// \param a_states     --> Vector of game states
    ////////////////////////////////////////////////////////////
	virtual void eventHandler(sf::RenderWindow& a_window, ResourceManager& a_rm, std::vector<std::unique_ptr<State>>& a_states) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Updates game elements
    ///
    /// This function updates all gameplay elements in its state.
    /// This function is called every frame.
    /// 
    /// \param a_dt  --> deltaTime
    /// \param a_rm --> ResourceManager
    ////////////////////////////////////////////////////////////
	virtual void update(sf::Time a_dt, ResourceManager& a_rm) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Draws objects to RenderWindow.
    ///
    /// This function draws objects to the RenderWindow.
    /// This function is called every frame.
    /// 
    /// \param a_window --> RenderWindow
    ////////////////////////////////////////////////////////////
	virtual void render(sf::RenderWindow& a_window) = 0;

    void setDefaultText(ResourceManager& a_rm, sf::Text& a_text, unsigned int a_charSize,
        sf::Vector2f a_pos, std::string a_font = "default", sf::Color fill = sf::Color::White)
    {
        a_text.setFont(*a_rm.getFont(a_font));
        a_text.setFillColor(fill);
        a_text.setCharacterSize(a_charSize);
        a_text.setPosition(a_pos);
    }
};