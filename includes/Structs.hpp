#ifndef STRUCTS_HPP
# define STRUCTS_HPP

# include <string>
# include <stack>

//::::::::::::::::::Commands:::::::::::::::::::::::::
typedef struct s_ircCommand{

	std::string				  prefix;
	std::string				  command;
	std::stack<std::string>	  arguments;

} t_ircComand;

#endif
