#include <cmath>
#include <stdexcept>
#include <sstream>

#include "vector2d.hpp"



// member-functions

// Constructor
Vector2D::Vector2D(const double& x1, const double& y1) : x(x1), y(y1)
{
    
}

double Vector2D::getLength() const
{
    return sqrt(pow(x, 2) + pow(y, 2));
}


void Vector2D::rotate(double degs)
{
	// sin and cos are included from <cmath>
	double rads = deg2rad(degs);
	double x2 = x * cos(rads) - y * sin(rads);
	double y2 = x * sin(rads) + y * cos(rads);
	x = x2;
	y = y2;
}


void Vector2D::mirror(const char& c)
{
	if (c == 'x')
        y = -y;
    else if (c == 'y')
        x = -x;
    else; // does nothing
}

void Vector2D::mirror(const Vector2D &vector)
{
    // Param. vector is the "mirror surface."
    double angle1 = angleBetween(vector, *this);
    this->rotate(-2 * angle1);
}

Vector2D Vector2D::getUnitVector() const
{
	Vector2D unit_vector = *this / this->getLength();
	if (unit_vector.getLength() < 0.99 || unit_vector.getLength() > 1.01)
        throw std::runtime_error("Invalid unit vector.");
	return unit_vector;
}

std::string Vector2D::str() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}


// non-member functions

std::ostream& operator<<(std::ostream& os, const Vector2D& v)
{
    os << "[" << v.x << ", " << v.y << "]";
    return os;
}


double operator*(const Vector2D& v1, const Vector2D& v2)
{
	return (v1.x * v2.x + v1.y * v2.y);
}


Vector2D operator*(const Vector2D& v1, double coeff)
{
	Vector2D new_vec = v1;
    new_vec.x *= coeff;
    new_vec.y *= coeff;
    return new_vec;
}


Vector2D operator/(const Vector2D& v1, double coeff)
{
	Vector2D new_vec = v1;
    new_vec.x /= coeff;
    new_vec.y /= coeff;
    return new_vec;
}


Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
{
	Vector2D sum = v1;
    sum.x += v2.x;
    sum.y += v2.y;
    return sum;
}


Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
{
	Vector2D vec = v1;
    vec.x -= v2.x;
    vec.y -= v2.y;
    return vec;
}


Vector2D operator-(const Vector2D& v)
{
	Vector2D comp(-v.x, -v.y); // complement
    return comp;
}

/******************************
 The following functions are static.
 ******************************/

Vector2D Vector2D::rotated(const Vector2D& v, double degs)
{
	Vector2D v2 = v;
    v2.rotate(degs);
    return v2;
}

Vector2D Vector2D::getMirrorNormal(Vector2D& v1, const Vector2D& mirrorVector)
{
    double angle1 = angleBetween(mirrorVector, v1);
    if (angle1 > 0)
        angle1 = 180 + angle1;
    return Vector2D::rotated(v1, -angle1 + 90);
}

bool Vector2D::isParallel(const Vector2D& v1, const Vector2D& v2)
{
	// Two vectors are parallel if their dot product is zero.
	return v1 * v2 == 0;
}


double Vector2D::angleBetween(const Vector2D& v1, const Vector2D& v2)
{
    // Direction is clockwise (same as the angle directon of SFML)
    return rad2deg(atan2(Vector2D::determinant(v1, v2), v1 * v2));
}

double Vector2D::determinant(const Vector2D &v1, const Vector2D &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

double Vector2D::axisAngle(const Vector2D& v)
{
	// vector [1, 0] is parallel to x-axis
	return angleBetween(v, {1, 0});
}


double Vector2D::deg2rad(const double& degs)
{
	return PI * degs / 180;
}

double Vector2D::rad2deg(const double& rads)
{
	return 180 * rads / PI;
}


Vector2D Vector2D::getUnitVector(const double &degs)
{
    double rads = deg2rad(degs);
    Vector2D unitVector(cos(rads), sin(rads));
    return unitVector;
}


structures::Point Vector2D::movePoint(const structures::Point &point, const Vector2D &v)
{
    structures::Point newPoint = point;
    newPoint.x += v.getX();
    newPoint.y += v.getY();
    return newPoint;
}







