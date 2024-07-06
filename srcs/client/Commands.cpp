# include "../../includes/Commands.hpp"
# include "../../includes/Server.hpp"
# include "../../includes/IrcErrors.hpp"

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
		messageToClient(client, replyGenerator(ERR_ALREADYREGISTRED, client.getNickname()));

	else if (client.getInput().arguments.empty())
		messageToClient(client, replyGenerator(ERR_NEEDMOREPARAMS, client.getNickname(), "PASS"));

	else if (client.getInput().arguments.size() < 2 && client.getInput().arguments.at(0) == serverInfo.password){
		client.setStatus("authenticated", true);
		client.setStatus("pass", true);
		messageToClient(client, "Password accepted");
	}

	else
		messageToClient(client, replyGenerator(ERR_PASSWDMISMATCH, client.getNickname()));
}

void	Commands::nickCommand( Client& client, struct ServerInfo& serverInfo ){
	if (trailingCheck(client.getInput().arguments))
		return ;

	if (!client.getStatus().authenticated){
		messageToClient(client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
		return ;
	}
	else if (client.getStatus().registered){
		messageToClient(client, replyGenerator(ERR_ALREADYREGISTRED, client.getNickname()));
		return ;
	}
	else if (client.getInput().arguments.empty() || client.getInput().arguments.at(0).empty()){
		messageToClient(client, replyGenerator(ERR_NEEDMOREPARAMS, client.getNickname(), "NICK"));
		return ;
	}
	else if (client.getInput().arguments.size() < 2)
	{
		for (size_t i = 0; i < serverInfo.clients.size(); ++i){
			if (client.getInput().arguments.at(0) == serverInfo.clients.at(i)->getNickname()){
				messageToClient(client, replyGenerator(ERR_NICKNAMEINUSE, client.getNickname()));
				return ;
			}
		}
		if (client.getInput().arguments.at(0).length() > 9)
			return ;
		client.setNickname(client.getInput().arguments.at(0));
		client.setStatus("nick", true);
		if (client.getStatus().user){
			client.setStatus("registered", true);
		}
		messageToClient(client, "Nickname accepted");
		if (client.getStatus().registered)
			messageToClient(client, replyGenerator(RPL_WELCOME, client.getNickname()));
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
		messageToClient(client, replyGenerator(ERR_NEEDMOREPARAMS, client.getNickname(), "USER"));
		return ;
	}

	if (!client.getStatus().authenticated){
		messageToClient(client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
		return ;
	}
	else if (client.getStatus().registered){
		messageToClient(client, replyGenerator(ERR_ALREADYREGISTRED, client.getNickname()));
		return ;
	}
	else if (client.getInput().arguments.empty() || client.getInput().arguments[0].empty()){
		std::string	  tmp = "unknown";
		client.setUsername(tmp);
		client.setStatus("user", true);
		messageToClient(client, "User accepted");
	}
	else if (!client.getInput().arguments.empty())
	{
		client.setUsername(client.getInput().arguments[0]);
		client.setStatus("user", true);
		if (client.getStatus().nick){
			client.setStatus("registered", true);
		}
		messageToClient(client, "User accepted");
		if (client.getStatus().registered){
			messageToClient(client, replyGenerator(RPL_WELCOME, client.getNickname()));
			// std::cout << CYAN << "IP--> " << client.getIpAddress() << RESET << std::endl;
		}
	}
}

// :::::::::::::::::::::::::::::::: JOIN COMMAND AND DEPENDECY FUNCTIONS ::::::::::::::::::::::::::::::::
//NOTE: there's isValidChannelName() in helpers.cpp
// bool ft_checkIfChannelNameIsValid(std::string channelName){
// 	if (channelName[0] != '#')
// 		return false;
// 	for (size_t i = 1; i < channelName.size(); i++){
// 		if (!isalnum(channelName[i]) && channelName[i] != '_')
// 			return false;
// 	}
// 	return true;
// }


void	Commands::joinCommand( Client& client, struct ServerInfo& serverInfo){
	// Check if the client is registered
	if (!client.getStatus().registered){
		messageToClient(client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
		return ;
	}
	
	// Check if the trailing is empty
	// TODO: WTF is this?
	if (trailingCheck(client.getInput().arguments)){
		std::cerr << RED << "Trailing is empty" << RESET << std::endl;
		// messageToClient(client, client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
		return ;
	}

	// Check if the channel name is valid
	std::string channelName = client.getInput().arguments[0];
	if (!isValidChannelName(channelName)){
		messageToClient(client, replyGenerator(ERR_NOSUCHCHANNEL, client.getNickname(), channelName));
		return ;
	}

	// Check if the channel already exists
	for (std::vector<Channel*>::iterator it = serverInfo.channels.begin(); it < serverInfo.channels.end(); it++){
		if ((*it)->getChannelName() == channelName){


			if (std::find(client.channels.begin(), client.channels.end(), (*it)->getChannelName()) == client.channels.end()){
			// if (std::find((*it)->getChannelClients().begin(), (*it)->getChannelClients().end(), client) == (*it)->getChannelClients().end()){
				(*it)->addClient(client);
				client.channels.push_back((*it)->getChannelName());
			}
			return ;
		}
	}

	// Create the channel
	Channel *channel = new Channel(channelName);
	serverInfo.channels.push_back(channel);
		// std::cout << GREEN << "Channel created" << RESET << std::endl;
	// Add the client to the channel
	channel->addClient(client);
	client.channels.push_back(channelName);
		// std::cout << GREEN << "Client added to the channel" << RESET << std::endl;
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void	Commands::privmsgCommand( Client& client, struct ServerInfo& serverInfo ){
	//TODO:
	// √ check if not REGISTERED
	// √ check msg destination
	// * check text msg validity

	if (!client.getStatus().registered){
		messageToClient(client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
	}

	s_prvMsgCommand			  privmsgInput;

	if (!privmsgAnalyser(client.getInput().arguments, privmsgInput, client))
		return ;
	else{
		//TODO:
		//	√ check if the target is user or a channel:
		//	  √ channel:
		//		√ check if the user is a member of the channel (client class will contain
		//		data structure that contains the names of the channels that the client is part of)
		//		√ check if the channel exists (to send nosuchchannel or cannotsendtochan)
		//	  √ user:
		//		√ search for the user if found sent the message if not return nosuchnick
		//

		while (!privmsgInput.targets.empty()){

			/* ~~~message to channel~~~ */
			if (isValidChannelName(privmsgInput.targets.top())){
				bool	n = false;
				for (size_t i = 0; i < serverInfo.channels.size(); ++i){
					if (serverInfo.channels.at(i)->getChannelName() == privmsgInput.targets.top()){
						//√ Channel exists √
						if (messageToChannel(*serverInfo.channels.at(i), client, privmsgInput.message)){
							privmsgInput.targets.pop();
							n = true;
							break ;
						}
						// else
							//display send() error if (!messageToChannel)
					}
				}
				//X Channel doesn't exist X
				if (!n)
					messageToClient(client, replyGenerator(ERR_NOSUCHCHANNEL, client.getNickname(), privmsgInput.targets.top()));
			}

			/* ~~~message to client~~~ */
			else {
				bool	n = false;
				for (size_t i = 0; i < serverInfo.clients.size(); ++i){
					if (serverInfo.clients.at(i)->getNickname() == privmsgInput.targets.top()){
						//√ Client exists √
						// std::cout << YELLOW << client.getIpAddress() <<  RESET << std::endl;
						s_messageInfo messageInfo = {&client, serverInfo.clients.at(i),
									  privmsgInput.message};
						if (messageToClient(messageInfo)){
							privmsgInput.targets.pop();
							n = true;
							break ;
						}
						// else
							//display send() error if (!messageToClient)
					}
				}
				//X Client doesn't exist X
				if (!n)
					messageToClient(client, replyGenerator(ERR_NOSUCHNICK, client.getNickname(), privmsgInput.targets.top()));
			}
			if (!privmsgInput.targets.empty())
				privmsgInput.targets.pop();
		}
	}
}



			/* ~~~channel commands ~~~ */
void	Commands::kickChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		messageToClient(client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
		return ;
	}

}

void	Commands::inviteChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		messageToClient(client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
		return ;
	}
}

void	Commands::topicChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		messageToClient(client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
		return ;
	}
}

void	Commands::modeChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		messageToClient(client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
		return ;
	}
}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Commands::~Commands( void ){
}
