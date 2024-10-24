#include "Circle.h"

void geometry::Circle::set(Point2d p, double r) {
    m_center = p;

    try {
        if (r >= 0) {
            m_r = r;
        }
        else {
            throw std::invalid_argument("Radius must be greater than 0!");
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }
}
std::vector<geometry::Point2d> geometry::Circle::boundingBox() const {
    std::vector<Point2d> res(4);

    // Calculate the bounding box corners based on the center and radius
   const double minX = m_center.x() - m_r;
   const double minY = m_center.y() - m_r;
   const double maxX = m_center.x() + m_r;
   const double maxY = m_center.y() + m_r;

    this->boundingBoxHelper(minY, minX, maxY, maxX, res);
    return res;
}
double geometry::Circle::perimeter() const {
    return 2 * PI * m_r;
}

