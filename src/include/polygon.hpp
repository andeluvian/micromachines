/*
 * File:   polygon.hpp
 * Author: Martin Vidjeskog
 *
 * Created on November 22, 2017, 4:53 PM
 *
 * Class representing a polygon.
 */

#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <SFML/Graphics/Shape.hpp>
#include <vector>
#include "line.hpp"

class Polygon {
public:

    /// Constructor.
    Polygon(const sf::Shape &shape);

    /// Check if polygon is intersecting with another polygon.
    bool intersects(const Polygon &poly2);

    /// Get Line-object that polygon is colliding at.
    /// If no collision, return Line with NAN-points.
    const Line getCrashedLine(const Polygon &poly2);

    /// Get Line-objects forming the polygon.
    const std::vector<Line> getLines() const;

private:

    // lines forming the polygon
    std::vector<Line> lines;
};



#endif /* POLYGON_HPP */

