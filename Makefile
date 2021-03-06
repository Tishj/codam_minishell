# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: rlucas <marvin@codam.nl>                     +#+                      #
#                                                    +#+                       #
#    Created: 2020/04/12 11:11:07 by rlucas        #+#    #+#                  #
#    Updated: 2020/06/24 15:00:00 by tbruinem      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBDIR = libft
SRCDIR = src
OBJDIR = obj
HEADERS = minishell.h msh_term.h
INCLUDES = -I includes/ -I $(LIBDIR)/includes/

SRC =	exit.c \
		tables.c \
		input/get_endstate.c \
		input/initialize_line_editor.c \
		input/refresh_cursor.c \
		input/prompt.c \
		input/handle_input.c \
		keyevents/backspace.c \
		keyevents/char_input.c \
		keyevents/cursor_end.c \
		keyevents/cursor_home.c \
		keyevents/cursor_move.c \
		keyevents/cursor_left.c \
		keyevents/cursor_right.c \
		keyevents/cursor_move_row.c \
		keyevents/cursor_move_word.c \
		keyevents/send_input.c \
		keyevents/special_command.c \
		keyevents/send_eof.c \
		term/clear_screen.c \
		term/clear_input.c \
		term/add_char.c \
		term/delete_char.c \
		term/delete_word.c \
		term/termcmd.c \
		vector/vec_add.c \
		vector/vec_getref.c \
		vector/vec_get.c \
		vector/vec_destroy.c \
		vector/vec_new.c \
		execute/execute.c \
		execute/get_program.c \
		execute/get_abspath.c \
		execute/run_builtin.c \
		execute/run_program.c \
		builtin/builtin_error.c \
		builtin/ft_cd.c \
		builtin/ft_export.c \
		builtin/ft_unset.c \
		builtin/ft_pwd.c \
		builtin/ft_echo.c \
		builtin/ft_exit.c \
		builtin/ft_env.c \
		builtin/get_cwd.c \
		commands/get_commands.c \
		commands/clear_commands.c \
		commands/new_command.c \
		commands/push_command.c \
		commands/run_commands.c \
		redirection/set_redirection.c \
		redirection/close_ifnot.c \
		redirection/close_all.c \
		redirection/close_iostream.c \
		redirection/in_out_redirection.c \
		utils/ft_strsdup.c \
		utils/ft_str2clear.c \
		utils/ft_str2cmpstr.c \
		utils/ft_str2print.c \
		utils/ft_str2len.c \
		utils/ft_str3join.c \
		utils/ft_no_ansi_strlen.c \
		env/env_init.c \
		env/env_clear.c \
		env/env_del.c \
		env/env_new.c \
		env/env_print.c \
		env/env_val_set.c \
		env/env_val_get.c \
		env/env_update.c \
		env/env_convert.c \
		env/env_unset.c \
		env/pwd_set.c \
		tokens/tokenizer.c \
		tokens/conv_tokens.c \
		tokens/check_esc_char.c \
		tokens/create_token.c \
		tokens/env_strclen.c \
		tokens/env_exitstatus.c \
		tokens/env_current_process.c \
		tokens/parse_error.c \
		tokens/lex_checkstate.c \
		tokens/update_lexer.c \
		tokens/init_lexer.c \
		debug/print_state.c \
		debug/debug_commands.c \
		debug/print_tokens.c

ifdef WITHBONUS
SRC +=	main_bonus.c \
		signals_bonus.c \
		tokens/quote_toks_bonus.c \
		tokens/gen_tokens_bonus.c \
		tokens/evaluate_env_bonus.c \
		term/init_readline_bonus.c \
		input/read_input_bonus.c
else
SRC +=	main.c \
		signals.c \
		tokens/quote_toks.c \
		tokens/gen_tokens.c \
		tokens/evaluate_env.c \
		term/init_readline.c \
		input/read_input.c
endif

OBJ := $(addprefix $(OBJDIR)/, $(SRC:%.c=%.o))
SRC := $(addprefix $(SRCDIR)/, $(SRC))

FLAGS = -Wall -Wextra -Werror
ifdef DEBUG
	FLAGS += -g -fsanitize=address
endif

all: lft $(NAME)

bonus:
	$(MAKE) WITHBONUS=1 all

$(NAME): $(OBJ)
	@echo "Compiling shell..."
	@gcc $(FLAGS) $(OBJ) -o $(NAME) $(INCLUDES) -L $(LIBDIR)/ -lft -ltermcap

lft:
	@$(MAKE) -sC $(LIBDIR)/

obj/%.o: src/%.c
	@mkdir -p $(@D)
	@echo "Compiling $@"
	@gcc -c $(INCLUDES) $(FLAGS) $< -o $@

clean:
	@echo "Removing objects of libraries..."
	@$(MAKE) -sC $(LIBDIR)/ clean -j
	@echo "Removing objects directory..."
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "Removing libraries and minishell executable..."
	@$(MAKE) -sC $(LIBDIR)/ fclean -j
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean all re bonus compile_library
