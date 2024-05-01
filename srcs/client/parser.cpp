#include "../../includes/Client.hpp"
#include "../../includes/macros.hpp"
#include <string>
#include <map>

bool	commandParser( std::string& input ){
	size_t	  position = 0;
	size_t	  prefixEnd; 
	size_t	  commandEnd; 
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
		std::string prefix = input.substr(position, prefixEnd - position);
		position = prefixEnd + 1;
	}
	// Parsing the actuall command
	commandEnd = input.find(' ', position);
		if (commandEnd == std::string::npos){
			std::string command = input.substr(position, commandEnd);
			/*
			---> commandsMapIterator iterator for the command map <---
			std::map<std::string, >::iterator 
			
			commandsMapIterator = commandsMap.find(command);

			if (commandsMapIterator != commandsMap.end())
				---> call the corresponding command function <---
			*/
		}
	return (true);
}
