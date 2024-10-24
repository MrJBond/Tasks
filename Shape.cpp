#include "Shape.h"

void geometry::Shape::boundingBoxHelper(double minY, double minX, double maxY, double maxX, std::vector<Point2d>& res) const {
    Point2d point1(minX, minY);
    Point2d point2(maxX, maxY);
    Point2d point3(minX, maxY);
    Point2d point4(maxX, minY);

    res[0] = point1;  // Bottom-left corner
    res[1] = point2;  // Top-right corner
    res[2] = point3;  // Top-left corner
    res[3] = point4;  // Bottom-right corner
}