#include "RSA.hpp"

RSA::Keys RSA::createKeys(int length_in_bits, int public_exponent_value){
	BigInt p = this->prime_rng.nextPrime(length_in_bits);
	BigInt q = this->prime_rng.nextPrime(length_in_bits);

	BigInt n = p * q;
	BigInt phi_n = (p - 1) * (q - 1);

	BigInt public_exponent;

	if (public_exponent_value == 0){
		public_exponent = this->prime_rng.nextPrime(length_in_bits / 2);
	} else {
		public_exponent =  BigInt(public_exponent_value);
	}

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

std::vector<BigInt> RSA::encrypt_(std::vector<BigInt> message, const BigInt& public_key, const BigInt& modulo){
	std::vector<BigInt> result;
	for (auto message_chunk : message){
		result.push_back(encrypt(message_chunk, public_key, modulo));
	}
	return result;
}

std::vector<BigInt> RSA::decrypt_(std::vector<BigInt> ciphertexts, const BigInt& private_key, const BigInt& modulo){
	std::vector<BigInt> result;
	for (auto ciphertext_chunk : ciphertexts){
		result.push_back(decrypt(ciphertext_chunk, private_key, modulo));
	}
	return result;
}

std::vector<BigInt> RSA::encrypt(std::string message, const BigInt& public_key, const BigInt& modulo){
	return encrypt_(BigIntMapping::toVectorBigInt(message), public_key, modulo);
}

std::string RSA::decrypt(std::vector<BigInt> ciphertext, const BigInt& private_key, const BigInt& modulo){
	return BigIntMapping::toString(decrypt_(ciphertext, private_key, modulo));
}

RSA::Signature RSA::sign(std::string& message, Keys keys){ 
	Signature signature;
	signature.public_key = keys.public_key;
	signature.modulo = keys.n;
	std::vector<BigInt> mapped_message = BigIntMapping::toVectorBigInt(message);
	signature.signatures = encrypt_(mapped_message, keys.private_key, signature.modulo);
	return signature;
}

bool RSA::verify(std::string& message, Signature signature){
	return BigIntMapping::toString(decrypt_(signature.signatures, signature.public_key, signature.modulo)) == message;
}

std::vector<BigInt> BigIntMapping::toVectorBigInt(std::string message){
	std::vector<BigInt> result;
	for (char symbol: message){
		result.push_back(BigInt(int(symbol)));
	}
	return result;
}

std::string BigIntMapping::toString(std::vector<BigInt> numbers){
	std::string result = "";
	for (BigInt number: numbers){
		result.push_back(char(number.firstDigit()));
	}
	return result;
}

void to_json(json& j, const RSA::Signature& signature){
	j = json{{"signatures", signature.signatures},
			 {"public_key", signature.public_key},
			 {"modulo", signature.modulo}};
}

void from_json(const json& j, RSA::Signature& signature){
	j.at("signatures").get_to(signature.signatures);
	j.at("public_key").get_to(signature.public_key);
	j.at("modulo").get_to(signature.modulo);
}
