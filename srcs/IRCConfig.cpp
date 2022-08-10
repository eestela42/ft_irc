#include "IRCConfig.hpp"
#include "main.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

IRCConfig::IRCConfig()
{
	/* Fichier de configuration */
	set("name", SERVERNAME);
	set("max", "1000");
	set("ping", "30");
	set("timeout", "60");

	set("oper_login", "oper");
	set("oper_password", "oper");

	/* Divers */
	set("max", "1000");
	set("time_unavailable", "35");
	
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IRCConfig::~IRCConfig()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void IRCConfig::set(std::string key, std::string value)
{
	config_map.insert(make_pair(key, value));
}


std::string IRCConfig::get(std::string key)
{
	std::map<std::string, std::string>::iterator it = config_map.find(key);
	if (it != config_map.end())
		return (it->second);
	return ("error");
}

/* ************************************************************************** */