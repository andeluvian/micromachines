#include "line.hpp"
#include "collision.hpp"

bool Line::intersects(const Line &line2) {
    return doIntersect(this->getStart(), this->getEnd(), line2.getStart(), line2.getEnd());
}