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

const LongHex& LongHex::concatenate(LongHex& other){
	this->bytes.insert(this->bytes.end(), other.getBytes().begin(), other.getBytes().end());
	return *this;
}

std::vector<byte> LongHex::getBytes(){
	return this->bytes;
}