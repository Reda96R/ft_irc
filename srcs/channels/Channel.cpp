# include "../../includes/Client.hpp"
# include "../../includes/Server.hpp"
# include "../../includes/Helpers.hpp"
# include "../../includes/Commands.hpp"
# include "../../includes/IrcErrors.hpp"
# include <ctime>
# include <sstream>
#include <string>

Channel::Channel(void) {

}

Channel::Channel(const Channel& other) {
    this->channelName = other.channelName;
    this->channelTopic = other.channelTopic;
    this->channelClients = other.channelClients;
    this->channelOperators = other.channelOperators;
    this->channelInviteOnly = other.channelInviteOnly;
    this->channelPasswordProtected = other.channelPasswordProtected;
    this->channelInviteOnly = other.channelInviteOnly;
    this->userLimit = other.userLimit;
    this->channelPassword = other.channelPassword;
    this->userLimitOnOff = other.userLimitOnOff;
}

Channel &Channel::operator=(const Channel& other) {
    this->channelName = other.channelName;
    this->channelTopic = other.channelTopic;
    this->channelClients = other.channelClients;
    this->channelOperators = other.channelOperators;
    this->channelInviteOnly = other.channelInviteOnly;
    this->channelPasswordProtected = other.channelPasswordProtected;
    this->channelInviteOnly = other.channelInviteOnly;
    this->userLimit = other.userLimit;
    this->channelPassword = other.channelPassword;
    this->userLimitOnOff = other.userLimitOnOff;
    return (*this);
}

// CONSTUCTOR / DESTRUCTOR
# include <stdint.h>
Channel::Channel(std::string& ChannelName){
    this->channelName = ChannelName;
    
    this->userLimit = SIZE_MAX;
    this->userCount = 1;

    this->channelPasswordProtected = false;
    this->channelInviteOnly = false;
    this->userLimitOnOff = false;
    this->topicProtected = false;
}


Channel::~Channel() {

}

// GETTERS

std::string Channel::getChannelName() const {
    return this->channelName;
}

std::string Channel::getChannelTopic() const {
    return (this->channelTopic);
}

std::string Channel::getChannelMode() const{
	std::string	  mode;
	if (this->channelPasswordProtected)
		mode += 'k';
	if (this->channelInviteOnly)
		mode += 'i';
	if (this->userLimitOnOff)
		mode += 'l';
	if (this->topicProtected)
		mode += 't';
	return (mode);
}

std::vector<Client*> Channel::getChannelClients() const {
    return this->channelClients;
}

std::vector<Client*> Channel::getChannelOperators() const {
    return this->channelOperators;
}

std::string	  Channel::getChannelClientsList() const {
	std::string				clientsList;
	std::vector<Client*>	clients = this->getChannelClients();
	std::vector<Client*>	operators = this->getChannelOperators();

	std::vector<Client*>::iterator it = clients.begin();
	if (it != clients.end()){
		clientsList += (*it)->getNickname();
		++it;
	}

	while (it != clients.end()){
		clientsList += " " + (*it)->getNickname();
		++it;
	}

	it = operators.begin();
	if (clientsList.empty() && it != operators.end()){
		clientsList += " @" + (*it)->getNickname();
		++it;
	}
	while (it != operators.end()){
		clientsList += " @" + (*it)->getNickname();
		++it;
	}
	return (clientsList);
}

std::string	  Channel::getChannelAge( void ) const{
	std::time_t	  now = std::time(NULL);

	double ageSeconds = std::difftime(now, this->channelCreationTime);

	std::ostringstream	  age;

	age.precision(2);
    age.setf(std::ios::fixed, std::ios::floatfield);

	if (ageSeconds < 60){
		age << ageSeconds << " seconds";
	}
	else if (ageSeconds < 3600){
		double ageMinutes = ageSeconds / 60.0;
		age << ageMinutes << " minutes";
	}
	else if (ageSeconds < 86400){
		double ageHours = ageSeconds / 3600.0;
		age << ageHours << " hours";
	}
	else{
		double ageDays = ageSeconds / 86400.0;
		age << ageDays << " days";
	}
	return (age.str());
}

