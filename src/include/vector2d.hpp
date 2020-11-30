#ifndef VECTOR2D_HH
#define VECTOR2D_HH

#define PI 3.141592654

#include <iostream>

#include "structures.hpp"

/**
 * Author: Miika Karsimus
 *
 * Utility class for representing 2-dimensional vector (x and y components).
 * Note that most part of the functions are overloaded operators.
 * It makes it easy to use the class. You can, for example, sum two vector by typing v1 + v2
 * or calculate the dot product of two vectors by typing v1 * v2.
 * 
 * The vector can be used for example representing velocity, acceleration, direction or 
 * just a point in xy-space.
 */


class Vector2D
{
public:
	/// Pass x and y components as parameters
    Vector2D(const double& x1, const double& y1);
    
    double getLength() const;
    
    // Self-explanatory functions.
    inline const double& getX() const { return x; }
    inline const double& getY() const { return y; }
    inline void setX(double x1) { x = x1; }
    inline void setY(double y1) { y = y1; }
   
    /// Rotate the vector clockwise (direction in SFML coordinate frame).
    void rotate(double degs);

    /// Mirror the vector about x or y-axis.
    void mirror(const char& c);
    
    /// Mirror the vector about and arbitrary vector.
    /// E.g. [0, 1] mirrored about [1, 1] yields [1, 0].
    void mirror(const Vector2D& vector);
    
    /// Returns a string representation of the vector.
    std::string str() const;

    /// Return an unit vector parallel to this vector.
    Vector2D getUnitVector() const;
    
    /// Get unit vector which angle is degs degrees relative to x-axis.
    /// e.g. calling with parameter 45 would result [0.707, 0.707].
    static Vector2D getUnitVector(const double& degs);
    
    
    /// Get vector which is the normal of mirrorVector. Direction depends on v1.
    static Vector2D getMirrorNormal(Vector2D& v1, const Vector2D& mirrorVector);
    
    /// Convert degrees to radians.
    static double deg2rad(const double& degs);
    
    /// Convert radians to degrees.
    static double rad2deg(const double& rads);
    
    /// Return angle between vector and x-axis.
    static double axisAngle(const Vector2D& v);
    
    /// Move point to the direction of the vector and return a new Point.
    /// E.g. calling with {2, 1}, {5, 7} yields {7, 8}.
    static structures::Point movePoint(const structures::Point& point, const Vector2D& v);
    
    /// Return a new rotated vector. Doesn't affect this vector.
    /// Note the difference between this and the member-function rotate.
    static Vector2D rotated(const Vector2D& v, double degs);
    
    /// Tell if two vector are parallel.
    static bool isParallel(const Vector2D& v1, const Vector2D& v2);
    
    /// Return the angle between two vectors in degrees.
    static double angleBetween(const Vector2D& v1, const Vector2D& v2);
    
    /// Determinant between two vector. Needed by the angleBetween() function.
    static double determinant(const Vector2D& v1, const Vector2D& v2);

private:
	double x; // x-component
    double y; // y-component

// The following functions are declared as friends in order to
// access the private members x and y.
friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);
friend double operator*(const Vector2D& v1, const Vector2D& v2);
friend Vector2D operator*(const Vector2D& v1, double coeff);
friend Vector2D operator/(const Vector2D& v1, double coeff);
friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
friend Vector2D operator-(const Vector2D& v);

friend class VehiclePhysics;
};


// overloaded operators

/// Print vector to stdout.
std::ostream& operator<<(std::ostream& os, const Vector2D& v);


// Dot product with another vector. Returns scalar (double).
double operator*(const Vector2D& v1, const Vector2D& v2);

/// Product with a floating point number.
Vector2D operator*(const Vector2D& v1, double coeff);

/// Divide vector with floating point number.
Vector2D operator/(const Vector2D& v1, double coeff);

/// Sum two vectors.
Vector2D operator+(const Vector2D& v1, const Vector2D& v2);

/// Substraction of two vectors.
Vector2D operator-(const Vector2D& v1, const Vector2D& v2);

/// Unary operator.
Vector2D operator-(const Vector2D& v);







#endif











