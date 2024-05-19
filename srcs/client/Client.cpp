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
	this->clientStatus.pass = false;
	this->clientStatus.nick = false;
	this->clientStatus.user = false;
	this->clientStatus.registered = false;
	this->clientStatus.authenticated = false;
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
struct s_status Client::getStatus( void ) const{
	return (this->clientStatus);
}

bool	Client::getType( void ) const{
	return (this->clientIsOperator);
}

std::string	  Client::getUsername( void ) const{
	return (this->clientUsername);
}

std::string	  Client::getNickname( void ) const{
	return (this->clientNickname);
}

struct s_ircCommand Client::getInput( void ) const{
	return (this->clientInput);
}

int  Client::getSocket( void ) const{
	return (this->clientSocket);
}

struct sockaddr_in Client::getAddress( void ) const{
	return (this->clientAddress);
}

int  Client::getPollFd( void ) const{
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

void	Client::setStatus( std::string target, bool value ){
	if (target == "pass")
		this->clientStatus.pass = value;
	else if (target == "nick")
		this->clientStatus.nick = value;
	else if (target == "user")
		this->clientStatus.user = value;
	else if (target == "authenticated")
		this->clientStatus.authenticated = value;
	else if (target == "registered")
		this->clientStatus.registered = value;
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

void	Client::setInput( std::string target, std::string& value ){
	if (target == "prefix")
		this->clientInput.prefix = value;
	else if (target == "command")
		this->clientInput.command = value;
	else if (target == "arguments")
		this->clientInput.arguments.push_back(value);
}

//::::::::::::::::::Methods:::::::::::::::::::::::::
bool	Client::clientAdd( int serverSocket, std::vector<Client>& clients){
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_sockfd = accept(serverSocket, (struct sockaddr *) &client_addr, &client_len);
        if (client_sockfd < 0) {
            std::cerr << RED << "Error accepting client connection" << RESET << std::endl;
            return false;
        } else {
            std::cout << GREEN << "New client connected" << RESET << std::endl;
        }

        this->setSocket(client_sockfd);
        this->setAddress(client_addr);
        this->setPollFd(client_sockfd);

        clients.push_back(*this);
		return true;
}

bool	Client::clientRecv( char *recv){
	bool				isInputValid = false;
	std::string			tmp;
	std::istringstream	iss(recv);

	// while (getline(iss, tmp)){
	// 	if (commandParser(tmp))
	// 		isInputValid = true;
	// 	else
	// 		isInputValid = false;
	// }
	return (isInputValid);
}

void	Client::clearInput( void ){
	this->clientInput.prefix.clear();
	this->clientInput.command.clear();
	this->clientInput.arguments.clear();
}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Client::~Client( void ){
}

//::::::::::::::::::Comparison operator:::::::::::::::::::::::::
bool	Client::operator==( const Client& rhs ) const{
	if (this->clientNickname == rhs.clientNickname)
		return (true);
	return (false);
}