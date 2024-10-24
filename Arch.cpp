#include "Arch.h"


// Helper function to calculate the angle of a point relative to the center
double geometry::Arch::calculateAngle(const Point2d& p ) const{
    double dx = p.x() - m_center.x();
    double dy = p.y() - m_center.y();
    return atan2(dy, dx);  // Returns angle in radians
}
double geometry::Arch::normalizeAngle(double angle) const{
    while (angle >= 2 * PI) {  // Reduce angle when it exceeds 2 * PI
        angle -= 2 * PI;
    }
    while (angle < 0) {        // Increase angle when it is negative
        angle += 2 * PI;
    }
    return angle;
}
double geometry::Arch::perimeter() const{

    // Calculate the angle of the point relative to the center
    double pointAngle = calculateAngle(m_point);

    // Normalize the angle to the range [0, 2*PI]
    pointAngle = normalizeAngle(pointAngle);

    if (m_nomalizedAngle1 <= PI && m_nomalizedAngle2 <= PI && pointAngle > m_nomalizedAngle1 && pointAngle < m_nomalizedAngle2 && m_nomalizedAngle1 < m_nomalizedAngle2) {
        return m_r * (m_nomalizedAngle2 - m_nomalizedAngle1);
    }
    else if (m_nomalizedAngle1 <= PI && m_nomalizedAngle2 <= PI && pointAngle < m_nomalizedAngle1 && pointAngle > m_nomalizedAngle2 && m_nomalizedAngle1 > m_nomalizedAngle2) {
        return m_r * abs(m_nomalizedAngle1 - m_nomalizedAngle2);
    }
    else if (m_nomalizedAngle1 > PI && m_nomalizedAngle2 > PI && pointAngle > m_nomalizedAngle1 && pointAngle < m_nomalizedAngle2 && m_nomalizedAngle1 < m_nomalizedAngle2) {
        return m_r * (m_nomalizedAngle2 - m_nomalizedAngle1);
    }
    else if (m_nomalizedAngle1 <= PI && m_nomalizedAngle2 > PI && pointAngle > m_nomalizedAngle1 && pointAngle < (m_nomalizedAngle2 - 2 * PI)) {
        return m_r * (m_nomalizedAngle2 - m_nomalizedAngle1);
    }
    else if (m_nomalizedAngle1 > PI && m_nomalizedAngle2 <= PI && (pointAngle > m_nomalizedAngle1 || pointAngle < m_nomalizedAngle2)) {
        return m_r * (abs(m_nomalizedAngle1 - m_nomalizedAngle2) + 2 * PI); // Wrap around the circle
    }
    else if (m_nomalizedAngle1 <= PI && m_nomalizedAngle2 <= PI && (pointAngle > m_nomalizedAngle1 && pointAngle > m_nomalizedAngle2)) {
        return m_r * (2*PI - abs(m_nomalizedAngle1 - m_nomalizedAngle2));
    }

    // (Pi*r*(angle1+angle2))/180 - degrees
    // r * (angle1+angle2) - radians
    return 0; // default
}


void geometry::Arch::setAngles(double angle_1, double angle_2) {

    // first - left
    // second - right
    m_angle1 = angle_1;
    m_angle2 = angle_2;
    angle_1 = angle_1 + PI / 2;
    angle_2 = PI / 2 - angle_2;
    m_nomalizedAngle1 = normalizeAngle(angle_1);
    m_nomalizedAngle2 = normalizeAngle(angle_2);
}



