#pragma once
#include "Shape.h"

namespace geometry {

    class Polyline : public Shape {

    protected:
        std::vector<Point2d> m_points;

    public:
        Polyline() = default;
        virtual ~Polyline() = default;

        std::vector<Point2d> getPoints() const override;
        void addPoint(const Point2d& point);
        void removePoint(size_t index);
        double perimeter() const override;
        size_t getPointCount() const;
        Point2d getPoint(size_t index) const;
        void clear();

        void set() { return; } // recursive base
        // To pass any number of points
        template<class T, class ...Points>
        void set(T p, Points ... points) {
            addPoint(p);
            set(points...);
        }

        // Check if the polyline is valid (needs at least 2 points)
        bool isValid() const override;

        std::vector<Point2d> boundingBox() const override;

        int getType() const override {
            return Objects::POLYLINE;
        }

        Point2d getCenter() override;
    };

}