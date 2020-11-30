#ifndef DYNAMIC_OBJECT_HH
#define DYNAMIC_OBJECT_HH

#include <iostream>
#include <SFML/Graphics.hpp> // Needed by sf::Clock

#include "vector2d.hpp"
#include "line.hpp"
#include "track.hpp"



/**
 * Author: Miika Karsimus
 *
 * This class includes physical features of a vehicle.
 * The class has properties, such as position, velocity,
 * acceleration, angular velocity and rotation. The object calucalates its position based on
 * elapsed time (sf::Clock is used). If you want to move the object, you should set a desired
 * acceleration to the object, using accelerate() function. Also position can be set directly,
 * meaning that the object "teleports".
 *
 * Note: Every VehiclePhysics has its own clock.
 * Clock is a private attribute and any other class don't have to access to it.
 *
 *----------------------------------------
 * How to use this class properly:
 *
 * If you want to obtain realistic physics, you should use accelerate() function to move the vehicle.
 * That function sets the direction of the acceleration to correspond the direction of the nose
 * of the vehicle. If you want to simulate force impulse (very short infinity acceleration), you
 * can modify the velocity direclty. This is useful especially, when simulating an elastic collision,
 * i.e. a collision which maintains kinetic energy and changes the velocity of the object.
 *
 *------------------------------------------
 * TODO:
 *
 * Reversing doesn't work properly. I am working on it.
 *
 */



class VehiclePhysics
{
public:
    /// Pass width and height of the vehicle as parameter.
    VehiclePhysics(const int& width, const int &height);
    
    /// Tell if object is moving forward, backward or is stopped.
    /// Note that enum values are int types.
    enum MovingState {
        FORWARD = 0,
        BACKWARD = 1,
        STOP = 2
    };
    
    inline const Vector2D& getPosition() const { return position; }
    inline const double& getX() const { return position.getX(); }
    inline const double& getY() const { return position.getY(); }
    inline const Vector2D& getVelocity() const { return velocity; }
    inline const Vector2D& getAcceleration() const { return acceleration; }
    inline const double& getAngularVelocity() const { return angularVelocity; }
    inline const double& getAngularAcceleration() const { return angularAcceleration; }
    inline const double& getRotation() const { return rotation; }
    inline const int& getHeight() const { return height; }
    inline const int& getWidth() const { return width; }
    
    /// Get angle between the nose direction and velocity direction.
    /// Can be between -180 and 180 degrees. 0 means that the vehicle is not sliding.
    const double getSlidingAngle() const;
    
    /// Tell if the velocity is forced to correspond the direction of the nose.
    
    void move(const double&, const double&);
    void setAcceleration(const Vector2D& acc);
    void setVelocity(const Vector2D& vel);
    void setAngularVelocity(const double& angVel); // deg/s
    void setAngularAcceleration(const double& angAcc);
    void rotate(double degs);
    
    /// Works only if locked velocity is false.
    void setRotation(double degs);
    
    /// Set acceleration towards the nose of the vehicle.
    /// Basic interface for manipulating the acceleration of the object.
    void accelerate();
    
    /// Stop moving immediately.
    void stop();
    
    /// Flip velocity about x or y-axis.
    void flipVelocity(const char& axis);
    
    /// Set reverse acceleration in relation to current velocity.
    /// If fullBrake is set to true, decceleration is higher.
    void brake(bool fullBrake = true); // override if necessary
    
    /// Reverse with vehicle.
    void reverse();
    
    /// Updates the position of the object based on elapsed time.
    void updatePosition();
    
    /// Fix velocity direction when collision occurs. Param. shape is vehicle shape.
    void handleCollision(const Line& line, const Track& track, sf::RectangleShape shape);
    
    /// Tell if the object is braking at the moment.
    bool isBraking = false;
    
    /// Tell if the object is moving at its maximum speed.
    bool atTopSpeed = false; // currently unused
    
    /// Tell if the velocity is forced to correspond the direction of the nose.
    /// i.e. If this is true, the direction of the velocity is ALWAYS same as rotation.
    /// After collision, lockedVelocity should be set to false.
    bool lockedVelocity = true;
    
    // Not used by this class. However, heat-guided missile uses this.
    bool isMissile = false;
    
    
private:
    int width;
    int height;
    
    // Quantities are initialized to zero-vectors
    Vector2D position = {0.0, 0.0};
    Vector2D velocity = {0.0, 0.0};
    Vector2D acceleration = {0.0, 0.0};
    
    double rotation = 0.0;
    
    int movingState = MovingState::STOP;
    
    /// Positive direction of angle is clockwise. Unit is deg/s.
    double angularVelocity = 0.0;
    
    double angularAcceleration = 0.0;
    
    /// Reference variables, used by updatePosition function.
    /// In order to calculate the current position, you have to know x0, y0 and v0
    /// because s = s0 + v0 * t + 0.5 * a * t^2.
    double pos_x0 = 0.0;
    double pos_y0 = 0.0;
    Vector2D vel0 = {0.0, 0.0};
    double rot0 = 0.0;
    double angVel0 = 0.0;
    
    /// Reset reference variables to correspond current values
    /// x0 = position.x, y0 = position.y and so on.
    /// Should be called every time, when the state of the object changes. In practice this means
    /// that, for example, if you change the acceleration of the object, reset is called.
    void reset();
    
    // The position if calculated using this clock.
    // Restarted every time the acceleration changes.
    sf::Clock clock;
    // Clock which tells the duration of the last collision.
    sf::Clock collisionClock;
    // Clock which tells the duration of the last reset() call.
    sf::Clock resetClock;
    
    // Test if the velocity reaches zero.
    // old velocity: x1 and y1, new velocity: x2 and y2
    // returns true if both y and x velocities changes their sign at the same time
    bool checkVelocitySign(double x1, double y1, double x2, double y2);
    
    /// Update angle based on elapsed time and angular velocity.
    void updateAngle(double& time);
    
    /// Adjust velocity and acceleration direction to correspond rotation.
    /// Call every time when rotation changes.
    void fixDirections();
    
    // Currently unused.
    double rotateTime;
    
    
};

// Print a string representation (acc, vel, rot etc.).
std::ostream& operator<<(std::ostream&, VehiclePhysics&);


#endif
