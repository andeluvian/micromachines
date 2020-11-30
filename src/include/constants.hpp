#ifndef CONSTANTS_HH
#define CONSTANTS_HH

#include <SFML/Graphics.hpp>

/*
 * Define constant quantities in this header. Include this in the file, where constants are needed.
 * Always use const keyword to ensure that value is not changed.
 */

// Window properties
const int HEIGHT = 800;
const int WIDTH = 1200;

// Laps, player status, etc.
const sf::Color TEXT_COLOR(230, 230, 230);


// quantities
const double ACC = 200;
const double MISSILE_ACC = 800;
const double MAX_SPEED = 1300;
const double ANG_VEL = 150;

// For AI
const double MISSILE_SPEED = 2600;
const double AI_ANG_VEL = 300; // Bot cheats a bit
const double MISSILE_ANG_VEL = 600;

const double BULLET_SPEED = 3000;

// Weapon damages
const double MISSILE_DMG = 10;

// Coefficient, which tells how many percent a vehicle maintains of its velocity
// after a collision. Default: 0.5 (50%).
const double ELASTIC_COEFF = 0.5;

//enum class for gamestates: use GameStates::STATE_SOMETHING to access
enum class GameStates
{
	STATE_EXIT,
	STATE_MAINMENU,
	STATE_RACETYPE,
	STATE_VEHICLESELECT,

	STATE_PRACTICE,			//these
	STATE_SPLITSCREEN,		//four
	STATE_TIMETRIAL,		//are 
	STATE_RACE,				//not real states, but change to state_track after the correct type was loaded

	STATE_TRACK,
};

#endif

