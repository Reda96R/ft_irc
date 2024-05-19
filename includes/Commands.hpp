#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <map>
# include "Client.hpp"
# include "Helpers.hpp"
# include "macros.hpp"


class Commands {
	private:
	std::map<std::string, void (Commands::*) ( Client& )> commandsMap; //containing the command and pointer to its function

	public:
		Commands( void );
		Commands( const Commands& );
		Commands &operator=( const Commands& );
		~Commands( void );

		//::::::::Getters and Setters::::::
		std::map<std::string, void (Commands::*) ( Client& )> getCommandMap( void ) const;
		
		
		//::::::::::::Commands:::::::::::::
			/* ~~~general commands ~~~ */
		void	passCommand( Client& client );
		void	nickCommand( Client& client );
		void	userCommand( Client& client );
		void	joinCommand( Client& client );
		void	privmsgCommand( Client& client );

			/* ~~~channel commands ~~~ */
		void	topicCommand( Client& client );
		void	kickCommand( Client& client );
		void	modeCommand( Client& client );
		void	inviteCommand( Client& client );
};

typedef  std::map<std::string, void (Commands::*) ( Client& )> t_commandsMap;

#endif
