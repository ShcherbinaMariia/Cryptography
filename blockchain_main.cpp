#include "blockchain_client.hpp"
#include<map>
#include<iostream>

int main(){
	BlockChainClient blockchain_client;
	std::map<std::string, RSA::Keys> account;
	std::map<std::string, BigInt> to;
	std::string alias;
	RSA rsa;

	std::string command;
	while (std::cin >> command){

		if (command == "create"){
			std::cout << "alias: ";
			std::cin >> alias;
			account[alias] = rsa.createKeys(32, 0);
			to[alias] = account[alias].public_key;
		}

		if (command == "mine"){
			std::cout << "alias: ";
			std::cin >> alias;
			if (blockchain_client.mineBlock(account[alias].public_key)) {
				std::cout << "\nSuccess.";
			} else {
				std::cout << "\nFailed.";
			}
		}

		if (command == "add"){
			std::string accFrom, accTo;
			double amount;
			std::cout << "Enter account aliases from, to and amount\n";
			std::cin >> accFrom >> accTo >> amount;
			blockchain_client.addSignedTransaction(account[accFrom], to[accTo], amount);
		}

		if (command == "verify"){
			if (blockchain_client.verifyBlockchain()){
				std::cout << "Verified. Data is consistent.";
			} else {
				std::cout << "Data is inconsistent";
			}
		}

		if (command == "set"){
			std::cout << "Enter alias and address\n" << std::endl;
			std::string address;
			std::cin >> alias >> address;
			to[alias] = BigInt(address);
		}

		if (command == "balance"){
			int block;
			std::cout << "Enter alias and block ";
			std::cin >> alias >> block;
			std::cout << blockchain_client.getBalance(to[alias], block);
		}

		if (command == "balances"){
			int block;
			std::cout << "Enter block ";
			std::cin >> block;
			auto result = blockchain_client.getBalances(block);

			for (auto pos = result.begin(); pos != result.end(); pos++){
				std::cout << "Address: " << (*pos).first.toString() << ". Balance: "  << (*pos).second << "\n";
			}
		}

		if (command == "list"){
			for (auto pos = account.begin(); pos != account.end(); pos++){
				std::cout << (*pos).first << " address: " << (*pos).second.public_key.toString() << "\n";
			}
		}

		if (command == "help"){
			std::cout << "create : add new address" << std::endl;
			std::cout << "mine : mine next block" << std::endl;
			std::cout << "list : list all account aliases" << std::endl;
			std::cout << "add : create transaction" << std::endl;
			std::cout << "verify : verify blockchain" << std::endl;
			std::cout << "set : set alias for address" << std::endl;
			std::cout << "balance : get balance" << std::endl;
			std::cout << "exit : exit client" << std::endl;
		}

		if (command == "exit"){
			blockchain_client.dump();
			return 0;
		}

		std::cout << "\n";
	}
}