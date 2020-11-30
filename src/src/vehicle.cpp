#include <sstream>

#include "vehicle.hpp"
#include "constants.hpp"



Vehicle::Vehicle(const int& width, const int& height)
: physics({width, height}), shape(sf::Vector2f(width, height))
{
    // Initialize both physics and shape with same width and height
    createStatusText();
    // Origin is the center point of the car. The car is rotated about that point.
    shape.setOrigin(width / 2, height / 2);
}

Vehicle::Vehicle(const int& width, const int& height, const std::string& textureName)
: physics({width, height}), shape(sf::Vector2f(width, height))
{
    // Initialize both physics and shape with same width and height
    createStatusText();
    // Origin is the center point of the car. The car is rotated about that point.
    shape.setOrigin(width / 2, height / 2);
    
    std::string PATH = "../images/" + textureName;
    
    if (!vehicleTexture.loadFromFile(PATH)) {
        PATH = "../../images/" + textureName;
        if (!vehicleTexture.loadFromFile(PATH)) {
            std::cerr << "Cannot load vehicle texture " << textureName << std::endl;
            shape.setFillColor(sf::Color(102, 102, 255));
        }
    }
    if (!explosionTexture.loadFromFile("../images/boom4.png")) {
        if (!explosionTexture.loadFromFile("../../images/boom4.png")) {
            std::cerr << "Cannot load explosion texture. " << textureName << std::endl;
            //shape.setFillColor(sf::Color(255, 50, 50));
        }
    }
    explosionSprite.setTexture(explosionTexture);
    shape.setTexture(&vehicleTexture);
    
}


Vehicle::~Vehicle()
{
    
}

VehiclePhysics& Vehicle::getPhysics()
{
    return physics;
}


sf::RectangleShape& Vehicle::getShape()
{
    return shape;
}

std::vector<std::unique_ptr<Weapon>>& Vehicle::getWeapons()
{
    return weapons;
}

std::deque<Bullet>& Vehicle::getBullets()
{
    return bullets;
}

sf::Sprite& Vehicle::getExplosionSprite()
{
    return explosionSprite;
}

void Vehicle::addWeapon(std::unique_ptr<Weapon> weapon)
{
    // Move ownership of the weapon to the weapons container
    if (weapon->getType() == Weapon::WeaponType::GUN) {
        
    }
    weapons.push_back(std::move(weapon));
}

void Vehicle::removeWeapon()
{
    if (weapons.size() == 0) return; // no weapons in the container
    weapons.erase(weapons.begin());
}

void Vehicle::addBullets(const int num)
{
    if (num < 0) return; // Invalid num
    bullets.insert(bullets.begin(), num, Bullet(30, 10));
	for (auto &b : bullets) {
		b.setOwnerID(ID); // ID of this vehicle
	}
}

void Vehicle::removeBullet()
{
    bullets.pop_front();
}

void Vehicle::update()
{
    physics.updatePosition();
    sync();
}

int Vehicle::shoot()
{
    int count = 0;
    for (Bullet &bullet : bullets) {
        count++;
        if (!bullet.isFlying) {
            bullet.launch(physics);
            break;
        }
    }
    return bullets.size() - count;
}

bool Vehicle::hasWeapon(Weapon::WeaponType type)
{
    // Use lambda to find a weapon with the specific type
    auto it = std::find_if(weapons.begin(), weapons.end(), [type](const std::unique_ptr<Weapon>& w) {
        return w->getType() == type;
    });
    // If not found, it has value weapons.end()
    return it != weapons.end();
}


const int& Vehicle::getHP() const
{
    return HP;
}

bool Vehicle::isDestroyed() const
{
    return HP == 0;
}

const int& Vehicle::damageVehicle(const int &amount)
{
    HP -= amount;
    if (HP <= 0) {
        HP = 0;
		destroyedFlag = true; //for sound
    }
    return HP;
}
void Vehicle::drawVehicle(sf::RenderWindow &window)
{
    window.draw(shape);
}

void Vehicle::drawText(sf::RenderWindow& window)
{
    std::stringstream ss;
    ss << getPhysics(); // Utilize overloaded << operator of Vector2D class
    statusText.setString(ss.str());
    window.draw(statusText);
}

