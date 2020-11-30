#ifndef GAME_HH
#define GAME_HH

#include <memory>

#include "soundhandler.hpp"
#include "mainmenu.hpp"
#include "race.hpp"
#include "menu.hpp"

/*
 * All content from main function is copied to gameLoop() function.
 *
 * This class could also have methods such as creating main menu, starting a new race, etc...
 *
 */


class Game {
public:

    Game();
    
    ~Game() {}

    // Prevent copying a Game object.
    Game(const Game&) = delete;

    // Run as long as the window is open.
    void run();
    void setNextState(GameStates);
    void createMainMenu();
    void gameLoop();
    
    bool setRaceType(Race::RaceType type);
	void setRaceVehicles();

    sf::Clock gameClock;


private:
    
    void loadBackgroundTexture();

    GameStates cState;
    sf::RenderWindow window;
    //Track track;
    SoundHandler soundHandler;
    //Car car1;
    //Camera camera;
    std::vector<std::unique_ptr<Menu>> menu;
    std::unique_ptr<Race> race; // polymorphism... Can be instance of Race, TimeTrial, etc...
    // This is for limiting AI 
    sf::Clock AIclock;
    
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::string vehicleImage;
    std::string backgroundImage;
    
    sf::Image image; // Image for background
    sf::Vector2f vehicleSize;
    
    sf::Thread updatingThread;
    
    void updateVehicles();
    
    int loopCount = 0; // For testing
	
	sf::Mutex mutex; // For thread safety. See SFML documentation for more information.
	
	bool backgroundLoaded = false;
};


#endif
