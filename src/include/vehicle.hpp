#ifndef VEHICLE_HH
#define VEHICLE_HH

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <deque>

#include "vehiclePhysics.hpp"
#include "weapon.hpp"
#include "bullet.hpp"

/**
 * Author: Miika Karsimus
 *
 * This class represents a single vehicle. Physical properties are definen in the VehiclePhysics class
 * while this class contains general features of vehicles.
 * Vehicle is drawn as a rectangle shape.
 *
 *
 *
 *----------------------------------------------------
 * VEHICLE CONTROLS:
 *
 * First player:
 * W: accelerate
 * S: Brake
 * D: Rotate right
 * A: Rotate left
 *
 * Second player:
 * Up: accelerate
 * Down: brake
 * Left: turn left
 * Right: turn right
 *
 * Vehicle also brakes automatically if throttle is not used (simulates friction) but in that case,
 * deceleration is slower.
 *
 *----------------------------------------------------
 *
 * Move vehicle towars its front by calling physics.accelerate() function.
 *
 */


class Vehicle
{
public:
    
    /*
     * LEFT = You control the vehicle with the keys W, A, S and D
     * RIGHT = You control the vehicle with the keys Up, left, down and right.
     */
    enum EventKeys {
        LEFT = 0,
        RIGHT = 1
    };
    
    /// Pass height and width as parameter.
    Vehicle(const int& width, const int& height);
    
    /// Pass height, width and file name for texture image.
    Vehicle(const int& width, const int& height, const std::string& textureName);
    
    virtual ~Vehicle();
    
    /// Use these to access private variables physics and shape.
    VehiclePhysics& getPhysics();
    sf::RectangleShape& getShape();
    
    // Must return a reference because unique_ptr cannot be copied.
    std::vector<std::unique_ptr<Weapon>>& getWeapons();
    
    // Access to bullets container.
    std::deque<Bullet>& getBullets();
    
    /// Add num bullets to Vehicle's bullets container.
    void addBullets(const int num);
    
    /// Remove one bullet from the front of the queue.
    /// I.e. the item which is added first, will be removed.
    void removeBullet();
    
    /// Add a weapon to the weapons container.
    /// Ownership of the object is moved to the container.
    void addWeapon(std::unique_ptr<Weapon> weapon);
    
    /// Remove the first (the oldest) weapon from the contaner.
    void removeWeapon();
    
    sf::Sprite& getExplosionSprite();
    
    const int& getHP() const;
    
    /// Returns true if HP is 0.
    bool isDestroyed() const;
    
    /// Updates position, velocity, angular velocity, acceleration based on elapsed time.
    /// Should be called, for example, 30 or 60 time per second.
    /// The best practice is to call this every time the window is updated.
    virtual void update();
    
    /// Shoot with gun. Return remaining ammunition or 0 if no ammo left.
    int shoot();
    
    /// Reduce HP by amount. Return remaining HP after damage.
    const int& damageVehicle(const int &amount);
    
    /// Draw the vehicle on the window.
    void drawVehicle(sf::RenderWindow& window);
    
    /// Draw acc, vel, rot etc. onto screen as string form.
    /// Not used in the final version.
    void drawText(sf::RenderWindow& window);
    
    /// Initialize drawable status text. See cpp file.
    void createStatusText();
    
    /// Handle keyboard events to control car moving.
    void handleEvents(sf::Event& event, EventKeys keys);
    
    /// Draw acceleration vector to window. It is just a thin line.
    void drawVector(sf::RenderWindow& window);
    
    // Status text is not used in the final version.
    void updateTextPosition(const sf::Vector2f& centerPoint);
    
    // Set unique ID to Vehicle.
    void setID(const int id);
    
    const int& getID() const;
    
    /// Tell if the vehicle has this type of weapon. 
    bool hasWeapon(Weapon::WeaponType type);
    
    // The number of checkpoints visited during this lap.
    unsigned int visitedCheckPoints = 0;
    
    // The last visited checkpoint. nullptr by default
    // Note that this is NOT a const pointer, but the object this points to is const.
    // i.e. non-const pointer to const value.
    // So you can assing this to point another object later.
    const sf::RectangleShape *lastCheckpoint = nullptr;
    
    /// Tell if all the checkpoints are passed during this lap.
    /// Call this, when the vehicle passes the finnish line.
    bool allCheckpointsPassed(const Track& track) const;
    
    const int& getRacePlace() const { return racePlace; }
    
    // Set race place. Leader has place 1.
    void setRacePlace(const int p);
    
    const int& getLaps() const { return laps; }
    
    void increaseLapCount() { laps++; }
    
	// For sound handling. Set true when hp drops to 0.
	bool destroyedFlag = false;


protected:
    // Derived classes can use these directly.
    VehiclePhysics physics;
    sf::RectangleShape shape;
    
private:
    
    // Amount of HP. Not used yet.
    int HP = 150;
    
    // ID of the vehicle. Each vehicle should have an unique ID.
    int ID;
    
    sf::Text statusText;
    // Texture object must be alive as long as texture is used.
    // That is why this is a member variable. 
    sf::Texture vehicleTexture;
    sf::Texture explosionTexture; // Booom!!!!
    sf::Sprite explosionSprite;
    
    
    // Note that font object must be alive as long as the text with this font
    // is drawn onto screen. Otherwise segmentation fault will arise.
    // This is the reason that font is a member variable instead of a local variable
    // in createStatusText() function.
    sf::Font textFont; // will be loaded from file
    
    // For event handling
    bool isAccelerating = false;
    
    // Sync member variable shape to have same values (position, rotatation, etc..) as physics
    // Check the method in cpp file to understand what it actually does
    // Note that this is a private function
    virtual void sync();
    
    /// Container to store all picked weapons.
    std::vector<std::unique_ptr<Weapon>> weapons;
    
    // Deque is used because bullets are always added to the end of the container
    // and removed from the beginning of the container.
    // I.e. push back, pop front
    std::deque<Bullet> bullets;
    
    // Place in the race. -1 by default;
    int racePlace = -1;
    
    // How many laps are driven.
    int laps = 0;
    

    
};




#endif
