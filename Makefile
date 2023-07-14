NAME	= ircserv
CC		= c++
FLAGS	= -Wall -Wextra -Werror -std=c++98
RM		= rm -rf
SRC_DIR	= src/
OBJ_DIR	= objects/
USER	= $(shell whoami)

SRC		= $(MAIN) $(PARSING) \
		  $(CLIENT) $(SERVER) \
		  $(CONFIG) $(ADMIN) \
		  $(CHANNEL) $(USER) \
		  $(MESSAGES) $(COMMANDS) \
		  $(BOT) \

MAIN		= main/Irc \

PARSING		= parsing/Parsing \
			  parsing/Parser \

SERVER		= server/Server \

CLIENT		= client/ClientAuthenticate \

ADMIN		= admin/Admin \

CONFIG		= config/Config \

USER		= user/User \

COMMANDS	= commands/Commands \
			  commands/ProcessCommands \

MESSAGES	= chat/Messages \

CHANNEL		= channel/Channel \

BOT			= bot/Bot \

SRCS	= $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC)))
OBJS	= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC)))

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Compiling.. ಥ⁠‿⁠ಥ$(COLOR_END)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)The project is compiled.. ಥ⁠‿⁠ಥ$(COLOR_END)"
# @echo "$(GREEN)Welcome to Interdimensional Rickroll Conspiracy.. ಥ⁠‿⁠ಥ$(COLOR_END)$(RICK)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(YELLOW)Cleaning...$(COLOR_END)"
	@$(RM) $(OBJ_DIR)
	@echo "$(GREEN)Cleaning done.$(COLOR_END)"

fclean: clean
	@echo "$(YELLOW)Cleaning everything...$(COLOR_END)"
	@$(RM) $(NAME)
	@echo "$(GREEN)Cleaning done.$(COLOR_END)"

re:
	@echo "$(YELLOW)Recompiling...$(COLOR_END)"
	@make fclean
	@make all
	@echo "$(GREEN)The project has been recompiled.$(COLOR_END)"

COLOR_END = \033[0;39m
CYAN3 = \033[1;4;96m
YELLOW = \033[1;33m
PURPLE = \033[1;35m
BWhite = \033[1;37m
RED = \033[1;91m
GREEN = \033[1;92m
CYAN2 = \x1B[1;36m
CYAN = \033[1;96m

RICK = $(shell open https://www.youtube.com/watch?v=dQw4w9WgXcQ)

.PHONY: all clean fclean re
