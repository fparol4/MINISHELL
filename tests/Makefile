CC      = cc
CFLAGS  = -g3 -Wall -Wextra -Werror

ROOT    = ..
LIBFT_A = $(ROOT)/lib/libft/libft.a

ENVM_SRCS = \
	$(ROOT)/src/envm/init.c \
	$(ROOT)/src/envm/get.c \
	$(ROOT)/src/envm/set.c \
	$(ROOT)/src/envm/unset.c \
	$(ROOT)/src/envm/toarr.c \
	$(ROOT)/src/envm/free.c

BUILTINS_SRCS = \
	$(ROOT)/src/builtin/echo.c \
	$(ROOT)/src/builtin/cd.c \
	$(ROOT)/src/builtin/pwd.c \
	$(ROOT)/src/builtin/export.c \
	$(ROOT)/src/builtin/unset.c \
	$(ROOT)/src/builtin/env.c \
	$(ROOT)/src/builtin/exit.c

EXECM_SRCS = \
	$(ROOT)/src/execm/execm.c

PIPELINE_SRCS = \
	$(ROOT)/src/pipeline/pipeline.c

REDIR_SRCS = \
	$(ROOT)/src/redir/redir.c

INCLUDES = \
	-I$(ROOT)/lib/libft \
	-I$(ROOT)/src/types \
	-I$(ROOT)/src/builtin \
	-I$(ROOT)/src/execm \
	-I$(ROOT)/src/pipeline \
	-I$(ROOT)/src/redir \
	-I.

all: test_envm test_builtins test_execm test_pipeline test_redir

$(LIBFT_A):
	$(MAKE) -sC $(ROOT)/lib/libft

test_envm: $(LIBFT_A)
	$(CC) $(CFLAGS) -DSNOW_ENABLED \
		$(INCLUDES) \
		$(ENVM_SRCS) envm.c \
		$(LIBFT_A) -o test_envm
	@./test_envm --log /tmp/snow_envm.log && cat /tmp/snow_envm.log

test_builtins: $(LIBFT_A)
	$(CC) $(CFLAGS) -DSNOW_ENABLED \
		$(INCLUDES) \
		$(ENVM_SRCS) $(BUILTINS_SRCS) test_builtins.c \
		$(LIBFT_A) -o test_builtins
	@./test_builtins --log /tmp/snow_builtins.log && cat /tmp/snow_builtins.log

clean:
	rm -f test_envm test_builtins test_execm test_pipeline test_redir

test_execm: $(LIBFT_A)
	$(CC) $(CFLAGS) -DSNOW_ENABLED \
		$(INCLUDES) \
		$(ENVM_SRCS) $(BUILTINS_SRCS) $(EXECM_SRCS) test_execm.c \
		$(LIBFT_A) -o test_execm
	@./test_execm --log /tmp/snow_execm.log && cat /tmp/snow_execm.log

test_pipeline: $(LIBFT_A)
	$(CC) $(CFLAGS) -DSNOW_ENABLED \
		$(INCLUDES) \
		$(ENVM_SRCS) $(BUILTINS_SRCS) $(EXECM_SRCS) $(PIPELINE_SRCS) $(REDIR_SRCS) test_pipeline.c \
		$(LIBFT_A) -lreadline -o test_pipeline
	@./test_pipeline --log /tmp/snow_pipeline.log && cat /tmp/snow_pipeline.log

test_redir: $(LIBFT_A)
	$(CC) $(CFLAGS) -DSNOW_ENABLED \
		$(INCLUDES) \
		$(ENVM_SRCS) $(BUILTINS_SRCS) $(EXECM_SRCS) $(PIPELINE_SRCS) $(REDIR_SRCS) test_redir.c \
		$(LIBFT_A) -lreadline -o test_redir
	@./test_redir --log /tmp/snow_redir.log && cat /tmp/snow_redir.log

.PHONY: all test_envm test_builtins test_execm test_pipeline test_redir clean
