#include <random>
#include "obstacle.hpp"


Obstacle::Obstacle(const std::string& textureName) : oil(sf::Vector2f(70, 70))//Constructor for the Obstacles class
{

}

void Obstacle::setSpawnPoint(const std::vector<structures::Point> points)
{
    
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, points.size() - 1); // guaranteed unbiased
    
    auto index = uni(rng);
    auto p = points[index];
    
    oil.setPosition(p.x, p.y);
    
    
    
}

sf::RectangleShape& Obstacle::getShape()
{
    return oil;
}

void Obstacle::drawObstacle(sf::RenderWindow& window) const
{
    window.draw(oil);
}
