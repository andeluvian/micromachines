/* 
 * File:   aivehicle.hpp
 * Author: Martin Vidjeskog
 *
 * Created on December 2, 2017, 3:42 PM
 * 
 * Class representing a race track.
 */

#ifndef TRACK_HH
#define TRACK_HH

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>

#include "structures.hpp"
#include "line.hpp"
#include "weapon.hpp"
#include "obstacle.hpp"

class Track {
public:

    /// Constructor.
    Track(const std::string &xmlfile);

    /// Draw the race track. 
    void drawTrack(sf::RenderWindow &window);

    /// Check if player is on finish line.
    bool isOnFinishLine(const sf::Shape &player) const;

    /// Check if object is colliding with wall.
    bool isWallHit(const sf::Shape &object) const;

    /// Set the finish line. 
    void setFinishLine(const sf::RectangleShape& finishLine);

    /// Set walls for track. 
    void setWalls(const std::vector<sf::RectangleShape> &newWalls);

    /// Set spawnpoints for track. 
    void setSpawnpoints(const std::vector<structures::Point> &newPoints);

    /// Set checkpoints for track. 
    void setCheckpoints(const std::vector<sf::RectangleShape> &newCheckpoints);

    /// Set weapon spawnpoints for track. 
    void setWeaponPoints(const std::vector<structures::Point> &newPoints);

    /// Get finish line.
    const sf::RectangleShape& getFinishLine() const;

    /// Get Line that object is colliding at. If no collision,
    /// a line with NAN-points is returned. 
    const Line getCrashedLine(const sf::Shape &object);

    /// Get wall pieces.
    const std::vector<sf::RectangleShape>& getWalls() const;

    /// Get player spawnpoints.
    const std::vector<structures::Point>& getSpawnpoints() const;

    /// Get checkpoints.
    const std::vector<sf::RectangleShape>& getCheckpoints() const;
    
    /// Get weapon spawnpoints.
    const std::vector<structures::Point>& getWeaponPoints() const;
    
    /// Test if rect hits the checkpoint with index.
    bool isCheckpointHit(const sf::RectangleShape& rect, const int& index) const;
    
    /// Test if player is hitting oilsplat.
    bool isOilSplatHit(const sf::Shape &player);
    
    /// Return the index of the collided weapon. If no collision, return -1.
    int handleWeaponHit(const sf::RectangleShape &vehicleShape);
    
    /// Remove weapon with index from the weapons container and return unique_ptr
    /// to the removed weapon (ownership is moved). Then it can be moved to Vehicle.
    std::unique_ptr<Weapon> pickWeapon(const int index);
    
    /// Add one weapon to weapons container.
    void addWeapon(std::unique_ptr<Weapon> weapon);
    
    /// Get all weapons at track.
    std::vector<std::unique_ptr<Weapon>>& getWeapons();
    
    /// Spawn a new weapon when certain time has elapsed.
    void spawnWeapon();
    

private:

    sf::RectangleShape finishLine; // finish line
    std::vector<sf::RectangleShape> walls; // walls
    std::vector<structures::Point> spawnPoints; // spawnpoints
    std::vector<sf::RectangleShape> checkPoints; // points for lap progress and AI
    std::vector<structures::Point> weaponPoints; // spawn points for weapons
    std::vector<Obstacle> obstacles;
    sf::Texture textureOil;
    sf::Texture textureFinish; // texture for finish line
    sf::Texture textureWall; // texture for walls
    
    // Weapons on the track are stored in this container
    // and weapons owned by vehicles are stored in similar container
    // in the Vehicle class. When you pick up a weapon, you should move it from
    // this container to Vehicle's container. Use std::move to move the ownership.
    std::vector<std::unique_ptr<Weapon>> weapons;
    
    // vector which contains indexes of used spawnpoints.
    std::vector<int> reservedSpawnpoints;
    
    // time for weapon spawning
    sf::Clock spawnClock;
    
    // missiles spawned
    int missilesSpawned = 0;
	
	// Time for next weapon spawn.
	int nextSpawnTime = 2;
};




#endif





