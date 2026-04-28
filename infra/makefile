NAME = infra.a

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror
AR = ar rcs
RM = rm -rf

LIBFT_DIR = ../libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -Iincludes -I$(LIBFT_DIR)

SRCS =	scanner_advance.c \
		scanner_current.c \
		scanner_init.c \
		scanner_is_end.c \
		scanner_peek.c \
		scanner_extract.c \
		scanner_mark_start.c \
		scanner_take_while.c \
		init_char_table.c

OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -sC $(LIBFT_DIR)

$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@$(MAKE) -sC $(LIBFT_DIR) clean

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re