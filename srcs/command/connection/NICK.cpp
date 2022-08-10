#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

static bool	is_nick(std::string nick)
{
	if (!is_letter(nick[0]) && !is_special(nick[0]))
		return (false);
	for (int i = 1; nick[i]; i++)
		if (nick[i] != '-' && !is_letter(nick[i]) && !is_special(nick[i]) && !is_digit(nick[i]))
			return (false);
	return (true);
}

void	IRCCommand::NICK()
{
	if (parameters.size() == 1)
		return reply(ERR_NONICKNAMEGIVEN);
	
	if (user->hasMode("r"))
		return reply(ERR_RESTRICTED);

	std::string nickname = parameters[1];
	if (!is_nick(nickname))
		return reply(ERR_ERRONEUSNICKNAME, nickname.c_str());

	if (server->isUnavailableNickname(nickname))
		return reply(ERR_UNAVAILRESOURCE, nickname.c_str());
		
	std::vector<IRCUser *> users = server->getUsers();
	for (size_t i = 0; i < users.size(); i++)
		if (users[i]->getNickname() == nickname)
			return reply(ERR_NICKNAMEINUSE, nickname.c_str());

	user->setNickname(nickname);
	if (user->getUsername() != "")
	{
		user->setStatus(ONLINE);
		reply(RPL_WELCOME, user->getNickname().c_str(), user->getUsername().c_str(), user->getHostname().c_str());
		reply(RPL_YOURHOST, SERVERNAME, "1.0");
		reply(RPL_CREATED, "today");
		reply(RPL_MYINFO, SERVERNAME, "1.0", "*", "*");
	}
}