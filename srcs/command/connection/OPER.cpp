#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void	IRCCommand::OPER()
{
    if (parameters.size() < 3)
        return reply(ERR_NEEDMOREPARAMS, "OPER");
    std::cout << server->getConfig("oper_login") << std::endl;
    if (parameters[1] != server->getConfig("oper_login") \
    || parameters[2] != server->getConfig("oper_password"))
        return reply(ERR_PASSWDMISMATCH);
    user->addUserMode('o');
    return reply(RPL_YOUREOPER);
}