#pragma once
#include "Shape.h"

namespace geometry {

    class Circle : public Shape
    {
    protected:
        double m_r = 0.0;
       // Point2d m_center = Point2d(0.0, 0.0);
    public:
        Circle() = default;
        Circle(Point2d center, double r) /* :m_center(center)*/ {
            set(center, r);
        }
        virtual ~Circle() = default;

        double getRadius() const {
            return m_r;
        }

        void set(Point2d p, double r);
        Point2d getCenter() override{
            return m_center;
        }
        bool isValid() const override { if (m_r <= 0) { return false; } return true; }

        std::vector<Point2d> boundingBox() const override;
        double perimeter() const override;

        // Override it to prevent the class from being abstract
        std::vector<Point2d> getPoints() const override {
            std::vector<Point2d> v;
            std::cout << "Circle and Arch have no points, only the center!";
            return v;
        }
        int getType() const override {
            return Objects::CIRCLE;
        }
    };

}