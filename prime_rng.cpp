#include "prime_rng.hpp"

const BigInt PrimeRNG::nextPrime(int length_in_bits){
	BigInt result;
	while (!Cryptography::isPrime(result))
		result = rng.next(length_in_bits);
	return result;
}

const BigInt PrimeRNG::nextPrime(const BigInt& from, const BigInt& to){
	BigInt result;
	while (!Cryptography::isPrime(result))
		result = rng.next(from, to);
	return result;
}