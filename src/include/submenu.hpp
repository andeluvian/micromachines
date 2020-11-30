#ifndef SUB_HH
#define SUB_HH

#include "race.hpp"
#include "menu.hpp"


/**
*	Author: Vili Karilas
*	
*	Contains all submenus that mainmenu opens. Derived from virtual class Menu.
*	modeMenu refers to the race select screen, where we pick a map and a race type. If we fail to load the map when going to
*	vehicleSelectMenu, we'll get thrown back to the main menu along with an error message.
**
*/

class modeMenu : public Menu
{
public:
	modeMenu();

	virtual void init() override;
	/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
	/// Pressing the mouse buttons calls the clickHandler
	/// Quitting the game by closing the window is handled in game.cpp
	virtual GameStates eventHandler(sf::Event&, sf::RenderWindow&) override;

	/// Called from eventHandler, this handles the case where a mouse button is pressed.
	/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
	/// Check if mouse is currently positioned in the right space (inside the specified buttons) and act accordingly.
	virtual GameStates clickHandler(float, float, sf::RenderWindow&) override;

};

/** vehicleSelectMenu is derived from virtual class Menu and is the last menu screen. 
*	Here we pick the vehicles/players that join in in our race.
*
*/
class vehicleSelectMenu : public Menu
{
public:
	vehicleSelectMenu(GameStates raceType);
	virtual void init() override;

	/// Prohibit certain buttons from being selected depending on the game type (timetrial = no AI, etc.)
	void initBans(GameStates raceType);

	/// overridden to allow for different sized buttons
	virtual void loadSprite(ButtonTexture ID, ButtonTexture newID, float posx, float posy) override;

	/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
	/// Pressing the mouse buttons calls the clickHandler
	/// Quitting the game by closing the window is handled in game.cpp
	virtual GameStates eventHandler(sf::Event&, sf::RenderWindow&) override;

	/// Called from eventHandler, this handles the case where a mouse button is pressed.
	/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
	/// Check if mouse is currently positioned in the right space (inside the specified buttons) and act accordingly.
	virtual GameStates clickHandler(float, float, sf::RenderWindow&) override;

private:
	GameStates raceType;
};
#endif
