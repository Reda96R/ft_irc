#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <vector>
# include <map>
# include <string>
# include <algorithm>
# include "Client.hpp"
# include "StatBot.hpp"
# include <ctime>

class Channel {
    private :
        // CHANNEL SPECS
        std::string channelName;
        std::string channelTopic;
        std::string channelPassword;
		std::time_t	channelCreationTime;
        ssize_t userLimit;
        ssize_t userCount;

        // CLIENTS IN CHANNEL
        std::vector<Client*> channelClients;
        std::vector<Client*> channelOperators;
        std::map<std::string, Client*> channelClientsMap;
        std::map<std::string, Client*> channelOperatorsMap;
        std::vector<Client*> invitedUsers;


        // MODES
        bool channelPasswordProtected;
        bool channelInviteOnly;
        bool userLimitOnOff;
        bool topicProtected;

        // CANONICAL FORM
        Channel(void);
        Channel(const Channel& other);
        Channel& operator=(const Channel& other);
    public :

        // STATBOT
		statBot	  statBot;

        // CONSTRUCTOR / DESTRUCTOR
        Channel(std::string& ChannelName);
        ~Channel();

        // GETTERS
        std::string			    getChannelName() const;
        std::string			    getChannelTopic() const;
        std::vector<Client*>    getChannelClients() const;
        std::vector<Client*>    getChannelOperators() const;
		std::string			    getChannelClientsList() const;
		std::string			    getChannelAge() const;
        ssize_t		            getUserLimit() const;
        ssize_t                 getUserCount() const;

        // SETTERS
        void setChannelName(Client &me, std::string& channelName);
        void setChannelTopic(Client &me, std::string& channelTopic);
        void setChannelCreationTime( void );
        void addClient(Client &me);
        void removeClient(Client &me, struct ServerInfo& serverInfo);
        void addOperator(Client &me);
        void removeOperator(Client &me);
        void AddInvitedUser(Client &me);
        void removeInvitedUser(Client &me);
        void setTopicProtected();
        void removeTopicProtected();


        // METHODS
        void inviteUser(Client &me, struct ServerInfo& serverInfo, std::string& user);
        void kickUser(Client &me, struct ServerInfo& serverInfo, std::string& user, std::string reason);

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
        bool comparePassword(std::string& password);

        // IS METHODS
        void isThereSomeoneLeftRightNow(struct ServerInfo& serverInfo);
        bool isClientInChannel(std::string& nickname);
        bool isClientInChannel(Client &client);
        bool isClientOperator(std::string& nickname);
        bool isClientOperator(Client &client);
        bool isClientInvited(Client &client);
        bool isChannelInviteOnly(void);
        bool isChannelUserLimitOnOff(void);
        bool isChannelPasswordProtected(void);
};

#endif
