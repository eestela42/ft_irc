#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void	channel_privmsg_notice(IRCCommand* command)
{
	IRCChannel* channel = command->getServer().getChannel(command->getParams()[1]);

	if ((channel->hasMode("m") && !channel->hasPrivilege(&command->getUser(), 'v')
			&& !channel->hasPrivilege(&command->getUser(), 'o'))
			|| (channel->hasMode("n") && !command->getUser().isOnChan(channel)))
		return ;
	command->getUser().sendTo(*channel, "NOTICE "+command->getParams()[1]+" :"+command->getParams()[2], 0);

}

void	user_privmsg_notice(IRCCommand* command, IRCUser &dest)
{
	if (dest.hasMode("a"))
		return;
	command->getUser().sendTo(dest, "NOTICE "+command->getParams()[1]+" :"+command->getParams()[2]);

}


void	IRCCommand::NOTICE()
{
	if (parameters.size() <= 1)
		return ;
	if (parameters.size() <= 2)
		return ;
	
	std::string dest = parameters[1];

	if (server->getChannel(dest) != NULL)
		channel_privmsg_notice(this);
	else if (dest[0] == '#')
	{
		if (!check_mask(dest, NULL))
			return ;
		if (is_mask(dest) && !user->hasMode("o"))
			return ;
		std::vector<IRCUser *> tab = server->getUserByMask(dest.substr(1));
		for (std::vector<IRCUser *>::iterator it = tab.begin(); it != tab.end(); it++)
			user->sendTo((**it), "NOTICE " + parameters[1] + " :" + parameters[2]);
	}
	else
	{
		IRCUser *tmpUser = server->getUser(dest);
		if (tmpUser == NULL)
			return ;
		user_privmsg_notice(this, *tmpUser);
	}
}
