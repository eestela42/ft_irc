#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"
#include "../../utils.hpp"

void    IRCCommand::PART()
{
	if (parameters.size() <= 1)
		return reply(ERR_NEEDMOREPARAMS, "PART");

	const std::vector<std::string> chan_names = split(parameters[1], ",");
	std::string msg = user->getNickname() + " has left the channel";
	
	if (parameters.size() > 2)
		msg = parameters[2];

	size_t n;
	IRCChannel *tmpChan;

	n = chan_names.size();
	for (size_t i = 0; i < n; i++)
	{
		
		tmpChan = server->getChannel(chan_names[i]); 
		if (tmpChan)
		{
			user->sendTo(*tmpChan, "PART "+chan_names[i]+" :"+msg);
			tmpChan->deleteChanUser(user);
			if (tmpChan->getChanUsers().size() == 0)
				server->deleteChannel(tmpChan);
		}
		else
			reply(ERR_NOSUCHCHANNEL, chan_names[i].c_str());
	}
	return;
}
