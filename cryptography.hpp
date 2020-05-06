#pragma once
#include "BigInt.hpp"
#include "rng.hpp"
#include <map>

class Cryptography {
private:
	static RNG rng;
public:

	// solve equation g^x = b (mod n)
	static const BigInt discrete_log(const BigInt& g, const BigInt& b, const BigInt& n);

	// return factors as a map (p_i, alpha_i)
	static const std::map<BigInt, int> factor(const BigInt& n);
	
	static bool isPrime(const BigInt& n);

	// calculates Euler's function
	static const BigInt phi(const BigInt& n);

	// calculates Mobius function
	static const int mu(const BigInt& n);

	static const int legendre_symbol(const BigInt& a, const BigInt& p);
	static const int jacobi_symbol(const BigInt& a, const BigInt& p);

	// solve equation x^2 = a (mod n)
	static const BigInt discrete_sqrt(const BigInt& a, const BigInt& p);
};