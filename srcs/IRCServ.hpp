#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# include <iostream>
# include <string>
# include "IRCConfig.hpp"
# include "IRCUser.hpp"
# include "IRCChannel.hpp"
# include "command/IRCCommand.hpp"
# include "main.hpp"

class IRCServ
{

	public:

		IRCServ(std::string port, std::string password);
		~IRCServ();
		
		void init();
		void execute();
		void sendPing(std::vector<IRCUser *> v_users);

		void acceptUser();
		void delUser(IRCUser &user);

		std::string getPrefix();

		std::string					getConfig(std::string key);
		std::vector<IRCUser *> 		getUsers(void);
		IRCUser* 					getUser(std::string username);
		std::vector<IRCUser *>		getUserByMask(std::string mask);

		std::vector<IRCChannel *>& 	getChannels(void);
		IRCChannel*    				getChannel(std::string chanName);
		void 						addChannel(IRCChannel *chan);
		void 						deleteChannel(IRCChannel *chan);
		void						addUnavailableNickname(std::string nick);
		bool						isUnavailableNickname(std::string nick);

	private:
		void display();
		IRCServ &		operator=( IRCServ const & rhs );
		
		std::map<int, IRCUser *> 			users;
		std::map<std::string, time_t>		unavailable_nicknames;
		std::vector<std::string>			oldNick;
		std::vector<IRCChannel *> 			channels;
		std::vector<pollfd> 				pollfds;

		IRCConfig					config;
		
		int 						listen_fd;
		time_t 						last_ping; // tout les config("ping") secondes, on envoie un ping

};

#endif /* ********************************************************* IRCSERV_H */