ssize_t Channel::getUserLimit() const {
    return this->userLimit;
}

ssize_t Channel::getUserCount() const {
    return this->userCount;
}

void Channel::setChannelTopic(Client &me, std::string& channelTopic) {

    if (!this->topicProtected || std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->channelTopic = channelTopic;
		std::string message;
		message = ":"  + me.getNickname()  +
				  "!~" + me.getUsername()  +
				  "@"  + me.getIpAddress() +
				  " TOPIC " + channelName + " " + channelTopic + "\n";
		for (size_t i = 0; i < this->getChannelOperators().size(); ++i)
			messageToClient(*this->getChannelOperators().at(i), message);
		for (size_t i = 0; i < this->getChannelClients().size(); ++i)
			messageToClient(*this->getChannelClients().at(i), message);
    }
    else {
        s_ircReply	  replyInfo = {1, ERR_CHANOPRIVSNEEDED, me.getNickname(), this->channelName, errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }
}

void	Channel::setChannelCreationTime( void ){
	this->channelCreationTime = std::time(NULL);
}

void Channel::addClient(Client &me) {
    this->channelClients.push_back(&me);
    this->userCount++;
}

void Channel::removeClient(Client &m, struct ServerInfo& serverInfo) {
    Client *me = &m;
    for (size_t i = 0; i < this->channelClients.size(); ++i) {
        if (this->channelClients[i] == me) {
            // messageToChannel(*this, m, "QUIT");
            this->channelClients.erase(this->channelClients.begin() + i);
            this->userCount--;
            break;
        }
    }
    for (size_t i = 0; i < this->channelOperators.size(); ++i) {
        if (this->channelOperators[i] == me) {
            // messageToChannel(*this, m, "QUIT");
            this->channelOperators.erase(this->channelOperators.begin() + i);
            this->userCount--;
            break;
        }
    }
    isThereSomeoneLeftRightNow(serverInfo);
}

void Channel::addOperator(Client &me) {
    this->channelOperators.push_back(&me);
    // remove from clients list
    for (size_t i = 0; i < this->channelClients.size(); ++i) {
        if (this->channelClients[i] == &me) {
            this->channelClients.erase(this->channelClients.begin() + i);
            break;
        }
    }

}

void Channel::removeOperator(Client &me) {
    this->channelOperators.erase(std::remove(this->channelOperators.begin(), this->channelOperators.end(), &me), this->channelOperators.end());
    // add to clients list
    this->channelClients.push_back(&me);
}

void Channel::AddInvitedUser(Client &me) {
    this->invitedUsers.push_back(&me);
}

void Channel::removeInvitedUser(Client &me) {
    this->invitedUsers.erase(std::remove(this->invitedUsers.begin(), this->invitedUsers.end(), &me), this->invitedUsers.end());
}

void Channel::setTopicProtected() {
    this->topicProtected = true;
}

void Channel::removeTopicProtected() {
    this->topicProtected = false;
}

// METHODS

void Channel::inviteUser(Client &me, struct ServerInfo& serverInfo, std::string& user) {

    // check if the client has the right to invite users
    // if not return ;
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) == this->channelOperators.end())
        return ;

    // check if the string provided is a valid nickname
    // if not return ;
    Client *client_requested = NULL;
    for (std::vector<Client*>::iterator it = serverInfo.clients.begin(); it < serverInfo.clients.end(); it++) {
        if ((*it)->getNickname() == user) {
            client_requested = *it;
            break ;
        }
    }

    if (client_requested == NULL)
        return ;

    // check if the user is already in the channel
    if (std::find(this->channelClients.begin(), this->channelClients.end(), client_requested) != this->channelClients.end())
        return ;

    // check if the user is already invited
    if (std::find(this->invitedUsers.begin(), this->invitedUsers.end(), client_requested) != this->invitedUsers.end())
        return ;

    // add the user to the invited list
    this->invitedUsers.push_back(client_requested);

	std::string message = ":"  + me.getNickname() +
			"!~" + me.getUsername()  +
			"@"  + me.getIpAddress() +
			" "  + "INVITE " + user + " " + this->getChannelName() + "\n";
	messageToClient(*client_requested, message);
}

