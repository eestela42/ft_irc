#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void IRCCommand::WALLOPS()
{
    if (parameters.size() <= 1)
        return reply(ERR_NEEDMOREPARAMS, "WALLOPS");

    std::vector<IRCUser*> users = server->getUsers();
    for (std::vector<IRCUser*>::iterator it = users.begin(); it != users.end(); it++)
        if ((*it)->hasMode("w"))
            user->sendTo(**it, "WALLOPS :" + parameters[1]);
}