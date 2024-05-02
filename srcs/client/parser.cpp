#include "../../includes/Client.hpp"
#include "../../includes/macros.hpp"
#include <string>
#include <map>

//TODO:
// !!! check for tabs also in find_first_not_of() !!!

bool	commandParser( std::string& input ){
	size_t		position = 0;
	size_t		prefixEnd;
	size_t		commandEnd;
	size_t		argumentEnd;
	bool		args = false;

	Client		test;


	//std::map<std::string, > map containing the command and pointer to its functionj
	
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
		test.clientCommand.prefix = input.substr(position, prefixEnd - position);
		position = input.find_first_not_of(' ', prefixEnd);
	}

	// Parsing the actuall command
	commandEnd = input.find(' ', position);
	if (commandEnd == std::string::npos)
		test.clientCommand.command  = input.substr(position, commandEnd);
	else{
		args = true;
		test.clientCommand.command = input.substr(position, commandEnd - position);
	}

	/*
	---> commandsMapIterator iterator for the command map <---
	std::map<std::string, >::iterator 
	
	commandsMapIterator = commandsMap.find(command);

	if (commandsMapIterator == commandsMap.end()){
		---> the command does not exist <---
		return (false);
	}
	
	TODO:
	---> save the command <---
	*/

	position = input.find_first_not_of(' ', commandEnd);

	// Parsing the command's arguments if found
	if (args){
		argumentEnd = input.find(' ', position);
		if (argumentEnd == std::string::npos)
			test.clientCommand.arguments.push(input.substr(position, argumentEnd));
		else{
			while (argumentEnd != std::string::npos){
				test.clientCommand.arguments.push(input.substr(position, argumentEnd - position));
				position = input.find_first_not_of(' ', argumentEnd);
				if (position == std::string::npos)
					break ;
				argumentEnd = input.find(' ', position);
			}
			if (argumentEnd == std::string::npos)
				test.clientCommand.arguments.push(input.substr(position, argumentEnd));
		}
	}

	// Command execution
	/*
	if (execute)
		return (true);
	*/

	
	// /*				~~~ TESTING ~~~
	
	// std::cout << "prefix---> " << test.clientCommand.prefix << std::endl;
	// std::cout << "command---> " << test.clientCommand.command << std::endl;
	// std::cout << "arg1---> " << test.clientCommand.arguments.top()<< std::endl;
	// test.clientCommand.arguments.pop();
	// std::cout << "arg2---> " << test.clientCommand.arguments.top()<< std::endl;

	// */
	return (true);
}
