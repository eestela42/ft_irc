#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"

void    IRCCommand::INVITE()
{
    const std::vector<std::string>& param = parameters;

    if (param.size() < 3)
	{
        return reply(ERR_NEEDMOREPARAMS, "INVITE");
	}
    IRCUser *invited = server->getUser(param[1]);
    if (invited == NULL)
        return reply(ERR_NOSUCHNICK, param[1].c_str());
    if (invited->hasMode("a"))
        return reply(RPL_AWAY, invited->getNickname().c_str(), invited->getAwayMessage().c_str());

    IRCChannel* channel = server->getChannel(param[2]);
	
    if (channel)
    {
        if (!user->isOnChan(channel))
            return reply(ERR_NOTONCHANNEL, channel->getChanNamespace().c_str());
        if (channel->hasMode("i") && !user->isOper(channel))
            return reply(ERR_CHANOPRIVSNEEDED, channel->getChanNamespace().c_str());
        if (invited->isOnChan(channel))
            return reply(ERR_USERONCHANNEL, invited->getUsername().c_str(), channel->getChanNamespace().c_str());
        channel->addInvited(invited);
    }
    user->sendTo(*invited, "INVITE " + parameters[1] + " " + parameters[2]);
    return reply(RPL_INVITING, invited->getNickname().c_str(), channel->getChanNamespace().c_str());
}


