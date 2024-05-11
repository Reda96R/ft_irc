#include "../../includes/Commands.hpp"
#include "../../includes/macros.hpp"
#include <string>
#include <sys/signal.h>

//::::::::::::::::::Constructors:::::::::::::::::::::::::
Commands::Commands( void ){
	this->commandsMap["PASS"] = &Commands::passCommand;
	this->commandsMap["NICK"] = &Commands::nickCommand;
	this->commandsMap["USER"] = &Commands::userCommand;
	this->commandsMap["JOIN"] = &Commands::joinCommand;
	this->commandsMap["PRIVMSG"] = &Commands::privmsgCommand;
}

//::::::::::::::::::Operators:::::::::::::::::::::::::
Commands& Commands::operator=( const Commands& rhs ){
	if (this != &rhs){
		this->commandsMap = rhs.commandsMap;
	}
	return (*this);
}


//::::::::::::::::::Getters and Setters:::::::::::::::::::::::::
std::map<std::string, void (Commands::*) ( Client& )> Commands::getCommandMap( void ) const{
	return (this->commandsMap);
}

//::::::::::::::::::Methods:::::::::::::::::::::::::
void	Commands::passCommand( Client& client ){
	std::string pass = "pass"; // this will be replaced with server password

	if (client.getStatus().authenticated)
		std::cerr << RED << client.getNickname() << " already registered" RESET << std::endl;
	else if (client.getInput().arguments.empty())
		std::cerr << RED << client.getNickname() << " need more parameters" RESET << std::endl;
	else if (client.getInput().arguments.size() < 2 && client.getInput().arguments[0] == pass)
	{
		client.setStatus("authenticated", true);
		client.setStatus("pass", true);
		std::cout << GREEN << client.getNickname() << " password accepted" << RESET << std::endl;
	}
	else
		std::cerr << RED << client.getNickname() << " password not accepted" << RESET << std::endl;
}

void	Commands::nickCommand( Client& client ){
	//TODO:
	// √ check if AUTHENTICATED
	// √ check if already REGISTERED
	// √ check if command's parameters exist
	// ---> check if the provided nickname already exsits <---
	// √ set the nick name

	if (!client.getStatus().authenticated){
		std::cerr << RED << client.getNickname() << " not authenticated" RESET << std::endl;
		return ;
	}
	else if (client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " already registered" RESET << std::endl;
		return ;
	}
	else if (client.getInput().arguments.empty() || client.getInput().arguments[0].empty()){
		std::cerr << RED << client.getNickname() << " need more parameters" RESET << std::endl;
		return ;
	}
	else if (client.getInput().arguments.size() < 2)
	{
		client.setNickname(client.getInput().arguments[0]);
		client.setStatus("nick", true);
		if (client.getStatus().user)
			client.setStatus("registered", true);
		std::cout << GREEN << client.getNickname() << " nickname accepted" << RESET << std::endl;
	}
}

void	Commands::userCommand( Client& client ){
	//TODO:
	// √ check if AUTHENTICATED
	// √ check if already REGISTERED
	// √ check if command's parameters exist if not set username to unknown
	// √ set the username

	if (!client.getStatus().authenticated){
		std::cerr << RED << client.getNickname() << " not authenticated" RESET << std::endl;
		return ;
	}
	else if (client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " already registered" RESET << std::endl;
		return ;
	}
	else if (client.getInput().arguments.empty() || client.getInput().arguments[0].empty()){
		std::string	  tmp = "unknown";
		client.setUsername(tmp);
		client.setStatus("user", true);
		std::cout << GREEN << client.getNickname() << " username accepted" << RESET << std::endl;
	}
	else if (client.getInput().arguments.size() < 2)
	{
		client.setUsername(client.getInput().arguments[0]);
		client.setStatus("user", true);
		if (client.getStatus().nick)
			client.setStatus("registered", true);
		std::cout << GREEN << client.getNickname() << " userame accepted" << RESET << std::endl;
	}
}

void	Commands::joinCommand( Client& client ){
	std::cout << "entering the join command" << std::endl;
	(void) client;
}

void	Commands::privmsgCommand( Client& client ){
	//TODO:
	// √ check if not REGISTERED
	// * check msg destination
	// * check text msg validity

	if (!client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " not registered" RESET << std::endl;
		return ;
	}
	if (client.getInput().arguments.empty())
		std::cout << RED << "Nothing to be sent" << RESET << std::endl;
	std::cout << client.getInput().arguments[0] << std::endl;
}


Commands::Commands( const Commands& src ){
	*this = src;
}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Commands::~Commands( void ){
}
