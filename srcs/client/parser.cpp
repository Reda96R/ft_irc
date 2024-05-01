#include "../../includes/Client.hpp"
#include "../../includes/macros.hpp"
#include <string>
#include <map>

bool	commandParser( std::string& input ){
	size_t		position = 0;
	size_t		prefixEnd;
	size_t		commandEnd;
	bool		args = false;

	std::string prefix;
	std::string command;
	std::string arguments;

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
		prefix = input.substr(position, prefixEnd - position);
		position = prefixEnd + 1;
	}

	// Parsing the actuall command
	commandEnd = input.find(' ', position);
	if (commandEnd == std::string::npos)
		command  = input.substr(position, commandEnd);
	else{
		args = true;
		command = input.substr(position, commandEnd - position);
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
	---> store the command in the commands stack <---
	
	*/
	// Parsing the command's arguments if found
	if (args){
		position = commandEnd + 1;

		arguments = input.substr(position, input.size() - 1);
	
		//TODO:
		//---> add each arg to a list <---
	}

	// Command execution
	/*
	if (execute)
		return (true);
	*/

	
	/*				~~~ TESTING ~~~
	
	std::cout << "prefix---> " << prefix << std::endl;
	std::cout << "command---> " << command << std::endl;
	std::cout << "args---> " << arguments << std::endl;

	*/
	return (true);
}
