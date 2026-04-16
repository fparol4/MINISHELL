NAME = minishell

CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INFRA_DIR = infra
INFRA = $(INFRA_DIR)/infra.a

INCLUDES = -Iincludes -Ilibft -Iinfra/includes

SRCS = src/lexer/main.c \
	   src/lexer/lexer.c \
	   src/lexer/utils.c

OBJ_DIR = obj
OBJS = $(SRCS:src/lexer/%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT) $(INFRA) $(NAME)

$(LIBFT):
	$(MAKE) -sC $(LIBFT_DIR)

$(INFRA):
	$(MAKE) -sC $(INFRA_DIR)

$(NAME): $(OBJS) $(LIBFT) $(INFRA)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(INFRA) -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: src/lexer/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -sC $(LIBFT_DIR) clean
	$(MAKE) -sC $(INFRA_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -sC $(LIBFT_DIR) fclean
	$(MAKE) -sC $(INFRA_DIR) fclean

re: fclean all

val:
	@valgrind -q --leak-check=full --show-leak-kinds=all track-origin=yes \
	--suppressions={ARQ_NAME} --track-fds=yes ./$(NAME)
.PHONY: all clean fclean re