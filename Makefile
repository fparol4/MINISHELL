NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror

LIBFT_DIR = libraries/libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I. -Iheaders -Ilibraries -Ilibraries/libft
TEST_FLAGS = -DSNOW_ENABLED

SHARED_SRCS = source/shared/error.c \
			  source/shared/free.c \
			  source/shared/string.c \
			  source/shared/signal.c

CORE_SRCS = source/shared/general.c

BULTIN_SRCS = source/modules/bultin/cd.c \
			  source/modules/bultin/echo.c \
			  source/modules/bultin/env.c \
			  source/modules/bultin/exit.c \
			  source/modules/bultin/export.c \
			  source/modules/bultin/pwd.c \
			  source/modules/bultin/unset.c

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

SRCS = source/main.c \
	   $(CORE_SRCS) \
	   $(SHARED_SRCS) \
	   $(BULTIN_SRCS) \
	   $(ENVM_SRCS) \
	   $(RUNNER_SRCS)

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

test: t\:envm t\:bultin t\:runner

t\:envm: $(LIBFT)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/envm.c \
		$(SHARED_SRCS) \
		$(ENVM_SRCS) \
		$(LIBFT) \
		-o /tmp/minishell_envm_tests
	/tmp/minishell_envm_tests

t\:bultin: $(LIBFT)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/bultin.c \
		$(SHARED_SRCS) \
		$(BULTIN_SRCS) \
		$(filter-out source/modules/envm/toarr.c,$(ENVM_SRCS)) \
		$(LIBFT) \
		-o /tmp/minishell_bultin_tests
	/tmp/minishell_bultin_tests

t\:runner: $(LIBFT)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/runner.c \
		$(RUNNER_SRCS) \
		$(SHARED_SRCS) \
		$(BULTIN_SRCS) \
		$(ENVM_SRCS) \
		$(LIBFT) -o /tmp/minishell_runner_tests
	/tmp/minishell_runner_tests

.PHONY: all clean fclean re test t\:envm t\:bultin t\:runner
