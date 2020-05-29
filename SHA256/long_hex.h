#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using byte = unsigned char;

class LongHex{
	const static char kAlphabet[16];
	std::vector<byte> bytes;
	
public:
	LongHex();
	LongHex(unsigned int* s, int n);
	const LongHex concatenate(LongHex& other);
	
	std::vector<byte> getBytes() const;
	void setBytes(std::vector<byte> bytes);

	bool operator == (LongHex const &other) const;
	bool operator != (LongHex const &other) const;
	bool hasZerosPrefix(int nBits) const;

	std::string toString();
};

void to_json(json& j, const LongHex& long_hex);
void from_json(const json& j, LongHex& long_hex);