#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <utility>

#include "menu.hpp"

/**
*	Author: Vili Karilas
*	
*	The main menu. Only thing we can do is press enter to go to race select or escape to quit.
**	eventHandler handles all events, calling clickHandler in the case of a mouse click.
*/

class MainMenu: public Menu
{
public:
	MainMenu();

	/// Loads the buttons for the main menu.
	virtual void init() override;
	/// Loads the buttons for a sub menu.
	void initSub();

	/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
	/// Pressing Enter will init race select screen
	/// Pressing Escape will quit the game
	/// Pressing the mouse buttons calls the clickHandler
	/// Quitting the game by closing the window is handled in game.cpp
	virtual GameStates eventHandler(sf::Event&, sf::RenderWindow&) override;

	/// Called from eventHandler, this handles the case where a mouse button is pressed.
	/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
	/// Check if mouse is currently positioned in the right space (inside the specified buttons) and act accordingly.
	virtual GameStates clickHandler(float, float, sf::RenderWindow&) override;
	
private:

};
#endif
