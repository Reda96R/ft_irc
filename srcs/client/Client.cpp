#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/macros.hpp"

//::::::::::::::::::Constructors:::::::::::::::::::::::::
Client::Client( void ){
	this->clientNickname = "-";
	this->clientUsername = "-";
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
	}
	return (*this);
}

bool	Client::operator==( const Client& rhs ) const{
	if (this->clientNickname == rhs.clientNickname)
		return (true);
	return (false);
}

//::::::::::::::::::Getters and Setters:::::::::::::::::::::::::
std::map<std::string, Channel*>	  Client::getChannels( void ){
	return (this->channels);
}

struct s_status Client::getStatus( void ) const{
	return (this->clientStatus);
}

std::string	  Client::getIpAddress( void ) const{
	return (this->clientIpAddress);
}

std::string	  Client::getRealname( void ) const{
	return (this->clientRealname);
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
	this->clientNickname = nickname;
}

void	Client::setUsername( std::string& username ){
	this->clientUsername = username;
}

void	Client::setRealname( std::string& realname ){
	this->clientRealname = realname;
}

void	Client::setIpAddress( std::string& ipAddress ){
	this->clientIpAddress = ipAddress;
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
	else if (target == "connected")
		this->clientStatus.connected = value;
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
	else if (target == "buffer")
		this->clientInput.buffer = value;
}
//::::::::::::::::::Methods:::::::::::::::::::::::::
bool	Client::clientAdd( int serverSocket, std::vector<Client*>& clients, std::vector<pollfd>& fds){
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_sockfd = accept(serverSocket, (struct sockaddr *) &client_addr, &client_len);

        if (client_sockfd < 0) {
            std::cerr << RED << "Error accepting client connection" << RESET << std::endl;
            return (false);
        } else {
            std::cout << GREEN << "New client connected" << RESET << std::endl;
        }

        this->setSocket(client_sockfd);
        this->setAddress(client_addr);

		//Ip address of the client 
		std::string	  ipAddress = inet_ntoa(client_addr.sin_addr);
		this->setIpAddress(ipAddress);

        this->setPollFd(client_sockfd);

		struct pollfd pfd;
		memset(&pfd, 0, sizeof(pfd));
		pfd.fd = getSocket();
		pfd.events = POLLIN;
		pfd.revents = 0;
		fds.push_back(pfd);
        clients.push_back(this);
		
		return (true);
}

void	Client::channelAdd( Channel& channel ){
	this->channels[channel.getChannelName()] = &channel;
}

void	Client::channelRemove( std::string channelName ){
	std::map<std::string, Channel*>::iterator it = this->channels.find(channelName);
	if (it != this->channels.end()) {
		this->channels.erase(this->channels.find(channelName));
	}
}

std::string	  sanitizeInput(std::string input ){
	std::string	  ret;
	size_t j = 0;
	for (size_t i = 0; i < input.size(); ++i){
		if (input[i] != EOF)
			ret[j++] = input[i];
	}
	return (ret);
}

void Client::quitAllChannels(struct ServerInfo& serverInfo) {
	std::map<std::string, Channel*>::iterator it;
	for (it = this->channels.begin(); it != this->channels.end(); ++it) {
		(it->second)->removeClient(*this, serverInfo);
		//this->channelRemove(it->first);
	}
}

bool Client::amIInChannel(std::string& channelName) {
	if (this->channels.find(channelName) != this->channels.end())
		return true;
	return false;
}

// Receives message from clients
bool	Client::clientRecv( struct ServerInfo& serverInfo ){
	int		ret;
    char	buf[1024];
    std::string		message;

    ret = recv(this->getSocket(), buf, sizeof(buf), 0);
	if (ret == -1){
        perror("recv");
		return (false);
	}
	else if (ret == 0) {
        std::cerr << RED << "Client disconnected" << RESET << std::endl;
		quitAllChannels(serverInfo);
		return (false);
	}
	else
    {
        buf[ret] = '\0';
        message = buf;

		std::stringstream iss(message);
		std::string		  tmp;

		while (getline(iss, tmp)){
			this->buffer += tmp;
			if (!iss.eof()){
				if (commandParser(this->buffer, *this, serverInfo) < 0){
					std::cerr << RED << "Client disconnected" << RESET << std::endl;
					return (false);
				}
				this->buffer.clear();
			}
			iss.clear();
		}
	}
	return (true);
}

void	Client::clearInput( void ){
	this->clientInput.prefix.clear();
	this->clientInput.command.clear();
	this->clientInput.arguments.clear();
}

//::::::::::::::::::Deconstructor:::::::::::::::::::::::::
Client::~Client( void ){
}
