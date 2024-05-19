#ifndef HELPERS_HPP
# define HELPERS_HPP

# include <sstream>
# include "Client.hpp"
# include "macros.hpp"

//::::::::::::::::::Channels:::::::::::::::::::::::::
bool		isValidChannelName( std::string& );

//::::::::::::::::::Commands:::::::::::::::::::::::::
bool		privmsgAnalyser(std::vector<std::string>, s_prvMsgCommand& );

#endif
