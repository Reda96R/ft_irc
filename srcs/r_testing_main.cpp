#include "../includes/Client.hpp"

int	  main ( void ){
	std::string input = ":pref JOIN -a1 -a2";
	Client	client;

	commandParser(input, client.getInput());

	std::cout << "prefix---> " << client.getInput().prefix << std::endl;
	std::cout << "command---> " << client.getInput().command << std::endl;
	std::cout << "arg1---> " << client.getInput().arguments.top()<< std::endl;
	client.getInput().arguments.pop();
	std::cout << "arg2---> " << client.getInput().arguments.top()<< std::endl;
}
