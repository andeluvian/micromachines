#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <SFML/Graphics.hpp>

#include "structures.hpp"


/*
Base class for obstacles

 
Obstacles:
Oil splat: Will cause car to slide around the circuit for few seconds.
 
 */


class Obstacle
{
public:
    Obstacle() {}
    Obstacle(const std::string& textureName);
    void setSpawnPoint(const std::vector<structures::Point> points);
    sf::RectangleShape& getShape();
    void drawObstacle(sf::RenderWindow& window) const;
    
    

private:
    sf::Texture obstacleTexture;
     sf::RectangleShape oil;

};
#endif
