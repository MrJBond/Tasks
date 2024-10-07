#include "Polygon.h"



bool areCollinear(const geometry::Point2d& p1, const geometry::Point2d& p2, const geometry::Point2d& p3){

	// area = 0 when
	/*
		1 a b
	det 1 m n   = 0 => my + xb + an + - mb - ay - xn = a(n-y)+m(y-b)+x(b-n) = 0
		1 x y
	*/
	return p1.x() * (p2.y() - p3.y()) + p2.x() * (p3.y() - p1.y()) + p3.x() * (p1.y() - p2.y()) == 0;
	}

bool isPolygon(const std::vector<geometry::Point2d>& points) {
	for (size_t i = 2; i < points.size(); ++i) {
		if (!areCollinear(points[0], points[i - 1], points[i])) {
			return true;  // At least three non-collinear points
		}
	}
	return false;  // All points are collinear
}

int orientation(const geometry::Point2d& p, const geometry::Point2d& q, const geometry::Point2d& r) {

    // Cross Product Calculation:

    // val == 0: Points are collinear
     // val > 0: Orientation is clockwise
      //  val < 0 : Orientation is counterclockwise

    int val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
    if (val == 0) return 0; // Collinear
    return (val > 0) ? 1 : 2; // Clockwise or counterclockwise
}

bool onSegment(const geometry::Point2d & p, const geometry::Point2d& q, const geometry::Point2d& r) {

    // Bounding Box Check: The function checks if point q 
    // lies within the axis-aligned bounding rectangle formed by points p and r

    if (q.x() <= std::max(p.x(), r.x()) && q.x() >= std::min(p.x(), r.x()) &&
        q.y() <= std::max(p.y(), r.y()) && q.y() >= std::min(p.y(), r.y()))
        return true;
    return false;
}

bool doIntersect(const geometry::Point2d& p1, const geometry::Point2d& q1, const geometry::Point2d& p2, const geometry::Point2d& q2) {
   
    // Find the four orientations needed for the general and special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // segments intersect if orientations differ
    if (o1 != o2 && o3 != o4) return true;  // General case

    // Special cases (collinear points)
     // check if points are collinear and lie on the other segment
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

bool isSelfIntersecting(const std::vector<geometry::Point2d>& points) {
    int n = points.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 2; j < n; ++j) {
            // Avoid checking adjacent edges and the first and last edges
            if ((i == 0 && j == n - 1) || j == i + 1) continue;
            if (doIntersect(points[i], points[(i + 1) % n], points[j], points[(j + 1) % n])) {
                return true;  // Self-intersecting
            }
        }
    }
    return false;
}


bool geometry::Polygon::isValid() {
	// At least 3 points
	if (points.size() < 3) {
		return false;
	}
	// Non-Collinear Points
	if (!isPolygon(this->points)) {
		return false;
	}
    // Check for self-intersection
    if (isSelfIntersecting(points)) 
        return false;  // Polygon self-intersects

    return true;  // Valid polygon

}
double geometry::Polygon::perimeter() {
    if (points.size() < 2 || !this->isValid()) {
        return 0.0; // Not enough points to form a polyline
    }
    double totalLength = 0.0;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        totalLength += distanceBetween(points[i], points[i + 1]);
    }
    // + the last side
    totalLength += distanceBetween(points[0], points[points.size() - 1]);
    return totalLength;
}