/* 
 * File:   collision.hpp
 * Author: Martin Vidjeskog
 *
 * Created on November 22, 2017, 4:03 PM
 * 
 * File contains methods to check if two lines (4 points) intersect.
 * Source: www.geeksforgeeks.org/check-if-two-given-line-segments-intersect
 */

#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "structures.hpp"


/// Given three colinear points p, q, r, the function checks if
/// point q lies on line segment 'pr'.
bool onSegment(const structures::Point &p, const structures::Point &q,
        const structures::Point &r);

/// To find orientation of ordered triplet (p, q, r).
/// The function returns following values
/// 0 --> p, q and r are colinear
/// 1 --> Clockwise
/// 2 --> Counterclockwise 
int orientation(const structures::Point &p, const structures::Point &q,
        const structures::Point &r);

/// The main function that returns true if line segment 'p1q1'
/// and 'p2q2' intersect.
bool doIntersect(const structures::Point &p1, const structures::Point q1,
        const structures::Point p2, const structures::Point q2);

#endif /* COLLISION_HPP */

