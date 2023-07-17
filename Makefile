NAME	= ircserv
CC		= c++
FLAGS	= -Wall -Wextra -Werror -std=c++98
RM		= rm -rf
SRC_DIR	= src/
OBJ_DIR	= objects/
USER	= $(shell whoami)

SRC		= $(MAIN) \
		  $(PARSING) \
		  $(SERVER) \
		  $(CONFIG) \
		  $(ADMIN) \
		  $(CLIENT) \
		  $(CHANNEL) \
		  $(USER) \
		  $(COMMANDS) \
		  $(MESSAGES) \
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
			  commands/CommandUtils \
			  commands/ProcessCommands \
			  commands/OnlyOperatorCommands \

MESSAGES	= chat/Messages \

CHANNEL		= channel/Channel \

BOT			= bot/Bot \

SRCS	= $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC)))
OBJS	= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC)))

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Compiling.. ಥ⁠‿⁠ಥ$(COLOR_END)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@printf "$(RED)  [$(YELLOW)"
	@for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64; do \
		printf "="; \
		sleep 0.1; \
	done
	@printf "$(RED)]$(BLUE)->$(GREEN) Done!\n"
	@echo "$(GREEN)The project is compiled.. ಥ⁠‿⁠ಥ$(COLOR_END)"
	@echo "$(YELLOW)Be careful $(USER) not to get rickrolled ಥ⁠‿⁠ಥ$(COLOR_END)"
# @echo "$(GREEN)Welcome to Interdimensional Rickroll Conspiracy.. ಥ⁠‿⁠ಥ$(COLOR_END)$(RICK)"
	@echo "$(GREEN)Well I guess it's too late ಥ⁠‿⁠ಥ$(COLOR_END)"


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