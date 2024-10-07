#pragma once
#include <iostream>

namespace geometry { // To let everyone know about operators

    class Point2d
    {
    public:
        Point2d(double xx, double yy);
        Point2d() = default;
        ~Point2d() = default;
        double x() const { return m_dX; }
        double y() const { return m_dY; }

    private:
        double m_dX;
        double m_dY;
    };

	// inline - to include them only once when linking 
	inline bool operator==(geometry::Point2d p1, geometry::Point2d p2) {
		return (p1.x() == p2.x() && p1.y() == p2.y());
	}
	inline bool operator!=(geometry::Point2d p1, geometry::Point2d p2) {
		return !(p1 == p2);
	}
	inline std::ostream& operator<<(std::ostream& os, geometry::Point2d p) {
		os << p.x() << " " << p.y() << '\n';
		return os;
	}
	template <class T, class U>
	inline std::ostream& operator<<(std::ostream& os, std::pair<T, U> p) {
		os << p.first << " " << p.second << '\n';
		return os;
	}

	enum Objects {
		RECT = 1,
		CIRCLE = 2,
		UNKNOWN = 3,
		ARCH = 4,
		POLYGON = 5,
		POLYLINE = 6
	};
}