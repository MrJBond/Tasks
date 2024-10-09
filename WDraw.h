#pragma once
#include "Shape.h"
#include "Circle.h"
#include "Rect.h"
#include "Arch.h"
#include "Polygon.h"
#include "Polyline.h"
#include "UnknownType.h"
#include <GLFW/glfw3.h>


using namespace geometry;
class WDraw
{

public:

    WDraw();
    virtual ~WDraw();

    /************************************************************************/
    /* Основной метод отрисовки                                             */
    /************************************************************************/
    virtual void drawSegment(const Point2d& p1, const Point2d& p2);

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
    virtual void drawText(const char* text);

    void drawObject(Shape* obj);
    int initGraph(const std::vector<Shape*>& shapes);
};
