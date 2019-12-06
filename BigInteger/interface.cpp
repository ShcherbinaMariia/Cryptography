#include <iostream>
#include "BigInt.hpp"
#include "modulars.cpp"
#include <string>
#include <sstream>

std::vector<std::string> split(std::string line) {
	std::vector<std::string> result;
	std::istringstream iss(line);
	for(std::string s; iss >> s; )
    	result.push_back(s);
    return result;
}

BigInt processSolveModulars() {
	int n;
	std::cout << "Enter number of equations: ";
	std::cin >> n;
	std::vector<int> remainders = std::vector<int>(n), primes = std::vector<int>(n);
	std::cout << "Enter each equation in format: residual modulo\n";
	for (int i = 0; i < n; i++){
		std::cin >> remainders[i] >> primes[i];
	}
	char c;
	std::getchar();
	return solveModulars(remainders, primes);
}

void help() {
	std::cout << "Please enter command: \n"
	<< "solve \n"
	<< "sqrt x \n"
	<< "add x y [mod p]\n"
	<< "sub x y [mod p]\n"
	<< "mul x y [mod p]\n"
	<< "div x y [mod p]\n"
	<< "pow x y [mod p]\n"
	<< "mod x y \n"
	<< "exit \n";
}

bool processLine(std::string line) {
	std::vector<std::string> tokens = split(line);

	std::string command = tokens[0];

	if (command == "exit"){
		return false;
	}

	if (command == "help"){
		help();
		return true;
	}

	BigInt result, x, y, modulo;
	bool resultByMod = false;
	
	if (command == "solve"){
		result = processSolveModulars();
		std::cout << result.toString() << "\n";
		return true;
	}

	x = BigInt(tokens[1]);

	if (command == "sqrt"){
		result = sqrt(x);
		std::cout << result.toString() << "\n";
		return true;
	}

	y = BigInt(tokens[2]);
	if (tokens.size() >= 4){
		resultByMod = true;
		modulo = BigInt(tokens[tokens.size() - 1]);
	}

	if (command == "pow"){
		if (resultByMod)
			result = power(x, y, modulo);
		else 
			result = power(x, y, BigInt());
	}

	if (command == "add"){
		result = x + y;
		if (resultByMod)
			result = result % modulo;
	}

	if (command == "sub"){
		result = x - y;
		if (resultByMod)
			result = result % modulo;
	}

	if (command == "mul"){
		result = x * y;
		if (resultByMod)
			result = result % modulo;
	}

	if (command == "div"){
		if (resultByMod){
			result = x * findInverse(y, modulo) % modulo;
		} else {
			result = x / y;
		}
	}

	if (command == "mod"){
		result = x % y;
	}

	std::cout << result.toString() << "\n";

	return true;
}