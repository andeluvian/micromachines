#include <SFML/Graphics.hpp>

#include "game.hpp"
#include "turbo.hpp"
#include "aicar.hpp"
#include "track.hpp"
#include "car.hpp"
#include "submenu.hpp"
#include "timeTrial.hpp"
#include "splitScreen.hpp"
#include "xmlParser.hpp"


Game::Game() :

cState(GameStates::STATE_MAINMENU),
soundHandler(SoundHandler()),
updatingThread(&Game::updateVehicles, this)

{
	menu.push_back(std::make_unique<MainMenu>());
}

void Game::loadBackgroundTexture()
{
    std::string path = "../images/" + backgroundImage;
    if (!image.loadFromFile(path)) {
        path = "../../images/" + backgroundImage;
        if (!image.loadFromFile(path)) {
            // Failed to load
        }
    }
    
    backgroundTexture.loadFromImage(image);
    backgroundTexture.setRepeated(true);
    
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setTextureRect(sf::IntRect(0, 0, 9000, 6000));
    backgroundSprite.move(-4000, -4000);
}

bool Game::setRaceType(Race::RaceType type)
{
    // Change view divider color if theme is space.
    bool changeColor = false;

	std::string xmlfile;
    if (menu.back()->getSelected(ButtonTexture::AI1)) {
		xmlfile = "Map1.xml";
        vehicleImage = "car2.png";
        backgroundImage = "background4.jpg";
        vehicleSize = {60, 30};
    }
    else if (menu.back()->getSelected(ButtonTexture::AI2)) {
		xmlfile = "Map2.xml";
        vehicleImage = "spaceship2.png";
        backgroundImage = "space1.png";
        vehicleSize = {60, 40};
        changeColor = true;
    }
    else {
		xmlfile = "Map1.xml";
        vehicleImage = "car2.png";
        backgroundImage = "background4.jpg";
        vehicleSize = {60, 30};
    }
	if (!backgroundLoaded) {
		loadBackgroundTexture();
		backgroundLoaded = true;
	}

    // If an error occurs during reading xml file, XMLException is thrown from the XMLParser class,
    // which is called from the Track constructor.
    // Track object is constructed in Race constructor.
    // That is why this try-catch block is here.
    try {
        switch (type) {
            case Race::TimeTrial:
                race = std::make_unique<TimeTrial>(xmlfile);
                break;
                
            case Race::SplitScreen:
                race = std::make_unique<SplitScreen>(xmlfile);
                if (changeColor) {
                    race->getViewDivider().setFillColor(sf::Color(50, 50, 50));
                }
                break;
                
            case Race::Practice:
                race = std::make_unique<Race>(xmlfile);
                break;
                
            case Race::NormalRace:
                race = std::make_unique<Race>(xmlfile);
                break;
                
            default:
                break;
        }
		if (!race->isSplitScreen()) {
			race->getCamera().zoomOut(20);
		}
		
    } // Try block ends here
    catch (XMLException &e) {
        std::cout << "Error occured while reading xml file." << std::endl
        << e.what() << std::endl;

		setNextState(GameStates::STATE_MAINMENU);
		menu.back()->setText(WIDTH * 0.5f, HEIGHT * 0.9f, "ERROR occurred while reading xml file.");
		return false;

    }
	return true;
}

void Game::setRaceVehicles()
{

	if (menu.back()->getSelected(ButtonTexture::AI0))
		std::cout << "No AI vehicles will be added." << std::endl;

	else if (menu.back()->getSelected(ButtonTexture::AI1))
	{
		std::cout << "One AI vehicle will be added." << std::endl;
		race->addAIVehicle(std::make_shared<AICar>(vehicleSize.x, vehicleSize.y, vehicleImage));
	}
	else if (menu.back()->getSelected(ButtonTexture::AI2))
	{
		std::cout << "Two AI vehicles will be added." << std::endl;
		race->addAIVehicle(std::make_shared<AICar>(vehicleSize.x, vehicleSize.y, vehicleImage));
		race->addAIVehicle(std::make_shared<AICar>(vehicleSize.x, vehicleSize.y, vehicleImage));
	}

	if (menu.back()->getSelected(ButtonTexture::PL0))
		std::cout << "No players will be added." << std::endl;
	else if (menu.front()->getSelected(ButtonTexture::PL1))
	{
		std::cout << "One player will be added." << std::endl;
		race->addVehicle(std::make_shared<Car>(vehicleSize.x, vehicleSize.y, vehicleImage));
	}
	else if (menu.back()->getSelected(ButtonTexture::PL2))
	{
		std::cout << "Two players will be added." << std::endl;
		race->addVehicle(std::make_shared<Car>(vehicleSize.x, vehicleSize.y, vehicleImage));
		race->addVehicle(std::make_shared<Car>(vehicleSize.x, vehicleSize.y, vehicleImage));
	}
}