void geometry::Arch::setPoint(Point2d point) {
    
    if (std::isnan(m_angle1) || std::isnan(m_angle2)) {
        std::cerr << "Set the angles first!" << std::endl;
        return;
    }
    else {
        // Helper function to convert polar coordinates to Cartesian
        auto polarToCartesian = [this](double angle) -> Point2d {
            return { m_center.x() + m_r * cos(angle), m_center.y() + m_r * sin(angle) };
            };

        // Get the points at the start and end angles
        const Point2d startPoint = polarToCartesian(m_nomalizedAngle1);
        const Point2d endPoint = polarToCartesian(m_nomalizedAngle2);

        // The point should be on the circle
        const double dis = distanceBetween(point, m_center);
        if(!isEqualDouble(dis, m_r, 1e-4)){
            std::cerr << "The point is not on the circle!\n" << 
                "Distance between the center and the control point is " << dis << std::endl;
            std::cerr << "The radius is " << m_r << std::endl;
            return;
        }
        // it should be equal neither to start nor to end 
        if (point == startPoint || point == endPoint) {

            std::cout << startPoint << endPoint << point;

            std::cerr << "The point is invalid!" << std::endl;
            return;
        }
        
        m_point = point;
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
std::vector<geometry::Point2d> geometry::Arch::boundingBox() const{
    std::vector<Point2d> res(4);

    // Helper function to convert polar coordinates to Cartesian
    auto polarToCartesian = [this](double angle) -> Point2d {
        return { m_center.x() + m_r * cos(angle), m_center.y() + m_r * sin(angle) };
        };

    // Get the points at the start and end angles
    const Point2d startPoint = polarToCartesian(m_nomalizedAngle1);
    const Point2d endPoint = polarToCartesian(m_nomalizedAngle2);

    // Initialize bounding box with the start and end points
    double x1 = std::min(startPoint.x(), endPoint.x());
    double y1 = std::min(startPoint.y(), endPoint.y());
    double x2 = std::max(startPoint.x(), endPoint.x());
    double y2 = std::max(startPoint.y(), endPoint.y());

    // Calculate the control point angle relative to the center
    const double controlAngle = calculateAngle(m_point);

    // Determine which portion of the arc to consider
    const double thetaStart = calculateAngle(startPoint);
    const double thetaEnd = calculateAngle(endPoint);

    // Handle both clockwise and counterclockwise cases based on the control point
    const bool isClockwise = (AngularDiffSigned(thetaStart, controlAngle) > 0);

    if (isClockwise) {
        // Clockwise case: Start angle to end angle, considering rightmost, bottommost, etc.

        // Check for rightmost (theta = 0)
        if (AnglesInClockwiseSequence(thetaStart, 0, thetaEnd)) {
            double x = m_center.x() + m_r;
            if (x > x2) x2 = x;
        }
        // Check for bottommost (theta = PI/2)
        if (AnglesInClockwiseSequence(thetaStart, PI / 2, thetaEnd)) {
            double y = m_center.y() + m_r;
            if (y > y2) y2 = y;
        }
        // Check for leftmost (theta = PI)
        if (AnglesInClockwiseSequence(thetaStart, PI, thetaEnd)) {
            double x = m_center.x() - m_r;
            if (x < x1) x1 = x;
        }
        // Check for topmost (theta = 3*PI/2)
        if (AnglesInClockwiseSequence(thetaStart, 3 * PI / 2, thetaEnd)) {
            double y = m_center.y() - m_r;
            if (y < y1) y1 = y;
        }
    }
    else {
        // Counterclockwise case: Adjust bounding box calculations similarly

        // Check for rightmost (theta = 0)
        if (AnglesInClockwiseSequence(thetaEnd, 0, thetaStart)) {
            double x = m_center.x() + m_r;
            if (x > x2) x2 = x;
        }
        // Check for bottommost (theta = PI/2)
        if (AnglesInClockwiseSequence(thetaEnd, PI / 2, thetaStart)) {
            double y = m_center.y() + m_r;
            if (y > y2) y2 = y;
        }
        // Check for leftmost (theta = PI)
        if (AnglesInClockwiseSequence(thetaEnd, PI, thetaStart)) {
            double x = m_center.x() - m_r;
            if (x < x1) x1 = x;
        }
        // Check for topmost (theta = 3*PI/2)
        if (AnglesInClockwiseSequence(thetaEnd, 3 * PI / 2, thetaStart)) {
            double y = m_center.y() - m_r;
            if (y < y1) y1 = y;
        }
    }

    // Add the final bounding box points
    const Point2d bottomLeft(x1, y1);
    const Point2d bottomRight(x2, y1);
    const Point2d topLeft(x1, y2);
    const Point2d topRight(x2, y2);

    res[0] = bottomLeft;
    res[1] = bottomRight;
    res[2] = topLeft;
    res[3] = topRight;

    return res;
}
