#include "../../includes/Commands.hpp"

//::::::::::::::::::Constructors:::::::::::::::::::::::::
Commands::Commands( void ){
			/* ~~~general commands ~~~ */
	this->commandsMap["PASS"] = &Commands::passCommand;
	this->commandsMap["NICK"] = &Commands::nickCommand;
	this->commandsMap["USER"] = &Commands::userCommand;
	this->commandsMap["JOIN"] = &Commands::joinCommand;
	this->commandsMap["PRIVMSG"] = &Commands::privmsgCommand;
			/* ~~~channel commands ~~~ */
	this->commandsMap["KICK"] = &Commands::kickChannelCommand;
	this->commandsMap["INVITE"] = &Commands::inviteChannelCommand;
	this->commandsMap["TOPIC"] = &Commands::topicChannelCommand;
	this->commandsMap["MODE"] = &Commands::modeChannelCommand;
}

Commands::Commands( const Commands& src ){
	*this = src;
}

//::::::::::::::::::Operators:::::::::::::::::::::::::
Commands& Commands::operator=( const Commands& rhs ){
	if (this != &rhs){
		this->commandsMap = rhs.commandsMap;
	}
	return (*this);
}

//::::::::::::::::::Getters and Setters:::::::::::::::::::::::::
std::map<std::string, void (Commands::*) ( Client&, struct ServerInfo& )> Commands::getCommandMap( void ) const{
	return (this->commandsMap);
}

//::::::::::::::::::Commands:::::::::::::::::::::::::
			/* ~~~general commands ~~~ */
void	Commands::passCommand( Client& client, struct ServerInfo& ){
	std::string pass = "pass"; // this will be replaced with server password

	if (trailingCheck(client.getInput().arguments))
		return ;

	if (client.getStatus().authenticated)
		std::cerr << RED << client.getNickname() << " already registered" RESET << std::endl;
	else if (client.getInput().arguments.empty())
		std::cerr << RED << client.getNickname() << " need more parameters" RESET << std::endl;
	else if (client.getInput().arguments.size() < 2 && client.getInput().arguments.at(0) == pass)
	{
		client.setStatus("authenticated", true);
		client.setStatus("pass", true);
		std::cout << GREEN << client.getNickname() << " password accepted" << RESET << std::endl;
	}
	else
		std::cerr << RED << client.getNickname() << " password not accepted" << RESET << std::endl;
}

void	Commands::nickCommand( Client& client, struct ServerInfo& ){
	//TODO:
	// √ check if AUTHENTICATED
	// √ check if already REGISTERED
	// √ check if command's parameters exist
	// ---> check if the provided nickname already exsits <---
	// √ set the nick name

	if (trailingCheck(client.getInput().arguments))
		return ;

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

void	Commands::userCommand( Client& client, struct ServerInfo& ){
	//TODO:
	// √ check if AUTHENTICATED
	// √ check if already REGISTERED
	// √ check if command's parameters exist if not set username to unknown
	// √ set the username

	if (trailingCheck(client.getInput().arguments))
		return ;

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

void	Commands::joinCommand( Client& client, struct ServerInfo& ){
	std::cout << "entering the join command" << std::endl;
	(void) client;
}

void	Commands::privmsgCommand( Client& client, struct ServerInfo& ){
	//TODO:
	// √ check if not REGISTERED
	// * check msg destination
	// * check text msg validity

	if (!client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " not registered" RESET << std::endl;
		return ;
	}

	s_prvMsgCommand			  privmsgInput;

	if (!privmsgAnalyser( client.getInput().arguments, privmsgInput))
		std::cout << RED << "failed" << RESET << std::endl;
	else{
		// std::cout << GREEN << "success" << RESET << std::endl;
		//TODO:
		//	- check if the target is user or a channel:
		//	  * channel:
		//		* check if the user is a member of the channel (client class will contain
		//		data structure that contains the names of the channels that the client is part of)
		//		 -->check if the channel exists (to send nosuchchannel or cannotsendtochan)
		//	  * user:
		//		* search for the user if found sent the message if not return nosuchnick
		//

		while (!privmsgInput.targets.empty()){
			if (isValidChannelName(privmsgInput.targets.top())){
				std::cout << GREEN << "valid channel" << RESET << std::endl;
			}
			else{
				std::cout << RED << "invalid channel" << RESET << std::endl;
			}
			privmsgInput.targets.pop();
		}
	}
}

			/* ~~~channel commands ~~~ */
void	Commands::kickChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " not registered" RESET << std::endl;
		return ;
	}

}

void	Commands::inviteChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " not registered" RESET << std::endl;
		return ;
	}
}

void	Commands::topicChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " not registered" RESET << std::endl;
		return ;
	}
}

void	Commands::modeChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " not registered" RESET << std::endl;
		return ;
	}
}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Commands::~Commands( void ){
}
