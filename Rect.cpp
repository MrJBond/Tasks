#include "Rect.h"

geometry::Point2d geometry::Rect::getCenter() {
    if (isValid()) {
        double m_center_x = (m_p1.x() + m_p3.x()) / 2.0;
        double m_center_y = (m_p1.y() + m_p3.y()) / 2.0;
        m_center = Point2d(m_center_x, m_center_y);
    }
    else {
        std::cout << "The Rect is invalid!" << std::endl;
    }
    return m_center;
}
std::vector<geometry::Point2d> geometry::Rect::getPoints() const {
    std::vector<Point2d> r(4);
    r[0] = m_p1;
    r[1] = m_p2;
    r[2] = m_p3;
    r[3] = m_p4;
    return r;
}
void geometry::Rect::set(Point2d p1, Point2d p2, Point2d p3, Point2d p4) {
    m_p1 = p1;
    m_p2 = p2;
    m_p3 = p3;
    m_p4 = p4;
}

void geometry::Rect::set(Point2d p1, Point2d p3) {
    m_p1 = p1;
    m_p3 = p3;

    // The given line is parallel to an axis
    // multiple rectangles could theoretically be constructed
    if (isEqualDouble(p1.x(), p3.x(), 1e-8) || isEqualDouble(p1.y(), p3.y(), 1e-8)) {
        throw std::invalid_argument("The points form an ambiguous rectangle");
    }
    else {

        m_p2 = Point2d(p1.x(), p3.y());
        m_p4 = Point2d(p3.x(), p1.y());
    }
}

bool geometry::Rect::isValid() const {
    // Check if opposite sides are equal
    const double d1 = std::hypot(m_p1.x() - m_p2.x(), m_p1.y() - m_p2.y());
    const double d2 = std::hypot(m_p2.x() - m_p3.x(), m_p2.y() - m_p3.y());
    const double d3 = std::hypot(m_p3.x() - m_p4.x(), m_p3.y() - m_p4.y());
    const double d4 = std::hypot(m_p4.x() - m_p1.x(), m_p4.y() - m_p1.y());

    if (!isEqualDouble(d1, d3, 1e-6) || !isEqualDouble(d2, d4, 1e-6)) {
        return false;
    }

    // Check if all angles are 90 degrees using dot product
    auto isRightAngle = [](const Point2d& pA, const Point2d& pB, const Point2d& pC) {
        // Vectors AB and BC
        const double ABx = pB.x() - pA.x();
        const double ABy = pB.y() - pA.y();
        const double BCx = pC.x() - pB.x();
        const double BCy = pC.y() - pB.y();
        // Dot product of AB and BC should be 0 for a right angle
        return isEqualDouble(ABx * BCx + ABy * BCy, 0., 1e-8);
        };

    if (!isRightAngle(m_p1, m_p2, m_p3) || !isRightAngle(m_p2, m_p3, m_p4) ||
        !isRightAngle(m_p3, m_p4, m_p1) || !isRightAngle(m_p4, m_p1, m_p2)) {
        return false;
    }

    return true; // All conditions for a valid rectangle are satisfied
}

std::vector<geometry::Point2d> geometry::Rect::boundingBox() const {
    std::vector<Point2d> res(4);

    const double minX = std::min({ m_p1.x(), m_p2.x(), m_p3.x(), m_p4.x() });
    const double minY = std::min({ m_p1.y(), m_p2.y(), m_p3.y(), m_p4.y() });
    const double maxX = std::max({ m_p1.x(), m_p2.x(), m_p3.x(), m_p4.x() });
    const double maxY = std::max({ m_p1.y(), m_p2.y(), m_p3.y(), m_p4.y() });

    boundingBoxHelper(minY, minX, maxY, maxX, res);
    return res;
}
double geometry::Rect::perimeter() const {
    if (this->isValid()) {
        const double d1 = std::hypot(m_p1.x() - m_p2.x(), m_p1.y() - m_p2.y());
        const double d2 = std::hypot(m_p2.x() - m_p3.x(), m_p2.y() - m_p3.y());
        const double d3 = std::hypot(m_p3.x() - m_p4.x(), m_p3.y() - m_p4.y());
        const double d4 = std::hypot(m_p4.x() - m_p1.x(), m_p4.y() - m_p1.y());
        return d1 + d2 + d3 + d4;
    }
    return 0;
}