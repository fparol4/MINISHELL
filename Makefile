NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror

INCLUDES = -I. -Iheaders -Ilibraries -Ilibraries/libft
TEST_FLAGS = -DSNOW_ENABLED

SHARED_SRCS = source/shared/error.c \
			  source/shared/free.c \
			  source/shared/general.c \
			  source/shared/quote.c \
			  source/shared/string.c \
			  source/shared/signal.c

CORE_SRCS = source/core/core.c \
			source/core/reader.c

BIN_SRCS = source/bin/cd.c \
		   source/bin/echo.c \
		   source/bin/env.c \
		   source/bin/exit.c \
		   source/bin/export/export.c \
		   source/bin/export/keys.c \
		   source/bin/export/print.c \
		   source/bin/pwd.c \
		   source/bin/unset.c

ENVM_SRCS = source/env/list.c \
			source/env/free.c \
			source/env/get.c \
			source/env/init.c \
			source/env/size.c \
			source/env/set.c \
			source/env/toarr.c \
			source/env/unset.c

ENVM_NO_TOARR_SRCS = source/env/list.c \
					 source/env/free.c \
					 source/env/get.c \
					 source/env/init.c \
					 source/env/size.c \
					 source/env/set.c \
					 source/env/unset.c

RUNNER_PATH_SRCS = source/runner/path/path.c \
				   source/runner/path/parts.c

RUNNER_PIPE_SRCS = source/runner/pipe/fd.c \
				   source/runner/pipe/pipe.c \
				   source/runner/pipe/process.c \
				   source/runner/pipe/tree.c

RUNNER_REDIR_SRCS = source/runner/redir/apply.c \
					source/runner/redir/buffer.c \
					source/runner/redir/expand.c \
					source/runner/redir/heredoc.c \
					source/runner/redir/heredoc_read.c \
					source/runner/redir/push.c \
					source/runner/redir/target.c

RUNNER_EXTERNAL_SRCS = source/runner/external/process.c \
					   source/runner/external/resolve.c

EXPANDER_SRCS = source/runner/expander/expand.c \
				source/runner/expander/list.c \
				source/runner/expander/quotes.c \
				source/runner/expander/variable.c \
				source/runner/expander/word.c

RUNNER_SRCS = $(RUNNER_PATH_SRCS) \
			  $(RUNNER_PIPE_SRCS) \
			  $(RUNNER_REDIR_SRCS) \
			  source/runner/status.c \
			  source/runner/control.c \
			  source/runner/builtin.c \
			  $(RUNNER_EXTERNAL_SRCS) \
			  $(EXPANDER_SRCS)

LEXER_SRCS = source/lexer/scanner/load.c \
			 source/lexer/scanner/move.c \
			 source/lexer/scanner/get.c \
			 source/lexer/scanner/table.c \
			 source/lexer/extract.c \
			 source/lexer/lexer.c \
			 source/lexer/rules.c \
			 source/lexer/state.c \
			 source/lexer/token.c

PARSER_SRCS = source/parser/parser.c \
			  source/parser/cleanup.c \
			  source/parser/command.c \
			  source/parser/errors.c \
			  source/parser/state.c \
			  source/parser/utils.c

SOURCES = source/main.c \
		  $(CORE_SRCS) \
		  $(SHARED_SRCS) \
		  $(BIN_SRCS) \
		  $(ENVM_SRCS) \
		  $(RUNNER_SRCS) \
		  $(LEXER_SRCS) \
		  $(PARSER_SRCS)

OBJ_DIR = build
OBJS = $(SOURCES:source/%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR = libraries/libft
LIBFT = $(LIBFT_DIR)/libft.a

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
	@mkdir -p tests/build
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/envm.c \
			$(SHARED_SRCS) \
			$(ENVM_SRCS) \
			$(LIBFT) \
			-lreadline \
			-o tests/build/envm
	@tests/build/envm

t\:builtin: $(LIBFT)
	@mkdir -p tests/build
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/bin.c \
			$(SHARED_SRCS) \
			$(BIN_SRCS) \
			source/runner/status.c \
			$(ENVM_NO_TOARR_SRCS) \
			$(LIBFT) \
			-lreadline \
			-o tests/build/builtin
	@tests/build/builtin

t\:runner: $(LIBFT)
	@mkdir -p tests/build
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/runner.c \
			$(RUNNER_SRCS) \
			$(SHARED_SRCS) \
			$(BIN_SRCS) \
			$(ENVM_SRCS) \
			$(LIBFT) -lreadline -o tests/build/runner
	@tests/build/runner

t\:core: $(LIBFT)
	@mkdir -p tests/build
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/core.c \
		source/core/core.c \
			$(SHARED_SRCS) \
			$(ENVM_SRCS) \
			$(LIBFT) \
			-lreadline \
			-o tests/build/core
	@tests/build/core

t\:lexer: $(LIBFT)
	@mkdir -p tests/build
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
			tests/lexer.c \
			$(LEXER_SRCS) \
			source/shared/error.c \
			source/shared/quote.c \
			source/shared/string.c \
			$(LIBFT) \
			-o tests/build/lexer
	@tests/build/lexer

t\:parser: $(LIBFT)
	@mkdir -p tests/build
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/parser.c \
			$(PARSER_SRCS) \
			$(LEXER_SRCS) \
			source/shared/error.c \
			source/shared/quote.c \
			source/shared/string.c \
			$(LIBFT) \
			-o tests/build/parser
	@tests/build/parser

.PHONY: all clean fclean re test force \
	t\:envm t\:builtin t\:runner t\:core t\:lexer t\:parser
