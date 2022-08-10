#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"

/*
CHANNEL MODES :
for users :
	- O : channel creator
	- o : channel operator
	- v : voice
for channels :
	- i : invite only
	- m : moderated
	- n : outside privmsg disabled
	- p : private
	- t : topic locked
	- k : key locked
	- l : user limit
*/

void    user_MODE(IRCCommand* command)
{

	if (command->getParams()[1] != command->getUser().getNickname())
		return command->reply(ERR_USERSDONTMATCH);

	std::string plus_flags = "iwr";
	std::string minus_flags = "iwo";
	
	std::string arg = command->getParams()[2];
	int sign = 0;

	for (size_t i = 0; i < arg.length(); i++)
	{
		if (arg[i] == '+' || arg[i] == '-')
			sign = (arg[i] == '+') ? 1 : -1;
		else if (sign == 1 && plus_flags.find(arg[i]) != std::string::npos)
		{
			command->getUser().addUserMode(arg[i]);
		}
		else if (sign == -1 && minus_flags.find(arg[i]) != std::string::npos)
		{
			command->getUser().deleteUserMode(arg[i]);
		}
		else
			command->reply(ERR_UMODEUNKNOWNFLAG);
	}
	return command->reply(RPL_UMODEIS, command->getUser().getUserModes().c_str());
}

void    channel_MODE(IRCCommand* command)
{
	IRCChannel* channel = command->getServer().getChannel(command->getParams()[1]);

	if (channel == NULL)
		return command->reply(ERR_NOSUCHNICK, command->getParams()[1].c_str());
	if (channel->hasMode("+"))
		return command->reply(ERR_NOCHANMODES, command->getParams()[1].c_str());
	if (!command->getUser().isOper(channel))
		return command->reply(ERR_CHANOPRIVSNEEDED, command->getParams()[1].c_str());
	if (command->getParams().size() < 3)
		return command->reply(RPL_CHANNELMODEIS, command->getParams()[1].c_str(), channel->getChanMode().c_str(), channel->getModeParams().c_str());

	std::string right_flags = "ov";
	std::string channel_flags = "Iimnptklb";
	std::vector<std::string> params = command->getParams();

	for (size_t p = 2; p < params.size(); p++)
	{
		std::string arg = params[p];
		int sign = 0;

		for (size_t i = 0; i < arg.length(); i++)
		{
			if (arg[i] == '+' || arg[i] == '-')
				sign = (arg[i] == '+') ? 1 : -1;
			else if (right_flags.find(arg[i]) != std::string::npos && sign != 0 && p+1 < params.size())
			{
				p++;
				IRCUser* user = command->getServer().getUser(params[p]); 
				if (user == NULL || !user->isOnChan(channel))
				{
					command->reply(ERR_USERNOTINCHANNEL, params[p].c_str(), params[1].c_str());
					continue;
				}
				if (sign == 1)
					channel->addPrivilege(user, arg[i]);
				else if (sign == -1) // a completer
					channel->deletePrivilege(user, arg[i]);
			}
			else if (channel_flags.find(arg[i]) != std::string::npos && sign == 1)
			{
				if ((std::string("Ibl").find(arg[i]) != std::string::npos && p+1 >= params.size()))
				{
					command->reply(ERR_NEEDMOREPARAMS, "MODE");
					continue;
				}
				if (arg[i] == 'I')
					channel->addInviteMask(params[++p]);
				else if (arg[i] == 'b')
					channel->addBannedMask(params[++p]);
				else if (arg[i] == 'l')
					channel->setUserLimit(atoi(params[++p].c_str()));
				else if (arg[i] == 'k')
				{
					++p;
					if (!channel->hasMode("k"))
						channel->setChanKey(params[++p]);
					else
					{
						command->reply(ERR_KEYSET, command->getParams()[1].c_str());    
						continue;
					}
				}
				channel->addChanMode(std::tolower(arg[i]));
			}
			else if (channel_flags.find(arg[i]) != std::string::npos && sign == -1)
			{
				if ((std::string("Ib").find(arg[i]) != std::string::npos && p+1 >= params.size()))
				{
					command->reply(ERR_NEEDMOREPARAMS, "MODE");
					continue;
				}
				if (arg[i] == 'I')
					channel->delInviteMask(params[++p]);
				else if (arg[i] == 'b')
					channel->delBannedMask(params[++p]);
				channel->deleteChanMode(arg[i]);
			}
			else if (sign == 0 && arg[i] == 'I')
			{
				for (std::set<std::string>::iterator it = channel->getInviteMasks().begin(); it != channel->getInviteMasks().end(); it++)
					command->reply(RPL_INVITELIST, command->getParams()[1].c_str(), (*it).c_str());
				command->reply(RPL_ENDOFINVITELIST, command->getParams()[1].c_str());
			}
			else if (sign == 0 && arg[i] == 'b')
			{
				for (std::set<std::string>::iterator it = channel->getBannedMasks().begin(); it != channel->getInviteMasks().end(); it++)
					command->reply(RPL_BANLIST, command->getParams()[1].c_str(), (*it).c_str());
				command->reply(RPL_ENDOFBANLIST, command->getParams()[1].c_str());
			}
			else
				command->reply(ERR_UNKNOWNMODE, &arg[i], command->getParams()[1].c_str());
		}
	}
	command->getUser().sendTo(*channel, command->getMessage());
	
}



void    IRCCommand::MODE()
{
	if (parameters.size() < 2)
		return reply(ERR_NEEDMOREPARAMS, "MODE");
	if (parameters.size() >= 3 && server->getUser(parameters[1]))
		user_MODE(this);
	else
		channel_MODE(this);

}