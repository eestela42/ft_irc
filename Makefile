NAME		= ircserv

CONNECTION	= PASS.cpp NICK.cpp USER.cpp PRIVMSG.cpp PING.cpp PONG.cpp WHOIS.cpp \
				MODE.cpp OPER.cpp WALLOPS.cpp QUIT.cpp AWAY.cpp KILL.cpp NOTICE.cpp

CHANNEL		= JOIN.cpp PART.cpp TOPIC.cpp INVITE.cpp NAMES.cpp LIST.cpp KICK.cpp

COMMAND		= IRCCommand.cpp syntax.cpp \
			$(addprefix connection/, $(CONNECTION)) \
			$(addprefix channel/, $(CHANNEL))

PURESRCS	= main.cpp utils.cpp IRCConfig.cpp IRCServ.cpp IRCUser.cpp  IRCChannel.cpp \
			$(addprefix command/, $(COMMAND))

SRCS		= $(addprefix srcs/, $(PURESRCS))

OBJS		= $(addprefix objs/, $(PURESRCS:.cpp=.o))

CFLAGS		= -Wall -Wextra -Werror -I.
 -fsanitize=address -std=c++98

all:		${NAME}

$(NAME):	${OBJS}
			c++ ${CFLAGS} -o ${NAME} ${OBJS}

objs/%.o:	srcs/%.cpp
			@mkdir -p objs/command/connection
			@mkdir -p objs/command/channel
			c++ $(CFLAGS) -o $@ -c $<

clean:
			rm -f ${OBJS}

fclean:		clean
			rm -f ${NAME}

re:			fclean all

.PHONY:		all clean fclean re