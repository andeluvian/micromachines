#include <cmath>

#include "missile.hpp"
#include "aivehicle.hpp"
#include "constants.hpp"
#include "polygon.hpp"

Missile::Missile(const int& width, const int& height) :
VehiclePhysics(width, height), shape(sf::Vector2f(width, height)) {
    if (!missileTexture.loadFromFile("../images/missile.png")) {
        if (!missileTexture.loadFromFile("../../images/missile.png")) {
            std::cerr << "Cannot load missile icon texture" << std::endl;
            shape.setFillColor(sf::Color(255, 0, 0));
        }
    }
    shape.setTexture(&missileTexture);
    this->isMissile = true;
}

sf::RectangleShape& Missile::getShape() {
    return shape;
}

void Missile::launchMissile(const int& ownerID, const int& checkpoint,
        const VehiclePhysics& vPhys) {
    this->ownerID = ownerID;
    targetCheckpoint = checkpoint;
    double posx = vPhys.getPosition().getX();
    double posy = vPhys.getPosition().getY();
    move(posx, posy);
    lockedVelocity = false;
    // Set initial rotation to correspond the rotation of the vehicle.
    setRotation(vPhys.getRotation());
    // Calculate lauching direction and speed
    Vector2D unitVector = Vector2D::getUnitVector(vPhys.getRotation());
    // Direction of unitVector is already correct but length is wrong
    // Calculate scalar speed: Initial speed of the vehicle + MISSILE_SPEED
    double speed = vPhys.getVelocity().getLength() + MISSILE_SPEED;
    // Set final velocity
    setVelocity(unitVector * speed);
    isFlying = true;
    this->fuelTime.restart();
}

void Missile::draw(sf::RenderWindow& window) {
    if (this->isFlying && !this->isDestroyed) {
        window.draw(shape);
    }
}

void Missile::moveMissile(const Race& race) {
    // check that fuel is not over
    if (fuelTime.getElapsedTime().asSeconds() > 10) {
        this->isFlying = false;
        this->isDestroyed = true;
    }
    if (!isFlying)
        return;
    // check flag for acceleration
    bool doAccelerate = true;
    // first get the targetCheckpoint
    const sf::RectangleShape target = AIVehicle::getTargetCheckpoint(race.getTrack(), targetCheckpoint);
    // check if missile is on target checkpoint
    // if on target -> increase targetCheckpoint, get next target
    Polygon polyMissile = Polygon(shape);
    Polygon polyTarget = Polygon(target);
    if (polyMissile.intersects(polyTarget)) {
        targetCheckpoint++;
        const sf::RectangleShape targetNew = AIVehicle::getTargetCheckpoint(race.getTrack(), targetCheckpoint);
        polyTarget = Polygon(targetNew);
        brake();
        doAccelerate = false;
    }
    // check if there is any vehicle in range (not the owner of missile)
    structures::Point targetPoint;
    structures::Point missilePoint = AIVehicle::getMiddlePoint(shape);
    bool vehicleInRange = false;
    int closest = 0; // ID of the closest target
    double minDistance = std::numeric_limits<double>::max();
    // first check regular player controlled vehicles. Bad style to use 
    // copy paste code. However... feel too lazy and really have no time.
    for (auto v : race.getVehicles()) {
        // check that vehicle is not owner 
        if (ownerID == v->getID() || v->isDestroyed()) {
            continue; // go to next vehicle
        }
        // otherwise get the middle point of vehicle and calculate distance
        // between missile and vehicle.
        structures::Point vehiclePoint = AIVehicle::getMiddlePoint(v->getShape());
        double distance = AIVehicle::calculateDistanceBetweenPoints(missilePoint, vehiclePoint);
        // update the distance to nearest vehicle that is in range
        if (distance <= 250 && distance < minDistance) {
            targetPoint = vehiclePoint;
            closest = v->getID();
            minDistance = distance;
            vehicleInRange = true;
        }
    }
    for (auto v : race.getAIVehicles()) {
        // check that vehicle is not owner 
        if (ownerID == v->getID() || v->isDestroyed()) {
            continue; // go to next vehicle
        }
        // otherwise get the middle point of vehicle and calculate distance
        // between missile and vehicle.
        structures::Point vehiclePoint = AIVehicle::getMiddlePoint(v->getShape());
        double distance = AIVehicle::calculateDistanceBetweenPoints(missilePoint, vehiclePoint);
        // update the distance to nearest vehicle that is in range
        if (distance <= 250 && distance < minDistance) {
            targetPoint = vehiclePoint;
            closest = v->getID();
            minDistance = distance;
            vehicleInRange = true;
        }
    }
    // if vehicle was in range -> check if there is a collision to it
    // if there is... do some dmg.
    if (vehicleInRange) {
        sf::FloatRect missileBounds = shape.getGlobalBounds();
        sf::FloatRect vehicleBounds = searchShapeByVehicleID(closest, race).getGlobalBounds();
        if (missileBounds.intersects(vehicleBounds)) {
            if (missileHitVehicle(closest, race) <= 0) {
                // destroy the missile.
                this->isFlying = false;
                this->isDestroyed = true;
                return;
            }
        }
    }
    // if vehicle was in range -> skip the next if sentence. Next 
    // is only for finding the middle point of target checkpoint.   
    if (!vehicleInRange) {
        targetPoint = AIVehicle::getMiddlePoint(target);
    }
    // get vector from missile to target
    Vector2D vectorMissile(missilePoint.x, missilePoint.y);
    Vector2D vectorTarget(targetPoint.x, targetPoint.y);
    Vector2D targetDirection = vectorTarget - vectorMissile;
    // get vector pointing to missiles's direction
    Vector2D missileDirection = AIVehicle::getAIDirection(shape);
    // get angle difference-between directions
    double angle = Vector2D::angleBetween(targetDirection, missileDirection);
    if (abs(angle) > 10) {
        if (angle > 0) {
            if (getAngularVelocity() != -MISSILE_ANG_VEL) {
                setAngularVelocity(-MISSILE_ANG_VEL);
            }
        } else {
            if (getAngularVelocity() != MISSILE_ANG_VEL) {
                setAngularVelocity(MISSILE_ANG_VEL);
            }
        }
    } else {
        // go straight
        if (getAngularVelocity() != 0) {
            setAngularVelocity(0);
            accelerating = false;
        }
        isBraking = false;
        doAccelerate = true;
    }

    if (doAccelerate && !isBraking && !accelerating) {
        accelerate();
        accelerating = true;
    }
    this->updatePosition();
    sync();
}

void Missile::sync() {
    shape.setPosition(getPosition().getX(), getPosition().getY());
    shape.setRotation(getRotation());
}

sf::RectangleShape Missile::searchShapeByVehicleID(const int& vehicleID,
        const Race& race) const {
    for (auto v : race.getVehicles()) {
        if (v->getID() == vehicleID)
            return v->getShape();
    }
    for (auto v : race.getAIVehicles()) {
        if (v->getID() == vehicleID)
            return v->getShape();
    }
    // in error situation
    return race.getVehicles()[0]->getShape();
}

int Missile::missileHitVehicle(const int& vehicleID, const Race& race) {
    for (auto v : race.getVehicles()) {
        if (v->getID() == vehicleID)
            return v->damageVehicle(MISSILE_DMG);
    }
    for (auto v : race.getAIVehicles()) {
        if (v->getID() == vehicleID)
            return v->damageVehicle(MISSILE_DMG);
    }
    // in error situation
    return 100;
}
