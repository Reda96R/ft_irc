#include "../includes/Client.hpp"
#include "../includes/macros.hpp"

int	  main ( void ){
	Client	client;

	std::string input = ":pref PASS  pass";
	commandParser(input, client);
	input = "NICK testnickname";
	commandParser(input, client);
	std::cout << YELLOW << "nickname :::> " << client.getNickname() << RESET << std::endl;

	std::cout << std::endl;
	std::cout << CYAN << "::::::::::::::::::::::::::::::::::::::::::::::" << RESET << std::endl;
	std::cout << std::endl;

	input = "USER testuser";
	commandParser(input, client);
	std::cout << YELLOW << "username :::> " << client.getUsername() << RESET << std::endl;

	// std::cout << "prefix---> " << client.getInput().prefix << std::endl;
	// std::cout << "command---> " << client.getInput().command << std::endl;
	// std::cout << "arg1---> " << client.getInput().arguments.top()<< std::endl;
	// client.getInput().arguments.pop();
	// std::cout << "arg2---> " << client.getInput().arguments.top()<< std::endl;
}
