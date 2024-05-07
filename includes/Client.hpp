#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include "Structs.hpp"
# include <netinet/in.h>

class Client {
	private:
		std::string				clientNickname;
		std::string				clientUsername;
		bool					clientIsOperator; // true if the client is an operator and not a regular user
		int						clientStatus;
		struct s_ircCommand		clientInput;

		int						clientSocket;
		int						clientPollFd;
		struct sockaddr_in		clientAddress;

	public:
		Client( void );
		Client( const Client& );
		Client& operator=( const Client& );
		~Client( void );

		//::::::::Getters and Setters:::::::::::::::
		void					setNickname( std::string& );
		void					setUsername( std::string& );
		void					setType( bool );
		void					setStatus( int );
		int						getStatus( void );
		bool					getType( void );
		std::string&			getUsername( void );
		std::string&			getNickname( void );

		void					setSocket( const int& );
		void					setAddress( struct sockaddr_in&  );
		void					setPollFd( const int& );
		int&					getSocket( void );
		struct sockaddr_in&		getAddress( void );
		int&					getPollFd( void );
		struct s_ircCommand&	getInput( void );

		//::::::::Methods:::::::::::::::
		void				clientAdd( void );
		bool				clientRecv( char *recv);
};



bool	commandParser( std::string& , s_ircCommand&, Client& );

#endif // !CLIENT_HPP
