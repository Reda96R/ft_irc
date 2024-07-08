# include "../../includes/Client.hpp"
# include "../../includes/macros.hpp"
# include "../../includes/Commands.hpp"
# include "../../includes/IrcErrors.hpp"
#include <string>

//TODO : 
/* !!! check for tabs also in find_first_not_of() !!! */

bool	commandParser( std::string& input, Client& client, struct ServerInfo& serverInfo){
	size_t			position = 0;

	size_t			prefixEnd;
	size_t			commandEnd;
	size_t			argumentEnd;
	bool			args = false;
	std::string		tmp;
	
	Commands		commands;
	t_commandsMap	commandsMap = commands.getCommandMap();

	if (input.empty()){
		std::cerr << "eof error" << std::endl;
		return (false);
	}
	
	if (!input.empty() && input[input.size() - 1] == '\r')
		input = input.substr(0, input.size() - 1);

	// Parsing prefix if found
	if (!input.empty() && input[position] == ':'){
		position++;
		prefixEnd = input.find(' ', position);
		if (prefixEnd == std::string::npos)
			return (false);
		tmp = input.substr(position, prefixEnd - position);
		client.setInput("prefix", tmp);
		position = input.find_first_not_of(' ', prefixEnd);
		if (position == std::string::npos)
			return (false);
	}

	// Parsing the actuall command
	commandEnd = input.find(' ', position);
	if (commandEnd == std::string::npos){
		tmp = input.substr(position, commandEnd);
		client.setInput("command", tmp);
	}
	else{
		args = true;
		tmp = input.substr(position, commandEnd - position);
		client.setInput("command", tmp);
	}

	t_commandsMap::iterator it = commandsMap.find(client.getInput().command);
	if (it == commandsMap.end()){
		messageToClient(client, replyGenerator(ERR_UNKNOWNCOMMAND, client.getNickname()));
		return (false);
	}
	position = input.find_first_not_of(' ', commandEnd);
	if (position == std::string::npos)
		args = false;

	// Parsing the command's arguments if found
	if (args){
		argumentEnd = input.find(' ', position);
		if (argumentEnd == std::string::npos){
			tmp = input.substr(position, argumentEnd);
			client.setInput("arguments", tmp);
		}
		else{
			//TODO: check for the last arg if it is trailing or not
			while (argumentEnd != std::string::npos){
				tmp = input.substr(position, argumentEnd - position);
				if (tmp.at(0) == ':'){
					tmp = input.substr(position, input.size() - position);
					client.setInput("arguments", tmp);
					break ;
				}
				client.setInput("arguments", tmp);
				position = input.find_first_not_of(' ', argumentEnd);
				if (position == std::string::npos)
					break ;
				argumentEnd = input.find(' ', position);
				//TODO: if arg is == npos then there is a last arg that needs to be parsed
				if (argumentEnd == std::string::npos){
					tmp = input.substr(position , input.size() - position);
					std::cout << "tmp--> "  << tmp << std::endl;
					if (tmp.at(0) == ':'){
						tmp = input.substr(position, input.size() - position);
						if (input.length() == position){
							tmp = input.substr(position + 1, input.size() - position);
						}
					std::cout << GREEN << tmp << RESET << std::endl;
						// tmp = input.substr(position + 1, argumentEnd);
					}
					client.setInput("arguments", tmp);
				}
			}
			//TODO: here we probably need to check for position == npos
		}
	}

	//TODO:
	// check if there's more left
	
	// Command execution
	void (Commands::*cmd)(Client&, struct ServerInfo&) = it->second;
	(commands.*cmd)(client, serverInfo);

	std::cout << CYAN << "command ---> " << client.getInput().command << RESET << std::endl;
	for (size_t i = 0; i < client.getInput().arguments.size(); ++i){
		std::cout << CYAN << "args ---> " << client.getInput().arguments[i] << RESET << std::endl;
	}

	client.clearInput();

	return (true);
}
