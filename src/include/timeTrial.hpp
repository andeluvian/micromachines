#ifndef TIME_TRIAL_HH
#define TIME_TRIAL_HH

#include <list>

#include "race.hpp"

/*
 * @Author: Miika Karsimus
 *
 * Class representing time trial race. See function descriptions in the base class.
 *
 * This class has some extra attributes and functions, which are not declared
 * in the base class.
 *
 */


class TimeTrial : public Race
{
public:
    
    // Calls the base class constructor.
    TimeTrial(std::string& xmlfile);
    
    virtual ~TimeTrial();
    
    // Only one vehicle can be added to TimeTrial
    virtual void addVehicle(std::shared_ptr<Vehicle> vehicle) override;
  
    /// Differs a bit from the base class version.
    virtual void update() override;
    
    virtual void checkHits() override;
    
private:
    double lastLapTime = 1000;
    double bestLapTime = 1000;
    std::list<double> lapTimes; // Currently unused
    
    std::string filepath;
    
    // Define where the lap times file is located and set the path
    // to filepath variable. The relative path depends on the used IDE/OS.
    void defineFilePath();
    
    // Save the last lap to the file.
    void saveLastLap();
    
    // Get best lap time from the file and store it to the bestLapTime variable.
    void getBestLapTime();
    
    // Lap times will be save to the file in a separate thread
    // because I/O is a slow operation and our game-loop cannot wait that.
    sf::Thread saveThread;
    
    sf::Text lapTimeText;
    
    // These functions are declared as empty in the base class.
    // Functionality is definen in this class.
    virtual void createLapTimeText() override;
    virtual void updateLapTimeText() override;
    virtual void drawTimeTrialObjects(sf::RenderWindow& window) override;
};


#endif
