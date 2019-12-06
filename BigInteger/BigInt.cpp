#include "BigInt.hpp"
#include "utils.cpp"
#include<iostream>

BigInt::BigInt(std::string s) {

	this->bits = std::vector<int>();
	if (s[0] == '-') {
		sign = false;
		s = s.erase(0, 1);
	}

	int numDigits = log10 (base);
	for (int i = s.size(); i > 0; i -= numDigits)
	{
		if (i < numDigits)
			this->bits.push_back (atoi (s.substr (0, i).c_str()));
		else
			this->bits.push_back (atoi (s.substr (i - numDigits, numDigits).c_str()));
	}
	deleteLeadingZeros(this->bits);
}

BigInt::BigInt(std::vector<int> bits, bool sign) {
	this->bits = bits;
	this->sign = sign;
	if (bits.size() == 1 && bits[0] == 0){
		this->sign = true;
	}
}

BigInt::BigInt() {
	this->bits = std::vector<int>();
	this->bits.push_back(0);
}

BigInt::BigInt(int n){
	this->sign = n>=0;
	n = abs(n);
	std::vector<int> result = std::vector<int>();
	if (n == 0){
		result.push_back(0);
	}
	while(n != 0) {
		result.push_back(n % BigInt::base);
		n /= BigInt::base;
	}
	this->bits = result;
}

bool BigInt::operator >= (BigInt const &other) const {
	if (this->sign != other.sign)
	{
		return this->sign;
	}
	if (this->bits.size() != other.bits.size()){
		return this->sign ?
		 this->bits.size() > other.bits.size() :
		 this->bits.size() < other.bits.size();
	}
	for (int i = this->bits.size() - 1; i >= 0; i--)
	{
		if (this->bits[i] > other.bits[i])
			return this->sign ? true : false;
		if (this->bits[i] < other.bits[i])
			return this->sign ? false : true;
	}
	return true;
}

bool BigInt::operator == (BigInt const &other) const {
	if (this->bits.size() != other.bits.size() || this->sign != other.sign){
		return false;
	}
	for (int i = this->bits.size() - 1; i >= 0; i--)
	{
		if (this->bits[i] != other.bits[i])
			return false;
	}
	return true;
}

bool BigInt::operator != (BigInt const &other) const {
	return !(*this == other);
}

bool BigInt::operator >(BigInt const &other) const {
	return (*this >= other) && !(*this == other);
}

bool BigInt::operator <(BigInt const &other) const {
	return !(*this >= other);
}

bool BigInt::operator <=(BigInt const &other) const {
	return !(*this > other);
}

const BigInt abs(const BigInt& value) {
	return BigInt(value.bits, true);
}

const BigInt operator + (const BigInt& a, const BigInt& b){
	
	if (a.sign != b.sign)
	{
		return a.sign ? a - abs(b) : b - abs(a);
	}

	bool sign = a.sign;
	
	int n = std::max(a.bits.size(), b.bits.size());
	std::vector<int> result = std::vector<int>(n, 0);

	int carry = 0;

	for (int i = 0; i < n; i++)
	{
		result[i] = (i < a.bits.size() ? a.bits[i] : 0) +  
					(i < b.bits.size() ? b.bits[i] : 0) + 
					carry;
		carry = result[i] / BigInt::base;
		result[i] %= BigInt::base;
	} 
	
	if (carry != 0)
	{
		result.push_back(carry);
	}
	return BigInt(result, sign);
}

const BigInt operator - (const BigInt& value) {
	return BigInt(value.bits, !value.sign);
}

const BigInt operator - (const BigInt& a, const BigInt& b) {
	if (!b.sign) {
		return a.sign ? a + abs(b) : abs(b) - abs(a);
	}
	if (b.sign && !a.sign){
		return - (abs(a) + b);
	}
	
	bool sign = true;
	std::vector<int> result = std::vector<int>();
	if (a < b) sign = false;
	int carry = 0;
	for (int i = 0; i < std::max(b.bits.size(), a.bits.size()); i++){
		int delta = 
			(i < a.bits.size() ? a.bits[i] : 0) -
			(i < b.bits.size() ? b.bits[i] : 0);

		if (!sign) delta = -delta;

		delta += carry;
		carry = 0;

		if (delta < 0){
			delta += BigInt::base;
			carry = -1;
		}

		result.push_back(delta);
	}

	deleteLeadingZeros(result);

	return BigInt(result, sign);
}

