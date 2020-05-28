#include "merkle_tree.hpp"

SHA256* MerkleTree::hasher = new SHA256();

LongHex MerkleTree::calculateRoot(std::vector<std::string>& inputs){
	if (inputs.size() == 0) 
		return LongHex();

	std::vector<std::vector<LongHex>> state;
	state.resize(2);

	int curr = 0;
	for (auto input: inputs){
		state[curr].push_back(hasher->hash(input));
	}

	while (state[curr].size() != 1){
		for (int j = 0; j < state[curr].size(); j += 2){
			LongHex node_value = state[curr][j];
			node_value = node_value.concatenate(
				(j + 1 < state[curr].size()) ? state[curr][j + 1] : node_value);
			state[1 - curr].push_back(hasher->hash(node_value));
		}
		state[curr].clear();
		curr = 1 - curr;
	}

	return state[curr][0];
}