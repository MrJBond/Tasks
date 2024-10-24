#pragma once
#include <vector>
#include "Point2d.h"

#define PI acos(-1)

namespace geometry {

    class Shape
    {
    public:
        Shape() = default;
        virtual ~Shape() = default;
        virtual std::vector<Point2d> getPoints() const = 0;

        virtual bool isValid() const = 0;
        virtual std::vector<Point2d> boundingBox() const = 0;
        virtual double perimeter() const = 0;
        virtual int getType() const = 0;
        virtual Point2d getCenter() = 0;
    protected:
        void boundingBoxHelper(double minY, double minX, double maxY, double maxX, std::vector<Point2d>& res) const;

        // Helper function to calculate distance between two points
        double distanceBetween(const Point2d& p1, const Point2d& p2) const {
            return std::hypot(p2.x() - p1.x(), p2.y() - p1.y());
        }
        Point2d m_center = Point2d(0., 0.);

    private:
        
    };

}