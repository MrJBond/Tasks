#pragma once
#include "Shape.h"

namespace geometry {

    class Rect : public Shape {
    private:
        Point2d m_p1 = Point2d(0.0, 0.0);
        Point2d m_p2 = Point2d(0.0, 0.0);
        Point2d m_p3 = Point2d(0.0, 0.0);
        Point2d m_p4 = Point2d(0.0, 0.0);
    public:
        Rect() = default;
        Rect(Point2d p1, Point2d p2, Point2d p3, Point2d p4) {
            set(p1, p2, p3, p4);
        }
        ~Rect() = default;

        std::vector<Point2d> getPoints() const override;

        void set(Point2d p1, Point2d p2, Point2d p3, Point2d p4);
        void set(Point2d p1, Point2d p3);
        bool isValid() const override;

        std::vector<Point2d> boundingBox() const override;
        double perimeter() const override;

        Point2d getCenter() override;

        int getType() const override {
            return Objects::RECT;
        }
    };

}