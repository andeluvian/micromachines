#include <iomanip>
#include <sstream>

#include "constants.hpp"
#include "race.hpp"
#include "missile.hpp"

Race::Race(std::string& xmlfile) : camera(WIDTH, HEIGHT), track(xmlfile),
viewDivider(sf::Vector2f(10, 2 * HEIGHT)) {
    //viewDivider is constructed here, but not used in this class.
    // Only SplitScreen uses it.
    if (!flagTexture.loadFromFile("../images/flag2.png")) {
        if (!flagTexture.loadFromFile("../../images/flag2.png")) {
            std::cerr << "Failed to load flag texture" << std::endl;
        }
    }
    flagShape.setTexture(flagTexture);
    flagShape.setPosition(400, 500);
    flagShape.setScale(0.25, 0.25);

    raceType = RaceType::NormalRace;
}

Race::~Race() {
    //bullets.clear();
}

std::vector<std::shared_ptr<Vehicle>> Race::getVehicles() const {
    return vehicles;
}

std::vector<std::shared_ptr<AIVehicle>> Race::getAIVehicles() const {
    return aivehicles;
}

Camera& Race::getCamera() {
    return camera;
}

sf::Clock& Race::getRaceClock() {
    return raceClock;
}

void Race::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    vehicle->getShape().setFillColor(sf::Color(50, 50, 180));
    vehicles.push_back(vehicle);
}

void Race::addAIVehicle(std::shared_ptr<AIVehicle> aivehicle) {
    aivehicle->getShape().setFillColor(sf::Color(180, 50, 50));
    aivehicles.push_back(aivehicle);
}

void Race::initialize() {

    /*** Make sure that the vehicles are added before calling this. ****/

    // Set colors to vehicles
    int j = 0;
    for (auto v : vehicles) {
        v->getShape().setFillColor(colors[j]);
        j++;
    }
    for (auto v : aivehicles) {
        v->getShape().setFillColor(colors[j]);
        j++;
    }

    int index = 0;
    for (decltype(vehicles.size()) i = 0; i != vehicles.size(); i++) {
        structures::Point spawnpoint = track.getSpawnpoints()[i];
        vehicles[i]->getPhysics().move(spawnpoint.x, spawnpoint.y);
        index++;
    }
    for (decltype(aivehicles.size()) i = 0; i != aivehicles.size(); i++) {
        structures::Point spawnpoint = track.getSpawnpoints()[index];
        aivehicles[i]->getPhysics().move(spawnpoint.x, spawnpoint.y);
        index++;
    }

    // Create all texts here.
    createTexts();
    createPlayerStatus();

    // Set vehicle places and set unique ID to each vehicle.
    int ID = 1;
    for (auto v : vehicles) {
        racePlaces.push_back(v);
        v->setID(ID);
        ID++;
    }
    for (auto v : aivehicles) {
        racePlaces.push_back(v);
        v->setID(ID);
        ID++;
    }
    fixPlaces();
}

void Race::drawObjects(sf::RenderWindow &window) {
    track.drawTrack(window);
    // We use auto because we don't know the actual type of vehicle (Car, Boat, etc.)
    for (auto v : vehicles) {
        if (!v->isDestroyed()) {
            v->drawVehicle(window);
        } else { // If the vehicle is destroyed, draw the greatest "animation" you have ever seen!
            v->getExplosionSprite().setPosition(v->getPhysics().getX() - 100, v->getPhysics().getY() - 100);
            v->getExplosionSprite().setScale(sf::Vector2f(0.1f, 0.1f)); // Original image is too big
            window.draw(v->getExplosionSprite());
        }
        // Draw bullets owned by vehicles
        for (Bullet &b : v->getBullets()) {
            if (b.isFlying) {
                b.draw(window);
            }
        }
        // Draw missiles owned by vehicles
        if (!v->getWeapons().empty() && v->getWeapons()[0]->getMissile() != NULL) {
            v->getWeapons()[0]->getMissile()->draw(window);
        }
    }

    // AI Vehicles
    for (auto v : aivehicles) {
        if (!v->isDestroyed()) {
            v->drawVehicle(window);
        } else {
            v->getExplosionSprite().setPosition(v->getPhysics().getX() - 100, v->getPhysics().getY() - 100);
            v->getExplosionSprite().setScale(sf::Vector2f(0.1f, 0.1f)); // Original image is too big
            window.draw(v->getExplosionSprite());
        }
    }

    //window.draw(clockText);
    window.draw(countdownText);
}

