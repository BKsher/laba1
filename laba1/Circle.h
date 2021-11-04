#pragma once
#include "Point.h"

class Circle
{
public:
	Point center;
	double radius;

	Circle(Point center, double radius) {
		this->center = center;
		this->radius = radius;
	}

	Circle(double centerX, double centerY, double radius) {
		this->center = Point(centerX, centerY);
		this->radius = radius;
	}
};

