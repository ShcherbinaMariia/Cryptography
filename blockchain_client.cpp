#include "blockchain_client.hpp"
#include "limits.h"
#include <iostream>

BlockChainClient::BlockChainClient(){
	this->blockChain = BlockChain::load();
}

bool BlockChainClient::mineBlock(Address address){
	MiningTransaction mining_transaction(address);
	std::vector<SignedTransaction> pool = blockChain.getPool();
	LongHex merkle_root = Block::getMerkleRoot(pool, mining_transaction);

	Block::Header header;
	header.block_id = blockChain.getNumberOfBlocks();
	header.merkle_root = merkle_root;
	header.previous_block_header_hash = blockChain.getLastBlockHeaderHash();

	Block block;
	block.setTransactions(pool, mining_transaction);

	for (unsigned int nonce = 0; nonce < UINT_MAX; nonce++){
		header.nonce = nonce;
		block.setHeader(header);
		if (block.verifyMiningTransaction()){
			return blockChain.addBlock(block);
		}
	}
	return false;
}

void BlockChainClient::addSignedTransaction(RSA::Keys from, Address to, double amount){
	SignedTransaction signed_transaction;

	RawTransaction raw_transaction;
	raw_transaction.tx_id = blockChain.getPoolSize();
	raw_transaction.from = from.public_key;
	raw_transaction.to = to;
	raw_transaction.amount = amount;
	
	signed_transaction.setRawTransaction(raw_transaction);
	std::string raw_transaction_hash = signed_transaction.transactionTextToSign();
	RSA rsa;
	signed_transaction.setSignature(rsa.sign(raw_transaction_hash, from));

	blockChain.addSignedTransaction(signed_transaction);
}

bool BlockChainClient::verifyBlockchain(){
	return blockChain.verify();
}

double BlockChainClient::getBalance(Address address, int block){
	return blockChain.getBalance(address, block);
}

std::map<Address, double> BlockChainClient::getBalances(int block){
	return blockChain.getBalances(block);
}

void BlockChainClient::dump(){
	blockChain.dump();
}