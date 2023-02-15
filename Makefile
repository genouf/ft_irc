FILES:=	main Server User Channel \
			Command/Connection/pass Command/Connection/user Command/Connection/nick Command/Connection/ping Command/Connection/pong Command/Connection/quit Command/Connection/oper\
			Command/Channel/join Command/Channel/list Command/Channel/part Command/Channel/topic Command/Channel/names Command/Channel/privmsg Command/Channel/invite\
			Command/Operator/kill \
			Command/who Command/motd Command/notice Command/info
NAME:= ircserv

# ------------------
CC:=c++
SRCPATH:=sources/
HDRPATH:=
CCHPATH:=obj/
IFLAGS:=-I ${HDRPATH}
CFLAGS:=-Wall -Wextra -Werror -std=c++98 -g3
# ==================

# ----- Colors -----
BLACK:="\033[1;30m"
RED:="\033[1;31m"
GREEN:="\033[1;32m"
CYAN:="\033[1;35m"
PURPLE:="\033[1;36m"
WHITE:="\033[1;37m"
EOC:="\033[0;0m"
# ==================

# ------ Auto ------
SRC:=$(addprefix $(SRCPATH),$(addsuffix .cpp,$(FILES)))
OBJ:=$(addprefix $(CCHPATH),$(addsuffix .o,$(FILES)))
# ==================

all: ${NAME}

${NAME}: ${OBJ}
	@echo ${CYAN} " - Compiling $@" $(RED)
	@${CC} ${CFLAGS} ${SRC} -o ${NAME}
	@echo $(GREEN) " - OK" $(EOC)

${CCHPATH}%.o: ${SRCPATH}%.cpp
	@mkdir -p $(@D)
	@echo ${PURPLE} " - Compiling $< into $@" ${EOC}
	@${CC} ${CFLAGS} -c $< -o $@

%.cpp:
	@echo ${RED}"Missing file : $@" ${EOC}

clean:
	@rm -rf ${CCHPATH}

fclean:	clean
	@rm -f ${NAME}
	@rm -rf ${NAME}.dSYM/

re:	fclean
	@${MAKE} all

.PHONY:	all clean fclean re
