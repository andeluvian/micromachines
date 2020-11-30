#ifndef BULLET_HPP
#define BULLET_HPP

#include <iostream>
#include <vector>
#include <deque>

#include "vehiclePhysics.hpp"

/*
 * Class representing a single bullet.
 * Utilizes vehicle physics to simulate bullet moving.
 *
 * Call launch() from the Race class when a proper keyboard event occurs.
 */


class Bullet
{
public:
    
    Bullet(const int& width, const int& height);
    ~Bullet() = default;
    Bullet(const Bullet&) = default;
    
    VehiclePhysics& getPhysics();
    sf::RectangleShape& getShape();
    
    /// Update position and sync shape with physics.
    void update();
    
    void draw(sf::RenderWindow& window);
    
    /// Set initial velocity.
    void launch(const VehiclePhysics& vPhys);
    
    /// If bullet has moved far away from the track, remove it from the
    /// container. Iterate through the whole bullets container.
    static void outOfBounds(std::deque<Bullet>& bullets);
  
    /// Tell if bullet is lauched. Set this true in the lauch() function.
    /// If this is false, the bullet will never be drawn or updated.
    bool isFlying = false;
	
	void setOwnerID(int id);
	
	const int &getOwnerID() const;
    
    
    
private:
    sf::RectangleShape shape;
    VehiclePhysics physics;
    sf::Texture bulletTexture; // Currently unused.
    
    /// Assign shape position and rotation to have same values as physics
    /// Called from update() function.
    void sync();
	
	// ID of the vehicle that owns this bullet.
	int ownerID;
    
};



#endif
