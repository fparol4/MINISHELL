#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>

#include "../libraries/libft//libft.h"
#include "./env.h"
#include "./builtin.h"

# define BUFFER_SIZE 4096

# define  MSG_MINISHELL "minishell:"

typedef enum e_cmd_type { CMD_COMMAND, CMD_PIPE, CMD_AND, CMD_OR } t_cmd_type;

typedef struct s_cmd_node {
  t_cmd_type type;
  char **args;
  struct s_cmd_node *left;
  struct s_cmd_node *right;
} t_cmd_node;

#endif
