#include "Rect.h"

std::vector<geometry::Point2d> geometry::Rect::getPoints() {
    std::vector<Point2d> r(4);
    r[0] = p1;
    r[1] = p2;
    r[2] = p3;
    r[3] = p4;
    return r;
}
void geometry::Rect::set(Point2d p1, Point2d p2, Point2d p3, Point2d p4) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->p4 = p4;
}

void geometry::Rect::set(Point2d p1, Point2d p3) {
    this->p1 = p1;
    this->p3 = p3;

    // The given line is parallel to an axis
    // multiple rectangles could theoretically be constructed
    if (p1.x() == p3.x() || p1.y() == p3.y()) {
        throw std::invalid_argument("The points form an ambiguous rectangle");
    }
    else {

        this->p2 = Point2d(p1.x(), p3.y());
        this->p4 = Point2d(p3.x(), p1.y());
    }
}

bool geometry::Rect::isValid() {
    // Check if opposite sides are equal
    double d1 = std::hypot(p1.x() - p2.x(), p1.y() - p2.y());
    double d2 = std::hypot(p2.x() - p3.x(), p2.y() - p3.y());
    double d3 = std::hypot(p3.x() - p4.x(), p3.y() - p4.y());
    double d4 = std::hypot(p4.x() - p1.x(), p4.y() - p1.y());

    if (d1 != d3 || d2 != d4) {
        return false;
    }

    // Check if all angles are 90 degrees using dot product
    auto isRightAngle = [](const Point2d& pA, const Point2d& pB, const Point2d& pC) {
        // Vectors AB and BC
        double ABx = pB.x() - pA.x();
        double ABy = pB.y() - pA.y();
        double BCx = pC.x() - pB.x();
        double BCy = pC.y() - pB.y();
        // Dot product of AB and BC should be 0 for a right angle
        return (ABx * BCx + ABy * BCy) == 0;
        };

    if (!isRightAngle(p1, p2, p3) || !isRightAngle(p2, p3, p4) ||
        !isRightAngle(p3, p4, p1) || !isRightAngle(p4, p1, p2)) {
        return false;
    }

    return true; // All conditions for a valid rectangle are satisfied
}

std::vector<geometry::Point2d> geometry::Rect::boundingBox() {
    std::vector<Point2d> res;

    double minX = std::min({ p1.x(), p2.x(), p3.x(), p4.x() });
    double minY = std::min({ p1.y(), p2.y(), p3.y(), p4.y() });
    double maxX = std::max({ p1.x(), p2.x(), p3.x(), p4.x() });
    double maxY = std::max({ p1.y(), p2.y(), p3.y(), p4.y() });

    this->boundingBoxHelper(minY, minX, maxY, maxX, res);
    return res;
}
double geometry::Rect::perimeter() {
    if (this->isValid()) {
        double d1 = std::hypot(p1.x() - p2.x(), p1.y() - p2.y());
        double d2 = std::hypot(p2.x() - p3.x(), p2.y() - p3.y());
        double d3 = std::hypot(p3.x() - p4.x(), p3.y() - p4.y());
        double d4 = std::hypot(p4.x() - p1.x(), p4.y() - p1.y());
        return d1 + d2 + d3 + d4;
    }
    return 0;
}