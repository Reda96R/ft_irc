#ifndef STRUCTS_HPP
# define STRUCTS_HPP

// #include "Client.hpp"
# include <string>
# include <stack>
# include <vector>

class Client;

//::::::::::::::::::Client:::::::::::::::::::::::::::
struct s_status { // struct showcasing the provided info and state of the client, true means that the user provided that info or that step is done
	bool	pass;
	bool	nick;
	bool	user;
	bool	authenticated;
	bool	registered;
	bool	connected;
};


//::::::::::::::::::Commands:::::::::::::::::::::::::
struct s_ircCommand {
	std::string					  prefix;
	std::string					  command;
	std::vector<std::string>	  arguments;
};

struct s_prvMsgCommand {
	std::string					  message;
	std::stack<std::string>		  targets;
};

struct s_messageInfo {
	std::string	  targetName;
	Client		  *sender;
	Client		  *receiver;
	std::string	  message;
};

#endif
