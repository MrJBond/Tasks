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

// Helpers for draw
double calcTheta(Circle* c, int i, int numSegments) {
    if (Arch* a = dynamic_cast<Arch*>(c); a != nullptr) {
        double theta = a->getNormalizedAngles().first + i * (a->getNormalizedAngles().second - a->getNormalizedAngles().first) / numSegments;
        a->getPointAngle() < PI ? theta *= (-1) : theta; // check control point
        return theta; // angle in radians
    }
    // for circle
    return 2.0 * PI * double(i) / double(numSegments); // angle in radians
}
std::pair<float, float> circlePoint(Circle* c, double theta) {
    Point2d center = c->getCenter();
    double radius = c->getRadius();

    // Calculate the x and y coordinates of the point on the arc
    float x = radius * cosf(theta) + center.x(); // x coordinate
    float y = radius * sinf(theta) + center.y(); // y coordinate

    return std::make_pair(x,y);
}
// These two will be passed as params
void WDraw::drawOpenGl(Shape* obj, sf::RenderWindow* window) {

    if (obj == nullptr) {
        std::cerr << "OpenGl: Object to draw is NULL!" << std::endl;
        return;
    }
    
    std::vector<Point2d> points;

    // just to make sure (we will need c in calcTheta and circlePoint)
    if (Circle* c = dynamic_cast<Circle*>(obj); c != nullptr) {
        int glConnection = GL_LINE_LOOP; // Use GL_LINE_LOOP to connect points in a loop (for circle)

        // if this is an Arch
        if (obj->getType() == Objects::ARCH) {
            glConnection = GL_LINE_STRIP; // Use GL_LINE_STRIP not to connect the first and the last (for Arch)
        }
        glBegin(glConnection);

        int numSegments = 200;
        for (int i = 0; i <= numSegments; ++i) {
            // Linearly interpolate the angle between start and end
            double theta = calcTheta(c, i, numSegments);

            std::pair<float, float> p = circlePoint(c, theta);
            float x = p.first;
            float y = p.second;
            glVertex2f(x, y);
        }
        glEnd();
    } // we don't pass it anyway but just to make sure
    else if (obj->getType() == Objects::UNKNOWN) {
        throw std::runtime_error("Unknown type doesn't have points!");
    }
    else { // in this case begin to draw as a polyline 
        points = obj->getPoints();
        glBegin(GL_LINE_STRIP); // without connecting the first and the last 
        for (const auto& p : points) {
            glVertex2f(p.x(), p.y());
        }
        // End the drawing
        glEnd();
        glFlush();
    }

    // in this case draw the last segment
    if (obj->getType() == Objects::POLYGON || obj->getType() == Objects::RECT) {
        glBegin(GL_LINES);
        glVertex2f(points[0].x(), points[0].y());
        glVertex2f(points[points.size() - 1].x(), points[points.size() - 1].y());
        glEnd();
        glFlush();
    }
}
void WDraw::drawSFMLline(SFML_line& line, double x, double y, double x2, double y2, float scaleFactor) {
    float scaleFactorY = -scaleFactor; // Y is reversed in SFML
    // Scale the points
    line.vertex1 = sf::Vertex(sf::Vector2f(x * scaleFactor, y * scaleFactorY));
    line.vertex2 = sf::Vertex(sf::Vector2f(x2 * scaleFactor, y2 * scaleFactorY));
}
void WDraw::drawSFML(Shape* obj, sf::RenderWindow* window) {

    if (obj == nullptr) {
        std::cerr << "SFML: Object to draw is NULL!" << std::endl;
        return;
    }
    
    std::vector<Point2d> points;
    float scaleFactor = 20.0f;

    // object in SFML is a set of lines
    std::vector<WDraw::SFML_line> objSFML;
    // just to make sure (we will need c in calcTheta and circlePoint)
    if (Circle* c = dynamic_cast<Circle*>(obj); c != nullptr) {
        Point2d center = c->getCenter();
        double radius = c->getRadius();

        int numSegments = 200;
        for (int i = 0; i <= numSegments; ++i) {
            // Linearly interpolate the angle between start and end
            double theta = calcTheta(c,i, numSegments);
            std::pair<float, float> p = circlePoint(c, theta);
            float x = p.first;
            float y = p.second;
            // second point (will be use for SFML)
            std::pair<float, float> p2 = circlePoint(c, theta + 0.05);
            float x2 = p2.first;
            float y2 = p2.second;
            SFML_line line;
            // Scale the points
            drawSFMLline(line, x, y, x2, y2, scaleFactor);
            objSFML.push_back(line);
        }
    } // we don't pass it anyway but just to make sure
    else if (obj->getType() == Objects::UNKNOWN) {
        throw std::runtime_error("Unknown type doesn't have points!");
    }
    else { // in this case begin to draw as a polyline 
        points = obj->getPoints();
        for (int i = 1; i < points.size(); ++i) { // Suppose the points are in the right order 
            SFML_line line;
            // Scale the points
            drawSFMLline(line, points[i - 1].x(), points[i - 1].y(), points[i].x(), points[i].y(), scaleFactor);
            objSFML.push_back(line);
        }
    }

    // in this case draw the last segment
    if (obj->getType() == Objects::POLYGON || obj->getType() == Objects::RECT) {
        // draw the last line
        SFML_line line;
        drawSFMLline(line, points[0].x(), points[0].y(), points[points.size() - 1].x(), points[points.size() - 1].y(), scaleFactor);
        objSFML.push_back(line);
    }

        for (WDraw::SFML_line l : objSFML) {
            sf::Vertex line[] = { l.vertex1, l.vertex2 };
            (*window).draw(line, 2, sf::Lines); // draw every line
        }
  }

