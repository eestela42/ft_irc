#include "IRCServ.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

IRCServ::IRCServ(std::string port, std::string password)
{
    config.set("port", port);
	config.set("password", password);
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCServ::~IRCServ() {
	for (std::vector<IRCChannel*>::iterator it = channels.begin(); it != channels.end(); ++it)
		delete *it;
	for (std::map<int, IRCUser*>::iterator it = users.begin(); it != users.end(); ++it)
		delete it->second;
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void IRCServ::acceptUser()
{
	struct sockaddr_in 	new_address;
	socklen_t			sin_size; // unsigned int;
	sin_size = sizeof(struct sockaddr_in);	
	
    int new_fd = accept(pollfds[0].fd, (struct sockaddr *)&new_address, &sin_size);
	if (new_fd == -1)
		return;
	
	/* Creation du user */
	users[new_fd] = new IRCUser(new_fd, new_address);

	/* Maj de pollfds */
	pollfds.push_back(pollfd());
	pollfds.back().fd = new_fd;
	pollfds.back().events = POLLIN;
}


void IRCServ::init(){

	int enable = 1;
	/* Create the socket = file descriptor */
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		exit_on_error(1,"socket");
	/* Avoid the "Address already in use"  message */
	if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable)))
		exit_on_error(1,"setsockopt");
	/* Set the socket non-blocking - If you try to read from a non-blocking socket and there’s no data there, it’s not allowed to block—it will return -1 */
	if (fcntl(listen_fd, F_SETFL, O_NONBLOCK) < 0)
		exit_on_error(1,"fcntl");

	struct sockaddr_in address;
	address.sin_family = AF_INET; // IPV4
	address.sin_addr.s_addr = INADDR_ANY; // Any IP
	address.sin_port = htons(atoi(config.get("port").c_str())); // Port
	memset(&(address.sin_zero), '\0', 8); // zero the rest of the struct
	

	/* Associate the socket with the port in fd and list */
	if (bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		exit_on_error(1, "bind");
	if (listen(listen_fd, address.sin_port) < 0)
		exit_on_error(1, "listen");

	/* Initialise la 1ere structure poll fd en ecoutant */
	pollfds.push_back(pollfd());
	pollfds.back().fd = listen_fd; 
	pollfds.back().events = POLLIN; // Alert me when data is ready to recv() on this socket.

}

void	IRCServ::sendPing(std::vector<IRCUser *> v_users)
{
	int ping = atoi(config.get("ping").c_str());
	int timeout = atoi(config.get("timeout").c_str());

	for (std::vector<IRCUser *>::iterator it = v_users.begin(); it != v_users.end(); ++it)
	{
		if ((*it)->time_inactive() >= timeout)
			(*it)->setStatus(DELETE);
		else if ((*it)->time_inactive() >= ping && time(NULL) - (*it)->last_ping >= ping)
		{
			(*it)->write("PING :" + config.get("name"));
			(*it)->last_ping = time(NULL);
		}
	}
}

void						IRCServ::addUnavailableNickname(std::string nick)
{
	unavailable_nicknames.insert(make_pair(nick, time(NULL)));
}

bool						IRCServ::isUnavailableNickname(std::string nick)
{
	std::map<std::string, time_t>::iterator it = unavailable_nicknames.find(nick);

	if (it == unavailable_nicknames.end())
		return (false);
	if (time(NULL) - it->second > atoi(config.get("time_unavailable").c_str()))
	{
		unavailable_nicknames.erase(it);
		return (false);
	}
	return (true);
}

void IRCServ::display()
{
	for (int i = 0; i < 100; i++)
		std::cout << std::endl;

	std::vector<IRCUser *>	users = getUsers();

	std::cout << "Users list :" << std::endl;
	for (std::vector<IRCUser *>::iterator it = users.begin(); it != users.end(); it++)
		std::cout << (*it)->getNickname() << std::endl;
	std::cout << "--END--" << std::endl << std::endl << std::endl; 

	std::vector<IRCChannel *> chans = getChannels();
	for (std::vector<IRCChannel *>::iterator it = chans.begin(); it != chans.end(); it++)
	{
		std::cout << "Channel " << (*it)->getChanNamespace() << " :" << std::endl;
		std::map<IRCUser *, std::string>	chanUsers = (*it)->getChanUsers();

		for (std::map<IRCUser *, std::string>::iterator it2 = chanUsers.begin(); it2 != chanUsers.end(); it2++)
			std::cout << it2->first->getNickname() << std::endl;

		std::cout << "--END--" << std::endl << std::endl;
	}
}

