#include "aivehicle.hpp"
#include "polygon.hpp"
#include <iostream>
#include <cmath> 
#include "constants.hpp"

AIVehicle::AIVehicle(const int& width, const int& height) : Vehicle(width, height) {
    targetCheckpoint = 0;
}

AIVehicle::AIVehicle(const int& width, const int& height, const std::string& textureName)
: Vehicle(width, height, textureName) {
    targetCheckpoint = 0;
}

void AIVehicle::moveAI(Track& track) {
    // check flag for acceleration
    bool doAccelerate = true;
    // first get the targetCheckpoint
    const sf::RectangleShape target = getTargetCheckpoint(track, targetCheckpoint);
    // check if AI is on target checkpoint
    // if on target -> increase targetCheckpoint, get next target
    Polygon polyAI = Polygon(shape);
    Polygon polyTarget = Polygon(target);
    if (polyAI.intersects(polyTarget)) {
        targetCheckpoint++;
        const sf::RectangleShape targetNew = getTargetCheckpoint(track, targetCheckpoint);
        polyTarget = Polygon(targetNew);
        physics.brake();
        doAccelerate = false;
    }
    // get middlepoint of target and AI
    structures::Point pointTarget = getMiddlePoint(target);
    structures::Point pointAI = getMiddlePoint(shape);
    // get vector from AI to target
    Vector2D vectorAI(pointAI.x, pointAI.y);
    Vector2D vectorTarget(pointTarget.x, pointTarget.y);
    Vector2D targetDirection = vectorTarget - vectorAI;
    // get vector pointing to AI's direction
    Vector2D AIDirection = getAIDirection(shape);
    // get angle difference-between directions
    double angle = Vector2D::angleBetween(targetDirection, AIDirection);
    if (abs(angle) > 10) {
        if (angle > 0) {
            if (getPhysics().getAngularVelocity() != -AI_ANG_VEL) {
                physics.setAngularVelocity(-AI_ANG_VEL); // see constants.hpp
            }
        } else {
            if (getPhysics().getAngularVelocity() != AI_ANG_VEL) {
                physics.setAngularVelocity(AI_ANG_VEL);
            }
        }
    } else {
        // go straight
        if (getPhysics().getAngularVelocity() != 0) {
            physics.setAngularVelocity(0);
            accelerating = false;
        }
        physics.isBraking = false;
        doAccelerate = true;
    }
    if (doAccelerate && !physics.isBraking && !accelerating) {
        physics.accelerate();
        accelerating = true;
    }
}

const sf::RectangleShape& AIVehicle::getTargetCheckpoint(const Track& track, int& index) {
    if (track.getCheckpoints().empty()) {
        throw std::runtime_error("Checkpoints are missing... AIVehicle::getTargetCheckpoint()!");
    }
    // if index is outside the limits set to range
    if (index < 0) {
        index = track.getCheckpoints().size() - 1;
    }
    if (static_cast<unsigned int> (index) >= track.getCheckpoints().size()) {
        index = 0;
    }
    return track.getCheckpoints()[index];
}

structures::Point AIVehicle::getMiddlePoint(const sf::RectangleShape& target) {
    structures::Point pointOriginal = {target.getPoint(0).x, target.getPoint(0).y};
    pointOriginal.x = pointOriginal.x + target.getSize().x / 2;
    pointOriginal.y = pointOriginal.y + target.getSize().y / 2;
    sf::Transform tran = target.getTransform();
    structures::Point pointTransformed = {tran.transformPoint(pointOriginal.x, pointOriginal.y).x,
        tran.transformPoint(pointOriginal.x, pointOriginal.y).y};
    return pointTransformed;
}

Vector2D AIVehicle::getAIDirection(sf::RectangleShape& shape) {
    structures::Point backLeft = {shape.getPoint(0).x, shape.getPoint(0).y};
    structures::Point frontLeft = {shape.getPoint(1).x, shape.getPoint(1).y};
    sf::Transform tran = shape.getTransform();
    structures::Point backTran = {tran.transformPoint(backLeft.x, backLeft.y).x,
        tran.transformPoint(backLeft.x, backLeft.y).y};
    structures::Point frontTran = {tran.transformPoint(frontLeft.x, frontLeft.y).x,
        tran.transformPoint(frontLeft.x, frontLeft.y).y};
    Vector2D back(backTran.x, backTran.y);
    Vector2D front(frontTran.x, frontTran.y);
    Vector2D vector = front - back;
    return vector;
}

int AIVehicle::calculateDistanceBetweenPoints(const structures::Point& point1,
        const structures::Point& point2) {
    int distance;
    distance = sqrt(pow((point1.x - point2.x), 2) + pow((point1.y - point2.y), 2));
    return distance;
}
