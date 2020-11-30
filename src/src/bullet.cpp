#include "bullet.hpp"
#include "constants.hpp"


Bullet::Bullet(const int& width, const int& height)
: shape(sf::Vector2f(width, height)), physics(width, height)
{
    shape.setFillColor(sf::Color(50, 205, 50));
    /*
    if (!bulletTexture.loadFromFile("../images/bullet1.jpg")) {
        if (!bulletTexture.loadFromFile("../../images/bullet1.jpg")) {
            std::cerr << "Failed to load bullet texture." << std::endl;
        }
    }
    shape.setTexture(&bulletTexture);
     */
}


VehiclePhysics& Bullet::getPhysics()
{
    return physics;
}

sf::RectangleShape& Bullet::getShape()
{
    return shape;
}

void Bullet::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

// static
void Bullet::outOfBounds(std::deque<Bullet>& bullets)
{
    for (auto it = bullets.begin(); it != bullets.end(); it++) {
        if (!it->isFlying) {
            continue;
        }
        double x = it->getPhysics().getX();
        double y = it->getPhysics().getY();
        if (x > 4000 || x < -4000 || y > 4000 || y < -4000) {
            it = bullets.erase(it);
            if (it == bullets.end()) {
                break;
            }
        }
    }
}


void Bullet::update()
{
    physics.updatePosition();
    sync();
}

void Bullet::launch(const VehiclePhysics& vPhys)
{
    double posx = vPhys.getPosition().getX();
    double posy = vPhys.getPosition().getY();
    getPhysics().move(posx, posy);
    update();
    getPhysics().lockedVelocity = false;
    // Set initial rotation to correspond the rotation of the vehicle.
    getPhysics().setRotation(vPhys.getRotation());
    // Calculate lauching direction and speed
    Vector2D unitVector = Vector2D::getUnitVector(vPhys.getRotation());
    // Direction of unitVector is already correct but length is wrong
    // Calculate scalar speed: Initial speed of the vehicle + BULLET_SPEED
    double speed = vPhys.getVelocity().getLength() + BULLET_SPEED;
    // Set final velocity
    getPhysics().setVelocity(unitVector * speed);
    isFlying = true;
}


// Private

void Bullet::sync()
{
    shape.setPosition(physics.getPosition().getX(), physics.getPosition().getY());
    shape.setRotation(physics.getRotation());
}

void Bullet::setOwnerID(int id)
{
	ownerID = id;
}

const int& Bullet::getOwnerID() const
{
	return ownerID;
}