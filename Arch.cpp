#include "Arch.h"


// Helper function to calculate the angle of a point relative to the center
double geometry::Arch::calculateAngle(const Point2d& p ){
    double dx = p.x() - center.x();
    double dy = p.y() - center.y();
    return atan2(dy, dx);  // Returns angle in radians
}
double geometry::Arch::normalizeAngle(double angle) {
    while (angle >= 2 * PI) {  // Reduce angle when it exceeds 2 * PI
        angle -= 2 * PI;
    }
    while (angle < 0) {        // Increase angle when it is negative
        angle += 2 * PI;
    }
    return angle;
}
double geometry::Arch::perimeter() {

    // Calculate the angle of the point relative to the center
    double pointAngle = calculateAngle(this->point);

    // Normalize the angle to the range [0, 2*PI]
    pointAngle = this->normalizeAngle(pointAngle);

    if (nomalizedAngle1 <= PI && nomalizedAngle2 <= PI && pointAngle > nomalizedAngle1 && pointAngle < nomalizedAngle2 && nomalizedAngle1 < nomalizedAngle2) {
        return r * (nomalizedAngle2 - nomalizedAngle1);
    }
    else if (nomalizedAngle1 <= PI && nomalizedAngle2 <= PI && pointAngle < nomalizedAngle1 && pointAngle > nomalizedAngle2 && nomalizedAngle1 > nomalizedAngle2) {
        return r * abs(nomalizedAngle1 - nomalizedAngle2);
    }
    else if (nomalizedAngle1 > PI && nomalizedAngle2 > PI && pointAngle > nomalizedAngle1 && pointAngle < nomalizedAngle2 && nomalizedAngle1 < nomalizedAngle2) {
        return r * (nomalizedAngle2 - nomalizedAngle1);
    }
    else if (nomalizedAngle1 <= PI && nomalizedAngle2 > PI && pointAngle > nomalizedAngle1 && pointAngle < (nomalizedAngle2 - 2 * PI)) {
        return r * (nomalizedAngle2 - nomalizedAngle1);
    }
    else if (nomalizedAngle1 > PI && nomalizedAngle2 <= PI && (pointAngle > nomalizedAngle1 || pointAngle < nomalizedAngle2)) {
        return r * (abs(nomalizedAngle1 - nomalizedAngle2) + 2 * PI); // Wrap around the circle
    }
    else if (nomalizedAngle1 <= PI && nomalizedAngle2 <= PI && (pointAngle > nomalizedAngle1 && pointAngle > nomalizedAngle2)) {
        return r * (2*PI - abs(nomalizedAngle1 - nomalizedAngle2));
    }

    // (Pi*r*(angle1+angle2))/180 - degrees
    // r * (angle1+angle2) - radians
    return 0; // default
}


void geometry::Arch::setAngles(double angle_1, double angle_2) {

    // first - left
    // second - right
    this->angle1 = angle_1;
    this->angle2 = angle_2;
    angle_1 = angle_1 + PI / 2;
    angle_2 = PI / 2 - angle_2;
    this->nomalizedAngle1 = normalizeAngle(angle_1);
    this->nomalizedAngle2 = normalizeAngle(angle_2);
}



void geometry::Arch::setPoint(Point2d point) {
    
    if (std::isnan(this->angle1) || std::isnan(this->angle2)) {
        std::cerr << "Set the angles first!" << std::endl;
        return;
    }
    else {
        // Helper function to convert polar coordinates to Cartesian
        auto polarToCartesian = [this](double angle) -> Point2d {
            return { center.x() + r * cos(angle), center.y() + r * sin(angle) };
            };

        // Get the points at the start and end angles
        Point2d startPoint = polarToCartesian(nomalizedAngle1);
        Point2d endPoint = polarToCartesian(nomalizedAngle2);

        // The point should be on the circle
        if (std::round(this->distanceBetween(point, center)*10*10*10)/(10*10*10) 
            != std::round(r*10*10*10)/(10*10*10)) {

            std::cerr << "The point is not on the circle!" << std::endl;
            return;
        }
        // it should be equal neither to start nor to end 
        if (point == startPoint || point == endPoint) {

            std::cout << startPoint << endPoint << point;

            std::cerr << "The point is invalid!" << std::endl;
            return;
        }
        
        this->point = point;
    } 
}


double AngularDiffSigned(double theta1, double theta2) {
    double dif = theta2 - theta1;
    while (dif > PI)  // Adjust the difference to the range (-PI, PI]
        dif -= 2 * PI;
    while (dif <= -PI)
        dif += 2 * PI;
    return dif;
}

bool AnglesInClockwiseSequence(double start, double mid, double end) {
    double diff1 = AngularDiffSigned(start, mid);
    double diff2 = AngularDiffSigned(mid, end);
    return (diff1 >= 0 && diff2 >= 0);
}
std::vector<geometry::Point2d> geometry::Arch::boundingBox() {
    std::vector<Point2d> res;

    // Helper function to convert polar coordinates to Cartesian
    auto polarToCartesian = [this](double angle) -> Point2d {
        return { center.x() + r * cos(angle), center.y() + r * sin(angle) };
        };

    // Get the points at the start and end angles
    Point2d startPoint = polarToCartesian(nomalizedAngle1);
    Point2d endPoint = polarToCartesian(nomalizedAngle2);

    // Initialize bounding box with the start and end points
    double x1 = std::min(startPoint.x(), endPoint.x());
    double y1 = std::min(startPoint.y(), endPoint.y());
    double x2 = std::max(startPoint.x(), endPoint.x());
    double y2 = std::max(startPoint.y(), endPoint.y());

    // Calculate the control point angle relative to the center
    double controlAngle = this->calculateAngle(this->point);

    // Determine which portion of the arc to consider
    double thetaStart = this->calculateAngle(startPoint);
    double thetaEnd = this->calculateAngle(endPoint);

    // Handle both clockwise and counterclockwise cases based on the control point
    bool isClockwise = (AngularDiffSigned(thetaStart, controlAngle) > 0);

    if (isClockwise) {
        // Clockwise case: Start angle to end angle, considering rightmost, bottommost, etc.

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
    }
    else {
        // Counterclockwise case: Adjust bounding box calculations similarly

        // Check for rightmost (theta = 0)
        if (AnglesInClockwiseSequence(thetaEnd, 0, thetaStart)) {
            double x = center.x() + r;
            if (x > x2) x2 = x;
        }
        // Check for bottommost (theta = PI/2)
        if (AnglesInClockwiseSequence(thetaEnd, PI / 2, thetaStart)) {
            double y = center.y() + r;
            if (y > y2) y2 = y;
        }
        // Check for leftmost (theta = PI)
        if (AnglesInClockwiseSequence(thetaEnd, PI, thetaStart)) {
            double x = center.x() - r;
            if (x < x1) x1 = x;
        }
        // Check for topmost (theta = 3*PI/2)
        if (AnglesInClockwiseSequence(thetaEnd, 3 * PI / 2, thetaStart)) {
            double y = center.y() - r;
            if (y < y1) y1 = y;
        }
    }

    // Add the final bounding box points
    Point2d bottomLeft(x1, y1);
    Point2d bottomRight(x2, y1);
    Point2d topLeft(x1, y2);
    Point2d topRight(x2, y2);

    res.push_back(bottomLeft);
    res.push_back(bottomRight);
    res.push_back(topLeft);
    res.push_back(topRight);

    return res;
}
