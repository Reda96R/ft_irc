#include "../../includes/Client.hpp"
#include "../../includes/macros.hpp"
#include <netinet/in.h>
#include <string>
#include <sstream>

//::::::::::::::::::Constructors:::::::::::::::::::::::::
Client::Client( void ){
	this->clientNickname = "-";
	this->clientUsername = "-";
	this->clientIsOperator = "0";
	this->clientStatus = OFFLINE;
}

Client::Client( const Client& src){
	*this = src;
}

//::::::::::::::::::Operators:::::::::::::::::::::::::
Client& Client::operator=( const Client& rhs ){
	if (this != &rhs){
		this->clientNickname = rhs.clientNickname;
		this->clientUsername = rhs.clientUsername;
		this->clientIsOperator = rhs.clientIsOperator;
	}
	return (*this);
}

//::::::::::::::::::Getters and Setters:::::::::::::::::::::::::
bool	Client::getType( void ){
	return (this->clientIsOperator);
}

std::string&	  Client::getUsername( void ){
	return (this->clientUsername);
}

std::string&	  Client::getNickname( void ){
	return (this->clientNickname);
}

int&  Client::getSocket( void ){
	return (this->clientSocket);
}

struct sockaddr_in& Client::getAddress( void ){
	return (this->clientAddress);
}

int&  Client::getPollFd( void ){
	return (this->clientPollFd);
}

void	Client::setNickname( std::string& nickname ){
	this ->clientNickname = nickname;
}

void	Client::setUsername( std::string& username ){
	this->clientUsername = username;
}

void	Client::setType( bool type ){
	this->clientIsOperator = type;
}

void	Client::setSocket( const int& socket ){
	this->clientSocket = socket;
}

void	Client::setAddress( struct sockaddr_in& adress ){
	this->clientAddress = adress;
}

void	Client::setPollFd( const int& fd ){
	this->clientPollFd = fd;
}

//::::::::::::::::::Methods:::::::::::::::::::::::::
void	Client::clientAdd( void ){
	Client		newClient;
	socklen_t	addressLen = sizeof(newClient.getAddress());
	int			serverSocket = 0; // this will be replaced with the actuall server socket

	newClient.setSocket(accept(serverSocket, (struct sockaddr *) &newClient.getAddress(), &addressLen));
	/*
	TODO: check if the client is connected succesfully or not
	 if (newClient.getSocket() < 0)
	 	error;
	 else
		 std::cout << GREEN << "New client connected" << std::endl;
	*/
	newClient.setPollFd(newClient.getSocket());
	newClient.clientStatus = ONLINE;
}

bool	Client::clientRecv( char *recv){
	bool				isInputValid;
	std::string			tmp;
	std::istringstream	iss(recv);

	while (getline(iss, tmp)){
		if (inputParser(tmp))
			isInputValid = true;
		else
			isInputValid = false;
	}
	return (isInputValid);
}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Client::~Client( void ){
}