void Vehicle::createStatusText()
{
    // Relative path to font directory depends on building environment
    // i.e. in which directory the executable is run.
    
    // Try this directory first
    if (!textFont.loadFromFile("../../images/fonts/open-sans/OpenSans-Regular.ttf"))
    {
        // If not found, try this directory
        if (!textFont.loadFromFile("../images/fonts/open-sans/OpenSans-Regular.ttf")) {
            std::cout << "Failed to load fonts." << std::endl;
            // Continue executing without fonts and text
        }
    }
    
    statusText.setFont(textFont);
    statusText.setString("[0.0, 0.0]");
    statusText.setCharacterSize(16);
#ifdef _WIN32
    statusText.setFillColor(sf::Color::Blue);
#else
    statusText.setColor(sf::Color::Blue);
#endif
    statusText.setPosition(WIDTH - 170, 10); // Upper right corner of the window
}

void Vehicle::handleEvents(sf::Event& event, EventKeys keys)
{
    // Control with the keys W, A, S and D.
    if (keys == EventKeys::LEFT) {
        if (event.type == sf::Event::KeyPressed) {
            
            // Accelerate
            if (event.key.code == sf::Keyboard::W && !isAccelerating) {
                physics.accelerate();
                isAccelerating = true;
            }
            
            // Heavy brake
            else if (event.key.code == sf::Keyboard::S) {
                physics.brake(true);
                isAccelerating = false;
            }
            else if (event.key.code == sf::Keyboard::R) {
                physics.reverse();
            }
            
            // Turn right
            else if (event.key.code == sf::Keyboard::D)
                physics.setAngularVelocity(ANG_VEL);
            // Turn left
            else if (event.key.code == sf::Keyboard::A)
                physics.setAngularVelocity(-ANG_VEL);
            
            
        }
        else if (event.type == sf::Event::KeyReleased) {
            
            // Stop accelerating and use light brakes (simulate friction)
            if (event.key.code == sf::Keyboard::W) {
                physics.brake(false); // false = light braking
                isAccelerating = false;
            }
            else if (event.key.code == sf::Keyboard::D) {
                if (physics.getAngularVelocity() > 0)
                    physics.setAngularVelocity(0);
            }
            else if (event.key.code == sf::Keyboard::A) {
                if (physics.getAngularVelocity() < 0)
                    physics.setAngularVelocity(0);
            }
        }
    }
    // Control with the keys up, down, right and left.
    else if (keys == EventKeys::RIGHT) {
        if (event.type == sf::Event::KeyPressed) {
            
            // Accelerate
            if (event.key.code == sf::Keyboard::Up && !isAccelerating) {
                physics.accelerate();
                isAccelerating = true;
            }
            
            // Heavy brake
            else if (event.key.code == sf::Keyboard::Down) {
                physics.brake(true);
                isAccelerating = false;
            }
            
            // Turn right
            else if (event.key.code == sf::Keyboard::Right)
                physics.setAngularVelocity(ANG_VEL);
            // Turn left
            else if (event.key.code == sf::Keyboard::Left)
                physics.setAngularVelocity(-ANG_VEL);
        }
        else if (event.type == sf::Event::KeyReleased) {
            
            // Stop accelerating and use light brakes (simulate friction)
            if (event.key.code == sf::Keyboard::Up) {
                physics.brake(false); // false = light braking
                isAccelerating = false;
            }
            else if (event.key.code == sf::Keyboard::Right) {
                if (physics.getAngularVelocity() > 0)
                    physics.setAngularVelocity(0);
            }
            else if (event.key.code == sf::Keyboard::Left) {
                if (physics.getAngularVelocity() < 0)
                    physics.setAngularVelocity(0);
            }
        }
    }
}


void Vehicle::sync()
{
    shape.setPosition(physics.getPosition().getX(), physics.getPosition().getY());
    shape.setRotation(physics.getRotation());
}


void Vehicle::drawVector(sf::RenderWindow& window)
{
    Vector2D startPoint = getPhysics().getPosition();
    Vector2D endPoint = startPoint + getPhysics().getAcceleration();
    
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(startPoint.getX(), startPoint.getY())),
        sf::Vertex(sf::Vector2f(endPoint.getX(), endPoint.getY()))
    };
    line[0].color = sf::Color(130, 75, 210);
    line[1].color = sf::Color(130, 75, 210);
    window.draw(line, 2, sf::Lines);
}

void Vehicle::updateTextPosition(const sf::Vector2f& centerPoint)
{
    
    int xPos = centerPoint.x + WIDTH / 4 + 100;
    int yPos = centerPoint.y - HEIGHT / 4 - 200;
    statusText.setPosition(xPos, yPos);
   
}

bool Vehicle::allCheckpointsPassed(const Track& track) const
{
    return visitedCheckPoints >= track.getCheckpoints().size();
}

void Vehicle::setRacePlace(const int p)
{
    if (p >= 0) {
        racePlace = p;
    }
}

void Vehicle::setID(const int id)
{
    ID = id;
}

const int& Vehicle::getID() const
{
    return ID;
}






