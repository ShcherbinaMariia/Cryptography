#include "blockchain.hpp"

class BlockChainClient{
	BlockChain blockChain;
public:
	BlockChainClient();
	bool mineBlock(Address address);
	void addSignedTransaction(RSA::Keys from, Address to, double amount);
	bool verifyBlockchain();
	double getBalance(Address address, int block);
	void dump();
	std::map<Address, double> getBalances(int block);
};