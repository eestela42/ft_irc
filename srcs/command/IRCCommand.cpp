#include "IRCCommand.hpp"
#include "../main.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

std::map<std::string, void (IRCCommand::*)()> IRCCommand::func_map;

std::vector<std::string> split_para(std::string str)
{
	std::vector<std::string>    res;
	int cont = 0;
	size_t  position = str.find(" ");
	while (str.size() && position != std::string::npos && cont < 15)
	{
		res.push_back(str.substr(0, position));
		str.erase(0, position + 1);
		if (str[0] == ':')
		{
			res.push_back(str.substr(1));
			str.erase();
		}
		position = str.find(" ");
		cont++;
	}
	if (str.size())
		res.push_back(str);
	return res;
}

IRCCommand::IRCCommand(IRCUser *usr, IRCServ *serv, std::string &msg) : user(usr), server(serv), message(msg)
{
	std::string delimiter(":");
	size_t		i = 0;


	if (msg[0] == ':')
	{
		i = msg.find(' ');
		this->prefix = msg.substr(1, i - 1);
		msg.erase(0, i + 1);
	}
	message = msg;
	this->parameters = split_para(msg);

}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCCommand::~IRCCommand()
{

}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void	IRCCommand::exec_command()
{
	std::string command = parameters[0];
	for (size_t i = 0; i < command.length(); ++i)
		command[i] = std::toupper(command[i]);
	if (command != "PASS" && user->getStatus() == PASSWORD)
		return ;
	if (func_map.find(command) != func_map.end())
		(this->*func_map[command])();
	else if (DEBUG)
		std::cout << "Command not found " << std::endl;
}

void	IRCCommand::reply(std::string msg, ...)
{
	va_list vl;
	std::string dest_nick = user->getNickname();

	if (dest_nick == "")
		dest_nick = "*";
	std::string	message(server->getPrefix()+ msg.substr(0, 4) + dest_nick +" ");
	size_t i = 4;

	va_start(vl, msg);
	while (i < msg.length())
	{
		if (msg[i] != '<')
			message += msg[i];
		else
		{
			while (msg[i] != '>')
				i++;
			message += va_arg(vl, char*);
		}
		i++;
	}
	if (DEBUG)
		std::cout << parameters[0] + " REPLY SENDING..\n";
	user->write(message);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


IRCServ						&IRCCommand::getServer() const {
	return *server;	}
IRCUser						&IRCCommand::getUser() const {
	return *user;	}
std::vector<std::string>	IRCCommand::getParams() const {
	return parameters;	}
std::string					IRCCommand::getMessage() const {
	return message;	}

/* ************************************************************************** */

void IRCCommand::init_func_map()
{
	func_map.insert(std::make_pair("PASS", &IRCCommand::PASS));
	func_map.insert(std::make_pair("AWAY", &IRCCommand::AWAY));
	func_map.insert(std::make_pair("NICK", &IRCCommand::NICK));
	func_map.insert(std::make_pair("NOTICE", &IRCCommand::NOTICE));
	func_map.insert(std::make_pair("KICK", &IRCCommand::KICK));
	func_map.insert(std::make_pair("KILL", &IRCCommand::KILL));
	func_map.insert(std::make_pair("USER", &IRCCommand::USER));
	func_map.insert(std::make_pair("PING", &IRCCommand::PING));
	func_map.insert(std::make_pair("PONG", &IRCCommand::PONG));
	func_map.insert(std::make_pair("JOIN", &IRCCommand::JOIN));
	func_map.insert(std::make_pair("PART", &IRCCommand::PART));
	func_map.insert(std::make_pair("TOPIC", &IRCCommand::TOPIC));
	func_map.insert(std::make_pair("MODE", &IRCCommand::MODE));
	func_map.insert(std::make_pair("INVITE", &IRCCommand::INVITE));
	func_map.insert(std::make_pair("NAMES", &IRCCommand::NAMES));
	func_map.insert(std::make_pair("LIST", &IRCCommand::LIST));
	func_map.insert(std::make_pair("PRIVMSG", &IRCCommand::PRIVMSG));
	func_map.insert(std::make_pair("OPER", &IRCCommand::OPER));
	func_map.insert(std::make_pair("QUIT", &IRCCommand::QUIT));
	func_map.insert(std::make_pair("WALLOPS", &IRCCommand::WALLOPS));
}
