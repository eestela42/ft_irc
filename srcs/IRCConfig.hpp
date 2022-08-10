#ifndef IRCCONFIG_HPP
# define IRCCONFIG_HPP

# include <iostream>
# include <string>
# include <map>

class IRCConfig
{

public:

	IRCConfig();
	~IRCConfig();

	void set(std::string key, std::string value);
	std::string get(std::string key);

private:
	std::map<std::string, std::string> config_map;
};

#endif
/* ******************************************************* IRCCONFIG_H */