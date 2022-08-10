#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"
#include "../../utils.hpp"

void    IRCCommand::TOPIC()
{
    if (getParams().size() <= 1)
        return reply(ERR_NEEDMOREPARAMS, "TOPIC");
    IRCChannel *channel = server->getChannel(getParams()[1]);
    if (!channel)
        return reply(ERR_NOSUCHCHANNEL, parameters[1].c_str());
    if (channel->hasMode("+"))
        return reply(ERR_NOCHANMODES, parameters[1].c_str());

    if (parameters.size() > 2)
    {
        if (channel->hasMode("t") && !user->isOper(channel))
            return reply(ERR_CHANOPRIVSNEEDED, parameters[1].c_str());
        channel->setTopic(getParams()[2]);
    }

    if (channel->getTopic().empty())
        return reply(RPL_NOTOPIC, getParams()[1].c_str());
    return reply(RPL_TOPIC, getParams()[1].c_str(), channel->getTopic().c_str());
}