#include "submenu.hpp"

modeMenu::modeMenu() : Menu()
{
	init();
}

void modeMenu::init()
{
	menuSprites.clear();
	menuStrings.clear();
	bgColor = sf::Color(30, 30, 30);

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::EXIT, 0, 0);
	menuSprites[ButtonTexture::EXIT].setScale(0.5f, 1.0f);
	setText(ButtonTexture::EXIT, "Back");

	//we use AI1 and AI2 for the buttontexturenames since I didn't really want to make another enum for this
	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::AI1, WIDTH * 0.25f, HEIGHT * 0.15f);
	menuSprites[ButtonTexture::AI1].setScale(0.48f, 1.0f);
	setText(ButtonTexture::AI1, "Map1");
	setSelected(ButtonTexture::AI1, true);

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::AI2, WIDTH * 0.5115f, HEIGHT * 0.15f);
	menuSprites[ButtonTexture::AI2].setScale(0.48f, 1.0f);
	setText(ButtonTexture::AI2, "Map2");


	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::RACE, WIDTH * 0.25f, HEIGHT * 0.3f);
	setText(ButtonTexture::RACE, "Race");

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::PRACTICE, WIDTH * 0.25f, HEIGHT * 0.45f);
	setText(ButtonTexture::PRACTICE, "Practice");

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::SPLITSCREEN, WIDTH * 0.25f, HEIGHT * 0.6f);
	setText(ButtonTexture::SPLITSCREEN, "Split Screen");

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::TIMETRIAL, WIDTH * 0.25f, HEIGHT * 0.75f);
	setText(ButtonTexture::TIMETRIAL, "Time Trial");
}

GameStates modeMenu::eventHandler(sf::Event& event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return)
		{
			pressedFlag = true;
			return GameStates::STATE_RACE;
		}

	else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
	{
			return GameStates::STATE_MAINMENU;
	}

	else if (event.type == sf::Event::MouseButtonReleased)
	{
		//To correctly get the mouse position when the screen size is changed, we need to use mapPixeltoCoords
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		sf::Vector2f mousepos = window.mapPixelToCoords(mouse);
		return(clickHandler(mousepos.x, mousepos.y, window));
	}

	else
		return(GameStates::STATE_RACETYPE);
}


/// To make the EventHandler a little less cluttered, this handles the case where a mouse button is pressed.
/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
/// Check if mouse is currently positioned in the right space (inside the specified buttons) and act accordingly.
GameStates modeMenu::clickHandler(float x, float y, sf::RenderWindow& window)
{

	for (auto i : menuSprites)
	{
		if (i.second.getGlobalBounds().contains(x, y))
		{
			switch (i.first)
			{
			case ButtonTexture::EXIT:
			{
				pressedFlag = true;
				return GameStates::STATE_MAINMENU;
			}

			case ButtonTexture::RACE:
			{
				pressedFlag = true;
				return GameStates::STATE_RACE;
			}

			case ButtonTexture::PRACTICE:
			{
				pressedFlag = true;
				return GameStates::STATE_PRACTICE;
			}

			case ButtonTexture::SPLITSCREEN:
			{
				pressedFlag = true;
				return GameStates::STATE_SPLITSCREEN;
			}

			case ButtonTexture::TIMETRIAL:
			{
				return GameStates::STATE_TIMETRIAL;
			}

			case ButtonTexture::AI1:
			{
				setSelected(ButtonTexture::AI1, true);
				setSelected(ButtonTexture::AI2, false);
				return GameStates::STATE_RACETYPE;
			}

			case ButtonTexture::AI2:
			{
				setSelected(ButtonTexture::AI1, false);
				setSelected(ButtonTexture::AI2, true);
				return GameStates::STATE_RACETYPE;
			}

			default:
				std::cout << "Background clicked, went back to main menu." << std::endl;
				return GameStates::STATE_RACETYPE;
			}
		}
	}
	return GameStates::STATE_RACETYPE;
}


vehicleSelectMenu::vehicleSelectMenu(GameStates raceType) : Menu(), raceType(raceType)
{
	init();
	initBans(raceType);
}

///Overloaded to get smaller buttons
void vehicleSelectMenu::loadSprite(ButtonTexture ID, ButtonTexture newID, float posx, float posy)
{

	sf::Sprite button;
	button.setTexture(getTexture(ID));
	button.setScale(0.5f, 1.0f);
	button.setPosition(posx, posy);
	menuSprites.insert({ newID, button });
}

