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

BUILTIN_SRCS = source/modules/bin/cd.c \
			   source/modules/bin/echo.c \
			   source/modules/bin/env.c \
			   source/modules/bin/exit.c \
			   source/modules/bin/export.c \
			   source/modules/bin/pwd.c \
			   source/modules/bin/unset.c

ENVM_SRCS = source/modules/env/list.c \
			source/modules/env/free.c \
			source/modules/env/get.c \
			source/modules/env/init.c \
			source/modules/env/size.c \
			source/modules/env/set.c \
			source/modules/env/toarr.c \
			source/modules/env/unset.c

EXPANDER_SRCS = source/modules/runner/expander/expand.c \
				source/modules/runner/expander/list.c \
				source/modules/runner/expander/quotes.c \
				source/modules/runner/expander/variable.c \
				source/modules/runner/expander/word.c

RUNNER_PATH_SRCS = source/modules/runner/path/path.c \
				  source/modules/runner/path/parts.c

RUNNER_PIPE_SRCS = source/modules/runner/pipe/fd.c \
				  source/modules/runner/pipe/pipe.c \
				  source/modules/runner/pipe/process.c \
				  source/modules/runner/pipe/tree.c

RUNNER_REDIR_SRCS = source/modules/runner/redir/apply.c \
				   source/modules/runner/redir/buffer.c \
				   source/modules/runner/redir/expand.c \
				   source/modules/runner/redir/heredoc.c \
				   source/modules/runner/redir/heredoc_read.c \
				   source/modules/runner/redir/push.c \
				   source/modules/runner/redir/target.c

RUNNER_EXTERNAL_SRCS = source/modules/runner/external/process.c \
					   source/modules/runner/external/resolve.c

RUNNER_SRCS = $(RUNNER_PATH_SRCS) \
			  $(RUNNER_PIPE_SRCS) \
			  $(RUNNER_REDIR_SRCS) \
			  source/modules/runner/status.c \
			  source/modules/runner/control.c \
			  source/modules/runner/builtin.c \
			  $(RUNNER_EXTERNAL_SRCS) \
			  $(EXPANDER_SRCS)

LEXER_SRCS = source/modules/lexer/scanner/load.c \
			 source/modules/lexer/scanner/move.c \
			 source/modules/lexer/scanner/get.c \
			 source/modules/lexer/scanner/table.c \
			 source/modules/lexer/extract.c \
			 source/modules/lexer/lexer.c \
			 source/modules/lexer/rules.c \
			 source/modules/lexer/state.c \
			 source/modules/lexer/token.c

PARSER_SRCS = source/modules/parser/parser.c \
			  source/modules/parser/cleanup.c \
			  source/modules/parser/command.c \
			  source/modules/parser/errors.c \
			  source/modules/parser/state.c \
			  source/modules/parser/utils.c

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

$(LIBFT): force
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
	@mkdir -p tests/dist
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/envm.c \
			$(SHARED_SRCS) \
			$(ENVM_SRCS) \
			$(LIBFT) \
			-lreadline \
			-o tests/dist/envm
	@tests/dist/envm

t\:builtin: $(LIBFT)
	@mkdir -p tests/dist
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/bultin.c \
			$(SHARED_SRCS) \
			$(BUILTIN_SRCS) \
			source/modules/runner/status.c \
			$(filter-out source/modules/env/toarr.c,$(ENVM_SRCS)) \
			$(LIBFT) \
			-lreadline \
			-o tests/dist/builtin
	@tests/dist/builtin

t\:runner: $(LIBFT)
	@mkdir -p tests/dist
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/runner.c \
		$(RUNNER_SRCS) \
		$(SHARED_SRCS) \
			$(BUILTIN_SRCS) \
			$(ENVM_SRCS) \
			$(LIBFT) -lreadline -o tests/dist/runner
	@tests/dist/runner

t\:core: $(LIBFT)
	@mkdir -p tests/dist
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/core.c \
		source/core/core.c \
			$(SHARED_SRCS) \
			$(ENVM_SRCS) \
			$(LIBFT) \
			-lreadline \
			-o tests/dist/core
	@tests/dist/core

t\:lexer: $(LIBFT)
	@mkdir -p tests/dist
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
			tests/lexer.c \
			$(LEXER_SRCS) \
			source/shared/error.c \
			source/shared/string.c \
			$(LIBFT) \
			-o tests/dist/lexer
	@tests/dist/lexer

t\:parser: $(LIBFT)
	@mkdir -p tests/dist
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/parser.c \
			$(PARSER_SRCS) \
			$(LEXER_SRCS) \
			source/shared/error.c \
			source/shared/string.c \
			$(LIBFT) \
			-o tests/dist/parser
	@tests/dist/parser

.PHONY: all clean fclean re test force \
	t\:envm t\:builtin t\:runner t\:core t\:lexer t\:parser
