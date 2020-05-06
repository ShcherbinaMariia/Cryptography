#include "cryptography.hpp"

class PrimeRNG{
private:
	RNG rng;
public:
	const BigInt nextPrime(int length_in_bits);
	const BigInt nextPrime(const BigInt& from, const BigInt& to);
};