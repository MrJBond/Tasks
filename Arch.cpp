#include "Arch.h"


// Helper function to calculate the angle of a point relative to the center
double geometry::Arch::calculateAngle(const Point2d& p ){
    double dx = p.x() - center.x();
    double dy = p.y() - center.y();
    return atan2(dy, dx);  // Returns angle in radians
}
double geometry::Arch::normalizeAngle(double angle) {
    if (angle < 0) {
        return (angle + 2*PI);
    }
    return angle;
}
double geometry::Arch::perimeter() {

    // Calculate the angle of the point relative to the center
    double pointAngle = calculateAngle(this->point);

    // Normalize the angle to the range [0, 2*PI]
    pointAngle = this->normalizeAngle(pointAngle);

    // Normalize angles of the arc to [0, 2*PI]
    double normalizedAngle1 = this->normalizeAngle(angle1);
    double normalizedAngle2 = this->normalizeAngle(angle2);


    // Determine which arc the point belongs to
    double angle; // angle of the arc

    if ((normalizedAngle1 <= pointAngle && pointAngle <= normalizedAngle2) ||
        (normalizedAngle1 > normalizedAngle2 && (pointAngle >= normalizedAngle1 || pointAngle <= normalizedAngle2))) {
        // The point is on the smaller arc
        angle = normalizedAngle2 + normalizedAngle1;
    }
    else {
        // The point is on the larger arc
        angle = 2 * PI - (normalizedAngle2 + normalizedAngle1);
    }

    // (Pi*r*(angle1+angle2))/180 - degrees
    // r * (angle1+angle2) - radians
    return r * angle;
}


void geometry::Arch::setAngles(double angle1, double angle2) {

    // Not to confuse the user (just left and right with positive numbers)
    if (angle1 < 0 || angle2 < 0) {
        throw std::invalid_argument("Angle must be greater than 0!");
    }
    if (angle1 + angle2 > 2 * PI) {
        throw std::invalid_argument("Invalid degree 0!");
    }
    this->angle1 = angle1;
    this->angle2 = angle2;
}



void geometry::Arch::setPoint(Point2d point) {
    
    if (std::isnan(this->angle1) || std::isnan(this->angle2)) {
        std::cerr << "Set the angles first!" << std::endl;
        return;
    }
    else {
        // Get start and end points
        // to count from the point (1,0) (on the unit circle)
        double angle_1 = this->angle1 + PI / 2;
        double angle_2 = PI / 2 - this->angle2;

        // Normalize angles to the range [0, 2*PI]
        angle_1 = this->normalizeAngle(angle_1);
        angle_2 = this->normalizeAngle(angle_2);

        // Helper function to convert polar coordinates to Cartesian
        auto polarToCartesian = [this](double angle) -> Point2d {
            return { center.x() + r * cos(angle), center.y() + r * sin(angle) };
            };

        // Get the points at the start and end angles
        Point2d startPoint = polarToCartesian(angle_1);
        Point2d endPoint = polarToCartesian(angle_2);
        
        // The point should be on the circle
        if (this->distanceBetween(point, center) != r) {
            std::cerr << "The point is not on the circle!" << std::endl;
            return;
        }
        // it should be equal neither to start nor to end 
        if (point == startPoint || point == endPoint) {
            std::cerr << "The point is invalid!" << std::endl;
            return;
        }
        
        this->point = point;
    } 
}


// Helper function to calculate angular difference between two angles in radians
double AngularDiffSigned(double theta1, double theta2) {
    double dif = theta2 - theta1;
    while (dif >= 2 * PI)  // Wrap angle difference within [0, 2*PI]
        dif -= 2 * PI;
    while (dif <= 0)
        dif += 2 * PI;
    return dif;
}

// Helper function to check if angles are in clockwise sequence
bool AnglesInClockwiseSequence(double x, double y, double z) {
    return AngularDiffSigned(x, y) + AngularDiffSigned(y, z) < 2 * PI;
}

std::vector<geometry::Point2d> geometry::Arch::boundingBox() {
    std::vector<Point2d> res;

    double angle_1 = this->normalizeAngle(this->angle1  + PI / 2);
    double angle_2 = this->normalizeAngle(PI / 2 -  this->angle2);

    // Helper function to convert polar coordinates to Cartesian
    auto polarToCartesian = [this](double angle) -> Point2d {
        return { center.x() + r * cos(angle), center.y() + r * sin(angle) };
        };

    // Get the points at the start and end angles
    Point2d startPoint = polarToCartesian(angle_1);
    Point2d endPoint = polarToCartesian(angle_2);


    

    // Initialize bounding box with the endpoints
    double x1 = startPoint.x();
    double y1 = startPoint.y();
    double x2 = x1, y2 = y1;

    // Adjust for second endpoint F
    if (endPoint.x() < x1) x1 = endPoint.x();
    if (endPoint.x() > x2) x2 = endPoint.x();
    if (endPoint.y() < y1) y1 = endPoint.y();
    if (endPoint.y() > y2) y2 = endPoint.y();

    // Calculate angles for the points E and F relative to center C
    double thetaStart = this->calculateAngle(startPoint);
    double thetaEnd = this->calculateAngle(endPoint);

    // Check for rightmost (theta = 0)
    if (AnglesInClockwiseSequence(thetaStart, 0, thetaEnd)) {
        double x = center.x() + r;
        if (x > x2) x2 = x;
    }
    // Check for bottommost (theta = PI/2)
    if (AnglesInClockwiseSequence(thetaStart, PI / 2, thetaEnd)) {
        double y = center.y() + r;
        if (y > y2) y2 = y;
    }

    // Check for leftmost (theta = PI)
    if (AnglesInClockwiseSequence(thetaStart, PI, thetaEnd)) {
        double x = center.x() - r;
        if (x < x1) x1 = x;
    }

    // Check for topmost (theta = 3*PI/2)
    if (AnglesInClockwiseSequence(thetaStart, 3 * PI / 2, thetaEnd)) {
        double y = center.y() - r;
        if (y < y1) y1 = y;
    }

    // Calculate the four corners of the bounding box
    Point2d bottomLeft(x1, y1);
    Point2d bottomRight(x2, y1);
    Point2d topLeft(x1, y2);
    Point2d topRight(x2, y2);

    res.push_back(bottomLeft);
    res.push_back(bottomRight);
    res.push_back(topLeft);
    res.push_back(topRight);
    // Return bounding box as a rectangle
    return res;

}