const BigInt operator * (const BigInt& a, const BigInt& b){
	bool sign = a.sign == b.sign ? true : false;
	std::vector<int> result = std::vector<int>();

	std::vector<std::complex<double>> fa(a.bits.begin(), a.bits.end()), fb(b.bits.begin(), b.bits.end());
	int n = 1;
	while (n < std::max(fa.size(), fb.size())) n *= 2;
	n *= 2;
	fa.resize(n);
	fb.resize(n);

	fft(fa);
	fft(fb);

	for (int i = 0; i < n; i++)
		fa[i] = fa[i] * fb[i];
	fft(fa, true /* inverse */);

	result.resize(n);
	int carry = 0;
	for (int i = 0; i < n; i++) {
		result[i] = int (fa[i].real() + 0.5) + carry;
		carry = result[i] / BigInt::base;
		result[i] %= BigInt::base;
	}
	deleteLeadingZeros(result);
	return BigInt(result, sign);
}

const BigInt operator * (const BigInt& a, int b){
	std::vector<int> result = std::vector<int>(a.bits.size());

	int carry = 0;
	for (int i = 0; i < a.bits.size(); i++){
		result[i] = a.bits[i] * b + carry;
		carry = result[i] / BigInt::base;
		result[i] %= BigInt::base;
	}
	if (carry != 0) result.push_back(carry);
	deleteLeadingZeros(result);
	return BigInt(result, a.sign);
}

const BigInt power(const BigInt& a, const BigInt& n, const BigInt& modulo){
	BigInt result = 1, mult = BigInt(a.bits, a.sign), exp = BigInt(n.bits, true);
	bool byModulo = modulo != BigInt(0);
	while (exp != 0){
		if (exp % 2 == 1){
			result = result * mult;
			if (byModulo) result = result % modulo;
		}

		mult = mult * mult;
		if (byModulo) mult = mult % modulo;

		exp = exp / BigInt(2);
	}
	return byModulo ? result % modulo : result;
}

int digitBinarySearch(const BigInt& a, const BigInt& b){
	int l = 0, m, r = BigInt::base - 1;
	while (r - l > 1){
		m = (l + r) / 2;
		if ( a < b * m)
			r = m;
		else 
			l = m;
	}
	if (a < b * r)
		return l;
	return r;
}

const BigInt operator / (const BigInt& a, const BigInt& b){
	bool sign = a.sign == b.sign ? true : false;
	std::vector<int> result = std::vector<int>();

	if (abs(a) < abs(b)) return BigInt(0);

	BigInt curr(std::vector<int>(b.bits.size() - 1), true);

	int index = a.bits.size() - b.bits.size() + 1;
	for (int i = index; i < a.bits.size(); i++) {
		curr.bits[i - index] = a.bits[i];
	}

	while (index != 0){
		index--;
		curr.bits.insert(curr.bits.begin(), a.bits[index]);
		deleteLeadingZeros(curr.bits);
		int digit = digitBinarySearch(curr, abs(b));
		result.push_back(digit);
		curr = curr - abs(b) * digit;
	}

	std::reverse(result.begin(), result.end());
	deleteLeadingZeros(result);
	return BigInt(result, sign);
}

const BigInt operator % (const BigInt& a, const BigInt& b){
	BigInt result = a - ((a / b) * b);
	if (result < BigInt(0)) {
		result = result + b;
	}
	return BigInt(result.bits, true);
}

const BigInt sqrt(const BigInt& a){
	if (!a.sign) {
		std::cout << "Complex numbers are not supported.";
		return a;
	}
	BigInt l(0), r(a.bits);
	while (abs(r - l) > BigInt(1)){
		BigInt m = (l + r) / BigInt(2);
		if (a >= m * m){
			l = m;
		}
		else {
			r = m;
		}
	}
	if (a >= r*r) return r;
	return l;
}

const BigInt gcdEx(BigInt a, BigInt b, BigInt& x, BigInt& y){
	if (a == 0){
		x = BigInt(0);
		y = BigInt(1);
		return b;
	}
	BigInt x1, y1, res;
	res = gcdEx(b%a, a, x1, y1);
	y = x1;
	x = y1 - (b / a) * x1;
	return res;
}

const BigInt gcd(const BigInt& a, const BigInt& b){
	if (a == 0){
		return b;
	}
	return gcd(b%a, a);
}

const BigInt findInverse(const BigInt& y, const BigInt& modulo){
	BigInt x1 = BigInt(), y1 = BigInt(), inverse_y;
	gcdEx(y, modulo, x1, y1);
	inverse_y = (x1 % modulo + modulo) % modulo;
	return inverse_y;
}

const BigInt randBigInt(const BigInt& p){
	return rand() * (p/(RAND_MAX) + 1) % p;
}

std::string BigInt::toString() const {

	int n = this->bits.size();
	int numDigits = log10 (base);
	std::string s = std::to_string(this->bits[n - 1]);
	std::string curr_s;

	for (int i = n - 2; i >= 0; i--){
		curr_s = std::to_string(this->bits[i]);
		while (curr_s.size() != numDigits) {
			curr_s = "0" + curr_s;
		}
		s += curr_s;
	}
	if (!sign) s = '-' + s;
	return s;
}