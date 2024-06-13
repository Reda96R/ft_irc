#include "../../includes/IrcErrors.hpp"

std::map<IrcErrors, std::string> errorMessages;

void initializeErrorMessages() {
    errorMessages[RPL_WELCOME]			= "Welcome to the Internet Relay Network!";
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
    errorMessages[ERR_PASSWDMISMATCH]	= "Password incorrect";
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
}

// Ensure the map is initialized at the start
struct ErrorMessagesInitializer {
    ErrorMessagesInitializer() {
        initializeErrorMessages();
    }
} errorMessagesInitializer;


std::string	  replyGenerator( IrcErrors errorCode, const std::string& sender, const std::string& target){
	return (":ircserv " + intToString(errorCode) + " " + sender + " "
			+ target + " :" + errorMessages.at(errorCode) + " \r\n"); 
}
