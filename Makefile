NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror

LIBFT_DIR = libraries/libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I. -Iheaders -Ilibraries -Ilibraries/libft
TEST_FLAGS = -DSNOW_ENABLED

SRCS = source/main.c \
	   source/shared/error.c \
	   source/shared/free.c \
	   source/shared/general.c \
	   source/shared/string.c \
	   source/modules/bultin/cd.c \
	   source/modules/bultin/echo.c \
	   source/modules/bultin/env.c \
	   source/modules/bultin/exit.c \
	   source/modules/bultin/export.c \
	   source/modules/bultin/pwd.c \
	   source/modules/bultin/unset.c \
	   source/modules/envm/free.c \
	   source/modules/envm/get.c \
	   source/modules/envm/init.c \
	   source/modules/envm/common.c \
	   source/modules/envm/size.c \
	   source/modules/envm/set.c \
	   source/modules/envm/toarr.c \
	   source/modules/envm/unset.c \
	   source/modules/runner/path.c \
	   source/modules/runner/pipe.c \
	   source/modules/runner/status.c \
	   source/modules/runner/exec.c \
	   source/modules/runner/expander.c

OBJ_DIR = obj
OBJS = $(SRCS:source/%.c=$(OBJ_DIR)/%.o)

SHARED_TEST_SRCS = source/shared/error.c \
				   source/shared/free.c \
				   source/shared/string.c

ENVM_TEST_SRCS = source/modules/envm/common.c \
				 source/modules/envm/free.c \
				 source/modules/envm/get.c \
				 source/modules/envm/init.c \
				 source/modules/envm/size.c \
				 source/modules/envm/set.c \
				 source/modules/envm/toarr.c \
				 source/modules/envm/unset.c

BULTIN_TEST_SRCS = source/modules/bultin/cd.c \
				   source/modules/bultin/echo.c \
				   source/modules/bultin/env.c \
				   source/modules/bultin/exit.c \
				   source/modules/bultin/export.c \
				   source/modules/bultin/pwd.c \
				   source/modules/bultin/unset.c

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
		$(SHARED_TEST_SRCS) \
		$(ENVM_TEST_SRCS) \
		$(LIBFT) \
		-o /tmp/minishell_envm_tests
	/tmp/minishell_envm_tests

t\:bultin: $(LIBFT)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/bultin.c \
		$(SHARED_TEST_SRCS) \
		$(BULTIN_TEST_SRCS) \
		$(filter-out source/modules/envm/toarr.c,$(ENVM_TEST_SRCS)) \
		$(LIBFT) \
		-o /tmp/minishell_bultin_tests
	/tmp/minishell_bultin_tests

t\:runner: $(LIBFT)
	$(CC) $(CFLAGS) $(TEST_FLAGS) $(INCLUDES) \
		tests/runner.c \
		source/modules/runner/path.c \
		source/modules/runner/pipe.c \
		source/modules/runner/status.c \
		source/modules/runner/exec.c \
		source/modules/runner/expander.c \
		$(SHARED_TEST_SRCS) \
		$(BULTIN_TEST_SRCS) \
		source/modules/envm/common.c \
		source/modules/envm/free.c \
		source/modules/envm/get.c \
		source/modules/envm/init.c \
		source/modules/envm/size.c \
		source/modules/envm/set.c \
		source/modules/envm/toarr.c \
		source/modules/envm/unset.c \
		$(LIBFT) -o /tmp/minishell_runner_tests
	/tmp/minishell_runner_tests

.PHONY: all clean fclean re test t\:envm t\:bultin t\:runner