void Race::drawStaticObjects(sf::RenderWindow &window) {
    window.draw(clockText);
    window.draw(lapsText);
    //window.draw(playerStatusText);
    drawPlayerWeapons(window);
    for (auto &t : playerTexts) {
        window.draw(t);
    }
    for (auto &t : botTexts) {
        window.draw(t);
    }
    for (sf::RectangleShape rect : helmetIcons) {
        window.draw(rect);
    }
    drawTimeTrialObjects(window); // Does nothing if caller is Race-class.
    drawViewDivider(window);
    if (isEnd) {
        window.draw(winnerText);
        window.draw(flagShape);
        if (flagShape.getPosition().y > 0) {
            flagShape.move(0, -2);
        }
    }
}

void Race::drawPlayerWeapons(sf::RenderWindow& window) {
    for (auto &v : vehicles) {
        for (auto &w : v->getWeapons()) {
            if (w->getType() != Weapon::WeaponType::UNDEFINED) {
                w->drawWeapon(window);
            }
        }
    }
}

void Race::createTexts() {

    // Load the font, which is used in all texts.
    if (!textFont.loadFromFile("../images/fonts/open-sans/OpenSans-Bold.ttf")) {
        if (!textFont.loadFromFile("../../images/fonts/open-sans/OpenSans-Bold.ttf")) {
            std::cerr << "Cannot load fonts." << std::endl;
        }
    }
    // Clock text, which shows the elapsed time.
    clockText.setFont(textFont);
    clockText.setCharacterSize(36);
    clockText.setPosition(WIDTH - 120, 5);
    clockText.setString("0.00");

    // Lap text, which shows lap progression.
    lapsText.setFont(textFont);
    lapsText.setCharacterSize(36);
    lapsText.setPosition(WIDTH - 400, 5);
    lapsText.setString("0");

#ifdef _WIN32
    clockText.setFillColor(sf::Color(200, 200, 200));
    lapsText.setFillColor(sf::Color(200, 200, 200));
#else
    clockText.setColor(sf::Color(200, 200, 200));
    lapsText.setColor(sf::Color(200, 200, 200));
#endif

    // Set position of the view divider. It's drawn only in Split Screen class.
    viewDivider.setPosition(WIDTH / 2 - 5, 0);

    // Create winner text
    winnerText.setFont(textFont);
    winnerText.setCharacterSize(58);
    winnerText.setPosition(400, 550);
#ifdef _WIN32
    winnerText.setFillColor(sf::Color(255, 153, 51));
#else
    winnerText.setColor(sf::Color(255, 153, 51));
#endif
}

void Race::createPlayerStatus() {

    // Create helmet icons
    if (!helmetTexture.loadFromFile("../images/helmet_icon3.png")) {
        if (!helmetTexture.loadFromFile("../../images/helmet_icon3.png")) {
            std::cerr << "Cannot load helmet textures." << std::endl;
        }
    }
    // Helmet icons for players
    for (decltype(vehicles.size()) i = 0; i != vehicles.size(); i++) {
        sf::RectangleShape rect(sf::Vector2f(30, 30));
        rect.setTexture(&helmetTexture);
        if (i > 3) throw std::logic_error("Only 4 default helmet colors set.");
        rect.setFillColor(helmetIconColors[i]);
        rect.setPosition(5, 5 + i * 30);
        helmetIcons.push_back(rect);
    }
    // Helmet icons for bots.
    for (decltype(aivehicles.size()) i = 0; i != aivehicles.size(); i++) {
        sf::RectangleShape rect(sf::Vector2f(30, 30));
        rect.setTexture(&helmetTexture);
        if (i > 3) throw std::logic_error("Only 4 default helmet colors set.");
        rect.setFillColor(helmetIconColors[i]);
        rect.setPosition(5, 80 + i * 30);
        helmetIcons.push_back(rect);
    }
    // Set text formatting for both player and bot texts.
    sf::Text text;
    text.setFont(textFont);
    text.setCharacterSize(20);
    //text.setPosition(40, 8);
    playerTexts.insert(playerTexts.begin(), vehicles.size(), text);
    int i = 0;
    int j = 0;
    for (sf::Text& txt : playerTexts) {
        txt.setString("Default"); // Changed in updateTexts
        txt.setPosition(40, 5 + i * 30);
        txt.setColor(colors[j]);
        i++;
        j++;
    }
    i = 0;
    botTexts.insert(botTexts.begin(), aivehicles.size(), text);
    for (sf::Text& txt : botTexts) {
        txt.setString("Default");
        txt.setPosition(40, 70 + 5 + i * 30);
        txt.setColor(colors[j]);
        i++;
        j++;
    }
}