bool Channel::isClientInChannel(std::string &nick) {
    //check in operators list
    for (std::vector<Client*>::iterator it = this->channelOperators.begin(); it < this->channelOperators.end(); it++) {
        if ((*it)->getNickname() == nick) {
            return true;
        }
    }
    //check in clients list
    for (std::vector<Client*>::iterator it = this->channelClients.begin(); it < this->channelClients.end(); it++) {
        if ((*it)->getNickname() == nick) {
            return true;
        }
    }
    return false;
}

bool Channel::isClientInChannel(Client &client) {
    //check in operators list
    for (std::vector<Client*>::iterator it = this->channelOperators.begin(); it < this->channelOperators.end(); it++) {
        if ((*it)->getNickname() == client.getNickname()) {
            return true;
        }
    }
    //check in clients list
    for (std::vector<Client*>::iterator it = this->channelClients.begin(); it < this->channelClients.end(); it++) {
        if ((*it)->getNickname() == client.getNickname()) {
            return true;
        }
    }
    return false;
}

void Channel::kickUser(Client &me, struct ServerInfo& serverInfo, std::string& user, std::string reason) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) == this->channelOperators.end()) {
        s_ircReply	  replyInfo = {1, ERR_CHANOPRIVSNEEDED, me.getNickname(), this->channelName, errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }

	if (me.getNickname() == user){	
		std::string message = ":ircserv 483 " + user + " :You can't kick yourself\n";
        messageToClient(me, message);
        return ;
	}

    Client *kick = NULL;
    for (std::vector<Client*>::iterator it = serverInfo.clients.begin(); it < serverInfo.clients.end(); it++) {
        if ((*it)->getNickname() == user) {
            kick = *it;
            break ;
        }
    }

    if (kick == NULL) {
        s_ircReply	  replyInfo = {1, ERR_NOSUCHNICK, me.getNickname(), user, errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }

    // check if the user is in the channel
	if (kick->getChannels().find(this->getChannelName()) == kick->getChannels().end()){
        s_ircReply      replyInfo = {1, ERR_USERNOTINCHANNEL, me.getNickname(), kick->getNickname(), errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }


    for (size_t i = 0; i < this->channelClients.size(); ++i) {
        if (this->channelClients[i] == kick) {
            this->channelClients.erase(this->channelClients.begin() + i);
            break;
        }
    }
    for (size_t i = 0; i < this->channelOperators.size(); ++i) {
        if (this->channelOperators[i] == kick) {
            this->channelOperators.erase(this->channelOperators.begin() + i);
            break;
        }
    } 

    // remove from client channels' list
    kick->channelRemove(this->channelName);
    this->userCount--;
    std::string message;
	message = ":"  + me.getNickname()  +
			  "!~" + me.getUsername()  +
			  "@"  + me.getIpAddress() +
			  " "  + "KICK" + " " + this->getChannelName() + " " + kick->getNickname() + " " + reason + "\n";

	for (size_t i = 0; i < this->getChannelOperators().size(); ++i)
		messageToClient(*this->getChannelOperators().at(i), message);
	for (size_t i = 0; i < this->getChannelClients().size(); ++i)
		messageToClient(*this->getChannelClients().at(i), message);
    messageToClient(*kick, message);

}

void Channel::makeOperator(Client &me, std::string& user) {
    // check if we have the right to make someone an operator
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) == this->channelOperators.end())
        return ;
    // check if the user is in the channel
    Client *op = NULL;
    for (std::vector<Client*>::iterator it = this->channelClients.begin(); it < this->channelClients.end(); it++) {
        if ((*it)->getNickname() == user) {
            op = *it;
            break ;
        }
    }
    // add the user to the operators list
    if (op != NULL) {
        this->channelOperators.push_back(op);
		this->channelClients.erase(std::remove(this->channelClients.begin(), this->channelClients.end(), op), this->channelClients.end());
    }
}

