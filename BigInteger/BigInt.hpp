#pragma once
#include <vector> 
#include <string>
#include <cmath>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class BigInt {
private:
	BigInt(std::vector<int> bits, bool sign=true);

public:
	static const int base = 10000;
	bool sign = true; // true for >= 0
	std::vector<int> bits;

	BigInt();
	BigInt(std::string s);
	BigInt(int n);

	bool operator > (BigInt const &other) const;
	bool operator >= (BigInt const &other) const;
	bool operator < (BigInt const &other) const;
	bool operator <= (BigInt const &other) const;
	bool operator == (BigInt const &other) const;
	bool operator != (BigInt const &other) const;

	std::string toString() const;
	int firstDigit() const;

	friend const BigInt abs(const BigInt& value);
	friend const BigInt sqrt(const BigInt& a);
	friend const BigInt gcdEx(BigInt a, BigInt b, BigInt& x, BigInt& y);
	friend const BigInt gcd(const BigInt& a, const BigInt& b);
	friend const BigInt findInverse(const BigInt& y, const BigInt& modulo);
	friend const BigInt operator - (const BigInt& value);
	friend const BigInt operator + (const BigInt& a, const BigInt& b);
	friend const BigInt operator - (const BigInt& a, const BigInt& b);
	friend const BigInt operator * (const BigInt& a, const BigInt& b);
	friend const BigInt operator / (const BigInt& a, const BigInt& b);
	friend const BigInt operator % (const BigInt& a, const BigInt& b);

	friend int digitBinarySearch(const BigInt& a, const BigInt& b);

	friend const BigInt operator * (const BigInt& a, int b);
	friend const BigInt power(const BigInt& a, const BigInt& n, const BigInt& modulo);
};

void to_json(json& j, const BigInt& big_int);
void from_json(const json& j, BigInt& big_int);