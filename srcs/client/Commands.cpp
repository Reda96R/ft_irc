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
	// TODO: WTF is this? Don't worry about it bro, ma lak dakhel. 
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
		// Check if the channel is password protected
		if (it->second->isChannelPasswordProtected()){
			// Check if there are enough arguments
			if (client.getInput().arguments.size() < 2){
				// s_ircReply	  replyInfo = {1, ERR_BADCHANNELKEY, client.getNickname(), channelName, errorMessages.at(replyInfo.errorCode) };
				// messageToClient(client, replyGenerator(replyInfo));
				std::string message = "KTB LPASSWORD A MONSIEUR";
				messageToClient(client, message);
				return ;
			}
			// Check if the password is correct
			if (!it->second->comparePassword(client.getInput().arguments[1])){
				// s_ircReply	  replyInfo = {1, ERR_BADCHANNELKEY, client.getNickname(), channelName, errorMessages.at(replyInfo.errorCode) };
				// messageToClient(client, replyGenerator(replyInfo));
				std::string message = "ATA SIR FI7ALATK";
				messageToClient(client, message);
				return ;
			}
		}


		// Check if the channel is full
		if (it->second->isChannelUserLimitOnOff()) {
			if (it->second->getUserLimit() < it->second->getUserCount() + 1){
				// s_ircReply	  replyInfo = {1, ERR_CHANNELISFULL, client.getNickname(), channelName, errorMessages.at(replyInfo.errorCode) };
				// messageToClient(client, replyGenerator(replyInfo));
				std::string message = "CHANNEL IS FULL  BRO";
				messageToClient(client, message);
				return ;
			}
		}

		// Check if the channel is invite only
		if ((it)->second->isChannelInviteOnly()){
			if (!(it)->second->isClientInvited(client)){
				// s_ircReply	  replyInfo = {1, ERR_INVITEONLYCHAN, client.getNickname(), channelName, errorMessages.at(replyInfo.errorCode) };
				// messageToClient(client, replyGenerator(replyInfo));
				std::string message = "U WERENT INVITED  BRO";
				messageToClient(client, message);
				return ;
			}
		}

		// Check if client is part of the channel
		std::map<std::string, Channel*> channels = client.getChannels();
		if (channels.find(channelName) == channels.end()){
			// Add client to channel
			it->second->addClient(client);
			client.channelAdd(*it->second);
			// Remove client from invited list
			it->second->removeInvitedUser(client);

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
					// Channel exists
					Channel* channel = it->second;
					// Check if the client is a member of the channel
					if (serverInfo.channels[privmsgInput.targets.top()]->isClientInChannel(client)){
						if (!messageToChannel(*channel, client, privmsgInput.message)){
							// Display send() error if messageToChannel fails
						}
					} else {
						// Client is not a member of the channel
						s_ircReply replyInfo = {1, ERR_CANNOTSENDTOCHAN, client.getNickname(), privmsgInput.targets.top(), "You are not a member of the channel."};
						messageToClient(client, replyGenerator(replyInfo));
					}
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
		it->second->removeClient(client, serverInfo);
		client.channelRemove(it->first);
	}
}

			/* ~~~channel commands ~~~ */
void	Commands::kickChannelCommand( Client& client, struct ServerInfo& serverInfo ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
	if (client.getInput().arguments.size() != 2 && client.getInput().arguments.size() != 3) {	
		messageToClient(client, "INVALID, USAGE: KICK <channel> <user> [reason]");
		return ;
	}	
	std::string channelName = client.getInput().arguments.at(0);
	std::string userName = client.getInput().arguments.at(1);
	if (client.getInput().arguments.size() == 3)
		std::string reasonKick = client.getInput().arguments.at(2);
	
	// Check if channelName is Valid
	if (client.amIInChannel(channelName) == false){
		messageToClient(client, "You are not a member of any channel with the specified name.\n");
		return ;
	}

	serverInfo.channels[channelName]->kickUser(client, serverInfo, userName, "");
}

void	Commands::inviteChannelCommand( Client& client, struct ServerInfo& serverInfo ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
	if (client.getInput().arguments.size() != 2){
		messageToClient(client, "INVALID, USAGE: INVITE <user> <channel>");
		return ;
	}
	std::string userName = client.getInput().arguments.at(0);
	std::string channelName = client.getInput().arguments.at(1);

	// check if the channel exists
	if (client.amIInChannel(channelName) == false){
		messageToClient(client, "You are not a member of any channel with the specified name.\n");
		return ;
	}

	// check if there is a user with the specified name (vector of clients)
	for (std::vector<Client*>::iterator it = serverInfo.clients.begin(); it != serverInfo.clients.end(); ++it){
		if ((*it)->getNickname() == userName){
			// check if the user is already in the channel
			if (serverInfo.channels[channelName]->isClientInChannel(userName)){
				messageToClient(client, "The user is already in the channel.\n");
				return ;
			}
			// check if the user is invited
			if (serverInfo.channels[channelName]->isClientInvited(*(*it))){
				messageToClient(client, "The user is already invited.\n");
				return ;
			}
			// invite the user
			serverInfo.channels[channelName]->inviteUser(client, serverInfo, userName);
			return ;
		}
	}

}

void	Commands::topicChannelCommand( Client& client, struct ServerInfo& serverInfo ) {
	(void)serverInfo;
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}
	// Change the topic of the channel
	if (client.getInput().arguments.size() > 2 || client.getInput().arguments.size() == 0){
		messageToClient(client, "INVALID, USAGE: TOPIC <channel> <topic>");
		return ;
	}

	std::string channelName = client.getInput().arguments.at(0);

	// Check if the channel exists [map] and that the second argument is not empty
	if (client.amIInChannel(channelName) == false){
		messageToClient(client, "You are not a member of any channel with the specified name.\n");
		return ;
	}

	if (client.getInput().arguments.size() == 1) {
 		std::string topic = client.getChannels().at(channelName)->getChannelTopic();
		if (topic.empty())
			topic = "No topic is set";
		else 
			topic = "Topic:" + topic;
		messageToClient(client, topic);
		return ;
	}

	std::string topic = client.getInput().arguments.at(1);

	client.getChannels()[channelName]->setChannelTopic(client, topic);
}

