#ifndef WEAPON_HH
#define WEAPON_HH

#include "SFML/Graphics.hpp"
#include "soundhandler.hpp"
#include "structures.hpp"

/*
 * Abstract base class for weapons.
 *
 * This class contains virtual functions, which can be overridden in derived classes.
 *
 *
 */

// forward declarations
class Missile;
class Vehicle;
class Race;

class Weapon {
public:
    
    enum WeaponType {
        UNDEFINED = 0,
        TURBO = 1,
        GUN = 2,
        MISSILE = 3
    };
    
    Weapon();
    // Pass texture name as parameter.
    Weapon(const std::string& textureName);
    virtual ~Weapon();
    Weapon(const Weapon&);

    /// Trigger the use of the weapon, gets the current game clock (as seconds) as parameter.
    /// Does nothing if the weapon is used already.
    /// NOTE. Converted to pure virtual function.
    /// Define in derived classes.
    virtual void useWeapon(float startTime) = 0;
    
    virtual void useWeapon() { }
    
    virtual void useWeapon(sf::RectangleShape& bulletShape) { }

    /// Updates the lifetime of the weapon: if enough time has passed (duration of weapon),
    /// returns false. If the weapon is still alive, returns true.
    virtual bool updateWeapon(float elapsedTime);

    /// Plays sounds as long as the weapon is alive.
    virtual void updateSound(SoundHandler&);

    /// Draw weapon icon on the screen.
    virtual void drawWeapon(sf::RenderWindow& window) const;
    
    /// Sen position of the shape to a random spawn point
    /// which is read from the Track class.
    virtual void setSpawnPoint(const std::vector<structures::Point>& points, std::vector<int>& reserved);
    sf::RectangleShape& getShape();

    /// Check if weapon is already used.
    const bool& isWeaponUsed() const {return isUsed;}
    
    /// Param. pType is enum WeaponType.
    void setType(const WeaponType pType);
    
    WeaponType getType() const;
    
    /// Get random number between low and high (including them).
    static int getRandomNumber(int low, int high);
    
    /// Initialize weapon controls
    virtual void initializeControls(Vehicle* vehicle, Race *race){};
    
    /// Get Missile
    virtual Missile* getMissile() {return NULL;}
    
protected:
    // Weapon icon that appears on the track.
    sf::RectangleShape shape;
    sf::Texture weaponTexture;
    

    bool isUsing = false;
    bool isUsed = false;
    float timer;
    
    WeaponType type = UNDEFINED;
};


#endif
