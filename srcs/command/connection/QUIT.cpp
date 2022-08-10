#include "../IRCCommand.hpp"
#include "../../IRCUser.hpp"


void	IRCCommand::QUIT()
{
	std::string message;

	if (parameters.size() > 1)
		message = parameters[1];

	std::map<int , IRCUser *>	users;
	for (std::map<int , IRCUser *>::iterator it = users.begin(); it != users.end(); it++)
		user->sendTo(*it->second, "QUIT :" + message); 

	user->write(server->getPrefix()+"ERROR :"+message);
	user->setStatus(DELETE);
}