void vehicleSelectMenu::init()
{
	menuSprites.clear();
	menuStrings.clear();
	bgColor = sf::Color(30, 30, 30);

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::EXIT, 0, 0);
	setText(ButtonTexture::EXIT, "Back");
	
	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::AI0, WIDTH * 0.0625f, HEIGHT * 0.25f);
	setText(ButtonTexture::AI0, "0 AI");

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::AI1, WIDTH * 0.375f, HEIGHT * 0.25f);
	setText(ButtonTexture::AI1, "1 AI");

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::AI2, WIDTH * 0.6875f, HEIGHT * 0.25f);
	setText(ButtonTexture::AI2, "2 AI");

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::PL0, WIDTH * 0.0625f, HEIGHT * 0.5f);
	setText(ButtonTexture::PL0, "0 player");

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::PL1, WIDTH * 0.375f, HEIGHT * 0.5f);
	setText(ButtonTexture::PL1, "1 player");

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::PL2, WIDTH * 0.6875f, HEIGHT * 0.5f);
	setText(ButtonTexture::PL2, "2 player");

	loadSprite(ButtonTexture::EMPTYBUTTON, ButtonTexture::START, WIDTH * 0.25f, HEIGHT * 0.75f);
	menuSprites[ButtonTexture::START].setScale(1.0f, 1.0f);
	setText(ButtonTexture::START, "Start!");


}

void vehicleSelectMenu::initBans(GameStates raceType)
{
	if (raceType == GameStates::STATE_PRACTICE || raceType == GameStates::STATE_TIMETRIAL)
	{
		setBanned(ButtonTexture::AI1, true);
		setBanned(ButtonTexture::AI2, true);
		setBanned(ButtonTexture::PL0, true);
		setBanned(ButtonTexture::PL2, true);
		setSelected(ButtonTexture::PL1, true);
		setSelected(ButtonTexture::AI0, true);
	}

	else if (raceType == GameStates::STATE_RACE)
	{
		setBanned(ButtonTexture::PL0, true);
		setSelected(ButtonTexture::PL1, true);
		setSelected(ButtonTexture::AI1, true);
	}
	else if (raceType == GameStates::STATE_SPLITSCREEN)
	{
		setBanned(ButtonTexture::PL0, true);
		setBanned(ButtonTexture::PL1, true);
		setSelected(ButtonTexture::PL2, true);
		setSelected(ButtonTexture::AI0, true);
	}

}

GameStates vehicleSelectMenu::eventHandler(sf::Event& event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return)
	{
		pressedFlag = true;
		return GameStates::STATE_TRACK;
	}

	else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
	{
		return GameStates::STATE_RACETYPE;
	}

	else if (event.type == sf::Event::MouseButtonReleased)
	{
		//To correctly get the mouse position when the screen size is changed, we need to use mapPixeltoCoords
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		sf::Vector2f mousepos = window.mapPixelToCoords(mouse);
		return(clickHandler(mousepos.x, mousepos.y, window));
	}

	else
		return(GameStates::STATE_VEHICLESELECT);
}

/// To make the EventHandler a little less cluttered, this handles the case where a mouse button is pressed.
/// Returns an enum class GameStates::STATE_XXXX which determines the game state of the next loop.
/// Check if mouse is currently positioned in the right space (inside the specified buttons) and act accordingly.
GameStates vehicleSelectMenu::clickHandler(float x, float y, sf::RenderWindow& window)
{

	for (auto i : menuSprites)
	{
		if (i.second.getGlobalBounds().contains(x, y) && !getBanned(i.first))
		{
			switch (i.first)
			{
			case ButtonTexture::EXIT:
			{
				return GameStates::STATE_RACETYPE;
			}

			case ButtonTexture::AI0:
			{
				setSelected(ButtonTexture::AI0, true);
				setSelected(ButtonTexture::AI1, false);
				setSelected(ButtonTexture::AI2, false);
				return GameStates::STATE_VEHICLESELECT;
			}

			case ButtonTexture::AI1:
			{
				setSelected(ButtonTexture::AI0, false);
				setSelected(ButtonTexture::AI1, true);
				setSelected(ButtonTexture::AI2, false);
				return GameStates::STATE_VEHICLESELECT;
			}

			case ButtonTexture::AI2:
			{
				setSelected(ButtonTexture::AI0, false);
				setSelected(ButtonTexture::AI1, false);
				setSelected(ButtonTexture::AI2, true);
				return GameStates::STATE_VEHICLESELECT;
			}

			case ButtonTexture::PL0:
			{
				setSelected(ButtonTexture::PL0, true);
				setSelected(ButtonTexture::PL1, false);
				setSelected(ButtonTexture::PL2, false);
				return GameStates::STATE_VEHICLESELECT;
			}

			case ButtonTexture::PL1:
			{
				setSelected(ButtonTexture::PL0, false);
				setSelected(ButtonTexture::PL1, true);
				setSelected(ButtonTexture::PL2, false);
				return GameStates::STATE_VEHICLESELECT;
			}

			case ButtonTexture::PL2:
			{
				setSelected(ButtonTexture::PL0, false);
				setSelected(ButtonTexture::PL1, false);
				setSelected(ButtonTexture::PL2, true);
				return GameStates::STATE_VEHICLESELECT;
			}

			case ButtonTexture::START:
			{
				pressedFlag = true;
				return GameStates::STATE_TRACK;
			}
			default:
				return GameStates::STATE_VEHICLESELECT;
			}
		}
	}
	return GameStates::STATE_VEHICLESELECT;
}
