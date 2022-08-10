#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"

void	IRCCommand::WHOIS()
{
	if (parameters.size() == 1)
		return (reply(ERR_NONICKNAMEGIVEN));

	IRCUser *user = server->getUser(parameters[1]);

    if (user == NULL)  
        return (reply(ERR_NOSUCHNICK, parameters[1].c_str()));
    return reply(RPL_WHOISUSER, user->getNickname().c_str(), user->getUsername().c_str(), user->getHostname().c_str(), user->getRealname().c_str());
}