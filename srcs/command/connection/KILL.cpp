#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void	IRCCommand::KILL()
{
	if (parameters.size() < 3)
		return (reply(ERR_NEEDMOREPARAMS, "KILL"));
	if (!user->hasMode("o"))
		return (reply(ERR_NOPRIVILEGES));
	IRCUser	*tmpUser = server->getUser(parameters[1]);
	if (!tmpUser)
		return (reply(ERR_NOSUCHNICK, parameters[1].c_str()));
	tmpUser->setStatus(DELETE);
	server->addUnavailableNickname(parameters[1]);
}