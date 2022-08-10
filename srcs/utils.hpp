/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:36:30 by upeyret           #+#    #+#             */
/*   Updated: 2022/08/07 00:31:45 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "main.hpp"

std::string					toupper(std::string str);
void    					disp(std::string msg);
void 						exit_on_error(int errnb, std::string msg);
std::vector<std::string>    split(std::string str, std::string delim);
bool						match_mask(std::string str, std::string mask);



#endif