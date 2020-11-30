#include <cmath>

#include "vehiclePhysics.hpp"
#include "constants.hpp"

VehiclePhysics::VehiclePhysics(const int& width, const int &height) : width(width), height(height) {

}

void VehiclePhysics::move(const double& x1, const double& y1) {
    
    position.x += x1;
    position.y += y1;
    reset();
}

void VehiclePhysics::reset() {
    
    pos_x0 = position.x;
    pos_y0 = position.y;
    rot0 = rotation;
    vel0 = getVelocity();
    angVel0 = angularVelocity;
    atTopSpeed = false;
    // Note that you don't have restart the clock manually, because it's restarted here
    clock.restart();
}

void VehiclePhysics::setAcceleration(const Vector2D& acc) {
    
    acceleration = acc;
    reset();
}

void VehiclePhysics::setVelocity(const Vector2D& vel) {
    
    velocity = vel;
    reset();
}

void VehiclePhysics::setAngularVelocity(const double& angVel) {
    
    angularVelocity = angVel;
    reset();
}

void VehiclePhysics::setAngularAcceleration(const double& angAcc) {
    
    angularAcceleration = angAcc;
    reset();
}

void VehiclePhysics::stop() {
    
    acceleration = {0, 0};
    velocity = {0, 0};
    angularAcceleration = 0;
    angularVelocity = 0;
    reset();
}

const double VehiclePhysics::getSlidingAngle() const {
    
    Vector2D noseDirection = Vector2D::getUnitVector(rotation);
    return Vector2D::angleBetween(noseDirection, velocity);
}

/*
 * Currently, this is only for testing.
 * works only with y parameter and left wall.
 */
void VehiclePhysics::flipVelocity(const char& axis) {
    
    if (axis == 'x') {
        setVelocity({getVelocity().x, -getVelocity().y});
    } else if (axis == 'y') {
        setVelocity({-getVelocity().x * 0.6, getVelocity().y * 0.6});
    } else
        return;
}

void VehiclePhysics::brake(bool fullBrake) {
    
    // If the velocity is zero, the function does nothing
    if (velocity.x == 0 && velocity.y == 0)
        return;

    // unit vector is reverse (minus sign) relative to velocity vector
    Vector2D unit_vector = -(velocity.getUnitVector());
    // Braking is 3 times faster than acceleration.
    Vector2D brake_vector = unit_vector * ACC * 0.5;

    if (fullBrake) {
        brake_vector = brake_vector * 6;
    }
    setAcceleration(brake_vector);

    isBraking = true;
}

void VehiclePhysics::reverse() {
    
    // Prohibit reverse in this case.
    if (movingState == MovingState::FORWARD)
        return;

    Vector2D unitVector = Vector2D::getUnitVector(rotation);
    setAcceleration(-unitVector * 50);
    movingState = MovingState::BACKWARD;
}

void VehiclePhysics::rotate(double degs) {
    
    // Use setRotation or setAngularVelocity instead.
    double elapsedTime = clock.getElapsedTime().asSeconds();
    if ((elapsedTime - rotateTime > 0) && (elapsedTime - rotateTime < 0.05))
        return;

    bool isBrakingBefore = isBraking;
    //Vector2D oldAcc = getAcceleration();

    rotation += degs;

    // Maintain current speed (direction changes of course)
    Vector2D unitVector = Vector2D::getUnitVector(rotation);
    double scalarSpeed = getVelocity().getLength();
    setVelocity(unitVector * scalarSpeed);

    // Maintain current acceleration (direction changes)
    double scalarAcc = getAcceleration().getLength();
    if (isBrakingBefore) {
        setAcceleration(-unitVector * scalarAcc);
        isBraking = true;
    } else {
        setAcceleration(unitVector * scalarAcc);
    }

    rotateTime = elapsedTime;
}

void VehiclePhysics::setRotation(double degs) {
    
    if (lockedVelocity) {
        return;
    }
    rotation = degs;
    reset();
}

void VehiclePhysics::accelerate() {
    
    Vector2D unit_vector = Vector2D::getUnitVector(rotation);
    if (isMissile) {
        setAcceleration(unit_vector * MISSILE_ACC);
    } else {
        setAcceleration(unit_vector * ACC);
    }
    if (collisionClock.getElapsedTime().asSeconds() > 0.3) {
        setVelocity(unit_vector * getVelocity().getLength());
        lockedVelocity = true;
    }
    isBraking = false;
}

