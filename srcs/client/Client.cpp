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
void	Client::clientAdd( void ){
	// TODO: check if the client is connected succesfully or not
	Client		newClient;
    socklen_t	len = sizeof(newClient.getAddress());

    newClient.setSocket(accept(this->getSocket(), &newClient.getAddress(), &len));
    if (newClient.getSocket() == -1)
        std::cerr << RED << "Error: accept" << RESET << std::endl;
    else
        std::cout << GREEN << "New client connected" << RESET << std::endl;
    newClient.setPollFd(newClient.getSocket());
    // add the new client to the list of clients
    // struct needs to be defined in the header file
}

void	Client::clientRecv( char *recv){
    // TODO: receive the message from the client
    int		ret;
    char	buf[1024];
    std::string		message;

    ret = recv(this->getSocket(), buf, 1024, 0);
    if (ret == -1)
        std::cerr << RED << "Error: receiving Failed" << RESET << std::endl;
    else if (ret == 0)
        std::cerr << RED << "Error: client disconnected" << RESET << std::endl;
    else
    {
        buf[ret] = '\0';
        message = buf;
        std::cout << GREEN << "Message received: " << message << RESET << std::endl;
        commandParser(message, *this);
    }
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