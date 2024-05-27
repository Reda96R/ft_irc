#include "../../includes/Helpers.hpp"

//::::::::::::::::::Channels:::::::::::::::::::::::::
bool	isValidChannelName( std::string& channelName ){
	if (channelName.find_first_of("\a:, ") != std::string::npos || channelName.length() > 200)
		return (false);
	if (!channelName.empty() && channelName.at(0) != '#' && channelName.at(0) != '&')
		return (false);
	return (true);
}

//::::::::::::::::::Commands:::::::::::::::::::::::::
bool	privmsgAnalyser( std::vector<std::string> arguments, s_prvMsgCommand& privmsgInput){
	if (arguments.empty() || arguments.size() < 2){
		if (arguments.front().at(0) == ':')
			std::cout << RED << "No Target" << RESET << std::endl;
		else
			std::cout << RED << "Nothing to be sent" << RESET << std::endl;
		return false;
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
	send(target.getPollFd(), message.c_str(), message.length(), 0);
	return (true);
}

bool	messageToChannel( Channel& target, Client& sender, std::string message){
	(void) sender;
	if (message.at(message.size() - 1) != '\n')
		message += '\n';
	for (size_t i = 0; i < target.getChannelClients().size() ; ++i){
		send(target.getChannelClients()[i].getPollFd(), message.c_str(), message.length(), 0);
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

