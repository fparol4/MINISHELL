NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
INCLUDES = -I. -Iheaders -Ilibraries -Ilibraries/libft

LIBFT_DIR = libraries/libft
LIBFT = $(LIBFT_DIR)/libft.a

# --- MAIN ---

SRC_SHARED = source/shared/error.c \
			  source/shared/free.c \
			  source/shared/quote.c \
			  source/shared/string.c \
			  source/shared/signal.c

SRC_CORE = source/core/core.c \
		   source/core/reader.c

SRC_BIN = source/bin/cd.c \
		  source/bin/echo.c \
		  source/bin/env.c \
		  source/bin/exit.c \
		  source/bin/export/export.c \
		  source/bin/export/keys.c \
		  source/bin/export/print.c \
		  source/bin/pwd.c \
		  source/bin/unset.c

SRC_ENVM = source/env/list.c \
		   source/env/free.c \
		   source/env/get.c \
		   source/env/init.c \
		   source/env/size.c \
		   source/env/set.c \
		   source/env/toarr.c \
		   source/env/unset.c

SRC_RUNNER = source/runner/path/path.c \
			 source/runner/path/parts.c \
			 source/runner/pipe/fd.c \
			 source/runner/pipe/cleanup.c \
			 source/runner/pipe/pipe.c \
			 source/runner/pipe/process.c \
			 source/runner/pipe/tree.c \
			 source/runner/redir/apply.c \
			 source/runner/redir/strbuilder.c \
			 source/runner/redir/expand.c \
			 source/runner/redir/heredoc.c \
			 source/runner/redir/heredoc_read.c \
			 source/runner/redir/push.c \
			 source/runner/redir/target.c \
			 source/runner/control.c \
			 source/runner/builtin.c \
			 source/runner/status.c \
			 source/runner/external/process.c \
			 source/runner/external/resolve.c \
			 source/runner/expander/expand.c \
			 source/runner/expander/list.c \
			 source/runner/expander/dispatch.c \
			 source/runner/expander/variable.c \
			 source/runner/expander/word.c

SRC_LEXER = source/lexer/scanner/load.c \
			source/lexer/scanner/move.c \
			source/lexer/scanner/get.c \
			source/lexer/scanner/table.c \
			source/lexer/extract.c \
			source/lexer/lexer.c \
			source/lexer/rules.c \
			source/lexer/state.c \
			source/lexer/token.c

SRC_PARSER = source/parser/parser.c \
			 source/parser/cleanup.c \
			 source/parser/command.c \
			 source/parser/errors.c \
			 source/parser/state.c \
			 source/parser/utils.c

SOURCES = source/main.c \
		  $(SRC_CORE) \
		  $(SRC_SHARED) \
		  $(SRC_BIN) \
		  $(SRC_ENVM) \
		  $(SRC_RUNNER) \
		  $(SRC_LEXER) \
		  $(SRC_PARSER)

OBJ_DIR = build
OBJS = $(SOURCES:source/%.c=$(OBJ_DIR)/%.o)

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

.PHONY: all clean fclean re

# --- TESTS ---

TEST_FLAGS = -DSNOW_ENABLED

test: t\:envm t\:builtin t\:runner t\:core t\:lexer t\:parser

t\:envm: $(LIBFT)
	@mkdir -p tester/build tester/_tmp
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tester/envm.c \
			$(SRC_SHARED) \
			$(SRC_ENVM) \
			$(LIBFT) \
			-lreadline \
			-o tester/build/envm
	@tester/build/envm

t\:builtin: $(LIBFT)
	@mkdir -p tester/build tester/_tmp
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tester/bin.c \
			$(SRC_SHARED) \
			$(SRC_BIN) \
			source/runner/status.c \
			$(SRC_ENVM) \
			$(LIBFT) \
			-lreadline \
			-o tester/build/builtin
	@tester/build/builtin

t\:runner: $(LIBFT)
	@mkdir -p tester/build tester/_tmp
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tester/runner.c \
			$(SRC_RUNNER) \
			$(SRC_SHARED) \
			$(SRC_BIN) \
			$(SRC_ENVM) \
			source/parser/cleanup.c \
			$(LIBFT) -lreadline -o tester/build/runner
	@tester/build/runner

t\:core: $(LIBFT)
	@mkdir -p tester/build tester/_tmp
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tester/core.c \
		source/core/core.c \
			$(SRC_SHARED) \
			$(SRC_ENVM) \
			$(LIBFT) \
			-lreadline \
			-o tester/build/core
	@tester/build/core

t\:lexer: $(LIBFT)
	@mkdir -p tester/build tester/_tmp
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
			$(SRC_LEXER) \
			tester/lexer.c \
			source/shared/error.c \
			source/shared/quote.c \
			source/shared/string.c \
			$(LIBFT) \
			-o tester/build/lexer
	@tester/build/lexer

t\:parser: $(LIBFT)
	@mkdir -p tester/build tester/_tmp
	@$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tester/parser.c \
			$(SRC_PARSER) \
			$(SRC_LEXER) \
			source/shared/error.c \
			source/shared/quote.c \
			source/shared/string.c \
			$(LIBFT) \
			-o tester/build/parser
	@tester/build/parser

val:
	@valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes \
		--suppressions=valgrind.supp \
		./minishell

.PHONY: all clean fclean re \
	test t\:envm t\:builtin t\:runner t\:core t\:lexer t\:parser val
