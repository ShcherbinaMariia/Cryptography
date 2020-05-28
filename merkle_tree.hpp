#include "sha256.h"
#include <vector>

class MerkleTree {
	static SHA256* hasher;

public:
	static LongHex calculateRoot(std::vector<std::string>& inputs);
};