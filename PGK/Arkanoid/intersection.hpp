#define INTERSECTION_HPP

#include "extraFunctions.hpp"

int onSegment(double ax, double ay, double bx, double by, double zx, double zy);
double orientation(double ax, double ay, double bx, double by, double zx, double zy);
int doIntersect(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy);
/*int trianglesIntersection(double ax, double ay, double bx, double by, double cx, double cy,
                          double dx, double dy, double ex, double ey, double fx, double fy);
*/