NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -Iincludes -Ilibft

SRCS = src/lexer/main.c \
	   src/lexer/lexer.c \

OBJ_DIR = obj
OBJS = $(SRCS:src/lexer/%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	$(MAKE) -sC $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: src/lexer/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -sC $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -sC $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re