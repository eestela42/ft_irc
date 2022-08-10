#include "IRCCommand.hpp"
#include "../IRCUser.hpp"

bool	recur(const char *str, const char *mask)
{
	if (!mask[0])
		return (!str[0]);
	if (mask[0] == '*')
	{
		while (true)
		{
			if (recur(str, mask + 1))
				return (true);
			if (!str[0])
				return (false);
			str++;
		}
	}
	else if (!str[0])
		return false;
	else if (mask[0] == '?' || mask[0] == str[0])
		return (recur(str + 1, mask + 1));
	return (false);
}

bool	match_mask(std::string str, std::string mask)
{
	return (recur(str.c_str(), mask.c_str()));
}

bool	is_letter(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (true);
	return (false);
}
bool	is_digit(char c)
{
	if ((c >= '0' && c <= '9'))
		return (true);
	return (false);
}

bool	is_special(char c)
{
	if ((c >= 91 && c <= 96) || (c >= 123 && c <= 125))
		return (true);
	return (false);
}

int is_nospcrlfcl(char c)
{
	return !(c == '\r' || c == '\n' || c == '\0' || c == ' ' || c == ':');
}
