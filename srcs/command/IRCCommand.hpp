#ifndef IRCCOMMAND_HPP
# define IRCCOMMAND_HPP

# include <iostream>
# include <string>
# include <vector>
# include <deque>
#include <stdarg.h>
# include "../main.hpp"
# include "../IRCConfig.hpp"
# include "../IRCUser.hpp"
# include "../IRCServ.hpp"
# include "replies.hpp"

class IRCServ;
class IRCCommand;
class IRCUser;

class IRCCommand
{
	static std::map<std::string, void (IRCCommand::*)()> func_map;

public:

	IRCCommand(IRCUser *usr, IRCServ *serv, std::string &msg);
	~IRCCommand();

	void	exec_command();
	void	reply(std::string msg, ...);

	std::vector<std::string>	getParams() const;
	IRCUser						&getUser() const;
	IRCServ						&getServer() const;
	std::string					getMessage() const;

	static void 		init_func_map();

private:
	IRCCommand();
	IRCCommand( IRCCommand const & src );
	IRCCommand &		operator=( IRCCommand const & rhs );
	
	IRCUser *user;
	IRCServ *server;

	std::string prefix;
	std::vector<std::string> parameters;
	std::string message;

	//commands

	void	PASS();
	void	AWAY();
	void	NICK();
	void	USER();
	void	WHOIS();
	void	WHO();
	void	MODE();
	void	PING();
	void	PONG();
	void	JOIN();
	void	KICK();
	void	KILL();
	void	PART();
	void	TOPIC();
	void	INVITE();
	void	NAMES();
	void	NOTICE();
	void	LIST();
	void	PRIVMSG();
	void	WALLOPS();
	void	OPER();
	void	QUIT();


};

std::ostream &			operator<<( std::ostream & o, IRCCommand const & i );


//syntaxe
bool	is_letter(char c);
bool	is_digit(char c);
bool	is_special(char c);

bool    is_channel_name(std::string str);
bool	check_mask(std::string dest, IRCCommand *com);
bool	is_mask(std::string dest);



#endif /* ****************************************************** IRCCOMMAND_H */