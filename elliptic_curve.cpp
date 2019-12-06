#include "elliptic_curve.hpp"
#include<iostream>

EllipticCurve::EllipticCurve(const BigInt& a, const BigInt& b, const BigInt& n){
	this->a = a;
	this->b = b;
	this->n = n;
}

bool EllipticCurve::isOnCurve(const BigInt& x, const BigInt& y){
	return (y*y - x*x*x - this->a*x - this->b) % this->n == 0;
}

Point::Point(const BigInt& x, const BigInt& y, EllipticCurve* curve){
	this->x = x;
	this->y = y;
	this->curve = curve;
	this->isZero = false;
}

Point::Point(){
	this->x = 0;
	this->y = 1;
	this->isZero = true;
}

const Point operator - (const Point &p){
	if (p.isZero) return Point();
	if (p.y == 0) return p;
	return Point(p.x, (-p.y) % p.curve->n, p.curve);
}

const bool operator == (const Point &p1, const Point &p2){
	return p1.x == p2.x && p1.y == p2.y;
}

const Point operator + (const Point &p1, const Point &p2){

	BigInt slope;

	if (p1.isZero) return p2;
	if (p2.isZero) return p1;

	if (p1 == p2){
		if (p1.y == 0){
			return Point();
		}
		slope = (3 * p1.x * p1.x + p1.curve->a) * findInverse(2 * p1.y, p1.curve->n);
	} 
	else {
		if (p1.x == p2.x){
			return Point();
		}
		if (p1.x > p2.x)
			slope = (p1.y - p2.y) * findInverse(p1.x - p2.x, p1.curve->n);
		else 
			slope = (p2.y - p1.y) * findInverse(p2.x - p1.x, p1.curve->n);
	}
	slope = slope % p1.curve->n;
	BigInt x3 = (slope*slope - (p1.x + p2.x));
	BigInt y3 = -p1.y + slope * (p1.x - x3);

	return Point(x3 % p1.curve->n, y3 % p1.curve->n, p1.curve);
}

const Point operator * (const BigInt& n, const Point& p){
	Point result = Point(), add = p;
	BigInt exp = n;
	while (exp != 0){
		if (exp % 2 == 1){
			result = result + add;
		}

		add = add + add;
		exp = exp / 2;
	}
	return result;
}