// 3toi lol

int ft_atoi(const std::string& str){
	int result = 0;
	for (size_t i = 0; i < str.size(); ++i){
		if (str[i] < '0' || str[i] > '9')
			return -1;
		result = result * 10 + str[i] - '0';
	}
	return result;
}

// MODE USE CASES:
// 1. INVITE: MODE #channel +i
// 2. TOPIC: MODE #channel +t
// 3. PASSWORD: MODE #channel +k password
// 4. USER LIMIT: MODE #channel +l limit
// 5. OPERATOR: MODE #channel +o user

void	Commands::modeChannelCommand( Client& client, struct ServerInfo& ){
	if (trailingCheck(client.getInput().arguments) || !client.getInput().prefix.empty())
		return ;
	if (!client.getStatus().registered){
		s_ircReply	  replyInfo = {1, ERR_NOTREGISTERED, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}

	if (client.getInput().arguments.size() < 2){
		messageToClient(client, "INVALID, USAGE: MODE <channel> <mode>");
		return ;
	}

	std::string channelName = client.getInput().arguments.at(0);
	std::string mode = client.getInput().arguments.at(1);

	// Check if the channel exists
	if (client.amIInChannel(channelName) == false){
		messageToClient(client, "You are not a member of any channel with the specified name.\n");
		return ;
	}

	// Check if the user is an operator
	if (!client.getChannels()[channelName]->isClientOperator(client)){
		messageToClient(client, "You are not an operator of the channel.\n");
		return ;
	}

	// Check if the mode is valid
	if (mode.length() < 2 || (mode[0] != '+' && mode[0] != '-') || (mode[1] != 'i' && mode[1] != 't' && mode[1] != 'k' && mode[1] != 'l' && mode[1] != 'o')){
		messageToClient(client, "INVALID, USAGE: MODE <channel> <mode : +-[i/t/k/l/o]>");
		return ;
	}

	// Check if the mode is invite only
	if (mode[1] == 'i'){
		if (mode[0] == '+')
			client.getChannels()[channelName]->setChannelInviteOnly(client);
		else
			client.getChannels()[channelName]->removeChannelInviteOnly(client);
	}

	// Check if the mode is topic protected
	if (mode[1] == 't'){
		if (mode[0] == '+')
			client.getChannels()[channelName]->setTopicProtected();
		else
			client.getChannels()[channelName]->removeTopicProtected();
	}

	// Check if the mode is password protected
	if (mode[1] == 'k'){
		if (mode[0] == '+'){
			if (client.getInput().arguments.size() < 3){
				messageToClient(client, "INVALID, USAGE: MODE <channel> +k <password>");
				return ;
			}
			client.getChannels()[channelName]->setChannelPassword(client, client.getInput().arguments.at(2));
		}
		else
			client.getChannels()[channelName]->removeChannelPassword(client);
	}

	// Check if the mode is user limit
	if (mode[1] == 'l'){
		if (mode[0] == '+'){
			if (client.getInput().arguments.size() < 3){
				messageToClient(client, "INVALID, USAGE: MODE <channel> +l <limit>");
				return ;
			}
			if (ft_atoi(client.getInput().arguments.at(2)) == -1){
				messageToClient(client, "INVALID, USAGE: MODE <channel> +l <limit: nmra>");
				return ;
			}
			// check if the user limit is valid if its less than the number of users in the channel
			if (client.getChannels()[channelName]->getUserCount() > (ssize_t)ft_atoi(client.getInput().arguments.at(2))){
				messageToClient(client, "INVALID, USAGE: MODE <channel> +l <limit: Greater than the number of users in the channel>");
				return ;
			}

			client.getChannels()[channelName]->setChannelUserLimit(client, (ssize_t)ft_atoi(client.getInput().arguments.at(2)));
		}
		else
			client.getChannels()[channelName]->removeChannelUserLimit(client);
	}

	// Check if the mode is operator
	if (mode[1] == 'o'){
		if (mode[0] == '+'){
			if (client.getInput().arguments.size() < 3){
				messageToClient(client, "INVALID, USAGE: MODE <channel> +o <user>");
				return ;
			}
			std::string userName = client.getInput().arguments.at(2);
			// check if the user exists in the channel
			if (client.getChannels()[channelName]->isClientInChannel(userName)){
				// check if the user is already an operator
				if (client.getChannels()[channelName]->isClientOperator(userName)){
					return ;
				}
				// make the user an operator
				client.getChannels()[channelName]->makeOperator(client, userName);
			}
			else
				messageToClient(client, "The user is not a member of the channel.\n");
		}
		else{
			if (client.getInput().arguments.size() < 3){
				messageToClient(client, "INVALID, USAGE: MODE <channel> -o <user>");
				return ;
			}
			std::string userName = client.getInput().arguments.at(2);
			// check if the user exists in the channel
			if (client.getChannels()[channelName]->isClientInChannel(userName)){
				// check if the user is an operator
				if (!client.getChannels()[channelName]->isClientOperator(userName)){
					return ;
				}
				// remove the user from the operator list
				client.getChannels()[channelName]->removeOperator(client, userName);
			}
			else
				messageToClient(client, "The user is not a member of the channel.\n");
		}
	}




}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Commands::~Commands( void ){
}
