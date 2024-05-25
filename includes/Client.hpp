#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <poll.h>
# include <vector>
# include <iostream>
# include <netinet/in.h>
# include "Structs.hpp"

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
		bool				clientAdd( int serverSocket, std::vector<Client*>& clients, std::vector<struct pollfd>& fds);
		// bool				clientRecv( char *recv);
		// void				clientAdd( void );
		bool				clientRecv( struct ServerInfo& serverInfo );
		// bool				clientRecv( void );

		void				clearInput( void );
};



bool	commandParser( std::string&, Client&, struct ServerInfo& serverInfo);

#endif // !CLIENT_HPP
