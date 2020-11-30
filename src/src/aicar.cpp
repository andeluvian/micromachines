#include <sstream>

#include "aicar.hpp"

AICar::AICar(const int& width, const int& height) : AIVehicle(width, height) {

}

AICar::AICar(const int& width, const int& height, const std::string& textureName)
: AIVehicle(width, height, textureName) {

}



