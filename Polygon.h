#pragma once
#include "Polyline.h"

namespace geometry {

	class Polygon : public Polyline // многоугольник
	{

	public:
		Polygon() = default;
		~Polygon() = default;

		bool isValid() override;

		double perimeter() override;
	};

}