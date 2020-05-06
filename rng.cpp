#include "rng.hpp"

RNG::RNG(){
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	bit_generator = BitGenerator(seed);
	digit_generator = IntGenerator(seed);
}

const BigInt RNG::next(int length_in_bits){
	BigInt mult = BigInt(1);
	BigInt result = BigInt(0);

	for (int i = 0; i < length_in_bits; i++){
		int next_bit = bit_generator();
		if (next_bit) result = result + mult;
		mult = mult * 2;
	}
	return result;
}

const BigInt RNG::next(const BigInt& p){
	BigInt result = BigInt(0);
	while (result < p){
		int next_digit = digit_generator() % BigInt::base;
		result = result * BigInt::base + next_digit;
	}
	return result % p;
}

const BigInt RNG::next(const BigInt& from, const BigInt& to){
	return from + next(to - from);
}