#ifndef HELPERS_HPP
# define HELPERS_HPP

# include <sstream>
# include <iomanip>
# include "Client.hpp"
# include "Channel.hpp"
#include "Structs.hpp"
# include "macros.hpp"

//::::::::::::::::::Channels:::::::::::::::::::::::::
bool		isValidChannelName( std::string& );

//::::::::::::::::::Commands:::::::::::::::::::::::::
bool		privmsgAnalyser(std::vector<std::string>, s_prvMsgCommand& , Client& );

bool		messageToClient( Client&, std::string );
bool		messageToClient( s_messageInfo );
bool		messageToChannel( Channel& , Client& , std::string );

bool		trailingCheck( std::vector<std::string> arguments );

void		compareStrings( const std::string& str1, const std::string& str2 );

std::string intToString( int );

#endif
