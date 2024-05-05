#include "../../includes/Commands.hpp"

//::::::::::::::::::Constructors:::::::::::::::::::::::::
Commands::Commands( void ){
	this->commandsMap["PASS"] = &Commands::passCommand;
	this->commandsMap["NICK"] = &Commands::nickCommand;
	this->commandsMap["USER"] = &Commands::userCommand;
	this->commandsMap["JOIN"] = &Commands::joinCommand;
	this->commandsMap["PRIVMSG"] = &Commands::privmsgCommand;
}

//::::::::::::::::::Operators:::::::::::::::::::::::::
Commands& Commands::operator=( const Commands& rhs ){
	if (this != &rhs){
		this->commandsMap = rhs.commandsMap;
	}
	return (*this);
}


//::::::::::::::::::Getters and Setters:::::::::::::::::::::::::
std::map<std::string, void (Commands::*) ( Client& )>& Commands::getCommandMap( void ){
	return (this->commandsMap);
}

//::::::::::::::::::Methods:::::::::::::::::::::::::
void	Commands::passCommand( Client& client ){
	(void) client;
}

void	Commands::nickCommand( Client& client ){
	(void) client;
}

void	Commands::userCommand( Client& client ){
	(void) client;
}

void	Commands::joinCommand( Client& client ){
	(void) client;
}

void	Commands::privmsgCommand( Client& client ){
	(void) client;
}


Commands::Commands( const Commands& src){
	*this = src;
}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Commands::~Commands( void ){
}
