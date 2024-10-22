#pragma once
#include "Shape.h"

namespace geometry {

    class Polyline : public Shape {

    protected:
        std::vector<Point2d> points;

    public:
        Polyline() = default;
        virtual ~Polyline() = default;

        std::vector<Point2d> getPoints() override;
        void addPoint(const Point2d& point);
        void removePoint(size_t index);
        double perimeter() override;
        size_t getPointCount() const;
        Point2d getPoint(size_t index) const;
        void clear();

        void set() { return; } // recursive base
        // To pass any number of points
        template<class T, class ...Points>
        void set(T p, Points ... points) {
            this->addPoint(p);
            set(points...);
        }

        // Check if the polyline is valid (needs at least 2 points)
        bool isValid() override;

        std::vector<Point2d> boundingBox() override;

    };

}