void Race::startRace() {
    raceClock.restart();
    isStarted = true;
}

void Race::checkCollisions() {
    /*****
     NOTE! This function is called from a separate thread in Game class.
     ******/
    for (auto& v : vehicles) {
        if (track.isWallHit(v->getShape())) {
            v->getPhysics().handleCollision(track.getCrashedLine(v->getShape()), track, v->getShape());
            v->damageVehicle(10); // Just testing. Not final.
            collisionFlag = true;
        }
        // Check checkpoint "collision"
        if (track.isCheckpointHit(v->getShape(), v->visitedCheckPoints)) {
            // v->visitedCheckPoints in an INTEGER value, which tells how many checkpoints are passed
            // during this lap
            v->lastCheckpoint = &(track.getCheckpoints()[v->visitedCheckPoints]);
            // now lastCheckpoint points to the last passed checkpoint (sf::RectangleShape in Track-class)
            v->visitedCheckPoints++; // Increase integer value by one
            fixPlaces();
        }
    }
    // check AI collisions
    for (auto& v : aivehicles) {
        if (track.isWallHit(v->getShape())) {
            v->getPhysics().handleCollision(track.getCrashedLine(v->getShape()), track, v->getShape());
            v->damageVehicle(10);
            collisionFlag = true;
        }
        // Check checkpoint "collision"
        if (track.isCheckpointHit(v->getShape(), v->visitedCheckPoints)) {
            // v->visitedCheckPoints in an INTEGER value, which tells how many checkpoints are passed
            // during this lap
            v->lastCheckpoint = &(track.getCheckpoints()[v->visitedCheckPoints]);
            // now lastCheckpoint points to the last passed checkpoint (sf::RectangleShape in Track-class)
            v->visitedCheckPoints++; // Increase integer value by one
            fixPlaces();
        }
    }
}

void Race::handleEvents(sf::Event &event) {
    // this part moves player controlled vehicle
    if (!isSplitScreen()) {
        for (auto v : vehicles) {
            // Test if vehicle is destroyed
            if (!v->isDestroyed()) {
                v->handleEvents(event, Vehicle::EventKeys::LEFT);
            }
        }
    } else {
        // Control the first car with W, A, S and D.
        vehicles[0]->handleEvents(event, Vehicle::EventKeys::LEFT);
        // Control the second car with Up, down, left, right.
        // Check that there are more than 1 vehicle.
        if (vehicles.size() > 1) {
            vehicles[1]->handleEvents(event, Vehicle::EventKeys::RIGHT);
        }
    }
    camera.handleEvents(event);
}

void Race::handleAI() {
    // this one moves AI
    for (auto v : aivehicles) {
        v->moveAI(track);
    }
}

void Race::update() {
    /*****
     NOTE! This function is called from a separate thread in Game class
     about 300 times per sec.
     ******/
    //camera.setViewToWindow(sf::RenderWindow &window)
    for (auto v : vehicles) {
        v->update();
        if (v->isDestroyed()) {
            v->getPhysics().stop();
        }
    }

    // update AIVehicles
    for (auto v : aivehicles) {
        v->update();
        if (v->isDestroyed()) {
            v->getPhysics().stop();
        }
    }
}

