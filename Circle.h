#pragma once
#include "Shape.h"

namespace geometry {

    class Circle : public Shape
    {
    protected:
        double r = 0.0;
        Point2d center = Point2d(0.0, 0.0);
    public:
        Circle() = default;
        Circle(Point2d center, double r) :center(center) {
            this->set(center, r);
        }
        virtual ~Circle() = default;

        double getRadius() const {
            return r;
        }

        void set(Point2d p, double r);
        Point2d getCenter() const {
            return this->center;
        }
        bool isValid() override { if (this->r <= 0) { return false; } return true; }

        std::vector<Point2d> boundingBox() override;
        double perimeter() override;

        // Override it to prevent the class from being abstract
        std::vector<Point2d> getPoints() override {
            std::vector<Point2d> v;
            std::cout << "Circle and Arch have no points, only the center!";
            return v;
        }
    };

}