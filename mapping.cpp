#include "mapping.hpp"
#include<iostream>

Mapping::Mapping(EllipticCurve* curve, Point p){
	Point g = Point();
	this->intToPoint_ = std::vector<Point>(256);
	for (int i = 0; i < 256; i++){
		g = g + p;
		this->intToPoint_[i] = g;
	}
	std::cout << "Mapping finished" << std::endl;
}

Mapping::Mapping(){
	this->intToPoint_ = std::vector<Point>();
}

Point Mapping::charToPoint(char c){
	return this->intToPoint_[int(c)];
}

char Mapping::pointToChar(Point p){
	for (int i = 0; i < 256; i++){
		if (this->intToPoint_[i] == p){
			return char(i);
		}
	}
	return '#';
}