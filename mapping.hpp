#include "elliptic_curve.hpp"
#include <vector>

class Mapping {
private:
	std::vector<Point> intToPoint_;

public:
	// create mapping between ASCII symbols and points on elliptic curve
	Mapping(EllipticCurve* curve, Point p);
	Mapping();

	Point charToPoint(char c);
	char pointToChar(Point p);
};