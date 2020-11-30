#include <vector>
#include <SFML/Graphics.hpp>

#include "polygon.hpp"
#include "structures.hpp"
#include "line.hpp"

// forward declaration
structures::Point getTransformedPoint(const sf::Shape &rect, const int& index);

Polygon::Polygon(const sf::Shape &shape) {
    // index of last point
    std::size_t last;
    // create lines from all points
    for (std::size_t i = 1; i < shape.getPointCount(); i++) {
        structures::Point start = getTransformedPoint(shape, i - 1);
        structures::Point end1 = getTransformedPoint(shape, i);
        Line line(start, end1);
        lines.push_back(line);
        last = i;
    }
    // the last line starts from last point and ends to first
    structures::Point start = getTransformedPoint(shape, last);
    structures::Point end1 = getTransformedPoint(shape, 0);
    Line line(start, end1);
    lines.push_back(line);
}

bool Polygon::intersects(const Polygon &poly2) {
    // loop through the lines and check for intersections
    for (Line lineOne : lines) {
        for (Line lineTwo : poly2.getLines()) {
            if (lineOne.intersects(lineTwo)) {
                return true;
            }
        }
    }
    return false;
}

const Line Polygon::getCrashedLine(const Polygon &poly2) {
    // loop through the lines and check for intersections
    for (Line lineOne : lines) {
        int index = 0;
        for (Line lineTwo : poly2.getLines()) {
            if (lineOne.intersects(lineTwo)) {
                return poly2.getLines()[index];
            }
            index++;
        }
    }
    // Failed to determine line --> return line with NaN-points.
    // Error handling must be performed where function is called.
    structures::Point point = {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
    Line line(point, point);
    return line;
}

const std::vector<Line> Polygon::getLines() const {
    return lines;
}

structures::Point getTransformedPoint(const sf::Shape &shape, const int& index) {
    structures::Point pointOriginal = {shape.getPoint(index).x, shape.getPoint(index).y};
    sf::Transform tran = shape.getTransform();
    structures::Point pointTransformed = {tran.transformPoint(pointOriginal.x, pointOriginal.y).x,
        tran.transformPoint(pointOriginal.x, pointOriginal.y).y};
    return pointTransformed;
}




