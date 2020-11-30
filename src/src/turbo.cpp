#include "turbo.hpp"

Turbo::Turbo(): Weapon("turbo.png")
{
	if (!weaponTexture.loadFromFile("../images/turbo.png")) {
		if (!weaponTexture.loadFromFile("../../images/turbo.png")) {
			std::cerr << "Cannot load turbo icon texture" << std::endl;
		}
	}
	shape.setTexture(&weaponTexture);
	setType(WeaponType::TURBO);
}

Turbo::~Turbo()
{

}

void Turbo::useWeapon(float startTime)
{
	if (isUsing)
		return;

	isUsing = true;
	timer = startTime;
}

void Turbo::updateSound(SoundHandler& soundHandler)
{
	if (isUsing && !soundHandler.isPlaying(SoundType::TURBO))
		soundHandler.playSound(SoundType::TURBO);
}

bool Turbo::updateWeapon(float elapsedTime)
{
	if (!isUsing)
		return false;
	else if (duration + timer - elapsedTime <= 0)
	{
		isUsing = false;
	}
	return true;
}
