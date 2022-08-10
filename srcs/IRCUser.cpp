#include "IRCUser.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

IRCUser::IRCUser(int my_fd, struct sockaddr_in 	my_address)
: fd(my_fd), address(my_address) {
	status = PASSWORD;
	nickname = "";
	username = "";
	realname = "";
	userModes = "";
	declareActive();
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCUser::~IRCUser() {}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void IRCUser::write(std::string msg) { waitingToSend.push_back(msg); }

void IRCUser::sendTo(IRCUser &dest, std::string msg)
{
	dest.write(getPrefix() + msg);
}

void IRCUser::sendTo(IRCChannel& channel, std::string msg, bool myself)
{
	std::map<IRCUser *, std::string> chanUsers = channel.getChanUsers();

	for (std::map<IRCUser *, std::string>::iterator it = chanUsers.begin(); it != chanUsers.end(); ++it)
	{
		if (myself || it->first != this)
			it->first->write(getPrefix(&channel) + msg);
	}
}

void IRCUser::receive(IRCServ *server)
{
	char buf[BUFFER_SIZE];
	int numbytes;
	
	if ((numbytes = recv(fd, buf, BUFFER_SIZE - 2, 0)) == -1) 
		exit_on_error(1, "recv");
	if (numbytes == 0)	// Client disconnected
	{
		setStatus(DELETE);
		return;
	}
	declareActive();
	buf[numbytes] = '\0';
	buffer += buf;
	std::string delimiter(MESSAGE_END);
	size_t position;
	/* Lecture message par message */
	while ((position = buffer.find(delimiter)) != std::string::npos)
	{
		std::string message = buffer.substr(0, position);
		// The substring is the portion of the object that starts at character position pos and spans len characters
		buffer.erase(0, position + delimiter.length());
		/* Prise en compte des messages vides */
		if (!message.length())
			continue;
		commands.push_back(new IRCCommand(this, server, message));
	}

}


void IRCUser::execute()
{
	for (std::deque<IRCCommand*>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		(*it)->exec_command();
		delete *it;
	}
	commands.clear();
}

void IRCUser::push(void)
{
	if (!waitingToSend.size())
	 	return;

	std::string buf;
	for (std::vector<std::string>::iterator it = waitingToSend.begin(); it != waitingToSend.end(); ++it)
	{
		if (DEBUG)
			std::cout << fd << " > " << *it << std::endl;
		buf += *it + MESSAGE_END;
	}
	waitingToSend.clear();

	if (buf.length())	{
		if (send(fd, buf.c_str(), buf.length(), 0) == -1)
			exit_on_error(1, "send");
	}
}
bool	IRCUser::hasChanInCommon(IRCUser *user, IRCServ *server)
{
	std::vector<IRCChannel *> chans = server->getChannels();

	for (std::vector<IRCChannel *>::iterator	it = chans.begin(); it != chans.end(); it++)
		if (isOnChan(*it) && user->isOnChan(*it))
			return (true);
	return (false);
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* Getters */
user_status &IRCUser::getStatus(){return (status);}
int &IRCUser::getFd(){return (fd);}
 
std::string 	IRCUser::getNickname() const { return nickname; }
std::string		IRCUser::getHostname() const { return inet_ntoa(address.sin_addr); }
std::string 	IRCUser::getRealname() const { return realname; }
std::string 	IRCUser::getUsername() const { return username; }

void 	IRCUser::setNickname(const std::string &nickname_) { nickname = nickname_; }
void 	IRCUser::setRealname(const std::string &realname_) { realname = realname_; }
void 	IRCUser::setUsername(const std::string &uname) { username = uname; }
void	IRCUser::setStatus(user_status s)	{	status = s;	}

std::string	IRCUser::getPrefix(IRCChannel* channel)
{
	if (this->status == PASSWORD || this->status == REGISTER)
		return("");
	std::string prefix = ":" + this->nickname;
	
	
	if (prefix.length() && this->getHostname().length())
	{
		if (this->username.length())
		{
			prefix += "!";
			if (channel != NULL && isOper(channel))
				prefix += "@";
			else if (channel != NULL && channel->hasPrivilege(this, 'v'))
				prefix += "+";
			prefix += this->username;
		}
		prefix += "@" + this->getHostname();
	}
	return (prefix + " ");
}

void IRCUser::addUserMode(char newMode)
{
	if(userModes.find(newMode) == std::string::npos)
		userModes.push_back(newMode);
}

void IRCUser::deleteUserMode(char newMode)
{
	size_t n;

	n = userModes.find(newMode);
	if (n != std::string::npos)
		userModes.erase(n, 1);
}

std::string IRCUser::getUserModes(void)
{
	return(userModes);
}

bool IRCUser::hasMode(std::string str)
{
	return (userModes.find(str) != std::string::npos);
}


bool	IRCUser::isOper(IRCChannel *channel)
{
	if (hasMode("r"))
		return (false);
	return (channel->hasPrivilege(this, 'o'));
}

bool	IRCUser::isOnChan(IRCChannel *channel)
{
	std::map<IRCUser*, std::string>::iterator it;
	std::map<IRCUser*, std::string> chanUsers = channel->getChanUsers();
	it = chanUsers.find(this);
	return (it != chanUsers.end());
}

std::string	IRCUser::getAwayMessage() const
{	return (away_message);}
void		IRCUser::setAwayMessage(const std::string &message)
{	away_message = message;}


void	IRCUser::declareActive()
{
	last_time_active = time(NULL);
	last_ping = last_time_active;
}
int	IRCUser::time_inactive()
{
	return (time(NULL) - last_time_active);
}

/* ************************************************************************** */

std::map<std::string, void (*)(IRCCommand *)> initCommandFunctionsMap()
{
	return std::map<std::string, void (*)(IRCCommand *)>();
}

std::map<std::string, void (*)(IRCCommand *)> IRCUser::MapCommands = initCommandFunctionsMap();
