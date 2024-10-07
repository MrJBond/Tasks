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
        virtual std::vector<Point2d> getPoints() = 0;

        virtual bool isValid() = 0;
        virtual std::vector<Point2d> boundingBox() = 0;
        virtual double perimeter() = 0;
    protected:
        void boundingBoxHelper(double minY, double minX, double maxY, double maxX, std::vector<Point2d>& res);

        // Helper function to calculate distance between two points
        double distanceBetween(const Point2d& p1, const Point2d& p2) const {
            return std::hypot(p2.x() - p1.x(), p2.y() - p1.y());
        }
    private:

    };

}