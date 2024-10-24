#pragma once
#include "Polyline.h"

namespace geometry {

	class Polygon : public Polyline // многоугольник
	{
	private:
		bool areCollinear(const geometry::Point2d& p1, const geometry::Point2d& p2, const geometry::Point2d& p3) const;
		bool isPolygon(const std::vector<geometry::Point2d>& points) const;
		int orientation(const geometry::Point2d& p, const geometry::Point2d& q, const geometry::Point2d& r) const;
		bool onSegment(const geometry::Point2d& p, const geometry::Point2d& q, const geometry::Point2d& r) const;
		bool doIntersect(const geometry::Point2d& p1, const geometry::Point2d& q1, const geometry::Point2d& p2, const geometry::Point2d& q2) const;
		bool isSelfIntersecting(const std::vector<geometry::Point2d>& points) const;
	public:
		Polygon() = default;
		~Polygon() = default;

		bool isValid() const override;

		double perimeter() const override;

		int getType() const override {
			return Objects::POLYGON;
		}
		Point2d getCenter() override;
	};

}