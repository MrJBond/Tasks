#include "input.h"


void EnterData::menuShapes() {
	system("cls");
	std::cout << "Enter a rectangle (1)" << std::endl;
	std::cout << "Enter a  circle (2)" << std::endl;
	std::cout << "Enter an unknown type (3)" << std::endl;
	std::cout << "Enter an arch (4)" << std::endl;
	std::cout << "Enter a polygon (5)" << std::endl;
	std::cout << "Enter a polyline (6)" << std::endl;
	std::cout << "Exit (7)" << std::endl;
	std::cout << "Enter the points in clockwise or counter clockwise order!" << std::endl;
}
Point2d EnterData::enterOnePoint(size_t n) {
	std::cout << "Enter the point " << n  << ": " << std::endl << std::endl;
	double x = 0., y = 0.;
	std::cout << "Enter x: ";
	x = getValidatedInput<double>();
	std::cout << "Enter y: ";
	y = getValidatedInput<double>();

	return Point2d(x,y);
}
std::vector<Point2d> EnterData::enterAllPoints(size_t n) {
	std::vector<Point2d> points(n);
	size_t i = 1;
	while (i < n+1) {
		points[i-1] = enterOnePoint(i);
		i++;
	}
	return points;
}
Rect* EnterData::enterRect() {
	Rect* rect = new Rect();
	std::vector<Point2d> points = enterAllPoints(4);
	rect->set(points[0], points[1], points[2], points[3]);

	if (rect->isValid()) {
		return rect;
	}
	else {
		delete rect;
		rect = nullptr;
	}
	return nullptr;
}
Circle* EnterData::enterCircle() {
	Circle* c = new Circle();
	Point2d center = enterOnePoint(1);
	double r = 0.;
	std::cout << "Enter radius ";
	r = getValidatedInput<double>();
	c->set(center, r);
	if (c->isValid()) {
		return c;
	}
	else {
		delete c;
		c = nullptr;
	}
	return nullptr;
}
Arch* EnterData::enterArch() {
	Circle* c = enterCircle();
	std::cout << "Enter angles: " << std::endl;
	double angle1 = 0., angle2 = 0.;
	angle1 = getValidatedInput<double>();
	angle2 = getValidatedInput<double>();
	std::cout << "For control: " << std::endl;
	Point2d controlPoint = enterOnePoint(1);
	Arch* a = nullptr;
	if (c != nullptr) {
		a = new Arch(c->getCenter(), c->getRadius(), angle1, angle2, controlPoint);
		delete c;
		c = nullptr;

		if (a->isValid())
			return a;
		else {
			delete a;
			a = nullptr;
		}
	}
	return nullptr;
}
Polyline* EnterData::enterPolyline() {
	size_t n = 0;
	std::cout << "How many points do you wnat to enter? ";
	n = getValidatedInput<size_t>();

	if (n <= 0) {
		return nullptr;
	}
	Polyline* polyline = new Polyline();
	
	std::vector<Point2d> points = enterAllPoints(n);

	for (Point2d p : points) {
		polyline->addPoint(p);
	}
	if (polyline->isValid())
		return polyline;
	else {
		delete polyline;
		polyline = nullptr;
	}
	return nullptr;
}
Polygon* EnterData::enterPolygon() {
	Polyline* polyline = enterPolyline();
	Polygon* polygon = nullptr;
	if (polyline != nullptr) {
		polygon = new Polygon();
		
		for (const Point2d p : polyline->getPoints()) {
			polygon->addPoint(p);
		}
		delete polyline;
		polyline = nullptr;

		if (polygon->isValid()) {
			return polygon;
		}
		else {
			delete polygon;
			polygon = nullptr;
		}
	}
	return nullptr;
}
UnknownType* EnterData::enterUnknown() {
	size_t n = 0;
	std::cout << "How many numbers do you wnat to enter? ";
	n = getValidatedInput<size_t>();
	UnknownType* ut = nullptr;
	if (n > 0) {
		ut = new UnknownType();
	}
	while (n > 0) {
		std::cout << "Enter a number ";
		double num = 0.;
		num = getValidatedInput<double>();
		ut->addEl(num);
		n--;
	}
	return ut;
}

std::vector<Shape*> EnterData::enterObjects() {
	std::vector<Shape*> res;

	bool exit = false;
	while (1) {
		if (exit) {
			break;
		}
		menuShapes();
		int answer = -1;
		answer = getValidatedInput<int>();

		switch (answer) {
		case Objects::RECT:
			this->collectObjects<Rect*>(res, [&]() {return this->enterRect(); });
			break;
		case Objects::CIRCLE:
			this->collectObjects<Circle*>(res, [&]() {return this->enterCircle(); });
			break;
		case Objects::UNKNOWN: 
			this->collectObjects<UnknownType*>(res, [&]() {return this->enterUnknown(); });
			break;
		case Objects::ARCH:  
			this->collectObjects<Arch*>(res, [&]() {return this->enterArch(); });
			break;
		case Objects::POLYGON: 
			this->collectObjects<Polygon*>(res, [&]() {return this->enterPolygon(); });
			break;
		case Objects::POLYLINE:
			this->collectObjects<Polyline*>(res, [&]() {return this->enterPolyline(); });
			break;
		case 7:
			exit = true;
			break;
		default:
			std::cout << "Wrong answer!" << std::endl;
			break;
		}
		if (!exit) { // skip when "exit"
			char c;
			std::cout << "Press any key and Enter to continue..." << std::endl;
			std::cin >> c;
		}
	}
	return res;
}