#pragma once
#include "prime_rng.hpp"
#include <vector>

class RSA{
private:
	PrimeRNG prime_rng;

	std::vector<BigInt> encrypt(std::vector<BigInt>& message, const BigInt& public_key, const BigInt& modulo);
	std::vector<BigInt> decrypt(std::vector<BigInt>& ciphertexts, const BigInt& private_key, const BigInt& modulo);

public:
	struct Keys{
		BigInt private_key;
		BigInt public_key;
		BigInt n;
	};

	struct Signature{
		std::vector<BigInt> signatures;
		BigInt public_key;
		BigInt modulo;
	};

	Keys createKeys(int length_in_bits, int public_exponent_value = 65537);
	const BigInt encrypt(const BigInt& message, const BigInt& public_key, const BigInt& modulo);
	const BigInt decrypt(const BigInt& ciphertext, const BigInt& private_key, const BigInt& modulo);

	RSA::Signature sign(std::string& message, RSA::Keys keys);
	bool verify(std::string& message, RSA::Signature signature);
};

class BigIntMapping{
public:
	static std::vector<BigInt> toVectorBigInt(std::string message);
	static std::string toString(std::vector<BigInt> numbers);
};

void to_json(json& j, const RSA::Signature& signature);
void from_json(const json& j, RSA::Signature& signature);