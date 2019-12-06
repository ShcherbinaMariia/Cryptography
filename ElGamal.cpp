#include "ElGamal.hpp"
#include "cryptography.hpp"

ElGamal::ElGamal(){
	EllipticCurve* curve = new EllipticCurve(/* a */ BigInt(0), /* b */ BigInt(7), 
	/* p */ BigInt("115792089237316195423570985008687907853269984665640564039457584007908834671663"));
	this->curve = curve;

	this->P = Point(/* x */ BigInt("55066263022277343669578718895168534326250603453777594175500187360389116729240"),
					/* y */ BigInt("32670510020758816978083085130507043184471273380659243275938904335757337482424"),
					curve);
	this->N = BigInt("115792089237316195423570985008687907852837564279074904382605163141518161494337");
	assert(this->curve->isOnCurve(this->P.x, this->P.y));

	this->m = Mapping(this->curve, this->P);
}

ElGamal::ElGamal(EllipticCurve* curve, Point P, BigInt N){
	this->curve = curve;
	this->P = P;
	this->N = N;
	this->m = Mapping(this->curve, this->P);
}

const std::pair<BigInt, Point> ElGamal::createKeys() {
	BigInt k = randBigInt(this->N - 1) + 1;
	return std::make_pair(k, k*this->P);
}

const std::pair<Point, Point> ElGamal::encrypt(const Point M, const Point Y) {
	BigInt r = randBigInt(this->N - 1) + 1;
	Point d, g, h;
	d = r * Y;
	g = r * this->P;
	h = M + d;
	return std::make_pair(g, h);
}

const std::pair<Point, Point> ElGamal::encrypt_char(char message, const Point openKey){
	return this->encrypt(this->m.charToPoint(message), openKey);
}

const Point ElGamal::decrypt(const BigInt& k, std::pair<Point, Point> c) {
	Point g, d, s, s1, M, h;
	g = c.first;
	h = c.second;
	s = k * g;
	s1 = -s;
	M = s1 + h;
	return M;
}

const char ElGamal::decrypt_char(const BigInt& k, std::pair<Point, Point> c){
	return this->m.pointToChar(this->decrypt(k, c));
}