void WDraw::drawUnknownType(UnknownType* ut) {
    this->drawText("\nThe unknown type : ");
    std::vector<double> data = ut->getData();
    for (const auto& e : data) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
    // Unknown does not have points so we don't use graphics here
}

void WDraw::drawPolyline(Shape* polyline, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f) {
    std::vector<Point2d> points = polyline->getPoints();
    // Print the lines
    for (int i = 1; i < points.size(); ++i) { // Suppose the points are in the right order 
        drawSegment(points[i - 1], points[i]);
    }
    if (f != nullptr)
        f(polyline, w);
    
}
void WDraw::drawPolygon(Shape* polygon, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f) {
    std::vector<Point2d> points = polygon->getPoints();
    // Draw the same thing as polyline
    drawPolyline(polygon, w, f);

    // Draw the last segment
    drawSegment(points[points.size() - 1], points[0]);
}
void WDraw::drawCircle(Circle* c, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f) {
    Point2d center = c->getCenter();
    double radius = c->getRadius();
    std::cout << center;
    drawText("And radius: ");
    std::cout << radius << std::endl;
    std::cout << std::endl;

    // if this is an Arch
    if (Arch* a = dynamic_cast<Arch*>(c); a != nullptr) {
        drawText("Angles: ");
        std::cout << a->getAngles();
        drawText("Control point: ");
        std::cout << a->getControlPoint();
        if(f != nullptr)
            f(a, w);
    }
    else {
        if (f != nullptr)
            f(c, w);
    }
}
void WDraw::drawArch(Arch* a, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f) {
    drawCircle(a, w, f);
}

void WDraw::drawObject(Shape* obj, sf::RenderWindow* w, std::function<void(Shape*, sf::RenderWindow*)> f) {

    if (obj == nullptr) {
        std::cerr << "Can not print null object!" << std::endl;
        return;
    }
    
    std::vector<Point2d> points;

    if (Arch* a = dynamic_cast<Arch*>(obj); a != nullptr) { // Arch before Circle otherwise it would cast Arch obj to Circle
        drawText("The arch with center: ");
        drawArch(a, w, f);
    }
    else if (Circle* c = dynamic_cast<Circle*>(obj); c != nullptr) {
        // special case for circle
        drawText("\nThe circle with center: ");
        drawCircle(c, w, f);
    }
    else if (UnknownType* ut = dynamic_cast<UnknownType*>(obj); ut != nullptr) {
        drawUnknownType(ut);
    }
    if (Polygon* polygon = dynamic_cast<Polygon*>(obj); polygon != nullptr) { // Polygon before Polyline otherwise it would cast Polygon obj to Polyline
        drawText("\nThe lines of the polygon : ");
        drawPolygon(polygon,w, f);
    }
    else if (Polyline* poly = dynamic_cast<Polyline*>(obj); poly != nullptr) {
        drawText("\nThe lines of the polyline : ");
        drawPolyline(poly, w, f);
    }
    else if (Rect* rect = dynamic_cast<Rect*>(obj); rect != nullptr) {
        drawText("\nThe lines of the rectangle : ");
        // The same as polygon
        drawPolygon(rect, w, f);
    }
    std::cout << std::endl << std::endl;
}

/************************************************************
                    init for OpenGL
*************************************************************/

int WDraw::initGraph(const std::vector<Shape*>& shapes) {

    GLFWwindow* window;

    // Initialize the library 
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context 
    window = glfwCreateWindow(1000, 600, "Geometry", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current 
    glfwMakeContextCurrent(window);

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        // Render here 
        glClear(GL_COLOR_BUFFER_BIT);


        // Set background color using normalized RGB values
        glClearColor(106.0f / 255.0f, 90.0f / 255.0f, 205.0f / 255.0f, 1.0f);  // Purple color
        glClear(GL_COLOR_BUFFER_BIT);

        double zoomFactor = 0.02; // scale

        // draw objects
        for (const auto& s : shapes) {

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glScalef(zoomFactor, zoomFactor, 1.0f); 

            // window is for SFML so nullptr here
            // pass lambda to be able to use this->
            drawObject(s, nullptr, [&](Shape* obj, sf::RenderWindow* w) {

                try {
                    drawOpenGl(obj, w);
                }
                catch (const std::runtime_error& e) {
                    std::cout << e.what();
                }
                });
        }
        // Without this, the newly rendered frame would not be displayed

        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // It ensures that the program responds to user input and other system-generated events
        // Poll for and process events 
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


/************************************************************
                    init for SFML
*************************************************************/

void WDraw::initGraphSFML(const std::vector<Shape*>& shapes) {

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML");

    // Create a view that will be applied to the window
    sf::View view = window.getDefaultView();

    // scale is negative because:

    /*
    OpenGL:
    Y-Axis: Increases upwards

    SFML:
    Y-Axis: Increases downwards
    */

    //float scaleFactor = 20.0f;
    view.zoom(4.5f);

    // Set the view to the window
   window.setView(view);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        for (const auto& shape : shapes) {
            // pass lambda to be able to use this->
            drawObject(shape, &window, [&](Shape* obj, sf::RenderWindow* window) {

                try {
                    drawSFML(obj, window);
                }
                catch (const std::runtime_error& e) {
                    std::cout << e.what();
                }
                });
        }
        window.display();
    }
}