#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"
#include "../../utils.hpp"

			             	
void	IRCCommand::KICK()
{
	if (parameters.size() < 3)
		return reply(ERR_NEEDMOREPARAMS, "KICK");

	const std::vector<std::string> chan_names = split(parameters[1], ",");
	const std::vector<std::string> users = split(parameters[2], ",");
	size_t n;
	IRCChannel *tmpChan;

	n = chan_names.size();
	std::string message;
	if (parameters.size() > 3)
		message = " :" + parameters[3];
	if (n == 1)
	{
		tmpChan = server->getChannel(chan_names[0]);
		if (tmpChan == NULL)
			return reply(ERR_NOSUCHCHANNEL, chan_names[0].c_str());
		if (!user->isOnChan(tmpChan))
			return reply(ERR_NOTONCHANNEL, chan_names[0].c_str());
		if (!user->isOper(tmpChan) || tmpChan->hasMode("+"))
			return reply(ERR_CHANOPRIVSNEEDED, chan_names[0].c_str());
		for (size_t i = 0; i < users.size(); i++)
		{
			IRCUser	*tmpUser = server->getUser(users[i]);
			if (!tmpUser || !tmpUser->isOnChan(tmpChan))
				return reply(ERR_USERNOTINCHANNEL, users[i].c_str(), chan_names[0].c_str() );
			user->sendTo(*tmpChan, "KICK " + tmpChan->getChanNamespace() + " " + tmpUser->getNickname() + message);
			tmpChan->deleteChanUser(tmpUser);
		}
	}
	else if (n > 1 && chan_names.size() == users.size())
	{
		for (size_t i = 0; i < chan_names.size(); i++)
		{
			tmpChan = server->getChannel(chan_names[i]);
			if (tmpChan == NULL)	{
				reply(ERR_NOSUCHCHANNEL, chan_names[i].c_str());
				continue ;
			}

			if (!user->isOnChan(tmpChan))	{
				reply(ERR_NOTONCHANNEL, chan_names[i].c_str());
				continue ;
			}
			if (!user->isOper(tmpChan) || tmpChan->hasMode("+"))	{
				reply(ERR_CHANOPRIVSNEEDED, chan_names[i].c_str());
				continue ;
			}
			IRCUser	*tmpUser = server->getUser(users[i]);
			if (!tmpUser || !tmpUser->isOnChan(tmpChan))	{
				return reply(ERR_USERNOTINCHANNEL, users[i].c_str(), chan_names[i].c_str());
				continue ;
			}
			user->sendTo(*tmpChan, "KICK " + tmpChan->getChanNamespace() + " " + tmpUser->getNickname() + message);
			tmpChan->deleteChanUser(tmpUser);
		}
	}
	else
		return reply(ERR_NEEDMOREPARAMS, "KICK");
	return;
}
