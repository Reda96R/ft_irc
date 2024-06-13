#include "../../includes/Helpers.hpp"
#include "../../includes/IrcErrors.hpp"

//::::::::::::::::::Channels:::::::::::::::::::::::::
bool	isValidChannelName( std::string& channelName ){
	if (channelName.find_first_of("\a:, ") != std::string::npos || channelName.length() > 200)
		return (false);
	if (!channelName.empty() && channelName.at(0) != '#' && channelName.at(0) != '&')
		return (false);
	return (true);
}

//::::::::::::::::::Commands:::::::::::::::::::::::::
//TODO
//replace errors with their numerical values
bool	privmsgAnalyser( std::vector<std::string> arguments, s_prvMsgCommand& privmsgInput, Client& client ){
	if (arguments.empty() || arguments.size() < 2){
		if (arguments.size() < 2)
			messageToClient(client, client, replyGenerator(ERR_NOTEXTTOSEND, client.getNickname()));
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

bool	messageToClient( Client& target, Client& sender, std::string message){
	(void) sender;
	if (message.at(message.size() - 1) != '\n')
		message += '\n';

	if (send(target.getPollFd(), message.c_str(), message.length(), 0) == -1)
		return (false);
	return (true);
}

bool	messageToChannel( Channel& target, Client& sender, std::string message){
	//Checking if the user is part of the channel
	std::vector<std::string>::iterator it = std::find(sender.channels.begin(), sender.channels.end(), target.getChannelName());
	if (it == sender.channels.end()){
		messageToClient(sender, sender, replyGenerator(ERR_NOTONCHANNEL, sender.getNickname(), target.getChannelName()));
		return (false);
	}

	if (message.at(message.size() - 1) != '\n')
		message += '\n';

	//Broadcasting the message
	for (size_t i = 0; i < target.getChannelClients().size() ; ++i){
		if (target.getChannelClients()[i].getNickname() != sender.getNickname()){
			if (!messageToClient(target.getChannelClients()[i], target.getChannelClients()[i], message)){
			// if (send(target.getChannelClients()[i].getPollFd(), message.c_str(), message.length(), 0) == -1){
				std::cout << RED << "send failure" << RESET << std::endl;
				return (false);
			}
		}
	}
	return (true);
}
bool	trailingCheck( std::vector<std::string> arguments ){
	for (size_t i = 0; i < arguments.size(); ++i){
		if (arguments[i].at(0) == ':')
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

std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}


