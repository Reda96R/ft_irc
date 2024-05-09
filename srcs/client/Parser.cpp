#include "../../includes/Client.hpp"
#include "../../includes/macros.hpp"
#include "../../includes/Commands.hpp"
#include <string>

//TODO : 
/* !!! check for tabs also in find_first_not_of() !!! */

// Try using a class Singlton
bool	commandParser( std::string& input, Client& client ){
	size_t			position = 0;

	size_t			prefixEnd;
	size_t			commandEnd;
	size_t			argumentEnd;
	bool			args = false;
	std::string		tmp;
	
	Commands		commands;
	t_commandsMap	commandsMap = commands.getCommandMap();
	
	// if (tmp.eof()){
	// 	std::cerr << "eof error" << std::endl;
	// 	return (false);
	// }
	
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
	if (it != commandsMap.end()){
		std::cout << GREY << " ̴ ̴ ̴ Command exists  ̴ ̴ ̴" << RESET << std::endl;
	}
	else{
		std::cout << GREY << " ̴ ̴ ̴ Command doesn't exists  ̴ ̴ ̴" << RESET << std::endl;
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
			while (argumentEnd != std::string::npos){
				tmp = input.substr(position, argumentEnd - position);
				client.setInput("arguments", tmp);
				position = input.find_first_not_of(' ', argumentEnd);
				if (position == std::string::npos)
					break ;
				argumentEnd = input.find(' ', position);
			}
			if (argumentEnd == std::string::npos){
				tmp = input.substr(position, argumentEnd);
				client.setInput("arguments", tmp);
			}
		}
	}

	//TODO:
	// check if there's more left

	// Command execution
	void (Commands::*cmd)(Client&) = it->second;
	(commands.*cmd)(client);
	client.clearInput();

	return (true);
}
