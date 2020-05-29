#include "group_rsa.hpp"
#include <iostream>

int GroupRSA::public_exponent = 65537;

bool GroupRSA::join(GroupRSAClient* new_client){
	for (auto clientEntry: clients){
		if (gcd(clientEntry.second->getModulo(), new_client->getModulo()) != 1) return false;
	}
	clients[new_client->getUsername()] = new_client;

	for (auto clientEntry: clients){
		clientEntry.second->updateModules(new_client->getUsername(), new_client->getModulo(), true);
	}
	return true;
}

void GroupRSA::left(GroupRSAClient* client_to_leave){
	for (auto clientEntry: clients){
		clientEntry.second->updateModules(client_to_leave->getUsername(), client_to_leave->getModulo(), false);
	}
	clients.erase(client_to_leave->getUsername());
}

void GroupRSA::sendMessage(Message message){
	for (auto clientEntry: clients){
		clientEntry.second->getMessage(message);
	}
}

GroupRSAClient::GroupRSAClient(std::string username){
	this->username = username;
	this->keys = rsa.createKeys(32, GroupRSA::public_exponent);
}

void GroupRSAClient::updateModules(std::string username, BigInt modulo, bool add){
	if (add) this->modules_by_username[username] = modulo;
	else this->modules_by_username.erase(username);
}

std::string GroupRSAClient::getUsername(){
	return this->username;
}

BigInt GroupRSAClient::getModulo(){
	return this->keys.n;
}

void GroupRSAClient::getMessage(Message message){
	std::cout << "User: " << this->getUsername() 
	<< "received message: " << this->decryptMessage(message.content)
	<< " " << "from user: " << message.author;
}

Message GroupRSAClient::createMessage(std::string message_content){
	return Message(this->getUsername(), this->encryptMessage(message_content));
}

const BigInt chineseRemaindersTheorem(std::vector<BigInt> remainders, std::vector<BigInt> modules) {
	int n = remainders.size();
	std::vector<std::vector<BigInt>> inverse;
	inverse.resize(n);
	
	for (int i = 0; i < n; i++){
		for(int j = i + 1; j < n; j++){
			inverse[i].push_back(findInverse(modules[i], modules[j]));
		}
	}

	BigInt result(0), multiplier(1);
	std::vector<BigInt> x;
	x.resize(n);

	for (int i = 0; i < n; i++){
		x[i] = remainders[i];
		for (int j = 0; j < i; j++){
			x[i] = (x[i] - x[j]) * inverse[j][i];
			x[i] = (x[i] + modules[i]) % modules[i];
		}
		result = result + multiplier * x[i];
		multiplier = multiplier * modules[i];
	}
	return result;
}

std::vector<BigInt> GroupRSAClient::encryptMessage(std::string message){
	std::vector<BigInt> modules;
	for (auto entry: modules_by_username) modules.push_back(entry.second);

	std::vector<std::vector<BigInt>> encrypted_by_modulo;
	for (auto modulo: modules){	
		encrypted_by_modulo.push_back(rsa.encrypt(message, keys.public_key, modulo));
	}

	std::vector<BigInt> remainders;
	std::vector<BigInt> encrypted_for_all;
	for (int chunk = 0; chunk < encrypted_by_modulo[0].size(); chunk++){
		remainders.clear();
		for (int client = 0; client < modules.size(); client++){
			remainders.push_back(encrypted_by_modulo[client][chunk]);
		}
		encrypted_for_all.push_back(chineseRemaindersTheorem(remainders, modules));
	}

	return encrypted_for_all;
}

std::string GroupRSAClient::decryptMessage(std::vector<BigInt> encrypted){
	return rsa.decrypt(encrypted, keys.private_key, keys.n);
}