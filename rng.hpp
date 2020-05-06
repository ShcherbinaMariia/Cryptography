#pragma once
#include "BigInt.hpp"
#include <random>
#include <chrono>

typedef std::independent_bits_engine<std::mt19937, 1, bool> BitGenerator; 
typedef std::independent_bits_engine<std::mt19937, 32, uint> IntGenerator;

class RNG{
private:
	BitGenerator bit_generator;
	IntGenerator digit_generator;
public:
	RNG();
	const BigInt next(int length_in_bits);
	const BigInt next(const BigInt& p);
	const BigInt next(const BigInt& from, const BigInt& to);
};