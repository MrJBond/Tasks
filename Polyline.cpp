#include "Polyline.h"

geometry::Point2d geometry::Polyline::getCenter() {
    if (isValid()) {

        // centroid
        double sumX = 0.0;
        double sumY = 0.0;
        for (const auto& point : m_points) {
            sumX += point.x();
            sumY += point.y();
        }
        const size_t count = m_points.size();
        m_center = Point2d(sumX / count, sumY / count);
    }
    else {
        std::cout << "The Polyline is invalid!" << std::endl;
    }
    return m_center;
}

std::vector<geometry::Point2d> geometry::Polyline::getPoints() const {
    return m_points;
}

void geometry::Polyline::addPoint(const geometry::Point2d& point) {
    m_points.push_back(point);
}
void geometry::Polyline::removePoint(size_t index) {
    if (index >= m_points.size()) {
        throw std::out_of_range("Index out of range");
    }
    m_points.erase(m_points.begin() + index);
}
double geometry::Polyline::perimeter() const {
    if (m_points.size() < 2 || !isValid()) {
        return 0.0; // Not enough points to form a polyline
    }
    double totalLength = 0.0;
    for (size_t i = 0; i < m_points.size() - 1; ++i) {
        totalLength += distanceBetween(m_points[i], m_points[i + 1]);
    }
    return totalLength;
}
size_t geometry::Polyline::getPointCount() const {
    return m_points.size();
}
geometry::Point2d geometry::Polyline::getPoint(size_t index) const {
    if (index >= m_points.size()) {
        throw std::out_of_range("Index out of range");
    }
    return m_points[index];
}
void geometry::Polyline::clear() {
    m_points.clear();
}

// Check if the polyline is valid (needs at least 2 points)
bool geometry::Polyline::isValid() const {
    return m_points.size() >= 2;
}

std::vector<geometry::Point2d> geometry::Polyline::boundingBox() const {
    std::vector<Point2d> res(4);
    if (m_points.empty() || !isValid()) {
        return res;
    }

    // Initialize min and max with the first point
    double minX = m_points[0].x();
    double minY = m_points[0].y();
    double maxX = m_points[0].x();
    double maxY = m_points[0].y();

    // Iterate through all points to find min and max x and y
    for (const Point2d& p : m_points) {
        if (p.x() < minX) minX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() > maxY) maxY = p.y();
    }

    boundingBoxHelper(minY, minX, maxY, maxX, res);
    return res;
}
