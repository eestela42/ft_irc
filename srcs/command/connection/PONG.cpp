#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void IRCCommand::PONG()
{
	if (!parameters.size())
		return reply(ERR_NOORIGIN);
}