#pragma once
#include "Shape.h"
#include <vector>

namespace geometry {

	class UnknownType : public Shape
	{
	private:
		std::vector<double> data;
	public:
		UnknownType() = default;
		~UnknownType() = default;
		std::vector<double> getData() const {
			return data;
		}
		void addEl(double n) {
			data.push_back(n);
		}
		void removeEl() {
			data.pop_back();
		}

		// Override those to prevent the class from being abstract
		std::vector<Point2d> getPoints() const override {
			std::vector<Point2d> v;
			std::cout << "The unknown type has no points!" << std::endl;
			return v;
		}

		bool isValid() const override {
			if (!data.empty()) {
				return true;
			}
			return false;
		}
		std::vector<Point2d> boundingBox() const override {
			std::vector<Point2d> v;
			std::cout << "The unknown type has no bounding box!" << std::endl;
			return v;

		}
		double perimeter() const override {
			std::cout << "The unknown type has no perimeter!" << std::endl;
			return 0.0;
		}
		int getType() const override {
			return Objects::UNKNOWN;
		}
		Point2d getCenter() override {
			std::cout << "The unknown type does not have the center" << std::endl;
			return Point2d();
		};
	};

}