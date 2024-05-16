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

		struct s_status			clientStatus;
		struct s_ircCommand		clientInput;

		int						clientSocket;
		int						clientPollFd;
		struct sockaddr_in		clientAddress;

	public:
		Client( void );
		Client( const Client& );
		Client& operator=( const Client& );
		~Client( void );

		//Comparison operator
		bool operator==( const Client& ) const;

		//::::::::Getters and Setters:::::::::::::::
		void					setNickname( std::string& );
		void					setUsername( std::string& );
		void					setType( bool );
		void					setStatus( std::string, bool );

		struct s_status			getStatus( void ) const;
		bool					getType( void ) const;
		std::string				getUsername( void ) const;
		std::string				getNickname( void ) const;

		void					setSocket( const int& );
		void					setAddress( struct sockaddr_in&  );
		void					setPollFd( const int& );
		void					setInput( std::string, std::string& );

		int						getSocket( void ) const;
		struct sockaddr_in		getAddress( void ) const;
		int						getPollFd( void ) const;
		struct s_ircCommand		getInput( void ) const;

		//::::::::Methods:::::::::::::::
		void				clientAdd( void );
		bool				clientRecv( char *recv);

		void				clearInput( void );
};



bool	commandParser( std::string&, Client& );

#endif // !CLIENT_HPP
