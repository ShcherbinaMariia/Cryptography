#include "RSA.hpp"

struct Message{
	std::string author;
	std::vector<BigInt> content;

	Message(std::string author, std::vector<BigInt> content){
		this->author = author;
		this->content = content;
	}
};

class GroupRSAClient{
	RSA rsa;
	RSA::Keys keys;
	std::string username;
	std::map<std::string, BigInt> modules_by_username; 
public:
	GroupRSAClient(std::string username);
	std::string getUsername();
	BigInt getModulo();

	void updateModules(std::string username, BigInt modulo, bool add = true);
	void getMessage(Message message);
	Message createMessage(std::string message_content);

	std::vector<BigInt> encryptMessage(std::string message);
	std::string decryptMessage(std::vector<BigInt> encrypted);
};

class GroupRSA {
	std::map<std::string, GroupRSAClient*> clients;
	
public:
	static int public_exponent;
	bool join(GroupRSAClient* client);
	void sendMessage(Message message);

	std::vector<BigInt> getModules();
	void left(GroupRSAClient* client);
};
