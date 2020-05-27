#include "RSA.hpp"

RSA::Keys RSA::createKeys(int length_in_bits, int public_exponent_value){
	BigInt p = this->prime_rng.nextPrime(length_in_bits);
	BigInt q = this->prime_rng.nextPrime(length_in_bits);

	BigInt n = p * q;
	BigInt phi_n = (p - 1) * (q - 1);

	BigInt public_exponent =  BigInt(public_exponent_value);
	BigInt private_exponent = findInverse(public_exponent, phi_n);

	RSA::Keys result;
	result.private_key = private_exponent;
	result.public_key = public_exponent;
	result.n = n;
	return result;
}

const BigInt RSA::encrypt(const BigInt& message, const BigInt& public_key, const BigInt& modulo){
	return power(message, public_key, modulo);
}

const BigInt RSA::decrypt(const BigInt& ciphertext, const BigInt& private_key, const BigInt& modulo){
	return power(ciphertext, private_key, modulo);
}

const BigInt RSA::createSignature(const BigInt& message, const BigInt& private_key, const BigInt& modulo){
	return encrypt(message, private_key, modulo);
}

bool RSA::verifySignature(const BigInt& message, const BigInt& signature, const BigInt& public_key, const BigInt& modulo){
	return decrypt(signature, public_key, modulo) == message;
}
