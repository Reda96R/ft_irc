#include "../../includes/Client.hpp"
#include "../../includes/macros.hpp"
#include "../../includes/Commands.hpp"
#include <string>

//TODO:
/* !!! check for tabs also in find_first_not_of() !!! */


bool	commandParser( std::string& input, struct s_ircCommand& clientInput ){
	size_t			position = 0;

	size_t			prefixEnd;
	size_t			commandEnd;
	size_t			argumentEnd;
	bool			args = false;
	
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
		clientInput.prefix = input.substr(position, prefixEnd - position);
		position = input.find_first_not_of(' ', prefixEnd);
	}

	// Parsing the actuall command
	commandEnd = input.find(' ', position);
	if (commandEnd == std::string::npos)
		clientInput.command  = input.substr(position, commandEnd);
	else{
		args = true;
		clientInput.command = input.substr(position, commandEnd - position);
	}

	t_commandsMap::iterator it = commandsMap.find(clientInput.command);
	if (it != commandsMap.end()){
		std::cout << "Command exists" << std::endl;
	}
	else{
		std::cout << "Command doesn't exists" << std::endl;
		return (false);
	}
	position = input.find_first_not_of(' ', commandEnd);

	// Parsing the command's arguments if found
	if (args){
		argumentEnd = input.find(' ', position);
		if (argumentEnd == std::string::npos)
			clientInput.arguments.push(input.substr(position, argumentEnd));
		else{
			while (argumentEnd != std::string::npos){
				clientInput.arguments.push(input.substr(position, argumentEnd - position));
				position = input.find_first_not_of(' ', argumentEnd);
				if (position == std::string::npos)
					break ;
				argumentEnd = input.find(' ', position);
			}
			if (argumentEnd == std::string::npos)
				clientInput.arguments.push(input.substr(position, argumentEnd));
		}
	}
	// check if there's more left

	// Command execution
	/*
	if (execute)
		return (true);
	*/

	return (true);
}
