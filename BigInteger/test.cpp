#include "BigInt.hpp"
#include <iostream>

const BigInt bigOne = BigInt(1), bigZero = BigInt(0);

bool test_rules() {

	std::vector<BigInt> numbers = std::vector<BigInt>();
	numbers.push_back(BigInt("12345"));
	numbers.push_back(BigInt("12167237126317"));
	numbers.push_back(BigInt("-133424353353"));
	numbers.push_back(BigInt("-12899345"));
	numbers.push_back(BigInt("12343342311213323445"));

	for (auto number: numbers){
		assert(number / number == bigOne);
		assert(number - number == bigZero);
		assert(number + -number == bigZero);
		assert(number % number == bigZero);

		assert(number + bigZero == number);
		assert(number * 1 == number);
		assert(number * bigOne == number);
		assert(number * bigZero == bigZero);

		assert(number + number == number * 2);

		assert(sqrt(number * number) == abs(number));

		assert(power(number, 0, bigZero) == bigOne);
		assert(power(number, 1, bigZero) == number);
		assert(power(number, 2, bigZero) == number * number);

		for (auto number2: numbers){

			assert(number + number2 == number2 + number);
			assert(number * number2 == number2 * number);
			assert(number - number2 == -(number2  - number));
			assert(number * number2 / number == number2);
			assert(number * number2 / number2 == number);

			assert(!(number > number2 & number2 > number));

			assert(abs(number) + abs(number2) >= abs(number + number2));

			assert(number * (number + number2) == number * number + number * number2);
		}
	}

	return true;
}

int main() {
	test_rules();
}