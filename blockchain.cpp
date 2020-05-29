#include "blockchain.hpp"
#include <fstream>
#include <iostream>

// serialization routines
void to_json(json& j, const RawTransaction& raw_transaction){
	j = json{{"tx_id", raw_transaction.tx_id},
			 {"from", raw_transaction.from},
			 {"to", raw_transaction.to},
			 {"amount", raw_transaction.amount}};
}

void from_json(const json& j, RawTransaction& raw_transaction){
	j.at("tx_id").get_to(raw_transaction.tx_id);
	j.at("from").get_to(raw_transaction.from);
	j.at("to").get_to(raw_transaction.to);
	j.at("amount").get_to(raw_transaction.amount);
}

void to_json(json& j, const SignedTransaction& signed_transaction){
	j = json{{"raw_transaction", signed_transaction.getRawTransaction()},
			 {"signature", signed_transaction.getSignature()}};
}

void from_json(const json& j, SignedTransaction& signed_transaction){
	RawTransaction raw_transaction;
	j.at("raw_transaction").get_to(raw_transaction);
	signed_transaction.setRawTransaction(raw_transaction);

	RSA::Signature signature;
	j.at("signature").get_to(signature);
	signed_transaction.setSignature(signature);
}

void to_json(json& j, const MiningTransaction& mining_transaction){
	j = json{{"raw_transaction", mining_transaction.getRawTransaction()}};
}

void from_json(const json& j, MiningTransaction& mining_transaction){
	RawTransaction raw_transaction;
	j.at("raw_transaction").get_to(raw_transaction);
	mining_transaction.setRawTransaction(raw_transaction);
}

void to_json(json& j, const Block::Header& header){
	j = json{{"block_id", header.block_id},
			 {"previous_block_header_hash", header.previous_block_header_hash},
			 {"merkle_root", header.merkle_root},
		     {"nonce", header.nonce}};
}

void from_json(const json& j, Block::Header& header){
	j.at("block_id").get_to(header.block_id);
	j.at("previous_block_header_hash").get_to(header.previous_block_header_hash);
	j.at("merkle_root").get_to(header.merkle_root);
	j.at("nonce").get_to(header.nonce);
}

void to_json(json& j, const Block& block){
	j = json{{"header", block.getHeader()},
			 {"signed_transactions", block.getSignedTransactions()},
			 {"mining_transaction", block.getMiningTransaction()}};
}

void from_json(const json& j, Block& block){
	Block::Header header;
	j.at("header").get_to(header);
	block.setHeader(header);

	std::vector<SignedTransaction> signed_transactions;
	j.at("signed_transactions").get_to(signed_transactions);

	MiningTransaction mining_transaction;
	j.at("mining_transaction").get_to(mining_transaction);

	block.setTransactions(signed_transactions, mining_transaction);
}

void to_json(json& j, const BlockChain& blockchain){
	j = json{{"blocks", blockchain.getBlocks()},
			 {"pool", blockchain.getPool()}};
}

void from_json(const json& j, BlockChain& blockchain){
	std::vector<Block> blocks;
	j.at("blocks").get_to(blocks);
	blockchain.setBlocks(blocks);

	std::vector<SignedTransaction> pool;
	j.at("pool").get_to(pool);
	blockchain.setPool(pool);
}

const int BlockChain::kTargetZerosPrefix = 2;
const LongHex BlockChain::kZeroBlockHash = LongHex();
const double BlockChain::kBlockReward = 10;
const Address BlockChain::kCoinMintingAddress = Address();
const std::string BlockChain::kDumpFile = "blockchain_dump.txt";

bool BlockChain::verify(){
	LongHex previous_block_header_hash = BlockChain::kZeroBlockHash;
	for (Block block: blocks){
		if (block.getHeader().previous_block_header_hash != previous_block_header_hash) return false;
		if (!block.verify(previous_block_header_hash)) return false;
		previous_block_header_hash = block.getHeaderHash();
	}
	return true;
}

std::vector<SignedTransaction> BlockChain::getPool() const{
	return this->pool;
}

bool Block::verify(LongHex previous_block_header_hash){
	if (this->getHeader().previous_block_header_hash != previous_block_header_hash)
		return false;

	for (SignedTransaction transaction: transactions){
		if (!transaction.verify()) return false;
	}
	return this->verifyMiningTransaction() && this->verifyMerkleRoot();
}

LongHex Block::getMerkleRoot(std::vector<SignedTransaction>& signed_transactions, MiningTransaction mining_transaction){
	std::vector<std::string> serialized_transactions;
	for (SignedTransaction signed_transaction: signed_transactions){
		json serialized_transaction = signed_transaction;
		serialized_transactions.push_back(serialized_transaction.dump());
	}
	json serialized_transaction = mining_transaction;
	serialized_transactions.push_back(serialized_transaction.dump());
	return MerkleTree::calculateRoot(serialized_transactions);
}

