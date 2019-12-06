#include <iostream>
#include <string>
#include "cryptography.hpp"
#include "ElGamal.hpp"

void help(){
	std::cout << "Please enter command: \n"
	<< "ElGamal\n"
	<< "prime x[check if x is a prime number]\n"
	<< "factor x[factorize x]\n"
	<< "log g b n[solve g^x = b(mod n) for x]\n"
	<< "phi n[euler function]\n"
	<< "mu n[mobius function]\n"
	<< "jacobi a n[calculates jacobi symbol (a n)]\n"
	<< "legendre a p[calculates legendre symbol (a p)]\n"
	<< "sqrt a p[solves x^2=a(mod p) for x]\n"
	<< "exit \n";
}

void factor(){
	std::string n;
	std::cin >> n;
	auto factors = Cryptography::factor(BigInt(n));
	for (auto f: factors){
		std::cout << f.first.toString() << " " << f.second << std::endl;
	}
}

void prime(){
	std::string n;
	std::cin >> n;
	std::cout << Cryptography::isPrime(BigInt(n)) << std::endl;
}

void log(){
	std::string g,b,n;
	std::cin >> g >> b >> n;
	std::cout << Cryptography::discrete_log(
		BigInt(g), BigInt(b), BigInt(n)).toString()
		<< std::endl;
}

void phi(){
	std::string n;
	std::cin >> n;
	std::cout << Cryptography::phi(BigInt(n)).toString() << std::endl;
}

void mu(){
	std::string n;
	std::cin >> n;
	std::cout << Cryptography::mu(BigInt(n)) << std::endl;
}

void legendre(){
	std::string a, n;
	std::cin >> a >> n;
	std::cout << Cryptography::legendre_symbol(BigInt(a), BigInt(n)) << std::endl;
}

void jacobi(){
	std::string a, n;
	std::cin >> a >> n;
	std::cout << Cryptography::jacobi_symbol(BigInt(a), BigInt(n)) << std::endl;
}

void sqrt(){
	std::string a,p;
	std::cin >> a >> p;
	std::cout << Cryptography::discrete_sqrt(a, p).toString() << std::endl;
}

void ElGamalDemo(){
	ElGamal ElGamal_ = ElGamal();

	auto keys = ElGamal_.createKeys();
	BigInt private_key = keys.first;
	auto public_key = keys.second;
	std::cout << "Keys created" << std::endl;

	std::string message, result="";
	std::cout << "Enter messsage:\n";
	std::cin >> message;

	for (char m: message){
		auto c = ElGamal_.encrypt_char(m, public_key);

		std::cout << "Encrypted point:\n" 
			<< "g = (" << c.first.x.toString() << ", " << c.first.y.toString() << ")" << std::endl
			<< "h = (" << c.second.x.toString() << ", " << c.second.y.toString() << ")" << std::endl;
		
		char d = ElGamal_.decrypt_char(private_key, c);
		result += d;
	}

	assert(result == message);
	std::cout << "Decrypted message:\n" << result << std::endl;

	return;
}

int main() {
	std::string command;
	help();
	while (true){
		std::cin >> command;

		if (command == "help"){
			help();
		}

		if (command == "factor"){
			factor();
		}

		if (command == "prime"){
			prime();
		}

		if (command == "log"){
			log();
		}

		if (command == "phi"){
			phi();
		}

		if (command == "mu"){
			mu();
		}

		if (command == "legendre"){
			legendre();
		}

		if (command == "jacobi"){
			jacobi();
		}

		if (command == "sqrt"){
			sqrt();
		}

		if (command == "ElGamal"){
			ElGamalDemo();
		}

		if (command == "exit"){
			return 0;
		}
	}
}