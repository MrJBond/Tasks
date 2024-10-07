#include "WDraw.h"
#include <stdio.h>

WDraw::WDraw()
{
}

WDraw::~WDraw()
{
}

void WDraw::drawSegment (const geometry::Point2d& p1, const geometry::Point2d& p2) {
    printf("(%f, %f) - (%f, %f)\n", p1.x(), p1.y(), p2.x(), p2.y());
}

void WDraw::drawText(const char* text) {
    printf("%s\n", text);
}
void WDraw::drawObject(Shape* obj) {

    if (obj == nullptr) {
        std::cerr << "Can not print null object!" << std::endl;
        return;
    }
    std::vector<Point2d> points;
    if (Arch* a = dynamic_cast<Arch*>(obj); a != nullptr) { // Arch before Circle otherwise it would cast Arch obj to Circle
        this->drawText("The arch with center: ");
        Point2d center = a->getCenter();
        double radius = a->getRadius();
        std::cout << center;
        this->drawText("Radius: ");
        std::cout << radius << std::endl;
        this->drawText("Angles: ");
        std::cout << a->getAngles();
        this->drawText("Control point: ");
        std::cout << a->getControlPoint();
    }
    else if (Circle* c = dynamic_cast<Circle*>(obj); c != nullptr) {
        // special case for circle
        this->drawText("\nThe circle with center: ");
        Point2d center = c->getCenter();
        double radius = c->getRadius();
        std::cout << center;
        this->drawText("And radius: ");
        std::cout << radius << std::endl;
        std::cout << std::endl;
    }
    else if (UnknownType* ut = dynamic_cast<UnknownType*>(obj); ut != nullptr) {
        this->drawText("\nThe unknown type : ");
        std::vector<double> data = ut->getData();
        for (const auto& e : data) {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
    else {
        points = obj->getPoints();
    }
    if (Polygon* polygon = dynamic_cast<Polygon*>(obj); polygon != nullptr) { // Polygon before Polyline otherwise it would cast Polygon obj to Polyline
        this->drawText("\nThe lines of the polygon : ");
        // Print the lines
        for (int i = 1; i < points.size(); ++i) { // Suppose the points are in the right order 
            this->drawSegment(points[i - 1], points[i]);
        }
        // The last segment
        this->drawSegment(points[points.size() - 1], points[0]);
    }
    else if (Polyline* poly = dynamic_cast<Polyline*>(obj); poly != nullptr) {
        this->drawText("\nThe lines of the polyline : ");
        // Print the lines
        for (int i = 1; i < points.size(); ++i) { // Suppose the points are in the right order 
            this->drawSegment(points[i - 1], points[i]);
        }
    }
    else if (Rect* rect = dynamic_cast<Rect*>(obj); rect != nullptr) {
        this->drawText("\nThe lines of the rectangle : ");
        // Print the lines
        for (int i = 1; i < points.size(); ++i) { // Suppose the points are in the right order 
            this->drawSegment(points[i - 1], points[i]);
        }
        // The last segment
        this->drawSegment(points[points.size() - 1], points[0]);
    }
    std::cout << std::endl << std::endl;
}