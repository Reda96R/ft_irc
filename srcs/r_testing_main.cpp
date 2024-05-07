#include "../includes/Client.hpp"

int	  main ( void ){
	Client	client;

	std::string input = ":pref PASS  pass";
	commandParser(input, client, client.getInput());
	input = "NICK reda";
	commandParser(input, client, client.getInput());
	std::cout << client.getNickname() << std::endl;
	input = "NICK test";
	commandParser(input, client, client.getInput());
	std::cout << client.getNickname() << std::endl;

	// std::cout << "prefix---> " << client.getInput().prefix << std::endl;
	// std::cout << "command---> " << client.getInput().command << std::endl;
	// std::cout << "arg1---> " << client.getInput().arguments.top()<< std::endl;
	// client.getInput().arguments.pop();
	// std::cout << "arg2---> " << client.getInput().arguments.top()<< std::endl;
}
