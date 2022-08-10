#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"

//	ERR_NOSUCHSERVER
//    RPL_WHOREPLY                  RPL_ENDOFWHO

bool	maskIsAnyWildcard(std::string mask)
{
	if (mask.size() > 1)
		return (false);
	if (mask[0] != '0' && mask[0] != '*' && mask[0] != '?')
		return (false);
	return (true);
}

void	IRCCommand::WHO()
{
	std::string				mask;
	std::string state;
	int found = 0;
	
	if (parameters.size() == 1)
		return ;
	if (parameters.size() >= 2)
		mask = parameters[1];
	if (maskIsAnyWildcard(mask))
	{
		found++;
		std::vector<IRCUser *>	list;
		list = server->getUsers();
		for (std::vector<IRCUser *>::iterator it = list.begin(); it != list.end(); it++)
		{
			if ((*it)->hasMode("i") || (parameters[2] == "o" && !(*it)->hasMode("o")))
				continue ;
			else if (user->hasChanInCommon(*it, server))
				continue ;
			if ((*it)->hasMode("a") != std::string::npos)
					state = "G";
				else
					state = "H";
			if ((*it)->hasMode("o"))
				state += "*";
			reply("RPL_WHOREPLY", "", (*it)->getUsername(), (*it)->getHostname(), getServer().getConfig("name"), (*it)->getNickname(), state, (*it)->getRealname());
		}
	}
	else if (parameters.size() >= 2)
	{
		std::map<IRCUser *, std::string> list;
		std::vector<IRCChannel *> chan = getServer().getChannels();
		for (std::vector<IRCChannel *>::iterator it_c = chan.begin(); it_c != chan.end(); it_c++)
		{
			if (!match_mask((*it_c)->getChanNamespace(), mask))
				continue ;
			found++;
			list = (*it_c)->getChanUsers();
			for (std::map<IRCUser *, std::string>::iterator it = list.begin(); it != list.end(); it++)
			{
				if ((parameters[2] == "o" && !(it->first)->isOper(*it_c)))
					continue ;
				if ((it->first)->hasMode("a") != std::string::npos)
					state = "G";
				else
					state = "H";
				if ((it->first)->hasMode("o"))
					state += "*";
				if ((it->first)->isOper(*it_c))
					state += "@";
				else if ((*it_c)->getChanMode().find("m") && (*it_c)->hasPrivilege((it->first), 'v'))
					state += "+";
				reply("RPL_WHOREPLY", (*it_c)->getChanNamespace(), (it->first)->getUsername(), (it->first)->getHostname(), getServer().getConfig("name"), (it->first)->getNickname(), state, (it->first)->getRealname());
			}
		}
	}
	if (!found)
	{
		std::vector<IRCUser *>	list = getServer().getUsers();
		for (std::vector<IRCUser *>::iterator it = list.begin(); it != list.end(); it++)
		{
			if (!match_mask((*it)->getNickname(), mask) && (parameters[2] != "o" || !(*it)->hasMode("0")))
				continue ;
			found++;
			if ((*it)->hasMode("a") != std::string::npos)
				state = "G";
			else
				state = "H";
			if ((*it)->hasMode("o"))
				state += "*";
			reply("RPL_WHOREPLY", "", (*it)->getUsername(), (*it)->getHostname(), getServer().getConfig("name"), (*it)->getNickname(), state, (*it)->getRealname());
		}
	}
	if (!found)
	{
		std::vector<IRCUser *>	list = getServer().getUsers();
		for (std::vector<IRCUser *>::iterator it = list.begin(); it != list.end(); it++)
		{
			if (!match_mask((*it)->getRealname(), mask) && (parameters[2] == "o" && !(*it)->hasMode("0")))
				continue ;
			found++;
			if ((*it)->hasMode("a")!= std::string::npos)
					state = "G";
			else
				state = "H";
			if ((*it)->hasMode("o"))
				state += "*";
			reply("RPL_WHOREPLY", "", (*it)->getUsername(), (*it)->getHostname(), getServer().getConfig("name"), (*it)->getNickname(), state, (*it)->getRealname());
		}
	}
	if (!found)
	{
		std::vector<IRCUser *>	list = getServer().getUsers();
		for (std::vector<IRCUser *>::iterator it = list.begin(); it != list.end(); it++)
		{
			if (!match_mask((*it)->getUsername(), mask) && (parameters[2] != "o" || !(*it)->hasMode("0")))
				continue ;
			found++;
			if ((*it)->hasMode("a") != std::string::npos)
					state = "G";
			else
				state = "H";
			if ((*it)->hasMode("o"))
				state += "*";
			reply("RPL_WHOREPLY", "", (*it)->getUsername(), (*it)->getHostname(), getServer().getConfig("name"), (*it)->getNickname(), state, (*it)->getRealname());
		}
	}
}