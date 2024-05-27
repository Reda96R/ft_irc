#include "../../includes/Commands.hpp"
# include "../../includes/Server.hpp"

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
void	Commands::passCommand( Client& client, struct ServerInfo& serverInfo ){
	if (trailingCheck(client.getInput().arguments))
		return ;

	if (client.getStatus().authenticated)
		std::cerr << RED << "Already registered" RESET << std::endl;

	else if (client.getInput().arguments.empty())
		std::cerr << RED <<  "Need more parameters" RESET << std::endl;

	else if (client.getInput().arguments.size() < 2 && client.getInput().arguments.at(0) == serverInfo.password){
		client.setStatus("authenticated", true);
		client.setStatus("pass", true);
		std::cout << GREEN << "Password accepted" << RESET << std::endl;
	}

	else
		std::cerr << RED << "Password not accepted" << RESET << std::endl;
}

void	Commands::nickCommand( Client& client, struct ServerInfo& serverInfo ){
	if (trailingCheck(client.getInput().arguments))
		return ;

	if (!client.getStatus().authenticated){
		std::cerr << RED << "Not authenticated" RESET << std::endl;
		return ;
	}
	else if (client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " already registered" RESET << std::endl;
		return ;
	}
	else if (client.getInput().arguments.empty() || client.getInput().arguments.at(0).empty()){
		std::cerr << RED << client.getNickname() << " need more parameters" RESET << std::endl;
		return ;
	}
	else if (client.getInput().arguments.size() < 2)
	{
		for (size_t i = 0; i < serverInfo.clients.size(); ++i){
			if (client.getInput().arguments.at(0) == serverInfo.clients.at(i)->getNickname()){
				std::cerr << RED << serverInfo.clients.at(i)->getNickname() << " nickname duplicated" << RESET << std::endl;
				return ;
			}
		}
		client.setNickname(client.getInput().arguments.at(0));
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
	// * parse the content
	// * set the username

	std::vector<std::string> arguments = client.getInput().arguments;
	for (size_t i = 0; i < arguments.size() - 1; ++i){
		if (arguments.at(i).at(0) == ':')
			return ;
	}

	if (arguments.size() < 4){
		std::cerr << RED << client.getNickname() << " need more parameters" RESET << std::endl;
		return ;
	}

	if (!client.getStatus().authenticated){
		std::cerr << RED << client.getNickname() << " not authenticated" RESET << std::endl;
		return ;
	}
	else if (client.getStatus().registered){ std::cerr << RED << client.getNickname() << " already registered" RESET << std::endl;
		return ;
	}
	else if (client.getInput().arguments.empty() || client.getInput().arguments[0].empty()){
		std::string	  tmp = "unknown";
		client.setUsername(tmp);
		client.setStatus("user", true);
		std::cout << GREEN << client.getNickname() << " username accepted" << RESET << std::endl;
	}
	else if (!client.getInput().arguments.empty())
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
#include <vector>
void	Commands::privmsgCommand( Client& client, struct ServerInfo& serverInfo ){
	//TODO:
	// √ check if not REGISTERED
	// * check msg destination
	// * check text msg validity

	if (!client.getStatus().registered){
		std::cerr << RED << client.getNickname() << " not registered" RESET << std::endl;
		return ;
	}

	s_prvMsgCommand			  privmsgInput;

	if (!privmsgAnalyser( client.getInput().arguments, privmsgInput)){
		std::cout << RED << "failed" << RESET << std::endl;
		return ;
	}
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

					/* ~~~message to channel~~~ */
			if (isValidChannelName(privmsgInput.targets.top())){
				std::cout << YELLOW << "~~~message to channel~~~" << RESET << std::endl;
				for (size_t i = 0; i < serverInfo.channels.size(); ++i){
					if (serverInfo.channels.at(i)->getChannelName() == privmsgInput.targets.top()){
						std::cout << GREEN << "√ Channel exists √" << RESET << std::endl;
						if (!messageToClient(*serverInfo.clients.at(i), client, privmsgInput.message)){
							std::cout << RED << "No such channel" << RESET << std::endl;
						}
					}
				}
			}

					/* ~~~message to client~~~ */
			else {
				std::cout << YELLOW << "~~~message to clients~~~" << RESET << std::endl;
				for (size_t i = 0; i < serverInfo.clients.size(); ++i){
					if (serverInfo.clients.at(i)->getNickname() == privmsgInput.targets.top()){
						std::cout << GREEN << "√ Client exists √" << RESET << std::endl;
						if (!messageToClient(*serverInfo.clients.at(i), client, privmsgInput.message)){
							std::cout << RED << "No such nick" << RESET << std::endl;
						}
					}
				}
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
