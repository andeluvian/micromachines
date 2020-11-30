/* 
 * File:   missile.hpp
 * Author: Martin Vidjeskog
 *
 * Created on December 10, 2017, 11:46 AM
 * 
 * Class representing a missile (ammunition for missile launcher).
 */

#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "vehiclePhysics.hpp"
#include "race.hpp"

class Missile : VehiclePhysics {
public:
    
    /// Constructor for Missile-object.
    Missile(const int& width, const int& height);
    
    /// Destructor.
    ~Missile() = default;
    
    /// Copy constructor.
    Missile(const Missile&) = default;
    
    /// Get missile shape.
    sf::RectangleShape& getShape();
    
    /// Draw missile.
    void draw(sf::RenderWindow& window);
    
    /// Method for launching the missile.
    void launchMissile(const int& ownerID, const int& checkpoint, 
            const VehiclePhysics& vPhys);
    
    /// Function for moving the missile.
    void moveMissile(const Race& race);
    
    /// Tell if missile is launched. Set this true in the lauch() function.
    /// If this is false, the missile will never be drawn or updated.
    bool isFlying = false;
    
    /// Tel if missile is destroyed.
    bool isDestroyed = false;
    
private:
    
    // Assign shape position and rotation to have same values as physics.
    void sync();
    
    // search and return rectangleshape corresponding to vehicle
    sf::RectangleShape searchShapeByVehicleID(const int& vehicleID, const Race& race) const;
    
    // do some damage for vehicle with given ID
    int missileHitVehicle(const int& vehicleID, const Race& race);
    
    // missile shape
    sf::RectangleShape shape;
    
    // texture for missile
    sf::Texture missileTexture; 
    
    // target checkpoint index
    int targetCheckpoint;
    
    // owned ID
    int ownerID;
    
    // check flag for acceleration
    bool accelerating = false;
    
    // clock that counts how long the missile
    // has been flying. after 10 s -> self destroy
    sf::Clock fuelTime;
    
};

#endif /* MISSILE_HPP */

