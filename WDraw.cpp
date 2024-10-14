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

// when objSFML is nullptr, we will work with OpenGL
void WDraw::drawObject(Shape* obj, std::vector<SFML_line>* objSFML, float scaleFactor) {

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

        // Graphics
        glBegin(GL_LINE_STRIP); // Use GL_LINE_STRIP not to connect the first and the last

        int numSegments = 200;
        for (int i = 0; i <= numSegments; ++i) {
            // Linearly interpolate the angle between start and end
            double theta = a->getNormalizedAngles().first + i * (a->getNormalizedAngles().second - a->getNormalizedAngles().first) / numSegments;

            a->getPointAngle() < PI ? theta *= (-1) : theta; // check control point


            // Calculate the x and y coordinates of the point on the arc
            float x = radius * cosf(theta) + center.x(); // x coordinate
            float y = radius * sinf(theta) + center.y(); // y coordinate
            // second point (will be use for SFML)
            float x2 = radius * cosf(theta + 0.05) + center.x(); // x coordinate
            float y2 = radius * sinf(theta + 0.05) + center.y(); // y coordinate

            ///////////////////////////////////////////////////////
            // for SFML
            //////////////////////////////////////////////////////
            if (objSFML != nullptr) {
                SFML_line line;
                // Scale the points
                line.vertex1 = sf::Vertex(sf::Vector2f(x * scaleFactor, y * scaleFactor));
                line.vertex2 = sf::Vertex(sf::Vector2f(x2 * scaleFactor, y2 * scaleFactor));
                (*objSFML).push_back(line);
            } /////////////////////////////////////////////////////////
            else { // OpenGl
                // Specify the vertex
                glVertex2f(x, y); // Move the point to the arc's center
            }
        }

        glEnd();

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

        // Graphics
        glBegin(GL_LINE_LOOP); // Use GL_LINE_LOOP to connect points in a loop

        int numSegments = 200;
        for (int i = 0; i < numSegments; ++i) {
            // Calculate angle for this segment
            float theta = 2.0f * PI * float(i) / float(numSegments); // angle in radians

            // Calculate the x and y coordinates of the point on the circle
            float x = radius * cosf(theta) + center.x(); // x coordinate
            float y = radius * sinf(theta) + center.y(); // y coordinate

            // second point (will be use for SFML)
            float x2 = radius * cosf(theta + 0.05) + center.x(); // x coordinate
            float y2 = radius * sinf(theta + 0.05) + center.y(); // y coordinate

            ///////////////////////////////////////////////////////
            // for SFML
            //////////////////////////////////////////////////////
            if (objSFML != nullptr) {
                SFML_line line;
                // Scale the points
                line.vertex1 = sf::Vertex(sf::Vector2f(x * scaleFactor, y * scaleFactor));
                line.vertex2 = sf::Vertex(sf::Vector2f(x2 * scaleFactor, y2 * scaleFactor));
                (*objSFML).push_back(line);
            } else{//////////////////////////////////////////////////////////

                // OpenGL

                // Specify the vertex
                glVertex2f(x, y); // Move point to the circle center
            }
        }

        glEnd();
    }
    else if (UnknownType* ut = dynamic_cast<UnknownType*>(obj); ut != nullptr) {
        this->drawText("\nThe unknown type : ");
        std::vector<double> data = ut->getData();
        for (const auto& e : data) {
            std::cout << e << " ";
        }
        std::cout << std::endl;

        // Unknown does not have points so we don't use graphics here
    }
    else {
        points = obj->getPoints();
    }
    if (Polygon* polygon = dynamic_cast<Polygon*>(obj); polygon != nullptr) { // Polygon before Polyline otherwise it would cast Polygon obj to Polyline
        this->drawText("\nThe lines of the polygon : ");
        // Print the lines
        for (int i = 1; i < points.size(); ++i) { // Suppose the points are in the right order 
            this->drawSegment(points[i - 1], points[i]);


            ///////////////////////////////////////////////////////
            // for SFML
            //////////////////////////////////////////////////////
            if (objSFML != nullptr) {
                SFML_line line;
                // Scale the points
                line.vertex1 = sf::Vertex(sf::Vector2f(points[i - 1].x() * scaleFactor, points[i - 1].y() * scaleFactor));
                line.vertex2 = sf::Vertex(sf::Vector2f(points[i].x() * scaleFactor, points[i].y() * scaleFactor));
                (*objSFML).push_back(line);
            }
        }
        // The last segment
        this->drawSegment(points[points.size() - 1], points[0]);


        ///////////////////////////////////////////////////////
        // for SFML
        if (objSFML != nullptr) {
            // draw the last line
            SFML_line line;
            line.vertex1 = sf::Vertex(sf::Vector2f(points[0].x() * scaleFactor, points[0].y() * scaleFactor));
            line.vertex2 = sf::Vertex(sf::Vector2f(points[points.size() - 1].x() * scaleFactor, points[points.size() - 1].y() * scaleFactor));
            (*objSFML).push_back(line);
        }
        else {
            //////////////////////////////////////////////////////
                // OpenGl

            // Graphics
            glBegin(GL_LINE_LOOP); // loop for polygon
            for (const auto& p : points) {
                glVertex2f(p.x(), p.y());
            }
            // End the drawing
            glEnd();
            glFlush();
        }
    }
    else if (Polyline* poly = dynamic_cast<Polyline*>(obj); poly != nullptr) {
        this->drawText("\nThe lines of the polyline : ");
        // Print the lines
        for (int i = 1; i < points.size(); ++i) { // Suppose the points are in the right order 
            this->drawSegment(points[i - 1], points[i]);

            ///////////////////////////////////////////////////////
            // for SFML
            //////////////////////////////////////////////////////
            if (objSFML != nullptr) {
                SFML_line line;
                // Scale the points
                line.vertex1 = sf::Vertex(sf::Vector2f(points[i - 1].x() * scaleFactor, points[i - 1].y() * scaleFactor));
                line.vertex2 = sf::Vertex(sf::Vector2f(points[i].x() * scaleFactor, points[i].y() * scaleFactor));
                (*objSFML).push_back(line);
            }
        }
        if (objSFML == nullptr) { // OpenGl
            // Graphics
            glBegin(GL_LINE_STRIP); // without connecting the first and the last 
            for (const auto& p : points) {
                glVertex2f(p.x(), p.y());
            }
            // End the drawing
            glEnd();
            glFlush();
        }
    }
    else if (Rect* rect = dynamic_cast<Rect*>(obj); rect != nullptr) {
        this->drawText("\nThe lines of the rectangle : ");
        // Print the lines
        for (int i = 1; i < points.size(); ++i) { // Suppose the points are in the right order 
            this->drawSegment(points[i - 1], points[i]);


            ///////////////////////////////////////////////////////
            // for SFML
            //////////////////////////////////////////////////////
            if (objSFML != nullptr) {
                SFML_line line;
                // Scale the points
                line.vertex1 = sf::Vertex(sf::Vector2f(points[i - 1].x() * scaleFactor, points[i - 1].y() * scaleFactor));
                line.vertex2 = sf::Vertex(sf::Vector2f(points[i].x() * scaleFactor, points[i].y() * scaleFactor));
                (*objSFML).push_back(line);
            }
        }        
        // The last segment
        this->drawSegment(points[points.size() - 1], points[0]);

        ///////////////////////////////////////////////////////
        // for SFML
        // draw the last line
        if (objSFML != nullptr) {
            SFML_line line;
            line.vertex1 = sf::Vertex(sf::Vector2f(points[0].x() * scaleFactor, points[0].y() * scaleFactor));
            line.vertex2 = sf::Vertex(sf::Vector2f(points[points.size() - 1].x() * scaleFactor, points[points.size() - 1].y() * scaleFactor));
            (*objSFML).push_back(line);
        }
        else {
            //////////////////////////////////////////////////////
            // OpenGl
            // Graphics
            glBegin(GL_LINE_LOOP); // loop for polygon
            for (const auto& p : points) {
                glVertex2f(p.x(), p.y());
            }
            // End the drawing
            glEnd();
            glFlush();
        }
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

            this->drawObject(s);
         
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

    float scaleFactor = -20.0f;
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
            std::vector<WDraw::SFML_line> obj; // pass as pointer to be able to set it to nullptr if we don't need it
            this->drawObject(shape, &obj, scaleFactor); // obj is a set of lines (because default shapes don't have points)
            for (WDraw::SFML_line l : obj) {
                sf::Vertex line[] = {l.vertex1, l.vertex2};
                window.draw(line, 2, sf::Lines); // draw every line
            }
        }
        window.display();
    }
}