#ifndef TURBO_HH
#define TURBO_HH

#include "soundhandler.hpp"
#include "weapon.hpp"


/** The turbo is a weapon that exponentially increases the speed of the using vehicle.
*	It always lasts for 3 seconds and the vehicle will be almost impossible to control during this time.
*/
class Turbo : public Weapon
{
public:
	Turbo();
	virtual ~Turbo();
	virtual void useWeapon(float startTime) override;
	virtual bool updateWeapon(float elapsedTime) override;
	void updateSound(SoundHandler& soundhandler) override;
    
private:
	const float duration = 3;
};

#endif
