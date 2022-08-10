#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"
#include "../../utils.hpp"

bool    is_channel_name(std::string str)
{
	if (str.length() > 50 || (str[0] != '&' && str[0] != '#' && str[0] != '+') ||\
		str.find(" ") != std::string::npos || str.find("^G") != std::string::npos ||\
		str.find(",") != std::string::npos)
		return (0);
	return (1);
}

void	IRCCommand::JOIN()
{
	if (parameters.size() < 2)
		return reply(ERR_NEEDMOREPARAMS, "JOIN");
	const std::vector<std::string> chan_names = split(parameters[1], ",");
	std::vector<std::string> chan_keys;
	if (parameters.size() > 2)
		chan_keys = split(parameters[2], ",");
	size_t n;
	IRCChannel *tmpChan;
	std::vector<IRCUser*> trash;

	if (chan_names.size() == 0)
	{
		return reply(ERR_NEEDMOREPARAMS, "JOIN");
	}
	if (parameters.size() == 2 && parameters[1] == "0")
	{
		parameters[1] = "";
		for (size_t i = 0; i < server->getChannels().size(); i++)
		{
			if (user->isOnChan(server->getChannels()[i]))
			{
				if (parameters[1] != "")
					parameters[1] += ",";
				parameters[1] += server->getChannels()[i]->getChanNamespace();
			}
		}
		PART();
		return;
	}
	n = chan_names.size();
	for (size_t i = 0; i < n; i++)
	{
		if (is_channel_name(chan_names[i]) == 0)
			return reply(ERR_NOSUCHCHANNEL, chan_names[i].c_str()); //return ?
		
		tmpChan = server->getChannel(chan_names[i]); 
		if (tmpChan)
		{
			if (tmpChan->hasMode("k") && (parameters.size() <= 2 || chan_keys[i] != tmpChan->getChanKey()))
				return reply(ERR_BADCHANNELKEY, tmpChan->getChanNamespace().c_str());
			if (tmpChan->hasMode("b") && tmpChan->isBanned(&this->getUser()) && !user->isOper(tmpChan))
				return reply(ERR_BANNEDFROMCHAN, tmpChan->getChanNamespace().c_str());
			if (tmpChan->hasMode("l") && tmpChan->getChanUsers().size() >= tmpChan->getUserLimit())
				return reply(ERR_CHANNELISFULL, tmpChan->getChanNamespace().c_str());
			if (tmpChan->hasMode("i") && !tmpChan->isInvited(&this->getUser()))
				return reply(ERR_INVITEONLYCHAN, tmpChan->getChanNamespace().c_str());
			tmpChan->addChanUser(user, "");
		}
		else
		{
			/* Gérer les différents cas des 4 préfixes avec ! et + notammment */
			tmpChan = new IRCChannel(chan_names[i], user); // Delete dans PART
			server->addChannel(tmpChan);
		}
		
		user->sendTo(*tmpChan, "JOIN "+chan_names[i]);

		if (tmpChan->getTopic().length() > 0)
			reply(RPL_TOPIC, tmpChan->getChanNamespace().c_str(), tmpChan->getTopic().c_str());
		reply(RPL_NAMREPLY, chanStatus(tmpChan).c_str(), tmpChan->getChanNamespace().c_str(), getUserList(tmpChan, trash).c_str());
	}
	return;
}
