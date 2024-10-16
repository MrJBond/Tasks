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
#include <functional> // for std::function



using namespace geometry;
class WDraw
{
private:
    /************************************************************************/
    /* Основной метод отрисовки                                             */
    /************************************************************************/
    virtual void drawSegment(const Point2d& p1, const Point2d& p2);

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
    virtual void drawText(const char* text);

    void drawCircle(Circle* c, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f);
    void drawArch(Arch* a, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f);
    void drawUnknownType(UnknownType* ut);

    void drawPolyline(Shape* polyline, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f);
    void drawPolygon(Shape* polygon, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f);

    // we need this window for SFML and we also want these functions to have the same params
    void drawOpenGl(Shape* obj, sf::RenderWindow* window = nullptr);
    void drawSFML(Shape* obj, sf::RenderWindow* window);

    // we will be working with lines because standard shapes don't provide points 
    struct SFML_line { sf::Vertex vertex1; sf::Vertex vertex2; };
    void drawSFMLline(SFML_line& line, double x, double y, double x2, double y2, float scaleFactor);
public:

    WDraw();
    virtual ~WDraw();

    // OpenGl
    int initGraph(const std::vector<Shape*>& shapes);
    // SFML
    void initGraphSFML(const std::vector<Shape*>& shapes);

    void drawObject(Shape* obj, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f);
};
