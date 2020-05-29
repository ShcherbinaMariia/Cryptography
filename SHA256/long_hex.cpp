#include "long_hex.h"

const char LongHex::kAlphabet[16] = {'0','1','2','3','4','5','6','7',
                                     '8','9','a','b','c','d','e','f'};

LongHex::LongHex(){
	bytes.push_back(0);
}

LongHex::LongHex(unsigned int* s, int n){
	bytes.resize(n);

	for (int i = 0; i < n; i++){
		for (int shift = 24; shift >= 0; shift -= 8){
			bytes.push_back((s[i] >> shift) & 0xFF);
		}
	}
}

std::string LongHex::toString(){
	std::string result = "";
	for (int i = 0; i < bytes.size(); i++){
		result += kAlphabet[bytes[i] >> 4 & 0xF];
		result += kAlphabet[bytes[i] & 0xF];
	}
	return result;
}

const LongHex LongHex::concatenate(LongHex& other){
	LongHex long_hex;
	std::vector<byte> bytes;
	bytes.resize(this->bytes.size() + other.getBytes().size());
	
	for (int i = 0; i < this->bytes.size(); i++){
		bytes[i] = this->bytes[i];
	}

	for (int i = 0; i < other.getBytes().size(); i++){
		bytes[this->bytes.size() + i] = other.getBytes()[i];
	}

	long_hex.setBytes(this->bytes);
	return long_hex;
}

std::vector<byte> LongHex::getBytes() const {
	return this->bytes;
}

void LongHex::setBytes(std::vector<byte> bytes) {
	this->bytes = bytes;
}

bool LongHex::operator == (LongHex const &other) const{
	if (this->bytes.size() != other.getBytes().size()) return false;

	for (int i = 0; i < this->bytes.size(); i++){
		if (this->bytes[i] != other.getBytes()[i]) return false;
	}
	return true;
}

bool LongHex::operator != (LongHex const &other) const{
	return !(*this == other);
}

bool LongHex::hasZerosPrefix(int nBits) const{
	int zeroBits = 0;
	int n = 0;
	while (n < this->bytes.size() && zeroBits < nBits){
		for (int shift = 7; shift >=0; shift--){
			if ((this->bytes[n] >> shift) & 1) return false;
			zeroBits++;
			if (zeroBits == nBits) return true;
		}
		n++;
	}
	return false;
}

void to_json(json& j, const LongHex& long_hex){
	j = json{{"bytes", long_hex.getBytes()}};
}

void from_json(const json& j, LongHex& long_hex){
	std::vector<byte> bytes;
	j.at("bytes").get_to(bytes);
	long_hex.setBytes(bytes);
}