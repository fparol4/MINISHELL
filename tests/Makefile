CC      = cc
CFLAGS  = -g3 -Wall -Wextra -Werror

ROOT    = ..
LIBFT_A = $(ROOT)/lib/libft/libft.a

ENV_SRCS = \
	$(ROOT)/src/env/init.c \
	$(ROOT)/src/env/get.c \
	$(ROOT)/src/env/set.c \
	$(ROOT)/src/env/unset.c \
	$(ROOT)/src/env/toarr.c \
	$(ROOT)/src/env/free.c

INCLUDES = \
	-I$(ROOT)/lib/libft \
	-I$(ROOT)/src/env \
	-I$(ROOT)/src \
	-I.

all: test_env

$(LIBFT_A):
	$(MAKE) -sC $(ROOT)/lib/libft

test_env: $(LIBFT_A)
	$(CC) $(CFLAGS) -DSNOW_ENABLED \
		$(INCLUDES) \
		$(ENV_SRCS) test_env.c \
		$(LIBFT_A) -o test_env
	@./test_env

clean:
	rm -f test_env

.PHONY: all test_env clean
