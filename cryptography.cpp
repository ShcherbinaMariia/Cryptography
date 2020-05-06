#include "cryptography.hpp"
#include<iostream>

// Number is prime with probability 1-2^(-S) if isPrime
const int S = 50;
RNG Cryptography::rng = RNG();

// Shanks method(baby-step-giant-step algorithm)
const BigInt Cryptography::discrete_log(const BigInt& g, const BigInt& b, const BigInt& n){
	BigInt m = sqrt(n) + 1, p = 1;
	BigInt gm = findInverse(power(g, m, n), n);
	std::map<BigInt, BigInt> g_j = std::map<BigInt, BigInt>();
	
	g_j[1] = 0;
	for (BigInt j = 1; j < m; j = j + BigInt(1)){
		p = p * g % n;
		g_j[p] = j;

		if (p == b)
			return j;
	}

	p = 1;
	for (BigInt i = 1; i < m; i = i + 1){
		p = p * gm % n;
		auto pos = g_j.find(b * p % n);
		if (pos!=g_j.end()){
			return pos->second + i * m;
		}
	}

	return -1;
}

bool witness(const BigInt& a, const BigInt& n){
	BigInt t = 0, u = n - 1;
	while (u % 2 == 0){
		t = t + 1;
		u = u / 2;
	}

	BigInt x_prev = power(a, u, n), x;

	for (BigInt i = 0; i < t; i = i + 1){
		x = x_prev * x_prev % n;
		if (x == 1 && x_prev != 1 && x_prev != n - 1) 
			return true;
		x_prev = x;
	}
	return x != 1;
}

// Miller-Rabin test
bool Cryptography::isPrime(const BigInt& n){
	if (n == 2) return true;
	if (n % 2 == 0) return false;
	BigInt a;
	for (int i = 0; i < S; i++){
		a = rng.next(n - 1) + 1;
		if (::witness(a, n))
			return false;
	}
	return true;
}

// Pollard`s rho-algorithm
const BigInt findFactor(const BigInt& n){
	int max_iterations = 10000, iteration = 0;
	BigInt x0 = 2;
	BigInt x_i = x0, x_2i = x0, d = BigInt(1);
	auto f = [n](const BigInt& x){return (x*x-1) % n;};
	while (iteration < max_iterations && (d == BigInt(1) || d == n)){
		d = gcd(n, abs(x_i - x_2i));
		x_i = f(x_i);
		x_2i = f(f(x_2i));
		iteration++;
	}
	return d;
}

const BigInt trivial_factors(const BigInt& n, std::map<BigInt, int>& fs){
	BigInt m = n;
	for (BigInt i = 2; i < 1000 && i*i < n; i = i + 1){
		if (m % i == 0) fs[i]=0;
		while (m % i == 0){
			fs[i]++;
			m = m / i;
		}
	}
	return m;
}

void factor(const BigInt& n, std::map<BigInt, int>& fs){
	if (n == 1) return;
	if (Cryptography::isPrime(n)){
		auto pos = fs.find(n);
		if (pos != fs.end())
			pos->second++;
		else 
			fs[n] = 1;
		return;
	}

	BigInt d = 1;

	while (d == 1 || d == n)
	  d = findFactor(n);
	
	factor(d, fs);
	factor(n/d, fs);
}

const std::map<BigInt, int> Cryptography::factor(const BigInt& n){
	auto factors = std::map<BigInt, int>();
	BigInt m = trivial_factors(n, factors);
	::factor(m, factors);
	return factors;
}

const BigInt Cryptography::phi(const BigInt& n){
	auto factors = factor(n);
	BigInt result = BigInt(1);
	for (auto factor_pair: factors){
		BigInt p = factor_pair.first, alpha = factor_pair.second, tmp;
		tmp = power(p, alpha - 1, n);
		result = result * (p*tmp - tmp);
	}
	return result;
}

const int Cryptography::mu(const BigInt& n){
	auto factors = factor(n);
	int result = factors.size() % 2 ? -1 : 1;
	for (auto factor_pair: factors){
		if (factor_pair.second > 1){
			return 0;
		}
	}
	return result;
}

const int Cryptography::legendre_symbol(const BigInt& a, const BigInt& p){
	return power(a, (p - 1)/2, p) == BigInt(1) ? 1 : -1;
}

const int Cryptography::jacobi_symbol(const BigInt& a, const BigInt& m){
	auto factors = factor(m);
	int result = 1;
	for (auto factor_pair: factors){
		BigInt p = factor_pair.first, alpha = factor_pair.second;
		if (alpha % 2 == 1){
			result *= legendre_symbol(a, p);
		}
	}
	return result;
}

// Cipolla`s algorithm
const BigInt Cryptography::discrete_sqrt(const BigInt& a, const BigInt& p){
	BigInt w = 1, b;
	while(legendre_symbol(w, p) == 1){
		b = rng.next(p);
		w = b*b - a;
		if (w == 0) return b;
	}

	BigInt deg = (p+1)/2, wk = 1, bk = power(b, deg, p), ck = 1;
	BigInt result = (wk * ck * bk) % p;
	for (BigInt k = 2; k <= deg; k = k + 2){
		wk = (wk * w) % p;
		bk = power(b, deg - k, p);
		ck = (ck * (deg - k + 1) * (deg - k + 2)) * findInverse((k - 1) * k, p) % p;
		result = (result + ck * bk * wk) % p;
	}
	return result;
}

