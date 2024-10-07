#pragma once
#include "Polyline.h"

namespace geometry {

	class Polygon : public Polyline // �������������
	{

	public:
		Polygon() = default;
		~Polygon() = default;

		bool isValid() override;

		double perimeter() override;
	};

}