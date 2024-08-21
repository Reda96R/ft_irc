#ifndef COMMANDS_HPP
# define COMMANDS_HPP

# include <map>
# include "Client.hpp"
# include "Channel.hpp"
# include "Helpers.hpp"
# include "macros.hpp"
# include "Server.hpp"


class Commands {
	private:
	std::map<std::string, void (Commands::*) ( Client&, struct ServerInfo& )> commandsMap; //containing the command and pointer to its function

	public:
		Commands( void );
		Commands( const Commands& );
		Commands &operator=( const Commands& );
		~Commands( void );

		//::::::::Getters and Setters::::::
		std::map<std::string, void (Commands::*) ( Client&, struct ServerInfo& )> getCommandMap( void ) const;
		
		
		//::::::::::::Commands:::::::::::::
			/* ~~~general commands ~~~ */
		void	passCommand( Client&, struct ServerInfo&);
		void	nickCommand( Client&, struct ServerInfo& );
		void	userCommand( Client&, struct ServerInfo& );
		void	joinCommand( Client&, struct ServerInfo& );
		void	privmsgCommand( Client&, struct ServerInfo& );
		void	pingCommand( Client&, struct ServerInfo& );
		void	quitCommand( Client&, struct ServerInfo& );

			/* ~~~channel commands ~~~ */
		void	kickChannelCommand( Client&, struct ServerInfo& );
		void	inviteChannelCommand( Client&, struct ServerInfo& );
		void	topicChannelCommand( Client&, struct ServerInfo& );
		void	modeChannelCommand( Client&, struct ServerInfo& );

			/* ~~~channel commands ~~~ */
		void	topicCommand( Client&  );
		void	kickCommand( Client&  );
		void	modeCommand( Client&  );
		void	inviteCommand( Client&  );
};

typedef  std::map<std::string, void (Commands::*) ( Client&, struct ServerInfo& )> t_commandsMap;

#endif
