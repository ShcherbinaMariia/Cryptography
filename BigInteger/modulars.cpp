#include "BigInt.hpp"

int powModP(int a, int n, int p){
	if (n == 1) return a % p;
	if (n%2 == 1)
		return (a * powModP(a, n-1, p)) % p;
	int tmp = powModP(a, n/2, p) % p;
	return (tmp * tmp) % p;

}

const BigInt solveModulars(std::vector<int> remainders, std::vector<int> primes) {
	int n = remainders.size();
	std::vector<std::vector<int>> inverse = std::vector<std::vector<int>>();
	inverse.resize(n);
	BigInt result(0), multiplier(1);
	for (int i = 0; i < n; i++){
		inverse[i].resize(n);
		for(int j = i + 1; j < n; j++){
			inverse[i][j] = powModP(primes[i], primes[j] - 2, primes[j]);
		}
	}

	std::vector<int> x = std::vector<int>(n, 0);
	for (int i = 0; i < n; i++){
		x[i] = remainders[i];
		for (int j = 0; j < i; j++){
			x[i] = (x[i] - x[j]) * inverse[j][i];
			x[i] = (x[i] + primes[i]) % primes[i];
		}
		result = result + multiplier * x[i];
		multiplier = multiplier * primes[i];
	}
	return result;
}