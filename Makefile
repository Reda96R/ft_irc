#::::::::::::::::OS::::::::::::::::#
OS 			= $(shell uname -s)

#:::::::::::::::VARS:::::::::::::::#
SRC_DIR		= src/
OBJ_DIR 	= .obj/
FSANITIZE 	= #-g -fsanitize=address
CPPFLAGS 	= -Wall -Werror -Wextra -std=c++98 $(HEADER) $(FSANITIZE)
CC 			= c++

#:::::::::::::::MANDATORY:::::::::::::::#
NAME 		= ircserv
HEADER 		= -I includes
MAIN 		= main 
M_OBJS 		= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(addprefix $(SRC_DIR), $(MAIN))))

#::::::::::::::::MLX:::::::::::::::#
ifeq ($(OS), Darwin)
# COMP 		= 
# COMP_O 	=
else
# COMP 		=
# COMP_O 	=
endif

all: $(NAME)

#::::::::::::::::PRS:::::::::::::::#
R_FILES 	=

R_OBJS 		= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(addprefix $(SRC_DIR)reda/, $(R_FILES))))

#::::::::::::::::RAY:::::::::::::::#
A_FILES 	=

A_OBJS 		= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(addprefix $(SRC_DIR)abdellah/, $(A_FILES))))

#::::::::::::::::TXT:::::::::::::::#
D_FILES_M 	=

D_OBJS 		= $(addprefix $(OBJ_DIR), $(addsuffix .o, $(addprefix $(SRC_DIR)adam/, $(D_FILES_M))))

#:::::::::::::Compile::::::::::::::#
$(NAME): $(M_OBJS) $(R_OBJS) $(A_OBJS) $(D_OBJS)
	@echo $(cursive)$(grey)":::Making object files:::"$(reset)
	@echo $(cursive)$(grey)":::Compiling $(NAME):::"$(reset)
	@$(CC) $(CPPFLAGS) $(M_OBJS) $(R_OBJS) $(A_OBJS) $(D_OBJS) -o $(NAME)
	@echo $(f_green)":::✅ $(NAME) is ready ✅:::"$(reset)

$(OBJ_DIR)%.o: %.c 
	@mkdir -p $(OBJ_DIR)src/reda $(OBJ_DIR)src/abdellah $(OBJ_DIR)src/adam
	@$(CC) $(CPPFLAGS) -c $< -o $@

#:::::::::::::Clean::::::::::::::#
clean:
	@echo $(cursive)$(grey)":::Deleting object files:::"$(reset)
	@rm -rf $(OBJ_DIR)
	@echo $(red)":::Deleted:::"$(reset)

fclean: clean
	@echo $(cursive)$(grey)":::Deleting executable:::"$(reset)
	@rm -rf $(NAME)
	@echo $(red)":::Deleted:::"$(reset)

re: fclean all

.PHONY: all clean fclean re os

define os
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(yellow) "" $(reset)
	@echo $(green) "for $(OS)" $(reset)
	@echo $(green) "" $(reset)
endef

#::::::::::::::Colors::::::::::::::#
black  		= "\033[0;30m"
red    		= "\033[0;31m"
green  		= "\033[0;32m"
f_green		= "\e[92;5;118m"
yellow 		= "\033[0;33m"
purple		= "\033[0;35m"
blue   		= "\033[0;34m"
cyan  		= "\033[0;36m"
grey   		= "\033[0;90m"
reset  		= "\033[0m"

#::::::::::::::Text:::::::::::::::#
bold     	= "\033[1m"
cursive    	= "\033[3m"
underline 	= "\033[4m"
