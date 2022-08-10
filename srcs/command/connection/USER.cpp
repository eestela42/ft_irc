#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"

void	IRCCommand::USER()
{
	IRCUser &user = this->getUser();

    if (parameters.size() < 5)
		return reply(ERR_NEEDMOREPARAMS, "USER");
    if (user.getStatus() != REGISTER)
		return reply(ERR_ALREADYREGISTRED);	
	if (parameters[1].find('@') != std::string::npos \
		|| parameters[1].find(' ') != std::string::npos \
		|| parameters[1].find('\0') != std::string::npos \
		|| parameters[1].find('\n') != std::string::npos \
		|| parameters[1].find('\r') != std::string::npos \
			)
			return ;
	user.setUsername(parameters[1]);

	
	if (user.getNickname() != "")
	{
		user.setStatus(ONLINE);
		reply(RPL_WELCOME, user.getNickname().c_str(), user.getUsername().c_str(), user.getHostname().c_str());
		reply(RPL_YOURHOST, SERVERNAME, "1.0");
		reply(RPL_CREATED, "today");
		reply(RPL_MYINFO, SERVERNAME, "1.0", "*", "*");
	}
}