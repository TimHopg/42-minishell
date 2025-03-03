# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: npizzi <npizzi@student.42lisboa.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/10 11:25:34 by thopgood          #+#    #+#              #      #
#    Updated: 2024/10/10 22:01:40 by oerm             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= minishell

CC 			= cc
RM 			= rm -rf
INCLUDE 	= -Iinclude

CFLAGS = -Wall -Wextra -Werror $(INCLUDE) # -g -O0 -fsanitize=address

SRC_DIR 	= src/
OBJ_DIR 	= obj/
LIBFT_DIR 	= libft/
LIBFT		= -L $(LIBFT_DIR) -lft
LDFLAGS  = -lreadline

SRC = 		minishell.c					\
			builtins/cd.c 				\
			builtins/echo.c				\
			builtins/env.c				\
			builtins/exit.c				\
			builtins/export.c			\
			builtins/ft_atoill.c		\
			builtins/pwd.c				\
			builtins/unset.c			\
			builtins/utils_one.c		\
			builtins/utils_two.c 		\
			cli/cli_utils.c				\
			cli/cli_utils_two.c     	\
			cli/shlvl.c					\
			exeggutor/ast_splitter.c 	\
			exeggutor/exec_utils_two.c	\
			exeggutor/exec_utils.c  	\
			exeggutor/exeggutor.c   	\
			exeggutor/file_utils.c  	\
			expansion/expand.c			\
			expansion/utils_one.c 		\
			expansion/utils_two.c		\
			expansion/wildcards.c		\
			heredoc/heredoc_one.c   	\
			heredoc/heredoc_two.c   	\
			heredoc/heredoc.c			\
			parse/parse_pipeline.c  	\
			parse/parse_pref_suf.c  	\
			parse/parse_redirs.c    	\
			parse/parse_utils.c     	\
			parse/parse.c      			\
			signals/signals_two.c   	\
			signals/signals.c       	\
			tokenise/list_utils_one.c 	\
			tokenise/list_utils_two.c 	\
			tokenise/tokenise_meta.c 	\
			tokenise/tokenise_meta2.c 	\
			tokenise/tokenise_utils.c	\
			tokenise/tokenise.c			\
			errors/error_handlers.c

OBJ 		= 	$(addprefix $(OBJ_DIR), $(SRC:.c=.o))

all: $(NAME)

bonus: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@make -C $(LIBFT_DIR)
	@echo ""${BLUE}$(NAME)""${NC}Compiling... "\c"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo ""${GREEN}Complete""$(NC)""

clean:
	@cd $(LIBFT_DIR) && $(MAKE) clean
	@echo ""${BLUE}$(NAME)""${NC}Cleaning..."\c"
	@$(RM) $(OBJ_DIR)
	@echo ""${GREEN}Complete""$(NC)""

fclean: clean
	@cd $(LIBFT_DIR) && $(MAKE) fclean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

# Colours
WHITE	=	'\033[0;37m'
YELLOW	=	'\033[0;33m'
BLUE	=	'\033[0;34m'
GREEN	=	'\033[0;32m'
RED		=	'\033[0;31m'
NC		=	'\033[0m' # no colour
#> /dev/null

val		= 	valgrind
valf	= 	clear; valgrind --suppressions=readline.supp --leak-check=full --track-fds=all --track-origins=yes --show-reachable=yes --show-leak-kinds=all -s
leaks	=	leaks --atExit -- 

run:
	@clear; ./minishell

rel: re
	@clear; ./minishell

val:
	@clear; $(valf) ./minishell

valre: re
	@clear; $(valf) ./minishell

env:
	@clear; env -i ./minishell

libft:
	make re -C $(LIBFT_DIR)

norm:
	norminette ./libft/src ./libft/include ./src ./include

# include in minishell folder
# readline.supp
#{ 
#    leak readline
#    Memcheck:Leak
#    ...
#    fun:readline
#}
#{
#    leak add_history
#    Memcheck:Leak
#    ...
#    fun:add_history
#}
