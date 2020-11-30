#include "missileLauncher.hpp"

MissileLauncher::MissileLauncher() : Weapon(), missile(60, 30) {
    // Load texture
    if (!weaponTexture.loadFromFile("../images/missileLaunch.png")) {
        if (!weaponTexture.loadFromFile("../../images/missileLaunch.png")) {
            std::cerr << "Cannot load missile launcher icon texture" << std::endl;
            shape.setFillColor(sf::Color(255, 0, 0));
        }
    }
    shape.setTexture(&weaponTexture);
    setType(WeaponType::MISSILE);
}

void MissileLauncher::useWeapon() {
    // check that control system is initialized
    if (this->owner != NULL && this->race != NULL) {
        if (!isUsing && !isUsed) {
            isUsing = true;
            isUsed = true;
            missile.launchMissile(owner->getID(), owner->visitedCheckPoints,
                    owner->getPhysics());
        }
    } else {
        std::cout << "Missile control system, was not initialized" << std::endl;
    }
}

void MissileLauncher::initializeControls(Vehicle* owner, Race * race) {
    this->owner = owner;
    this->race = race;
}
void MissileLauncher::updateSound(SoundHandler& soundHandler) {
	if (isUsing) {
		if (!soundHandler.isPlaying(SoundType::ROCKET))
			soundHandler.playSound(SoundType::ROCKET);
	}
}