void Race::updateTexts() {

    // Update the content of the clock text.
    if (isStarted) {
        std::stringstream ss;
        // Convert float to string. Use precision of two decimal digits.
        ss << std::fixed << std::setprecision(2) << raceClock.getElapsedTime().asSeconds();
        clockText.setString(ss.str());
    }

    // Update also the laps text.
    std::stringstream ss2;
    ss2 << "Lap: " << lapsDriven;
    // If race type is time trial, only driven laps are shown.
    // e.g. Lap: 5 instead of Lap: 5/8
    if (raceType != RaceType::TimeTrial) {
        ss2 << "/" << totalLaps;
    }

    lapsText.setString(ss2.str());


    // Update player texts in the corner
    ss2.str("");
    int playerCount = vehicles.size() + aivehicles.size();
    for (unsigned i = 0; i != vehicles.size(); i++) {
        ss2 << "Player " << vehicles[i]->getID() << ": HP: " << vehicles[i]->getHP()
                << "  (" << vehicles[i]->getRacePlace() << "/" << playerCount << ")";
        playerTexts[i].setString(ss2.str());
        ss2.str("");
    }
    ss2.str("");
    for (unsigned i = 0; i != aivehicles.size(); i++) {
        ss2 << "AI " << aivehicles[i]->getID() << ": HP: " << aivehicles[i]->getHP()
                << "  (" << aivehicles[i]->getRacePlace() << "/" << playerCount << ")";
        botTexts[i].setString(ss2.str());
        ss2.str("");
    }




    /* Polymorphism...
     * In case of Race-class, this function does nothing
     * In case of TimeTrial-class, this function shows the last lap time.
     */
    updateLapTimeText();
}

void Race::updateWeaponIcons() {
    int i = 0;
    for (auto &vehicle : vehicles) {
        int j = 0;
        for (auto &weapon : vehicle->getWeapons()) {
            weapon->getShape().setPosition(270 + (j * 30), 5 + (i * 25));
            weapon->getShape().setScale(0.5f, 0.5f); // Decrease icon size by 50%
            j++;
        }
        i++;
    }
}

void Race::showCountdown(const int count) {
    countdownText.setFont(textFont);
    countdownText.setString("3");
    countdownText.setCharacterSize(200);
    //countdownText.setPosition(camera.getCenter().x, camera.getCenter().y);
#ifdef _WIN32
    countdownText.setFillColor(sf::Color(210, 50, 135));
#else
    countdownText.setColor(sf::Color(255, 0, 0));
#endif
}

void Race::handleCountdownEvents() {

    if (isEnd) {
        return;
    }
	
    double elapsedTime = raceClock.getElapsedTime().asSeconds();
    if (elapsedTime < 1)
        countdownText.setString("3");
    else if (elapsedTime < 2)
        countdownText.setString("2");
    else if (elapsedTime < 3)
        countdownText.setString("1");
    else if (elapsedTime < 4) {
        countdownText.setString("");
        startRace();
    } else {
        countdownText.setString("");
	}
	
}

void Race::handleWeaponEvents(sf::Event &event) {
    if (event.type == sf::Event::KeyPressed) {

        if (event.key.code == sf::Keyboard::E) {
            // Vehicle 1 uses weapon
            if (vehicles[0]->getWeapons().size() > 0) {
                // Get reference to the FIRST weapon (unique_ptr).
                // You can use the weapons in the same order they are picked from the track.
                auto &weapon = vehicles[0]->getWeapons().front();

                if (weapon->getType() == Weapon::WeaponType::GUN) {
                    std::cout << "USING GUN!!!" << std::endl;
                    weapon->useWeapon(); //added this to get sound to play
                    if (vehicles[0]->shoot() <= 0) {
                        // Running out of ammo -> remove weapon from the vehicle
                        vehicles[0]->removeWeapon();
                    }
                } else if (weapon->getType() == Weapon::WeaponType::TURBO) {
                    // Use turbo
                    weapon->useWeapon(getRaceClock().getElapsedTime().asSeconds());
                } else if (weapon->getType() == Weapon::WeaponType::MISSILE) {
                    // Use missile here
                    weapon->initializeControls(vehicles[0].get(), this);
                    weapon->useWeapon();
                }
            }
        } else if (event.key.code == sf::Keyboard::L) {
            // Vehicle 2 uses weapon
            if (vehicles.size() > 1) {
                // Get reference to the FIRST weapon (unique_ptr)
                if (vehicles[1]->getWeapons().size() > 0) {
                    auto &weapon = vehicles[1]->getWeapons().front();
                    if (weapon->getType() == Weapon::WeaponType::GUN) {
                        weapon->useWeapon(); //added this to get sound to play
                        if (vehicles[1]->shoot() <= 0) {
                            vehicles[1]->removeWeapon();
                        }
                    } else if (weapon->getType() == Weapon::WeaponType::TURBO) {
                        // Use turbo
                        weapon->useWeapon(getRaceClock().getElapsedTime().asSeconds());

                    } else if (weapon->getType() == Weapon::WeaponType::MISSILE) {
                        weapon->initializeControls(vehicles[1].get(), this);
                        weapon->useWeapon();
                    }
                }
            }
        }
    }

}

