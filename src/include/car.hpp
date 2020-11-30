#ifndef CAR_HH
#define CAR_HH

#include "vehicle.hpp"

/*
 * Class representing a car. Inherits Vehicle. See the Vehicle class.
 *
 */

class Car : public Vehicle
{
public:
    
    Car(const int& width, const int& height);
    
    Car(const int& width, const int& height, const std::string& textureName);
    
    virtual ~Car();
    

    
private:
    
    
 
};



#endif
