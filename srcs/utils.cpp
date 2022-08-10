/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 14:23:54 by upeyret           #+#    #+#             */
/*   Updated: 2022/06/14 15:39:35 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void    disp_err(std::string msg)
{
	std::cerr << "error:" << msg << std::endl;
}

void exit_on_error(int errnb, std::string msg)
{
	disp_err(msg);
	exit(errnb);
}

std::vector<std::string>    split(std::string str, std::string delim)
{
	std::vector<std::string>    res;

	size_t  position = str.find(delim);
	while (str.size() && position != std::string::npos)
	{
		res.push_back(str.substr(0, position));
		str.erase(0, position + delim.length());
		position = str.find(delim);
	}
	if (str.size())
		res.push_back(str);
	return res;
}

std::string 	toupper(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		str[i] = std::toupper(str[i]);
	return str;
}