#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <poll.h>
# include <vector>
# include <map>
# include <netinet/in.h>
# include "Structs.hpp"

class Channel;

class Client {
	private:
		std::string				clientNickname;
		std::string				clientUsername;
		std::string				clientRealname;
		bool					clientIsOperator; // true if the client is an operator and not a regular user
		struct s_status			clientStatus;
		struct s_ircCommand		clientInput;
		int						clientSocket;
		int						clientPollFd;
		std::string				clientIpAddress;
		struct sockaddr_in		clientAddress;

		std::map<std::string, Channel*> channels;


	public:
		Client( void );
		Client( const Client& );
		Client& operator=( const Client& );
		~Client( void );

		std::string				buffer;



		//Comparison operator
		bool operator==( const Client& ) const;

		//::::::::Getters and Setters:::::::::::::::
		void					setNickname( std::string& );
		void					setUsername( std::string& );
		void					setRealname( std::string& );
		void					setIpAddress( std::string& );
		void					setType( bool );
		void					setStatus( std::string, bool );

		struct s_status			getStatus( void ) const;
		bool					getType( void ) const;
		std::string				getIpAddress( void ) const;
		std::string				getRealname( void ) const;
		std::string				getUsername( void ) const;
		std::string				getNickname( void ) const;
		std::map<std::string, Channel*>				getChannels( void );

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
		void				channelAdd( Channel& );
		void				channelRemove( std::string );
		// bool				clientRecv( char *recv);
		// void				clientAdd( void );
		bool				clientRecv( struct ServerInfo& serverInfo );
		// bool				clientRecv( void );

		void				clearInput( void );

		void				quitChannel(Channel& channel);
		void				quitAllChannels(struct ServerInfo& serverInfo);
		bool				amIInChannel(std::string& channelName);
};


#endif // !CLIENT_HPP

