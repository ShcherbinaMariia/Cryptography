#pragma once
#include <string>
#include <vector>

using byte = unsigned char;

class LongHex{
	const static char kAlphabet[16];
	std::vector<byte> bytes;
	
public:
	LongHex();
	LongHex(unsigned int* s, int n);
	const LongHex& concatenate(LongHex& other);
	std::vector<byte> getBytes();

	std::string toString();
};