/* 
 * File:   missileLauncher.hpp
 * Author: Martin Vidjeskog
 *
 * Created on December 10, 2017, 11:39 AM
 * 
 * Class representing a missile launcher (Weapon).
 */

#ifndef MISSILELAUNCHER_HPP
#define MISSILELAUNCHER_HPP

#include "weapon.hpp"
#include "missile.hpp"

class MissileLauncher : public Weapon {
public:

    /// Constructor.
    MissileLauncher();

    /// Destructor.
    virtual ~MissileLauncher() = default;

    /// Trigger the use of the weapon.
    /// Does nothing if the weapon is used already.
    virtual void useWeapon() override;

    /// NOT IMPLEMENTED! Trigger the use of the weapon.
    /// Does nothing if the weapon is used already.
    virtual void useWeapon(float startTime) override {
    };

    // Initialize weapon control systems. Race has information of both 
    // vehicles as track. Both of them are needed.
    void initializeControls(Vehicle* owner, Race* race) override;

    /// Get missile. If there is no missile, returns NULL.
    virtual Missile* getMissile() override {
        return &missile;
    }


	virtual void updateSound(SoundHandler&) override;

private:

    // Ammunition for launcher
    Missile missile;

    // owner of missile 
    Vehicle* owner = NULL;

    // This is for guiding system. Missile needs information about environment
    // other players in order to be able to annihilate them.
    Race* race = NULL;


};



#endif /* MISSILELAUNCHER_HPP */

