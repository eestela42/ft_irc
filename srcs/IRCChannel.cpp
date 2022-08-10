#include "IRCUser.hpp"
#include "IRCChannel.hpp"
#include <sstream>
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

IRCChannel::IRCChannel()
{

}

IRCChannel::IRCChannel(std::string name, IRCUser* User)
{
	if (name[0] == '+')
		addChanMode(name[0]);
	else
		this->addChanUser(User, "o");

	this->setChanNamespace(name);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCChannel::~IRCChannel()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void IRCChannel::write(std::string msg)
{
	for (std::map<IRCUser *, std::string>::iterator it = chanUsers.begin(); it != chanUsers.end(); ++it)
	{
		it->first->write(msg);
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void IRCChannel::addChanMode(char newMode)
{
	if(chanModes.find(newMode) == std::string::npos)
		chanModes.push_back(newMode);
}

void IRCChannel::deleteChanMode(char newMode)
{
	size_t n;

	n = chanModes.find(newMode);
	if(n != std::string::npos)
		chanModes.erase(n, 1);
}

std::string IRCChannel::getChanMode(void)
{
	return (chanModes);
}

bool IRCChannel::hasMode(std::string str)
{
	return (chanModes.find(str) != std::string::npos);
}

std::set<std::string>& IRCChannel::getInviteMasks()	{
	return inviteMasks;
}
void	IRCChannel::addInvited(IRCUser *newUser)
{
	invitedUsers.insert(newUser);
}
bool IRCChannel::isInvited(IRCUser* user)
{
	if (invitedUsers.find(user) != invitedUsers.end())
		return (true);
	for (std::set<std::string>::iterator it = inviteMasks.begin(); it != inviteMasks.end(); ++it)
	{
		if (match_mask(user->getHostname(), *it))
			return (true);
	}
	return (false);
}
void	IRCChannel::addInviteMask(std::string mask)
{
	inviteMasks.insert(mask);
}
void	IRCChannel::delInviteMask(std::string mask)
{
	inviteMasks.erase(mask);
}

std::set<std::string>& IRCChannel::getBannedMasks()	{
	return bannedMasks;
}
bool IRCChannel::isBanned(IRCUser* User)
{
	for (std::set<std::string>::iterator it = bannedMasks.begin(); it != bannedMasks.end(); ++it)
	{
		if (match_mask(User->getHostname(), *it))
			return (true);
	}
	return (false);
}
void	IRCChannel::addBannedMask(std::string mask)
{
	bannedMasks.insert(mask);
}
void	IRCChannel::delBannedMask(std::string mask)
{
	bannedMasks.erase(mask);
}


void IRCChannel::addChanUser(IRCUser *newUser, std::string rights)
{
	for (std::map<IRCUser*, std::string>::iterator it = chanUsers.begin(); it != chanUsers.end(); ++it)
	{
		if (it->first == newUser)
			return;
	}
	chanUsers.insert(make_pair(newUser, rights));
}
void IRCChannel::deleteChanUser(IRCUser *newUser) { chanUsers.erase(newUser);}
std::map<IRCUser*, std::string> IRCChannel::getChanUsers(void){ return(chanUsers); }


void IRCChannel::addPrivilege(IRCUser* user, char p)
{
	std::string& privileges = chanUsers[user];

	if (privileges.find(p) == std::string::npos)
		privileges.push_back(p);
}

void IRCChannel::deletePrivilege(IRCUser* user, char p){
	std::string& privileges = chanUsers[user];
	size_t n;

	n = privileges.find(p);
	if(n != std::string::npos)
		privileges.erase(n, 1);
}

bool	IRCChannel::hasPrivilege(IRCUser* user, char p)
{

	std::map<IRCUser*, std::string>::iterator it = chanUsers.find(user);
	
	if (it == chanUsers.end())
		return false;

	std::string& privileges = it->second;

	return (privileges.find(p) != std::string::npos);
}


void IRCChannel::setTopic(std::string newTopic)
{
	chanTopic = newTopic;
}

std::string IRCChannel::getTopic(void)
{
	return(chanTopic);
}



void IRCChannel::setChanNamespace(std::string newName)
{
	chanNamespace = newName;
}

std::string IRCChannel::getChanNamespace(void)
{
	return(chanNamespace);
}


void IRCChannel::setUserLimit(unsigned long newUserLimit)
{
	userLimit = newUserLimit;
}

unsigned long IRCChannel::getUserLimit(void)
{
	return(userLimit);
}

void IRCChannel::setChanKey(std::string newKey)
{
	chanKey = newKey;
}

std::string IRCChannel::getChanKey(void)
{
	return(chanKey);
}

std::string IRCChannel::getModeParams(void){
	std::stringstream res;

	for (size_t i = 0; i < chanModes.size(); i++)
	{
		if (chanModes[i] == 'k')
			res << " " + chanKey;
		else if (chanModes[i] == 'l')
			res << " " << userLimit;
		else if (chanModes[i] == 'I')
		{
			std::set<std::string>::iterator it = inviteMasks.begin();
			if (it != inviteMasks.end())
				res << " " << *it;
			for (it++; it != inviteMasks.end(); ++it)
					res << "," << *it;
		}
		else if (chanModes[i] == 'b')
		{
			std::set<std::string>::iterator it = bannedMasks.begin();
			if (it != bannedMasks.end())
				res << " " << *it;
			for (it++; it != bannedMasks.end(); ++it)
					res << "," << *it;
		}
	}
	return (res.str());
}

/* ************************************************************************** */