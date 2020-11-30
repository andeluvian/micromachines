#include "gun.hpp"
#include "structures.hpp"


Gun::Gun() : Weapon()
{
    // Load texture
    if (!weaponTexture.loadFromFile("../images/laser_gun1.png")) {
        if (!weaponTexture.loadFromFile("../../images/laser_gun1.png")) {
            std::cerr << "Cannot load gun icon texture" << std::endl;
        }
    }
    shape.setTexture(&weaponTexture);
    shape.setFillColor(sf::Color(255, 0, 0));
    setType(WeaponType::GUN);
}

Gun::~Gun()
{
    
}

void Gun::useWeapon(float startTime)
{
	isUsing = true;
    std::cout << "Gun used" << std::endl;
}


void Gun::useWeapon()
{
    std::cout << "Gun used" << std::endl;
	isUsing = true;
    //bullet.getShape().setPosition(300, 300);
}

void Gun::updateSound(SoundHandler& soundHandler)
{
	if (isUsing)
	{
		soundHandler.playSound(SoundType::GUNSHOT);
		isUsing = false;
	}
}