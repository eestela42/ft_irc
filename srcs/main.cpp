/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 13:13:37 by upeyret           #+#    #+#             */
/*   Updated: 2022/08/10 12:45:53 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

bool is_over = false;

void sig_handler(int) { is_over = true; }

int main(int ac, char **av)
{
    if (ac != 3)
        exit_on_error(1, "Please enter: ./ircserv <port> <password>");
    IRCServ server = IRCServ(av[1], av[2]);
    signal(SIGINT, sig_handler);
	IRCCommand::init_func_map();

    server.init();
	while (!is_over)
		server.execute();
	std::cout << "\nSERVER IRC IS NOW OFF\n";
	return 0;
}