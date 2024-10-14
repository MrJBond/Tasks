#pragma once
#include "Shape.h"
#include "Circle.h"
#include "Rect.h"
#include "Arch.h"
#include "Polygon.h"
#include "Polyline.h"
#include "UnknownType.h"
#include <GLFW/glfw3.h>
#include <SFML\Graphics.hpp>



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

   
    int initGraph(const std::vector<Shape*>& shapes);

    // SFML
    void initGraphSFML(const std::vector<Shape*>& shapes);
    // we will be working with lines because standard shapes don't provide points 
    struct SFML_line { sf::Vertex vertex1; sf::Vertex vertex2; };

    // object in SFML is a set of lines and a scal factor for this object
    // default values when not dealing with SFML
    void drawObject(Shape* obj, std::vector<SFML_line>* objSFML = nullptr, float scaleFactor = 1.0);
    
};
