#include "../../includes/Helpers.hpp"

//::::::::::::::::::Channels:::::::::::::::::::::::::
bool	isValidChannelName( std::string& channelName ){
	std::cout << CYAN << channelName << RESET << std::endl;
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