void Channel::removeOperator(Client &me, std::string& user) {
    // check if we have the right to make someone a normal client
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) == this->channelOperators.end())
        return ;
    // check if the user is in the operators list
    Client *op = NULL;
    for (std::vector<Client*>::iterator it = this->channelOperators.begin(); it < this->channelOperators.end(); it++) {
        if ((*it)->getNickname() == user) {
            op = *it;
            break ;
        }
    }
    // remove the user from the operators list
    if (op != NULL){
		this->channelClients.push_back(op);
        this->channelOperators.erase(std::remove(this->channelOperators.begin(), this->channelOperators.end(), op), this->channelOperators.end());
	}
}

void Channel::setChannelPassword(Client &me, std::string& password) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->channelPasswordProtected = true;
        this->channelPassword = password;
    }
    else {
        s_ircReply	  replyInfo = {1, ERR_CHANOPRIVSNEEDED, me.getNickname(), this->channelName, errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }
}

void Channel::removeChannelPassword(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->channelPasswordProtected = false;
        this->channelPassword = "";
    }
    else {
        s_ircReply	  replyInfo = {1, ERR_CHANOPRIVSNEEDED, me.getNickname(), this->channelName, errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }
}

void Channel::setChannelInviteOnly(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->channelInviteOnly = true;
    }
    else {
        s_ircReply    replyInfo = {1, ERR_CHANOPRIVSNEEDED, me.getNickname(), this->channelName, errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }
}

void Channel::removeChannelInviteOnly(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->channelInviteOnly = false;
    }
    else {
        s_ircReply    replyInfo = {1, ERR_CHANOPRIVSNEEDED, me.getNickname(), this->channelName, errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }
}

void Channel::setChannelUserLimit(Client &me, ssize_t limit) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->userLimitOnOff = true;
        this->userLimit = limit;
    }
    else {
        s_ircReply    replyInfo = {1, ERR_CHANOPRIVSNEEDED, me.getNickname(), this->channelName, errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }
}

void Channel::removeChannelUserLimit(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->userLimitOnOff = false;
        this->userLimit = SIZE_MAX;
    }
    else {
        s_ircReply    replyInfo = {1, ERR_CHANOPRIVSNEEDED, me.getNickname(), this->channelName, errorMessages.at(replyInfo.errorCode) };
        messageToClient(me, replyGenerator(replyInfo));
        return ;
    }
}

void Channel::removeChannel(void) {
    this->~Channel();
}

void Channel::sendMessage(Client &usr, std::string& message) {
    if (usr.getSocket() != -1)
        send(usr.getSocket(), message.c_str(), message.length(), 0);
    else 
        return ;
}

bool Channel::comparePassword(std::string& password) {
    if (this->channelPassword == password) 
        return true;
    return false;
}

// IS METHODS

void Channel::isThereSomeoneLeftRightNow(struct ServerInfo& serverInfo) {
    int totalUser;

    totalUser = this->channelClients.size() + this->channelOperators.size();
    if (totalUser == 0)
    {
        serverInfo.channels.erase(this->getChannelName());
        delete this;
    }
}

bool Channel::isClientOperator(std::string& nickname) {
    for (std::vector<Client*>::iterator it = this->channelOperators.begin(); it < this->channelOperators.end(); it++) {
        if ((*it)->getNickname() == nickname) {
            return true;
        }
    }
    return false;
}

bool Channel::isClientOperator(Client &client) {
    for (std::vector<Client*>::iterator it = this->channelOperators.begin(); it < this->channelOperators.end(); it++) {
        if ((*it)->getNickname() == client.getNickname()) {
            return true;
        }
    }
    return false;
}

bool Channel::isClientInvited(Client &client) { 
    if (std::find(this->invitedUsers.begin(), this->invitedUsers.end(), &client) != this->invitedUsers.end())
        return true;
    return false;
}

bool Channel::isChannelInviteOnly(void) {
    return this->channelInviteOnly;
}

bool Channel::isChannelUserLimitOnOff(void) {
    return this->userLimitOnOff;
}

bool Channel::isChannelPasswordProtected(void) {
    return this->channelPasswordProtected;
}