std::string Transaction::transactionTextToSign(){
	SHA256 sha256;
	RawTransaction raw_transaction = this->getRawTransaction();
	json raw_transaction_json = raw_transaction;
	std::string raw_transaction_hash = sha256.hash(raw_transaction_json.dump()).toString();
	return raw_transaction_hash;
}

bool SignedTransaction::verify(){ 
	RSA rsa;
	if (this->getRawTransaction().from == BlockChain::kCoinMintingAddress) return false;
	std::string raw_transaction_hash = this->transactionTextToSign();
	return rsa.verify(raw_transaction_hash, this->signature) && this->signature.public_key == this->getRawTransaction().from;
}

void SignedTransaction::setSignature(RSA::Signature signature){
	this->signature = signature;
}

RSA::Signature SignedTransaction::getSignature() const {
	return this->signature;
}

bool MiningTransaction::verify(){
	RawTransaction raw_transaction = this->getRawTransaction();
	return raw_transaction.amount == BlockChain::kBlockReward && raw_transaction.from == BlockChain::kCoinMintingAddress;
}

MiningTransaction::MiningTransaction(){}

MiningTransaction::MiningTransaction(Address address){
	RawTransaction raw_transaction;
	raw_transaction.from = BlockChain::kCoinMintingAddress;
	raw_transaction.amount = BlockChain::kBlockReward;
	raw_transaction.to = address;
	this->setRawTransaction(raw_transaction);
}

RawTransaction Transaction::getRawTransaction() const{
	return this->transaction;
}

void Transaction::setRawTransaction(RawTransaction transaction){
	this->transaction = transaction;
}

Block::Header Block::getHeader() const{
	return this->header;
}

void Block::setHeader(Block::Header header){
	this->header = header;
}

void Block::setTransactions(std::vector<SignedTransaction> signed_transactions, MiningTransaction mining_transaction){
	this->transactions = signed_transactions;
	this->mining_transaction = mining_transaction;
}

MiningTransaction Block::getMiningTransaction() const{
	return this->mining_transaction;
}

std::vector<SignedTransaction> Block::getSignedTransactions() const{
	return this->transactions;
}

LongHex Block::getHeaderHash(){
	SHA256 sha256; 
	json header = this->getHeader();
	return sha256.hash(header.dump());
}

bool Block::verifyMerkleRoot(){
	return this->getHeader().merkle_root == Block::getMerkleRoot(this->transactions, this->mining_transaction);
}

bool Block::verifyMiningTransaction(){
	return this->getHeaderHash().hasZerosPrefix(BlockChain::kTargetZerosPrefix);
}

void Block::updateBalances(std::map<Address, double>& balances){
	balances[mining_transaction.getRawTransaction().to] += mining_transaction.getRawTransaction().amount;

	for (SignedTransaction transaction: transactions){
		RawTransaction raw_transaction;
		balances[raw_transaction.from] -= balances[raw_transaction.amount];
		balances[raw_transaction.to] += balances[raw_transaction.amount];
	}
}

bool BlockChain::addBlock(Block block){
	if (!block.verify(this->getLastBlockHeaderHash()))
		return false;
	
	this->blocks.push_back(block);
	this->pool.clear();
	return true;
}

bool BlockChain::addSignedTransaction(SignedTransaction signed_transaction){
	if (!signed_transaction.verify()) return false;
	pool.push_back(signed_transaction);
	return true;
}

LongHex BlockChain::getLastBlockHeaderHash(){
	return this->blocks.size() != 0 ? this->blocks.back().getHeaderHash() : kZeroBlockHash;
}

int BlockChain::getNumberOfBlocks(){
	return this->blocks.size();
}

int BlockChain::getPoolSize(){
	return this->pool.size();
}

std::vector<Block> BlockChain::getBlocks() const{
	return this->blocks;
}

void BlockChain::setPool(std::vector<SignedTransaction> pool){
	this->pool = pool;
}

void BlockChain::setBlocks(std::vector<Block> blocks){
	this->blocks = blocks;
}

std::map<Address, double> BlockChain::getBalances(int block){
	std::map<Address, double> balance;
	
	if (block == -1){
		block = getNumberOfBlocks();
	}
	
	if (block > getNumberOfBlocks()) return balance;

	for (int i = 0; i < block; i++){
		this->blocks[i].updateBalances(balance);
	}

	return balance;
}

double BlockChain::getBalance(Address address, int block){
	return getBalances(block)[address];
}

void BlockChain::dump(){
	json blockchain_json = *this;
	std::ofstream dump_file;
	dump_file.open(kDumpFile);
	dump_file << blockchain_json.dump();
    dump_file.close();
}

BlockChain BlockChain::load(){
	std::ifstream dump_file;
	dump_file.open(kDumpFile);

	if (!dump_file.is_open()){
		return BlockChain();
	}
	
	json blockchain_json;
	dump_file >> blockchain_json;
    dump_file.close();

    auto blockchain = blockchain_json.get<BlockChain>();

    return blockchain;
}
