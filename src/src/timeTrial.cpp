#include <SFML/System.hpp>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>

#include "timeTrial.hpp"
#include "constants.hpp"

TimeTrial::TimeTrial(std::string& xmlfile) : Race(xmlfile), saveThread(&TimeTrial::saveLastLap, this)
{
    // Call Race() constructor
    // Construct the thread object
    createLapTimeText();
    raceType = RaceType::TimeTrial;
    defineFilePath();
    getBestLapTime();
    
}

TimeTrial::~TimeTrial()
{
    
}



void TimeTrial::addVehicle(std::shared_ptr<Vehicle> vehicle) {
    
    /* In final version, this will be uncommented.
    if (vehicles.size() >= 1) {
        // Only one vehicle can be added to time trial.
        return;
    }
     */
    
    vehicle->getShape().setFillColor(sf::Color(50, 50, 180));
    vehicles.push_back(vehicle);
}

void TimeTrial::defineFilePath()
{
    // laps.txt should be in the same directory as xml files.
    // If laps.txt doesn't exist, it will be created in saveLastLap().
    std::string path = "../xml/Map1.xml";
    std::ifstream in(path);
    if (in) {
        filepath = "../xml/laps.txt";
    }
    else {
        path = "../../xml/Map1.xml";
        in.open(path);
        if (in.good()) {
            filepath = "../../xml/laps.txt";
        }
        else { // Not found. Lap are not saved and the best lap is not loaded.
            filepath = "";
        }
    }
    
}

void TimeTrial::saveLastLap()
{
    
    std::ofstream out(filepath, std::ofstream::app);
    if (!out) {
        out.clear();
        out.open(filepath); // create file if it doesn't exist
        if (!out) {
            std::cout << "Failed to open file laps.txt" << std::endl;
            return;
        }
    }
    
    out << lastLapTime << std::endl;

    
}

void TimeTrial::getBestLapTime()
{
    std::ifstream in(filepath);
    if (!in) {
        std::cout << "Failed to open laps.txt." << std::endl;
        return;
    }
    std::vector<double> lapTimes;
    std::string buffer;
    while (std::getline(in, buffer)) {
        try {
            lapTimes.push_back(std::stod(buffer)); //stod converts string to double
        }
        // Cannot convert string to int. Continue to the next i
        catch (std::invalid_argument &e) {
            std::cerr << "Invalid laps.txt file." << std::endl
            << e.what() << std::endl;
        }
    }
    // Get the smallest value (best lap time)
    if (lapTimes.size() > 0) {
        auto it = std::min_element(lapTimes.begin(), lapTimes.end());
        if (it != lapTimes.end()) {
            bestLapTime = *it;
        }
    }
}

void TimeTrial::update() {
    //camera.setViewToWindow(sf::RenderWindow &window)
    for (auto v : vehicles) {
        v->update();
        if (v->isDestroyed()) {
            v->getPhysics().stop();
        }
    }
    
    // Test obstacle and weapon hits
    for (auto v : vehicles) {
        if (track.isOilSplatHit(v->getShape())) {
            std::cout << "OILSPLAT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
            v->getPhysics().setAngularVelocity(100);
            // Allow sliding until the driver releases throttle
            v->getPhysics().lockedVelocity = false;
        }
    }
}


void TimeTrial::createLapTimeText()
{
    lapTimeText.setFont(textFont);
    lapTimeText.setCharacterSize(32);
#ifdef _WIN32
    lapTimeText.setFillColor(TEXT_COLOR);
#else
    lapTimeText.setColor(TEXT_COLOR);
#endif
    lapTimeText.setString("Placeholder text");
    lapTimeText.setPosition(WIDTH / 2, HEIGHT - 70);
}

void TimeTrial::updateLapTimeText()
{
    std::stringstream ss;
    ss << "Last lap: ";
    if (lastLapTime < 1000) {
        // Use two digits precision.
        ss << std::fixed << std::setprecision(2) << lastLapTime << " s";
        lapTimeText.setString(ss.str());
    }
    else {
        ss << "-";
    }
    ss << "   Best lap: ";
    if (bestLapTime < 1000) {
        ss << std::fixed << std::setprecision(2) << bestLapTime << " s";
    }
    else {
        ss << "-";
    }
    lapTimeText.setString(ss.str());
}

void TimeTrial::drawTimeTrialObjects(sf::RenderWindow& window)
{
    // Draw the latest lap time
    window.draw(lapTimeText);
}

void TimeTrial::checkHits()
{
    /* This overridden virtual function is simpler that Race class version.
     * No weapon hits are tested. Only one vehicle is handled. Clock is restarted after each lap.
     */
    // Test finish line hit.
    auto &vehicle = vehicles[0]; // local alias
    if (track.isOnFinishLine(vehicle->getShape())) {
        if (vehicle->allCheckpointsPassed(track)) {
            lastLapTime = raceClock.restart().asSeconds();
            if (lastLapTime < bestLapTime) {
                bestLapTime = lastLapTime;
            }
            vehicle->increaseLapCount();
            lapsDriven++;
            vehicle->visitedCheckPoints = 0;
            saveThread.launch();
        }
    }
    // Test oil splat hit.
    if (track.isOilSplatHit(vehicle->getShape())) {
        int num = Weapon::getRandomNumber(0, 1);
        if (num == 0) {
            vehicle->getPhysics().setAngularVelocity(100);
        }
        else {
            vehicle->getPhysics().setAngularVelocity(-100);
        }
        vehicle->getPhysics().lockedVelocity = false;
    }
    
}





