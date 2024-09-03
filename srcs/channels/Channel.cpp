# include "../../includes/Channel.hpp"
# include "../../includes/Client.hpp"
# include "../../includes/Server.hpp"
# include <ctime>
# include <sstream>

// USELESS CANONICAL FORM
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

Channel::Channel(std::string& ChannelName){
    this->channelName = ChannelName;
    
    this->channelTopic = "";

    this->channelPasswordProtected = false;
    this->channelInviteOnly = false;
    this->userLimitOnOff = false;
}


Channel::~Channel() {

}

// GETTERS

std::string Channel::getChannelName() const {
    return this->channelName;
}

std::string Channel::getChannelTopic() const {
    return this->channelTopic;
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
		// if (this->getChannelOperators().find((*it)->getNickname))
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

// SETTERS

void Channel::setChannelName(Client &me, std::string& channelName) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end())
        this->channelName = channelName;
    else
        return ;
}

void Channel::setChannelTopic(Client &me, std::string& channelTopic) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end())
        this->channelTopic = channelTopic;
    else
        return ;
}

void	Channel::setChannelCreationTime( void ){
	this->channelCreationTime = std::time(NULL);
}

void Channel::addClient(Client &me) {
    this->channelClients.push_back(&me);
}

void Channel::removeClient(Client &m) {
    Client *me = &m;
    for (size_t i = 0; i < this->channelClients.size(); ++i) {
        if (this->channelClients[i] == me) {
            this->channelClients.erase(this->channelClients.begin() + i);
            break;
        }
    }
}
void Channel::addOperator(Client &me) {
    this->channelOperators.push_back(&me);
}

void Channel::removeOperator(Client &me) {
    this->channelOperators.erase(std::remove(this->channelOperators.begin(), this->channelOperators.end(), &me), this->channelOperators.end());
}

void Channel::AddInvitedUser(Client &me) {
    this->invitedUsers.push_back(&me);
}

void Channel::removeInvitedUser(Client &me) {
    this->invitedUsers.erase(std::remove(this->invitedUsers.begin(), this->invitedUsers.end(), &me), this->invitedUsers.end());
}

// METHODS

void Channel::inviteUser(Client &me, struct ServerInfo& serverInfo, std::string& user) {
    // i need a the list of USERS in the server
    if (this->channelInviteOnly == false)
        return ;

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
    // send a private message to the user -- REDA
}

void Channel::kickUser(Client &me, struct ServerInfo& serverInfo, std::string& user) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) == this->channelOperators.end())
        return ;

    Client *kick = NULL;
    for (std::vector<Client*>::iterator it = serverInfo.clients.begin(); it < serverInfo.clients.end(); it++) {
        if ((*it)->getNickname() == user) {
            kick = *it;
            break ;
        }
    }

    if (kick == NULL)
        return ;

    this->channelClients.erase(std::remove(this->channelClients.begin(), this->channelClients.end(), kick), this->channelClients.end());
    this->channelOperators.erase(std::remove(this->channelOperators.begin(), this->channelOperators.end(), kick), this->channelOperators.end());
    this->invitedUsers.erase(std::remove(this->invitedUsers.begin(), this->invitedUsers.end(), kick), this->invitedUsers.end());
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
    if (op != NULL)
        this->channelOperators.push_back(op);
}

void Channel::removeOperator(Client &me, std::string& user) {
    // check if we have the right to remove someone from the operators
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
    if (op != NULL)
        this->channelOperators.erase(std::remove(this->channelOperators.begin(), this->channelOperators.end(), op), this->channelOperators.end());
}

void Channel::setChannelPassword(Client &me, std::string& password) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->channelPasswordProtected = true;
        this->channelPassword = password;
    }
    else
        return ;
}

void Channel::removeChannelPassword(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->channelPasswordProtected = false;
        this->channelPassword = "";
    }
    else
        return ;
}

void Channel::setChannelInviteOnly(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->channelInviteOnly = true;
    }
    else
        return ;
}

void Channel::removeChannelInviteOnly(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->channelInviteOnly = false;
    }
    else
        return ;
}

void Channel::setChannelUserLimit(Client &me, ssize_t limit) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->userLimitOnOff = true;
        this->userLimit = limit;
    }
    else
        return ;
}

void Channel::removeChannelUserLimit(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), &me) != this->channelOperators.end()) {
        this->userLimitOnOff = false;
        this->userLimit = 0;
    }
    else
        return ;
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

//TODO: fix the pointer issue
// void Channel::sendChannelMessage(Client &me, std::string& message) {
//     std::vector<Client>::iterator it = this->channelOperators.begin();
//     std::string fullMessage = me.getNickname() + " : " + message;
//     while (it != this->channelClients.end()) {
//         this->sendMessage((*it), fullMessage);
//         it++;
//     }
// }
