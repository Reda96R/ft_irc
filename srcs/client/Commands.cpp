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
		for (size_t i = 0; i < serverInfo.clients.size(); ++i){
			if (client.getInput().arguments.at(0) == serverInfo.clients.at(i)->getNickname()){
				s_ircReply	  replyInfo = {1, ERR_NICKNAMEINUSE, serverInfo.clients.at(i)->getNickname(), "", errorMessages.at(replyInfo.errorCode) };
				messageToClient(client, replyGenerator(replyInfo));
				return ;
			}
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
			s_ircReply	  replyInfo = {2, RPL_WELCOME, client.getNickname(), client.getNickname() + "!", errorMessages.at(replyInfo.errorCode) };
			messageToClient(client, replyGenerator(replyInfo));
		}
	}
}

void	Commands::userCommand( Client& client, struct ServerInfo& ){
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
	if (trailingCheck(client.getInput().arguments)){
		std::cerr << RED << "Trailing is empty" << RESET << std::endl;
		// messageToClient(client, client, replyGenerator(ERR_NOTREGISTERED, client.getNickname()));
		return ;
	}

	// Check if the channel name is valid
	std::string channelName = client.getInput().arguments[0];
	if (!isValidChannelName(channelName)){
		s_ircReply	  replyInfo = {1, ERR_NOSUCHCHANNEL, client.getNickname(), channelName, errorMessages.at(replyInfo.errorCode) };
		messageToClient(client, replyGenerator(replyInfo));
		return ;
	}

	// Check if the channel already exists
	for (std::vector<Channel*>::iterator it = serverInfo.channels.begin(); it < serverInfo.channels.end(); it++){
		if ((*it)->getChannelName() == channelName){
			if (std::find(client.channels.begin(), client.channels.end(), (*it)->getChannelName()) == client.channels.end()){
			// if (std::find((*it)->getChannelClients().begin(), (*it)->getChannelClients().end(), client) == (*it)->getChannelClients().end()){
				(*it)->addClient(client);
				client.channels.push_back((*it)->getChannelName());

				std::string message = ":"  + client.getNickname()  +
						  "!~" + client.getUsername()  +
						  "@"  + client.getIpAddress() +
						  " "  + "JOIN" + " " + (*it)->getChannelName();
				for (size_t i = 0; i < (*it)->getChannelClients().size(); ++i)
					messageToClient(*(*it)->getChannelClients().at(i), message);
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
				bool	n = false;
				for (size_t i = 0; i < serverInfo.channels.size(); ++i){
					if (serverInfo.channels.at(i)->getChannelName() == privmsgInput.targets.top()){
						//√ Channel exists √
						if (messageToChannel(*serverInfo.channels.at(i), client, privmsgInput.message)){
							n = true;
							break ;
						}
						// else
							//display send() error if (!messageToChannel)
					}
				}
				//X Channel doesn't exist X
				if (!n){
					s_ircReply	  replyInfo = {1, ERR_NOSUCHCHANNEL, client.getNickname(), privmsgInput.targets.top(), errorMessages.at(replyInfo.errorCode) };
					messageToClient(client, replyGenerator(replyInfo));
				}
			}

			/* ~~~message to client~~~ */
			else {
				bool	n = false;
				for (size_t i = 0; i < serverInfo.clients.size(); ++i){
					if (serverInfo.clients.at(i)->getNickname() == privmsgInput.targets.top()){
						//√ Client exists √
						if (&client == serverInfo.clients.at(i)){
							n = true;
							break ;
						}
						s_messageInfo messageInfo = {serverInfo.clients.at(i)->getNickname(), &client, serverInfo.clients.at(i),
									  privmsgInput.message};
						if (messageToClient(messageInfo)){
							n = true;
							break ;
						}
						// else
							//display send() error if (!messageToClient)
					}
				}
				//X Client doesn't exist X
				if (!n){
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
	std::string message = "ERROR :Closing Link: " + client.getInput().arguments.at(0) + "!\n";
	send(client.getPollFd(), message.c_str(), message.length(), 0);
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
