#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
class Client {
	private:
		std::string	  nickname;
		std::string	  username;
		bool		  isOperator; // true if the client is an operator and not a regular user

	public:
		Client( void );
		Client(const Client& src);
		Client &operator=(const Client &);
		~Client();

		//::::::::::setters and getters::::::::::
		void		setNickname( std::string );
		void		setUsername( std::string );
		void		setType( bool );
		bool		getType( void );
		std::string	getUsername( void );
		std::string	getNickname( void );

};

#endif // !CLIENT_HPP
