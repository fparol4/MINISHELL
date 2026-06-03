NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror

LIBFT_DIR = libraries/libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I. -Iheaders -Ilibraries -Ilibraries/libft
TEST_FLAGS = -DSNOW_ENABLED

SHARED_SRCS = source/shared/error.c \
			  source/shared/free.c \
			  source/shared/general.c \
			  source/shared/string.c \
			  source/shared/signal.c

CORE_SRCS = source/core/core.c \
			source/core/reader.c

BUILTIN_SRCS = source/modules/builtin/cd.c \
			   source/modules/builtin/echo.c \
			   source/modules/builtin/env.c \
			   source/modules/builtin/exit.c \
			   source/modules/builtin/export.c \
			   source/modules/builtin/pwd.c \
			   source/modules/builtin/unset.c

ENVM_SRCS = source/modules/envm/common.c \
			source/modules/envm/free.c \
			source/modules/envm/get.c \
			source/modules/envm/init.c \
			source/modules/envm/size.c \
			source/modules/envm/set.c \
			source/modules/envm/toarr.c \
			source/modules/envm/unset.c

RUNNER_SRCS = source/modules/runner/path.c \
			  source/modules/runner/pipe.c \
			  source/modules/runner/redir.c \
			  source/modules/runner/status.c \
			  source/modules/runner/control.c \
			  source/modules/runner/builtin.c \
			  source/modules/runner/external.c \
			  source/modules/runner/expander.c

LEXER_SRCS = source/modules/lexer/scanner/utils.c \
			 source/modules/lexer/extract.c \
			 source/modules/lexer/lexer.c \
			 source/modules/lexer/lexer_init.c \
			 source/modules/lexer/lexer_rules.c \
			 source/modules/lexer/quote_state.c \
			 source/modules/lexer/token.c \
			 source/modules/lexer/token_table.c \
			 source/modules/lexer/token_type.c

PARSER_SRCS = source/modules/parser/dynarray.c \
			  source/modules/parser/parser.c \
			  source/modules/parser/parser_cleanup.c \
			  source/modules/parser/parser_command.c \
			  source/modules/parser/parser_errors.c \
			  source/modules/parser/parser_state.c \
			  source/modules/parser/parser_utils.c

SRCS = source/main.c \
	   $(CORE_SRCS) \
	   $(SHARED_SRCS) \
	   $(BUILTIN_SRCS) \
	   $(ENVM_SRCS) \
	   $(RUNNER_SRCS) \
	   $(LEXER_SRCS) \
	   $(PARSER_SRCS)

OBJ_DIR = build
OBJS = $(SRCS:source/%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -sC $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: source/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -sC $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -sC $(LIBFT_DIR) fclean

re: fclean all

test: t\:envm t\:builtin t\:runner t\:core t\:lexer t\:parser

t\:envm: $(LIBFT)
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/envm.c \
			$(SHARED_SRCS) \
			$(ENVM_SRCS) \
			$(LIBFT) \
			-lreadline \
			-o /tmp/minishell_envm_tests
	@/tmp/minishell_envm_tests

t\:builtin: $(LIBFT)
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/bultin.c \
			$(SHARED_SRCS) \
			$(BUILTIN_SRCS) \
			source/modules/runner/status.c \
			$(filter-out source/modules/envm/toarr.c,$(ENVM_SRCS)) \
			$(LIBFT) \
			-lreadline \
			-o /tmp/minishell_builtin_tests
	@/tmp/minishell_builtin_tests

t\:runner: $(LIBFT)
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/runner.c \
		$(RUNNER_SRCS) \
		$(SHARED_SRCS) \
			$(BUILTIN_SRCS) \
			$(ENVM_SRCS) \
			$(LIBFT) -lreadline -o /tmp/minishell_runner_tests
	@/tmp/minishell_runner_tests

t\:core: $(LIBFT)
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/core.c \
		source/core/core.c \
			$(SHARED_SRCS) \
			$(ENVM_SRCS) \
			$(LIBFT) \
			-lreadline \
			-o /tmp/minishell_core_tests
	@/tmp/minishell_core_tests

t\:lexer: $(LIBFT)
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
			tests/lexer.c \
			$(LEXER_SRCS) \
			source/shared/error.c \
			source/shared/string.c \
			$(LIBFT) \
			-o /tmp/minishell_lexer_tests
	@/tmp/minishell_lexer_tests

t\:parser: $(LIBFT)
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/parser.c \
			$(PARSER_SRCS) \
			$(LEXER_SRCS) \
			source/shared/error.c \
			source/shared/string.c \
			$(LIBFT) \
			-o /tmp/minishell_parser_tests
	@/tmp/minishell_parser_tests

.PHONY: all clean fclean re test \
	t\:envm t\:builtin t\:runner t\:core t\:lexer t\:parser
