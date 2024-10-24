#pragma once
#include "Circle.h"
#include <cmath>

namespace geometry {

	class Arch : public Circle
	{
	private:
		double m_angle1 = std::nan("1"); // to the left from (0,1) (on the unit circle)
		double m_angle2 = std::nan("1"); // to the right from (0,1) (on the unit circle)
		double m_nomalizedAngle1 = std::nan("1");
		double m_nomalizedAngle2 = std::nan("1");
		Point2d m_point; // one point on the arch - to identify the arch because two arches are possible 

		// Helper function to calculate the angle of a point relative to the center
		double calculateAngle(const Point2d& p) const;
		double normalizeAngle(double angle) const;

	public:
		Arch() = default;
		Arch(Point2d center, double r, double angle1, double angle2, Point2d point) :
			Circle(center, r) {
			setAngles(angle1, angle2);
			setPoint(point);
		}
		~Arch() = default;
		double perimeter() const override;

		std::vector<Point2d> boundingBox() const override;

		void setAngles(double angle1, double angle2);

		void setPoint(Point2d point);
		Point2d getControlPoint() const {
			return m_point;
		}


		std::pair<double, double> getAngles() const {
			return std::make_pair(m_angle1, m_angle2);
		}

		std::pair<double, double> getNormalizedAngles() const {
			return std::make_pair(m_nomalizedAngle1, m_nomalizedAngle2);
		}
		double getPointAngle() const{
			return normalizeAngle(calculateAngle(m_point));
		}
		int getType() const override {
			return Objects::ARCH;
		}
	};

}