void Game::updateVehicles()
{
    while (window.isOpen()) {
        loopCount++;
        //std::cout << "Loop count: " << loopCount << std::endl;
        // For testing the frequency of the loop
		
		// Block another thread until vehicles are updated, because
		// we shouldn't draw and update the camera while the position of the vehcle
		// is changing.
        mutex.lock(); 
        race->update();
		mutex.unlock(); // Release lock
        race->checkCollisions();
        sf::sleep(sf::milliseconds(2));
        // If CPU load gets too high, we could sleep a bit longer.
        // Otherwise, 2 ms is good or even 1 ms, if the vehicles are still lagging
        // while accelerating or turning.
        // With MacBook pro, this loop runs about 360 times per sec.
        // The frequency of the window loop depends on the monitor's refresh rate and
        // it is about 60-100 /s.
    }
}

void Game::run() {
    // create the window
    window.create(sf::VideoMode(WIDTH, HEIGHT), "Micro Machines");

    window.setVerticalSyncEnabled(true);


    // run the program as long as the window is open
    while (window.isOpen()) {
        // A simple state handler. Runs the main menu loop until Enter is pressed,
        // then switches the cState (in game.hpp) to STATE_TRACK, and on the next cycle we're on the track screen.
        // It's easy to expand this idea to go back and forth from different scenes.
        switch (cState) {
            case GameStates::STATE_MAINMENU:
                createMainMenu();
                break;

			case GameStates::STATE_RACETYPE:
				createMainMenu();
				break;
			
			case GameStates::STATE_VEHICLESELECT:
				createMainMenu();
				break;

            case GameStates::STATE_TRACK:
				menu.clear();
                gameLoop();
                break;

            case GameStates::STATE_EXIT:
                window.close();
                break;

            default:
                std::cout << "Error occurred when checking state." << std::endl;
                window.close();
                break;

        }
    }
}

void Game::setNextState(GameStates nextState)
// A "state machine" in one function... not the best of ideas if I must admit
// Should not be called every tick: only call when the state is actually changing!
{
	if (nextState == GameStates::STATE_EXIT) window.close();

	if (cState == GameStates::STATE_TRACK) return; //can't move back to menu from the track mode

	// first check for temporary states: a little messy but does the job
	switch (nextState) {

	case(GameStates::STATE_PRACTICE): {
		if (!setRaceType(Race::RaceType::Practice))
			return;
		else
		{
			menu.clear();
			menu.push_back(std::make_unique<vehicleSelectMenu>(GameStates::STATE_PRACTICE));
			nextState = GameStates::STATE_VEHICLESELECT;
			break;
		}
	}

	case(GameStates::STATE_TIMETRIAL): {

			if (!setRaceType(Race::RaceType::TimeTrial))
				return;
			else
			{
				menu.clear();
				menu.push_back(std::make_unique<vehicleSelectMenu>(GameStates::STATE_TIMETRIAL));
				nextState = GameStates::STATE_VEHICLESELECT;
				break;
			}
		}

	case(GameStates::STATE_SPLITSCREEN): {
			if (!setRaceType(Race::RaceType::SplitScreen))
				return;
			else
			{
				menu.clear();
				menu.push_back(std::make_unique<vehicleSelectMenu>(GameStates::STATE_SPLITSCREEN));
				nextState = GameStates::STATE_VEHICLESELECT;
				break;
			}
		}

	case(GameStates::STATE_RACE): {
			if (!setRaceType(Race::RaceType::NormalRace))
				return;
			else
			{
				menu.clear();
				menu.push_back(std::make_unique<vehicleSelectMenu>(GameStates::STATE_RACE));
				nextState = GameStates::STATE_VEHICLESELECT;
				break;
			}
		}
	default: break;
	}

	//then check for real states
	if (nextState == cState) return;
	switch (nextState)
	{
	case(GameStates::STATE_TRACK): {

			setRaceVehicles();
			race->initialize(); // Make sure that vehicles are added before calling this.
			race->showCountdown(3);
			AIclock.restart();

			updatingThread.launch();
			race->getRaceClock().restart();

			// We should use std::make_unique instead of new (basically same thing)
			//std::unique_ptr<Turbo> turbo = std::unique_ptr<Turbo>(new Turbo());
			/****************
			 Turbo should be added in Race::handleWeaponEvents like other weapon
			 *****************/
			 //race->getVehicles()[0]->addWeapon(std::move(turbo));
			race->updateWeaponIcons();

			cState = nextState;
			std::cout << "State change success. The screen should change to STATE_TRACK." << std::endl;
			break;
		}
	case(GameStates::STATE_RACETYPE):{
			menu.clear();
			menu.push_back(std::make_unique<modeMenu>());
			cState = nextState;
			break;
		}
	case(GameStates::STATE_VEHICLESELECT): {
			cState = nextState;
			break;
		}
	case(GameStates::STATE_MAINMENU): {
			menu.clear();
			menu.push_back(std::make_unique<MainMenu>());
			cState = nextState;
			break;
		}
	default: break;
	}

}

