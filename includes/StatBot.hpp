#ifndef STATBOT_HPP
# define STATBOT_HPP

# include <map>
# include <vector>
# include <iostream>

class Channel;

class statBot{
	public:
		void									  ircBot( Channel& channel );
		std::map<std::string, int>				  messagesCount;
		std::vector<std::pair<std::string, int> > getStats( Channel& channel );

};

#endif
