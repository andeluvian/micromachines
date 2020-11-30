/* 
 * File:   AICar.hpp
 * Author: Martin Vidjeskog
 *
 * Created on December 1, 2017, 7:20 PM
 * 
 * Class represents car with AI.
 */

#ifndef AICAR_HPP
#define AICAR_HPP

#include "aivehicle.hpp"

class AICar : public AIVehicle {
public:

    /// Constructor 1
    AICar(const int& width, const int& height);

    /// Constructor 2
    AICar(const int& width, const int& height, const std::string& textureName);

    /// Destructor
    virtual ~AICar() = default;

private:


};


#endif /* AICAR_HPP */

