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

SRCS		= $(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC)))
OBJS		= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC)))

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(YELLOW)Compiling.. ಥ⁠‿⁠ಥ$(COLOR_END)"
	@sleep 0.1
	@echo "$(YELLOW)Be careful not to get rickrolled ಥ⁠‿⁠ಥ$(COLOR_END)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@printf "$(YELLOW)Mining sockets   $(COLOR_END)"
	make displaypercent
	@printf "$(YELLOW)Cutting ears to have something to listen with   $(COLOR_END)"
	make displaypercent
	@printf "$(YELLOW)Forging switch statements   $(COLOR_END)"
	make displaypercent
	@printf "$(YELLOW)Cooking sphagetti, ala code   $(COLOR_END)"
	make displaypercent
	@printf "$(YELLOW)Constructing the Bot service   $(COLOR_END)"
	make displaypercent
	@printf "$(YELLOW)Creating General channel   $(COLOR_END)"
	make displaypercent
	@printf "$(YELLOW)Creating users   $(COLOR_END)"
	make displaypercent
	@printf "$(YELLOW)Begging to the server not to crash   $(COLOR_END)"
	make displaypercent
	@printf "$(YELLOW)Saying final prayers   $(COLOR_END)"
	make displaypercent
	@printf "$(GREEN) Done!\n"
	@echo "$(GREEN)The project is compiled.. ಥ⁠‿⁠ಥ$(COLOR_END)"
# @echo "$(GREEN)Welcome to Interdimensional Rickroll Conspiracy.. ಥ⁠‿⁠ಥ$(COLOR_END)$(RICK)"
	@echo "$(GREEN)Well I guess it's too late ಥ⁠‿⁠ಥ$(COLOR_END)"

displaypercent:
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\r%d%% " $$percentage; \
		printf "$(YELLOW)"; \
		for j in $$(seq $$i); do \
			printf "$(BLUE)█"; \
		done; \
		sleep 0.05; \
	done

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

.PHONY: all clean fclean re displaypercent