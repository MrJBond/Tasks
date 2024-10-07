#include "Polyline.h"


std::vector<geometry::Point2d> geometry::Polyline::getPoints() {
    return points;
}

void geometry::Polyline::addPoint(const geometry::Point2d& point) {
    points.push_back(point);
}
void geometry::Polyline::removePoint(size_t index) {
    if (index >= points.size()) {
        throw std::out_of_range("Index out of range");
    }
    points.erase(points.begin() + index);
}
double geometry::Polyline::perimeter() {
    if (points.size() < 2 || !this->isValid()) {
        return 0.0; // Not enough points to form a polyline
    }
    double totalLength = 0.0;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        totalLength += distanceBetween(points[i], points[i + 1]);
    }
    return totalLength;
}
size_t geometry::Polyline::getPointCount() const {
    return points.size();
}
geometry::Point2d geometry::Polyline::getPoint(size_t index) const {
    if (index >= points.size()) {
        throw std::out_of_range("Index out of range");
    }
    return points[index];
}
void geometry::Polyline::clear() {
    points.clear();
}

// Check if the polyline is valid (needs at least 2 points)
bool geometry::Polyline::isValid() {
    return points.size() >= 2;
}

std::vector<geometry::Point2d> geometry::Polyline::boundingBox() {
    std::vector<Point2d> res;
    if (points.empty() || !this->isValid()) {
        return res;
    }

    // Initialize min and max with the first point
    double minX = points[0].x();
    double minY = points[0].y();
    double maxX = points[0].x();
    double maxY = points[0].y();

    // Iterate through all points to find min and max x and y
    for (const Point2d& p : points) {
        if (p.x() < minX) minX = p.x();
        if (p.y() < minY) minY = p.y();
        if (p.x() > maxX) maxX = p.x();
        if (p.y() > maxY) maxY = p.y();
    }

    this->boundingBoxHelper(minY, minX, maxY, maxX, res);
    return res;
}
