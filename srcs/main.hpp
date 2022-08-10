/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adesvall <adesvall@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 14:25:45 by upeyret           #+#    #+#             */
/*   Updated: 2022/03/10 16:49:18 by adesvall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP

/* ------------ INCLUDES ------------ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <poll.h>
#include <fcntl.h>



#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

#include "utils.hpp"
#include "IRCServ.hpp"
#include "IRCConfig.hpp"
#include "IRCUser.hpp"
#include "command/IRCCommand.hpp"

/* ------------ MACROS ------------ */

#define MYPORT 6667    // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold
#define ERR_NBARG 11
#define DEBUG 0


#define SERVERNAME "ft_irc.net"
#define LOCALHOST "127.0.0.1"
#define DEF_PORT "6667"
#define BUFFER_SIZE	512
#define DEF_PASS "123"
#define FD_REQUEST 1
#define FD_COMMIT 2
#define MAX_MESSAGE_LEN 510
#define INFO "BANDA IRC server"
#define MESSAGE_END "\r\n"


#endif