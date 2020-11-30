/* 
 * File:   aivehicle.hpp
 * Author: Martin Vidjeskog
 *
 * Created on December 2, 2017, 3:42 PM
 * 
 * Class representing AI-controlled vehicle.
 */

#ifndef AIVEHICLE_HPP
#define AIVEHICLE_HPP

#include "vehicle.hpp"
#include "track.hpp"

class AIVehicle : public Vehicle {
public:

    /// Constructor 1.
    AIVehicle(const int& width, const int& height);

    /// Constructor 2.
    AIVehicle(const int& width, const int& height, const std::string& textureName);

    /// Destructor.
    virtual ~AIVehicle() = default;

    /// Function to move AI.
    void moveAI(Track& track);

    /// Function for getting checkpoint that AI should aim at.
    static const sf::RectangleShape& getTargetCheckpoint(const Track& track, int& index);

    /// Function for getting middle point of sf::Rectangleshape.
    static structures::Point getMiddlePoint(const sf::RectangleShape& target);

    /// Function for getting AI driving direction.
    static Vector2D getAIDirection(sf::RectangleShape& shape);

    /// Function for getting distance between two points.
    static int calculateDistanceBetweenPoints(const structures::Point& point1,
            const structures::Point& point2);

private:

    // index value for the next checkpoint
    int targetCheckpoint;

    // acceleration flag
    bool accelerating = false;

};

#endif /* AIVEHICLE_HPP */

