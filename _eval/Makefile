
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
		--suppressions=tester/valgrind.supp \
		./minishell

.PHONY: all clean fclean re \
	test t\:envm t\:builtin t\:runner t\:core t\:lexer t\:parser val
