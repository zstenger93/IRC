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
	@echo "$(RED)Welcome to Interdimensional Rickroll Conspiracy.. ಥ⁠‿⁠ಥ$(COLOR_END)"
	@sleep 0.1
	@echo "$(BWhite)Be careful not to get rickrolled ಥ⁠‿⁠ಥ$(COLOR_END)"
	@echo "$(YELLOW)Compiling.. ಥ⁠‿⁠ಥ\n$(COLOR_END)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rMining sockets                       $(RED) %d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rSetting up listening ears             $(RED)%d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rForging switch statements             $(RED)%d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rCooking sphagetti, ala code           $(RED)%d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rConstructing the Bot service          $(RED)%d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rCreating General channel              $(RED)%d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rCreating users                        $(RED)%d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rRealizing send makes no sendse        $(RED)%d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rBegging to the server not to crash    $(RED)%d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 35); do \
		percentage=$$(($$i * 100 / 35)); \
		printf "$(YELLOW)\rSaying final prayers                  $(RED)%d%% $(BLUE)" $$percentage; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(GREEN) Done!\n\n"
	@for i in $$(seq 25); do \
		percentage=$$(($$i * 100 / 50)); \
		if [ $$percentage -le 24 ]; then \
			printf "$(YELLOW)\rAvoiding brain damage                $(RED) %d%%  $(BLUE)" $$percentage; \
		else \
			printf "$(YELLOW)\rAvoiding brain damage                $(RED) %d%%  $(RED)" $$percentage; \
		fi; \
		for j in $$(seq $$i); do \
			printf "█"; \
		done; \
		sleep 0.02; \
	done
	@printf "$(RED) CRASH!\n\n"
	@echo "$(BWhite)The project is compiled.. ಥ⁠‿⁠ಥ$(COLOR_END)"
	@for i in $$(seq 1); do \
		sleep 1; \
		$(RICK); \
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
BLUE = \033[1;34m

RICK = open https://www.youtube.com/watch?v=dQw4w9WgXcQ

.PHONY: all clean fclean re displaypercent