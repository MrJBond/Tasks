#pragma once
#include "Circle.h"
#include <cmath>

namespace geometry {

	class Arch : public Circle
	{
	private:
		double angle1 = std::nan("1"); // to the left from (0,1) (on the unit circle)
		double angle2 = std::nan("1"); // to the right from (0,1) (on the unit circle)
		double nomalizedAngle1 = std::nan("1");
		double nomalizedAngle2 = std::nan("1");
		Point2d point; // one point on the arch - to identify the arch because two arches are possible 

		// Helper function to calculate the angle of a point relative to the center
		double calculateAngle(const Point2d& p);
		double normalizeAngle(double angle);

	public:
		Arch() = default;
		Arch(Point2d center, double r, double angle1, double angle2, Point2d point) :
			Circle(center, r) {
			this->setAngles(angle1, angle2);
			this->setPoint(point);
		}
		~Arch() = default;
		double perimeter() override;

		std::vector<Point2d> boundingBox() override;

		void setAngles(double angle1, double angle2);

		void setPoint(Point2d point);
		Point2d getControlPoint() const {
			return this->point;
		}


		std::pair<double, double> getAngles() const {
			return std::make_pair(angle1, angle2);
		}

		std::pair<double, double> getNormalizedAngles() const {
			return std::make_pair(nomalizedAngle1, nomalizedAngle2);
		}
		double getPointAngle() {
			return this->normalizeAngle(this->calculateAngle(this->point));
		}
	};

}