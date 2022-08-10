#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"
#include <algorithm>

std::string getUserList(IRCChannel *channel, std::vector<IRCUser *>& undisplayedUsers)
{
	std::string userStr = "";
	std::map<IRCUser *, std::string> chanUsers = channel->getChanUsers();
	for (std::map<IRCUser *, std::string>::iterator it = chanUsers.begin(); it != chanUsers.end(); ++it)
	{
		if (userStr != "")
			userStr += " ";
		if ((it->first)->isOper(channel))
			userStr += "@";
		else if (channel->hasMode("m") && channel->hasPrivilege(it->first, 'v'))
			userStr += "+";
		userStr += it->first->getNickname();

		std::vector<IRCUser *>::iterator it2 = std::find(undisplayedUsers.begin(), undisplayedUsers.end(), it->first);
		if (it2 != undisplayedUsers.end())
			undisplayedUsers.erase(it2);
	}
	return userStr;
}

std::string	chanStatus(IRCChannel *chan)
{
	if (!chan)
		return ("");
	if (chan->hasMode("p"))
		return ("*");
	return ("=");

}

void	IRCCommand::NAMES()
{
	std::vector<IRCUser *> undisplayedUsers;

	if (parameters.size() == 1)
	{
		undisplayedUsers = server->getUsers();
		
		std::vector<IRCChannel *> channels = server->getChannels();
		for (size_t i = 0; i < channels.size(); i++)
			reply(RPL_NAMREPLY, chanStatus(channels[i]).c_str(), channels[i]->getChanNamespace().c_str(), getUserList(channels[i], undisplayedUsers).c_str());

		std::string userStr = "";
		for (std::vector<IRCUser *>::iterator it = undisplayedUsers.begin(); it != undisplayedUsers.end(); ++it)
		{
			if (userStr != "")
				userStr += " ";
			userStr += (*it)->getNickname();
		}
		reply(RPL_NAMREPLY, "=","*", userStr.c_str());
		return reply(RPL_ENDOFNAMES, "*", "End of NAMES list");
	}

	const std::vector<std::string> chan_names = split(parameters[1], ",");
	IRCChannel *tmpChan;

	for (size_t i = 0; i < chan_names.size(); i++)
	{
		tmpChan = server->getChannel(chan_names[i]);
		if (tmpChan != NULL)
		{
			std::string userStr = getUserList(tmpChan, undisplayedUsers);
			reply(RPL_NAMREPLY, chanStatus(tmpChan).c_str(), chan_names[i].c_str(), userStr.c_str());
		}
		reply(RPL_ENDOFNAMES, chan_names[i].c_str());
	}

}