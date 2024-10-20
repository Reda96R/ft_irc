#ifndef IRCERRORS_HPP
# define IRCERRORS_HPP

# include <iostream>
# include <map>
# include "Helpers.hpp"


enum IrcErrors{
	RPL_WELCOME			  = 001,
	RPL_NOTOPIC			  = 331,
	ERR_NOSUCHNICK		  = 401,
	ERR_NOSUCHCHANNEL	  = 403,
	ERR_CANNOTSENDTOCHAN  = 404,
	ERR_NOTEXTTOSEND	  = 412,
	ERR_BADMASK			  = 415,
	ERR_UNKNOWNCOMMAND	  = 421,
	ERR_NONICKNAMEGIVEN	  = 431,
	ERR_NICKNAMEINUSE	  = 433,
	ERR_USERNOTINCHANNEL  = 441,
	ERR_NOTONCHANNEL	  = 442,
	ERR_USERONCHANNEL	  = 443,
	ERR_NOTREGISTERED	  = 451,
	ERR_NEEDMOREPARAMS	  = 461,
	ERR_ALREADYREGISTRED  = 462,
	ERR_PASSWDMISMATCH	  = 464,
	ERR_KEYSET			  = 467,
	ERR_CHANNELISFULL	  = 471,
	ERR_UNKNOWNMODE		  = 472,
	ERR_INVITEONLYCHAN	  = 473,
	ERR_BANNEDFROMCHAN	  = 474,
	ERR_BADCHANNELKEY	  = 475,
	ERR_BADCHANMASK		  = 476,
	ERR_NOCHANMODES		  = 477,
	ERR_BANLISTFULL		  = 478,
	ERR_CHANOPRIVSNEEDED  = 482,
	/** ADAM_ERROR_CODES = 86 LBSSALA **/

	// KICK --> Already included

	// RPL_KICK				= 404,

	// INVITE

	RPL_INVITING			= 341,
	RPL_AWAY				= 301,

	// TOPIC

	RPL_TOPIC  		  		= 332,

	// MODE

	RPL_CHANNELMODEIS		= 324,
	RPL_BANLIST				= 367,
	RPL_EXCEPTLIST			= 348,
	RPL_INVITELIST			= 346,
	RPL_ENDOFBANLIST		= 368,
	RPL_ENDOFEXCEPTLIST		= 349,
	RPL_ENDOFINVITELIST		= 347,
	RPL_UNIQOPIS			= 325,


	ERR_UMODEUNKNOWNFLAG  = 501
};

struct s_ircReply {
	int					type;
	IrcErrors			errorCode;
	const std::string&	sender;
	const std::string&	target;
	const std::string&	message;

};

//nick + target + channel
	// ERR_USERNOTINCHANNEL  = 441,

extern std::map<IrcErrors, std::string> errorMessages;

std::string	  replyGenerator( s_ircReply replyInfo );



#endif

/*
type1:
	:ircserv + errorCode + sender + target + :message
type2:
	:ircserv + errorCode + sender + :message + target
type3:
	:ircserv + errorCode + sender + target + message
*/
