/* 
 * File:   line.hpp
 * Author: Martin Vidjeskog
 *
 * Created on November 22, 2017, 4:53 PM
 * 
 * Class representing a line. Line has two points.
 */

#ifndef LINE_HPP
#define LINE_HPP

#include "structures.hpp"

class Line {
public:

    /// Constructor
    Line(const structures::Point &start, const structures::Point &end) : start(start), end(end) {
    };

    /// Function for checking if two lines are intersecting.
    bool intersects(const Line &line2);

    /// Function for getting starting point of line.
    const structures::Point& getStart() const {
        return start;
    };

    /// Function for getting ending point of line.
    const structures::Point& getEnd() const {
        return end;
    };

private:

    // line starting point
    structures::Point start;
    
    // line ending point
    structures::Point end; 
};

#endif
