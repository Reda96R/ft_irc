#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include <string>
# include <algorithm>
# include "Client.hpp"

class Channel {
    private :
        // CHANNEL SPECS
        std::string channelName;
        std::string channelTopic;
        std::string channelPassword;
        ssize_t userLimit;

        // CLIENTS IN CHANNEL
        std::vector<Client> channelClients;
        std::vector<Client> channelOperators;
        std::vector<Client> invitedUsers;

        // MODES
        bool channelPasswordProtected;
        bool channelInviteOnly;
        bool userLimitOnOff;

        // CANONICAL FORM
        Channel(void);
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);
    public :

        // CONSTRUCTOR / DESTRUCTOR
        Channel(std::string& ChannelName);
        ~Channel();

        // GETTERS
        std::string getChannelName() const;
        std::string getChannelTopic() const;
        std::vector<Client> getChannelClients() const;

        // SETTERS
        void setChannelName(Client &me, std::string& channelName);
        void setChannelTopic(Client &me, std::string& channelTopic);
        void addClient(Client &me);
        void removeClient(Client &me);
        void addOperator(Client &me);
        void removeOperator(Client &me);
        void AddInvitedUser(Client &me);
        void removeInvitedUser(Client &me);


        // METHODS
        void inviteUser(Client &me, struct ServerInfo& serverInfo, std::string& user);
        void kickUser(Client &me, std::string& user);

        void makeOperator(Client &me, std::string& user);
        void removeOperator(Client &me, std::string& user);

        void setChannelPassword(Client &me, std::string& password);
        void removeChannelPassword(Client &me);

        void setChannelInviteOnly(Client &me);
        void removeChannelInviteOnly(Client &me);
    
        void setChannelUserLimit(Client &me, ssize_t limit);
        void removeChannelUserLimit(Client &me);

        void removeChannel(void);

        void sendMessage(Client &usr, std::string& message);

        void sendChannelMessage(Client &me, std::string& message);    
};

#endif
