#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void	channel_privmsg(IRCCommand* command)
{
	IRCChannel* channel = command->getServer().getChannel(command->getParams()[1]);

	if ((channel->hasMode("m") && (!channel->hasPrivilege(&command->getUser(), 'v')
			&& !channel->hasPrivilege(&command->getUser(), 'o')))
			|| (channel->hasMode("n") && !command->getUser().isOnChan(channel)))
		return command->reply(ERR_CANNOTSENDTOCHAN, command->getParams()[1].c_str());
	command->getUser().sendTo(*channel, "PRIVMSG "+command->getParams()[1]+" :"+command->getParams()[2], 0);

}

void	user_privmsg(IRCCommand* command, IRCUser &dest)
{
	if (dest.hasMode("a"))
		return command->reply(RPL_AWAY, dest.getNickname().c_str(), dest.getAwayMessage().c_str());
	command->getUser().sendTo(dest, "PRIVMSG "+ command->getParams()[1] + " :" + command->getParams()[2]);

}

bool	check_mask(std::string dest, IRCCommand *com)
{
	if (dest.find('.') == std::string::npos)
	{
		if (com) com->reply(ERR_NOTOPLEVEL);
		return false;
	}
	for (int i = dest.size() - 1; i >= 0; i--)
	{
		if (dest[i] == '.')
			return (true);
		if (dest[i] == '*' || dest[i] == '?')
		{
			if (com) com->reply(ERR_WILDTOPLEVEL);
			return false;
		}	
	}
	return (true);
}

bool	is_mask(std::string dest)
{
	if (dest.find('*') || dest.find('?'))
		return true;
	return false;
}

void	IRCCommand::PRIVMSG()
{
	if (parameters.size() <= 1)
		return reply(ERR_NORECIPIENT, "PRIVMSG");
	if (parameters.size() <= 2)
		return reply(ERR_NOTEXTTOSEND);
	
	std::string dest = parameters[1];

	if (server->getChannel(dest) != NULL)
		channel_privmsg(this);
	else if (dest[0] == '#')
	{
		if (!check_mask(dest, this))
			return ;
		if (is_mask(dest) && !user->hasMode("o"))
			return reply(ERR_NOPRIVILEGES);
		std::vector<IRCUser *> tab = server->getUserByMask(dest.substr(1));
		for (std::vector<IRCUser *>::iterator it = tab.begin(); it != tab.end(); it++)
			user->sendTo((**it), "PRIVMSG " + parameters[1] + " :" + parameters[2]);
	}
	else
	{
		IRCUser *tmpUser = server->getUser(dest);
		if (tmpUser == NULL)
			return reply(ERR_NOSUCHNICK, dest.c_str());
		user_privmsg(this, *tmpUser);
	}
}