void Race::updateSounds(SoundHandler& soundHandler) {
    if (!soundHandler.isPlaying(SoundType::ENGINE) && !getVehicles()[0]->isDestroyed())
        soundHandler.playAsLoop(SoundType::ENGINE);
    if (soundHandler.isPlaying(SoundType::ENGINE) && getVehicles()[0]->isDestroyed())
        soundHandler.stopSound(SoundType::ENGINE);

    soundHandler.revvingSound(SoundType::ENGINE, getVehicles()[0]->getPhysics().getVelocity().getLength() / 500);

    for (auto &v : vehicles) {
        if (v->destroyedFlag) {
            soundHandler.playSound(SoundType::EXPLOSION);
            v->destroyedFlag = false;
        }
        for (auto &w : v->getWeapons()) {
            w->updateSound(soundHandler); //maybe these could've been flags as well...
        }
    }
    if (pickedUpFlag) {
        soundHandler.playSound(SoundType::PICKUP);
        pickedUpFlag = false;
    }
    if (collisionFlag) {
        soundHandler.playRandomPitch(SoundType::COLLISION, 0.2f);
        collisionFlag = false;
    }
}

void Race::updateTurbo() {
    for (auto &v : vehicles) {
        for (unsigned i = 0; i < v->getWeapons().size(); i++) {
            auto &weapon = v->getWeapons()[i];
            if (weapon->getType() == Weapon::WeaponType::TURBO) {
                //Note: updateWeapon(...) does nothing if the weapon is not set as used with useWeapon().
                //It also automatically sets used = false if the timer ran out.

                if (weapon->updateWeapon(getRaceClock().getElapsedTime().asSeconds())) {
                    double cSpeed = v->getPhysics().getVelocity().getLength();
                    if (cSpeed > 1 && cSpeed < (MAX_SPEED * 1.2))
                        v->getPhysics().setVelocity(v->getPhysics().getVelocity() * 1.05); //go fast

                    if (!weapon->updateWeapon(getRaceClock().getElapsedTime().asSeconds()))
                        v->removeWeapon();

                }

            }
        }
    }

}

void Race::updateBullets() {
    for (auto &v : vehicles) {

        for (Bullet &b : v->getBullets()) {
            if (!b.isFlying) {
                continue;
            }
            b.update();

            // Test if bullet hit with AI vehicles
            for (auto &ai : aivehicles) {
                if (ai->getShape().getGlobalBounds().intersects(b.getShape().getGlobalBounds())) {
                    ai->damageVehicle(40);
                }
            }
            // Test if bullet hits other vehicles
            for (auto &v2 : vehicles) {
                // Cannot hit the owner of the bullet
                if (v2->getID() != v->getID()) {
                    if (v2->getShape().getGlobalBounds().intersects(b.getShape().getGlobalBounds())) {
                        v2->damageVehicle(40);
                    }
                }
            }
        }
        Bullet::outOfBounds(v->getBullets());
    }
}

void Race::updateMissiles() {
    for (auto &v : vehicles) {
        bool removeWeapon = false;
        for (auto &w : v->getWeapons()) {
            if (w->getType() == Weapon::WeaponType::MISSILE) {
                if (w->getMissile() != NULL) {
                    if (w->getMissile()->isFlying)
                        w->getMissile()->moveMissile(*this);

                    // if missile is destroyed, remove it from player
                    if (w->getMissile()->isDestroyed)
                        removeWeapon = true;
                }
            }
        }
        if (removeWeapon)
            v->removeWeapon();
    }
}

