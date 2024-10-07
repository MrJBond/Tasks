#pragma once
#include "Shape.h"

namespace geometry {

    class Rect : public Shape {
    private:
        Point2d p1;
        Point2d p2;
        Point2d p3;
        Point2d p4;
    public:
        Rect() = default;
        Rect(Point2d p1, Point2d p2, Point2d p3, Point2d p4) {
            this->set(p1, p2, p3, p4);
        }
        ~Rect() = default;

        std::vector<Point2d> getPoints() override;

        void set(Point2d p1, Point2d p2, Point2d p3, Point2d p4);
        void set(Point2d p1, Point2d p3);
        bool isValid() override;

        std::vector<Point2d> boundingBox() override;
        double perimeter() override;

    };

}