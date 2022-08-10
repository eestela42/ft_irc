#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"
#include "../../IRCChannel.hpp"


void	IRCCommand::LIST()
{
    if (parameters.size() == 1)
    {
        std::vector<IRCChannel *> channels = server->getChannels();
        for (size_t i = 0; i < channels.size(); i++)
		{
			if (!channels[i]->hasMode("p") || user->isOnChan(channels[i]))
	            reply(RPL_LIST, channels[i]->getChanNamespace().c_str(), channels[i]->getChanMode().c_str(), channels[i]->getTopic().c_str());
		}
        return reply(RPL_LISTEND, "End of LIST");
    }

    const std::vector<std::string> chan_names = split(parameters[1], ",");
	IRCChannel *tmpChan;

	for (size_t i = 0; i < chan_names.size(); i++)
	{
		tmpChan = server->getChannel(chan_names[i]);
		if (tmpChan != NULL)
		{
			reply(RPL_LIST, chan_names[i].c_str(), tmpChan->getChanMode().c_str(), "");
		}
	}
    reply(RPL_LISTEND);
}