void VehiclePhysics::handleCollision(const Line &line, const Track& track, sf::RectangleShape shape) {
    
    // Note that param. shape is a copy of the vehicle shape and moving param. shape
    // doesn't affect the vehicle shape.
	
	if (std::isnan(line.getStart().x) || std::isnan(line.getStart().y)) {
		return;
	}

    Vector2D oldVelocity = velocity;
    // If velocity is (almost) zero.
    if (velocity.getLength() < 1) {
        if (angularVelocity > 0)
            rotation -= 5;
        else
            rotation += 5;
        angularVelocity = 0;
        reset();
        return;
    }

    // After collision, the velocity is not locked to correspond
    // the nose direction. This state takes few seconds. See fixDirections() function.
    lockedVelocity = false;
    Vector2D startPoint(line.getStart().x, line.getStart().y);
    Vector2D endPoint(line.getEnd().x, line.getEnd().y);
    // Line vector is parallel to the collided line.
    Vector2D lineVector = endPoint - startPoint;

    // Calculate the new velocity. See Vector2D class.
    if (velocity.getLength() > 1)
        velocity.mirror(lineVector);
    // Reduce the amount of speed by 50%.
    velocity = velocity * ELASTIC_COEFF;

    // Move the vehicle a few units towards the normal of the collided line
    // to avoid oscilation and getting inside the collided wall.
    Vector2D normalVector = Vector2D::getMirrorNormal(oldVelocity, lineVector);
    Vector2D moveVector = normalVector.getUnitVector() * 8;
    if (oldVelocity.getLength() > 500)
        moveVector = moveVector * 2;

    // Rotate a bit reverse direction in relation to angular velocity.
    if (angularVelocity > 0)
        rotation -= 5;
    else
        rotation += 5;

    Vector2D oldPosition = position; // Save old position before moving.
    move(moveVector.getX(), moveVector.getY());

    // Adjust shape position to correspond physics position.
    shape.setPosition(getX(), getY());

    // Test if the vehicle is still colliding. This is possible,
    // if the direction of the move vector is wrong.
    if (track.isWallHit(shape)) {
        // Turn move vector around.
        moveVector = -moveVector * 2;
        // Undo the moving (wrong direction) and move to the opposite direction.
        position = oldPosition + moveVector;
        // Now the vehicle doesn't collide with the wall and get inside the wall.
    }

    reset();
    collisionClock.restart();
}

void VehiclePhysics::fixDirections() {
    
    if (resetClock.getElapsedTime().asSeconds() < 0.1) {
        return;
    }
    // Fix velocity direction.
    if (lockedVelocity)
        velocity = Vector2D::getUnitVector(rotation) * velocity.getLength();

    // Fix acceleration direction.
    Vector2D unitVectorRot = Vector2D::getUnitVector(rotation);
    double currAccScalar = getAcceleration().getLength();
    if (isBraking) {
        Vector2D unitVectorVel = getVelocity().getUnitVector();
        // Set direction of braking to correspond the direction of velocity instead of rotation
        setAcceleration(-unitVectorVel * currAccScalar);
    } else
        setAcceleration(unitVectorRot * currAccScalar);
    resetClock.restart();
}

void VehiclePhysics::updateAngle(double &time) {
    
    // Rotation velocity depends on angular acceleration and time.
    angularVelocity = angVel0 + angularAcceleration * time;
    // Rotation depends on angular acceleration and time.
    rotation = rot0 + angVel0 * time + 0.5 * angularAcceleration * (time * time);
    fixDirections();
}

void VehiclePhysics::updatePosition() {
    
    // Position depends on elapsed time
    double time = clock.getElapsedTime().asSeconds();

    // Aliases
    double& x = position.x;
    double& y = position.y;

    // v = v0 + a * t
    velocity.x = vel0.x + acceleration.x * time;
    velocity.y = vel0.y + acceleration.y * time;

    // s = s0 + v0 * t + 0.5 * a * t^2
    x = pos_x0 + vel0.x * time + 0.5 * acceleration.x * (time * time);
    y = pos_y0 + vel0.y * time + 0.5 * acceleration.y * (time * time);

    updateAngle(time);

    /// Vehicle is braking and its speed approaches zero.
    if (isBraking && velocity.getLength() < 30) {
        setAcceleration({0, 0});
        setVelocity({0, 0});
        setAngularVelocity(0);
        isBraking = false;
    }

    // If speed reaches MAX_SPEED
    if (isMissile && velocity.getLength() > MISSILE_SPEED){
        setAcceleration({0, 0});
        velocity.x *= 0.98;
        velocity.y *= 0.98;
    }
    else if (!isMissile && velocity.getLength() > MAX_SPEED) {
        setAcceleration({0, 0});
        velocity.x *= 0.98;
        velocity.y *= 0.98;
    }
}

bool VehiclePhysics::checkVelocitySign(double x1, double y1, double x2, double y2) {
    
    // the product of two numbers is negative if the signs differ
    // i.e. one is positive and the other is negative
    return (x1 * x2 <= 0) && (y1 * y2 <= 0);
}

/*
 * Overloaded operators
 */

std::ostream& operator<<(std::ostream& os, VehiclePhysics& dynObj) {
    os << "pos: " << dynObj.getPosition() << std::endl
            << "vel: " << dynObj.getVelocity() << std::endl
            << "angVel: " << dynObj.getAngularVelocity() << std::endl
            << "acc: " << dynObj.getAcceleration() << std::endl
            << "rot: " << int(dynObj.getRotation()) % 360 << std::endl;
    return os;
}











