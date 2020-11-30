#include <random>

#include "weapon.hpp"

Weapon::Weapon(const std::string& textureName) : shape(sf::Vector2f(50, 50))
{
    // Load texture
    std::string filepath = "../images/" + textureName;
    if (!weaponTexture.loadFromFile(filepath)) {
        filepath = "../../images/" + textureName;
        if (!weaponTexture.loadFromFile(filepath)) {
			// Failed to load
        }
    }
    shape.setTexture(&weaponTexture);
    //shape.setFillColor(sf::Color(0, 0, 0));
    setType(WeaponType::UNDEFINED); // define type in subclasses
}

Weapon::Weapon() : shape(sf::Vector2f(50, 50))
{
    
}

Weapon::~Weapon()
{
    
}

Weapon::Weapon(const Weapon& weapon)
{
    
}

void Weapon::setSpawnPoint(const std::vector<structures::Point>& points, std::vector<int>& reserved)
{
    auto index = Weapon::getRandomNumber(0, points.size() - 1);
	int count = 0;
	// Check if index is in reserved list.
	while (std::find(reserved.begin(), reserved.end(), index) != reserved.end()) {
		index = Weapon::getRandomNumber(0, points.size() - 1);
		count++;
		// Prevent infinity loop (if all indexes are reserved for some reason)
		if (count > 20) {
			break;
		}
	}
	std::cout << "COUNT: " << count << std::endl;
    auto p = points[index];
    shape.setPosition(p.x, p.y);
	reserved.push_back(index);
}

/*
void Weapon::useWeapon(float startTime)
{
	std::cout << "A weapon was used" << std::endl;
	if (!isUsing)
	{
		isUsing = true;
		timer = startTime;
	}
}
*/
 

bool Weapon::updateWeapon(float elapsedTime)
{
	if (!isUsing)
		return false;
	// if(durationLimit + timer - elapsedTime <= 0) return false;
	// note that resetting the startTime is not necessary, as it's always set on useWeapon
	return true;
}
void Weapon::updateSound(SoundHandler& soundhandler)
{
	return;
}

sf::RectangleShape& Weapon::getShape()
{
    return shape;
}

void Weapon::drawWeapon(sf::RenderWindow& window) const
{
    window.draw(shape);
}

void Weapon::setType(const Weapon::WeaponType pType)
{
    type = pType;
}

Weapon::WeaponType Weapon::getType() const
{
    return type;
}

// static
// Get random number between low and hight (including them)
int Weapon::getRandomNumber(int low, int high) {
    
    std::random_device rd; // Init seed
    std::mt19937 rng(rd()); // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(low, high);
    return uni(rng);
}





