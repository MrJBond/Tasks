#include "Circle.h"

void geometry::Circle::set(Point2d p, double r) {
    this->center = p;

    try {
        if (r >= 0) {
            this->r = r;
        }
        else {
            throw std::invalid_argument("Radius must be greater than 0!");
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }
}
std::vector<geometry::Point2d> geometry::Circle::boundingBox() {
    std::vector<Point2d> res;

    // Calculate the bounding box corners based on the center and radius
    double minX = center.x() - r;
    double minY = center.y() - r;
    double maxX = center.x() + r;
    double maxY = center.y() + r;

    this->boundingBoxHelper(minY, minX, maxY, maxX, res);
    return res;
}
double geometry::Circle::perimeter() {
    return 2 * PI * r;
}