void Game::createMainMenu() {
    // The main loop for the main menu. Check mainmenu.cpp for more on what happens with the update events.

	GameStates newState = cState;

    sf::Event event;
    // check all the window's events that were triggered since the last iteration of the loop
    while (window.pollEvent(event)) {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();

        else {
			newState = menu.back()->eventHandler(event, window);
        }
    }
	if (newState != cState)
		setNextState(newState);


	if (menu.empty())
		window.close();
	menu.back()->updateSound(soundHandler);
	menu.back()->drawHandler(window);
    window.display();
}

void Game::gameLoop() {


    sf::Event event;
    
    while (window.pollEvent(event)) {
        
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            window.close();

        if (race->isStarted) {
            race->handleEvents(event);
            mutex.lock();
            race->handleWeaponEvents(event);
            mutex.unlock();

			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
			{
				setNextState(GameStates::STATE_EXIT); //press esc to exit track mode and quit the game
			}
			
        }
    }
    
    
    // handle AI updates
    if (race->isStarted) {
        race->handleAI();
    }


    if (!race->isStarted) {
        race->handleCountdownEvents();
    }

    //race->update();
    //race->checkCollisions();
    // The camera follows the first vehicle.
    //race->getCamera().followVehicle(*(race->getVehicles()[0]), Camera::Views::DEFAULT);
	race->updateBullets();
	race->updateTurbo();
    race->updateMissiles();
	race->updateSounds(soundHandler);


    race->updateTexts(); // This should be moved  inside the race class.
    race->checkHits(); // finish line hit and weapon hit 

    // Clear the window before drawing anything.
    window.clear(sf::Color(80, 80, 80));
    
    if (race->isSplitScreen()) {
        
        // First, draw obejcts on the left side of the window.
		mutex.lock();
        race->getCamera().followVehicle(*(race->getVehicles()[0]), Camera::Views::LEFT);
        race->getCamera().setViewToWindow(window, Camera::Views::LEFT);
        window.draw(backgroundSprite);
        race->drawObjects(window);
		
        
        // Change the camera to follow the second vehicle or AI vehicle.
        if (race->getVehicles().size() > 1) {
            race->getCamera().followVehicle(*(race->getVehicles()[1]), Camera::Views::RIGHT);
        }
        else if (race->getAIVehicles().size() > 0) {
            race->getCamera().followVehicle(*(race->getAIVehicles()[0]), Camera::Views::RIGHT);
        }
        
        // Draw objects on the right side of the window.
        race->getCamera().setViewToWindow(window, Camera::Views::RIGHT);
        window.draw(backgroundSprite);
        race->drawObjects(window);
		mutex.unlock();
    }
    else {
        // If not split screen, use the full size default view, which follows
        // the first vehicle.
		mutex.lock(); // Block another thread until everything is drawn.
		race->getCamera().followVehicle(*(race->getVehicles()[0]), Camera::Views::DEFAULT);
        race->getCamera().setViewToWindow(window, Camera::Views::DEFAULT);
        window.draw(backgroundSprite);
        race->drawObjects(window);
		mutex.unlock(); // Release lock
        
    }
    // Draw static objects (e.g. texts) with using the static view, which never moves.
    race->getCamera().setViewToWindow(window, Camera::Views::STATIC);
    race->drawStaticObjects(window);

    // end the current frame
    window.display();
}
