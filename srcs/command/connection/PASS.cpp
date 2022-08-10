#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void	IRCCommand::PASS()
{
	if (parameters.size() <= 1)
		return reply(ERR_NEEDMOREPARAMS, "PASS");
    if (user->getStatus() != PASSWORD)
		return reply(ERR_ALREADYREGISTRED);
	if (parameters[1] != server->getConfig("password"))
	{
		user->setStatus(DELETE);
		return reply(ERR_PASSWDMISMATCH);
	}
	user->setStatus(REGISTER);
}