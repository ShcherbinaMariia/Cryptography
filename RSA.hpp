#include "prime_rng.hpp"

class RSA{
	struct Keys{
		BigInt private_key;
		BigInt public_key;
		BigInt n;
	};

private:
	PrimeRNG prime_rng;
public:
	Keys createKeys(int length_in_bits, int public_exponent_value = 65537);
	const BigInt encrypt(const BigInt& message, const BigInt& public_key, const BigInt& modulo);
	const BigInt decrypt(const BigInt& ciphertext, const BigInt& private_key, const BigInt& modulo);

	const BigInt createSignature(const BigInt& message, const BigInt& private_key, const BigInt& modulo);
	bool verifySignature(const BigInt& message, const BigInt& signature, const BigInt& public_key, const BigInt& modulo);
};