void Race::checkHits() {
    // Test finish line hit.
    for (auto v : vehicles) {
        if (track.isOnFinishLine(v->getShape())) {
            if (v->allCheckpointsPassed(track)) {
                if (raceType == RaceType::TimeTrial) {
                    raceClock.restart();
                }
                v->increaseLapCount();
                // If the leader passes the finish line
                // increase total laps count
                std::cout << "Player place: " << v->getRacePlace() << std::endl;
                if (v->getRacePlace() == 1) {
                    lapsDriven++;
                    if (lapsDriven > totalLaps && !isEnd) {
                        endRace();
                    }
                }
                v->visitedCheckPoints = 0;
            }
        }
    }
    // Test finish line hit.
    for (auto v : aivehicles) {
        if (track.isOnFinishLine(v->getShape())) {
            if (v->allCheckpointsPassed(track)) {
                v->increaseLapCount();
                // If the leader passes the finish line
                // increase total laps count
                std::cout << "Bot place: " << v->getRacePlace() << std::endl;
                if (v->getRacePlace() == 1) {
                    lapsDriven++;
                }
                if (lapsDriven > totalLaps && !isEnd) {
                    endRace();
                }
                v->visitedCheckPoints = 0;
            }
        }
    }

    // Test obstacle and weapon hits
    for (auto v : vehicles) {
        if (track.isOilSplatHit(v->getShape())) {
            int num = Weapon::getRandomNumber(0, 1);
            if (num == 0) {
                v->getPhysics().setAngularVelocity(100);
            } else {
                v->getPhysics().setAngularVelocity(-100);
            }
            // Allow sliding until the driver releases throttle
            v->getPhysics().lockedVelocity = false;
        }
        int index = track.handleWeaponHit(v->getShape());
        if (index != -1) {
            std::cout << "WEAPON HIT!" << std::endl;
            // Move weapon (unique_ptr) from track to vehicle.
            // std::move is used to move the ownership.
            // Create temporary variable for weapon
            auto &w = track.getWeapons()[index];
            // Pick weapon if the vehicle has not this type of weapon yet.
            // i.e. a vehicle can have only 1 weapon of each type.
            if (!v->hasWeapon(w->getType())) {
                pickedUpFlag = true; //flag so that sound will play
                v->addWeapon(track.pickWeapon(index));
                // If picked weapon is Gun, add ammunition
                if (v->getWeapons()[v->getWeapons().size() - 1]->getType() == Weapon::WeaponType::GUN) {
                    v->addBullets(20); // number of bullets to be added
                }
            }
            updateWeaponIcons();

        }
    }

}

void Race::fixPlaces() {
    // Sort racePlaces vector with std::sort and lambda.
    std::sort(racePlaces.begin(), racePlaces.end(), [](const std::shared_ptr<Vehicle> v1, const std::shared_ptr<Vehicle> v2)
            -> bool { // This lambda body defines, how the items are sorted.
                if (v1->getLaps() > v2->getLaps()) {
                    return true;
                } else if (v1->getLaps() < v2->getLaps()) {
                    return false;
                } else { // Laps are equal. Compare visited checkpoints (int).
                    if (v1->visitedCheckPoints >= v2->visitedCheckPoints) {
                        return true;
                    }
                }
                return false;
            });

    // Now racePlaces vector is sorted. Set race places to vehicles.
    int i = 1;
    for (auto v : racePlaces) {
        v->setRacePlace(i);
        i++;
    }
}

void Race::endRace() {
    
    isEnd = true;
    isStarted = false;
    // Stop all vehicles.
    for (auto v : vehicles) {
        v->getPhysics().brake();
    }
    for (auto v : aivehicles) {
        v->getPhysics().brake();
    }
    updateWinnerText();
}

void Race::updateWinnerText() {
    // Get winner ID from the vehicle.
    const int winnerID = racePlaces[0]->getID();
    std::stringstream ss;
    ss << "WINNER: Player " << winnerID;
    winnerText.setString(ss.str());
}

