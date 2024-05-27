#ifndef HELPERS_HPP
# define HELPERS_HPP

# include <sstream>
# include "Client.hpp"
# include "Channel.hpp"
# include "macros.hpp"

//::::::::::::::::::Channels:::::::::::::::::::::::::
bool		isValidChannelName( std::string& );

//::::::::::::::::::Commands:::::::::::::::::::::::::
bool		privmsgAnalyser(std::vector<std::string>, s_prvMsgCommand& );
bool		messageToClient( Client& , Client& , std::string );
bool		trailingCheck( std::vector<std::string> arguments );

void		compareStrings(const std::string& str1, const std::string& str2);

#endif
