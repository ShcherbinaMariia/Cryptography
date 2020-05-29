#pragma once
#include <vector>
#include <map>
#include "RSA.hpp"
#include "sha256.h"
#include "merkle_tree.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using Address = BigInt;

struct RawTransaction {
	unsigned int tx_id;
	Address from;
	Address to;
	double amount;
};

class Transaction{
protected:
	RawTransaction transaction;
public:
	RawTransaction getRawTransaction() const;
	std::string transactionTextToSign();
	void setRawTransaction(RawTransaction transaction);
	virtual bool verify() = 0;
};

class SignedTransaction : public Transaction{
	RSA::Signature signature;
public:
	bool verify();
	void setSignature(RSA::Signature signature);
	RSA::Signature getSignature() const;
};

class MiningTransaction : public Transaction{
public:
	MiningTransaction();
	MiningTransaction(Address address);
	bool verify();
};

class Block {
public:
	struct Header{
		unsigned int block_id;
		LongHex previous_block_header_hash;
		LongHex merkle_root;
		unsigned int nonce;
	};
	static LongHex getMerkleRoot(std::vector<SignedTransaction>& signed_transactions, MiningTransaction mining_transaction);
	bool verify(LongHex previous_block_header_hash);
	LongHex getHeaderHash();
	Block::Header getHeader() const;
	void setHeader(Block::Header header);
	void setTransactions(std::vector<SignedTransaction> signed_transactions, MiningTransaction mining_transaction);
	MiningTransaction getMiningTransaction() const;
	std::vector<SignedTransaction> getSignedTransactions() const;
	bool verifyMiningTransaction();
	void updateBalances(std::map<Address, double>& balances);

private:
	Header header;
	std::vector<SignedTransaction> transactions;
	MiningTransaction mining_transaction;
	bool verifyMerkleRoot();
};

class BlockChain {
	std::vector<Block> blocks;
	std::vector<SignedTransaction> pool;
	std::vector<std::map<Address, double>> balances;

public:
	const static int kTargetZerosPrefix;
	const static LongHex kZeroBlockHash;
	const static double kBlockReward;
	const static Address kCoinMintingAddress;
	const static std::string kDumpFile;
	
	static BlockChain load();
	void dump();

	std::vector<SignedTransaction> getPool() const;
	std::vector<Block> getBlocks() const;

	void setPool(std::vector<SignedTransaction> pool);
	void setBlocks(std::vector<Block> blocks);

	int getPoolSize();
	bool verify();
	bool addBlock(Block block);
	bool addSignedTransaction(SignedTransaction signed_transaction);
	LongHex getLastBlockHeaderHash();
	int getNumberOfBlocks();

	std::map<Address, double> getBalances(int block = -1);
	double getBalance(Address address, int block = -1);
};