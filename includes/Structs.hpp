#ifndef STRUCTS_HPP
# define STRUCTS_HPP

# include <string>
# include <vector>

//::::::::::::::::::Client:::::::::::::::::::::::::::
struct s_status { // struct showcasing the provided info and state of the client, true means that the user provided that info or that step is done
	bool	pass;
	bool	nick;
	bool	user;
	bool	authenticated;
	bool	registered;
};

//::::::::::::::::::Commands:::::::::::::::::::::::::
struct s_ircCommand {
	std::string				  prefix;
	std::string				  command;
	std::vector<std::string>	  arguments;
};


#endif
