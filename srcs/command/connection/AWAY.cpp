#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void	IRCCommand::AWAY()
{
	if (parameters.size() == 1)
	{
		user->setAwayMessage("");
		user->deleteUserMode('a');
		return reply(RPL_UNAWAY);
	}
	user->setAwayMessage(parameters[1]);
	user->addUserMode('a');
	return reply(RPL_NOWAWAY);
}