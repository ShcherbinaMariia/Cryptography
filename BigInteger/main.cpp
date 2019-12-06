#include "interface.cpp"

int main() {
	help();
	while (true){
		std::string line;
		std::getline (std::cin, line);
		if (!processLine(line)) return 0;
	}
}