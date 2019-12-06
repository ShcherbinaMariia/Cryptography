#pragma once
#include "BigInt.hpp"

struct EllipticCurve{

	BigInt a;
	BigInt b;
	BigInt n;

	EllipticCurve(const BigInt& a, const BigInt& b, const BigInt& n);

	bool isOnCurve(const BigInt& x, const BigInt& y);
};

struct Point{
	BigInt x;
	BigInt y;
	bool isZero;
	EllipticCurve* curve;

	Point(); // construct "infinity" (0, 1, 0) point
	Point(const BigInt& x, const BigInt& y, EllipticCurve* curve);

	friend const bool operator == (const Point &p1, const Point &p2);
	friend const Point operator + (const Point &p1, const Point &p2);
	friend const Point operator * (const BigInt &n, const Point &p);
	friend const Point operator - (const Point &p);
};