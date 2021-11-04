#pragma once
#include "Point.h"

class Line
{
public:
	double k, b; // y = k*x + b

	Line(double k, double b) {
		this->k = k;
		this->b = b;
	}

	Line(Point A, Point B) {
		if (A.x == B.x) B.x += 1e-3;
		this->k = (B.y - A.y) / (B.x - A.x);
		this->b = A.y - A.x * this->k;
	}
};

