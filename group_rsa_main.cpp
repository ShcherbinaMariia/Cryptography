#include "group_rsa.hpp"
int main(){
	GroupRSA group_rsa;

	GroupRSAClient* alice = new GroupRSAClient("alice");
	GroupRSAClient* bob = new GroupRSAClient("bob");
	GroupRSAClient* carol = new GroupRSAClient("carol");

	group_rsa.join(alice);
	group_rsa.sendMessage(alice->createMessage("Hi!"));
}