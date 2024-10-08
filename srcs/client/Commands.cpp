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
	this->commandsMap["PING"] = &Commands::pingCommand;
	this->commandsMap["QUIT"] = &Commands::quitCommand;

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

	if (client.getStatus().authenticated){
		s_ircReply	  replyInfo = {1, ERR_ALREADYREGISTRED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
	}

	else if (client.getInput().arguments.empty()){
		s_ircReply	  replyInfo = {1, ERR_NEEDMOREPARAMS, client.getNickname(), "PASS", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
	}

	else if (client.getInput().arguments.size() < 2 && client.getInput().arguments.at(0) == serverInfo.password){
		client.setStatus("authenticated", true);
		client.setStatus("pass", true);
	}

	else{
		s_ircReply	  replyInfo = {1, ERR_PASSWDMISMATCH, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
	}
}

void	Commands::nickCommand( Client& client, struct ServerInfo& serverInfo ){
	if (trailingCheck(client.getInput().arguments))
		return ;

	if (!client.getStatus().authenticated){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
	else if (client.getInput().arguments.empty() || client.getInput().arguments.at(0).empty()){
		s_ircReply	  replyInfo = {1, ERR_NEEDMOREPARAMS, client.getNickname(), "NICK", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}

	else if (client.getInput().arguments.size() < 2)
	{
		std::map<std::string, Client*>::iterator it;
		it = serverInfo.clientsMap.find(client.getInput().arguments.at(0));

		if (it != serverInfo.clientsMap.end()){
			s_ircReply	  replyInfo = {1, ERR_NICKNAMEINUSE, it->second->getNickname(), "", errorMessages.at(replyInfo.errorCode) };
			messageToClient(client, replyGenerator(replyInfo));
			return ;
		}

		if (client.getInput().arguments.at(0).length() > 9)
			return ;

		//Changing the Nickname
		if (client.getStatus().registered){
			std::string oldnick = client.getNickname();
			client.setNickname(client.getInput().arguments.at(0));
			std::string	  message = oldnick + " Changed his nickname to " + client.getNickname();
			for (size_t i = 0; i < serverInfo.clients.size(); ++i){
				if (serverInfo.clients[i]->getNickname() != client.getNickname()){
					s_messageInfo messageInfo = {serverInfo.clients.at(i)->getNickname(), &client, serverInfo.clients.at(i), message};
					messageToClient(messageInfo);
				}
			}
			return ;
		}
		client.setNickname(client.getInput().arguments.at(0));
		client.setStatus("nick", true);
		if (client.getStatus().user){
			client.setStatus("registered", true);
		}
		if (client.getStatus().registered){
			client.setStatus("connected", true);
			serverInfo.clientsMap[client.getNickname()] = &client;
			s_ircReply	  replyInfo = {2, RPL_WELCOME, client.getNickname(), client.getNickname() + "!", errorMessages.at(replyInfo.errorCode) };
			messageToClient(client, replyGenerator(replyInfo));
		}
	}
}

void	Commands::userCommand( Client& client, struct ServerInfo& serverInfo ){
	//TODO:
	// √ check if authenticated
	// √ check if already registered
	// √ check if command's parameters exist if not set username to unknown
	// √ parse the content
	// √ set the username
	// √ handle the server and host names

	if (!client.getStatus().authenticated || client.getStatus().registered){
		if (!client.getStatus().authenticated){
			s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
			messageToClient(client, replyGenerator(replyInfo));
		}
		else{
			s_ircReply	  replyInfo = {1, ERR_ALREADYREGISTRED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
			messageToClient(client, replyGenerator(replyInfo));
		}
		return ;
	}

	std::vector<std::string> arguments = client.getInput().arguments;

	if (arguments.size() != 4){
		if (arguments.empty() || arguments.size() < 4){
			s_ircReply	  replyInfo = {1, ERR_NEEDMOREPARAMS, client.getNickname(), "USER", errorMessages.at(replyInfo.errorCode) };
			messageToClient(client, replyGenerator(replyInfo));
		}
		return ;
	}

	for (size_t i = 0; i < arguments.size() - 1; ++i){
		if (arguments.at(i).at(0) == ':')
			return ;
	}

	if (!arguments.empty())
	{
		client.setUsername(arguments[0]);
		if (arguments.at(3).at(0) == ':'){
			arguments.at(3) = arguments.at(3).substr(1, arguments.at(3).size() - 1);
		}
		client.setRealname(arguments.at(3));
		client.setStatus("user", true);
		if (client.getStatus().nick){
			client.setStatus("registered", true);
		}
		if (client.getStatus().registered){
			client.setStatus("connected", true);
			serverInfo.clientsMap[client.getNickname()] = &client;
			s_ircReply	  replyInfo = {2, RPL_WELCOME, client.getNickname(), client.getNickname() + "!", errorMessages.at(replyInfo.errorCode) };
			messageToClient(client, replyGenerator(replyInfo));
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
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
	
	// Check if the trailing is empty
	// TODO: WTF is this?
	// if (trailingCheck(client.getInput().arguments)){
	// 	std::cerr << RED << "Trailing is empty" << RESET << std::endl;
		// messageToClient(client, client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
	// 	return ;
	// }

	// Check if the channel name is valid
	std::string channelName = client.getInput().arguments[0];
	if (!isValidChannelName(channelName)){
		s_ircReply	  replyInfo = {1, ERR_NOSUCHCHANNEL, client.getNickname(), channelName, errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}

	// Check if the channel already exists
	std::map<std::string, Channel*>::iterator	it = serverInfo.channels.find(channelName);
	if (it != serverInfo.channels.end()){
		// Check if client is part of the channel
		std::map<std::string, Channel*> channels = client.getChannels();
		if (channels.find(channelName) == channels.end()){
			// Add client to channel
			it->second->addClient(client);
			client.channelAdd(*it->second);

			std::string	  message = ":"  + client.getNickname()  +
					  "!~" + client.getUsername()  +
					  "@"  + client.getIpAddress() +
					  " "  + "JOIN" + " " + it->second->getChannelName() + "\n";
			for (size_t i = 0; i < it->second->getChannelOperators().size(); ++i)
				messageToClient(*(it->second)->getChannelOperators().at(i), message);
			for (size_t i = 0; i < it->second->getChannelClients().size(); ++i)
				messageToClient(*(it->second)->getChannelClients().at(i), message);

			if (it->second->getChannelTopic().empty()){
				s_ircReply	  replyInfo = {3, RPL_NOTOPIC, client.getNickname(), it->second->getChannelName() , ":" + errorMessages.at(replyInfo.errorCode) };
				messageToClient(client, replyGenerator(replyInfo));
			}
			else{
				s_ircReply	  replyInfo = {3, RPL_TOPIC, client.getNickname(), it->second->getChannelName() , ":" + it->second->getChannelTopic() };
				messageToClient(client, replyGenerator(replyInfo));
			}

			message = ":" + client.getNickname()  +
			"!~" + client.getUsername()           +
			"@"  + serverInfo.servIpAddress       +
			" 353 "  + client.getNickname()       +
			" = " + it->second->getChannelName()       +
			" :" + it->second->getChannelClientsList() +
			"\n";
			messageToClient(client, message);

			message = ":"  + serverInfo.servIpAddress   +
			" 366 "  + client.getNickname()   +
			"  " + it->second->getChannelName()  +
			" :End of /NAMES list.\n";
			messageToClient(client, message);
		}
		else{
			return ;
		}
	}
	else{
		// Create the channel
		Channel *channel = new Channel(channelName);
		serverInfo.channels[channelName] = channel;
		// serverInfo.channels.push_back(channel);
		//
		// Add the client as operator
		channel->setChannelCreationTime();
		channel->addOperator(client);

		std::string message = ":"  + client.getNickname()  +
				  "!~" + client.getUsername()  +
				  "@"  + client.getIpAddress() +
				  " "  + "JOIN" + " " + channel->getChannelName() + "\n";
		messageToClient(client, message);

		message = ":"  + client.getNickname()       +
				  "!~" + client.getUsername()       +
				  "@"  + serverInfo.servIpAddress   +
				  " 353 "  + client.getNickname()   +
				  " = " + channel->getChannelName() +
				  " :" + "@" + client.getNickname() +
				  "\n";
		messageToClient(client, message);

		message = ":"  + serverInfo.servIpAddress   +
				  " 366 "  + client.getNickname()   +
				  "  " + channel->getChannelName()  +
				  " :End of /NAMES list.\n";
		messageToClient(client, message);

		// std::cout << GREEN << "Channel created" << RESET << std::endl;
		client.channelAdd(*channel);
		// std::cout << GREEN << "Client added to the channel" << RESET << std::endl;
	}
}

// ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void	Commands::privmsgCommand( Client& client, struct ServerInfo& serverInfo ){
	//TODO:
	// √ check if not REGISTERED
	// √ check msg destination
	// √ check text msg validity

	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
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
				std::map<std::string, Channel*>::iterator	it;
				it = serverInfo.channels.find(privmsgInput.targets.top());
				if (it != serverInfo.channels.end()){
					//√ Channel exists √
					if (!messageToChannel(*it->second, client, privmsgInput.message)){
						//display send() error if (!messageToChannel)
					}
				}
				else{
					s_ircReply	  replyInfo = {1, ERR_NOSUCHCHANNEL, client.getNickname(), privmsgInput.targets.top(), errorMessages.at(replyInfo.errorCode) };
					messageToClient(client, replyGenerator(replyInfo));
				}
			}
			/* ~~~message to client~~~ */
			else {
				std::map<std::string, Client*>::iterator	it;
				it = serverInfo.clientsMap.find(privmsgInput.targets.top());
				if (it != serverInfo.clientsMap.end()){
					if (client.getNickname() != it->second->getNickname()){
						s_messageInfo messageInfo = {it->second->getNickname(),
													&client, it->second, privmsgInput.message};
						if (!messageToClient(messageInfo)){
							//display send() error if (!messageToCient)
						}
					}
				}
				//X Client doesn't exist X
				else{
					s_ircReply	  replyInfo = {1, ERR_NOSUCHNICK, client.getNickname(), privmsgInput.targets.top(), errorMessages.at(replyInfo.errorCode) };
					messageToClient(client, replyGenerator(replyInfo));
				}
			}
			if (!privmsgInput.targets.empty())
				privmsgInput.targets.pop();
		}
	}
}

void	Commands::pingCommand( Client& client, struct ServerInfo& serverInfo){
	(void)serverInfo;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
	std::string message = ":ircserv PONG ircserv " + client.getInput().arguments.at(0) + "\n";
	send(client.getPollFd(), message.c_str(), message.length(), 0);
}

void	Commands::quitCommand( Client& client, struct ServerInfo& serverInfo){
	(void)serverInfo;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}

	client.setStatus("connected", false);
	std::map<std::string, Channel*> channels = client.getChannels();
	std::map<std::string, Channel*>::iterator it;
	for (it = channels.begin() ; it != channels.end(); ++it){
		messageToChannel(*it->second, client, "QUIT");
		it->second->removeClient(client);
		client.channelRemove(it->first);
	}
}

			/* ~~~channel commands ~~~ */
void	Commands::kickChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
}

void	Commands::inviteChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
}

void	Commands::topicChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
}

void	Commands::modeChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Commands::~Commands( void ){
}
