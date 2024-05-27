# include "../../includes/Channel.hpp"

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

std::vector<Client> Channel::getChannelClients() const {
    return this->channelClients;
}

// SETTERS

void Channel::setChannelName(Client &me, std::string& channelName) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end())
        this->channelName = channelName;
    else
        return ;
}

void Channel::setChannelTopic(Client &me, std::string& channelTopic) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end())
        this->channelTopic = channelTopic;
    else
        return ;
}

// METHODS

void Channel::inviteUser(Client &me, std::string& user) {
    (void) me;
    (void) user;
    // i need a the list of USERS in the server
    // iterates over them all, 
        // if any of the user nicknames matches the one provided above
            // send a private message in this format : "Invitation to ($server_name) from ($client_who_sent_request->name)"
        // else
            // return ;
}


void Channel::kickUser(Client &me, std::string& user) {
    std::vector<Client>::iterator it = this->channelOperators.begin();
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end()) {
        while (it != this->channelClients.end()) {
            if (it->getNickname() == user) {
                this->channelClients.erase(it);
                return ;
            }
            it++;
        }
    }
    else
        return ;
}

void Channel::makeOperator(Client &me, std::string& user) {
    std::vector<Client>::iterator it = this->channelOperators.begin();
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end()) {
        while (it != this->channelClients.end()) {
            if (it->getNickname() == user) {
                this->channelOperators.push_back(*it);
                return ;
            }
            it++;
        }
    }
    else
        return ;
}

void Channel::removeOperator(Client &me, std::string& user) {
    std::vector<Client>::iterator it = this->channelOperators.begin();
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end()) {
        while (it != this->channelClients.end()) {
            if (it->getNickname() == user) {
                this->channelOperators.erase(it);
                return ;
            }
            it++;
        }
    }
    else
        return ;
}

void Channel::setChannelPassword(Client &me, std::string& password) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end()) {
        this->channelPasswordProtected = true;
        this->channelPassword = password;
    }
    else
        return ;
}

void Channel::removeChannelPassword(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end()) {
        this->channelPasswordProtected = false;
        this->channelPassword = "";
    }
    else
        return ;
}

void Channel::setChannelInviteOnly(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end()) {
        this->channelInviteOnly = true;
    }
    else
        return ;
}

void Channel::removeChannelInviteOnly(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end()) {
        this->channelInviteOnly = false;
    }
    else
        return ;
}

void Channel::setChannelUserLimit(Client &me, ssize_t limit) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end()) {
        this->userLimitOnOff = true;
        this->userLimit = limit;
    }
    else
        return ;
}

void Channel::removeChannelUserLimit(Client &me) {
    if (std::find(this->channelOperators.begin(), this->channelOperators.end(), me) != this->channelOperators.end()) {
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

void Channel::sendChannelMessage(Client &me, std::string& message) {
    std::vector<Client>::iterator it = this->channelOperators.begin();
    std::string fullMessage = me.getNickname() + " : " + message;
    while (it != this->channelClients.end()) {
        this->sendMessage((*it), fullMessage);
        it++;
    }
}
