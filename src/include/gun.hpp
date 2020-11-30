#ifndef LASER_GUN_HPP
#define LASER_GUN_HPP

#include "weapon.hpp"

class Gun : public Weapon
{
public:
    Gun();
    virtual ~Gun();
    
    virtual void useWeapon(float time) override;
    
    virtual void useWeapon() override;
    
	virtual void updateSound(SoundHandler&) override;
    // void getBullet
    
    
private:
    
    
    
    
};

#endif