void IRCServ::execute()
{
	std::vector<IRCUser *> v_users = getUsers();


	/* reactualise les revents*/
	if (poll(&pollfds[0], pollfds.size(), 600) == -1)
		return;

	/* Accepte un(e) nouveau(lle) client(e) */
	if (pollfds[0].revents == POLLIN) // Alert me when data is ready to recv() on this socket.
		acceptUser();
	else
		/* Accepte les messages de tous les clients qui ont un truc a dire*/
		for (std::vector<pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); ++it)	{
			if ((*it).revents == POLLIN)	{
				this->users[(*it).fd]->receive(this);
				this->users[(*it).fd]->execute();
			}
		}

	sendPing(v_users);

	/* Le serveur renvoie a chaque client les message en attente */
	for (std::vector<IRCUser *>::iterator it = v_users.begin(); it != v_users.end(); ++it)
			(*it)->push();

	/* Delete tous les clients qui ont ferme la connection de leur cote (DELETE <=> recv = 0) */
	for (std::vector<IRCUser *>::iterator it = v_users.begin(); it != v_users.end(); ++it)
	{
		if ((*it)->getStatus() == DELETE)
			delUser(*(*it));
	}
	display();
}

void IRCServ::delUser(IRCUser &user)
{
	std::vector<IRCChannel *> chans = getChannels();
	for (std::vector<IRCChannel *>::iterator it = chans.begin(); it != chans.end(); it++)
	{
		if (user.isOnChan((*it)))
		{
			(*it)->deleteChanUser(&user);
			if ((*it)->getChanUsers().size() == 0)
				deleteChannel((*it));
		}
	}

	std::map<int, IRCUser *>::iterator it(users.find(user.getFd()));
	if (it != users.end())
	{
		users.erase(it);
		close(user.getFd());
	}

	for (std::vector<pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); ++it)
		if (it->fd == user.getFd())
		{	
			pollfds.erase(it);
			break;
		}
	
	close(user.getFd());
	delete &user;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


std::string IRCServ::getPrefix()
{
	return (":" + config.get("name") + " ");
}

std::string	IRCServ::getConfig(std::string key) { return (config.get(key)); }

IRCChannel*    IRCServ::getChannel(std::string chanName)
{
    size_t i = 0;
    while (i < channels.size())
    {
        if (toupper(channels[i]->getChanNamespace()) == toupper(chanName))
            return (channels[i]);
        i++;
    }
    return (NULL);
}


std::vector<IRCUser *> IRCServ::getUsers(void)
{
	std::vector<IRCUser *> out;
	for (std::map<int, IRCUser *>::iterator it = users.begin() ; it != users.end() ; it++)
		out.push_back(it->second);
	return (out);
}

IRCUser* IRCServ::getUser(std::string username)
{
	for (std::map<int, IRCUser *>::iterator it = users.begin() ; it != users.end() ; it++)
		if (it->second->getNickname() == username)
			return it->second;
	return NULL;
}

std::vector<IRCUser *> IRCServ::getUserByMask(std::string mask)
{
	std::vector<IRCUser *> tab;
	for (std::map<int, IRCUser *>::iterator it = users.begin() ; it != users.end() ; it++)
		if (match_mask(it->second->getHostname(), mask))
			tab.push_back(it->second);
	return (tab);
}

std::vector<IRCChannel *>& IRCServ::getChannels(void)
{
	return (channels);
}

void IRCServ::addChannel(IRCChannel *chan)
{
	channels.push_back(chan);
}


void	IRCServ::deleteChannel(IRCChannel *chan)
{
	for (std::vector<IRCChannel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		if ((*it) == chan)
		{
			delete *it;
			channels.erase(it);
			return;
		}
}


/* ************************************************************************** */