#include "../../includes/Client.hpp"
#include <string>

//::::::::::::::::::Constructors:::::::::::::::::::::::::
Client::Client( void ){
	this->nickname = "-";
	this->username = "-";
	this->isOperator = "0";
}

Client::Client( const Client& src){
	*this = src;
}

//::::::::::::::::::Operators:::::::::::::::::::::::::
Client& Client::operator=( const Client& rhs ){
	if (this != &rhs){
		this->nickname = rhs.nickname;
		this->username = rhs.username;
		this->isOperator = rhs.isOperator;
	}
	return (*this);
}

//::::::::::::::::::Getters and Setters:::::::::::::::::::::::::
bool	Client::getType( void ){
	return (this->isOperator);
}

std::string	  Client::getUsername( void ){
	return (this->username);
}

std::string	  Client::getNickname( void ){
	return (this->nickname);
}

void	Client::setNickname( std::string nickname ){
	this ->nickname = nickname;
}

void	Client::setUsername( std::string username ){
	this->username = username;
}

void	Client::setType( bool type ){
	this->isOperator = type;
}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Client::~Client( void ){
}
