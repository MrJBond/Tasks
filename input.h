#pragma once
#include <iostream>
#include "Shape.h"
#include "Rect.h"
#include "Circle.h"
#include "Polyline.h"
#include "Polygon.h"
#include "Arch.h"
#include "UnknownType.h"
#include <functional>

using namespace geometry;

class EnterData {
private:
	void menuShapes();
	Point2d enterOnePoint(size_t n);
	std::vector<Point2d> enterAllPoints(size_t n);
	Rect* enterRect();
	Circle* enterCircle();
	Arch* enterArch();
	Polyline* enterPolyline();
	Polygon* enterPolygon();
	UnknownType* enterUnknown();

	template<class T>
	void collectObjects(std::vector<Shape*>& res, std::function<T()> f) {
		if (T obj = f(); obj != nullptr)
			res.push_back(obj);
		else
			std::cout << "\nThe shape is invalid!" << std::endl;
	}
public:
	EnterData() = default;
	~EnterData() = default;

	std::vector<Shape*> enterObjects();

	// to prevent,
	// for example if user enters a string when we are expecting an int
	template<typename T>
	T getValidatedInput() {
		T input;
		while (true) {
			std::cin >> input;
			if (std::cin.fail()) {
				std::cin.clear(); // Clear the error flag
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
				std::cout << "Invalid input. Please enter a valid value: ";
			}
			else {
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
				return input; // Return valid input
			}
		}
	}

};
