#include "Shape.h"

void geometry::Shape::boundingBoxHelper(double minY, double minX, double maxY, double maxX, std::vector<Point2d>& res) {
    Point2d point1(minX, minY);
    Point2d point2(maxX, maxY);
    Point2d point3(minX, maxY);
    Point2d point4(maxX, minY);

    res.push_back(point1);  // Bottom-left corner
    res.push_back(point2);  // Top-right corner
    res.push_back(point3);  // Top-left corner
    res.push_back(point4);  // Bottom-right corner
}