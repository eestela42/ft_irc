#ifndef IRCCHANNEL_HPP
# define IRCCHANNEL_HPP

# include <iostream>
# include <string>
# include <map>
# include <set>
# include <vector>
//# include "IRCUser.hpp"

class IRCUser;
class IRCChannel
{

public:

	IRCChannel();
	IRCChannel(std::string name, IRCUser* User);
	~IRCChannel();


	void write(std::string msg);

	void addChanMode(char mode);
	void deleteChanMode(char mode);
	std::string getChanMode(void);
	bool hasMode(std::string str);

	void	addInvited(IRCUser *newUser);
	bool 	isInvited(IRCUser* User);
	void	addInviteMask(std::string mask);
	void	delInviteMask(std::string mask);
	std::set<std::string>& getInviteMasks();

	bool 	isBanned(IRCUser* User);
	void	addBannedMask(std::string mask);
	void	delBannedMask(std::string mask);
	std::set<std::string>& getBannedMasks();

	void addChanUser(IRCUser *newUser, std::string rights);
	void deleteChanUser(IRCUser *newUser);
	std::map<IRCUser *, std::string> getChanUsers(void);

	void addPrivilege(IRCUser* user, char p);
	void deletePrivilege(IRCUser* user, char p);
	bool hasPrivilege(IRCUser* user, char p);

	void setTopic(std::string str);
	std::string getTopic(void);

	void setChanNamespace(std::string str);
	std::string getChanNamespace(void);

	void setUserLimit(unsigned long newUserLimit);
	unsigned long getUserLimit(void);
	
	void setChanKey(std::string newKey);
	std::string getChanKey(void);

	std::string getModeParams(void);


private:

	std::string chanNamespace;
	std::string chanModes;
	std::string chanTopic;
	unsigned long userLimit;
	std::string chanKey;
	std::map<IRCUser *, std::string> chanUsers;
	std::set<IRCUser *> invitedUsers;
	std::set<std::string> inviteMasks;
	std::set<std::string> bannedMasks;

};


std::string getUserList(IRCChannel *channel, std::vector<IRCUser *>& undisplayedUsers);
std::string	chanStatus(IRCChannel *chan);

#endif


