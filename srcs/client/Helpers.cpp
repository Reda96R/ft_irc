#include "../../includes/Helpers.hpp"
#include "../../includes/IrcErrors.hpp"
#include "../../includes/StatBot.hpp"

//::::::::::::::::::Channels:::::::::::::::::::::::::
bool	isValidChannelName( std::string& channelName ){
	if (channelName.find_first_of("\a:, ") != std::string::npos || channelName.length() > 200)
		return (false);
	if (!channelName.empty() && channelName.at(0) != '#' && channelName.at(0) != '&')
		return (false);
	return (true);
}

//::::::::::::::::::Commands:::::::::::::::::::::::::

//				  :::Messaging:::
bool	privmsgAnalyser( std::vector<std::string> arguments, s_prvMsgCommand& privmsgInput, Client& client ){
	if (arguments.empty() || arguments.size() < 2){
		if (arguments.size() < 2){
			s_ircReply	  replyInfo = {1, ERR_NOTEXTTOSEND, client.getNickname(), "", errorMessages.at(replyInfo.errorCode) };
			messageToClient(client, replyGenerator(replyInfo));
		}
		return (false);
	}

	//Target parsing
	std::istringstream	iss(arguments.front());
	std::string			target;

	while (getline(iss, target, ','))
		privmsgInput.targets.push(target);
	
	//Message parsing
	privmsgInput.message = arguments.at(1);
	if (privmsgInput.message.at(0) == ':')
		privmsgInput.message = privmsgInput.message.substr(1, privmsgInput.message.size() - 1);
	return (true);
}

//Used to generate replies from server to client
std::string	  replyGenerator( s_ircReply replyInfo ){
	std::string	  reply;

	//sender + target + message
	if (replyInfo.type == 1) {
		reply = ":ircserv " + intToString(replyInfo.errorCode) + " "
							+ replyInfo.sender + " "
							+ replyInfo.target + " "
							+ ":" + replyInfo.message + "\n";
	}
	//sender + message
	else if (replyInfo.type == 2) {
		reply = ":ircserv " + intToString(replyInfo.errorCode) + " " + replyInfo.sender + " "
							+ ":" + replyInfo.message + " "
							+ replyInfo.target + "\n";
	}
	else {
		reply = ":ircserv " + intToString(replyInfo.errorCode) + " "
							+ replyInfo.sender + " "
							+ replyInfo.target + " "
							+ replyInfo.message + "\n";
	}
	return (reply);
}

//Used to send replies from server to client
bool	messageToClient( Client& target, std::string message){
	if (message.at(message.size() - 1) != '\n')
		message += '\n';

	if (send(target.getPollFd(), message.c_str(), message.length(), 0) == -1)
		return (false);
	return (true);
}

//Used to send messages from client to client
bool	messageToClient( s_messageInfo messageInfo ){
	if (!messageInfo.receiver->getStatus().connected)
		return (true);
	if (messageInfo.message.at(messageInfo.message.size() - 1) != '\n')
		messageInfo.message += '\n';

	//Formating the sender in the message
	messageInfo.message = ":"  + messageInfo.sender->getNickname()  +
						  "!~" + messageInfo.sender->getUsername()  +
						  "@"  + messageInfo.sender->getIpAddress() +
						  " "  + "PRIVMSG" + " " + messageInfo.targetName + " " + messageInfo.message;

	if (send(messageInfo.receiver->getPollFd(), messageInfo.message.c_str(), messageInfo.message.length(), 0) == -1)
		return (false);
	return (true);
}

//Used to send messages from client to channel
bool	messageToChannel( Channel& target, Client& sender, std::string message){
	//Checking if the user is part of the channel
	std::map<std::string, Channel*> channels = sender.getChannels();
	if (channels.find(target.getChannelName()) == channels.end()){
		s_ircReply	  replyInfo = {1, ERR_NOSUCHCHANNEL, sender.getNickname(), target.getChannelName(), errorMessages.at(replyInfo.errorCode) };
		messageToClient(sender, replyGenerator(replyInfo));
		return (false);
	}

	if (message.at(message.size() - 1) != '\n')
		message += '\n';


	//Broadcasting the message
	//Operators
	for (size_t i = 0; i < target.getChannelOperators().size() ; ++i){
		if (target.getChannelOperators()[i]->getNickname() != sender.getNickname()){
			s_messageInfo messageInfo = {target.getChannelName(), &sender,
										target.getChannelOperators()[i], message};
			if (!messageToClient(messageInfo)){
				std::cout << RED << "Send failure" << RESET << std::endl;
				return (false);
			}
		}
	}
	//Clients
	for (size_t i = 0; i < target.getChannelClients().size() ; ++i){
		if (target.getChannelClients()[i]->getNickname() != sender.getNickname()){
			s_messageInfo messageInfo = {target.getChannelName(), &sender,
										target.getChannelClients()[i], message};
			if (!messageToClient(messageInfo)){
				std::cout << RED << "Send failure" << RESET << std::endl;
				return (false);
			}
		}
	}

	if (message == "!statbot\n"){
		ircBot(target);
	}

	else{
		std::map<std::string, int>::iterator it =
		target.statBot.messagesCount.find(sender.getNickname());
		if (it != target.statBot.messagesCount.end()){
			it->second++;
		}
		else
			target.statBot.messagesCount[sender.getNickname()] = 1;
	}

	return (true);
}

bool	trailingCheck( std::vector<std::string> arguments ){
	for (size_t i = 0; i < arguments.size(); ++i){
		if (arguments.at(0).at(0) == ':')
			return (true);
	}
	return (false);
}

//::::::::::::::::::General:::::::::::::::::::::::::
void compareStrings(const std::string& str1, const std::string& str2) {
    std::pair<std::string::const_iterator, std::string::const_iterator> result = std::mismatch(str1.begin(), str1.end(), str2.begin());

    if (result.first == str1.end() && result.second == str2.end()) {
        std::cout << "The strings are identical." << std::endl;
    } else {
        std::cout << "The strings differ at position " 
                  << std::distance(str1.begin(), result.first) << std::endl;
        std::cout << "String 1 has: " << *result.first << std::endl;
        std::cout << "String 2 has: " << *result.second << std::endl;
    }
}

//Changes int to std::string while filling the empty spaces (potentially 2) with zeros
std::string intToString(int value) {
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << value;
    return oss.str();
}

