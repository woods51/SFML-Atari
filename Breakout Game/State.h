#pragma once

#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "Button.h"

class State
{
public:
	State() {

	}

	virtual void inputHandler(sf::Keyboard::Key, bool) = 0;
	virtual void eventHandler(sf::RenderWindow&, ResourceManager&, std::vector<std::unique_ptr<State>>&) = 0;
	virtual void update(sf::Time, ResourceManager&) = 0;
	virtual void render(sf::RenderWindow&) = 0;

    ////////////////////////////////////////////////////////////
    /// \brief Pop the event on top of the event queue, if any, and return it
    ///
    /// This function is not blocking: if there's no pending event then
    /// it will return false and leave \a event unmodified.
    /// Note that more than one event may be present in the event queue,
    /// thus you should always call this function in a loop
    /// to make sure that you process every pending event.
    /// \code
    /// sf::Event event;
    /// while (window.pollEvent(event))
    /// {
    ///    // process event...
    /// }
    /// \endcode
    ///
    /// \param event Event to be returned
    ///
    /// \return True if an event was returned, or false if the event queue was empty
    ///
    /// \see waitEvent
    ///
    ////////////////////////////////////////////////////////////
};