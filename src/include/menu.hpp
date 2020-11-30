#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include "soundhandler.hpp"
#include "constants.hpp"

/**
*	Author: Vili Karilas
*
*	Pure virtual base class for menus.
*	Override the following: init(), eventHandler() and clickHandler().
*	Init() should call loadSprite() and setText() for each button the menu uses, and set the background color.
*/

enum class ButtonTexture
	//Enum class for buttons. Use ButtonTexture::XXXXX to access.
{
	EMPTYBUTTON,
	SELECTEDBUTTON,
	BANNEDBUTTON,
	START,
	OPTION,
	EXIT,
	RACE,
	PRACTICE,
	SPLITSCREEN,
	TIMETRIAL,
	AI0,
	AI1,
	AI2,
	PL0,
	PL1,
	PL2,
	BACKGROUND //not implemented
};


class Menu
{
public:
	Menu();
	~Menu();

	/// Pure virtual. Call loadSprite() and setText() for each button here.
	virtual void init() = 0;

	/// Load a texture and store it to menuTextures.
	void load(ButtonTexture ID, const std::string& filename);

	/// Load the sprite for a button, set its position and add it to menuSprites.
	/// newID allows you to use multiples of the same texture and have different implementations for them.
	/// For example:	load(ButtonTexture::EMPTYBUTTON, ButtonTexture::RACE, 0.2f * width, 0.2f * height)
	///					load(ButtonTexture::EMPTYBUTTON, ButtonTexture::TIMETRIAL, 0.2f * width, 0.35 * height)
	/// This is more efficient than calling the other load with different ButtonTextures for everything.
	virtual void loadSprite(ButtonTexture ID, ButtonTexture newID, float posx, float posy);

	/// Takes enum class ButtonTexture::BUTTONNAME as a parameter
	/// Returns a reference to that button.
	sf::Texture& getTexture(ButtonTexture ID);

	/// Const version of the getTexture function.
	/// Takes enum class ButtonTexture::BUTTONNAME as a parameter
	/// Returns a reference to that button.
	const sf::Texture& getTexture(ButtonTexture ID) const;

	/// Sets a string and text to a button, centering the text.
	/// Note that text centering with large font sizes doesn't work properly.
	void setText(ButtonTexture ID, const std::string& str);

	/// Sets a string with absolute position to be drawn on the menu.
	/// Note that text centering with large font sizes doesn't work properly.
	void setText(float x, float y, const std::string& str);

	void setBanned(ButtonTexture ID, bool value);
	bool getBanned(ButtonTexture ID);
	void setSelected(ButtonTexture ID, bool value);
	bool getSelected(ButtonTexture ID);

	/// Draws the items in the menuSprites map. Called every cycle.
	void drawHandler(sf::RenderWindow& window);

	/// Handles events, use to call clickHandler(). Pure virtual.
	virtual GameStates eventHandler(sf::Event&, sf::RenderWindow&) = 0; 

	/// Handles clicks, called from eventHandler(). Pure virtual.
	virtual GameStates clickHandler(float x, float y, sf::RenderWindow& window) = 0;

	void updateSound(SoundHandler& soundhandler);


protected:
	sf::Font font;
	std::vector<sf::Text> menuStrings;

	std::vector<ButtonTexture> bannedButtons;
	std::vector<ButtonTexture> selectedButtons;
	std::map<ButtonTexture, sf::Sprite> menuSprites;
	std::map<ButtonTexture, std::unique_ptr<sf::Texture> > menuTextures;
	//a unique_ptr allows us to access multiple sprites with relative ease.

	const int width;
	const int height;
	//sf::Texture bgText; //background texture (not implemented)
	sf::Color bgColor = sf::Color::White; //background color: if you're in a submenu, it's not white
	bool pressedFlag = false; //flag for sound. turn on if a button is pressed (setSelected changes) or we move to next menu
};
#endif
