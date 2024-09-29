#include "../../includes/IrcErrors.hpp"

std::map<IrcErrors, std::string> errorMessages;

void initializeErrorMessages() {
    errorMessages[RPL_WELCOME]			= "Welcome to the Internet Relay Network";
    errorMessages[ERR_NOSUCHNICK]		= "No such nick/channel";
    errorMessages[ERR_NOSUCHCHANNEL]	= "No such channel";
    errorMessages[ERR_CANNOTSENDTOCHAN] = "Cannot send to channel";
    errorMessages[ERR_NOTEXTTOSEND]		= "No text to send";
    errorMessages[ERR_BADMASK]			= "Bad Server/host mask";
    errorMessages[ERR_UNKNOWNCOMMAND]	= "Unknown command";
    errorMessages[ERR_NONICKNAMEGIVEN]	= "No nickname given";
    errorMessages[ERR_NICKNAMEINUSE]	= "Nickname is already in use";
    errorMessages[ERR_USERNOTINCHANNEL] = "They aren't on that channel";
    errorMessages[ERR_NOTONCHANNEL]		= "You're not on that channel";
    errorMessages[ERR_USERONCHANNEL]	= "is already on channel";
    errorMessages[ERR_NOTREGISTERED]	= "You have not registered";
    errorMessages[ERR_NEEDMOREPARAMS]	= "Not enough parameters";
    errorMessages[ERR_ALREADYREGISTRED] = "Unauthorized command (already registered)";
    errorMessages[ERR_PASSWDMISMATCH]	= "Password required";
    errorMessages[ERR_KEYSET]			= "Channel key already set";
    errorMessages[ERR_CHANNELISFULL]	= "Cannot join channel (+l)";
    errorMessages[ERR_UNKNOWNMODE]		= "is unknown mode char to me";
    errorMessages[ERR_INVITEONLYCHAN]	= "Cannot join channel (+i)";
    errorMessages[ERR_BANNEDFROMCHAN]	= "Cannot join channel (+b)";
    errorMessages[ERR_BADCHANNELKEY]	= "Cannot join channel (+k)";
    errorMessages[ERR_BADCHANMASK]		= "Bad Channel Mask";
    errorMessages[ERR_NOCHANMODES]		= "Channel doesn't support modes";
    errorMessages[ERR_BANLISTFULL]		= "Channel list is full";
    errorMessages[ERR_CHANOPRIVSNEEDED] = "You're not channel operator";
    errorMessages[ERR_UMODEUNKNOWNFLAG] = "Unknown MODE flag";

    // KICK

    errorMessages[RPL_KICK]				= "has been kicked";

    // INVITE
    errorMessages[RPL_INVITING]			= "is inviting you to";
    errorMessages[RPL_AWAY]				= "is away";

	// TOPIC
    errorMessages[RPL_TOPIC]			= "Topic:";
    errorMessages[RPL_NOTOPIC]			= "No topic is set";

	
    // MODE 
    errorMessages[RPL_CHANNELMODEIS]	= "Channel Mode is";
    errorMessages[RPL_BANLIST]			= "Ban List";
    errorMessages[RPL_EXCEPTLIST]		= "Exception List";
    errorMessages[RPL_INVITELIST]		= "Invite List";
    errorMessages[RPL_ENDOFBANLIST]		= "End of Ban List";
    errorMessages[RPL_ENDOFEXCEPTLIST]	= "End of Exception List";
    errorMessages[RPL_ENDOFINVITELIST]	= "End of Invite List";
    errorMessages[RPL_UNIQOPIS]			= "is the unique channel operator";

}

// Ensure the map is initialized at the start
struct ErrorMessagesInitializer {
    ErrorMessagesInitializer() {
        initializeErrorMessages();
    }
} errorMessagesInitializer;


