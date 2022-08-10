#ifndef IRCUSER_HPP
# define IRCUSER_HPP

# include <iostream>
# include <string>
# include "main.hpp"
# include "IRCConfig.hpp"
# include "command/IRCCommand.hpp"
# include "IRCServ.hpp"

class IRCServ;
class IRCCommand;
class IRCChannel;

enum user_status
{
	PASSWORD,
	REGISTER,
	ONLINE,
	DELETE
};


class IRCUser
{

	public:

		IRCUser(int my_fd, struct sockaddr_in 	my_address);
		~IRCUser();

		void receive(IRCServ *server);
		void execute();
		void push(void);
		void sendTo(IRCUser &toUser, std::string message);
		void sendTo(IRCChannel& channel, std::string msg, bool myself = 1);
		void write(std::string msg);
		bool hasChanInCommon(IRCUser *user, IRCServ *server);



		void	setStatus(user_status s);

		user_status &getStatus();
		int &getFd();
		std::map<std::string, void (*)(IRCCommand *)> &getAllCommandFunctions();
		std::string	getPrefix(IRCChannel* channel = NULL);

		std::string getNickname() const;
		void setNickname(const std::string &nick);

		std::string getHostname() const;

		void setUsername(const std::string &uname);
		std::string getUsername() const;

		std::string getRealname() const;
		void setRealname(const std::string &nick);

		void addUserMode(char newMode);
		void deleteUserMode(char newMode);
		std::string getUserModes(void);
		bool hasMode(std::string str);

		bool	isOnChan(IRCChannel *channel);
		bool	isOper(IRCChannel *channel);

		std::string	getAwayMessage() const;
		void	setAwayMessage(const std::string &message);

		void	declareActive();
		int		time_inactive();
		time_t	last_ping;

	private:
		static std::map<std::string, void (*)(IRCCommand *)> MapCommands;
		
		
		IRCUser();
		IRCUser( IRCUser const & src );
		IRCUser &		operator=( IRCUser const & rhs );
		
	/* Variables */
		int 								fd;
		struct sockaddr_in 					address;
		std::string 						buffer;
		std::deque <IRCCommand *>			commands;
		user_status							status;
		std::string							userModes;


		std::string							nickname;
		std::string							username;
		std::string							realname;

		std::string							away_message;
		time_t								last_time_active;
		std::vector<std::string> 			waitingToSend;

		
};


#endif /* ********************************************************* IRCUSER_H */