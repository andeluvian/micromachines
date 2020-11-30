#include "mainmenu.hpp"


/** Loads all textures to map menuTextures and all sprites to map menuSprites.
*/
MainMenu::MainMenu() : Menu()
{
	init();
}


/// Loads the buttons for the main menu.
void MainMenu::init()
{
	menuSprites.clear();
	menuStrings.clear();
	setText(WIDTH*0.5f, HEIGHT*0.1f, "Credits: Hannes Pitkanen, Martin Vidjeskog, Miika Karsimus, Vili Karilas");
	bgColor = sf::Color(30, 30, 30);
	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::START, WIDTH * 0.25f, HEIGHT * 0.35f);
	setText(ButtonTexture::START, "Start");
	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::EXIT, WIDTH * 0.25f, HEIGHT * 0.55f);
	setText(ButtonTexture::EXIT, "Exit");
}

/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
/// Pressing Enter will init race select screen
/// Pressing Escape will quit the game
/// Pressing the mouse buttons calls the clickHandler
/// Quitting the game by closing the window is handled in game.cpp
GameStates MainMenu::eventHandler(sf::Event& event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return)
		// NOTE: Using KeyPressed instead of KeyReleased will ruin everything (at least on my computer)
	{
		pressedFlag = true;
		return GameStates::STATE_RACETYPE;
	}

	else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
	{
		return GameStates::STATE_EXIT;
	}

	else if (event.type == sf::Event::MouseButtonReleased)
	{
		//To correctly get the mouse position when the screen size is changed, we need to use mapPixeltoCoords
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		sf::Vector2f mousepos = window.mapPixelToCoords(mouse);
		return(clickHandler(mousepos.x, mousepos.y, window));
	}

	else
		return(GameStates::STATE_MAINMENU);
}

/// To make the EventHandler a little less cluttered, this handles the case where a mouse button is pressed.
/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
/// Check if mouse is currently positioned in the right space (inside the specified buttons) and act accordingly.
GameStates MainMenu::clickHandler(float x, float y, sf::RenderWindow& window)
{

	for (auto i : menuSprites)
	{
		if (i.second.getGlobalBounds().contains(x, y))
		{
			switch (i.first)
			{
			case ButtonTexture::START:
			{
				pressedFlag = true;
				return GameStates::STATE_RACETYPE;
			}

			case ButtonTexture::EXIT:
			{
				return GameStates::STATE_EXIT;
			}

			default:
				return GameStates::STATE_MAINMENU;
			}
		}
	}
	return GameStates::STATE_MAINMENU;
}
