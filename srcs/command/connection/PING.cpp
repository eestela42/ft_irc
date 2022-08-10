#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void IRCCommand::PING()
{
	if (!parameters.size())
		return reply(ERR_NOORIGIN);
	user->write("PONG :" + std::string(SERVERNAME));
}