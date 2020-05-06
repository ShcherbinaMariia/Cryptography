#include "elliptic_curve.hpp"
#include "mapping.hpp"
#include "rng.hpp"

class ElGamal{
	EllipticCurve* curve;
	Point P; // generator of some cyclic subgroup G
	BigInt N; // order of a group
	Mapping m;
	RNG rng;

public:
	// using secp256k1
	ElGamal();

	// customized parameters
	ElGamal(EllipticCurve* curve, Point P, BigInt N);

	const std::pair<BigInt, Point> createKeys();

	const std::pair<Point, Point> encrypt(const Point message, const Point openKey);
	const Point decrypt(const BigInt& k, std::pair<Point, Point> c);

	const std::pair<Point, Point> encrypt_char(char message, const Point openKey);
	const char decrypt_char(const BigInt& k, std::pair<Point, Point> c);
};